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
/// \file H3DFunctionNode.cpp
/// \brief Cpp file for H3DFunctionNode.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DFunctionNode.h>

using namespace H3D;

H3DFunctionNode::H3DFunctionNode( Inst< SFNode>  _metadata,
                                  Inst< MFDouble  > _input,
                                  Inst< Value    > _output ) :
  X3DNode( _metadata ),
  input( _input ),
  output( _output ),
  functionChanged( new Field ) {
  functionChanged->setName( "functionChanged" );

  output->setValue( 0, id );
  
  input->routeNoEvent( output, id );
}

void H3DFunctionNode::Value::update() {
  H3DFunctionNode *function = static_cast< H3DFunctionNode *>( owner );
  const vector< H3DDouble > &input = 
    static_cast< MFDouble * >( routes_in[0] )->getValue();

  if( input.size() != function->nrInputValues() ) {
    H3DUtil::Console(3) 
      << "Invalid number of parameters in \"input\" field in \""
      << owner->getName() << "\". Got " << input.size() 
      << ", expected " << function->nrInputValues() << endl;
    value = 0;
  }

  if( input.size() <= 8 ) {
    H3DDouble inp[8];
    for( unsigned int i = 0; i < input.size(); ++i ) {
      inp[i] = input[i];
    }
    value = function->evaluate( inp );
  } else {
    H3DDouble *inp = new H3DDouble[input.size()];
    for( unsigned int i = 0; i < input.size(); ++i ) {
      inp[i] = input[i];
    }
    value = function->evaluate( inp );
    delete[] inp;
  }
}
