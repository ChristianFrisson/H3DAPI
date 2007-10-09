//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file ViscosityEffect.cpp
/// \brief cpp file for ViscosityEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include <ViscosityEffect.h> 
#include <H3DHapticsDevice.h>
#include <HAPI/HapticViscosity.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ViscosityEffect::database( 
                                       "ViscosityEffect", 
                                       &(newInstance<ViscosityEffect>), 
                                       typeid( ViscosityEffect ),
                                       &H3DForceEffect::database );

namespace ViscosityEffectInternals {
  FIELDDB_ELEMENT( ViscosityEffect, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViscosityEffect, viscosity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViscosityEffect, force, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ViscosityEffect, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViscosityEffect, deviceIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViscosityEffect, dampingFactor, INPUT_OUTPUT );
}

/// Constructor
ViscosityEffect::ViscosityEffect( Inst< SFFloat > _radius,
                                  Inst< SFFloat > _viscosity,
                                  Inst< SFVec3f > _force,
                                  Inst< SFBool  > _enabled, 
                                  Inst< SFInt32 > _deviceIndex,
                                  Inst< SFNode  >  _metadata,
                                  Inst< SFFloat > _dampingFactor ) :
  H3DForceEffect( _metadata ),
  radius( _radius ),
  viscosity( _viscosity ),
  force( _force ),
  enabled( _enabled ),
  deviceIndex( _deviceIndex ),
  dampingFactor( _dampingFactor ) {
  
  type_name = "ViscosityEffect";

  database.initFields( this );
  
  radius->setValue( 0.0025f );
  viscosity->setValue( 0.015f );
  force->setValue( Vec3f( 0,0,0 ), id );
  enabled->setValue( true );
  deviceIndex->setValue( 0 );
  dampingFactor->setValue( 0.6f );
}

// TODO: remove counter when Sensable fixes the bug with invalid positions
// at startup. HLAPI reports the wrong position for the first couple of loops
// which can cause the spring to be added even though it should not.
int viscous_counter = 0;
void ViscosityEffect::traverseSG( TraverseInfo &ti ) {
  if( viscous_counter < 5 ) {
    viscous_counter++;
  } else {
    if( ti.hapticsEnabled()  ) {
      if( enabled->getValue() ) {
        int device_index = deviceIndex->getValue();
        H3DHapticsDevice *hd = ti.getHapticsDevice( device_index );
        ti.addForceEffect( device_index, new HAPI::HapticViscosity(
                            viscosity->getValue() / 1000,
                            radius->getValue() * 1000,
                            dampingFactor->getValue(),
                            false ) );
      }
    }
  }
}
