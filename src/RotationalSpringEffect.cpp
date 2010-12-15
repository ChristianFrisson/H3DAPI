//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2010, SenseGraphics AB
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
/// \file RotationalSpringEffect.cpp
/// \brief cpp file for RotationalSpringEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/RotationalSpringEffect.h> 
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase RotationalSpringEffect::database( 
                                       "RotationalSpringEffect", 
                                       &(newInstance<RotationalSpringEffect>), 
                                       typeid( RotationalSpringEffect ),
                                       &H3DForceEffect::database );

namespace RotationalSpringEffectInternals {
  FIELDDB_ELEMENT( RotationalSpringEffect, desiredAxis, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RotationalSpringEffect, torque, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RotationalSpringEffect, springConstant, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RotationalSpringEffect, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RotationalSpringEffect, deviceIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RotationalSpringEffect, damping, INPUT_OUTPUT );
}

/// Constructor
RotationalSpringEffect::RotationalSpringEffect( Inst< SFVec3f     > _desiredAxis,
                            Inst< SFVec3f     > _torque,
                            Inst< SFFloat     > _springConstant,
                            Inst< SFBool      > _enabled,
                            Inst< SFInt32     > _deviceIndex,
                            Inst< SFNode      >  _metadata,
                            Inst< SFFloat     > _damping ) :
  H3DForceEffect( _metadata ),
  desiredAxis( _desiredAxis ),
  torque( _torque ),
  springConstant( _springConstant ),
  enabled( _enabled ),
  deviceIndex( _deviceIndex ),
  damping( _damping ),
  haptic_rotational_spring( new HAPI::HapticRotationalSpring() ),
  last_enabled( true ) {
  
  type_name = "RotationalSpringEffect";

  database.initFields( this );

  desiredAxis->setValue( Vec3f( 0,0,1 ) );
  torque->setValue( Vec3f( 0,0,0 ) );
  springConstant->setValue( 100 );
  enabled->setValue( true );
  deviceIndex->setValue( 0 );
  damping->setValue( 0 );
}

// TODO: remove counter when Sensable fixes the bug with invalid orientations
// at startup. HLAPI reports the wrong orientation for the first couple of loops
// which can cause the spring to be added even though it should not.
int rot_spring_counter = 0;

void RotationalSpringEffect::traverseSG( TraverseInfo &ti ) {
  bool tmp_enabled = enabled->getValue();
  if( rot_spring_counter < 5 ) {
    rot_spring_counter++;
  } else {
    if( tmp_enabled && !ti.getHapticsDevices().empty() ) {
      int device_index = deviceIndex->getValue();
      if( ti.hapticsEnabled( device_index ) ) { 
        H3DHapticsDevice *hd = ti.getHapticsDevice( device_index );
        haptic_rotational_spring->setDesiredAxis(
          ti.getAccForwardMatrix().getScaleRotationPart() *
          desiredAxis->getValue() );
        haptic_rotational_spring->setSpringConstant( springConstant->getValue() );
        haptic_rotational_spring->setDamping( damping->getValue() );
        ti.addForceEffect( device_index, haptic_rotational_spring.get() );
        Vec3f f = (Vec3f) haptic_rotational_spring->getLatestTorque();
        torque->setValue( f );
      }
    } else if( last_enabled != tmp_enabled ) {
      torque->setValue( Vec3f( 0, 0, 0 ) );
    }
  }
  last_enabled = tmp_enabled;
}
