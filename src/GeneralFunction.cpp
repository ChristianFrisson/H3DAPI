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
/// \file GeneralFunction.cpp
/// \brief CPP file for GeneralFunction.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeneralFunction.h>

#ifdef HAVE_FPARSER
#include <HAPI/ParsedFunction.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GeneralFunction::database( "GeneralFunction", 
                                           &(newInstance<GeneralFunction>), 
                                           typeid( GeneralFunction ),
                                           &H3DFunctionNode::database );

namespace GeneralFunctionInternals {
  FIELDDB_ELEMENT( GeneralFunction, function, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GeneralFunction, params, INPUT_OUTPUT );
}


GeneralFunction::GeneralFunction( Inst< SFNode  > _metadata,
                                  Inst< SFString > _function,
                                  Inst< SFString > _params,
                                  Inst< SFFunctionObject > _functionObject ) :
  H3DFunctionNode( _metadata ),
  function( _function ),
  params( _params ), 
  functionObject( _functionObject ) {

  type_name = "GeneralFunction";
  database.initFields( this );

  functionObject->setOwner( this );
  functionObject->setName( "functionObject" );

  function->setValue( "x" );
  params->setValue( "x" );

  function->route( functionObject );
  params->route( functionObject );

  functionObject->route( functionChanged );
}

H3DDouble GeneralFunction::evaluate(H3D::H3DDouble *x) {
  HAPI::ParsedFunction *f = functionObject->getValue(); 
  if( !f ) return 0;
  else return f->evaluate( x );
}

void GeneralFunction::SFFunctionObject::update() {
  if( !value ) value = new HAPI::ParsedFunction;
  const string &function = 
    static_cast< SFString * >( routes_in[0] )->getValue();
  const string &param = 
    static_cast< SFString * >( routes_in[1] )->getValue();
  if( !value->setFunctionString( function, param ) ) {
    H3DUtil::Console(3) << "Could not parse function expression: \"" << function
                        << "\" in ParsedFunction node(" << owner->getName() 
                        <<"). " << endl;
  }
}


unsigned int GeneralFunction::nrInputValues() {
  const string &p = params->getValue();
  unsigned int nr_commas = 0;
  bool has_non_whitespace = false;
  for( unsigned int i = 0; i < p.size(); ++i ) {
    if( !has_non_whitespace && !isspace(p[i]) ) {
      has_non_whitespace = true;
    }
    if( p[i] == ',' ) ++nr_commas;
  }
  
  if( has_non_whitespace ) return nr_commas + 1;
  else return 0;
}

HAPI::HAPIFunctionObject *GeneralFunction::getAsHAPIFunctionObject() {
  HAPI::ParsedFunction * return_function = new HAPI::ParsedFunction;
  if( !return_function->setFunctionString( function->getValue(),
                                           params->getValue() ) ) {
    H3DUtil::Console(3) << "Could not parse function expression: \"" 
                        << function->getValue() << "\" in ParsedFunction node("
                        << getName() << "). " << endl;
    delete return_function;
    return NULL;
  }

  return return_function;
}

GeneralFunction::SFFunctionObject::SFFunctionObject() {
  value = new HAPI::ParsedFunction;
}

GeneralFunction::SFFunctionObject::~SFFunctionObject() {
  if( value ) {
    delete value;
    value = NULL;
  }
}

#endif
