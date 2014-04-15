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
/// \file TextureCoordinate3D.h
/// \brief Header file for TextureCoordinate3D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURECOORDINATE3D_H__
#define __TEXTURECOORDINATE3D_H__

#include <H3D/X3DTextureCoordinateNode.h>
#include <GL/glew.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TextureCoordinate3D
  /// \brief The TextureCoordinate3D node is a geometry property node 
  /// that specifies a set of 3D texture coordinates used by vertex-based
  /// geometry nodes (EXAMPLE  IndexedFaceSet and ElevationGrid) to map 
  /// textures to vertices.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageTexture3D.x3d">ImageTexture3D.x3d</a>
  ///     ( <a href="examples/ImageTexture3D.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile TextureCoordinate3D.dot
  class H3DAPI_API TextureCoordinate3D : public X3DTextureCoordinateNode {
  public:
    /// Constructor.
    TextureCoordinate3D( Inst< SFNode >  _metadata = 0,
                         Inst< MFVec3f>  _point   = 0 );

    /// Destructor
    virtual ~TextureCoordinate3D();

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
    /// For a TextureCoordinate3D node it is
    inline virtual bool supportsGetTexCoord( unsigned int texture_unit ) {
      return true;
    }

    /// Gets texture coordinate of the given index and texture unit.
    virtual Vec4f getTexCoord( int index, unsigned int texture_unit ) {
      Vec3f p = point->getValueByIndex( index );
      return Vec4f( p.x, p.y, p.z, 1 );
    }

    /// A vector of 3d texture coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile TextureCoordinate3D_point.dot 
    auto_ptr< MFVec3f >  point;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
