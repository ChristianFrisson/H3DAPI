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
/// \file ColorInterpolator.cpp
/// \brief CPP file for ColorInterpolator, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ColorInterpolator.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ColorInterpolator::database( 
   "ColorInterpolator", 
   &(newInstance<ColorInterpolator>), 
   typeid( ColorInterpolator ),
   &X3DInterpolatorNode::database );

namespace ColorInterpolatorInternals {
  FIELDDB_ELEMENT( ColorInterpolator, keyValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ColorInterpolator, value_changed, OUTPUT_ONLY );
}


ColorInterpolator::ColorInterpolator( Inst< SFNode  >  _metadata,
                                      Inst< SFFloat >  _set_fraction,
                                      Inst< MFFloat >  _key,
                                      Inst< MFColor >  _keyValue,
                                      Inst< SFValue >  _value_changed ) :
  X3DInterpolatorNode( _metadata, _set_fraction, _key ),
  keyValue     ( _keyValue      ),
  value_changed( _value_changed ) {

  type_name = "ColorInterpolator";
  database.initFields( this );

  set_fraction->route( value_changed, id );
  key->route( value_changed, id );
  keyValue->route( value_changed, id );
}

// HSV - RGB conversion routines from 
// http://www.acm.org/jgt/papers/SmithLyons96/hsv_rgb.html
Vec3f ColorInterpolator::SFValue::RGBToHSV( const RGB &rgb ) {
  int i; 	
  H3DFloat v, x, f; 	
  H3DFloat R = rgb.r, G = rgb.g, B = rgb.b;
  x = H3DMin(R, H3DMin( G, B ) ); 	
  v = H3DMax(R, H3DMax( G, B ) ); 	
  if(v == x) return Vec3f(0, 0, v); 	
  f = (R == x) ? G - B : ((G == x) ? B - R : R - G); 	
  i = (R == x) ? 3 : ((G == x) ? 5 : 1); 	
  return Vec3f(i - f /(v - x), (v - x)/v, v);
}

RGB ColorInterpolator::SFValue::HSVToRGB( const Vec3f &hsv ) {
  H3DFloat h = hsv.x, s = hsv.y, v = hsv.z, m, n, f; 	
  int i; 	
  if( h == 0.0 ) return RGB(v, v, v); 	
  i = (int) H3DFloor(h); 	
  f = h - i; 	
  if(!(i & 1)) f = 1 - f; // if i is even 	
  m = v * (1 - s); 	
  n = v * (1 - s * f); 	
  switch (i) { 	
  case 6: 	
  case 0: return RGB(v, n, m); 	
  case 1: return RGB(n, v, m); 	
  case 2: return RGB(m, v, n);
  case 3: return RGB(m, n, v); 	
  case 4: return RGB(n, m, v); 	
  case 5: return RGB(v, m, n);
  // should never happen
  default: return RGB( 0, 0, 0 );
  }
}

void ColorInterpolator::SFValue::update() {
  ColorInterpolator *interpolator = 
    static_cast<ColorInterpolator*>( getOwner() );
  H3DFloat fraction = 
    static_cast<SFFloat*>(routes_in[0])->getValue( interpolator->id );
  int key_size = static_cast<MFFloat*>(routes_in[1])->size();
  const vector< RGB > &key_values = 
    static_cast<MFColor*>(routes_in[2])->getValue();
  H3DFloat weight;
  int key_index = interpolator->lookupKey( fraction, 
                                           weight );
  
  if( key_size > 0 && key_index >= 0 ) {
    if (weight<=0) 
      value = key_values.front();
    else if (weight>=1)
      value = key_values.back();
    else {
      RGB a = key_values[ key_index ];
      RGB b = key_values[ key_index+1 ];
      Vec3f hsv_a = RGBToHSV(a);
      Vec3f hsv_b = RGBToHSV(b);
      H3DFloat range = H3DAbs( hsv_b.x - hsv_a.x );
      if( range <= 3 ) { 
        value = HSVToRGB( (1-weight)*hsv_a + (weight)*hsv_b );
      } else {
        range = 6 - range;
        H3DFloat step = range * weight;
        H3DFloat h = hsv_a.x < hsv_b.x ? hsv_a.x - step: hsv_a.x + step;
        if( h < 0 ) h += 6;
        if( h > 6 ) h -= 6;
        value = HSVToRGB( Vec3f( h,
                                 (1-weight)*hsv_a.y + (weight)*hsv_b.y,
                                 (1-weight)*hsv_a.z + (weight)*hsv_b.z ) );
      }
    }
  }
}
