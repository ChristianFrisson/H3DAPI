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
/// \file H3DNavigationDevices.cpp
/// \brief CPP file for H3DNavigationDevices, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DNavigationDevices.h>
#include <H3D/DeviceInfo.h>

using namespace H3D;

H3DNavigationDevices::DeviceMap H3DNavigationDevices::all_devices;

H3DNavigationDevices::H3DNavigationDevices( H3DNavigation * h3d_navigation ) :
shouldGetInfo( new SFBool ) {
  shouldGetInfo->setValue( false );
  all_devices[h3d_navigation].push_back( this );
  use_center = false;
  zoom = false;
}

void H3DNavigationDevices::resetAll() {
  move_dir = Vec3f();
  rel_rot = Rotation();
  use_center = false;
  zoom = false;
}

MouseNavigation::MouseNavigation( H3DNavigation * h3d_navigation ) :
  H3DNavigationDevices( h3d_navigation ),
  left_button( false ),
  enabled( true ) {
}

void MouseNavigation::resetAll() {
  H3DNavigationDevices::resetAll();
  shouldGetInfo->setValue( false );
}

void MouseNavigation::disableDevice() {
  enabled = false;
}

void MouseNavigation::enableDevice() {
  enabled = true;
}

void MouseNavigation::leftButtonUpdate( bool _left_button ) {
  if( enabled ) {
    if( nav_type == "LOOKAT" ) {
      if( _left_button ) {
        move_dir = Vec3f( mouse_pos.x, mouse_pos.y, 0 );
        rel_rot = Rotation();
        center_of_rot = Vec3f();
        shouldGetInfo->setValue( true );
      }
    }
    else {
      move_dir = Vec3f();
      rel_rot = Rotation();
      center_of_rot = Vec3f();
      shouldGetInfo->setValue( false );
    }
  }
  left_button = _left_button;
}

void MouseNavigation::motionUpdate( int x, int y ) {
  if( enabled ) {
    if( left_button ) {
      Vec2f motion( x - mouse_pos.x, y - mouse_pos.y );
      if( motion * motion > Constants::f_epsilon ) {
        Vec2f perp = Vec2f( -motion.y, -motion.x );
        perp.normalize();
        if( nav_type == "EXAMINE" || nav_type == "ANY" ) {
          rel_rot *=
            Rotation( perp.x, perp.y, 0, motion.length() * 0.01f );
          center_of_rot = Vec3f();
        }
        else if( nav_type == "WALK" ) {
          H3DFloat abs_x = H3DAbs( motion.x );
          if( abs_x > Constants::f_epsilon ) {
            rel_rot *= Rotation( 0, -motion.x / abs_x, 0, abs_x * 0.01f );
          }
          else
            rel_rot *= Rotation();
          move_dir = Vec3f();
          center_of_rot = Vec3f();
        }
        else if( nav_type == "FLY" ) {
          rel_rot *= Rotation( perp.x, perp.y, 0, motion.length() * 0.01f );
          move_dir = Vec3f();
          center_of_rot = Vec3f();
        }
        else {
          rel_rot = Rotation();
          move_dir = Vec3f();
          center_of_rot = Vec3f();
        }
        shouldGetInfo->setValue( true );
      }
    } else {
      if( nav_type != "LOOKAT" ) {
        move_dir = Vec3f();
        rel_rot = Rotation();
        center_of_rot = Vec3f();
        shouldGetInfo->setValue( false );
        use_center = false;
      }
    }
  }
  mouse_pos = Vec2f( (H3DFloat)x, (H3DFloat)y );
}

void MouseNavigation::scrollWheelUpdate( bool up ) {
  if( enabled && ( nav_type == "EXAMINE" || nav_type == "ANY" ) ) {
    if( up ) move_dir += Vec3f( 0, 0, -1 );
    else move_dir += Vec3f( 0, 0, 1 );
    zoom = true;
    shouldGetInfo->setValue( true );
  }
}

KeyboardNavigation::KeyboardNavigation( H3DNavigation * h3d_navigation ) :
  H3DNavigationDevices( h3d_navigation ) {
  upPressed = downPressed = leftPressed = rightPressed = false;
}

void KeyboardNavigation::resetAll() {
  shouldGetInfo->setValue( false );
}

void KeyboardNavigation::handleKeyAction( int key, bool pressed ) {
  if( pressed ) {
    switch( key ) {
      case KeySensor::UP: {
        upPressed = true;
        break;
      }
      case KeySensor::DOWN: {
        downPressed = true;
        break;
      }
      case KeySensor::LEFT: {
        leftPressed = true;
        break;
      }
      case KeySensor::RIGHT: {
        rightPressed = true;
        break;
      }
      default: { }
    }
  } else {
    switch( key ) {
      case KeySensor::UP: {
        upPressed = false;
        break;
      }
      case KeySensor::DOWN: {
        downPressed = false;
        break;
      }
      case KeySensor::LEFT: {
        leftPressed = false;
        break;
      }
      case KeySensor::RIGHT: {
        rightPressed = false;
        break;
      }
      default: { }
    }
  }

  Vec3f temp_move_dir;
  Rotation temp_rel_rot;
  bool temp_value = false;
  string nav_type = getNavType();
  if( nav_type == "WALK" || nav_type == "FLY" ) {
    if( upPressed ) {
      temp_move_dir += Vec3f( 0, 0, -1 );
      temp_rel_rot = Rotation();
      temp_value = true;
    }
    if( downPressed ) {
      temp_move_dir += Vec3f( 0, 0, 1 );
      temp_rel_rot = Rotation();
      temp_value = true;
    }
    if( leftPressed ) {
      temp_move_dir += Vec3f( -1, 0, 0 );
      temp_rel_rot = Rotation();
      temp_value = true;
    }
    if( rightPressed ) {
      temp_move_dir += Vec3f( 1, 0, 0 );
      temp_rel_rot = Rotation();
      temp_value = true;
    }
  }
  else if( nav_type == "EXAMINE" || nav_type == "ANY" ) {
    if( upPressed ) {
      temp_move_dir = Vec3f( 0, 0, 0 );
      temp_rel_rot *= Rotation( 1, 0, 0, 0.01f );
      temp_value = true;
    }
    if( downPressed ) {
      temp_move_dir = Vec3f( 0, 0, 0 );
      temp_rel_rot *= Rotation( 1, 0, 0, -0.01f );
      temp_value = true;
    }
    if( leftPressed ) {
      temp_move_dir = Vec3f( 0, 0, 0 );
      temp_rel_rot *= Rotation( 0, 1, 0, 0.01f );
      temp_value = true;
    }
    if( rightPressed ) {
      temp_move_dir = Vec3f( 0, 0, 0 );
      temp_rel_rot *= Rotation( 0, 1, 0, -0.01f );
      temp_value = true;
    }
  }
  move_dir = temp_move_dir;
  rel_rot = temp_rel_rot;
  center_of_rot = Vec3f();
  shouldGetInfo->setValue( temp_value );
}

HapticDeviceNavigation::HapticDeviceNavigation(
  H3DNavigation * h3d_navigation ) :
  H3DNavigationDevices( h3d_navigation ),
  calculateHapticDeviceMoveInfo( new CalculateHapticDeviceMoveInfo ) {

  calculateHapticDeviceMoveInfo->the_owner = this;
  calculateHapticDeviceMoveInfo->setValue( false );
  DeviceInfo *di = DeviceInfo::getActive();
  if( di && !di->device->empty() ) {
    H3DHapticsDevice *hd =
      static_cast< H3DHapticsDevice * >(di->device->getValueByIndex( 0 ) );
    hd->mainButton->routeNoEvent( calculateHapticDeviceMoveInfo );
    hd->devicePosition->route( calculateHapticDeviceMoveInfo );
    calculateHapticDeviceMoveInfo->route( shouldGetInfo );
  } else {
    Console(4) << "No haptics devices in the scene. "
               << "Navigation with haptics device will not be used." << endl;
  }
  calculate_center = false;
}

void HapticDeviceNavigation::resetAll() {
  move_dir = Vec3f();
}

void HapticDeviceNavigation::CalculateHapticDeviceMoveInfo::update( ) {
  DeviceInfo *di = DeviceInfo::getActive();
  H3DHapticsDevice *hd =
    static_cast< H3DHapticsDevice * >(di->device->getValueByIndex( 0 ) );
  bool tmp_button_pressed = static_cast< SFBool * >(routes_in[0])->getValue();
  Vec3f device_pos = static_cast< SFVec3f * >(routes_in[1])->getValue();
  Rotation device_orn = hd->deviceOrientation->getValue();
  Vec3f scaling_stuff = hd->positionCalibration->getValue().getScalePart();
  if( tmp_button_pressed != button_pressed ) {
    button_pressed = tmp_button_pressed;
    if( button_pressed ) {
      last_orn = device_orn;
      last_pos = device_pos;
      the_owner->calculate_center = true;
    } else {
      the_owner->calculate_center = false;
    }
  }
  
  if( button_pressed ) {
    string nav_type = the_owner->getNavType();
    if( nav_type == "EXAMINE" || nav_type == "ANY" ) {
      Rotation this_orn = device_orn;
      the_owner->rel_rot = -(this_orn * -last_orn);
      last_orn = this_orn;
      Vec3f temp_move_dir = last_pos - device_pos;
      temp_move_dir.x *= scaling_stuff.x;
      temp_move_dir.y *= scaling_stuff.y;
      temp_move_dir.z *= scaling_stuff.z;
      the_owner->move_dir = temp_move_dir;
      last_pos = device_pos;
      the_owner->use_center = true;
    }
    else if( nav_type == "WALK" ) {
      Vec3f dist_change = device_pos - last_pos;
      dist_change.y = 0;
      dist_change.normalizeSafe();
      Rotation this_orn = device_orn;
      last_orn = this_orn;
      Rotation rel_rot = Rotation();
      try {
        Vec3f point = this_orn * Vec3f( 0, 0, 1 );
        point.y = 0.0f;
        point.normalize();
        rel_rot = Rotation( Vec3f( 0, 0, 1 ), point );
        if( rel_rot.angle < 0.02 )
          rel_rot.angle = 0.0f;
        else
          rel_rot.angle = 0.01f * rel_rot.angle;
      }
      catch( const Exception::H3DException & ) {
      }
      the_owner->move_dir = dist_change;
      the_owner->rel_rot = rel_rot;
      the_owner->use_center = false;
    }
    else if( nav_type == "FLY" ) {
      the_owner->rel_rot = Rotation();
      Vec3f dist_change = device_pos - last_pos;
      dist_change.y = 0;
      dist_change.normalizeSafe();
      Rotation this_orn = device_orn;
      last_orn = this_orn;
      Rotation rel_rot = Rotation();
      try {
        Vec3f point1 = this_orn * Vec3f( 0, 0, 1 );
        Vec3f point2 = point1;
        point1.y = 0.0f;
        point1.normalize();
        rel_rot = Rotation( Vec3f( 0, 0, 1 ), point1 );
        
        if( rel_rot.angle < 0.02 )
          rel_rot.angle = 0.0f;
        else
          rel_rot.angle = 0.01f * rel_rot.angle;
        try {
          point2.x = 0.0f;
          point2.normalize();
          Rotation rel_rot2 = Rotation( Vec3f( 0, 0, 1 ), point2 );
          if( rel_rot2.angle < 0.02 )
            rel_rot2.angle = 0.0f;
          else
            rel_rot2.angle = 0.01f * rel_rot2.angle;
          rel_rot = rel_rot * rel_rot2;
        }
        catch( const Exception::H3DException & ) {
        }
      }
      catch( const Exception::H3DException & ) {
      }
      the_owner->move_dir = dist_change;
      the_owner->rel_rot = rel_rot;
      the_owner->use_center = false;
    }
    else if( nav_type == "LOOKAT" ) {
    }
    value = true;
    return;
  }
  value = false;
}

Vec3f HapticDeviceNavigation::getCenterOfRot() {
  DeviceInfo *di = DeviceInfo::getActive();
  if( di && !di->device->empty() ) {
    if( calculate_center ) {
      center_of_rot = static_cast< H3DHapticsDevice * >
        (di->device->getValueByIndex( 0 ) )->weightedProxyPosition->getValue();
      calculate_center = false;
    }
    return center_of_rot;
  } else return Vec3f();
}

SWSNavigation::SWSNavigation( H3DNavigation * h3d_navigation ) :
  H3DNavigationDevices( h3d_navigation ),
  calculateSWSMoveInfo( new CalculateSWSMoveInfo ),
  sws( new SpaceWareSensor ) {
  calculateSWSMoveInfo->the_owner = this;
  calculateSWSMoveInfo->setValue( false );

  sws->instantTranslation->route( calculateSWSMoveInfo );
  sws->instantRotation->route( calculateSWSMoveInfo );
  sws->instantPitch->route( calculateSWSMoveInfo );
  calculateSWSMoveInfo->route( shouldGetInfo );
}

void SWSNavigation::resetAll() {
  H3DNavigationDevices::resetAll();
  calculateSWSMoveInfo->setValue( false );
}

void SWSNavigation::CalculateSWSMoveInfo::update( ) {
  string nav_type = the_owner->getNavType();
  if( event.ptr == routes_in[0] ) {
    if( nav_type == "WALK" ) {
      Vec3f direction = static_cast< SFVec3f * >( routes_in[0] )->getValue();
      direction.y = 0.0f;
      direction.normalizeSafe();
      the_owner->move_dir = direction;
      value = true;
    }
    else if( nav_type == "FLY" ) {
      Vec3f direction = static_cast< SFVec3f * >( routes_in[0] )->getValue();
      direction.normalizeSafe();
      the_owner->move_dir = direction;
      value = true;
    }
  }
  else if( event.ptr == routes_in[1] ) {
    if( nav_type == "EXAMINE" || nav_type == "FLY" || nav_type == "ANY" ) {
      the_owner->rel_rot =
        static_cast< SFRotation * >( routes_in[1] )->getValue();
      value = true;
    }
  }
  else if( event.ptr == routes_in[2] ) {
    if( nav_type == "WALK" ) {
      the_owner->rel_rot =
        static_cast< SFRotation * >( routes_in[2] )->getValue();
      value = true;
    }
  }
  else
    value = false;
}
