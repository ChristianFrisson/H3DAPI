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
/// \file BooleanSequencer.cpp
/// \brief CPP file for BooleanSequencer, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "BooleanSequencer.h"

using namespace H3D;

H3DNodeDatabase BooleanSequencer::database( 
        "BooleanSequencer", 
        &(newInstance< BooleanSequencer > ),
        typeid( BooleanSequencer ),
        &X3DSequencerNode::database 
        );

namespace BooleanSequencerInternals {
  FIELDDB_ELEMENT( BooleanSequencer, keyValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( BooleanSequencer, value_changed, OUTPUT_ONLY );
}

BooleanSequencer::BooleanSequencer( 
										Inst< SFNode							 > _metadata,
										Inst< SFBool							 > _next,
										Inst< SFBool							 > _previous,
										Inst< SFFloat							 > _set_fraction,
										Inst< MFFloat							 > _key,
										Inst< KeyValues < MFBool > > _keyValue,
										Inst< ValueChanged < SFBool, MFBool > > _value_changed ) :
  X3DSequencerNode( _metadata, _next, _previous, _set_fraction, _key ),
  keyValue        ( _keyValue      ),
  value_changed		( _value_changed ) {

  type_name = "BooleanSequencer";

	next->routeNoEvent( value_changed, id );
	previous->routeNoEvent( value_changed, id );
	set_fraction->routeNoEvent( value_changed, id );
	key->routeNoEvent( value_changed, id );
	keyValue->routeNoEvent( value_changed, id );

  database.initFields( this );
}
