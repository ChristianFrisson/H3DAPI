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
/// \file GaussianFunction.cpp
/// \brief CPP file for GaussianFunction.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "GaussianFunction.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GaussianFunction::database( "GaussianFunction", 
                                              &(newInstance<GaussianFunction>), 
                                              typeid( GaussianFunction ),
                                              &H3DFunctionNode::database );

namespace GaussianFunctionInternals {
  FIELDDB_ELEMENT( GaussianFunction, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFunction, amplitude, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFunction, width, INPUT_OUTPUT );
}


GaussianFunction::GaussianFunction( Inst< SFNode  > _metadata,
                                    Inst< SFFloat > _center,
                                    Inst< SFFloat > _amplitude,
                                    Inst< SFFloat > _width ) :
  H3DFunctionNode( _metadata ),
  center( _center ),
  amplitude( _amplitude ), 
  width( _width ) {

  type_name = "GaussianFunction";
  database.initFields( this );

  center->setValue( 0 );
  amplitude->setValue( 1 );
  width->setValue( 1 );
}

H3DFloat GaussianFunction::get(H3D::H3DFloat x) {
  H3DFloat diff = x - center->getValue();
  H3DFloat w = width->getValue();
  return amplitude->getValue() * H3DExp( -(diff*diff )/( w * w ) );
}
