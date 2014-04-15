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
                            Inst< MFVec3f     > _torque,
                            Inst< SFFloat     > _springConstant,
                            Inst< SFBool      > _enabled,
                            Inst< MFInt32     > _deviceIndex,
                            Inst< SFNode      >  _metadata,
                            Inst< SFFloat     > _damping ) :
  H3DForceEffect( _metadata, _deviceIndex ),
  desiredAxis( _desiredAxis ),
  torque( _torque ),
  springConstant( _springConstant ),
  enabled( _enabled ),
  damping( _damping ),
  last_enabled( true ) {

  type_name = "RotationalSpringEffect";

  database.initFields( this );

  desiredAxis->setValue( Vec3f( 0,0,1 ) );
  springConstant->setValue( 100 );
  enabled->setValue( true );
  damping->setValue( 0 );
}

void RotationalSpringEffect::traverseSG( TraverseInfo &ti ) {
  bool tmp_enabled = enabled->getValue();
  if( tmp_enabled && !ti.getHapticsDevices().empty() ) {
    vector< H3DInt32 > device_index = deviceIndex->getValue();
    if( device_index.empty() ) {
      for( unsigned int i = 0; i < ti.getHapticsDevices().size(); ++i )
        device_index.push_back( i );
    }
    for( unsigned int i = 0; i < device_index.size(); ++i ) {
      int index = device_index[i];
      if( index >= 0 && ti.hapticsEnabled( index ) ) { 
        H3DHapticsDevice *hd = ti.getHapticsDevice( index );
        if( index >= (int)haptic_rotational_spring.size() )
          haptic_rotational_spring.resize( index + 1, NULL );
        if( index >= (int)torque->size() )
          torque->resize( index + 1, Vec3f( 0, 0, 0 ), id );
        if( !haptic_rotational_spring[index] )
          haptic_rotational_spring.set( index, new HAPI::HapticRotationalSpring() );
        haptic_rotational_spring[index]->setDesiredAxis(
          ti.getAccForwardMatrix().getScaleRotationPart() *
          desiredAxis->getValue() );
        haptic_rotational_spring[index]->setSpringConstant( springConstant->getValue() );
        haptic_rotational_spring[index]->setDamping( damping->getValue() );
        ti.addForceEffect( index, haptic_rotational_spring[index] );
        Vec3f f = (Vec3f) haptic_rotational_spring[index]->getLatestTorque();
        torque->setValue( index, f, id );
      }
    }
  } else if( last_enabled != tmp_enabled ) {
    for( unsigned int i = 0; i < torque->size(); ++i )
      torque->setValue( i, Vec3f( 0, 0, 0 ), id );
  }
  last_enabled = tmp_enabled;
}
