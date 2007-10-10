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
/// \file TextureMatrixTransform.cpp
/// \brief CPP file for TextureMatrixTransform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TextureMatrixTransform.h>
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureMatrixTransform::database( 
                                           "TextureMatrixTransform", 
                                           &(newInstance<TextureMatrixTransform>), 
                                           typeid( TextureMatrixTransform ),
                                           &X3DTextureTransformNode::database );

namespace TextureMatrixTransformInternals {
  FIELDDB_ELEMENT( TextureMatrixTransform, matrix, INPUT_OUTPUT );
}

TextureMatrixTransform::TextureMatrixTransform( 
                                   Inst< DisplayList > _displayList,
                                   Inst< SFNode      > _metadata,
                                   Inst< SFMatrix4f  > _matrix ):
  X3DTextureTransformNode( _displayList, _metadata ),
  matrix( _matrix ) {
  type_name = "TextureMatrixTransform";
  database.initFields( this );
  matrix->setValue( Matrix4f() );

  matrix->route( displayList );
}

void TextureMatrixTransform::render() {
  GLint saved_mode;
  glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
  glMatrixMode( GL_TEXTURE );
  const Matrix4f &m = matrix->getValue();
  
  GLfloat mv[] = { 
    m[0][0], m[1][0], m[2][0], m[3][0],
    m[0][1], m[1][1], m[2][1], m[3][1],
    m[0][2], m[1][2], m[2][2], m[3][2],
    m[0][3], m[1][3], m[2][3], m[3][3] };
  glLoadMatrixf( mv );
  glMatrixMode( saved_mode );
}


