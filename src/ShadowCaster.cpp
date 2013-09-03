//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file ShadowCaster.cpp
/// \brief CPP file for ShadowCaster
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShadowCaster.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/DirectionalLight.h>
#include <H3D/NavigationInfo.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShadowCaster::database( "ShadowCaster", 
                                          &(newInstance<ShadowCaster>), 
                                          typeid( ShadowCaster ),
                                          &X3DChildNode::database );

namespace ShadowCasterInternals {
  FIELDDB_ELEMENT( ShadowCaster, object, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowCaster, light, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowCaster, shadowDarkness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowCaster, shadowDepthOffset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowCaster, algorithm, INPUT_OUTPUT );
}

ShadowCaster::ShadowCaster( 
                           Inst< SFNode>  _metadata,
                           Inst< MFShadowObjectNode > _object,
                           Inst< MFLightNode        > _light,
                           Inst< SFFloat            > _shadowDarkness,
                           Inst< SFFloat            > _shadowDepthOffset,
                           Inst< DisplayList        > _displayList,
                           Inst< SFString           > _algorithm ) :
  X3DChildNode( _metadata ),
  H3DDisplayListObject( _displayList ),
  object( _object ),
  light( _light ),
  shadowDarkness( _shadowDarkness ),
  shadowDepthOffset( _shadowDepthOffset ),
  algorithm( _algorithm ) {
  type_name = "ShadowCaster";
  database.initFields( this );

  displayList->setOwner( this );

  algorithm->addValidValue( "ZPASS" );
  algorithm->addValidValue( "ZFAIL" );
  algorithm->setValue( "ZPASS" );
  shadowDarkness->setValue( 0.4f );
  shadowDepthOffset->setValue( 6 );
  displayList->setCacheMode( DisplayList::OFF );
}

void ShadowCaster::addHeadLight() {
  bool head_light = false;
  NavigationInfo *ni = NavigationInfo::getActive();
  if( ni ) {
    head_light = 
      ni->headlightShadows->getValue() && 
      ni->headlight->getValue();
  }
  
  if( head_light ) {
    X3DViewpointNode *vp = X3DViewpointNode::getActive();
    Vec3f direction = Vec3f( 0, 0, -1 );
    if( vp ) {
      direction = 
	vp->accForwardMatrix->getValue().getRotationPart() * 
	(vp->totalOrientation->getValue() * Vec3f( 0, 0, -1 ));
    }
    DirectionalLight *dir_light = new DirectionalLight();
    dir_light->direction->setValue( direction );
    light->push_back( dir_light );
  }
}

void ShadowCaster::render() {
  if( X3DShapeNode::geometry_render_mode == X3DShapeNode::SOLID ||
      X3DShapeNode::geometry_render_mode == X3DShapeNode::TRANSPARENT_BACK ||
      object->empty() || light->empty() ) return;
  X3DChildNode::render();

  // set the darkness so that if a point is in shadow from all lights,
  // the darkness should be shadowDarkness.
  // Basically the solution to (1-x)^nr_lights = (1-shadowDarkness)
  H3DFloat darkness = 1 - H3DPow( 1.f - shadowDarkness->getValue(),
                                  1.f / light->size() );
  
  for( MFLightNode::const_iterator l = light->begin(); 
       l != light->end(); ++l ) {

    glClear(GL_STENCIL_BUFFER_BIT );
    glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | 
                  GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );
    glDisable( GL_LIGHTING );// Turn Off Lighting
    glDepthMask( GL_FALSE );// Turn Off Writing To The Depth-Buffer
    glDepthFunc( GL_LESS );
    glDisable( GL_ALPHA_TEST );
    glEnable( GL_STENCIL_TEST );// Turn On Stencil Buffer Testing
    glColorMask( GL_FALSE, GL_FALSE, 
                 GL_FALSE, GL_FALSE );// Don't Draw Into The Colour Buffer
    glEnable( GL_CULL_FACE );
    
    const string &alg = algorithm->getValue();
    glStencilFunc( GL_ALWAYS, 1, 0xFFFFFFFFL );
    
    if( alg == "ZPASS" ) {
      glStencilOp( GL_KEEP, GL_KEEP, GL_INCR );
      glCullFace( GL_BACK );
    } else {
      glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
      glCullFace(GL_FRONT);
    }

    // The shadow geometry does not use the exact same rendering function
    // as the original geometry, since it needs to render triangles facing the
    // light differently from the ones facing away from the light.
    // In order to do this the boundTree triangles of the geometry are used.
    // These triangles have been collected with a HAPI::FeedbackBufferCollector
    // using OpenGL. Somewhere in this process, possibly in the transformation
    // from window coordinates to world coordinates(model view matrix seems ok),
    // the original coordinate used in rendering differs from the coordinate
    // in the boundTree by some epsilon value due to floating point inaccuracies.
    // This means that when drawing caps for ZFAIL algorithm, we can get a case
    // of z-fighting between the original geometry and the shadow geometry,
    // making the geometry flicker.
    // 
    // In order to avoid this we glPolygonOffset to offset the shadow geometry
    // depth value. The values chosen depends on the scene but according to
    // Mark_Kilgard in the presentation "Shadow mapping with todays OpenGL hardware"
    // the values -1, 4 work well most of the time for shadow mapping. We 
    // noticed however that using the scale part introduces artifacts in the
    // shadow volumes(look like the gaps between edges) so instead we
    // do not use the scale and have a default offset of 6 to compensate.
    glPolygonOffset( 0, shadowDepthOffset->getValue() );
    glEnable( GL_POLYGON_OFFSET_FILL );

    // First Pass. Increase Stencil Value In The Shadow 
    for( MFShadowObjectNode::const_iterator o = object->begin(); 
         o != object->end(); ++o ) {

      static_cast< H3DShadowObjectNode * >(*o)->renderShadow( static_cast< X3DLightNode * >(*l), alg == "ZFAIL" );
    }
  
    // Second Pass. Decrease Stencil Value In The Shadow
    if( alg == "ZPASS" ) {
      glStencilOp( GL_KEEP, GL_KEEP, GL_DECR );
      glCullFace( GL_FRONT );
    } else {
      glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
      glCullFace(GL_BACK);
    }
    
    for( MFShadowObjectNode::const_iterator o = object->begin(); 
         o != object->end(); ++o ) {
      static_cast< H3DShadowObjectNode * >(*o)->renderShadow( static_cast< X3DLightNode * >(*l), alg == "ZFAIL" );
    }

    // Enable Rendering To Colour Buffer For All Components
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    
    // Draw A Shadowing Rectangle Covering The Entire Screen
    glColor4f( 0.0f, 0.0f, 0.0f, darkness );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();
    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glBegin (GL_QUADS);
    glVertex3i (-1, -1, -1);
    glVertex3i (1, -1, -1);
    glVertex3i (1, 1, -1);
    glVertex3i (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix ();
    glPopAttrib(); 
  }

}
