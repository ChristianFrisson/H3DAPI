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
/// \file PrototypeInstance.cpp
/// \brief CPP file for PrototypeInstance.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PrototypeInstance.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PrototypeInstance::database( 
                                            "PrototypeInstance", 
                                            NULL,
                                            typeid( PrototypeInstance ),
                                            &X3DPrototypeInstance::database );

Node* PrototypeInstance::clone ( bool deepCopy, DeepCopyMap& deepCopyMap ) {
  // Basic clone
  PrototypeInstance* n= new PrototypeInstance ( getClonedInstance ( prototyped_node.get(), deepCopy, deepCopyMap ) );
  n->metadata->setValue ( getClonedInstance ( metadata->getValue (), deepCopy, deepCopyMap ) );

  // Clone prototyped_node_extras
  n->prototyped_node_extras.reserve ( prototyped_node_extras.size() );
  for ( AutoRefVector< Node >::const_iterator i= prototyped_node_extras.begin(); i != prototyped_node_extras.end(); ++i ) {
    n->prototyped_node_extras.push_back ( getClonedInstance ( *i, deepCopy, deepCopyMap ) );
  }

  return n;
}