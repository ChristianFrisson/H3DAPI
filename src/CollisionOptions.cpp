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
/// \file CollisionOptions.cpp
/// \brief CPP file for CollisionOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CollisionOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase CollisionOptions::database( "CollisionOptions", 
                                      &(newInstance<CollisionOptions>),
                                      typeid( CollisionOptions ),
                                      &H3DOptionNode::database );

namespace CollisionOptionsInternals {
  FIELDDB_ELEMENT( CollisionOptions, avatarCollision, INPUT_OUTPUT );
}

CollisionOptions::CollisionOptions( 
                           Inst< SFNode>  _metadata,
                           Inst< SFBool  > _avatarCollision ) :
  H3DOptionNode( _metadata ),
  avatarCollision( _avatarCollision ) {
  
  type_name = "CollisionOptions";
  database.initFields( this );

  avatarCollision->setValue( true );
}
