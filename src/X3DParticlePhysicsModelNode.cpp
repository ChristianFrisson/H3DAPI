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
/// \file X3DParticlePhysicsModelNode.cpp
/// \brief CPP file for X3DParticlePhysicsModelNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DParticlePhysicsModelNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DParticlePhysicsModelNode::database( 
                                   "X3DParticlePhysicsModelNode", 
                                   NULL, 
                                   typeid( X3DParticlePhysicsModelNode ),
                                   &X3DNode::database );

namespace X3DParticlePhysicsModelNodeInternals {
  FIELDDB_ELEMENT( X3DParticlePhysicsModelNode, enabled, INPUT_OUTPUT );
}

X3DParticlePhysicsModelNode::X3DParticlePhysicsModelNode( 
                      Inst< SFNode  > _metadata,
                      Inst< SFBool > _enabled ):
  X3DNode( _metadata ),
  enabled( _enabled ) {

  type_name = "X3DParticlePhysicsModelNode";
  database.initFields( this );

  enabled->setValue( true );
}




