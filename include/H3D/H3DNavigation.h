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
  
  /// H3DNavigation is a class used for navigation in H3DAPI.
  /// By default navigation can be done with mouse and keyboard.
  /// Each H3DWindowNode should have a H3DNavigation instance.
  class H3DAPI_API H3DNavigation {
  public:

    typedef enum {
      ALL = 1,
      MOUSE = 2,
      KEYBOARD = 3,
      HAPTICSDEVICE = 4,
      SWS = 5
    } NavigationDevices;

    /// Constructor
    H3DNavigation();

    /// Destructor
    ~H3DNavigation() {
      old_vp.reset( NULL );
      h3d_navigations.remove( this );
    }

    /// Does navigation for different navigation modes.
    void doNavigation( string navigation_type, X3DViewpointNode * vp,
                              X3DChildNode *topNode, bool detect_collision,
                              const vector< H3DFloat > &avatar_size, 
                              H3DFloat speed,
                              const vector< string > &transition_type,
                              H3DTime transition_time );

    /// Use values in the enum NavigationDevices to disable a certain device
    static void disableDevice( int device );

    /// Use values in the enum NavigationDevices to enable a certain device
    static void enableDevice( int device );

    static bool isEnabled( int device );

    /// Compares the given X3DViewpointNode to the X3DViewpointNode used by
    /// the NavigationInfo. Used in order to make sure that the same viewpoint
    /// is used for graphics and haptics when a NavigationInfo is in use.
    inline X3DViewpointNode * viewpointToUse(
      X3DViewpointNode *potential_vp ) {
      if( old_vp.get() && old_vp.get() != potential_vp ) {
         return old_vp.get();
      }
      return potential_vp;
    }

    /// Same as the non-static version but can be used to give an index from
    /// the list instead. Used in order to call viewpointToUse from nodes that
    /// does not actually know which window they belong to, or which does not
    /// belong to any window, such as H3DHapticsDevice.
    static X3DViewpointNode * viewpointToUse( X3DViewpointNode *potential_vp,
                                              int nav_index );

    static string getTransitionType(
      const vector< string > &transition_types );

    /// Called when left button has changed.
    inline void leftButtonUpdate( bool _left_button ) {
      if( mouse_nav.get() )
        mouse_nav->leftButtonUpdate( _left_button );
    }

    /// Called for each movement change
    inline void motionUpdate( int x, int y ) {
      if( mouse_nav.get() )
        mouse_nav->motionUpdate( x, y );
    }

    /// Called when scroll wheel updates.
    inline void scrollWheelUpdate( bool up ) {
      if( mouse_nav.get() )
        mouse_nav->scrollWheelUpdate( up );
    }

    inline void handleKeyAction( int key, bool pressed ) {
      if( keyboard_nav.get() )
        keyboard_nav->handleKeyAction( key, pressed );
    }

    // Used to force jump of a viewpoint, for example when an anchor is
    // selected.
    static bool force_jump;

  protected:
    /// Devices which it is possible to use for navigation.
    auto_ptr< MouseNavigation > mouse_nav;
    auto_ptr< KeyboardNavigation > keyboard_nav;
    auto_ptr< HapticDeviceNavigation > haptic_device_nav;
    auto_ptr< SWSNavigation > sws_navigation;

    // Time of last update.
    H3DTime last_time;

    // Variables needed to correctly do linear interpolation between
    // two viewpoints.
    // Flag used to know if we are in linear interpolation mode.
    bool linear_interpolate;
    Vec3f goal_position;
    Rotation goal_orientation;
    Rotation old_vp_orientation;
    Vec3f start_position, move_direction;
    Rotation start_orientation;
    Vec3f old_vp_pos;
    H3DTime start_time;
    AutoRef< X3DViewpointNode > old_vp;
    // List of H3DNavigation instances, needed by the static functions.
    static list< H3DNavigation * > h3d_navigations;

    // Returns true if the scaling factors are uniform, i.e. the same.
    bool isUniformScaling( const Vec3f &scale );

  };

}

#endif
