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
/// \file HapticsOptions.cpp
/// \brief CPP file for HapticsOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HapticsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HapticsOptions::database( "HapticsOptions", 
                                      &(newInstance<HapticsOptions>),
                                      typeid( HapticsOptions ),
                                      &H3DOptionNode::database );

namespace HapticsOptionsInternals {
  FIELDDB_ELEMENT( HapticsOptions, touchableFace, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticsOptions, maxDistance, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticsOptions, lookAheadFactor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticsOptions, useBoundTree, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticsOptions, interpolateForceEffects, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticsOptions, dynamicMode, INPUT_OUTPUT );
}

HapticsOptions::HapticsOptions( 
                           Inst< SFNode   >  _metadata,
                           Inst< SFString >  _touchableFace,
                           Inst< SFFloat  > _maxDistance,
                           Inst< SFFloat  > _lookAheadFactor,
                           Inst< SFBool   > _useBoundTree,
                           Inst< SFBool   > _interpolateForceEffects,
                           Inst< SFString > _dynamicMode ) :
  H3DOptionNode( _metadata ),
  touchableFace( _touchableFace ),
  maxDistance( _maxDistance ),
  lookAheadFactor( _lookAheadFactor ),
  useBoundTree ( _useBoundTree ),
  interpolateForceEffects( _interpolateForceEffects ),
  dynamicMode( _dynamicMode ) {

  type_name = "HapticsOptions";

  database.initFields( this );

  touchableFace->route( updateOption );
  maxDistance->route( updateOption );
  lookAheadFactor->route( updateOption );
  useBoundTree->route( updateOption );
  interpolateForceEffects->route( updateOption );
  dynamicMode->route( updateOption );

  touchableFace->addValidValue( "AS_GRAPHICS" );
  touchableFace->addValidValue( "FRONT" );
  touchableFace->addValidValue( "BACK" );
  touchableFace->addValidValue( "FRONT_AND_BACK" );
  touchableFace->setValue( "AS_GRAPHICS" );
  maxDistance->setValue( 0.015f );
  lookAheadFactor->setValue( 3 );
  useBoundTree->setValue( true );
  interpolateForceEffects->setValue( true );
  dynamicMode->addValidValue( "TRANSFORM_CHANGED" );
  dynamicMode->addValidValue( "ALWAYS" );
  dynamicMode->addValidValue( "NEVER" );
  dynamicMode->setValue( "TRANSFORM_CHANGED" );
}


