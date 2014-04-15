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
/// \file TextureCoordinate4D.h
/// \brief Header file for TextureCoordinate4D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURECOORDINATE4D_H__
#define __TEXTURECOORDINATE4D_H__

#include <H3D/X3DTextureCoordinateNode.h>
#include <GL/glew.h>
#include <H3D/MFVec4f.h>

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
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TextureCoordinate4D.x3d">TextureCoordinate4D.x3d</a>
  ///     ( <a href="examples/TextureCoordinate4D.x3d.html">Source</a> )
  class H3DAPI_API TextureCoordinate4D : public X3DTextureCoordinateNode {
  public:
    /// Constructor.
    TextureCoordinate4D( Inst< SFNode >  _metadata = 0,
                         Inst< MFVec4f>  _point   = 0 );

    /// Destructor
    virtual ~TextureCoordinate4D();

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

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( );

    /// VBO rendering implementation
    virtual void renderVBO ( );

    /// VBO disabling implementation
    virtual void disableVBO ( );

    /// Check if this vertex attribute needs to be rendered
    virtual bool preRenderCheckFail ( );

    /// Returns the number of texture coordinates this node can render.
    virtual unsigned int nrAvailableTexCoords() {
      return point->size();
    }

    /// Returns true if the getTexCoord function is available for use.
    /// For a TextureCoordinate node it is
    inline virtual bool supportsGetTexCoord( unsigned int texture_unit ) {
      return true;
    }

    /// Gets texture coordinate of the given index and texture unit.
    virtual Vec4f getTexCoord( int index, unsigned int texture_unit ) {
      return point->getValueByIndex( index );
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
