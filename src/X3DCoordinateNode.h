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
/// \file X3DCoordinateNode.h
/// \brief Header file for X3DCoordinateNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DCOORDINATENODE_H__
#define __X3DCOORDINATENODE_H__

#include "X3DGeometricPropertyNode.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DCoordinateNode
  /// \brief This is the base node type for all coordinate node types
  /// in X3D. All coordinates are specified in nodes derived from this
  /// abstract node type.
  ///
  /// 
  class H3DAPI_API X3DCoordinateNode : public X3DGeometricPropertyNode {
  public:
    /// Constructor.
    X3DCoordinateNode( Inst< SFNode>  _metadata = 0 );

    // Gets the coordinate of a given index.
    virtual Vec3f getCoord( int index ) = 0;

    /// Perform the OpenGL commands to render a vertex given the index
    /// of the vertex.
    virtual void render( int index ) {}

    /// Perform the OpenGL commands to render all verties as a vertex
    /// array.
    virtual void renderArray() {}

    /// Disable the array state enabled in renderArray().
    virtual void disableArray() {}

    /// Returns the default xml containerField attribute value.
    /// For this node it is "coord".
    ///
    virtual string defaultXMLContainerField() {
      return "coord";
    }

    /// Returns the number of coordinates this coordinate node can render.
    virtual unsigned int nrAvailableCoords() = 0; 
  };
}

#endif
