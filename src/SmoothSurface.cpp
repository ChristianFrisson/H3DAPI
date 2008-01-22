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
/// \file SmoothSurface.cpp
/// \brief cpp file for SmoothSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DApi.h>
#include <H3D/SmoothSurface.h>
#include <HAPI/FrictionSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SmoothSurface::database( 
                                        "SmoothSurface", 
                                        &(newInstance<SmoothSurface>), 
                                        typeid( SmoothSurface ),
                                        &H3DSurfaceNode::database );

namespace SmoothSurfaceInternals {
  FIELDDB_ELEMENT( SmoothSurface, stiffness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SmoothSurface, damping, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SmoothSurface, mmStiffness,  INITIALIZE_ONLY );
}

/// Constructor.
SmoothSurface::SmoothSurface( Inst< UpdateStiffness > _stiffness,
                              Inst< UpdateDamping   > _damping,
                              Inst< SFBool          > _mmStiffness ):
  stiffness( _stiffness ),
  damping( _damping ),
  mmStiffness( _mmStiffness ) {
  type_name = "SmoothSurface";
  database.initFields( this );
  
  mmStiffness->setValue( true );
  stiffness->setValue( 0.5 );
  damping->setValue( 0 );
}

void SmoothSurface::initialize() {
  H3DSurfaceNode::initialize();
  hapi_surface.reset(
    new HAPI::FrictionSurface( mmStiffness->getValue() ?
                                stiffness->getValue() :
                                stiffness->getValue() * 0.001f,
                               damping->getValue() * 0.001f ) );
}

void SmoothSurface::UpdateStiffness::setValue( const H3DFloat &f, int id ){
  SFFloat::setValue( f, id );
  SmoothSurface *ss = 
    static_cast< SmoothSurface * >( getOwner() );
  if( ss->hapi_surface.get() ) {
    if( ss->mmStiffness->getValue() ) {
      static_cast< HAPI::FrictionSurface * >( ss->hapi_surface.get() )
        ->stiffness = f;
    } else {
      static_cast< HAPI::FrictionSurface * >( ss->hapi_surface.get() )
        ->stiffness = f * 0.001f;
    }
  }
}

void SmoothSurface::UpdateStiffness::update() {
  SFFloat::update();
  SmoothSurface *ss = 
    static_cast< SmoothSurface * >( getOwner() );
  if( ss->hapi_surface.get() ) {
    if( ss->mmStiffness->getValue() ) {
      static_cast< HAPI::FrictionSurface * >( ss->hapi_surface.get() )
        ->stiffness = value;
    } else {
      static_cast< HAPI::FrictionSurface * >( ss->hapi_surface.get() )
        ->stiffness = value * 0.001f;
    }
  }
}

void SmoothSurface::UpdateDamping::setValue( const H3DFloat &f, int id ){
  SFFloat::setValue( f, id );
  SmoothSurface *ss = 
    static_cast< SmoothSurface * >( getOwner() );
  if( ss->hapi_surface.get() ) {
    static_cast< HAPI::FrictionSurface * >( ss->hapi_surface.get() )
      ->damping = f / 1000; 
  }
}

void SmoothSurface::UpdateDamping::update() {
  SFFloat::update();
  SmoothSurface *ss = 
    static_cast< SmoothSurface * >( getOwner() );
  if( ss->hapi_surface.get() ) {
    static_cast< HAPI::FrictionSurface * >( ss->hapi_surface.get() )
      ->damping = value / 1000; 
  }
}
