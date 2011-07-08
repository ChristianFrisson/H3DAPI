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
/// \file X3DEnvironmentTextureNode.h
/// \brief Header file for X3DEnvironmentTextureNode, X3D abstract scene-graph 
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DENVIRONMENTTEXTURENODE_H__
#define __X3DENVIRONMENTTEXTURENODE_H__

#include <H3D/H3DSingleTextureNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DEnvironmentTextureNode
  /// This abstract node type is the base type for all node types that 
  /// specify cubic environment map sources for texture images.
  ///
  /// Cubic environment maps support reflection and specular highlighting
  /// in a simple way. A collection of 2D texture maps define each side of
  /// the cube. All source images shall be square and provide source data 
  /// in powers of two numbers of pixels. Source images in a cubic 
  /// environment map shall have identical sizes. Providing differently sized
  /// images or rectangular images shall be an error.
  /// 
  /// For each texture, the three-dimensional texture coordinates (s,t,r) are
  /// treated as a direction vector from the local origin. Each texel drawn
  /// onto the geometry is treated as the texel in the environment map that
  /// is "seen" from this direction vector.
  ///
  /// Cubic environment maps define a single texture as consisting of six 
  /// separate images, one for each side of a cube. This component defines
  /// the six sides as front, back, left, right, top and bottom.
  class H3DAPI_API X3DEnvironmentTextureNode : public H3DSingleTextureNode {
  public:
    /// Constructor.
    X3DEnvironmentTextureNode( Inst< DisplayList > _displayList = 0,
                               Inst< SFNode>  _metadata = 0 ) :
      H3DSingleTextureNode( _displayList, _metadata ),
      cube_map_id( 0 ),
      texture_unit( GL_TEXTURE0_ARB ) {}

    /// Get the OpenGL texture id that is used for this texture.
    virtual GLuint getTextureId() {
      return cube_map_id;
    }

    /// Get the OpenGL texture unit that is used for this texture.
    virtual GLuint getTextureUnit() {
      return texture_unit;
    }

    /// Get the OpenGL texture target that is used for this texture.
    virtual GLenum getTextureTarget() {
      return GL_TEXTURE_CUBE_MAP_ARB;
    }

  protected:
    GLenum cube_map_id;
    
    /// The OpenGL texture unit that is used to render this texture.
    GLint texture_unit;
  };
}

#endif
