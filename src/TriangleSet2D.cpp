//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file TriangleSet2D.cpp
/// \brief CPP file for TriangleSet2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "TriangleSet2D.h"
#include "MultiTexture.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TriangleSet2D::database( "TriangleSet2D", 
                               &(newInstance<TriangleSet2D>), 
                               typeid( TriangleSet2D ),
                               &X3DGeometryNode::database );

namespace TriangleSet2DInternals {
  FIELDDB_ELEMENT( TriangleSet2D, vertices, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TriangleSet2D, solid, INPUT_OUTPUT );
}


TriangleSet2D::TriangleSet2D( Inst< SFNode      > _metadata,
                              Inst< SFBound     > _bound,
                              Inst< DisplayList > _displayList,
                              Inst< MFBool      > _isTouched,
                              Inst< MFVec3f     > _force,
                              Inst< MFVec3f     > _contactPoint,
                              Inst< MFVec3f     > _contactNormal,
                              Inst< MFVec2f     > _vertices,
                              Inst< SFBool      > _solid ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  vertices( _vertices ),
  solid( _solid ) {

  type_name = "TriangleSet2D";
  database.initFields( this );

  solid->setValue( false );

  vertices->route( bound );
  vertices->route( displayList );
  solid->route( displayList );
}

void TriangleSet2D::render() {
  // render the points
  const vector< Vec2f > &v = vertices->getValue();

  if( solid->getValue() ) {
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
  } else {
    glDisable( GL_CULL_FACE );
  }

  BoxBound *bb = dynamic_cast< BoxBound * >( bound->getValue() );
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
  if( bb ) {
    const Vec3f &center = bb->center->getValue();
    const Vec3f &size = bb->size->getValue();
    H3DFloat sparams[4] = {1/size.x,0,0,0.5f - center.x / size.x};
    H3DFloat tparams[4] = {0,1/size.y,0,0.5f - center.y / size.y};
    
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; i++ ) {
        glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
        glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
        glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
        glEnable( GL_TEXTURE_GEN_S );
        glEnable( GL_TEXTURE_GEN_T );
      }
    } else {
      glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
      glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
      glEnable( GL_TEXTURE_GEN_S );
      glEnable( GL_TEXTURE_GEN_T );
    }
  }

  glBegin( GL_TRIANGLES );
  glNormal3f( 0.f, 0.f, 1.f );
  for( unsigned int i = 0; i < v.size(); i+=3 ) {
    glVertex2f( v[i].x, v[i].y );
    glVertex2f( v[i+1].x, v[i+1].y );
    glVertex2f( v[i+2].x, v[i+2].y );
  }
  glEnd();

  if( bb && mt ) {
    size_t texture_units = mt->texture->size();
    for( size_t i = 0; i < texture_units; i++ ) {
      glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
    }
  } else {
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
  }
}

void TriangleSet2D::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
#ifdef HAVE_OPENHAPTICS
    ti.addHapticShapeToAll( getOpenGLHapticShape( 
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix(),
                                                 vertices->size() ) );
#endif
  }
}
