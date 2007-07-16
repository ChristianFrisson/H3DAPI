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
/// \file Switch.cpp
/// \brief CPP file for Switch, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <Switch.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Switch::database( 
                                 "Switch", 
                                 &(newInstance<Switch>), 
                                 typeid( Switch ),
                                 &(X3DGroupingNode::database ) );

namespace SwitchInternals {
  FIELDDB_ELEMENT( Switch, whichChoice, INPUT_OUTPUT );
}

Switch::Switch( 
               Inst< MFChild  > _addChildren,
               Inst< MFChild  > _removeChildren,
               Inst< MFChild  > _children,
               Inst< SFNode   > _metadata,
               Inst< SFBound  > _bound,
               Inst< SFVec3f  > _bboxCenter,
               Inst< SFVec3f  > _bboxSize,
               Inst< SFInt32  > _whichChoice ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children, _metadata, 
                   _bound,_bboxCenter, _bboxSize ),
  whichChoice   ( _whichChoice  ) {

  type_name = "Switch";
  database.initFields( this );
  
  whichChoice->setValue( -1 );
  whichChoice->route( displayList );
  whichChoice->route( bound );
}


void Switch::render() {
  int choice = whichChoice->getValue();
  if( choice < 0 || (size_t)choice > children->size() - 1 ) return;
  X3DChildNode *child_node = children->getValueByIndex( choice );
  H3DDisplayListObject *dlo = 
    dynamic_cast< H3DDisplayListObject * >(child_node);
  if( dlo )
    dlo->displayList->callList();
  else {
    if( child_node ) child_node->render();
  }
}

void Switch::SFBound::update() {
  int choice = static_cast< SFInt32 * >(routes_in[0])->getValue();
  unsigned int nr_bounds = routes_in.size() - 1;
  if( choice >= 0 && (unsigned int)choice < nr_bounds  )
    value = static_cast< SFBound * >( routes_in[choice+1] )->getValue();
}

void Switch::traverseSG( TraverseInfo &ti ) {
  int choice = whichChoice->getValue();
  if( choice < 0 || (size_t)choice > children->size() - 1 ) return;
  X3DChildNode *child_node = children->getValueByIndex( choice );
  if( child_node ) child_node->traverseSG( ti );
}
