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
/// \file X3DPointingDeviceSensorNode.h
/// \brief Header file for X3DPointingDeviceSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPOINTINGDEVICESENSORNODE_H__
#define __X3DPOINTINGDEVICESENSORNODE_H__

#include <H3D/X3DSensorNode.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/SFString.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DPointingDeviceSensorNode
  /// \brief This abstract node type is the base type for all 
  /// pointing device sensors.
  ///
  /// Pointing-device sensors detect user pointing events such as the user
  /// clicking on a piece of geometry (i.e., TouchSensor). The following node
  /// types are pointing-device sensors:
  /// <ul>
  /// <li>CylinderSensor</a></li>
  /// <li>PlaneSensor</a></li>
  /// <li>SphereSensor</a></li>
  /// <li>TouchSensor</a></li>
  /// </ul>
  ///
  /// A pointing-device sensor is activated when the user locates the pointing
  /// device over geometry that is influenced by that specific pointing-device
  /// sensor. Pointing-device sensors have influence over all geometry that is
  /// descended from the sensor's parent groups. Typically, the pointing-device
  /// sensor is a sibling to the geometry that it influences. In other cases,
  /// the sensor is a sibling to groups which contain geometry
  /// (i.e., are influenced by the pointing-device sensor).
  ///
  /// The appearance properties of the geometry do not affect activation of the
  /// sensor. In particular, transparent materials or textures shall be treated
  /// as opaque with respect to activation of pointing-device sensors.
  ///
  /// For a given user activation, the lowest enabled pointing-device sensor in
  /// the hierarchy is activated. All other pointing-device sensors above the
  /// lowest enabled pointing-device sensor are ignored. The hierarchy is
  /// defined by the geometry node over which the pointing-device sensor is
  /// located and the entire hierarchy upward. If there are multiple
  /// pointing-device sensors tied for lowest, each of these is activated
  /// simultaneously and independently, possibly resulting in multiple sensors
  /// activating and generating output simultaneously. This feature allows
  /// combinations of pointing-device sensors
  /// (e.g., TouchSensor and PlaneSensor). If a pointing-device sensor appears
  /// in the transformation hierarchy multiple times (DEF/USE), it shall be
  /// tested for activation in all of the coordinate systems in which it
  /// appears.
  ///
  /// If a pointing-device sensor is not enabled when the pointing-device
  /// button is activated, it will not generate events related to the pointing
  /// device until after the pointing device is deactivated and the sensor is
  /// enabled (i.e., enabling a sensor in the middle of dragging does not
  /// result in the sensor activating immediately).
  ///
  /// The pointing device controls a pointer in the virtual world. While 
  /// activated by the pointing device, a sensor will generate events as 
  /// the pointer moves. Typically the pointing device may be categorized as 
  /// either 2D (e.g., conventional mouse) or 3D (e.g., wand). It is suggested
  /// that the pointer controlled by a 2D device is mapped onto a plane a fixed
  /// distance from the viewer and perpendicular to the line of sight. The
  /// mapping of a 3D device may describe a 1:1 relationship between movement
  /// of the pointing device and movement of the pointer.
  ///
  /// The position of the pointer defines a bearing which is used to determine
  /// which geometry is being indicated. When implementing a 2D pointing device
  /// it is suggested that the bearing is defined by the vector from the viewer
  /// position through the location of the pointer. When implementing a 3D
  /// pointing device it is suggested that the bearing is defined by extending
  /// a vector from the current position of the pointer in the direction
  /// indicated by the pointer. 
  ///
  /// In all cases the pointer is considered to be indicating a specific
  /// geometry when that geometry is intersected by the bearing. If the bearing
  /// intersects multiple sensors' geometries, only the sensor nearest to the
  /// pointer will be eligible for activation.
  /// \par Internal routes:
  /// \dotfile X3DPointingDeviceSensorNode.dot
  // TODO: Allowing for 3D pointing device. 
  // (which could be a haptics device perhaps)
  class H3DAPI_API X3DPointingDeviceSensorNode : 
    public X3DSensorNode {
  public:

    /// Constructor.
    X3DPointingDeviceSensorNode( Inst< SFString > _description = 0,
                                 Inst< SFBool   > _enabled  = 0,
                                 Inst< SFNode   > _metadata = 0,
                                 Inst< SFBool   > _isActive = 0,
                                 Inst< SFBool   > _isOver = 0 );

    ~X3DPointingDeviceSensorNode();

    // Fields
    /// The description field in a X3DPointingDeviceSensorNode node specifies a
    /// textual description of the X3DPointingDeviceSensorNode node. This may 
    /// be used by browser-specific user interfaces that wish to present users 
    /// with more detailed information about the X3DPointingDeviceSensorNode.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile X3DPointingDeviceSensorNode_description.dot
    auto_ptr< SFString > description;

    /// The isOver field reflects the state of the pointing device with regard
    /// to whether it is pointing towards the X3DPointingDeviceSensorNode 
    /// node's geometry or not. When the pointing device changes state from a
    /// position such that its bearing does not intersect any of the 
    /// X3DPointingDeviceSensorNode node's geometry to one in which it does
    /// intersect geometry, an isOver TRUE event is generated. When the
    /// pointing device moves from a position such that its bearing intersects
    /// geometry to one in which it no longer intersects the geometry, or some
    /// other geometry is obstructing the X3DPointingDeviceSensorNode node's
    /// geometry, an isOver FALSE event is generated. These events are
    /// generated only when the pointing device has moved and changed `over'
    /// state. Events are not generated if the geometry itself is animating
    /// and moving underneath the pointing device.
    ///
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile X3DPointingDeviceSensorNode_isOver.dot
    auto_ptr< SFBool > isOver;

    /// Called to detect and set properties of X3DPointingDeviceSensors.
    /// The call to this function is done in H3DWindowNode::render
    /// \param n The node to do intersection tests with, e.g. the sceneRoot.
    /// \param from The start of the line segment for which to intersect with.
    /// \param to The end of the line segment for which to intersect with..
    static void updateX3DPointingDeviceSensors( Node * n,
                                                const Vec3f &from,
                                                const Vec3f &to );

    /// Update fields that depends on the status of the primary pointing device
    /// button. So far only implemented for mouse.
    static void updateButtonDependentFields( bool primary_button );

    /// Called to query, whether any X3DPointingDeviceSensors is in isOver
    /// state.
    static bool anyIsOver();

    /// Called to query, whether any X3DPointingDeviceSensors is in isActive
    /// state.
    static bool anyIsActive();

    /// Returns true if there are any X3DPointingDeviceSensors in the scene.
    static inline bool instancesExists() {
      return !instances.empty();
    }

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:

    // The SetIsEnabled class is specialized to call setIsEnabled function
    // when the enabled field chances value. If enabled is set to true while
    // the primary pointing device button is pressed then no events are sent
    // until the primary pointing device button is released.
    // If the enabled field is set to false and isActive and isOver are true
    // then those are set to false.
    //
    // - routes_in[0] is the enabled field of X3DPointingDeviceSensorNode
    class H3DAPI_API SetIsEnabled: 
      public AutoUpdate< SFBool > {
    public:
      virtual void setValue( const bool &b, int id = 0 ) {
        AutoUpdate< SFBool >::setValue( b, id );
        X3DPointingDeviceSensorNode *pdsn =
          static_cast< X3DPointingDeviceSensorNode * >(owner);
        pdsn->setIsEnabled( pdsn->last_primary_button_value );
      }
    protected:
      virtual void update() {
        AutoUpdate< SFBool >::update();
        X3DPointingDeviceSensorNode *pdsn =
          static_cast< X3DPointingDeviceSensorNode * >(owner);
        pdsn->setIsEnabled( pdsn->last_primary_button_value );
      }
    };
#ifdef __BORLANDC__
    friend class SetIsEnabled;
#endif

    // This value is used in onIsOver. Set here instead of sending as argument
    // cause it is more effective to go through all instances of
    // X3DPointingDeviceSensorNodes twice than it is to compare pointers for
    // all devices to know if it should be true or not.
    bool new_value;

    // This value must be true for a sensor to become active.
    bool lowest_enabled;

    // Called to generate isOver events if they should be
    // generated.
    virtual void onIsOver( IntersectionInfo *result = 0,
                           Matrix4f *global_to_local = 0 ) {
      if( is_enabled && ( isActive->getValue() || number_of_active == 0 ) ) {
        if( new_value != isOver->getValue() )
          isOver->setValue( new_value, id );
      }
    }

    // To indicate how many active devices there are.
    static int number_of_active;
    
    // used instead of enabled to enable and disable X3DPointingDeviceSensors.
    // correctly.
    bool is_enabled;

    // Used to call setIsEnabled when the enabled field is changed.
    // C++ only field.
    auto_ptr< SetIsEnabled > setIsEnabledField;

    // Contains information about the last value of the primary button of the
    // pointing device sensor and the intersection points.
    static bool last_primary_button_value;
    static Vec3f last_from;
    static Vec3f last_to;

    // Used to setIsEnabled if needed.
    void setIsEnabled( bool primary_button );
    void setIsActive( bool primary_button );

    // Virtual function that should check the isActive field and if it
    // is true then update specific fields. If isActive field is false then
    // reset state of internal variables. Do not call enabled->getValue() in
    // this function.
    // _enabled - Should be treated as enabled->getValue().
    // from - The start of the line segment for which to intersect with.
    // to - The end of the line segment for which to intersect with.
    virtual void setDragOutputEvents( bool _enabled,
                                      const Vec3f &from,
                                      const Vec3f &to ) {};

  private:
    // The instances of X3DPointingDeviceSensorNode that has been created.
    static vector< X3DPointingDeviceSensorNode * > instances;
  };
}

#endif
