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
/// \file HaptikHapticsDevice.cpp
/// \brief cpp file for HaptikHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "HaptikHapticsDevice.h"

using namespace H3D;

H3DNodeDatabase HaptikHapticsDevice::database( "HaptikHapticsDevice", 
                                               &(newInstance<HaptikHapticsDevice>),
                                               typeid( HaptikHapticsDevice ),
                                               &H3DThreadedHapticsDevice::database ); 

namespace HaptikHapticsDeviceInternals {
  FIELDDB_ELEMENT( HaptikHapticsDevice, deviceName, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikHapticsDevice, modelName, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikHapticsDevice, manufacturer, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikHapticsDevice, selectedDevice, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikHapticsDevice, set_selectedDevice, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HaptikHapticsDevice, preferredDeviceType, INPUT_OUTPUT );
}


/// Constructor.
HaptikHapticsDevice::HaptikHapticsDevice( 
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
               Inst< SFBool          > _initialized            ,
               Inst< MFString        > _deviceName             ,
               Inst< MFString        > _modelName              ,     
               Inst< MFString        > _manufacturer           ,
               Inst< SelectDevice    > _selectedDevice         ,
               Inst< SFInt32         > _set_selectedDevice     ,
               Inst< SFString        > _preferredDeviceType ) :
  H3DThreadedHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
                            _trackerOrientation, _positionCalibration, 
                            _orientationCalibration, _proxyPosition,
                            _weightedProxyPosition, _proxyWeighting, _mainButton,
                            _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
                            _stylus, _initialized ),
  deviceName( _deviceName ),
  modelName( _modelName ),
  manufacturer( _manufacturer ),
  selectedDevice( _selectedDevice ),
  set_selectedDevice( _set_selectedDevice ),
  preferredDeviceType( _preferredDeviceType )
#ifdef HAVE_HAPTIK
  , haptik_device( NULL )
#endif
  {

  type_name = "HaptikHapticsDevice";  
  database.initFields( this );

  set_selectedDevice->setValue( -1 );
  preferredDeviceType->setValue( "DEFAULT_DEVICE" );

#ifdef HAVE_HAPTIK
  for(UINT32 i = 0 ; i<haptik.numberOfDevices ; i++) {
    deviceName->push_back( haptik.device[i].name, id );
    modelName->push_back( haptik.device[i].model, id );
    manufacturer->push_back( haptik.device[i].manufacturer, id );
  }
#else
  Console(4) << "Cannot use HaptikHapticsDevice since H3D API is compiled"
             << "without support for it. Define HAVE_HAPTIK in H3DApi.h to "
             << " support it." << endl;
#endif
  set_selectedDevice->routeNoEvent( selectedDevice, id );
  preferredDeviceType->route( selectedDevice, id );
}


void HaptikHapticsDevice::initDevice() {
#ifdef HAVE_HAPTIK
  if( !initialized->getValue() ) {
    selectedDevice->upToDate();
    if (haptik.numberOfDevices == 0) {
      cerr << "No devices found in HaptikHapticsDevice node. " 
           <<" Check Haptik configuration file!" << endl;
      return;
    }

    if (haptik_device == NULL) {
      cerr << "Warning: HaptikHapticsDevice could not be initialized. "
           << "Make sure that the selected device is connected." << endl;
      return;
    }
    haptik_device->Init();
    haptik_device->Start();
  }
#endif
  H3DThreadedHapticsDevice::initDevice();
}

void HaptikHapticsDevice::disableDevice() {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    haptik_device->Stop();
  }
#endif
  H3DThreadedHapticsDevice::disableDevice();
}


/// Get the position of the haptics device. Only to be called in the 
/// haptics loop.
Vec3f HaptikHapticsDevice::getPosition() {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikData data;
    haptik_device->Read( data );
    return Vec3f( data.position.x, data.position.y, data.position.z );
  }
#endif
  return Vec3f( 0, 0, 0 );
}

/// Get the velocity of the haptics device. Only to be called in the 
/// haptics loop.
Vec3f HaptikHapticsDevice::getVelocity() {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikData data;
    haptik_device->Read( data );
    return Vec3f( data.velocity.x, data.velocity.y, data.velocity.z );
  } 
#endif
  return Vec3f( 0, 0, 0 );
}

bool HaptikHapticsDevice::getButtonStatus() {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikData data;
    haptik_device->Read( data );
    return (bool)data.Button( 0 );
  }
#endif
  return false;
}

/// Get the orientation of the haptics device. Only to be called in the 
/// haptics loop.
Rotation HaptikHapticsDevice::getOrientation() {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikData data;
    haptik_device->Read( data );
    RSLib::Math::Vector4 &r0 = data.matrix.Row( 0 );
    RSLib::Math::Vector4 &r1 = data.matrix.Row( 1 );
    RSLib::Math::Vector4 &r2 = data.matrix.Row( 2 );
    Matrix3f m( r0.x, r1.x, r2.x,
                r0.y, r1.y, r2.y,
                r0.z, r1.z, r2.z );
    return Rotation( m );
  }
#endif
  return Rotation( 1, 0, 0, 0 );
}

/// Send the force to render on the haptics device. Only to be called in the 
/// haptics loop.
void HaptikHapticsDevice::sendForce( const Vec3f &f ) {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikData data;
    data.forceFeedback.x = f.x;
    data.forceFeedback.y = f.y;
    data.forceFeedback.z = f.z;
    data.torqueFeedback.x = last_torque.x;
    data.torqueFeedback.y = last_torque.y;
    data.torqueFeedback.z = last_torque.z;
    last_force = (Vec3f)f;
    haptik_device->Write( data );
  }
#endif
}

/// Send the torque to render on the haptics device. Only to be called in the 
/// haptics loop.
void HaptikHapticsDevice::sendTorque( const Vec3f &f ) {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikData data;
    haptik_device->Read( data );
    data.forceFeedback.x = last_force.x;
    data.forceFeedback.y = last_force.y;
    data.forceFeedback.z = last_force.z;
    data.torqueFeedback.x = f.x;
    data.torqueFeedback.y = f.y;
    data.torqueFeedback.z = f.z;
    last_torque = (Vec3f)f;
    haptik_device->Write( data );
  }
#endif
}

void HaptikHapticsDevice::SelectDevice::update() {
#ifdef HAVE_HAPTIK
  H3DInt32 index = static_cast< SFInt32 * >( routes_in[0] )->getValue();
  const string &preferred_device = 
    static_cast< SFString * >( routes_in[1] )->getValue();
  HaptikHapticsDevice *hd = static_cast< HaptikHapticsDevice * >( getOwner() );
  if( index >= 0 ) {
    hd->changeHaptikDevice( index );
  } else {
    if( preferred_device == "DEFAULT_DEVICE" ) {
      hd->changeHaptikDevice( HAPTIK_DEFAULT_DEVICE );
    } else if( preferred_device == "MOUSE_SPECTRE" ) {
      hd->changeHaptikDevice( HAPTIK_MOUSE_SPECTRE );
    } else if( preferred_device == "BETAMAX_PLAYER" ) {
      hd->changeHaptikDevice( HAPTIK_BETAMAX_PLAYER );
    } else if( preferred_device == "BETAMAX_RECORDER" ) {
      hd->changeHaptikDevice( HAPTIK_BETAMAX_RECORDER );
    } else if( preferred_device == "PHANTOM_DESKTOP" ) {
      hd->changeHaptikDevice( HAPTIK_PHANTOM_DESKTOP );
    } else if( preferred_device == "PHANTOM_PREMIUM" ) {
      hd->changeHaptikDevice( HAPTIK_PHANTOM_PREMIUM );
    } else if( preferred_device == "PHANTOM_OMNI" ) {
      hd->changeHaptikDevice( HAPTIK_PHANTOM_OMNI );
    } else if( preferred_device == "DELTA" ) {
      hd->changeHaptikDevice( HAPTIK_DELTA );
    } else if( preferred_device == "OMEGA" ) {
      hd->changeHaptikDevice( HAPTIK_OMEGA );
    } else if( preferred_device == "FREEDOM6S" ) {
      hd->changeHaptikDevice( HAPTIK_FREEDOM6S );
    } else if( preferred_device == "CUBIC" ) {
      hd->changeHaptikDevice( HAPTIK_CUBIC );
    } else {
      cerr << "Warning: Invalid value \"" << preferred_device 
           << " specified preferredDeviceType field in HaptikHapticsDevice." 
           << "Using \"DEFAULT_DEVICE\"." <<  endl; 
      hd->changeHaptikDevice( HAPTIK_DEFAULT_DEVICE );
    }
  }

  // find out which index the selected device has.
  if( hd->haptik_device ) {
    RSLib::HaptikDeviceInfo info;
    hd->haptik_device->GetInfo( info );
    int dev_index = -1;
    for(UINT32 i = 0 ; i< hd->haptik.numberOfDevices ; i++) {
      if( hd->haptik.device[i].id == info.id ) {
        dev_index = i;
        break;
      }
    }
    value = dev_index;
  } else {
    value = -1;
  }
#else
  value = -1;
#endif
}

void HaptikHapticsDevice::changeHaptikDevice( UINT32 device_id ) {
#ifdef HAVE_HAPTIK
  if( haptik_device ) {
    RSLib::HaptikDeviceInfo info;
    haptik_device->GetInfo( info );
    int index = -1;
      
    for(UINT32 i = 0 ; i<haptik.numberOfDevices ; i++) {
      if( haptik.device[i].id == info.id ) {
        index = i;
        break;
      }
    }
    assert( index != -1 );
    if( device_id != index && device_id != info.id ) {
      if( initialized->getValue() ) {
        disableDevice();
        haptik_device = 
          (RSLib::IHaptikDeviceInterface) 
          haptik.GetDeviceInterface( device_id );
        initDevice();
      } else {
        haptik_device->Release();
        haptik_device = 
          (RSLib::IHaptikDeviceInterface) 
          haptik.GetDeviceInterface( device_id );
      }
    } 

  } else {
    haptik_device = 
      (RSLib::IHaptikDeviceInterface) haptik.GetDeviceInterface( device_id );
  }
#endif
}
