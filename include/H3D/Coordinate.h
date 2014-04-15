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
/// \file Coordinate.h
/// \brief Header file for Coordinate, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COORDINATE_H__
#define __COORDINATE_H__

#include <H3D/X3DCoordinateNode.h>
#include <H3D/FieldTemplates.h>
#include <GL/glew.h>
#include <H3D/MFVec3f.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Coordinate
  /// \brief This node defines a set of 3D coordinates to be used in the coord
  /// field of vertex-based geometry nodes. 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/IndexedFaceSet.x3d">IndexedFaceSet.x3d</a>
  ///     ( <a href="examples/IndexedFaceSet.x3d.html">Source</a> )
  /// \par Internal routes:
  /// \dotfile Coordinate.dot
  class H3DAPI_API Coordinate : public X3DCoordinateNode {
  public:
    /// Constructor.
    Coordinate( Inst< SFNode  >  _metadata = 0,
                Inst< MFVec3f > _point    = 0);

    /// Destructor.
    virtual ~Coordinate();

    /// Perform the OpenGL commands to render a vertex given the index
    /// of the vertex. We install the vertex as glVectex3f.
    virtual void render( int index ) { 
      const Vec3f &v = point->getValueByIndex( index );
      glVertex3f( v.x, v.y, v.z );
    };

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    // Gets the coordinate of a given index.
    virtual Vec3f getCoord( int index ){ 
      return point->getValueByIndex( index );
    }

    /// Returns the number of coordinates this coordinate node can render.
    virtual unsigned int nrAvailableCoords() {
      return point->size();
    };

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( );

    /// VBO rendering implementation
    virtual void renderVBO ( );

    /// VBO disabling implementation
    virtual void disableVBO ( );

    /// Check if this vertex attribute needs to be rendered
    virtual bool preRenderCheckFail ( );

    /// A vector of Vec3f defining points in 3d-space.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// \dotfile Coordinate_point.dot
    auto_ptr< MFVec3f > point;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
