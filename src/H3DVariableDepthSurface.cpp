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
/// \file H3DVariableDepthSurface.cpp
/// \brief cpp file for H3DVariableDepthSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DVariableDepthSurface.h"
#include <HAPI/DepthMapHAPISurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DVariableDepthSurface::database( 
                                   "H3DVariableDepthSurface", 
                                   NULL,
                                   typeid( H3DVariableDepthSurface ),
                                   &H3DSurfaceNode::database );

namespace H3DVariableDepthSurfaceInternals {
  FIELDDB_ELEMENT( H3DVariableDepthSurface, stiffness,        INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DVariableDepthSurface, damping,          INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DVariableDepthSurface, staticFriction,   INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DVariableDepthSurface, dynamicFriction,  INPUT_OUTPUT );
}

H3DVariableDepthSurface::H3DVariableDepthSurface(
                    Inst< UpdateStiffness       > _stiffness,
                    Inst< UpdateDamping         > _damping,
									  Inst< UpdateStaticFriction  > _staticFriction,
									  Inst< UpdateDynamicFriction > _dynamicFriction ):
  stiffness( _stiffness ),
  damping( _damping ),
  staticFriction( _staticFriction ),
  dynamicFriction( _dynamicFriction ) {

  type_name = "H3DVariableDepthSurface";
  database.initFields( this );
  stiffness->setValue( 0.5 );
  damping->setValue( 0 );
  staticFriction->setValue( 0.1 );
  dynamicFriction->setValue( 0.4 );
}

void H3DVariableDepthSurface::UpdateStiffness::
  setValue( const H3DFloat &f, int id ) {
  SFFloat::setValue( f, id );
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >
      ( hvds->hapi_surface.get() )->stiffness = f * hvds->conversion_to_HAPI;
  }
}

void H3DVariableDepthSurface::UpdateStiffness::update() {
  SFFloat::update();
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >( hvds->hapi_surface.get() )
      ->stiffness = value * hvds->conversion_to_HAPI;
  }
}

void H3DVariableDepthSurface::
  UpdateDamping::setValue( const H3DFloat &f, int id ) {
  SFFloat::setValue( f, id );
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >
      ( hvds->hapi_surface.get() )->damping = f * hvds->conversion_to_HAPI;
  }
}

void H3DVariableDepthSurface::UpdateDamping::update() {
  SFFloat::update();
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >( hvds->hapi_surface.get() )
      ->damping = value * hvds->conversion_to_HAPI;
  }
}

void H3DVariableDepthSurface::UpdateStaticFriction::
      setValue( const H3DFloat &f, int id ) {
  SFFloat::setValue( f, id );
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >( hvds->hapi_surface.get() )
      ->static_friction = f;
  }
}

void H3DVariableDepthSurface::UpdateStaticFriction::update() {
  SFFloat::update();
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >( hvds->hapi_surface.get() )
      ->static_friction = value;
  }
}

void H3DVariableDepthSurface::UpdateDynamicFriction::
    setValue( const H3DFloat &f, int id ) {
  SFFloat::setValue( f, id );
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >( hvds->hapi_surface.get() )
      ->dynamic_friction = f;
  }
}

void H3DVariableDepthSurface::UpdateDynamicFriction::update() {
  SFFloat::update();
  H3DVariableDepthSurface *hvds = 
    static_cast< H3DVariableDepthSurface * >( getOwner() );
  if( hvds->hapi_surface.get() ) {
    static_cast< HAPI::HAPIVariableDepthSurface * >( hvds->hapi_surface.get() )
      ->dynamic_friction = value;
  }
}
