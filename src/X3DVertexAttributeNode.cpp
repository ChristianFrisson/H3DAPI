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
/// \file X3DVertexAttributeNode.cpp
/// \brief CPP file for X3DVertexAttributeNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DVertexAttributeNode.h>
#include <GL/glew.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DVertexAttributeNode::database( 
                                   "X3DVertexAttributeNode", 
				   NULL, 
                                   typeid( X3DVertexAttributeNode ),
                                   &X3DGeometricPropertyNode::database );

namespace X3DVertexAttributeNodeInternals {
  FIELDDB_ELEMENT( X3DVertexAttributeNode, name, INPUT_OUTPUT );
}

X3DVertexAttributeNode::X3DVertexAttributeNode( Inst< SFNode   > _metadata,
                                                Inst< SFString > _name ):
  X3DGeometricPropertyNode( _metadata ),
  name( _name ),
  attrib_index( -1 ) {

  type_name = "X3DVertexAttributeNode";
  database.initFields( this );
}

