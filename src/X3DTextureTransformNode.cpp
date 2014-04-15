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
/// \file X3DTextureTransformNode.cpp
/// \brief CPP file for X3DTextureTransformNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTextureTransformNode.h>
#include <H3D/MultiTexture.h>

using namespace H3D;

X3DTextureTransformNode::X3DTextureTransformNode( 
                              Inst< DisplayList > _displayList,
                              Inst< SFNode>  _metadata ) :
  X3DAppearanceChildNode( _displayList, _metadata ) {
  type_name = "X3DTextureTransformNode";
}


/// This function will be called by the X3DShapeNode before any rendering 
/// of geometry and before the call to the render function. So this is the
/// place to save the states that are going to be changed in render() in
/// order to restore it in postRender().
void X3DTextureTransformNode::preRender() {
  // we push all current texture transforms onto the stack
  size_t texture_units = 1;
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
  if( mt ) {
    texture_units = mt->texture->size();
    GLint saved_texture;
    glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
    GLint saved_mode;
    glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
    for( size_t i = 0; i < texture_units; ++i ) {
      glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
      glMatrixMode( GL_TEXTURE );
      glPushMatrix();
    }
    glActiveTexture( saved_texture );
    glMatrixMode( saved_mode );
  } else {
    GLint saved_mode;
    glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
    glMatrixMode( GL_TEXTURE );
    glPushMatrix();
    glMatrixMode( saved_mode );
  }
}

/// This function will be called by the X3DShapeNode after the geometry
/// has been rendered to restore the states to what it was before 
/// the call to preRender().
void X3DTextureTransformNode::postRender() {
  // we push all current texture transforms onto the stack
  size_t texture_units = 1;
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
  if( mt ) {
    texture_units = mt->texture->size();
    GLint saved_texture;
    glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
    GLint saved_mode;
    glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
    for( size_t i = 0; i < texture_units; ++i ) {
      glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
      glMatrixMode( GL_TEXTURE );
      glPopMatrix();
    }
    glActiveTexture( saved_texture );
    glMatrixMode( saved_mode );
  } else {
    GLint saved_mode;
    glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
    glMatrixMode( GL_TEXTURE );
    glPopMatrix();
    glMatrixMode( saved_mode );
  }
}

void X3DTextureTransformNode::renderForTextureUnit(
  unsigned int texture_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) texture_unit );
  render();
  glActiveTexture( saved_texture );
}

void X3DTextureTransformNode::renderForTextureUnits( unsigned int start_unit,
                                                     unsigned int end_unit ) {
  
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  for( unsigned int i = start_unit; i <= end_unit; ++i ) {
    renderForTextureUnit( i );
  }
  
  glActiveTexture( saved_texture );
}
