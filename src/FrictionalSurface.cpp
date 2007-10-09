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
/// \file FrictionalSurface.cpp
/// \brief cpp file for FrictionalSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3DApi.h>
#include <FrictionalSurface.h>
#include <HAPI/FrictionSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FrictionalSurface::database( 
                                            "FrictionalSurface", 
                                            &(newInstance<FrictionalSurface>),
                                            typeid( FrictionalSurface ),
                                            &SmoothSurface::database
                                            );

namespace FritionalSurfaceInternals {
  FIELDDB_ELEMENT( FrictionalSurface, staticFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FrictionalSurface, dynamicFriction, INPUT_OUTPUT );
}

FrictionalSurface::FrictionalSurface( 
        Inst< UpdateStiffness > _stiffness,
        Inst< UpdateDamping > _damping,
        Inst< UpdateStaticFriction > _staticFriction,
        Inst< UpdateDynamicFriction > _dynamicFriction ):
  SmoothSurface( _stiffness, _damping ),
  staticFriction( _staticFriction ),
  dynamicFriction( _dynamicFriction ),
  in_static_contact( true ) {
  type_name = "FrictionalSurface";
  database.initFields( this );
  
  staticFriction->setValue( 0.1f );
  dynamicFriction->setValue( 0.4f );
}

void FrictionalSurface::initialize() {
  SmoothSurface::initialize();
  HAPI::FrictionSurface *friction_surface =
    static_cast< HAPI::FrictionSurface * >( hapi_surface.get() );
  friction_surface->static_friction = staticFriction->getValue();
  friction_surface->dynamic_friction = dynamicFriction->getValue();
}

void FrictionalSurface::UpdateStaticFriction::
      setValue( const H3DFloat &f, int id ) {
  SFFloat::setValue( f, id );
  FrictionalSurface *fs = 
    static_cast< FrictionalSurface * >( getOwner() );
  if( fs->hapi_surface.get() ) {
    static_cast< HAPI::FrictionSurface * >( fs->hapi_surface.get() )
      ->static_friction = f;
  }
}

void FrictionalSurface::UpdateStaticFriction::update() {
  SFFloat::update();
  FrictionalSurface *fs = 
    static_cast< FrictionalSurface * >( getOwner() );
  if( fs->hapi_surface.get() ) {
    static_cast< HAPI::FrictionSurface * >( fs->hapi_surface.get() )
      ->static_friction = value;
  }
}

void FrictionalSurface::UpdateDynamicFriction::
    setValue( const H3DFloat &f, int id ) {
  SFFloat::setValue( f, id );
  FrictionalSurface *fs = 
    static_cast< FrictionalSurface * >( getOwner() );
  if( fs->hapi_surface.get() ) {
    static_cast< HAPI::FrictionSurface * >( fs->hapi_surface.get() )
      ->dynamic_friction = f;
  }
}

void FrictionalSurface::UpdateDynamicFriction::update() {
  SFFloat::update();
  FrictionalSurface *fs = 
    static_cast< FrictionalSurface * >( getOwner() );
  if( fs->hapi_surface.get() ) {
    static_cast< HAPI::FrictionSurface * >( fs->hapi_surface.get() )
      ->dynamic_friction = value;
  }
}