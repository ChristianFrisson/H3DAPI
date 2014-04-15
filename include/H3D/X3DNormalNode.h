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
/// \file X3DNormalNode.h
/// \brief Header file for X3DNormalNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DNORMALNODE_H__
#define __X3DNORMALNODE_H__

#include <H3D/X3DGeometricPropertyNode.h>
#include <H3D/GLVertexAttributeObject.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DNormalNode
  /// \brief This is the base node type for all normal node types in X3D. All
  /// normals are specified in nodes derived from this abstract node type.
  ///
  /// 
  class H3DAPI_API X3DNormalNode : 
    public X3DGeometricPropertyNode,
    public GLVertexAttributeObject{
  public:
    
    /// Constructor.
    X3DNormalNode( Inst< SFNode>  _metadata = 0 );

    // Gets the normal of a given index.
    virtual Vec3f getNormal( int index ) = 0;

    /// Perform the OpenGL commands to render a normal given the index
    /// of the normal.
    virtual void render( int index ) {}

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray() {}

    /// Disable the array state enabled in renderArray().
    virtual void disableArray(){}

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( ){};

    /// VBO rendering implementation
    virtual void renderVBO ( ){};

    /// VBO disabling implementation
    virtual void disableVBO ( ){};
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "normal".
    ///
    virtual string defaultXMLContainerField() {
      return "normal";
    }

    /// Returns the number of normals this normal node can render.
    virtual unsigned int nrAvailableNormals() = 0; 
  };
}

#endif
