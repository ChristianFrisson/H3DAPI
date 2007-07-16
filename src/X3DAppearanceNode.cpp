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
/// \file X3DAppearanceNode.cpp
/// \brief CPP file for X3DAppearanceNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <X3DAppearanceNode.h>

using namespace H3D;

H3DNodeDatabase X3DAppearanceNode::database( "X3DAppearanceNode", 
                                             NULL,
                                             typeid( X3DAppearanceNode ),
                                             &X3DNode::database );

bool X3DAppearanceNode::default_using_multi_pass_transparency = true;

namespace X3DAppearanceNodeInternals {
  FIELDDB_ELEMENT( X3DAppearanceNode, surface, INPUT_OUTPUT );
}

X3DAppearanceNode::X3DAppearanceNode( Inst< DisplayList > _displayList,
                                      Inst< SFNode>  _metadata,
                                      Inst< SFSurface>  _surface ) :
  X3DNode( _metadata ),
  H3DDisplayListObject( _displayList ),
  surface( _surface )
	{

  type_name = "X3DAppearanceNode";
  database.initFields( this );

  displayList->setName( "displayList" );
  displayList->setOwner( this );
}

void X3DAppearanceNode::traverseSG( TraverseInfo &ti ) {
  ti.setCurrentSurface(static_cast< H3DSurfaceNode * >( surface->getValue() ) ); 
}

