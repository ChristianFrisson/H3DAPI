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
/// \file X3DPointingDeviceSensorNode.h
/// \brief Header file for X3DPointingDeviceSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPOINTINGDEVICESENSORNODE_H__
#define __X3DPOINTINGDEVICESENSORNODE_H__

#include <H3D/MouseSensor.h>
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
  ///
  // TODO: Allowing for 3D pointing device. 
  // (which could be a haptics device perhaps)
  // Maybe change behaviour for 
  // the case when several sensors are defined under eachother
  // read behaviour of multisensor.x3d ( in X3Ds conformance test and test)
  // to understand.

  class H3DAPI_API X3DPointingDeviceSensorNode : 
    public X3DSensorNode {
  public:

    /// The SetIsActive class is specialize field to set the isActive field.
    /// isActive is set to true if the primary pointing device button is
    /// pressed while isOver is true. isActive is set to false when the
    /// primary pointing device button is released if it was previously 
    /// pressed.
    ///
    /// - routes_in[0] is the isOver field
    /// - routes_in[1] is the leftButton field of a MouseSensor
    class H3DAPI_API SetIsActive: 
      public AutoUpdate< TypedField < SFBool, Types< SFBool, SFBool > > > {
    public:
      SetIsActive() {
        left_mouse_miss = false;
      }

      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b, id );
      }
    protected:
      virtual void update();

      bool left_mouse_miss;
    };
#ifdef __BORLANDC__
    friend class SetIsActive;
#endif

    /// The SetIsEnabled class is specialize to check if the primary pointing
    /// device button is pressed when the enabled field is set to true. 
    /// If so then enabled will not
    /// cause events until the primary pointing device button is released.
    /// Also if the enabled field is false and isActive is true then isActive
    /// is set to false.
    ///
    /// - routes_in[0] is the enabled field of X3DPointingDeviceSensorNode
    /// - routes_in[1] is the leftButton field of MouseSensor
    class H3DAPI_API SetIsEnabled: 
      public AutoUpdate< TypedField < SFBool, Types< SFBool, SFBool > > > {
    public:
      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b, id );
      }
    protected:
      virtual void update() {
        SFBool::update();
        bool _enabled = static_cast< SFBool * >( routes_in[0] )->getValue();
        bool leftButton = static_cast< SFBool * >( routes_in[1] )->getValue();
        
        X3DPointingDeviceSensorNode *pdsn = 
            static_cast< X3DPointingDeviceSensorNode * >( getOwner() );
        if( _enabled != pdsn->is_enabled ) {
          if( leftButton && _enabled && !pdsn->is_enabled ) {
            pdsn->is_enabled = false;
          }
          else {
            pdsn->is_enabled = _enabled;
          }
        }

        if( routes_in[0] == event.ptr && 
            !_enabled && pdsn->isActive->getValue( pdsn->id ) ) {
          pdsn->isActive->setValue( _enabled, pdsn->id );
          number_of_active--;
        }
      }
    };
#ifdef __BORLANDC__
    friend class SetIsEnabled;
#endif

    /// Constructor.
    X3DPointingDeviceSensorNode( Inst< SFString > _description = 0,
                                 Inst< SFBool >  _enabled  = 0,
                                 Inst< SFNode >  _metadata = 0,
                                 Inst< SFBool >  _isActive = 0,
                                 Inst< SFBool > _isOver = 0 );

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

    /// Add the geometryNode to the vector of geometryNodes.
    /// Called in traverseSG function of X3DGeometryNode.
    void addGeometryNode( Node * n, H3DInt32 geom_index );

    /// Returns the index of the pt_dev_sensor in which the geometry
    /// with the correct index exist.
    int findGeometry( pair< Node * , H3DInt32 > &geom );

    /// Sets the current_matrix to m
    void setCurrentMatrix( Matrix4f m ) {
      pt_matrices[ current_pt_id ] = m;
    }

    H3DInt32 increaseIndex( TraverseInfo &ti );

    static void updateX3DPointingDeviceSensors( Node * n );
    static void clearGeometryNodes();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:

    /// Called to generate isOver events if they should be
    /// generated.
    virtual void onIsOver( bool newValue, 
      IntersectionInfo &result, int pt_id ) {
      if( is_enabled && ( isActive->getValue() || number_of_active == 0 ) ) {
        if( newValue != isOver->getValue() )
          isOver->setValue( newValue, id );
      }
    }

    /// \brief Used to find out if the 2D pointing device ( e.g. the mouse )
    /// has moved. In that case collision with all geometries need to be done.
    static bool has2DPointingDeviceMoved( Vec2f & pos );

    // static variables for pointing Device 2D (e.g. mouse )
    static MouseSensor *mouse_sensor;
    static Vec2f pos_device2D;
    static Vec3f near_plane_pos;
    static Vec3f far_plane_pos;

    // To indicate how many active devices there are.
    static int number_of_active;
    
    // used instead of enabled to enable and disable X3DPointingDeviceSensors.
    // correctly.
    bool is_enabled;

    // Instances of specialized fields.
    auto_ptr< SetIsEnabled > setIsEnabled;
    auto_ptr< SetIsActive > setIsActive;

    typedef map< H3DInt32, vector< pair< Node *, H3DInt32 > > >
      PtIdGeomIdMap;
    // Vectors for geometries and the corresponding local transformation
    // matrix of the X3DPointingDeviceSensor for that geometry.
    
    PtIdGeomIdMap geometry_nodes;
    map< H3DInt32, Matrix4f > pt_matrices;

    H3DInt32 current_pt_id;
    TraverseInfo * last_ti_ptr;
    
  private:
    // The instances of X3DPointingDeviceSensorNode that has been created.
    static vector< X3DPointingDeviceSensorNode * > instances;
  };
}

#endif
