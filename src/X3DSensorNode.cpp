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
/// \file X3DSensorNode.cpp
/// \brief CPP file for X3DSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <X3DSensorNode.h>

using namespace H3D;

H3DNodeDatabase X3DSensorNode::database( 
        "X3DSensorNode", 
        NULL,
        typeid( X3DSensorNode ),
        &X3DChildNode::database 
        );

namespace X3DSensorNodeInternals {
  FIELDDB_ELEMENT( X3DSensorNode, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DSensorNode, isActive, OUTPUT_ONLY );
}

X3DSensorNode::X3DSensorNode( Inst< SFBool >  _enabled,
                              Inst< SFNode >  _metadata,
                              Inst< SFBool >  _isActive ) :
  X3DChildNode( _metadata ),
  enabled ( _enabled  ),
  isActive( _isActive ) {

  type_name = "X3DSensorNode";
  database.initFields( this );

  enabled->setValue( true );
  isActive->setValue( false, id );
}


