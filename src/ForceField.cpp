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
/// \file ForceField.cpp
/// \brief cpp file for ForceField
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/ForceField.h> 

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ForceField::database( "ForceField", 
                                      &(newInstance<ForceField>), 
                                      typeid( ForceField ),
                                      &H3DForceEffect::database );

namespace ForceFieldInternals {
  FIELDDB_ELEMENT( ForceField, force, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ForceField, torque, INPUT_OUTPUT );
}

/// Constructor
ForceField::ForceField( Inst< SFVec3f > _force,
                        Inst< SFNode>  _metadata,
                        Inst< SFVec3f > _torque,
                        Inst< MFInt32 > _deviceIndex ) :
  H3DForceEffect( _metadata, _deviceIndex ),
  force( _force ),
  torque( _torque ) {

  type_name = "ForceField";
  database.initFields( this );
  force->setValue( Vec3f( 0,0,0 ) );
  torque->setValue( Vec3f( 0,0,0 ) );
}

void ForceField::traverseSG( TraverseInfo &ti ) {
  const vector< H3DInt32 > &device_index = deviceIndex->getValue();
  if( !ti.hapticsDisabledForAll() ) {
    Matrix3f rotation = ti.getAccForwardMatrix().getRotationPart();
    if( device_index.empty() ) {
      ti.addForceEffectToAll(
        new HAPI::HapticForceField( rotation * force->getValue(),
                                    rotation * torque->getValue() ) );
    } else {
      Vec3f tmp_force( rotation * force->getValue() );
      Vec3f tmp_torque( rotation * torque->getValue() );
      for( unsigned int i = 0; i < device_index.size(); ++i ) {
        if( device_index[i] >= 0 && ti.hapticsEnabled( device_index[i] ) ) {
          ti.addForceEffect( device_index[i],
                             new HAPI::HapticForceField( tmp_force,
                                                         tmp_torque ) );
        }
      }
    }
  }
}

