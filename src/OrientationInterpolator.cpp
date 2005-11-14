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
/// \file OrientationInterpolator.cpp
/// \brief CPP file for OrientationInterpolator, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "OrientationInterpolator.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OrientationInterpolator::database( 
                                   "OrientationInterpolator", 
                                   &(newInstance<OrientationInterpolator>), 
                                   typeid( OrientationInterpolator ),
                                   &X3DInterpolatorNode::database );

namespace OrientationInterpolatorInternals {
  FIELDDB_ELEMENT( OrientationInterpolator, keyValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OrientationInterpolator, value_changed, OUTPUT_ONLY );
}

OrientationInterpolator::OrientationInterpolator( 
                                  Inst< SFNode     > _metadata,
                                  Inst< SFFloat    > _set_fraction,
                                  Inst< MFFloat    > _key,
                                  Inst< MFRotation > _keyValue,
                                  Inst< SFValue    > _value_changed ) :
  X3DInterpolatorNode( _metadata, _set_fraction, _key ),
  keyValue     ( _keyValue      ),
  value_changed( _value_changed ) {

  type_name = "OrientationInterpolator";
  database.initFields( this );

  set_fraction->route( value_changed, id );
  key->route( value_changed, id );
  keyValue->route( value_changed, id );
}


void OrientationInterpolator::SFValue::update() {
  OrientationInterpolator *interpolator = 
    static_cast<OrientationInterpolator*>( getOwner() );
  H3DFloat fraction = 
    static_cast<SFFloat*>(routes_in[0])->getValue( interpolator->id );
  int key_size = static_cast<MFFloat*>( routes_in[1] )->size();
  const vector< Rotation > &key_values = 
    static_cast< MFRotation * >( routes_in[2] )->getValue();
  H3DFloat weight;
  int key_index = interpolator->lookupKey( fraction, 
                                           weight );
  if( key_size > 0 && key_index >= 0 ) {
    if (weight<=0) 
      value = key_values.front();
    else if (weight>=1)
      value = key_values.back();
    else {
      const Rotation &a = key_values[ key_index ];
      const Rotation &b = key_values[ key_index+1 ];
      value = a.slerp( b, weight );
    }
  }
}
