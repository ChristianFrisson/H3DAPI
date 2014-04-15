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
/// \file H3DStiffnessSurfaceNode.cpp
/// \brief cpp file for H3DStiffnessSurfaceNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DApi.h>
#include <H3D/H3DStiffnessSurfaceNode.h>
#include <HAPI/HAPIFrictionSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DStiffnessSurfaceNode::database( 
                                       "H3DStiffnessSurfaceNode", 
                                       NULL,
                                       typeid( H3DStiffnessSurfaceNode ),
                                       &H3DSurfaceNode::database );

namespace H3DStiffnessSurfaceNodeInternals {
  FIELDDB_ELEMENT( H3DStiffnessSurfaceNode, stiffness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DStiffnessSurfaceNode, damping, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DStiffnessSurfaceNode, useRelativeValues, INITIALIZE_ONLY );
}

/// Constructor.
H3DStiffnessSurfaceNode::H3DStiffnessSurfaceNode(
                              Inst< UpdateStiffness > _stiffness,
                              Inst< UpdateDamping   > _damping,
                              Inst< SFBool          > _useRelativeValues ):
  stiffness( _stiffness ),
  damping( _damping ),
  useRelativeValues( _useRelativeValues ) {
  type_name = "H3DStiffnessSurfaceNode";
  database.initFields( this );
  
  useRelativeValues->setValue( true );
  stiffness->setValue( 0.5 );
  damping->setValue( 0 );
}

void H3DStiffnessSurfaceNode::UpdateStiffness::onValueChange( const H3DFloat &v ) {
  H3DStiffnessSurfaceNode *ss = 
    static_cast< H3DStiffnessSurfaceNode * >( getOwner() );
  if( ss->hapi_surface.get() ) {
    static_cast< HAPI::HAPIFrictionSurface * >( ss->hapi_surface.get() )
      ->stiffness = v;
  }
}

void H3DStiffnessSurfaceNode::UpdateDamping::onValueChange( const H3DFloat &v ) {
  H3DStiffnessSurfaceNode *ss = 
    static_cast< H3DStiffnessSurfaceNode * >( getOwner() );
  if( ss->hapi_surface.get() ) {
    static_cast< HAPI::HAPIFrictionSurface * >( ss->hapi_surface.get() )
      ->damping = v;
  }
}

