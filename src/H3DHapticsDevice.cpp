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
/// \file H3DHapticsDevice.cpp
/// \brief cpp file for H3DHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "H3DHapticsDevice.h"

using namespace H3D;

H3DNodeDatabase H3DHapticsDevice::database( "H3DHapticsDevice", 
                                         NULL,
                                         typeid( H3DHapticsDevice ) ); 

namespace H3DHapticsDeviceInternals {
  FIELDDB_ELEMENT( H3DHapticsDevice, devicePosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, deviceOrientation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, trackerPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, trackerOrientation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, positionCalibration, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, orientationCalibration, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, proxyPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, weightedProxyPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, proxyWeighting, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, mainButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, force, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, torque, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, inputDOF, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, outputDOF, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, hapticsRate, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, stylus, INPUT_OUTPUT );
}


/// Constructor.
H3DHapticsDevice::H3DHapticsDevice( Inst< SFVec3f         > _devicePosition,
               Inst< SFRotation      > _deviceOrientation      ,
               Inst< TrackerPosition > _trackerPosition        ,
               Inst< TrackerOrientation > _trackerOrientation  ,
               Inst< PosCalibration  > _positionCalibration    ,
               Inst< OrnCalibration  > _orientationCalibration ,
               Inst< SFVec3f         > _proxyPosition          ,
               Inst< WeightedProxy   > _weightedProxyPosition  ,     
               Inst< SFFloat         > _proxyWeighting         ,
               Inst< SFBool          > _mainButton             ,
               Inst< SFVec3f         > _force                  ,
               Inst< SFVec3f         > _torque                 ,
               Inst< SFInt32         > _inputDOF               ,
               Inst< SFInt32         > _outputDOF              ,
               Inst< SFInt32         > _hapticsRate            ,
               Inst< SFNode          > _stylus                 ,
               Inst< SFBool          > _initialized             ):
  devicePosition( _devicePosition ),
  deviceOrientation( _deviceOrientation ),
  trackerPosition( _trackerPosition ),
  trackerOrientation( _trackerOrientation ),
  positionCalibration( _positionCalibration ),
  orientationCalibration( _orientationCalibration ),
  proxyPosition( _proxyPosition ),
  weightedProxyPosition( _weightedProxyPosition ),
  proxyWeighting( _proxyWeighting ),
  mainButton( _mainButton ),
  force( _force ),
  torque( _torque ),
  inputDOF( _inputDOF ),
  outputDOF( _outputDOF ),
  hapticsRate( _hapticsRate ),
  stylus( _stylus ),
  initialized( _initialized ) {

  type_name = "H3DHapticsDevice";  
  database.initFields( this );

  initialized->setValue( false );
  proxyWeighting->setValue( 0.95f );
  mainButton->setValue( false, id );

  positionCalibration->route( trackerPosition, id );
  devicePosition->route( trackerPosition, id );

  orientationCalibration->route( trackerOrientation, id );
  deviceOrientation->route( trackerOrientation, id );
  
  proxyPosition->route( weightedProxyPosition, id );
  trackerPosition->route( weightedProxyPosition, id );
  proxyWeighting->route( weightedProxyPosition, id );
}
