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
/// \file CylinderSensor.h
/// \brief Header file for CylinderSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CYLINDERSENSOR_H__
#define __CYLINDERSENSOR_H__

#include <H3D/X3DDragSensorNode.h>
#include <H3D/SFRotation.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class CylinderSensor
  /// \brief The CylinderSensor node maps pointer motion
  /// (e.g., a mouse or wand) into a rotation on an invisible cylinder that
  /// is aligned with the Y-axis of the local sensor coordinate system.
  ///
  /// The local sensor coordinate system is created by applying the
  /// axisRotation field value to the local coordinate system. The
  /// CylinderSensor uses the descendent geometry of its parent node to
  /// determine whether it is liable to generate events.
  ///
  /// The description field in the CylinderSensor node specifies a textual
  /// description of the CylinderSensor node. This may be used by
  /// browser-specific user interfaces that wish to present users with more
  /// detailed information about the CylinderSensor.
  ///
  /// The enabled field enables and disables the CylinderSensor node. If TRUE,
  /// the sensor reacts appropriately to user events. If FALSE, the sensor does
  /// not track user input or send events. If enabled receives a FALSE event
  /// and isActive is TRUE, the sensor becomes disabled and deactivated, and
  /// outputs an isActive FALSE event. If enabled receives a TRUE event the
  /// sensor is enabled and ready for user activation.
  ///
  /// A CylinderSensor node generates events when the pointing device is
  /// activated while the pointer is indicating any descendent geometry nodes
  /// of the sensor's parent group. Activating and manipulating
  /// pointing device sensors, for more details on using the pointing device
  /// to activate the CylinderSensor.
  ///
  /// Upon activation of the pointing device while indicating the sensor's
  /// geometry, an isActive TRUE event is sent. The initial acute angle between
  /// the bearing vector and the local sensor coordinate system Y-axis of the
  /// CylinderSensor node determines whether the sides of the invisible
  /// cylinder or the caps (disks) are used for manipulation. If the initial
  /// angle is less than the diskAngle, the geometry is treated as an
  /// infinitely large disk lying in Y=0 plane of the local sensor coordinate
  /// system and coincident with the initial intersection point. Dragging
  /// motion is mapped into a rotation around the +Y-axis vector of the local
  /// sensor coordinate system. The perpendicular vector from the initial
  /// intersection point to the Y-axis defines zero rotation about the Y-axis
  /// of the local sensor coordinate system. For each subsequent position of
  /// the bearing, a rotation_changed event is sent that equals the sum of the
  /// rotation about the +Y-axis vector of the local sensor coordinate system
  /// (from the initial intersection to the new intersection) plus the offset
  /// value. trackPoint_changed events reflect the unclamped drag position on
  /// the surface of this disk. When the pointing device is deactivated and
  /// autoOffset is TRUE, offset is set to the last value of rotation_changed
  /// and an offset_changed event is generated.
  ///
  /// If the initial acute angle between the bearing vector and the local
  /// sensor coordinate system Y-axis of the CylinderSensor node is greater
  /// than or equal to diskAngle, then the sensor behaves like a cylinder. The
  /// shortest distance between the point of intersection (between the bearing
  /// and the sensor's geometry) and the Y-axis of the parent group's local
  /// coordinate system determines the radius of an invisible cylinder used to
  /// map pointing device motion and marks the zero rotation value. For each
  /// subsequent position of the bearing, a rotation_changed event is sent that
  /// equals the sum of the right-handed rotation from the original
  /// intersection about the +Y-axis vector plus the offset value.
  /// trackPoint_changed events reflect the unclamped drag position on the
  /// surface of the invisible cylinder. When the pointing device is
  /// deactivated and autoOffset is TRUE, offset is set to the last rotation
  /// angle and an offset_changed event is generated.
  ///
  /// When the sensor generates an isActive TRUE event, it grabs all further
  /// motion events from the pointing device until it is released and generates
  /// an isActive FALSE event (other pointing-device sensors shall not generate
  /// events during this time). Motion of the pointing device while isActive is
  /// TRUE is referred to as a "drag" operation. If a 2D pointing device is in
  /// use, isActive events will typically reflect the state of the primary
  /// button associated with the device (i.e., isActive is TRUE when the
  /// primary button is pressed and FALSE when it is released). If a 3D
  /// pointing device (e.g., a wand) is in use, isActive events will typically
  /// reflect whether the pointer is within or in contact with the sensor's
  /// geometry.
  ///
  /// While the pointing device is activated, trackPoint_changed and
  /// rotation_changed events are output and are interpreted from pointing
  /// device motion based on the sensor's local coordinate system at the time
  /// of activation. trackPoint_changed events represent the unclamped
  /// intersection points on the surface of the invisible cylinder or disk.
  /// If the initial angle results in cylinder rotation (as opposed to disk
  /// behaviour) and if the pointing device is dragged off the cylinder while
  /// activated, browsers may interpret this in a variety of ways (e.g., clamp
  /// all values to the cylinder and continuing to rotate as the point is
  /// dragged away from the cylinder). Each movement of the pointing device
  /// while isActive is TRUE generates trackPoint_changed and rotation_changed
  /// events.
  ///
  /// The minAngle and maxAngle fields clamp rotation_changed events to a range
  /// of values. If minAngle is greater than maxAngle, rotation_changed events
  /// are not clamped. The minAngle and maxAngle fields are restricted to the
  /// range [-2pi, 2pi].
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/CylinderSensor.x3d">CylinderSensor.x3d</a>
  ///     ( <a href="examples/CylinderSensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile CylinderSensor.dot
  class H3DAPI_API CylinderSensor : 
    public X3DDragSensorNode {
  public:

    /// The CheckAngleLimits field checks that the float is in the
    /// correct range. In case it is not within range an error message
    /// is sent and the value is clamped to be within range.
    class H3DAPI_API CheckAngleLimits: 
      public SFFloat {
    public:
      CheckAngleLimits() { limit = 2 * (H3DFloat)Constants::pi; }

      virtual void setValue( const H3DFloat &b, int id = 0 ) {
        if( b < -limit ) {
          SFFloat::setValue( -limit, id );
          Console(3) << "Warning: The value of field " << getName()
                     << " in CylinderSensor node( "
                     << owner->getName() 
                     << " ) is outside valid range [-2pi, 2pi]."
                     << " Value set to -2pi" << endl;
        }
        else if( b > limit ) {
          SFFloat::setValue( limit, id );
          Console(3) << "Warning: The value of field " << getName()
                     << " in CylinderSensor node( "
                     << owner->getName() 
                     << " ) is outside valid range [-2pi, 2pi]."
                     << " Value set to 2pi" << endl;
        }
        else
          SFFloat::setValue( b, id );
      }
    protected:
      virtual void update() {
        SFFloat::update();
        if( value < -limit ) {
          value = -limit;
          Console(3) << "Warning: The value of field " << getName()
                     << " in CylinderSensor node( "
                     << owner->getName() 
                     << " ) is outside valid range [-2pi, 2pi]."
                     << " Value set to -2pi" << endl;
        }
        else if( value > limit ) {
          value  = limit;
          Console(3) << "Warning: The value of field " << getName()
                     << " in CylinderSensor node( "
                     << owner->getName() 
                     << " ) is outside valid range [-2pi, 2pi]."
                     << " Value set to 2pi" << endl;
        }
      }

      H3DFloat limit;
    };
#ifdef __BORLANDC__
    friend class CheckAngleLimits;
#endif

    /// Constructor.
    CylinderSensor( Inst< SFBool           > _autoOffset = 0,
                    Inst< SFString         > _description = 0,
                    Inst< SFFloat          > _diskAngle = 0,
                    Inst< SFBool           > _enabled = 0,
                    Inst< CheckAngleLimits > _maxAngle = 0,
                    Inst< SFNode           > _metadata = 0,
                    Inst< CheckAngleLimits > _minAngle = 0,
                    Inst< SFFloat          > _offset = 0,
                    Inst< SFBool           > _isActive = 0,
                    Inst< SFBool           > _isOver = 0,
                    Inst< SFRotation       > _rotation_changed = 0,
                    Inst< SFVec3f          > _trackPoint_changed = 0,
                    Inst< SFRotation > _axisRotation      = 0 );

    /// Destructor
    ~CylinderSensor();

    // fields
    /// Used when determining whether the sides of the invisible cylinder
    /// or the caps (disks) are used for manipulation.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> pi/12 \n
    /// <b>Valid range:</b> [0-pi/2]
    ///
    /// \dotfile CylinderSensor_diskAngle.dot
    auto_ptr< SFFloat > diskAngle;

    /// Used to clamp rotation values. If maxAngle > minAngle no clamping
    /// is done.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Valid range:</b> [-2pi-2pi]
    ///
    /// <b>Default value:</b> -1 \n
    ///
    /// \dotfile CylinderSensor_maxAngle.dot
    auto_ptr< CheckAngleLimits > maxAngle;

    /// Used to clamp rotation values. If minAngle < maxAngle no clamping
    /// is done.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Valid range:</b> [-2pi-2pi]
    ///
    /// <b>Default value:</b> 0 \n
    ///
    /// \dotfile CylinderSensor_minAngle.dot
    auto_ptr< CheckAngleLimits > minAngle;

    /// When the pointing device is deactivated and autoOffset is TRUE,
    /// offset is set to the last rotation angle
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> 0 \n
    ///
    /// \dotfile CylinderSensor_offset.dot
    auto_ptr< SFFloat > offset;

    /// For each position of the bearing when isActive is true,
    /// a rotation_changed event is sent which corresponds to the sum of
    /// the relative rotation from the original intersection point plus
    /// the offset value.
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile CylinderSensor_rotation_changed.dot
    auto_ptr< SFRotation > rotation_changed;

    /// The local sensor coordinate system is created by applying the
    /// axisRotation field value to the local coordinate system
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 1 0 \n
    ///
    /// \dotfile CylinderSensor_axisRotation.dot
    auto_ptr< SFRotation > axisRotation;

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

    // Intersect segment S(t)=sa+t(sb-sa), 0<=t<=1 against cylinder specified
    // by ca, cb and r (see below).
    int intersectSegmentCylinder( Vec3f sa, Vec3f sb, float &t );

    // intersection between the segment from a to b and the plane defined
    // by planeD and planeNormal (see below). Results are stored in
    // t and q.
    int intersectLinePlane( Vec3f a, Vec3f b, float &t, Vec3f &q );

    // Last intersection point with the cylinder. Only used when sensor is
    // active.
    Vec3f last_intersection;

    // Cylinder segment points. These are modified to make sure that
    // intersection with cylinder fails only when outside the radius, not the
    // caps.
    Vec3f ca, cb;
    // The y_axis, that is, the direction of the cylinder. Used to simplify
    // some calculations since it is always the same value.
    Vec3f y_axis;

    // The radius of the cylinder.
    H3DFloat radius;
    // A number used for plane equation and cylinder cap plane intersections.
    H3DFloat plane_d;

    // The transformation from global to local of the geometry for which this
    // sensor is active. It will not change while sensor is active.
    Matrix4f geometry_global_to_local_original;

    // If true a new Cylinder will be defined.
    bool new_cylinder, use_caps, prev_new_cylinder;
  };
}

#endif
