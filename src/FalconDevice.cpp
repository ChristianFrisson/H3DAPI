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
/// \file FalconDevice.cpp
/// \brief cpp file for FalconDevice
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/FalconDevice.h>
#include <HAPI/FalconHapticsDevice.h>
#include <HAPI/NiFalconHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase FalconDevice::database( "FalconDevice", 
                                          &(newInstance<FalconDevice>),
                                          typeid( FalconDevice ),
                                          &H3DHapticsDevice::database ); 
namespace FalconDeviceInternals {
  FIELDDB_ELEMENT( FalconDevice, deviceName, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FalconDevice, deviceIndex, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FalconDevice, deviceModelType, OUTPUT_ONLY );
  FIELDDB_ELEMENT( FalconDevice, maxWorkspaceDimensions, OUTPUT_ONLY );
  FIELDDB_ELEMENT( FalconDevice, preferredDriver, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FalconDevice, usedDriver, INPUT_OUTPUT );
}

/// Constructor.
FalconDevice::FalconDevice( 
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
               Inst< SFHapticsRendererNode > _hapticsRenderer,
               Inst< MFVec3f         > _proxyPositions,
               Inst< SFBool          > _followViewpoint,
               Inst< SFString        > _deviceName,
               Inst< SFInt32         > _deviceIndex ) :
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
              _trackerOrientation, _positionCalibration, 
              _orientationCalibration, _proxyPosition,
              _weightedProxyPosition, _proxyWeighting, _mainButton,
              _secondaryButton, _buttons,_force, _torque, _inputDOF,
              _outputDOF, _hapticsRate, _desiredHapticsRate, _stylus,
              _hapticsRenderer, _proxyPositions, _followViewpoint ),
  deviceName( _deviceName ),
  deviceIndex( _deviceIndex ),
  deviceModelType ( new SFString ),
  maxWorkspaceDimensions( new MFVec3f ),
  preferredDriver( new SFString ),
  usedDriver( new SFString ) { 

  type_name = "FalconDevice";  
  database.initFields( this );
  hapi_device.reset(0);
  maxWorkspaceDimensions->resize( 2, Vec3f(0,0,0), id );
  preferredDriver->addValidValue( "NOVINT" );
  preferredDriver->addValidValue( "NIFALCON" );
  preferredDriver->setValue( "NOVINT" );
  deviceIndex->setValue( 0 );

  usedDriver->setValue( "NONE", id );
}

void FalconDevice::initialize() {
  H3DHapticsDevice::initialize();
  H3DInt32 index     = deviceIndex->getValue();
  const string &name = deviceName->getValue();

  const string &preferred_driver = preferredDriver->getValue();

  if( preferred_driver == "NIFALCON" ) {
#ifdef HAVE_NIFALCONAPI
    hapi_device.reset( new HAPI::NiFalconHapticsDevice( index ) );
  usedDriver->setValue( "NIFALCON" );
#else
#ifdef HAVE_FALCONAPI
  if( name != "" ) {
    hapi_device.reset( new HAPI::FalconHapticsDevice( name ) );
  } else {
    hapi_device.reset( new HAPI::FalconHapticsDevice( index ) );
  }
  usedDriver->setValue( "NOVINT" );
#else
  Console(4) << "Cannot use FalconDevice. HAPI compiled without"
             << " FalconAPI support. Recompile HAPI with "
             << "HAVE_FALCONAPI defined"
             << " in order to use it." << endl;
#endif // HAVE_FALCONAPI
#endif // HAVE_NIFALCONAPI
  } 
  else {
    if( preferred_driver != "NOVINT" ) {
      Console(4) << "Invalid value \"" << preferred_driver 
                 << "\" of preferredDriver field in FalconDevice node." 
                 << "Should be \"NOVINT\" or \"NIFALCON\". Using "
                 << "\"NOVINT\" instead" << endl;
    }
#ifdef HAVE_FALCONAPI
    if( name != "" ) {
      hapi_device.reset( new HAPI::FalconHapticsDevice( name ) );
    } else {
      hapi_device.reset( new HAPI::FalconHapticsDevice( index ) );
    }
    usedDriver->setValue( "NOVINT" );
#else
#ifdef HAVE_NIFALCONAPI
    hapi_device.reset( new HAPI::NiFalconHapticsDevice( index ) );
    usedDriver->setValue( "NIFALCON" );
#else
    Console(4) << "Cannot use FalconDevice. HAPI compiled without"
               << " FalconAPI support. Recompile HAPI with "
               << "HAVE_FALCONAPI defined"
               << " in order to use it." << endl;
#endif // HAVE_NIFALCONAPI
#endif // HAVE_FALCONAPI
  }
}

H3DHapticsDevice::ErrorCode FalconDevice::initDevice() {
  HAPI::HAPIHapticsDevice::ErrorCode e = H3DHapticsDevice::initDevice();
#ifdef HAVE_FALCONAPI
   HAPI::FalconHapticsDevice *pd = 
    dynamic_cast< HAPI::FalconHapticsDevice * >(hapi_device.get() );
  if( e == HAPI::HAPIHapticsDevice::SUCCESS && pd ) {
    deviceModelType->setValue( pd->getDeviceModel(), id );
    HAPI::Vec3 max, min;
    pd->getWorkspaceDimensions( min, max );
    maxWorkspaceDimensions->setValue( 0, (Vec3f)min, id ); 
    maxWorkspaceDimensions->setValue( 1, (Vec3f)max, id ); 
  }
#endif
  return e;
}


