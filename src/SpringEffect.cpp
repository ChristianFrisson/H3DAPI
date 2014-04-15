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
/// \file SpringEffect.cpp
/// \brief cpp file for SpringEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/SpringEffect.h>
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SpringEffect::database( 
                                       "SpringEffect", 
                                       &(newInstance<SpringEffect>), 
                                       typeid( SpringEffect ),
                                       &H3DForceEffect::database );

namespace SpringEffectInternals {
  FIELDDB_ELEMENT( SpringEffect, position, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, force, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, springConstant, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, startDistance, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, escapeDistance, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, active, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SpringEffect, damping, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, positionInterpolation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpringEffect, interpolatedPosition, OUTPUT_ONLY );
}

/// Constructor
SpringEffect::SpringEffect( Inst< SFVec3f     > _position,
                            Inst< MFVec3f     > _force,
                            Inst< SFFloat     > _springConstant,
                            Inst< SFFloat     > _startDistance,
                            Inst< SFFloat     > _escapeDistance,
                            Inst< SFBool      > _active, 
                            Inst< MFInt32     > _deviceIndex,
                            Inst< SFNode      >  _metadata,
                            Inst< SFFloat     > _damping,
                            Inst< SFFloat     > _positionInterpolation,
                            Inst< SFVec3f     > _interpolatedPosition ) :
  H3DForceEffect( _metadata, _deviceIndex ),
  position( _position ),
  force( _force ),
  springConstant( _springConstant ),
  startDistance( _startDistance ),
  escapeDistance( _escapeDistance ),
  active( _active ),
  damping( _damping ),
  positionInterpolation( _positionInterpolation ),
  interpolatedPosition ( _interpolatedPosition ) {
  
  type_name = "SpringEffect";

  database.initFields( this );
  
  position->setValue( Vec3f( 0,0,0 ) );
  springConstant->setValue( 100 );
  startDistance->setValue( 0.01f );
  escapeDistance->setValue( 0.01f );
  active->setValue( false, id );
  damping->setValue( 0 );
  positionInterpolation->setValue( 1 );
  
}

void SpringEffect::traverseSG( TraverseInfo &ti ) {
  if( !ti.getHapticsDevices().empty()) {
    vector< H3DInt32 > device_index = deviceIndex->getValue();
    if( device_index.empty() ) {
      for( unsigned int i = 0; i < ti.getHapticsDevices().size(); ++i )
        device_index.push_back( i );
    }
    bool any_active = false;
    for( unsigned int i = 0; i < device_index.size(); ++i ) {
      int index = device_index[i];
      if( index >= 0 && ti.hapticsEnabled( index ) ) {
        H3DHapticsDevice *hd = ti.getHapticsDevice( index );
        // the tracker position in local coordinates.
        const Vec3f &pos = 
          ti.getAccInverseMatrix() * 
          hd->trackerPosition->getValue();
        const Vec3f &spring_pos = position->getValue();
        H3DFloat distance = ( pos - spring_pos ).length();
        if( index >= (int)haptic_spring.size() )
          haptic_spring.resize( index + 1, NULL );
        if( index >= (int)force->size() )
          force->resize( index + 1, Vec3f( 0, 0, 0 ), id );
        if( haptic_spring[index] ) {
          if( distance >= escapeDistance->getValue() ) {
            haptic_spring.set( index, NULL );
            force->setValue( index, Vec3f( 0, 0, 0 ), id );
          } else {
            haptic_spring[index]->setPosition( ti.getAccForwardMatrix() * spring_pos );
            haptic_spring[index]->setSpringConstant( springConstant->getValue() );
            haptic_spring[index]->setDamping( damping->getValue() );
            haptic_spring[index]->setPositionInterpolation( positionInterpolation->getValue() );
            interpolatedPosition->setValue ( ti.getAccInverseMatrix() * Vec3f(haptic_spring[index]->getInterpolatedPosition ()), id );
            ti.addForceEffect( index, haptic_spring[index] );
            any_active = true;
            force->setValue( index, (Vec3f) haptic_spring[index]->getLatestForce(), id );
          }
        } else {
          if( distance <= startDistance->getValue() ) {
            if( !haptic_spring[index] )
              haptic_spring.set( index, createHAPISpring() );
            haptic_spring[index]->setPosition( ti.getAccForwardMatrix() * spring_pos );
            haptic_spring[index]->setSpringConstant( springConstant->getValue() );
            haptic_spring[index]->setDamping( damping->getValue() );
            haptic_spring[index]->setPositionInterpolation( positionInterpolation->getValue() );
            interpolatedPosition->setValue ( ti.getAccInverseMatrix() * Vec3f(haptic_spring[index]->getInterpolatedPosition ()), id );
            ti.addForceEffect( index, haptic_spring[index] );
            any_active = true;
          }
        }
      }
    }
    if( any_active ) {
      active->setValue( true, id );
    } else if( active->getValue( id ) ) {
      active->setValue( false, id );
    }
  }
}
