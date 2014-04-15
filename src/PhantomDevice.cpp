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
/// \file PhantomDevice.cpp
/// \brief cpp file for PhantomDevice
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/PhantomDevice.h>
#include <HAPI/PhantomHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase PhantomDevice::database( "PhantomDevice", 
                                          &(newInstance<PhantomDevice>),
                                          typeid( PhantomDevice ),
                                          &H3DHapticsDevice::database ); 
namespace PhantomDeviceInternals {
  FIELDDB_ELEMENT( PhantomDevice, deviceName, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, HDAPIVersion, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, deviceModelType, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, deviceDriverVersion, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, deviceFirmwareVersion, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, deviceVendor, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, deviceSerialNumber, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, maxWorkspaceDimensions, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, usableWorkspaceDimensions, OUTPUT_ONLY );  
  FIELDDB_ELEMENT( PhantomDevice, tabletopOffset, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, maxForce, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, maxContinuousForce, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, gimbalAngles, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, jointAngles, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, needsCalibration, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, calibrate, INPUT_ONLY );
  FIELDDB_ELEMENT( PhantomDevice, motorTemperatures, OUTPUT_ONLY );
}

unsigned int PhantomDevice::nr_initialized_devices = 0;
bool PhantomDevice::started_scheduler = false;
unsigned int PhantomDevice::render_shapes_called = 0;

/// Constructor.
PhantomDevice::PhantomDevice( 
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
               Inst< SFString        > _deviceName ) :
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
              _trackerOrientation, _positionCalibration, 
              _orientationCalibration, _proxyPosition,
              _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
              _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
              _desiredHapticsRate, _stylus ),
    deviceName( _deviceName ),
  HDAPIVersion( new SFString ),
  deviceModelType( new SFString ),
  deviceDriverVersion( new SFString ),
  deviceFirmwareVersion( new SFDouble ),
  deviceVendor(  new SFString ),
  deviceSerialNumber( new SFString ),
  maxWorkspaceDimensions( new MFVec3f ),
  usableWorkspaceDimensions( new MFVec3f ),
  tabletopOffset( new SFFloat ),
  maxForce( new SFFloat ),
  maxContinuousForce( new SFFloat ),
  gimbalAngles( new SFVec3f ),
  jointAngles( new SFVec3f ),
  needsCalibration( new SFBool ),
  motorTemperatures( new MFDouble ),
  calibrate( new Calibrate ) { 

  type_name = "PhantomDevice";  
  database.initFields( this );

  hapi_device.reset(0);

  maxForce->setValue( 0, id );
  maxContinuousForce->setValue( 0, id );
  tabletopOffset->setValue( 0, id );
  deviceFirmwareVersion->setValue( 0, id );
  maxWorkspaceDimensions->resize( 2, Vec3f(0,0,0), id );
  usableWorkspaceDimensions->resize( 2, Vec3f(0,0,0), id );
  needsCalibration->setValue( false, id );
  motorTemperatures->setValue( vector< H3DDouble >( 6, 0 ), id );

  desiredHapticsRate->setValue( 1000, id );
}

void PhantomDevice::initialize() {
  H3DHapticsDevice::initialize();
#ifdef HAVE_OPENHAPTICS
  hapi_device.reset( new HAPI::PhantomHapticsDevice( deviceName->getValue() ) );
#else
  Console(4) << "Cannot use PhantomDevice. HAPI compiled without"
             << " OpenHaptics support. Recompile HAPI with "
             << "HAVE_OPENHAPTICS defined"
             << " in order to use it." << endl;
#endif
}

H3DHapticsDevice::ErrorCode PhantomDevice::initDevice() {
#ifdef HAVE_OPENHAPTICS
  HAPI::PhantomHapticsDevice::setEnableStartScheduler( false );
#endif
  HAPI::HAPIHapticsDevice::ErrorCode e = H3DHapticsDevice::initDevice();
#ifdef HAVE_OPENHAPTICS
   HAPI::PhantomHapticsDevice *pd = 
    dynamic_cast< HAPI::PhantomHapticsDevice * >(hapi_device.get() );
  if( e == HAPI::HAPIHapticsDevice::SUCCESS && pd ) {
    HDAPIVersion->setValue( pd->getHDAPIVersion(), id );
    deviceModelType->setValue( pd->getDeviceModelType(), id );
    deviceDriverVersion->setValue( pd->getDeviceDriverVersion(), id );
    deviceVendor->setValue( pd->getDeviceVendor(), id );
    deviceSerialNumber->setValue( pd->getDeviceSerialNumber(), id );
    deviceFirmwareVersion->setValue( pd->getDeviceFirmwareVersion(), id );
    HAPI::Vec3 max, min;
    pd->getMaxWorkspaceDimensions( min, max );
    maxWorkspaceDimensions->setValue( 0, (Vec3f)min, id ); 
    maxWorkspaceDimensions->setValue( 1, (Vec3f)max, id ); 
    pd->getUsableWorkspaceDimensions( min, max );
    usableWorkspaceDimensions->setValue( 0, (Vec3f)min, id ); 
    usableWorkspaceDimensions->setValue( 1, (Vec3f)max, id ); 
    tabletopOffset->setValue((H3DFloat) pd->getTabletopOffset(), id );
    maxForce->setValue( (H3DFloat)pd->getMaxForce(), id );
    maxContinuousForce->setValue( (H3DFloat)pd->getMaxContinuousForce(), id );
    needsCalibration->setValue( pd->needsCalibration(), id );
    inputDOF->setValue( pd->getInputDOF(), id );
    outputDOF->setValue( pd->getOutputDOF(), id );

    ++nr_initialized_devices;
    render_shapes_called = 0;
    started_scheduler = false;
  }
#endif
  return e;
}

H3DHapticsDevice::ErrorCode PhantomDevice::releaseDevice() {
  H3DHapticsDevice::ErrorCode e = H3DHapticsDevice::releaseDevice();
#ifdef HAVE_OPENHAPTICS
  if( e == HAPI::HAPIHapticsDevice::SUCCESS ) {
    --nr_initialized_devices;
  }
  HAPI::PhantomHapticsDevice::setEnableStartScheduler( true );
#endif
  return e;
}

void PhantomDevice::updateDeviceValues() {
#ifdef HAVE_OPENHAPTICS
  H3DHapticsDevice::updateDeviceValues();
  HAPI::PhantomHapticsDevice *pd = 
    dynamic_cast< HAPI::PhantomHapticsDevice *>(hapi_device.get() );
  if( pd ) {
    bool b = pd->needsCalibration();
    if( needsCalibration->getValue() != b ) {
      needsCalibration->setValue( b, id );
    }
    gimbalAngles->setValue( (Vec3f)pd->getGimbalAngles(), id );
    jointAngles->setValue( (Vec3f) pd->getJointAngles(), id );
    motorTemperatures->setValue( pd->getMotorTemperatures(), id );
  }
#endif
}

void PhantomDevice::Calibrate::onValueChange( const bool &value ) {
#ifdef HAVE_OPENHAPTICS
  if ( value ) {
    PhantomDevice *h3d_device = static_cast< PhantomDevice * >( getOwner() );
    HAPI::PhantomHapticsDevice *hapi_device = static_cast< HAPI::PhantomHapticsDevice * >( h3d_device->hapi_device.get() );
    if( hapi_device ) {
      hapi_device->calibrateDevice();
    }
  }
#endif
}

void PhantomDevice::postInit() {
#ifdef HAVE_OPENHAPTICS
  if( hapi_device.get() && !started_scheduler ) {
    ++render_shapes_called;
    if( nr_initialized_devices == render_shapes_called ) {
      HAPI::PhantomHapticsDevice::startScheduler();
      started_scheduler = true;
    }
  }
#endif
}
