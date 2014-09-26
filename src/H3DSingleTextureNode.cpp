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
/// \file H3DSingleTextureNode.cpp
/// \brief cpp file for H3DSingleTextureNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DSingleTextureNode.h> 
#include <H3D/X3DShaderNode.h>

using namespace H3D;

/// Constructor
H3DSingleTextureNode::H3DSingleTextureNode( 
  Inst< DisplayList > _displayList,
  Inst< SFNode  > _metadata ): 
  X3DTextureNode( _displayList, _metadata ),
  texture_handle ( 0 ) {
}

H3DSingleTextureNode::~H3DSingleTextureNode () {
  if ( X3DShaderNode::use_bindless_textures ) {
    makeNonResident ();
  }
}

bool H3DSingleTextureNode::makeResident () {
  if ( !texture_handle ) {
#ifdef GL_ARB_bindless_texture
    texture_handle= glGetTextureHandleARB ( getTextureId() );
    if ( glGetError() == GL_NO_ERROR ) {
      glMakeTextureHandleResidentARB ( texture_handle );
      if ( glGetError() == GL_NO_ERROR ) {
        return true;
      }
    }
#endif

    texture_handle= 0;
  }

  return false;
}

void H3DSingleTextureNode::makeNonResident () {
  if ( texture_handle ) {
#ifdef GL_ARB_bindless_texture
    if ( glIsTextureHandleResidentARB ( texture_handle ) ) {
      glMakeTextureHandleNonResidentARB ( texture_handle );
    }
#endif

    // Ignore errors making the texture non-resident
    // todo: Why are these errors detected here, e.g. when resizing fbtg?
    glGetError ();

    texture_handle= 0;
  }
}