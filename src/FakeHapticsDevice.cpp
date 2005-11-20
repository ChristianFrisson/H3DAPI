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
/// \file FakeHapticsDevice.cpp
/// \brief cpp file for FakeHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "FakeHapticsDevice.h"

using namespace H3D;

H3DNodeDatabase FakeHapticsDevice::database
( "FakeHapticsDevice", 
  &(newInstance< FakeHapticsDevice >),
  typeid( FakeHapticsDevice ),
  &H3DFakeHapticsDevice::database ); 

namespace FakeHapticsDeviceInternals {
  FIELDDB_ELEMENT( FakeHapticsDevice, set_devicePosition, INPUT_ONLY );
  FIELDDB_ELEMENT( FakeHapticsDevice, set_deviceOrientation, INPUT_ONLY );
  FIELDDB_ELEMENT( FakeHapticsDevice, set_mainButton, INPUT_ONLY );
}

/// Constructor.
FakeHapticsDevice::FakeHapticsDevice( 
         Inst< ThreadSafeSField< SFVec3f > > _devicePosition,
         Inst< ThreadSafeSField< SFRotation > > _deviceOrientation      ,
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
         Inst< SFVec3f         > _set_devicePosition     ,
         Inst< SFRotation      > _set_deviceOrientation  ,
         Inst< SFBool          > _set_mainButton ):
  H3DFakeHapticsDevice( _devicePosition, _deviceOrientation, 
                        _trackerPosition, _trackerOrientation,
                        _positionCalibration, _orientationCalibration,
                        _proxyPosition, _weightedProxyPosition,
                        _proxyWeighting, _mainButton, _force,
                        _torque, _inputDOF, _outputDOF, _hapticsRate,
                        _stylus, _initialized, _set_devicePosition,
                        _set_deviceOrientation, _set_mainButton) {

  type_name = "FakeHapticsDevice";  
  database.initFields( this );
}










