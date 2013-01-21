//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file H3DSingleTextureNode.h
/// \brief Header file for H3DSingleTextureNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSINGLETEXTURENODE_H__
#define __H3DSINGLETEXTURENODE_H__

#include <H3D/X3DTextureNode.h>
#include <H3D/H3DImageObject.h>
#include <H3D/TextureProperties.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {
  /// \ingroup AbstractNodes
  /// \brief This abstract node type is the base type for all texture 
  /// nodes that is defined by a single texture.
  ///
  /// More specifically the texture can be defined by a single OpenGL 
  /// texture id.
  class H3DAPI_API H3DSingleTextureNode : 
    public X3DTextureNode {
  public:
    
    /// Constructor.
    H3DSingleTextureNode( Inst< DisplayList > _displayList = 0,
			  Inst< SFNode  > _metadata  = 0 ): 
      X3DTextureNode( _displayList, _metadata ) {
    }

    /// Get the OpenGL texture id that is used for this texture.
    virtual GLuint getTextureId() = 0;

    /// Get the OpenGL texture unit that is used for this texture.
    virtual GLuint getTextureUnit() = 0;

    /// Get the OpenGL texture target that is used for this texture.
    virtual GLenum getTextureTarget() = 0;
  };
}

#endif
