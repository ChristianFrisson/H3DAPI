//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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
/// \file PlanarReflector.cpp
/// \brief CPP file for PlanarReflector, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PlanarReflector.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/Scene.h>
#include <H3D/X3DBackgroundNode.h>
#include <H3D/ClipPlane.h>
#include <H3D/X3DShapeNode.h>

using namespace H3D;

H3DNodeDatabase PlanarReflector::database( 
        "PlanarReflector", 
        &(newInstance< PlanarReflector >),
        typeid( PlanarReflector ),
        &X3DChildNode::database 
        );

namespace PlanarReflectorInternals {
  FIELDDB_ELEMENT( PlanarReflector, geometry, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlanarReflector, reflectivity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlanarReflector, color, INPUT_OUTPUT );
}

PlanarReflector::PlanarReflector( 
                           Inst< SFNode           > _metadata,
                           Inst< SFBound          > _bound,
                           Inst< SFVec3f          > _bboxCenter,
                           Inst< SFVec3f          > _bboxSize,
                           Inst< DisplayList      > _displayList,
                           Inst< SFGeometryNode   > _geometry,
                           Inst< SFFloat          > _reflectivity,
                           Inst< SFColor          > _color
) :
  X3DChildNode( _metadata ),
  X3DBoundedObject( _bound, _bboxCenter, _bboxSize ),
  H3DDisplayListObject( _displayList ),
  geometry  ( _geometry   ),
  reflectivity( _reflectivity ),
  color( _color ),
  use_geometry_bound( false ) {

  type_name = "PlanarReflector";
  database.initFields( this );

  displayList->setOwner( this );
  bound->setOwner( this );
  
  reflectivity->setValue( (H3DFloat) 0.9 );
  color->setValue( RGB( 1, 1, 1 ) );

  // never generate a display list.
  Scene::time->route( displayList );
  geometry->route( displayList );
  reflectivity->route( displayList );
  color->route( displayList );
}


void PlanarReflector::renderPostViewpoint ( X3DChildNode *n,
                                            X3DViewpointNode *vp ) {
  glMatrixMode( GL_MODELVIEW );

  GLfloat mv[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mv );
  Matrix4f vp_transform( mv[0], mv[4], mv[8], mv[12], 
                         mv[1], mv[5], mv[9], mv[13], 
                         mv[2], mv[6], mv[10], mv[14], 
                         mv[3], mv[7], mv[11], mv[15] );

  // draw mirror geometry into stencil buffer
  glPushMatrix();
  GLfloat t[16] = { local_to_global[0][0], local_to_global[1][0],
                    local_to_global[2][0], local_to_global[3][0],
                    local_to_global[0][1], local_to_global[1][1],
                    local_to_global[2][1], local_to_global[3][1],
                    local_to_global[0][2], local_to_global[1][2],
                    local_to_global[2][2], local_to_global[3][2],
                    local_to_global[0][3], local_to_global[1][3],
                    local_to_global[2][3], local_to_global[3][3] };
  glMultMatrixf( t );
  
  glClear( GL_STENCIL_BUFFER_BIT );

  // write 1 into all values in the stencil buffer used when rendering the
  // geometry
  glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
  glStencilFunc( GL_ALWAYS, 1, 1 );
  glEnable( GL_STENCIL_TEST );
  // don't write to frame buffer or depth buffer
  glColorMask( 0, 0, 0, 0 );
  glDisable( GL_DEPTH_TEST );
  
  // draw mirror geometry
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->displayList->callList();
  
  // reset to frite to frame and depth buffer again
  glColorMask( 1, 1, 1, 1 );
  glEnable( GL_DEPTH_TEST );

  glPopMatrix();

  // set up test to only render the parts where the mirror geometry was
  // rendered
  glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
  glStencilFunc( GL_EQUAL, 1, 1 );
  glClear( GL_DEPTH_BUFFER_BIT );

  glPushMatrix();
  
  // calculate reflection matrix
  Matrix4f full_vp = vp_transform * local_to_global;
  Vec3f P = full_vp * Vec3f( 0, 0, 0 );
  Vec3f V = full_vp.getRotationPart() * Vec3f( 0, 0, 1 );
  Matrix4f m( 1 - 2*V.x*V.x, -2*V.x*V.y, -2*V.x*V.z, 2*(P*V)*V.x,
              -2*V.x*V.y, 1 - 2*V.y*V.y, -2*V.y*V.z, 2*(P*V)*V.y,
              -2*V.x*V.z, -2*V.y*V.z, 1 - 2*V.z*V.z, 2*(P*V)*V.z,
              0, 0, 0, 1 );

  // the reflection matrix
  GLfloat rm[16] = { m[0][0], m[1][0], m[2][0], m[3][0],
  m[0][1], m[1][1], m[2][1], m[3][1],
  m[0][2], m[1][2], m[2][2], m[3][2],
  m[0][3], m[1][3], m[2][3], m[3][3] };

  // the transformation used for viewpoint
  GLfloat vp_m[16] = { vp_transform[0][0], vp_transform[1][0],
                       vp_transform[2][0], vp_transform[3][0],
                       vp_transform[0][1], vp_transform[1][1],
                       vp_transform[2][1], vp_transform[3][1],
                       vp_transform[0][2], vp_transform[1][2],
                       vp_transform[2][2], vp_transform[3][2],
                       vp_transform[0][3], vp_transform[1][3],
                       vp_transform[2][3], vp_transform[3][3] };

  // draw background
  X3DBackgroundNode *background = X3DBackgroundNode::getActive();
  if( background ) {
    GLint front_face;
    glGetIntegerv( GL_FRONT_FACE, &front_face );
    if( front_face == GL_CW ) {
      // we are in mirrored mode, so we have to set the front face
      // to the opposite in order for it to be right when mirrored.
      glFrontFace( GL_CCW );
    } else {
      glFrontFace( GL_CW );
    }
    glPushMatrix();
    Matrix4f tt = m * vp_transform;
    Matrix3f rt = tt.getRotationPart();
    GLfloat tm[16] = { rt[0][0], rt[1][0], rt[2][0], 0,
                       rt[0][1], rt[1][1], rt[2][1], 0,
                       rt[0][2], rt[1][2], rt[2][2], 0,
                       0, 0, 0, 1 };

    glLoadMatrixf( tm );
    
    glDepthMask( GL_FALSE );
    background->renderBackground();
    glDepthMask( GL_TRUE );

    glPopMatrix();
    glFrontFace( front_face );
  }

  // set up clip plane to clip everything behind the mirror
  glMultMatrixf( t );
  GLint front_face;
  glGetIntegerv( GL_FRONT_FACE, &front_face );
  // Make sure that there are enough free clip plane indicies and
  // use the correct one and increase nr_active_clip_planes so that
  // ClipPlane nodes work fine.
  if( ClipPlane::checkIfFreeClipPlaneIndex() ) {
    int plane_index = ClipPlane::nr_active_clip_planes++;
    Vec3f point( 0, 0, 0 );
    Vec3f normal( 0, 0, -1 );
    GLdouble e[] = {  normal.x, normal.y, normal.z, 
                   -normal.x*point.x - normal.y*point.y - normal.z*point.z };
    glClipPlane( GL_CLIP_PLANE0 + plane_index, e );
    glEnable( GL_CLIP_PLANE0 + plane_index );
    
    // draw scene
    glLoadMatrixf( rm );
    glMultMatrixf( vp_m );
    
    // reflection matrix contains a negative scaling so we need to change
    // front face.
    if( front_face == GL_CW ) {
      // we are in mirrored mode, so we have to set the front face
        // to the opposite in order for it to be right when mirrored.
      glFrontFace( GL_CCW );
    } else {
      glFrontFace( GL_CW );
    }

    if( multi_pass_transparency ) {
      X3DShapeNode::geometry_render_mode = X3DShapeNode::SOLID; 
      n->render();
      X3DShapeNode::geometry_render_mode = X3DShapeNode::TRANSPARENT_BACK; 
      n->render();
      X3DShapeNode::geometry_render_mode = X3DShapeNode::TRANSPARENT_FRONT; 
      n->render();
      X3DShapeNode::geometry_render_mode = X3DShapeNode::ALL; 
    } else {
      X3DShapeNode::geometry_render_mode = X3DShapeNode::ALL; 
      n->render();
    }
    glDisable( GL_CLIP_PLANE0 + plane_index );
    --ClipPlane::nr_active_clip_planes;
  }
  glPopMatrix();
  glDisable( GL_STENCIL_TEST );

  // draw mirror geometry
  glClear( GL_DEPTH_BUFFER_BIT );
  glPushMatrix();
  glMultMatrixf( t );
  glEnable( GL_BLEND );
  glDisable( GL_LIGHTING );
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  const RGB &c = color->getValue();

  glColor4f( c.r, c.g, c.b, 1 - reflectivity->getValue() );
  if( g ) g->render();
  glEnable( GL_LIGHTING );
  glPopMatrix();

  glFrontFace( front_face );
    /*
  glPushMatrix();
  GLfloat t[16] = { local_to_global[0][0], local_to_global[1][0],
                    local_to_global[2][0], local_to_global[3][0],
                    local_to_global[0][1], local_to_global[1][1],
                    local_to_global[2][1], local_to_global[3][1],
                    local_to_global[0][2], local_to_global[1][2],
                    local_to_global[2][2], local_to_global[3][2],
                    local_to_global[0][3], local_to_global[1][3],
                    local_to_global[2][3], m[3][3] };
  glMultMatrixf( t );
  
  glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
  glStencilFunc( GL_ALWAYS, 1, 1 );
  glEnable( GL_STENCIL_TEST );

  glColorMask( 0, 0, 0, 0 );
  
  // blending maybe?
  //  render();
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->displayList->callList();
  
  glColorMask( 1, 1, 1, 1 );

  glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
  glStencilFunc( GL_ALWAYS, 1, 1 );
 // glClearColor( 0, 1, 0, 1 );
  //glClear( GL_COLOR_BUFFER_BIT );


    glClear( GL_DEPTH_BUFFER_BIT );
   if( g ) g->displayList->callList();
  glDisable( GL_STENCIL_TEST );
//   X3DGeometryNode *g = geometry->getValue();

 
 //if( g ) g->displayList->callList();

  glPopMatrix();
  */
  
}

void PlanarReflector::traverseSG( TraverseInfo &ti ) {
  local_to_global = ti.getAccForwardMatrix();
  Node *g = geometry->getValue();
  if ( g ) g->traverseSG( ti );
  X3DChildNode::traverseSG( ti );
  H3DMultiPassRenderObject::traverseSG( ti );
}

bool PlanarReflector::lineIntersect(
                           const Vec3f &from,
                           const Vec3f &to,
                          LineIntersectResult &result ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) return g->lineIntersect( from,
                                   to,
                                   result ); 
  else return false; 
}

void PlanarReflector::closestPoint( const Vec3f &p,
                                    NodeIntersectResult &result ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->closestPoint( p, result );
}

bool PlanarReflector::movingSphereIntersect( H3DFloat radius,
                                             const Vec3f &from, 
                                             const Vec3f &to,
                                             NodeIntersectResult &result ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) return g->movingSphereIntersect( radius, from, to, result );
  else return false;
}
