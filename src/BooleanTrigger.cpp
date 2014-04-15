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
/// \file BooleanTrigger.cpp
/// \brief CPP file for BooleanTrigger, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/BooleanTrigger.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase BooleanTrigger::database( 
                                   "BooleanTrigger", 
                                   &(newInstance<BooleanTrigger>), 
                                   typeid( BooleanTrigger ),
                                   &X3DTriggerNode::database );

namespace BooleanTriggerInternals {
  FIELDDB_ELEMENT( BooleanTrigger, set_triggerTime, INPUT_ONLY );
  FIELDDB_ELEMENT( BooleanTrigger, triggerTrue, OUTPUT_ONLY );
}

BooleanTrigger::BooleanTrigger( Inst< SFNode     > _metadata,
                                Inst< SFTime     > _set_triggerTime,
                                Inst< SetBoolean > _triggerTrue ):
  X3DTriggerNode ( _metadata        ),
  set_triggerTime( _set_triggerTime ),
  triggerTrue    ( _triggerTrue     ) {

  type_name = "BooleanTrigger";
  database.initFields( this );
  
  set_triggerTime->route( triggerTrue, id);
}
