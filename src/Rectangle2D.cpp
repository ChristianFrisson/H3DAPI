//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file Rectangle2D.cpp
/// \brief CPP file for Rectangle2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <Rectangle2D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Rectangle2D::database( "Rectangle2D", 
                               &(newInstance<Rectangle2D>), 
                               typeid( Rectangle2D ),
                               &X3DGeometryNode::database );

namespace Rectangle2DInternals {
  FIELDDB_ELEMENT( Rectangle2D, size, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Rectangle2D, solid, INPUT_OUTPUT );
}


Rectangle2D::Rectangle2D( Inst< SFNode      > _metadata,
                          Inst< SFBound     > _bound,
                          Inst< DisplayList > _displayList,
                          Inst< MFBool      > _isTouched,
                          Inst< MFVec3f     > _force,
                          Inst< MFVec3f     > _contactPoint,
                          Inst< MFVec3f     > _contactNormal,
                          Inst< SFVec2f     > _size,
                          Inst< SFBool      > _solid ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  size    ( _size     ),
  solid   ( _solid    ) {

  type_name = "Rectangle2D";
  database.initFields( this );

  size->setValue( Vec2f( 2, 2 ) );
  solid->setValue( false );

  size->route( bound );
  
  size->route( displayList );
  solid->route( displayList );

}

void Rectangle2D::render() {
  H3DFloat half_x = size->getValue().x / 2;
  H3DFloat half_y = size->getValue().y / 2;
  
  glBegin( GL_QUADS );
  
  glNormal3f  ( 0, 0, 1 );

  renderTexCoordForActiveTexture( Vec3f( 1, 1, 0 ) );
  glVertex3f  ( half_x, half_y, 0 );
  renderTexCoordForActiveTexture( Vec3f( 0, 1, 0 ) );
  glVertex3f  ( -half_x, half_y, 0 );
  renderTexCoordForActiveTexture( Vec3f( 0, 0, 0 ) );
  glVertex3f  ( -half_x, -half_y, 0 );
  renderTexCoordForActiveTexture( Vec3f( 1, 0, 0 ) );
  glVertex3f  ( half_x, -half_y, 0 );
  
  glEnd();
}

void Rectangle2D::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}

