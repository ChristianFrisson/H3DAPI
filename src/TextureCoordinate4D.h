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
/// \file TextureCoordinate4D.h
/// \brief Header file for TextureCoordinate4D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURECOORDINATE4D_H__
#define __TEXTURECOORDINATE4D_H__

#include <X3DTextureCoordinateNode.h>
#include <GL/glew.h>
#include <MFVec4f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TextureCoordinate4D
  /// \brief The TextureCoordinate4D node is a geometry property node that 
  /// specifies a set of 4D (homogeneous 3D) texture coordinates used by 
  /// vertex-based geometry nodes (e.g., IndexedFaceSet) to map 3D textures
  /// to vertices.
  ///
  /// \par Internal routes:
  /// \dotfile TextureCoordinate4D.dot
  class H3DAPI_API TextureCoordinate4D : public X3DTextureCoordinateNode {
  public:
    /// Constructor.
    TextureCoordinate4D( Inst< SFNode >  _metadata = 0,
                         Inst< MFVec4f>  _point   = 0 );

    /// Returns true.
    virtual bool supportsExplicitTexCoords() {
      return true;
    }

    /// Perform the OpenGL commands to render a texture coordinate given the 
    /// index of the texture coordinate. Installs the texture coordinate
    /// as a glTexCoord2f
    virtual void render( int index );

    /// Render the texture transform for the currently active  texture unit.
    virtual void renderForTextureUnit( int index,
                                       unsigned int texture_unit );

    /// Perform the OpenGL commands to render all verties as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Returns the number of texture coordinates this node can render.
    virtual unsigned int nrAvailableTexCoords() {
      return point->size();
    }

    /// A vector of 4d texture coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile TextureCoordinate4D_point.dot 
    auto_ptr< MFVec4f >  point;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
