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
/// \file X3DSequencerNode.cpp
/// \brief CPP file for X3DSequencerNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DSequencerNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DSequencerNode::database( 
                                   "X3DSequencerNode", 
                                   NULL, 
                                   typeid( X3DSequencerNode ),
                                   &X3DChildNode::database );

namespace X3DSequencerNodeInternals {
  FIELDDB_ELEMENT( X3DSequencerNode, next, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DSequencerNode, previous, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DSequencerNode, set_fraction, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DSequencerNode, key, INPUT_OUTPUT );
}

X3DSequencerNode::X3DSequencerNode( Inst< SFNode  > _metadata,
                                    Inst< SFBool  > _next,
                                    Inst< SFBool  > _previous,
                                    Inst< SFFloat  > _set_fraction,
                                    Inst< MFFloat > _key ):
  X3DChildNode( _metadata     ),
  next          ( _next  ),
  previous      ( _previous  ),
  set_fraction  ( _set_fraction  ),
  key            ( _key ) {

  type_name = "X3DSequencerNode";
  database.initFields( this );
}
