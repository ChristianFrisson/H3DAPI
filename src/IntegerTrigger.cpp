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
/// \file IntegerTrigger.cpp
/// \brief CPP file for IntegerTrigger, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/IntegerTrigger.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase IntegerTrigger::database( 
                                   "IntegerTrigger", 
                                   &(newInstance<IntegerTrigger>), 
                                   typeid( IntegerTrigger ),
                                   &X3DTriggerNode::database );

namespace IntegerTriggerInternals {
  FIELDDB_ELEMENT( IntegerTrigger, set_boolean, INPUT_ONLY );
  FIELDDB_ELEMENT( IntegerTrigger, integerKey, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IntegerTrigger, triggerValue, OUTPUT_ONLY );
}

IntegerTrigger::IntegerTrigger( Inst< SFNode          > _metadata,
                                Inst< SFBool          > _set_boolean,
                                Inst< SFInt32          > _integerKey,
                                Inst< SetTriggerValue > _triggerValue ):
  X3DTriggerNode( _metadata     ),
  set_boolean   ( _set_boolean  ),
  integerKey    ( _integerKey   ),
  triggerValue  ( _triggerValue ){

  type_name = "IntegerTrigger";
  database.initFields( this );

  integerKey->setValue( -1 );

  set_boolean->route( triggerValue, id );

}
