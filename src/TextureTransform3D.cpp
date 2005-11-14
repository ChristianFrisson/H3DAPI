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
/// \file TextureTransform3D.cpp
/// \brief CPP file for TextureTransform3D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "TextureTransform3D.h"
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureTransform3D::database( 
                                           "TextureTransform3D", 
                                           &(newInstance<TextureTransform3D>), 
                                           typeid( TextureTransform3D ),
                                           &X3DTextureTransformNode::database );

namespace TextureTransform3DInternals {
  FIELDDB_ELEMENT( TextureTransform3D, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureTransform3D, rotation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureTransform3D, scale, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureTransform3D, translation, INPUT_OUTPUT );
}


TextureTransform3D::TextureTransform3D( 
                                   Inst< DisplayList > _displayList,
                                   Inst< SFNode      > _metadata,
                                   Inst< SFVec3f     > _center,
                                   Inst< SFRotation  > _rotation,
                                   Inst< SFVec3f     > _scale,
                                   Inst< SFVec3f     > _translation ) :
  X3DTextureTransformNode( _displayList, _metadata ),
  center     ( _center      ),
  rotation   ( _rotation    ),
  scale      ( _scale       ),
  translation( _translation ) {
  type_name = "TextureTransform3D";
  database.initFields( this );
  center->setValue( Vec3f( 0, 0, 0 ) );
  rotation->setValue( Rotation( 0, 0, 1, 0 ) );
  scale->setValue( Vec3f( 1, 1, 1 ) );
  translation->setValue( Vec3f( 0, 0, 0 ) );

  center->route( displayList );
  rotation->route( displayList );
  scale->route( displayList );
  translation->route( displayList );
}

void TextureTransform3D::render() {
  GLint saved_mode;
  glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
  glMatrixMode( GL_TEXTURE );
  glLoadIdentity();
  const Vec3f & c = center->getValue();
  const Rotation &r = rotation->getValue();
  const Vec3f & s = scale->getValue();
  const Vec3f & t = translation->getValue();
  glTranslatef( -c.x, -c.y, -c.z );
  glScalef( s.x, s.y, s.z );
  glRotated( r.angle * 180 / Constants::pi, r.axis.x, r.axis.y, r.axis.z );
  glTranslatef( c.x, c.y, c.z );
  glTranslatef( t.x, t.y, t.z );
  glMatrixMode( saved_mode );
}


