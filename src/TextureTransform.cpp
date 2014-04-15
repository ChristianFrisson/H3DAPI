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
/// \file TextureTransform.cpp
/// \brief CPP file for TextureTransform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TextureTransform.h>
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureTransform::database( 
                                           "TextureTransform", 
                                           &(newInstance<TextureTransform>), 
                                           typeid( TextureTransform ),
                                           &X3DTextureTransform2DNode::database );

TextureTransform::TextureTransform( 
                                  Inst< DisplayList > _displayList,
                                  Inst< SFNode >  _metadata,
                                  Inst< SFVec2f>  _center,
                                  Inst< SFFloat>  _rotation,
                                  Inst< SFVec2f>  _scale,
                                  Inst< SFVec2f>  _translation ) :
  X3DTextureTransform2DNode( _displayList, _metadata, _center, 
                             _rotation, _scale, _translation ) {
  type_name = "TextureTransform";
}

void TextureTransform::render() {
  GLint saved_mode;
  glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
  glMatrixMode( GL_TEXTURE );
  glLoadIdentity();
  const Vec2f & c = center->getValue();
  H3DFloat r = rotation->getValue();
  const Vec2f & s = scale->getValue();
  const Vec2f & t = translation->getValue();
  glTranslatef( -c.x, -c.y, 0 );
  glScalef( s.x, s.y, 1 );
  glRotated( r * 180 / Constants::pi, 0, 0, 1 );
  glTranslatef( c.x, c.y, 0 );
  glTranslatef( t.x, t.y, 0 );
  glMatrixMode( saved_mode );
}


