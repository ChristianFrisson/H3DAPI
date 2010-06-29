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
/// \file CoordinateDouble.h
/// \brief Header file for CoordinateDouble, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COORDINATEDOUBLE_H__
#define __COORDINATEDOUBLE_H__

#include <H3D/X3DCoordinateNode.h>
#include <H3D/FieldTemplates.h>
#include <GL/glew.h>
#include <H3D/MFVec3d.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class CoordinateDouble
  /// \brief This node defines a set of 3D coordinates to be used in the coord
  /// field of vertex-based geometry nodes. Unlike the Coordinate node it allows 
  /// the definition of 3D coordinates in double precision floating point values.
  ///
  /// \par Internal routes:
  /// \dotfile Coordinate.dot
  class H3DAPI_API CoordinateDouble : public X3DCoordinateNode {
  public:
    /// Constructor.
    CoordinateDouble( Inst< SFNode  >  _metadata = 0,
                      Inst< MFVec3d >  _point    = 0 );

    /// Destructor.
    virtual ~CoordinateDouble();

    /// Perform the OpenGL commands to render a vertex given the index
    /// of the vertex. We install the vertex as glVertex3d.
    virtual void render( int index ) { 
      const Vec3d &v = point->getValueByIndex( index );
      glVertex3d( v.x, v.y, v.z );
    };

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    // Gets the coordinate of a given index.
    // TODO: change return type to Vec3f
    virtual Vec3f getCoord( int index ){ 
      return static_cast< Vec3f >( point->getValueByIndex( index ) );
    }

    /// Returns the number of coordinates this coordinate node can render.
    virtual unsigned int nrAvailableCoords() {
      return point->size();
    }; 

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// buffer object.
    virtual void renderVertexBufferObject();

    /// Disable the vertex buffer object enabled in renderVertexBufferObject().
    virtual void disableVertexBufferObject();

    /// A vector of Vec3d defining points in 3d-space.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// \dotfile Coordinate_point.dot
    auto_ptr< MFVec3d > point;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    // Internal field used to know if vertex buffer object can be created.
    auto_ptr< Field > vboFieldsUpToDate;
    // The index for the vertex buffer object
    GLuint *vbo_id;
  };
}

#endif
