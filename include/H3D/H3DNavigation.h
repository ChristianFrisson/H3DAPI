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
/// \file H3DNavigation.h
/// \brief Header file for H3DNavigation, classes for navigation
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DNAVIGATION_H__
#define __H3DNAVIGATION_H__

#include <H3D/H3DNavigationDevices.h>
#include <H3D/X3DViewpointNode.h>


namespace H3D {
  
  /// H3DNavigation is a Singleton class used for navigation in H3DAPI.
  /// By default navigation can be done with mouse and keyboard.
  class H3DAPI_API H3DNavigation {
  public:

    typedef enum {
      ALL = 1,
      MOUSE = 2,
      KEYBOARD = 3,
      HAPTICSDEVICE = 4,
      SWS = 5
    } NavigationDevices;

    /// destroy the instance when not needed anymore
    static void destroy();

    /// does navigation for different navigation modes.
    static void doNavigation( string navigation_type, X3DViewpointNode * vp,
                              X3DChildNode *topNode, bool detect_collision,
                              const vector< H3DFloat > &avatar_size, 
                              H3DFloat speed );

    /// Use values in the enum NavigationDevices to disable a certain device
    static void disableDevice( int device );

    /// Use values in the enum NavigationDevices to enable a certain device
    static void enableDevice( int device );

    static bool isEnabled( int device );

  private:
    /// Takes care of navigation
    void navigate( string navigation_type, X3DViewpointNode * vp,
                   X3DChildNode *topNode, bool detect_collision,
                   const vector< H3DFloat > &avatar_size, 
                   H3DFloat speed );
    
    /// An instance of this class
    static H3DNavigation * instance;

    /// Private declarations of constructors for singleton class.
    H3DNavigation();
    H3DNavigation(const H3DNavigation&) {}
    H3DNavigation& operator = (const H3DNavigation&) { return *this; }
    ~H3DNavigation() {}

    /// Devices which it is possible to use for navigation.
    auto_ptr< MouseNavigation > mouse_nav;
    auto_ptr< KeyboardNavigation > keyboard_nav;
    auto_ptr< HapticDeviceNavigation > haptic_device_nav;
    auto_ptr< SWSNavigation > sws_navigation;

    H3DTime last_time;
  };

}

#endif
