//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file EaseInEaseOut.cpp
/// \brief CPP file for EaseInEaseOut, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/EaseInEaseOut.h>

using namespace H3D;


// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase EaseInEaseOut::database( 
                           "EaseInEaseOut", 
                           &( newInstance<EaseInEaseOut> ), 
                           typeid( EaseInEaseOut ),
                           &X3DChildNode::database );

namespace EaseInEaseOutInternals {
  FIELDDB_ELEMENT( EaseInEaseOut, set_fraction, INPUT_ONLY );
  FIELDDB_ELEMENT( EaseInEaseOut, easeInEaseOut, INPUT_OUTPUT );
  FIELDDB_ELEMENT( EaseInEaseOut, key, INPUT_OUTPUT );
  FIELDDB_ELEMENT( EaseInEaseOut, modifiedFraction_changed, OUTPUT_ONLY );
}

EaseInEaseOut::EaseInEaseOut( 
                         Inst< SFFloat    > _set_fraction,
                         Inst< MFVec2f    > _easeInEaseOut,
                         Inst< MFFloat    > _key,
                         Inst< SFNode     > _metadata,
                         Inst< SFFraction > _modifiedFraction_changed ) :
  X3DChildNode( _metadata ),
  set_fraction( _set_fraction ),
  easeInEaseOut( _easeInEaseOut ),
  key( _key ),
  modifiedFraction_changed( _modifiedFraction_changed ) {
  
  type_name = "EaseInEaseOut";
  database.initFields( this );

  set_fraction->route( modifiedFraction_changed, id );
  key->route( modifiedFraction_changed, id );
  easeInEaseOut->route( modifiedFraction_changed, id );
}

int EaseInEaseOut::lookupKey( H3DFloat f, H3DFloat &p ) {
  vector<H3DFloat> keys = key->getValue();
  if( keys.size() == 0 ) return -1;
  if ( keys.size() == 1 || f <= keys[0] ) {
    p = 0;
    return 0;
  }
  if ( f >= keys[keys.size()-1] ) {
    p = 1;
    return (int)keys.size() - 2;
  }

  // we know here that f is larger than keys[0] so we know that we will
  // go into the if-statement at some point.
  for( size_t i = keys.size()-1; true; i--) {
    if ( f > keys[i] ) {
      p = (f-keys[i])/(keys[i+1]-keys[i]);
      if( p < 0 ) p = 0;
      if( p > 1 ) p = 1;
      return (int) i;
    }
  }
  return -1;
}

void EaseInEaseOut::checkKeyEaseInEaseOut() {
  if ( key->size() != easeInEaseOut->size() ) {
    stringstream s;
    s << "Number of field values in key and easeInEaseOut "
      << "must be the same (in \"" << getName() << "\" node) ";
    throw UnequalFieldValues( s.str(), H3D_FULL_LOCATION );
  }
}

void EaseInEaseOut::SFFraction::update() {
  EaseInEaseOut * e = static_cast< EaseInEaseOut* >( getOwner() );
  // Check that number of values in easeInEaseOut and key are equal
  e->checkKeyEaseInEaseOut();
  H3DFloat fraction 
    = static_cast< SFFloat* >(routes_in[0])->getValue(e->id);
  vector< H3DFloat > key 
    = static_cast< MFFloat* >(routes_in[1])->getValue();
  vector< Vec2f > ease 
    = static_cast< MFVec2f* >(routes_in[2])->getValue();
  
  // No keys, return
  if ( key.size() == 0 ) return;

  // Calculations as specified in X3D specification
  H3DFloat u, eout, ein;
  int key_index = e->lookupKey( fraction, u );

  eout = ease.at( key_index ).y;
  ein = ease.at( (key_index+1)%key.size() ).x;
  
  H3DFloat sum, t = 0;
  sum = ein + eout;

  if ( sum < 0 ) {
    value = u;
  }
  if ( sum > 1.0 ) {
    ein /= sum;
    eout /= sum;
  }
  t = 1.0f / ( 2-eout-ein );
  if ( u < eout ) {
    value = ( t/eout ) * u * u;
  } else if ( u < (1.0-ein) ) {
    value = t * ( 2*u-eout );
  } else {
    value = 1.0f - ( (t*(1-u)*(1-u))/ein );
  }
}

