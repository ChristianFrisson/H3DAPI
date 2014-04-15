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
/// \file CoordinateInterpolator2D.cpp
/// \brief CPP file for CoordinateInterpolator2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CoordinateInterpolator2D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase CoordinateInterpolator2D::database( 
                                      "CoordinateInterpolator2D", 
                                      &(newInstance<CoordinateInterpolator2D>), 
                                      typeid( CoordinateInterpolator2D ),
                                      &X3DInterpolatorNode::database );

namespace CoordinateInterpolator2DInternals {
  FIELDDB_ELEMENT( CoordinateInterpolator2D, value_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( CoordinateInterpolator2D, keyValue, INPUT_OUTPUT );
}


CoordinateInterpolator2D::CoordinateInterpolator2D( 
                                      Inst< SFNode   >  _metadata,
                                      Inst< SFFloat  >  _set_fraction,
                                      Inst< MFFloat  >  _key,
                                      Inst< MFVec2f  >  _keyValue,
                                      Inst< MFValue  >  _value_changed ) :
  X3DInterpolatorNode( _metadata, _set_fraction, _key ),
  keyValue     ( _keyValue      ),
  value_changed( _value_changed ) {

  type_name = "CoordinateInterpolator2D";
  database.initFields( this );
  
  set_fraction->route( value_changed, id );
  key->route( value_changed, id ) ;
  keyValue->route( value_changed, id );
}


void CoordinateInterpolator2D::MFValue::update() {
  CoordinateInterpolator2D *interpolator = 
    static_cast<CoordinateInterpolator2D*>( getOwner() );
  H3DFloat fraction = static_cast<SFFloat*>(routes_in[0])->getValue(interpolator->id);
  int key_size = static_cast<MFFloat*>(routes_in[1])->size();
  H3DFloat weight;
  int key_index = 
    static_cast<CoordinateInterpolator2D*>(owner)->lookupKey( fraction,
                                                              weight );
  vector< Vec2f > key_values = static_cast<MFVec2f*>(routes_in[2])->getValue();
  int value_size = 0;
  if( key_size != 0 )
   value_size = (int) key_values.size() / key_size;
  value.resize( value_size );

  if ( key_index >= 0 && 
       (key_index + 2)* value_size - 1 < (int)key_values.size() ) {
    if (weight<=0) 
      for (int x = 0; x < value_size; ++x )
  value[x] = key_values[ key_index*value_size + x ];
    else if (weight>=1)
      for (int x = 0; x < value_size; ++x )
  value[x] = key_values[ (key_index+1)*value_size + x];
    else { // else, interpolate linearly
      for (int x = 0; x < value_size; ++x ) {
  Vec2f a = key_values[ key_index*value_size + x ];
  Vec2f b = key_values[ (key_index+1)*value_size + x  ];
  value[ x ] = (1-weight)*a + (weight)*b;
      }
    }
  }
}
