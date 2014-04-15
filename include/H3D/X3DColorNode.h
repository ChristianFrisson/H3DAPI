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
/// \file X3DColorNode.h
/// \brief Header file for X3DColorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DCOLORNODE_H__
#define __X3DCOLORNODE_H__

#include <H3D/X3DGeometricPropertyNode.h>
#include <H3D/GLVertexAttributeObject.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DColorNode
  /// \brief This is the base node type for color specifications in X3D. 
  ///
  /// 
  class H3DAPI_API X3DColorNode : 
    public X3DGeometricPropertyNode,
    public GLVertexAttributeObject{
  public:
    /// Constructor.
    X3DColorNode( Inst< SFNode>  _metadata = 0 );

    /// Get the color at index i.
    virtual RGBA getColor( unsigned int i ) = 0;

    /// Perform the OpenGL commands to render a color given the index
    /// of the color.
    virtual void render( int index ) {}
    
    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray() {}

    /// Disable the array state enabled in renderArray().
    virtual void disableArray() {}

    /// Enable state needed before rendering the color.
    virtual void preRender();

    /// Disable state set in preRender() function.
    virtual void postRender();

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( ){};

    /// VBO rendering implementation
    virtual void renderVBO ( ){};

    /// VBO disabling implementation
    virtual void disableVBO ( ){};

    /// Returns the default xml containerField attribute value.
    /// For this node it is "color".
    ///
    virtual string defaultXMLContainerField() {
      return "color";
    }

    /// Returns the number of color this color node can render.
    virtual unsigned int nrAvailableColors() = 0; 
  };
}

#endif
