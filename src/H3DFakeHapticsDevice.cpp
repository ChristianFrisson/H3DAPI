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
/// \file H3DFakeHapticsDevice.cpp
/// \brief cpp file for H3DFakeHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "H3D/H3DFakeHapticsDevice.h"

using namespace H3D;

H3DNodeDatabase H3DFakeHapticsDevice::database
( "H3DFakeHapticsDevice", 
  NULL,
  typeid( H3DFakeHapticsDevice ),
  &H3DHapticsDevice::database ); 

/// Constructor.
H3DFakeHapticsDevice::H3DFakeHapticsDevice( 
          Inst< SFVec3f         > _devicePosition         ,
          Inst< SFRotation      > _deviceOrientation      ,
          Inst< TrackerPosition > _trackerPosition        ,
          Inst< TrackerOrientation > _trackerOrientation  ,
          Inst< PosCalibration  > _positionCalibration    ,
          Inst< OrnCalibration  > _orientationCalibration ,
          Inst< SFVec3f         > _proxyPosition          ,
          Inst< WeightedProxy   > _weightedProxyPosition  ,
          Inst< SFFloat         > _proxyWeighting         ,
          Inst< SFBool          > _mainButton            ,
          Inst< SFBool          > _secondaryButton       ,
          Inst< SFInt32         > _buttons                ,
          Inst< SFVec3f         > _force                  ,
          Inst< SFVec3f         > _torque                 ,
          Inst< SFInt32         > _inputDOF               ,
          Inst< SFInt32         > _outputDOF              ,
          Inst< SFInt32         > _hapticsRate            ,
          Inst< SFInt32         > _desiredHapticsRate     ,
          Inst< SFNode          > _stylus                 ,
          Inst< SFHapticsRendererNode > _hapticsRenderer  ,
          Inst< MFVec3f         > _proxyPositions         ,
          Inst< SFBool          > _followViewpoint        ,
          Inst< ThreadSafeSField< SFVec3f > > _set_devicePosition,
          Inst< ThreadSafeSField< SFRotation > > _set_deviceOrientation,
          Inst< ThreadSafeSField< SFBool > > _set_mainButton          ):
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
        _trackerOrientation, _positionCalibration, 
        _orientationCalibration, _proxyPosition,
        _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
        _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
        _desiredHapticsRate,
        _stylus, _hapticsRenderer, _proxyPositions, _followViewpoint ),
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
  // set_mainButton have to be set to false because the value of this
  // field is used to set the buttons variable.
  set_mainButton->setValue( false );

  H3DFakeHapticsDevice::FakeHapticsDevice *fd = 
    new H3DFakeHapticsDevice::FakeHapticsDevice;

  fd->owner = this;
  
  hapi_device.reset( fd );

  inputDOF->setValue( 6, id );
  outputDOF->setValue( 0, id );
}

void H3DFakeHapticsDevice::FakeHapticsDevice::updateDeviceValues( 
                DeviceValues &dv, HAPI::HAPITime dt ) {
  HAPIHapticsDevice::updateDeviceValues( dv, dt );
  dv.position = owner->set_devicePosition->getValue();
  dv.orientation = owner->set_deviceOrientation->getValue();
  // Button status is an int, if we want to make sure that the mainButton
  // bit is set then the integer have to be 1 and nothing else.
  if( owner->set_mainButton->getValue() )
    dv.button_status = 1;
  else
    dv.button_status = 0;
}














