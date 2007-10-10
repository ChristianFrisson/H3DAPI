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
/// \file CoordinateInterpolator.cpp
/// \brief CPP file for CoordinateInterpolator, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CoordinateInterpolator.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase CoordinateInterpolator::database( 
                                "CoordinateInterpolator", 
                                &(newInstance<CoordinateInterpolator>), 
                                typeid( CoordinateInterpolator ),
                                &X3DInterpolatorNode::database );

namespace CoordinateInterpolatorInternals {
  FIELDDB_ELEMENT( CoordinateInterpolator, keyValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CoordinateInterpolator, value_changed, OUTPUT_ONLY );
}


CoordinateInterpolator::CoordinateInterpolator( 
                                   Inst< SFNode  >  _metadata,
                                   Inst< SFFloat >  _set_fraction,
                                   Inst< MFFloat >  _key,
                                   Inst< MFVec3f >  _keyValue,
                                   Inst< MFValue >  _value_changed ) :
  X3DInterpolatorNode( _metadata, _set_fraction, _key ),
  keyValue     ( _keyValue      ),
  value_changed( _value_changed ) {

  type_name = "CoordinateInterpolator";
  database.initFields( this );
  
  set_fraction->route( value_changed, id );
  key->route( value_changed, id ) ;
  keyValue->route( value_changed, id );
  
}


