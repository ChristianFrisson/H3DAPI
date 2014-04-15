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
/// \file PlaneSensor.h
/// \brief Header file for PlaneSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PLANESENSOR_H__
#define __PLANESENSOR_H__

#include <H3D/X3DDragSensorNode.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFRotation.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PlaneSensor
  /// \brief The PlaneSensor node maps pointing device motion into
  /// two-dimensional translation in a plane parallel to the Z=0 plane of the
  /// local sensor coordinate system.
  ///
  /// The local sensor coordinate system is created by applying the
  /// axisRotation field value to the local coordinate system. The PlaneSensor
  /// node uses the descendent geometry of its parent node to determine whether
  /// it is liable to generate events.
  ///
  /// The description field in the PlaneSensor node specifies a textual
  /// description of the PlaneSensor node. This may be used by browser-specific
  /// user interfaces that wish to present users with more detailed information
  /// about the PlaneSensor.
  ///
  /// The enabled field enables and disables the PlaneSensor. If enabled is
  /// TRUE, the sensor reacts appropriately to user events. If enabled is
  /// FALSE, the sensor does not track user input or send events. If enabled
  /// receives a FALSE event and isActive is TRUE, the sensor becomes disabled
  /// and deactivated, and outputs an isActive FALSE event. If enabled receives
  /// a TRUE event, the sensor is enabled and made ready for user activation.
  ///
  /// The PlaneSensor node generates events when the pointing device is
  /// activated while the pointer is indicating any descendent geometry nodes
  /// of the sensor's parent group.
  ///
  /// Upon activation of the pointing device (e.g., mouse button down) while
  /// indicating the sensor's geometry, an isActive TRUE event is sent. Pointer
  /// motion is mapped into relative translation in the tracking plane,
  /// (a plane parallel to the sensor's local sensor coordinate system Z=0
  /// plane and coincident with the initial point of intersection). For each 
  /// subsequent movement of the bearing, a translation_changed event is output
  /// which corresponds to the sum of the relative translation from the
  /// original intersection point to the intersection point of the new bearing
  /// in the plane plus the offset value. The sign of the translation is
  /// defined by the Z=0 plane of the local sensor coordinate system.
  /// trackPoint_changed events reflect the unclamped drag position on the
  /// surface of this plane. When the pointing device is deactivated and
  /// autoOffset is TRUE, offset is set to the last translation_changed value
  /// and an offset_changed event is generated.
  /// 
  /// minPosition and maxPosition may be set to clamp translation_changed 
  /// events to a range of values as measured from the origin of the Z=0 plane
  /// of the local sensor coordinate system. If the X or Y component of
  /// minPosition is greater than the corresponding component of maxPosition,
  /// translation_changed events are not clamped in that dimension. If the X or
  /// Y component of minPosition is equal to the corresponding component of
  /// maxPosition, that component is constrained to the given value. This
  /// technique provides a way to implement a line sensor that maps dragging
  /// motion into a translation in one dimension.
  ///
  /// While the pointing device is activated and moved, trackPoint_changed and
  /// translation_changed events are sent. trackPoint_changed events represent
  /// the unclamped intersection points on the surface of the tracking plane.
  /// If the pointing device is dragged off of the tracking plane while
  /// activated (e.g., above horizon line), browsers may interpret this in a
  /// variety ways (e.g., clamp all values to the horizon. H3DAPI resend the
  /// last event and write an error message ). Each movement of
  /// the pointing device, while isActive is TRUE, generates trackPoint_changed
  /// and translation_changed events.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PlaneSensor.x3d">PlaneSensor.x3d</a>
  ///     ( <a href="examples/PlaneSensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile PlaneSensor.dot
  // NOTE: If the scenegraph is changed while isActive is true for a 
  // planeSensor the planeSensor might not be working correctly anymore.
  class H3DAPI_API PlaneSensor : 
    public X3DDragSensorNode {
  public:

    /// Class used to store some information about axisRotation when
    /// offset is set, in order to set the correct offset value when
    /// mouse is clicked.
    class H3DAPI_API SFOffset :
      public OnValueChangeSField< AutoUpdate< SFVec3f > > {
    public:
    protected:
      virtual void onValueChange( const Vec3f &new_value ) {
        PlaneSensor * ps = static_cast< PlaneSensor * >(getOwner());
        ps->last_offset_axis_rotation_inv =
          Matrix4f( ps->axisRotation->getValue() ).inverse();
      }
    };

    /// Constructor.
    PlaneSensor(  Inst< SFBool >  _autoOffset           = 0,
                  Inst< SFString > _description         = 0,
                  Inst< SFBool >  _enabled              = 0,
                  Inst< SFVec2f > _maxPosition          = 0,
                  Inst< SFNode >  _metadata             = 0,
                  Inst< SFVec2f > _minPosition          = 0,
                  Inst< SFOffset > _offset               = 0,
                  Inst< SFBool >  _isActive             = 0,
                  Inst< SFBool > _isOver                = 0,
                  Inst< SFVec3f >  _trackPoint_changed  = 0,
                  Inst< SFVec3f >  _translation_changed = 0,
                  Inst< SFRotation > _axisRotation      = 0 );

    ~PlaneSensor();

    // fields
    /// maxPosition can be used to clamp translation_changed events.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> -1 -1 \n
    ///
    /// \dotfile PlaneSensor_maxPosition.dot
    auto_ptr< SFVec2f > maxPosition;

    /// minPosition can be used to clamp translation_changed events
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> 0 0 \n
    ///
    /// \dotfile PlaneSensor_minPosition.dot
    auto_ptr< SFVec2f > minPosition;

    /// Stores the last translation_changed value at deactivation
    /// if autoOffset is true.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> 0 0 0 \n
    ///
    /// \dotfile PlaneSensor_offset.dot
    auto_ptr< SFOffset > offset;

    /// For each subsequent movement of the bearing, a translation_changed
    /// event is output which corresponds to the sum of the relative
    /// translation from the original intersection point to the intersection
    /// point of the new bearing in the plane plus the offset value.
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile PlaneSensor_translation_changed.dot
    auto_ptr< SFVec3f > translation_changed;

    /// The local sensor coordinate system is created by applying the
    /// axisRotation field value to the local coordinate system
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 1 0 \n
    ///
    /// \dotfile PlaneSensor_axisRotation.dot
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

    // intersection between the segment from a to b and the plane defined
    // by planeD and planeNormal (see below). Results are stored in
    // t and q.
    int intersectLinePlane( Vec3f a, Vec3f b, float &t, Vec3f &q );

    // Last intersection point with the cylinder. Only used when sensor is
    // active.
    Vec3f last_intersection;

    // Normal of the plane to intersect with.
    Vec3f plane_normal;
    // A number used for plane equation and intersections.
    H3DFloat plane_d;
    // The transformation from global to local of the geometry for which this
    // sensor is active. It will not change while sensor is active.
    Matrix4f active_global_to_local_matrix;
    // If true a new plane will be defined.
    bool new_plane;
    // The last rotation used when offset was set.
    Matrix4f last_offset_axis_rotation_inv;
  };
}

#endif
