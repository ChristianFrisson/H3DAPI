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
/// \file ProximitySensor.h
/// \brief Header file for ProximitySensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PROXIMITYSENSOR_H__
#define __PROXIMITYSENSOR_H__

#include <H3D/X3DEnvironmentalSensorNode.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFRotation.h>
#include <H3D/X3DViewpointNode.h>
#include <H3D/NavigationInfo.h>



namespace H3D {

  /// \ingroup AbstractNodes
  /// \class ProximitySensor
  /// \brief The ProximitySensor node generates events when the viewer enters,
  /// exits, and moves within a region in space (defined by a box). A 
  /// proximity sensor is enabled or disabled by sending it an enabled event
  /// with a value of TRUE or FALSE. A disabled sensor does not send events.
  /// A ProximitySensor node generates isActive TRUE/FALSE events as the viewer
  /// enters and exits the rectangular box defined by its center and size 
  /// fields. Browsers shall interpolate viewer positions and timestamp the
  /// isActive events with the exact time the viewer first intersected the
  /// proximity region. The center field defines the centre point of the 
  /// proximity region in object space. The size field specifies a vector 
  /// which defines the width (x), height (y), and depth (z) of the box
  /// bounding the region. The components of the size field shall be greater
  /// than or equal to zero. ProximitySensor nodes are affected by the 
  /// hierarchical transformations of their parents.
  ///
  /// The enterTime event is generated whenever the isActive TRUE event is
  /// generated (user enters the box), and exitTime events are generated
  /// whenever an isActive FALSE event is generated (user exits the box).
  ///
  /// The centerOfRotation_changed field sends events whenever the user is
  /// contained within the proximity region and the center of rotation of the
  /// viewer for EXAMINE mode changes with respect to the ProximitySensor 
  /// node's coordinate system. This may result when the bound Viewpoint 
  /// nodes's center of rotation changes, when a new viewpoint is bound, when
  /// the user changes the center of rotation through the browser's user 
  /// interface, or from changes to the ProximitySensor node's coordinate 
  /// system. centerOfRotation_changed events are only generated when the 
  /// currently bound NavigationInfo node includes LOOKAT navigation. 
  ///
  /// The position_changed and orientation_changed fields send events whenever
  /// the user is contained within the proximity region and the position and
  /// orientation of the viewer changes with respect to the ProximitySensor
  /// node's coordinate system including enter and exit times. The viewer
  /// movement may be a result of a variety of circumstances resulting from
  /// browser navigation, ProximitySensor node's coordinate system changes, or
  /// bound Viewpoint node's position or orientation changes.
  ///
  /// Each ProximitySensor node behaves independently of all other 
  /// ProximitySensor nodes. Every enabled ProximitySensor node that is
  /// affected by the viewer's movement receives and sends events, possibly 
  /// resulting in multiple ProximitySensor nodes receiving and sending events
  /// simultaneously. Unlike TouchSensor nodes, there is no notion of a
  /// ProximitySensor node lower in the scene graph "grabbing" events.
  /// Instanced (DEF/USE) ProximitySensor nodes use the union of all the boxes
  /// to check for enter and exit. A multiply instanced ProximitySensor node 
  /// will detect enter and exit for all instances of the box and send 
  /// enter/exit events appropriately. For non-overlapping bounding boxes, 
  /// position_changed and orientation_changed events are calculated relative
  /// to the coordinate system associated with the bounding box in which the
  /// proximity was detected. However, the results are undefined if the any of
  /// the boxes of a multiply instanced ProximitySensor node overlap.
  ///
  /// A ProximitySensor node that surrounds the entire world has an enterTime 
  /// equal to the time that the world was entered and can be used to start up
  /// animations or behaviours as soon as a world is loaded. A ProximitySensor 
  /// node with a box containing zero volume (i.e., any size field element of 
  /// 0.0) cannot generate events. This is equivalent to setting the enabled 
  /// field to FALSE.
  ///
  /// A ProximitySensor read from an X3D file shall generate isActive TRUE,
  /// position_changed, orientation_changed and enterTime events if the sensor
  /// is enabled and the viewer is inside the proximity region. A 
  /// ProximitySensor inserted into the transformation hierarchy shall 
  /// generate isActive TRUE, position_changed, orientation_changed and 
  /// enterTime events if the sensor is enabled and the viewer is inside
  /// the proximity region. A ProximitySensor removed from the transformation
  /// hierarchy shall generate isActive FALSE, position_changed, 
  /// orientation_changed and exitTime events if the sensor is enabled and the
  /// viewer is inside the proximity region.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ProximitySensor.x3d">ProximitySensor.x3d</a>
  ///     ( <a href="examples/ProximitySensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ProximitySensor.dot

  class H3DAPI_API ProximitySensor : public X3DEnvironmentalSensorNode {
  public:
    

    /// The SetBoolean class is specialize to set the value of either
    /// the enterTime or exitTime field in the ProximitySensor node
    /// it resides in, depending on the value that is is given.
    class H3DAPI_API SetTime: public AutoUpdate< SFBool > {
    public:
      // Sets either the exitTime or enterTime field depending
      // on the new value.
      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b , id );
        ProximitySensor *ps = 
          static_cast< ProximitySensor * >( getOwner() );
      if( value )
      ps->enterTime->setValue( TimeStamp() , ps->id ); 
        else 
            ps->exitTime->setValue( TimeStamp() , ps->id ); 
      }
    protected:
      // Sets either the exitTime or enterTime field depending
      // on the new value.
      virtual void update() {
        SFBool::update();
        ProximitySensor *ps = 
          static_cast< ProximitySensor * >( getOwner() );
        if( value )
          ps->enterTime->setValue( TimeStamp() , ps->id );
        else 
          ps->exitTime->setValue( TimeStamp(), ps->id ); 
      }
    };

    /// Constructor.
    ProximitySensor( Inst< SFNode > _metadata = 0,
                     Inst< SFVec3f > _center = 0,
                     Inst< SFBool > _enabled = 0,
                     Inst< SFVec3f > _size = 0,
                     Inst< SFTime > _enterTime = 0,
                     Inst< SFTime > _exitTime = 0,
                     Inst< SFBool > _isActive = 0,
                     Inst< SFVec3f > _centerOfRotation_changed = 0,
                     Inst< SFRotation > _orientation_changed = 0,
                     Inst< SFVec3f > _position_changed = 0);


    /// Traverse function
    virtual void traverseSG( TraverseInfo &ti ); 


    // Fields

    /// The centerOfRotation_changed field sends events whenever the user is
    /// contained within the proximity region and the center of rotation of the
    /// viewer for EXAMINE mode changes with respect to the ProximitySensor 
    /// node's coordinate system.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile ProximitySensor_centerOfRotation_changed.dot
    auto_ptr< SFVec3f >  centerOfRotation_changed;

    /// The orientation_changed fields send events whenever the user is
    /// contained within the proximity region and the orientation of the viewer
    /// changes with respect to the ProximitySensor
    /// node's coordinate system including enter and exit times.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile ProximitySensor_orientation_changed.dot
    auto_ptr< SFRotation >  orientation_changed;

    /// The position_changed fields send events whenever the user is
    /// contained within the proximity region and the position of the viewer
    /// changes with respect to the ProximitySensor
    /// node's coordinate system including enter and exit times.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile ProximitySensor_position_changed.dot
    auto_ptr< SFVec3f >  position_changed;


    /// Sets enterTime if the set_boolean value is set 
    /// to TRUE else sets exitTime.
    /// Only accessible in C++.
    /// 
    /// \dotfile ProximitySensor_set_time.dot
    auto_ptr< SetTime > set_time;

    /// Adress of traverseInfo 
    /// only interested in adress, what it points to will be invalid
    TraverseInfo *prev_travinfoadr;


    // Internal Vectors for previous position of viewpoint
    Vec3f prev_vp_pos;
    Vec3f can_prev_vp_pos;

  
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
