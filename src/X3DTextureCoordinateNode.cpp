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
/// \file X3DTextureCoordinateNode.cpp
/// \brief CPP file for X3DTextureCoordinateNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DTextureCoordinateNode.h"
#include "GL/glew.h"

using namespace H3D;

X3DTextureCoordinateNode::X3DTextureCoordinateNode( 
                                                   Inst< SFNode>  _metadata ) :
  X3DGeometricPropertyNode( _metadata ) {
  type_name = "X3DTextureCoordinateNode";
}

void X3DTextureCoordinateNode::renderForTextureUnits( int index,
                                                      unsigned int start_unit,
                                                      unsigned int end_unit ) {
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    renderForTextureUnit( index, i );
  }
}

void X3DTextureCoordinateNode::renderArrayForTextureUnits( unsigned int start_unit,
                                                           unsigned int end_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &saved_texture );
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    renderArrayForTextureUnit( i );
  }
  glClientActiveTexture( saved_texture );
}

/// Disable the array state enabled in renderArrayForTextureUnits().
void X3DTextureCoordinateNode::disableArrayForTextureUnits( 
                                   unsigned int start_unit,
                                   unsigned int end_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &saved_texture );
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    disableArrayForTextureUnit( i );
  }
  glClientActiveTexture( saved_texture );
}
