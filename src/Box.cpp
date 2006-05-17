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
/// \file Box.cpp
/// \brief CPP file for Box, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Box.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Box::database( "Box", 
                               &(newInstance<Box>), 
                               typeid( Box ),
                               &X3DGeometryNode::database );

namespace BoxInternals {
  FIELDDB_ELEMENT( Box, size, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Box, solid, INPUT_OUTPUT );
}


Box::Box( 
         Inst< SFNode  >  _metadata,
         Inst< SFBound >  _bound,
         Inst< SFVec3f >  _size,
         Inst< SFBool  >  _solid ) :
  X3DGeometryNode( _metadata, _bound ),
  size    ( _size     ),
  solid   ( _solid    ) {

  type_name = "Box";
  database.initFields( this );

  size->setValue( Vec3f( 2, 2, 2 ) );
  solid->setValue( true );

  size->route( bound );
  
  size->route( displayList );
  solid->route( displayList );

}

void Box::render() {
  if( solid->getValue() ) {
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
  } else {
    glDisable( GL_CULL_FACE );
  }
  H3DFloat x = size->getValue().x / 2;
  H3DFloat y = size->getValue().y / 2;
  H3DFloat z = size->getValue().z / 2;
  
  glBegin( GL_QUADS );
  
  // +z
  glNormal3f  ( 0, 0, 1 );

  glTexCoord2f( 1, 1 );
  glVertex3f  ( x, y, z );
  glTexCoord2f( 0, 1 );
  glVertex3f  ( -x, y, z );
  glTexCoord2f( 0, 0 );
  glVertex3f  ( -x, -y, z );
  glTexCoord2f( 1, 0 );
  glVertex3f  ( x, -y, z );
  
  // -z
  glNormal3f  ( 0, 0, -1 );

  glTexCoord2f( 0, 0 );
  glVertex3f  ( x, -y, -z );
  glTexCoord2f( 1, 0 );
  glVertex3f  ( -x, -y, -z );
  glTexCoord2f( 1, 1 );
  glVertex3f  ( -x, y, -z );
  glTexCoord2f( 0, 1 );
  glVertex3f  ( x, y, -z );
  
  // +y
  glNormal3f  ( 0, 1, 0 );

  glTexCoord2f( 1, 0 );
  glVertex3f  ( x, y, z );
  glTexCoord2f( 1, 1 );
  glVertex3f  ( x, y, -z );
  glTexCoord2f( 0, 1 );
  glVertex3f  ( -x, y, -z );
  glTexCoord2f( 0, 0 );
  glVertex3f  ( -x, y, z );

  // -y
  glNormal3f  ( 0, -1, 0 );

  glTexCoord2f( 0, 1 );
  glVertex3f  ( -x, -y, z );
  glTexCoord2f( 0, 0 );
  glVertex3f  ( -x, -y, -z );
  glTexCoord2f( 1, 0 );
  glVertex3f  ( x, -y, -z );
  glTexCoord2f( 1, 1 );
  glVertex3f  ( x, -y, z );
  
  // +x
  glNormal3f  ( 1, 0, 0 );

  glTexCoord2f( 0, 1 );
  glVertex3f  ( x, y, z );
  glTexCoord2f( 0, 0 );
  glVertex3f  ( x, -y, z );
  glTexCoord2f( 1, 0 );
  glVertex3f  ( x, -y, -z );
  glTexCoord2f( 1, 1 );
  glVertex3f  ( x, y, -z );
  
  // -x
  glNormal3f  ( -1, 0, 0 );

  glTexCoord2f( 0, 1 );
  glVertex3f  ( -x, y, -z );
  glTexCoord2f( 0, 0 );
  glVertex3f  ( -x, -y, -z );
  glTexCoord2f( 1, 0 );
  glVertex3f  ( -x, -y, z );
  glTexCoord2f( 1, 1 );
  glVertex3f  ( -x, y, z );
  
  glEnd();
}

void Box::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
    ti.addHapticShapeToAll( getOpenGLHapticShape( ti.getCurrentSurface(),
                                                  ti.getAccForwardMatrix(),
                                                  24 ) );
  }
}
