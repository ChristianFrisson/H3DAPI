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
/// \file TextureCoordinate.h
/// \brief Header file for TextureCoordinate, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURECOORDINATE_H__
#define __TEXTURECOORDINATE_H__

#include "X3DTextureCoordinateNode.h"
#include "GL/glew.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class TextureCoordinate
  /// \brief The TextureCoordinate node is a geometry property node 
  /// that specifies a set of 2D texture coordinates used by vertex-based
  /// geometry nodes (EXAMPLE  IndexedFaceSet and ElevationGrid) to map 
  /// textures to vertices.
  ///
  /// \par Internal routes:
  /// \dotfile TextureCoordinate.dot
  class H3DAPI_API TextureCoordinate : public X3DTextureCoordinateNode {
  public:
    /// Constructor.
    TextureCoordinate( Inst< SFNode >  _metadata = 0,
                       Inst< MFVec2f>  _point   = 0 );

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

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the given texture unit.
    virtual void renderArrayForTextureUnit( unsigned int texture_unit );

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Disable the array state enabled in renderArrayForTextureUnit().
    virtual void disableArrayForTextureUnit( unsigned int texture_unit );

    /// Returns the number of texture coordinates this node can render.
    virtual unsigned int nrAvailableTexCoords() {
      return point->size();
    }

    /// A vector of 2d texture coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile TextureCoordinate_point.dot 
    auto_ptr< MFVec2f >  point;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
