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
/// \file SphereSensor.h
/// \brief Header file for SphereSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPHERESENSOR_H__
#define __SPHERESENSOR_H__

#include <H3D/X3DDragSensorNode.h>
#include <H3D/SFRotation.h>

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
  /// \par Internal routes:
  /// \dotfile SphereSensor.dot
 
  class H3DAPI_API SphereSensor : 
    public X3DDragSensorNode {
  public:


    /// The Set_SphereEvents field takes care of sending trackPoint_changed,
    /// rotation_changed, and offset events when they should be sent.
    ///
    /// - routes_in[0] is the position field of a MouseSensor
    /// - routes_in[1] is the isActive field.
    class H3DAPI_API Set_SphereEvents: 
      public AutoUpdate< TypedField < SFBool, Types< SFVec2f, SFBool > > > {
    public:

      /// Constructor
      Set_SphereEvents() {
        new_radius = true;
        center = Vec3f( 0, 0, 0 );
      }

      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b, id );
      }
    protected:
      /// update the value of this field and set the fields mentioned above
      virtual void update();

      /// Intersects segment s = a1 + t(a2-a1) with sphere defined by center
      /// and radius (see below), if intersecting, returns t value of
      /// intersection and intersection point q.
      int intersectSegmentSphere( Vec3f a1, Vec3f a2, H3DFloat & t, Vec3f &q );

      H3DFloat radius;
      Vec3f original_intersection, center;
      Matrix4f original_transform_matrix;
      /// If true a new radius will be defined.
      bool new_radius;
    };
#ifdef __BORLANDC__
    friend class Set_SphereEvents;
#endif

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
    /// Called to generate isOver events and other events (dependent on isOver)
    /// if they should be generated.
    virtual void onIsOver( IntersectionInfo *result = 0,
                           Matrix4f *global_to_local = 0 );

    auto_ptr< Set_SphereEvents > set_SphereEvents;
    Vec3f original_intersection;
    Matrix4f intersection_matrix;
  };
}

#endif
