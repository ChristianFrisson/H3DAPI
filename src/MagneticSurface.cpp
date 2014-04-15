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
/// \file MagneticSurface.cpp
/// \brief cpp file for MagneticSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DApi.h>
#include <H3D/MagneticSurface.h>
#include <H3D/H3DHapticsDevice.h>
#ifdef HAVE_OPENHAPTICS
#include <HAPI/OpenHapticsRenderer.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MagneticSurface::database( 
                                        "MagneticSurface", 
                                        &(newInstance<MagneticSurface>), 
                                        typeid( MagneticSurface ),
                                        &H3DSurfaceNode::database );

namespace MagneticSurfaceInternals {
  FIELDDB_ELEMENT( MagneticSurface, stiffness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MagneticSurface, damping, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MagneticSurface, staticFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MagneticSurface, dynamicFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MagneticSurface, snapDistance, INPUT_OUTPUT );
}

/// Constructor.
MagneticSurface::MagneticSurface(
              Inst< UpdateStiffness       > _stiffness,
              Inst< UpdateDamping         > _damping,
              Inst< UpdateStaticFriction  > _staticFriction,
              Inst< UpdateDynamicFriction > _dynamicFriction,
              Inst< UpdateSnapDistance    > _snapDistance ):
  stiffness( _stiffness ),
  damping( _damping ),
  staticFriction( _staticFriction ),
  dynamicFriction( _dynamicFriction ),
  snapDistance( _snapDistance ) {
  type_name = "MagneticSurface";
  database.initFields( this );

  stiffness->setValue( 0.5f );
  damping->setValue( 0 );
  staticFriction->setValue( (H3DFloat)0.1 );
  dynamicFriction->setValue( (H3DFloat)0.4 );  
  snapDistance->setValue( (H3DFloat)0.01 );
}

void MagneticSurface::initialize() {
  H3DSurfaceNode::initialize();
#ifdef HAVE_OPENHAPTICS
  hapi_surface.reset(
    new HAPI::OpenHapticsRenderer::OpenHapticsSurface(
                              stiffness->getValue(),
                              damping->getValue(),
                              staticFriction->getValue(),
                              dynamicFriction->getValue(),
                              true,
                              snapDistance->getValue() * 1e3 ) );
#endif
}

void MagneticSurface::UpdateStiffness::onValueChange( const H3DFloat &f ) {
#ifdef HAVE_OPENHAPTICS
  MagneticSurface *ms = 
    static_cast< MagneticSurface * >( getOwner() );
  if( ms->hapi_surface.get() ) {
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
      ( ms->hapi_surface.get() )->stiffness = f;
  }
#endif
}

void MagneticSurface::UpdateDamping::onValueChange( const H3DFloat &f ) {
#ifdef HAVE_OPENHAPTICS
  MagneticSurface *ms = 
    static_cast< MagneticSurface * >( getOwner() );
  if( ms->hapi_surface.get() ) {
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
      ( ms->hapi_surface.get() )->damping = f;
  }
#endif
}

void MagneticSurface::UpdateStaticFriction::
      onValueChange( const H3DFloat &f ) {
#ifdef HAVE_OPENHAPTICS
  MagneticSurface *ms = 
    static_cast< MagneticSurface * >( getOwner() );
  if( ms->hapi_surface.get() ) {
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
      ( ms->hapi_surface.get() )->static_friction = f;
  }
#endif
}

void MagneticSurface::UpdateDynamicFriction::
    onValueChange( const H3DFloat &f ) {
#ifdef HAVE_OPENHAPTICS
  MagneticSurface *ms = 
    static_cast< MagneticSurface * >( getOwner() );
  if( ms->hapi_surface.get() ) {
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
      ( ms->hapi_surface.get() )->dynamic_friction = f;
  }
#endif
}

void MagneticSurface::UpdateSnapDistance::
    onValueChange( const H3DFloat &f ) {
#ifdef HAVE_OPENHAPTICS
  MagneticSurface *ms = 
    static_cast< MagneticSurface * >( getOwner() );
  if( ms->hapi_surface.get() ) {
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
      ( ms->hapi_surface.get() )->snap_distance = f * 1e3;
  }
#endif
}

