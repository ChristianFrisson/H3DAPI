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
/// \file H3DFrictionalSurfaceNode.cpp
/// \brief cpp file for H3DFrictionalSurfaceNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DApi.h>
#include <H3D/H3DFrictionalSurfaceNode.h>
#include <HAPI/HAPIFrictionSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DFrictionalSurfaceNode::database( 
                                      "H3DFrictionalSurfaceNode", 
                                      NULL,
                                      typeid( H3DFrictionalSurfaceNode ),
                                      &H3DStiffnessSurfaceNode::database );

namespace FritionalSurfaceInternals {
  FIELDDB_ELEMENT( H3DFrictionalSurfaceNode, staticFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DFrictionalSurfaceNode, dynamicFriction, INPUT_OUTPUT );
}

H3DFrictionalSurfaceNode::H3DFrictionalSurfaceNode( 
        Inst< UpdateStiffness > _stiffness,
        Inst< UpdateDamping > _damping,
        Inst< UpdateStaticFriction > _staticFriction,
        Inst< UpdateDynamicFriction > _dynamicFriction,
        Inst< SFBool          > _useRelativeValues ):
  H3DStiffnessSurfaceNode( _stiffness, _damping, _useRelativeValues ),
  staticFriction( _staticFriction ),
  dynamicFriction( _dynamicFriction ) {
  type_name = "H3DFrictionalSurfaceNode";
  database.initFields( this );
  
  staticFriction->setValue( 0.1f );
  dynamicFriction->setValue( 0.4f );
}

void H3DFrictionalSurfaceNode::UpdateStaticFriction::
  onValueChange( const H3DFloat &v ) {
  H3DFrictionalSurfaceNode *fs = 
    static_cast< H3DFrictionalSurfaceNode * >( getOwner() );
  if( fs->hapi_surface.get() ) {
    static_cast< HAPI::HAPIFrictionSurface * >( fs->hapi_surface.get() )
      ->static_friction = v;
  }
}

void H3DFrictionalSurfaceNode::UpdateDynamicFriction::
  onValueChange( const H3DFloat &v ) {
  H3DFrictionalSurfaceNode *fs = 
    static_cast< H3DFrictionalSurfaceNode * >( getOwner() );
  if( fs->hapi_surface.get() ) {
    static_cast< HAPI::HAPIFrictionSurface * >( fs->hapi_surface.get() )
      ->dynamic_friction = v;
  }
}

