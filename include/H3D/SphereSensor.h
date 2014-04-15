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
/// \file SphereSensor.h
/// \brief Header file for SphereSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPHERESENSOR_H__
#define __SPHERESENSOR_H__

#include <H3D/X3DDragSensorNode.h>
#include <H3D/SFRotation.h>
#include <H3D/SFVec2f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class SphereSensor
  /// \brief The SphereSensor node maps pointing device motion into spherical
  /// rotation about the origin of the local coordinate system.
  /// The SphereSensor node uses the descendent geometry of its parent node to
  /// determine whether it is liable to generate events.
  ///
  /// The description field in the SphereSensor node specifies a textual
  /// description of the SphereSensor node. This may be used by
  /// browser-specific user interfaces that wish to present users with more
  /// detailed information about the SphereSensor.
  ///
  /// The enabled field enables and disables the SphereSensor node. If enabled
  /// is TRUE, the sensor reacts appropriately to user events. If enabled is
  /// FALSE, the sensor does not track user input or send events. If enabled
  /// receives a FALSE event and isActive is TRUE, the sensor becomes disabled
  /// and deactivated, and outputs an isActive FALSE event. If enabled receives
  /// a TRUE event the sensor is enabled and ready for user activation.
  ///
  /// The SphereSensor node generates events when the pointing device is
  /// activated while the pointer is indicating any descendent geometry nodes
  /// of the sensor's parent group. 
  ///
  /// Upon activation of the pointing device (e.g., mouse button down) over
  /// the sensor's geometry, an isActive TRUE event is sent. The vector defined
  /// by the initial point of intersection on the SphereSensor's geometry and
  /// the local origin determines the radius of the sphere that is used to map
  /// subsequent pointing device motion while dragging. The virtual sphere
  /// defined by this radius and the local origin at the time of activation is
  /// used to interpret subsequent pointing device motion and is not affected
  /// by any changes to the sensor's coordinate system while the sensor is
  /// active. For each position of the bearing, a rotation_changed event is
  /// sent which corresponds to the sum of the relative rotation from the
  /// original intersection point plus the offset value. trackPoint_changed
  /// events reflect the unclamped drag position on the surface of this sphere.
  /// When the pointing device is deactivated and autoOffset is TRUE, offset is
  /// set to the last rotation_changed value and an offset_changed event is
  /// generated.
  ///
  /// While the pointing device is activated, trackPoint_changed and
  /// rotation_changed events are output. trackPoint_changed events represent
  /// the unclamped intersection points on the surface of the invisible sphere.
  /// If the pointing device is dragged off the sphere while activated,
  /// browsers may interpret this in a variety of ways (e.g., clamp all values
  /// to the sphere or continue to rotate as the point is dragged away from the
  /// sphere). Each movement of the pointing device while isActive is TRUE
  /// generates trackPoint_changed and rotation_changed events.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SphereSensor.x3d">SphereSensor.x3d</a>
  ///     ( <a href="examples/SphereSensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SphereSensor.dot
  class H3DAPI_API SphereSensor : 
    public X3DDragSensorNode {
  public:

    /// Constructor.
    SphereSensor(     Inst< SFBool >  _autoOffset = 0,
                      Inst< SFString > _description = 0,
                      Inst< SFBool >  _enabled = 0,
                      Inst< SFNode >  _metadata = 0,
                      Inst< SFRotation  > _offset = 0,
                      Inst< SFBool >  _isActive = 0,
                      Inst< SFBool > _isOver = 0,
                      Inst< SFRotation  >  _rotation_changed = 0,
                      Inst< SFVec3f >  _trackPoint_changed = 0 );

    // fields
    /// When the pointing device is deactivated and autoOffset is TRUE,
    /// offset is set to the last rotation_changed value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 1 0 0 \n
    ///
    /// \dotfile SphereSensor_offset.dot
    auto_ptr< SFRotation > offset;

   
    /// For each position of the bearing when isActive is true,
    /// a rotation_changed event is sent which corresponds to the sum of
    /// the relative rotation from the original intersection point plus
    /// the offset value.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile SphereSensor_rotation_changed.dot
    auto_ptr< SFRotation > rotation_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:

    // Virtual function that should check the isActive field and if it
    // is true then update specific fields. If isActive field is false then
    // reset state of internal variables. Do not call enabled->getValue() in
    // this function.
    // _enabled - Should be treated as enabled->getValue().
    // from - The start of the line segment for which to intersect with.
    // to - The end of the line segment for which to intersect with.
    virtual void setDragOutputEvents( bool _enabled,
                                      const Vec3f &from,
                                      const Vec3f &to );

    // Called to generate isOver events and other events which dependens on
    // isOver.
    virtual void onIsOver( IntersectionInfo *result = 0,
                           Matrix4f *global_to_local = 0 );

    // Stores points of intersection with any of the geometry which causes this
    // sensor to generate isOver events.
    Vec3f geometry_intersection;

    // Stores global_to_local matrix of the geometry which causes this
    // sensor to generate isOver events.
    Matrix4f geometry_global_to_local;

    /// Intersects segment s = a1 + t(a2-a1) with sphere defined by center
    /// and radius (see below), if intersecting, returns t value of
    /// intersection and intersection point q.
    int intersectSegmentSphere( Vec3f a1, Vec3f a2, H3DFloat & t, Vec3f &q );

    // Radius of the sphere.
    H3DFloat radius;

    // Last intersection point with the cylinder. Only used when sensor is
    // active.
    Vec3f last_intersection;
    // The center of the sphere. Is always Vec3f( 0, 0, 0 ).
    Vec3f center;

    // The transformation from global to local of the geometry for which this
    // sensor is active. It will not change while sensor is active.
    Matrix4f active_global_to_local_matrix;
    /// If true a new radius will be defined.
    bool new_radius, prev_new_radius;
  };
}

#endif
