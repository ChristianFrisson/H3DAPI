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
/// \file H3DNavigationDevices.h
/// \brief Header file for H3DNavigationDevices, classes for navigation
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DNAVIGATIONDEVICES_H__
#define __H3DNAVIGATIONDEVICES_H__

#include <H3D/MouseSensor.h>
#include <H3D/KeySensor.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFRotation.h>
#include <H3D/SpaceWareSensor.h>

namespace H3D {

  // Forward Declaration
  class H3DNavigation;

  /// Base class for devices that should be used to navigate the scene.
  class H3DAPI_API H3DNavigationDevices {
  public:

    struct MoveInfo{
    public:
      MoveInfo() {
        zoom = false;
        use_center_sum = false;
      }
      Vec3f translation_sum;
      Rotation rotation_sum;
      Vec3f center_of_rot_sum;
      bool use_center_sum;
      bool zoom;
    };

    /// Constructor.
    H3DNavigationDevices( H3DNavigation * h3d_navigation );

    /// Destructor
    virtual ~H3DNavigationDevices() {
      for( DeviceMap::iterator i = all_devices.begin();
           i != all_devices.end(); ++i ) {
        (*i).second.remove( this );
      }
    }

    /// Call this function to sum up movement changes from all devices
    /// returns true if there are changes.
    static bool getMoveInfo( MoveInfo &move_info,
                             H3DNavigation * h3d_navigation ) {
      bool somethingmoved = false;
      int centerCounter = 0;
      for( list< H3DNavigationDevices * >::iterator i =
             all_devices[h3d_navigation].begin();
           i != all_devices[h3d_navigation].end(); ++i ) {
        if( (*i)->shouldGetInfo->getValue() ) {
          move_info.translation_sum += (*i)->move_dir;
          move_info.rotation_sum = move_info.rotation_sum * (*i)->rel_rot;
          move_info.zoom = (*i)->zoom;
          if( (*i)->use_center ) {
            move_info.use_center_sum = true;
            ++centerCounter;
            move_info.center_of_rot_sum += (*i)->getCenterOfRot();
          }
          (*i)->resetAll();
          somethingmoved = true;
        }
      }
      if( move_info.use_center_sum )
        move_info.center_of_rot_sum /= centerCounter;
      return somethingmoved;
    }

    /// reset values of movement variables
    virtual void resetAll();
    
    /// virtual function to remove routes for this device.
    virtual void disableDevice() {};

    /// virtual function to enable routes for this device.
    virtual void enableDevice() {};
    
    /// sets type of navigation for all devices
    static void setNavTypeForAll( string &_nav_type,
                                  H3DNavigation * h3d_navigation ) {
      for( list< H3DNavigationDevices * >::iterator i =
             all_devices[h3d_navigation].begin();
           i != all_devices[h3d_navigation].end(); ++i ) {
          (*i)->setNavType( _nav_type );
      }
    }

    /// Set type of navigation
    void setNavType( string &_nav_type ) {
      nav_type = _nav_type;
    }

    /// Get the current type of navigation
    string getNavType( ) {
      return nav_type;
    }

    /// Get the center or rotation.
    virtual Vec3f getCenterOfRot(){
      return center_of_rot;
    }

  protected:
    // If this field is true then move_dir, rel_rot, center_of_rot, use_center
    // and zoom contains something useful.
    auto_ptr< SFBool > shouldGetInfo;
    // Need to map device to H3DNavigation.
    typedef map< H3DNavigation*, list< H3DNavigationDevices * > > DeviceMap;
    static DeviceMap all_devices;
    // Navigation type.
    string nav_type;

    // move_dir is the distance to move for WALK, FLY and EXAMINE mode while it
    // is the direction to move when zooming in EXAMINE mode.
    Vec3f move_dir;
    // The relative rotation for navigation.
    Rotation rel_rot;
    // The center of rotation, only used if use_center is true.
    Vec3f center_of_rot;
    // If true center_of_rot is used.
    bool use_center;
    // If true we are in zoom mode.
    bool zoom;
  };


  /// Take care of mouseNavigation
  class H3DAPI_API MouseNavigation : public H3DNavigationDevices {
  public:
    /// Constructor.
    MouseNavigation( H3DNavigation * h3d_navigation );

    /// Reset all variables
    virtual void resetAll();

    /// virtual function to remove routes for this device.
    virtual void disableDevice();

    /// virtual function to enable routes for this device.
    virtual void enableDevice();

    /// Check if MouseNavigation is enabled.
    inline bool isEnabled() { 
      return enabled;
    }

    /// Called when left button has changed.
    void leftButtonUpdate( bool _left_button );

    /// Called for each movement change
    void motionUpdate( int x, int y );

    /// Called when scroll wheel updates.
    void scrollWheelUpdate( bool up );
  protected:
    // Contains left button value
    bool left_button;
    // Contains mouse pos value
    Vec2f mouse_pos;
    // True if MouseNavigation is enabled.
    bool enabled;
  };

  /// Takes care of navigation using keyboard.
  class H3DAPI_API KeyboardNavigation : public H3DNavigationDevices {
  public:

    /// Constructor.
    KeyboardNavigation( H3DNavigation * h3d_navigation );

    /// Reset variables.
    virtual void resetAll();

    /// Handle key pressed to set navigation correctly.
    void handleKeyAction( int key, bool pressed );

  protected:
    // Variables used to indicate which keys are pressed and which are not.
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
  };

  /// Takes care of navigation using a haptics device.
  class H3DAPI_API HapticDeviceNavigation : public H3DNavigationDevices {
  public:

    class CalculateHapticDeviceMoveInfo :
      public TypedField< SFBool, Types< SFBool, SFVec3f > > {

    public:
      CalculateHapticDeviceMoveInfo() {
        button_pressed = false;
      }

      virtual void update();

      HapticDeviceNavigation *the_owner;
    protected:
      bool button_pressed;
      Rotation last_orn;
      Vec3f last_pos;
      Vec3f last_weight_pos;
    };
#ifdef __BORLANDC__
    friend class CalculateHapticDeviceMoveInfo;
#endif

    /// Constructor.
    HapticDeviceNavigation( H3DNavigation * h3d_navigation );

    virtual void resetAll();

    virtual Vec3f getCenterOfRot();

  protected:
    auto_ptr< CalculateHapticDeviceMoveInfo > calculateHapticDeviceMoveInfo;
    // Needed because the center of rotation should be calculated as late
    // as possible and only once. Used in getCenterOfRot();
    bool calculate_center;

  };

  /// Takes care of navigation with SpaceWareSensor.
  /// The first created SpaceWareSensor is used.
  class H3DAPI_API SWSNavigation : public H3DNavigationDevices {
  public:

    class CalculateSWSMoveInfo :
      public AutoUpdate< TypedField< SFBool, Types< SFVec3f, SFRotation, SFRotation > > > {

    public:
      CalculateSWSMoveInfo() {
      }

      virtual void update();

      SWSNavigation *the_owner;
    protected:
    };
#ifdef __BORLANDC__
    friend class CalculateSWSMoveInfo;
#endif

    /// Constructor.
    SWSNavigation( H3DNavigation * h3d_navigation );

    virtual void resetAll();

  protected:
    auto_ptr< CalculateSWSMoveInfo > calculateSWSMoveInfo;
    auto_ptr< SpaceWareSensor > sws;
  };
}

#endif
