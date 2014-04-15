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
/// \file ForceDimensionDevice.cpp
/// \brief cpp file for ForceDimensionDevice
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/ForceDimensionDevice.h>
#ifdef HAVE_DHDAPI
#include <HAPI/ForceDimensionHapticsDevice.h>
#endif

using namespace H3D;

H3DNodeDatabase ForceDimensionDevice::database( "ForceDimensionDevice", 
                                            &(newInstance<ForceDimensionDevice>),
                                            typeid( ForceDimensionDevice ),
                                            &H3DHapticsDevice::database ); 
namespace ForceDimensionDeviceInternals {
  FIELDDB_ELEMENT( ForceDimensionDevice, useGravityCompensation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ForceDimensionDevice, reset, INPUT_ONLY );
  FIELDDB_ELEMENT( ForceDimensionDevice, waitForReset, INPUT_ONLY );
  FIELDDB_ELEMENT( ForceDimensionDevice, endEffectorMass, INPUT_ONLY );
  FIELDDB_ELEMENT( ForceDimensionDevice, useBrakes, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ForceDimensionDevice, deviceType, OUTPUT_ONLY );
}

/// Constructor.
ForceDimensionDevice::ForceDimensionDevice( 
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
               Inst< GravityComp     > _useGravityCompensation,
               Inst< Reset           > _reset,
               Inst< WaitReset       > _waitForReset,
               Inst< EffectorMass    > _endEffectorMass,
               Inst< Brakes          > _useBrakes,
               Inst< SFInt32         > _deviceType ) :
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
              _trackerOrientation, _positionCalibration, 
              _orientationCalibration, _proxyPosition,
              _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
              _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
              _desiredHapticsRate, _stylus,_hapticsRenderer, _proxyPositions,
              _followViewpoint ),
  useGravityCompensation( _useGravityCompensation ),
  reset( _reset ),
  waitForReset( _waitForReset ),
  endEffectorMass( _endEffectorMass ),
  useBrakes( _useBrakes ),
  deviceType( _deviceType ) {

  type_name = "ForceDimensionDevice";  
  database.initFields( this );

#ifdef HAVE_DHDAPI
  hapi_device.reset( new HAPI::ForceDimensionHapticsDevice );
#else
  Console(4) << "Cannot use ForceDimensionDevice. HAPI compiled without"
       << " DHDAPI support. Recompile HAPI with HAVE_DHDAPI defined"
       << " in order to use it." << endl;
#endif

  useGravityCompensation->setValue( true );
  useBrakes->setValue( false );
  deviceType->setValue( -1, id );
}


void ForceDimensionDevice::Reset::onNewValue( const bool &v ) {
#ifdef HAVE_DHDAPI
  ForceDimensionDevice *fd = 
    static_cast< ForceDimensionDevice * >( getOwner() );
  HAPI::ForceDimensionHapticsDevice * dhd = 
    static_cast< HAPI::ForceDimensionHapticsDevice * >( fd->hapi_device.get() );
  if( v ) dhd->reset();
#endif
}

void ForceDimensionDevice::WaitReset::onNewValue( const bool &v ) {
#ifdef HAVE_DHDAPI
  ForceDimensionDevice *fd = 
    static_cast< ForceDimensionDevice * >( getOwner() );
  HAPI::ForceDimensionHapticsDevice * dhd = 
    static_cast< HAPI::ForceDimensionHapticsDevice * >( fd->hapi_device.get() );
  if( v ) dhd->waitForReset();
#endif
}

void ForceDimensionDevice::GravityComp::onValueChange( const bool &v ) {
#ifdef HAVE_DHDAPI
  ForceDimensionDevice *fd = 
    static_cast< ForceDimensionDevice * >( getOwner() );
  HAPI::ForceDimensionHapticsDevice * dhd = 
    static_cast< HAPI::ForceDimensionHapticsDevice * >( fd->hapi_device.get() );
  dhd->useGravityCompensation( v );
#endif
}

void ForceDimensionDevice::EffectorMass::onValueChange( const H3DFloat &v ) {
#ifdef HAVE_DHDAPI
  ForceDimensionDevice *fd = 
    static_cast< ForceDimensionDevice * >( getOwner() );
  HAPI::ForceDimensionHapticsDevice * dhd = 
    static_cast< HAPI::ForceDimensionHapticsDevice * >( fd->hapi_device.get() );
  dhd->setEffectorMass( v );
#endif
}

void ForceDimensionDevice::Brakes::onValueChange( const bool &v ) {
#ifdef HAVE_DHDAPI
  ForceDimensionDevice *fd = 
    static_cast< ForceDimensionDevice * >( getOwner() );
  HAPI::ForceDimensionHapticsDevice * dhd = 
    static_cast< HAPI::ForceDimensionHapticsDevice * >( fd->hapi_device.get() );
  dhd->useBrakes( v );
#endif
}


H3DHapticsDevice::ErrorCode ForceDimensionDevice::initDevice() {
  HAPI::HAPIHapticsDevice::ErrorCode e = H3DHapticsDevice::initDevice();
#ifdef HAVE_DHDAPI
  HAPI::ForceDimensionHapticsDevice *dhd = 
    static_cast< HAPI::ForceDimensionHapticsDevice * >( hapi_device.get() );
  if( dhd )
    deviceType->setValue( dhd->getDeviceType(), id ); 

#endif
  return e;

}

H3DHapticsDevice::ErrorCode ForceDimensionDevice::releaseDevice() {
  HAPI::HAPIHapticsDevice::ErrorCode e = H3DHapticsDevice::releaseDevice();
  deviceType->setValue( -1, id );
  return e;
}
