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
/// \file ColorRGBA.h
/// \brief Header file for ColorRGBA, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COLORRGBA_H__
#define __COLORRGBA_H__

#include <H3D/X3DColorNode.h>
#include <H3D/FieldTemplates.h>
#include <GL/glew.h>
#include <H3D/MFColorRGBA.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class ColorRGBA
  /// \brief This node defines a set of RGBA colours to be used in the 
  /// fields of another node. 
  ///
  /// RGBA color nodes are only used to specify multiple colours with alpha
  /// for a single geometric shape, such as colours for the faces or
  /// vertices of an IndexedFaceSet. A Material node is used to specify the
  /// overall material parameters of lit geometry. If both a Material node
  /// and a ColorRGBA node are specified for a geometric shape, the colours
  /// shall replace the diffuse and transparency components of the material.
  ///
  /// RGB or RGBA textures take precedence over colours; specifying both an
  /// RGB or RGBA texture and a Color node for geometric shape will result
  /// in the Color node being ignored. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ColorRGBA.x3d">ColorRGBA.x3d</a>
  ///     ( <a href="examples/ColorRGBA.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ColorRGBA.dot
  class H3DAPI_API ColorRGBA : public X3DColorNode {
  public:
    /// Constructor.
    ColorRGBA( Inst< MFColorRGBA>  _color    = 0,
               Inst< SFNode     >  _metadata = 0 );

    /// Destructor.
    virtual ~ColorRGBA();

    /// Get the color at index i.
    virtual RGBA getColor( unsigned int i ) {
      return color->getValueByIndex( i );
    }

    /// Perform the OpenGL commands to render a color given the index
    /// of the color. Installs the normal as a glColor4f.
    virtual void render( int index ) {
      const RGBA &c = color->getValueByIndex( index );
      glColor4f( c.r, c.g, c.b, c.a );
    }

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Enable state needed before rendering the color.
    virtual void preRender() {
      X3DColorNode::preRender();
      glEnable( GL_BLEND );
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    /// Disable state set in preRender() function.
    virtual void postRender() {
      X3DColorNode::postRender();
      glDisable( GL_BLEND );
    }

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

    /// A vector of ColorRGBA values.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile ColorRGBA_color.dot
    auto_ptr< MFColorRGBA >  color;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
