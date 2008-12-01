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
  
  class H3DAPI_API H3DNavigationDevices {
  public:

    /// Constructor.
    H3DNavigationDevices();

    /// Destructor
    virtual ~H3DNavigationDevices() {
      h3dnavigations.remove( this );
    }

    /// Call this function to sum up movement changes from all devices
    /// returns true if there are changes.
    static bool getMoveInfo( Vec3f &translationSum, Rotation &rotationSum,
                             Vec3f &center_of_rot_sum, bool &use_center_sum ) {
      bool somethingmoved = false;
      int centerCounter = 0;
      use_center_sum = false;
      for( list< H3DNavigationDevices * >::iterator i = h3dnavigations.begin();
           i != h3dnavigations.end(); i++ ) {
        if( (*i)->shouldGetInfo->getValue() ) {
          translationSum += (*i)->move_dir;
          rotationSum = rotationSum * (*i)->rel_rot;
          if( (*i)->use_center ) {
            use_center_sum = true;
            centerCounter++;
            center_of_rot_sum += (*i)->getCenterOfRot();
          }
          (*i)->resetAll();
          somethingmoved = true;
        }
      }
      if( use_center_sum )
        center_of_rot_sum /= centerCounter;
      return somethingmoved;
    }

    /// reset values of movement variables
    virtual void resetAll();
    
    /// virtual function to remove routes for this device.
    virtual void disableDevice() {};

    /// virtual function to enable routes for this device.
    virtual void enableDevice() {};
    
    /// sets type of navigation for all devices
    static void setNavTypeForAll( string &_nav_type ) {
      for( list< H3DNavigationDevices * >::iterator i = h3dnavigations.begin();
           i != h3dnavigations.end(); i++ ) {
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

    /// needs to be public so field in sub-classes can set directly.
    Vec3f move_dir;
    Rotation rel_rot;
    Vec3f center_of_rot;
    bool use_center;

    virtual Vec3f getCenterOfRot(){
      return center_of_rot;
    }

  protected:
    auto_ptr< SFBool > shouldGetInfo;
    static list< H3DNavigationDevices * > h3dnavigations;
    string nav_type;
  };


  /// Take care of mouseNavigation
  class H3DAPI_API MouseNavigation : public H3DNavigationDevices {
  public:

    class CalculateMouseMoveInfo :
      public AutoUpdate< TypedField< SFBool, Types< SFBool, SFVec2f,
                                                    SFBool, SFBool > > > {
    public:
      virtual void update();
      MouseNavigation *the_owner;
    };
#ifdef __BORLANDC__
    friend class CalculateMouseMoveInfo;
#endif

    /// Constructor.
    MouseNavigation();

    virtual void resetAll();

    /// virtual function to remove routes for this device.
    virtual void disableDevice();

    /// virtual function to enable routes for this device.
    virtual void enableDevice();

    inline bool isEnabled() { 
      if( mouseSensor.get() && mouseSensor->enabled->getValue() ) return true;
      return false;
    }

  protected:
    auto_ptr< CalculateMouseMoveInfo > calculateMouseMoveInfo;
    auto_ptr< MouseSensor > mouseSensor;
  };

  /// Takes care of navigation using keyboard.
  class H3DAPI_API KeyboardNavigation : public H3DNavigationDevices {
  public:

    class CalculateKeyboardMoveInfo :
      public AutoUpdate< TypedField< SFBool, Types< SFInt32, SFInt32 > > > {
    public:
      CalculateKeyboardMoveInfo() {
        upPressed = downPressed = leftPressed = rightPressed = false;
      }
      virtual void update();

      KeyboardNavigation *the_owner;
    protected:
      bool upPressed;
      bool downPressed;
      bool leftPressed;
      bool rightPressed;
    };
#ifdef __BORLANDC__
    friend class CalculateKeyboardMoveInfo;
#endif

    /// Constructor.
    KeyboardNavigation();

    virtual void resetAll();

  protected:
    auto_ptr< CalculateKeyboardMoveInfo > calculateKeyboardMoveInfo;
    auto_ptr< KeySensor > keySensor;
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
    HapticDeviceNavigation();

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
    SWSNavigation();

    virtual void resetAll();

  protected:
    auto_ptr< CalculateSWSMoveInfo > calculateSWSMoveInfo;
    auto_ptr< SpaceWareSensor > sws;
  };
}

#endif
