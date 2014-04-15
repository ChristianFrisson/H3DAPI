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
/// \file ViscosityEffect.cpp
/// \brief cpp file for ViscosityEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/ViscosityEffect.h> 
#include <H3D/H3DHapticsDevice.h>
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
  FIELDDB_ELEMENT( ViscosityEffect, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViscosityEffect, dampingFactor, INPUT_OUTPUT );
}

/// Constructor
ViscosityEffect::ViscosityEffect( Inst< SFFloat > _radius,
                                  Inst< SFFloat > _viscosity,
                                  Inst< SFBool  > _enabled, 
                                  Inst< MFInt32 > _deviceIndex,
                                  Inst< SFNode  >  _metadata,
                                  Inst< SFFloat > _dampingFactor ) :
  H3DForceEffect( _metadata, _deviceIndex ),
  radius( _radius ),
  viscosity( _viscosity ),
  enabled( _enabled ),
  dampingFactor( _dampingFactor ) {
  
  type_name = "ViscosityEffect";

  database.initFields( this );
  
  radius->setValue( 0.0025f );
  viscosity->setValue( 0.015f );
  enabled->setValue( true );
  dampingFactor->setValue( 0.6f );
}

void ViscosityEffect::traverseSG( TraverseInfo &ti ) {
  if( enabled->getValue() ) {
    const vector< H3DInt32 > &device_index = deviceIndex->getValue();
    if( device_index.empty() ) {
      for( unsigned int i = 0; i < ti.getHapticsDevices().size(); ++i ) {
        if( ti.hapticsEnabled( i ) ) {
          ti.addForceEffect( i,
                             new HAPI::HapticViscosity(
                                                   viscosity->getValue(),
                                                   radius->getValue(),
                                                   dampingFactor->getValue() ) );
        }
      }
    } else {
      for( unsigned int i = 0; i < device_index.size(); ++i ) {
        if( device_index[i] >= 0 && ti.hapticsEnabled( device_index[i] ) ) {
          ti.addForceEffect( device_index[i],
                             new HAPI::HapticViscosity(
                                                   viscosity->getValue(),
                                                   radius->getValue(),
                                                   dampingFactor->getValue() ) );
        }
      }
    }
  }
}
