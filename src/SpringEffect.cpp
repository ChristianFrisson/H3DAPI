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
/// \file SpringEffect.cpp
/// \brief cpp file for SpringEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include "SpringEffect.h" 
#include "H3DHapticsDevice.h"

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
  FIELDDB_ELEMENT( SpringEffect, deviceIndex, INPUT_OUTPUT );
}

/// Constructor
SpringEffect::SpringEffect( Inst< SFVec3f     > _position,
                            Inst< SFVec3f     > _force,
                            Inst< SFFloat     > _springConstant,
                            Inst< SFFloat     > _startDistance,
                            Inst< SFFloat     > _escapeDistance,
                            Inst< SFBool      > _active, 
                            Inst< SFInt32     > _deviceIndex,
                            Inst< SFNode      >  _metadata ) :
  H3DForceEffect( _metadata ),
  position( _position ),
  force( _force ),
  springConstant( _springConstant ),
  startDistance( _startDistance ),
  escapeDistance( _escapeDistance ),
  active( _active ),
  deviceIndex( _deviceIndex ),
  haptic_spring( new HapticSpring( Matrix4f(), 1 ) ) {
  
  type_name = "SpringEffect";

  database.initFields( this );
  
  position->setValue( Vec3f( 0,0,0 ) );
  force->setValue( Vec3f( 0,0,0 ) );
  springConstant->setValue( 100 );
  startDistance->setValue( 0.01f );
  escapeDistance->setValue( 0.01f );
  active->setValue( false, id );
  deviceIndex->setValue( 0 );
  
}

// TODO: remove counter when Sensable fixes the bug with invalid positions
// at startup. HLAPI reports the wrong position for the first couple of loops
// which can cause the spring to be added even though it should not.
int counter = 0;

#ifdef USE_HAPTICS
void SpringEffect::traverseSG( TraverseInfo &ti ) {
  if( counter < 5 ) {
    counter++;
  } else {
    if( ti.hapticsEnabled()  ) {
      int device_index = deviceIndex->getValue();
      H3DHapticsDevice *hd = ti.getHapticsDevice( device_index );
      // the tracker position in local coordinates.
      const Vec3f &pos = 
        ti.getAccInverseMatrix() * 
        hd->trackerPosition->getValue();
      const Vec3f &spring_pos = position->getValue();
      H3DFloat distance = ( pos - spring_pos ).length();
      if( active->getValue() ) {
        if( distance >= escapeDistance->getValue() ) {
          active->setValue( false, id );
          force->setValue( Vec3f( 0, 0, 0 ) );
        } else {
          haptic_spring->setPosition( spring_pos );
          haptic_spring->setSpringConstant( springConstant->getValue() );
          ti.addForceEffect( device_index, haptic_spring.get() );
          Vec3f f = haptic_spring->getLatestForce();
          force->setValue( f );
        }
      } else {
        if( distance <= startDistance->getValue() ) {
          active->setValue( true, id );
          haptic_spring->setPosition( spring_pos );
          haptic_spring->setSpringConstant( springConstant->getValue() );
          ti.addForceEffect( device_index, haptic_spring.get() );
        }
      }
    }
  }
}
#endif
