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
/// \file EntactDevice.cpp
/// \brief cpp file for EntactDevice
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/EntactDevice.h>
#include <HAPI/EntactHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase EntactDevice::database( "EntactDevice", 
                                        &(newInstance<EntactDevice>),
                                        typeid( EntactDevice ),
                                        &H3DHapticsDevice::database ); 
namespace EntactDeviceInternals {
  FIELDDB_ELEMENT( EntactDevice, needsCalibration, OUTPUT_ONLY );
  FIELDDB_ELEMENT( EntactDevice, calibrate, INPUT_ONLY );
  FIELDDB_ELEMENT( EntactDevice, serialNumber, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( EntactDevice, ipAddress, INITIALIZE_ONLY );
}

/// Constructor.
EntactDevice::EntactDevice( 
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
         Inst< SFInt32            > _serialNumber,
         Inst< SFBool             > _needsCalibration,
         Inst< Calibrate          > _calibrate,
         Inst< SFString          > _ipAddress ) :
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
              _trackerOrientation, _positionCalibration, 
              _orientationCalibration, _proxyPosition,
              _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
              _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
              _desiredHapticsRate, _stylus,_hapticsRenderer, _proxyPositions,
        _followViewpoint ),
  serialNumber( _serialNumber ),
  needsCalibration( _needsCalibration ),
  calibrate( _calibrate ),
  ipAddress( _ipAddress ) {

  type_name = "EntactDevice";  
  database.initFields( this );

  serialNumber->setValue( -1, id );
  needsCalibration->setValue( false, id );
  ipAddress->setValue( "", id );
}

void EntactDevice::initialize() {
#ifdef HAVE_ENTACTAPI
  hapi_device.reset( new HAPI::EntactHapticsDevice( serialNumber->getValue(),
                                                    ipAddress->getValue() ) );
#else
  Console(4) << "Cannot use EntactDevice. HAPI compiled without"
             << " Entact support. Recompile HAPI with HAVE_ENTACTAPI defined"
             << " in order to use it." << endl;
#endif
}

void EntactDevice::updateDeviceValues() {
#ifdef HAVE_ENTACTAPI
  H3DHapticsDevice::updateDeviceValues();
  HAPI::EntactHapticsDevice *ed = 
    dynamic_cast< HAPI::EntactHapticsDevice *>(hapi_device.get() );
  if( ed ) {
    bool b = ed->needsCalibration();
    if( needsCalibration->getValue() != b ) {
      needsCalibration->setValue( b, id );
    }
  }
#endif
}

void EntactDevice::Calibrate::onValueChange( const bool &value ) {
#ifdef HAVE_ENTACTAPI
  if ( value ) {
    EntactDevice *h3d_device = static_cast< EntactDevice * >( getOwner() );
    HAPI::EntactHapticsDevice *hapi_device = static_cast< HAPI::EntactHapticsDevice * >( h3d_device->hapi_device.get() );
    if( hapi_device ) {
      hapi_device->calibrateDevice();
    }
  }
#endif
}
