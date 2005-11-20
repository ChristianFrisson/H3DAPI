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
/// \file H3DFakeHapticsDevice.cpp
/// \brief cpp file for H3DFakeHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "H3DFakeHapticsDevice.h"

using namespace H3D;

H3DNodeDatabase H3DFakeHapticsDevice::database
( "H3DFakeHapticsDevice", 
  &(newInstance< H3DFakeHapticsDevice >),
  typeid( H3DFakeHapticsDevice ),
  &H3DThreadedHapticsDevice::database ); 

/// Constructor.
H3DFakeHapticsDevice::H3DFakeHapticsDevice( 
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
  H3DThreadedHapticsDevice( _devicePosition, _deviceOrientation, 
                            _trackerPosition, _trackerOrientation,
                            _positionCalibration, _orientationCalibration,
                            _proxyPosition, _weightedProxyPosition,
                            _proxyWeighting, _mainButton, _force,
                            _torque, _inputDOF, _outputDOF, _hapticsRate,
                            _stylus, _initialized ),
  set_devicePosition( _set_devicePosition ),
  set_deviceOrientation( _set_deviceOrientation ),
  set_mainButton( _set_mainButton ) {

  type_name = "H3DFakeHapticsDevice";  
  database.initFields( this );

  set_devicePosition->setOwner( this );
  set_devicePosition->setName( "set_devicePosition" );
  set_deviceOrientation->setOwner( this );
  set_deviceOrientation->setName( "set_deviceOrientation" );
  set_mainButton->setOwner( this );
  set_mainButton->setName( "set_mainButton" );

  set_devicePosition->route( devicePosition, id );
  set_deviceOrientation->route( deviceOrientation, id );
  set_mainButton->route( mainButton, id );


  inputDOF->setValue( 6, id );
  outputDOF->setValue( 0, id );
}


Vec3f H3DFakeHapticsDevice::getPosition() {
  // devicePosition is thread safe so we can use getValue()
  return devicePosition->getValue();
}

Vec3f H3DFakeHapticsDevice::getVelocity() {
  return Vec3f();
}

Rotation H3DFakeHapticsDevice::getOrientation() {
  // deviceOrientation is thread safe so we can use getValue()
  return deviceOrientation->getValue();
}

bool H3DFakeHapticsDevice::getButtonStatus() {
  // mainButton is thread safe so we can use getValue()
  return mainButton->getValue();
}











