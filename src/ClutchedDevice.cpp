//////////////////////////////////////////////////////////////////////////////
//    Copyright 2011-2013, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file AnyDevice.cpp
/// \brief cpp file for AnyDevice
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/ClutchedDevice.h> 
#include <HAPI/ClutchedHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase ClutchedDevice::database( "ClutchedDevice", 
                                            &(newInstance<ClutchedDevice>),
                                            typeid( ClutchedDevice ),
                                            &H3DHapticsDevice::database ); 
namespace ClutchedDeviceInternals {
  FIELDDB_ELEMENT( ClutchedDevice, hapticsDevice, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClutchedDevice, enableClutch, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClutchedDevice, resetClutchOrientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClutchedDevice, resetClutchPosition, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClutchedDevice, unclutchedPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ClutchedDevice, unclutchedOrientation, OUTPUT_ONLY );
}

/// Constructor.
ClutchedDevice::ClutchedDevice( 
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
               Inst< SFHapticsDevice > _hapticsDevice,
               Inst< EnableClutch       > _enableClutch,
               Inst< ResetClutchOrientation > _resetClutchOrientation,
               Inst< ResetClutchPosition > _resetClutchPosition,
               Inst< SFVec3f            > _unclutchedPosition,
               Inst< SFRotation         > _unclutchedOrientation ):
  hapticsDevice( _hapticsDevice ),
  enableClutch ( _enableClutch ),
  resetClutchOrientation ( _resetClutchOrientation ),
  resetClutchPosition ( _resetClutchPosition ),
  unclutchedPosition ( _unclutchedPosition ),
  unclutchedOrientation ( _unclutchedOrientation ),
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
              _trackerOrientation, _positionCalibration, 
              _orientationCalibration, _proxyPosition,
              _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
              _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
              _desiredHapticsRate, _stylus ) {

  type_name = "ClutchedDevice";  
  database.initFields( this );

  enableClutch->setValue ( false );
  resetClutchOrientation->setValue ( false );
  resetClutchPosition->setValue ( false );

  hapi_device.reset( new HAPI::ClutchedHapticsDevice );
}

void ClutchedDevice::updateDeviceValues() {
  H3DHapticsDevice::updateDeviceValues ();

  if ( H3DHapticsDevice* hd= hapticsDevice->getValue() ) {
    hd->updateDeviceValues ();
  }

  HAPI::ClutchedHapticsDevice *hapi_clutched_device = static_cast< HAPI::ClutchedHapticsDevice * >( hapi_device.get() );
  HAPI::HAPIHapticsDevice::DeviceValues dv = 
      hapi_clutched_device->getUnclutchedDeviceValues();

  unclutchedPosition->setValue ( Vec3f(dv.position), id );
  unclutchedOrientation->setValue ( Rotation(dv.orientation), id );
}

void ClutchedDevice::initialize () {
  H3DHapticsDevice::initialize ();

  if ( H3DHapticsDevice* hd= hapticsDevice->getValue() ) {
    // Ensure the wrapped device is initialized first, otherwise the device may not function correctly
    if ( !hd->isInitialized() ){
      hd->initialize();      
    }
    hd->stylus->route( stylus );

    HAPI::ClutchedHapticsDevice* clutched_device = static_cast< HAPI::ClutchedHapticsDevice * >( hapi_device.get() );
    if ( HAPI::HAPIHapticsDevice* hapi_hd= hd->getHAPIDevice() ) {
      clutched_device->setActualHapticsDevice ( *hapi_hd );
    }
  }
}

ClutchedDevice::ErrorCode ClutchedDevice::initDevice() {
  // Also call initDevice() on the wrapped device, since some device nodes use
  // this function to do extra initialization (e.g., PhantomDevice)
  ErrorCode e= H3DHapticsDevice::initDevice ();
  if ( H3DHapticsDevice* hd= hapticsDevice->getValue() ) {
    return hd->initDevice();
  }
  return e;
}

ClutchedDevice::ErrorCode ClutchedDevice::releaseDevice() {
  // Also call releaseDevice() on the wrapped device, since some device nodes use
  // this function to do extra clean up (e.g., PhantomDevice)
  ErrorCode e= H3DHapticsDevice::releaseDevice ();
  if ( H3DHapticsDevice* hd= hapticsDevice->getValue() ) {
    return hd->releaseDevice();
  }
  return e;
}

void ClutchedDevice::renderShapes( const HapticShapeVector &shapes, 
                                   unsigned int layer ) {
  // Also call renderShapes() on the wrapped device, since some device nodes use
  // this function to do extra initialization (e.g., PhantomDevice)
  H3DHapticsDevice::renderShapes ( shapes, layer );
  if ( H3DHapticsDevice* hd= hapticsDevice->getValue() ) {
    hd->renderShapes( shapes, layer );
  }
}

void ClutchedDevice::EnableClutch::onValueChange( const bool &value ) {
  ClutchedDevice *h3d_device = static_cast< ClutchedDevice * >( getOwner() );
  HAPI::ClutchedHapticsDevice *hapi_device = static_cast< HAPI::ClutchedHapticsDevice * >( h3d_device->hapi_device.get() );
  if( hapi_device ) {
    hapi_device->enableClutch( value );
  }
}

void ClutchedDevice::ResetClutchOrientation::onNewValue( const bool &value ) {
  if ( value ) {
    ClutchedDevice *h3d_device = static_cast< ClutchedDevice * >( getOwner() );
    HAPI::ClutchedHapticsDevice *hapi_device = static_cast< HAPI::ClutchedHapticsDevice * >( h3d_device->hapi_device.get() );
    if( hapi_device ) {
      hapi_device->resetClutch( true, false );
    }
  }
}

void ClutchedDevice::ResetClutchPosition::onNewValue( const bool &value ) {
  if ( value ) {
    ClutchedDevice *h3d_device = static_cast< ClutchedDevice * >( getOwner() );
    HAPI::ClutchedHapticsDevice *hapi_device = static_cast< HAPI::ClutchedHapticsDevice * >( h3d_device->hapi_device.get() );
    if( hapi_device ) {
      hapi_device->resetClutch( false, true );
    }
  }
}