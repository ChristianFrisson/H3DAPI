//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file DHDHapticsDevice.cpp
/// \brief cpp file for DHDHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "DHDHapticsDevice.h"

using namespace H3D;

H3DNodeDatabase DHDHapticsDevice::database( "DHDHapticsDevice", 
                                            &(newInstance<DHDHapticsDevice>),
                                            typeid( DHDHapticsDevice ),
                                            &H3DThreadedHapticsDevice::database ); 

namespace DHDHapticsDeviceInternals {
}


/// Constructor.
DHDHapticsDevice::DHDHapticsDevice( 
               Inst< ThreadSafeSField< SFVec3f > > _devicePosition,
               Inst< ThreadSafeSField< SFRotation > > _deviceOrientation,
               Inst< TrackerPosition > _trackerPosition        ,
               Inst< TrackerOrientation > _trackerOrientation  ,
               Inst< PosCalibration  > _positionCalibration    ,
               Inst< OrnCalibration  > _orientationCalibration ,
               Inst< SFVec3f         > _proxyPosition          ,
               Inst< WeightedProxy   > _weightedProxyPosition  ,     
               Inst< SFFloat         > _proxyWeighting         ,
               Inst< ThreadSafeSField< SFBool > > _mainButton  ,
               Inst< ThreadSafeSField< SFVec3f > > _force      ,
               Inst< ThreadSafeSField< SFVec3f > > _torque     ,
               Inst< SFInt32         > _inputDOF               ,
               Inst< SFInt32         > _outputDOF              ,
               Inst< SFInt32         > _hapticsRate            ,
               Inst< SFNode          > _stylus                 ,
               Inst< SFBool          > _initialized ) :
  H3DThreadedHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
                            _trackerOrientation, _positionCalibration, 
                            _orientationCalibration, _proxyPosition,
                            _weightedProxyPosition, _proxyWeighting, _mainButton,
                            _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
                            _stylus, _initialized ),
  device_id( -1 ) {

  type_name = "DHDHapticsDevice";  
  database.initFields( this );

#ifndef HAVE_DHDAPI
  Console(4) << "Cannot use DHDHapticsDevice since H3D API is compiled without"
             << " support for it. Define HAVE_DHDAPI in H3DApi.h to "
             << " support it." << endl;
#endif
}


void DHDHapticsDevice::initDevice() {
#ifdef HAVE_DHDAPI
  if( !initialized->getValue() ) {
    device_id = dhdOpen();
    if( device_id == -1 ) {
      Console(4) << "Warning: Failed to open Omega device. Error: " 
                 << dhdErrorGetLastStr() << endl;
    }
  }
#endif
  H3DThreadedHapticsDevice::initDevice();
}

void DHDHapticsDevice::disableDevice() {
#ifdef HAVE_DHDAPI
  if( device_id != -1 ) {
    int id = device_id;
    device_id = -1;
    dhdClose( id );
  }
#endif
  H3DThreadedHapticsDevice::disableDevice();
}


/// Get the position of the haptics device. Only to be called in the 
/// haptics loop.
Vec3f DHDHapticsDevice::getPosition() {
#ifdef HAVE_DHDAPI
  if( device_id != -1 ) {
    Vec3d r;
    dhdGetPosition( &r.z, &r.x, &r.y, device_id );
    return (Vec3f)r;
  }
#endif
  return Vec3f( 0, 0, 0 );
}

/// Get the velocity of the haptics device. Only to be called in the 
/// haptics loop.
Vec3f DHDHapticsDevice::getVelocity() {
  return Vec3f( 0, 0, 0 );
}

bool DHDHapticsDevice::getButtonStatus() {
#ifdef DHD_API
  if( device_id != -1 ) {
    return dhdGetButton( 0, device_id ) == DHD_ON;
  } 
#endif
  return false;
}

/// Get the orientation of the haptics device. Only to be called in the 
/// haptics loop.
Rotation DHDHapticsDevice::getOrientation() {
#ifdef DHD_API
  if( device_id != -1 ) {
    Vec3d r;
    dhdGetOrientationRad( &r.z, &r.x, &r.y, device_id );
    return Rotation( (Vec3f) r );
  }
#endif
  return Rotation( 1, 0, 0, 0 );
}

/// Send the force to render on the haptics device. Only to be called in the 
/// haptics loop.
void DHDHapticsDevice::sendForce( const Vec3f &f ) {
#ifdef DHD_API
  if( device_id != -1 ) {
    dhdSetForce( f.z, f.x, f.y, device_id );
  }
#endif
}

/// Send the torque to render on the haptics device. Only to be called in the 
/// haptics loop.
void DHDHapticsDevice::sendTorque( const Vec3f &f ) {
#ifdef DHD_API
  if( device_id != -1 ) {
    dhdSetTorque( f.z, f.x, f.y, device_id );
  }
#endif
}

