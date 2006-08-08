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
/// \file ToggleGroup.cpp
/// \brief CPP file for ToggleGroup.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ToggleGroup.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ToggleGroup::database( "ToggleGroup", 
                                       &(newInstance<ToggleGroup>), 
                                       typeid( ToggleGroup ),
                                       &X3DGroupingNode::database );

namespace ToggleGroupInternals {
  FIELDDB_ELEMENT( ToggleGroup, hapticsOn, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ToggleGroup, graphicsOn, INPUT_OUTPUT );
}

ToggleGroup::ToggleGroup( 
             Inst< MFChild > _addChildren,
             Inst< MFChild > _removeChildren,
             Inst< MFChild > _children,
             Inst< SFNode  > _metadata,
             Inst< SFBound > _bound,
             Inst< SFVec3f > _bboxCenter,
             Inst< SFVec3f > _bboxSize,
             Inst< SFBool  > _hapticsOn,
             Inst< SFBool  > _graphicsOn ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children, 
                   _metadata, _bound, _bboxCenter, _bboxSize ),
  hapticsOn( _hapticsOn ),
  graphicsOn( _graphicsOn ) {
  type_name = "ToggleGroup";
  database.initFields( this );

  hapticsOn->setValue( true );
  graphicsOn->setValue( true );

  graphicsOn->route( displayList );

}


void ToggleGroup::render() {
  if( graphicsOn->getValue() )
    X3DGroupingNode::render();
}

#ifdef USE_HAPTICS
void ToggleGroup::traverseSG( TraverseInfo &ti ) {
  bool is_enabled = ti.hapticsEnabled();
  if( is_enabled && !hapticsOn->getValue() ) {
    ti.disableHaptics();
    X3DGroupingNode::traverseSG( ti );
    ti.enableHaptics();
  } else {
    X3DGroupingNode::traverseSG( ti );
  }
}
#endif
