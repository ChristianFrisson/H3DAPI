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
/// \file Color.h
/// \brief Header file for Color, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COLOR_H__
#define __COLOR_H__

#include <H3D/X3DColorNode.h>
#include <GL/glew.h>
#include <H3D/MFColor.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Color
  /// \brief This node defines a set of RGB colours to be used in the fields of
  /// another node. 
  ///
  /// Color nodes are only used to specify multiple colours for a single
  /// geometric shape, such as colours for the faces or vertices of an
  /// IndexedFaceSet. A Material node is used to specify the overall
  /// material parameters of lit geometry. If both a Material node and a
  /// Color node are specified for a geometric shape, the colours shall
  /// replace the diffuse component of the material.
  ///
  /// RGB or RGBA textures take precedence over colours; specifying both an
  /// RGB or RGBA texture and a Color node for geometric shape will result
  /// in the Color node being ignored.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/IndexedFaceSet.x3d">IndexedFaceSet.x3d</a>
  ///     ( <a href="examples/IndexedFaceSet.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Color.dot
  class H3DAPI_API Color : public X3DColorNode {
  public:
    /// Constructor.
    Color( Inst< MFColor>  _color    = 0,
           Inst< SFNode >  _metadata = 0 );

    /// Destructor.
    virtual ~Color();

    /// Get the color at index i.
    virtual RGBA getColor( unsigned int i ) {
      RGB c = color->getValueByIndex( i );
      return RGBA( c.r, c.g, c.b, 1 );
    }

    /// Perform the OpenGL commands to render a color given the index
    /// of the color. Installs the normal as a glColor3f.
    virtual void render( int index ) {
      const RGB &c = color->getValueByIndex( index );
      glColor3f( c.r, c.g, c.b );
    }

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Returns the number of color this color node can render.
    virtual unsigned int nrAvailableColors() {
      return color->size();
    }

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( );

    /// VBO rendering implementation
    virtual void renderVBO ( );

    /// VBO disabling implementation
    virtual void disableVBO ( );

    /// Check if this vertex attribute needs to be rendered
    virtual bool preRenderCheckFail ( );

    /// A vector of Color values.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Color_color.dot
    auto_ptr< MFColor >  color;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
