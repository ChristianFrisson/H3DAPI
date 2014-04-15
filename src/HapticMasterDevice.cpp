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
/// \file H3DAPI/src/HapticMasterDevice.cpp
/// \brief cpp file for HapticMasterDevice
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/HapticMasterDevice.h>
#include <HAPI/HapticMasterDevice.h>

using namespace H3D;

H3DNodeDatabase HapticMasterDevice::database( "HapticMasterDevice", 
                                          &(newInstance<HapticMasterDevice>),
                                          typeid( HapticMasterDevice ),
                                          &H3DHapticsDevice::database ); 
namespace HapticMasterDeviceInternals {
  FIELDDB_ELEMENT( HapticMasterDevice, deviceName, INITIALIZE_ONLY );
}

/// Constructor.
HapticMasterDevice::HapticMasterDevice( 
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
  deviceName( _deviceName ) { 

  type_name = "HapticMasterDevice";  
  database.initFields( this );
  hapi_device.reset(0);

  deviceName->setValue( "vmd" );
}

void HapticMasterDevice::initialize() {
  H3DHapticsDevice::initialize();
#ifdef HAVE_HAPTIC_MASTER_API
  hapi_device.reset( new HAPI::HapticMasterDevice( deviceName->getValue()) );
#else
  Console(4) << "Cannot use HapticMasterDevice. HAPI compiled without"
             << " HapticMaster support. Recompile HAPI with "
             << "HAVE_HAPTIC_MASTER_API defined"
             << " in order to use it." << endl;
#endif
}

