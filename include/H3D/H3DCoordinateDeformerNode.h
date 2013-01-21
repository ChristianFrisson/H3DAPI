//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file H3DCoordinateDeformerNode.h
/// \brief Header file for H3DCoordinateDeformerNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DCOORDINATEDEFORMERNODE_H__
#define __H3DCOORDINATEDEFORMERNODE_H__

#include <H3D/Node.h>

namespace H3D {

  /// \ingroup Abstract Nodes
  /// \brief The H3DCoordinateDeformerNode is the abstract base class for
  /// all nodes specifying deformation of coordinates based on contact
  /// of haptics devices. 
  ///
  /// All subclasses are required to implement the deformPoints method
  /// to specify how the coordinates should be deformed.
  ///
  /// For usages of the node type, see DeformableShape.
  class H3DAPI_API H3DCoordinateDeformerNode : public Node {
  public:

    /// Constructor.       
    H3DCoordinateDeformerNode() {}

    /// The deformPoints method determines the deformation of coordinates
    /// based on the interaction of a haptics device. The is_touched, 
    /// touch_points, touch_normal and touch_force parameters all are vectors
    /// of the same size, where the first value in the vector is the value
    /// for haptics device 0, the second for haptics device 1, and so on.
    /// \param is_touched Specifies if a haptics device is in contact or not.
    /// \param touch_point The contact point in local coordinates for each 
    /// device.
    /// \param touch_normal The normal at the contact point for each device.
    /// \param touch_force The force with which the surface is pressed for 
    /// each device.
    /// \param penetration_points The weightedProxyPosition of each device.
    /// \param orig_points The original points before any deformation.
    /// \param resting_points The resting coordinates (see DeformableShape)
    /// \param deformed_points The deformed coordinates (see DeformableShape)
    /// \param new_resting_points Output parameter that specifies the new
    /// resting coordinates after the deformation. If no changes are made
    /// to the resting coordinates, return an empty vector.
    /// \param new_deformed_points Output parameter that specifies the new
    /// deformed coordinates after the current deformation. If no changes are
    /// made to the deformed coordinates, return an empty vector.
    virtual void deformPoints( const vector< bool  > &is_touched,
                               const vector< Vec3f > &touch_point,
                               const vector< Vec3f > &touch_normal,
                               const vector< Vec3f > &touch_force,
                               const vector< Vec3f > &penetration_points,
                               const vector< Vec3f > &orig_points,
                               const vector< Vec3f > &resting_points,
                               const vector< Vec3f > &deformed_points,
                               vector< Vec3f > &new_resting_points,
                               vector< Vec3f > &new_deformed_points ) = 0;

    /// Returns the default xml containerField attribute value.
    /// For this node it is "deformer".
    virtual string defaultXMLContainerField() {
      return "deformer";
    }

  };
}

#endif
