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
/// \file BooleanToggle.cpp
/// \brief CPP file for BooleanToggle, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/BooleanToggle.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase BooleanToggle::database( 
                                   "BooleanToggle", 
                                   &(newInstance<BooleanToggle>), 
                                   typeid( BooleanToggle ),
                                   &X3DChildNode::database );

namespace BooleanToggleInternals {
  FIELDDB_ELEMENT( BooleanToggle, set_boolean, INPUT_ONLY );
  FIELDDB_ELEMENT( BooleanToggle, toggle, INPUT_OUTPUT );
}

BooleanToggle::BooleanToggle( Inst< SFNode       > _metadata,
                              Inst< SetBoolean   > _set_boolean,
                              Inst< SFBool       > _toggle ):
  X3DChildNode( _metadata    ),
  set_boolean ( _set_boolean ),
  toggle      ( _toggle      ) {

  type_name = "BooleanToggle";
  database.initFields( this );

	toggle->setValue( false );
}
