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
/// \file HaptikDevice.cpp
/// \brief cpp file for HaptikDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HaptikDevice.h>
#include <HAPI/HaptikHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase HaptikDevice::database( "HaptikDevice", 
                                               &(newInstance<HaptikDevice>),
                                               typeid( HaptikDevice ),
                                               &H3DHapticsDevice::database ); 

namespace HaptikDeviceInternals {
  FIELDDB_ELEMENT( HaptikDevice, deviceName, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikDevice, modelName, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikDevice, manufacturer, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikDevice, selectedDevice, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HaptikDevice, set_selectedDevice, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HaptikDevice, preferredDeviceType, INPUT_OUTPUT );
}


/// Constructor.
HaptikDevice::HaptikDevice( 
               Inst< SFVec3f         > _devicePosition,
               Inst< SFRotation      > _deviceOrientation,
               Inst< TrackerPosition > _trackerPosition,
               Inst< TrackerOrientation > _trackerOrientation,
               Inst< PosCalibration  > _positionCalibration,
               Inst< OrnCalibration  > _orientationCalibration,
               Inst< SFVec3f         > _proxyPosition,
               Inst< WeightedProxy   > _weightedProxyPosition,     
               Inst< SFFloat         > _proxyWeighting,
               Inst< SFBool          > _mainButton,
               Inst< SFBool          > _secondaryButton,
               Inst< SFInt32         > _buttons,
               Inst< SFVec3f         > _force,
               Inst< SFVec3f         > _torque,
               Inst< SFInt32         > _inputDOF,
               Inst< SFInt32         > _outputDOF,
               Inst< SFInt32         > _hapticsRate,
               Inst< SFInt32         > _desiredHapticsRate,
               Inst< SFNode          > _stylus,
               Inst< MFString        > _deviceName             ,
               Inst< MFString        > _modelName              ,     
               Inst< MFString        > _manufacturer           ,
               Inst< SelectDevice    > _selectedDevice         ,
               Inst< SFInt32         > _set_selectedDevice     ,
               Inst< SFString        > _preferredDeviceType ) :
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
                    _trackerOrientation, _positionCalibration, 
                    _orientationCalibration, _proxyPosition,
                    _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
                    _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
                    _desiredHapticsRate, _stylus ),
  deviceName( _deviceName ),
  modelName( _modelName ),
  manufacturer( _manufacturer ),
  selectedDevice( _selectedDevice ),
  set_selectedDevice( _set_selectedDevice ),
  preferredDeviceType( _preferredDeviceType )
  {

  type_name = "HaptikDevice";  
  database.initFields( this );

  set_selectedDevice->setValue( -1 );
  preferredDeviceType->setValue( "DEFAULT_DEVICE" );

#ifdef HAVE_HAPTIK_LIBRARY
  RSLib::Haptik haptik;
  for(UINT32 i = 0 ; i<haptik.numberOfDevices ; ++i) {
    deviceName->push_back( haptik.device[i].name, id );
    modelName->push_back( haptik.device[i].model, id );
    manufacturer->push_back( haptik.device[i].manufacturer, id );
  }
#else
  Console(4) << "Cannot use HaptikDevice since H3D API is compiled"
             << "without support for it. Define HAVE_HAPTIK_LIBRARY in "
             << " HAPI.h to support it." << endl;
#endif
  set_selectedDevice->routeNoEvent( selectedDevice, id );
  preferredDeviceType->route( selectedDevice, id );
}

void HaptikDevice::initialize() {
  H3DHapticsDevice::initialize();
#ifdef HAVE_HAPTIK_LIBRARY
  hapi_device.reset( new HAPI::HaptikHapticsDevice );
#else
   Console(4) << "Cannot use HaptikDevice since H3D API is compiled"
              << "without support for it. Define HAVE_HAPTIK_LIBRARY in "
              << " HAPI.h to support it." << endl;
#endif
}

void HaptikDevice::SelectDevice::update() {
#ifdef HAVE_HAPTIK_LIBRARY
  H3DInt32 index = static_cast< SFInt32 * >( routes_in[0] )->getValue();
  const string &preferred_device = 
    static_cast< SFString * >( routes_in[1] )->getValue();
  HaptikDevice *hd = static_cast< HaptikDevice * >( getOwner() );
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
           << " specified preferredDeviceType field in HaptikDevice." 
           << "Using \"DEFAULT_DEVICE\"." <<  endl; 
      hd->changeHaptikDevice( HAPTIK_DEFAULT_DEVICE );
    }
  }

  // find out which index the selected device has.
  if( hd->hapi_device.get() ) {
    value = static_cast< HAPI::HaptikHapticsDevice * >( hd->hapi_device.get() )
      ->getHaptikIndex();;
  } else {
    value = -1;
  }
#else
  value = -1;
#endif
}

void HaptikDevice::changeHaptikDevice( unsigned int device_id ) {
#ifdef HAVE_HAPTIK_LIBRARY
  if( hapi_device.get() ) {
    static_cast< HAPI::HaptikHapticsDevice * >( hapi_device.get() )->
      changeHaptikDevice( device_id );
  } else {
    HAPI::HaptikHapticsDevice * temp_device = new HAPI::HaptikHapticsDevice;
    temp_device->changeHaptikDevice( device_id );
    hapi_device.reset( temp_device );
  }
#endif
}
