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
/// \file OpenHapticsSurface.cpp
/// \brief cpp file for OpenHapticsSurface
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/OpenHapticsSurface.h>

#ifdef HAVE_OPENHAPTICS
#include <HAPI/OpenHapticsRenderer.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OpenHapticsSurface::database( 
                                        "OpenHapticsSurface", 
                                        &(newInstance<OpenHapticsSurface>), 
                                        typeid( OpenHapticsSurface ),
                                        &MagneticSurface::database );

namespace OpenHapticsSurfaceInternals {
  FIELDDB_ELEMENT( OpenHapticsSurface, magnetic, INPUT_OUTPUT );
}

/// Constructor.
OpenHapticsSurface::OpenHapticsSurface(
              Inst< UpdateStiffness > _stiffness,
              Inst< UpdateDamping > _damping,
              Inst< UpdateStaticFriction > _staticFriction ,
              Inst< UpdateDynamicFriction > _dynamicFriction,
              Inst< UpdateMagnetic  > _magnetic ,
              Inst< UpdateSnapDistance > _snapDistance ) :
  MagneticSurface( _stiffness, _damping, _staticFriction,
                   _dynamicFriction, _snapDistance ),
  magnetic( _magnetic ) {
  type_name = "OpenHapticsSurface";
  database.initFields( this );
  
  magnetic->setValue( false );
}

void OpenHapticsSurface::initialize() {
  MagneticSurface::initialize();
  HAPI::OpenHapticsRenderer::OpenHapticsSurface * open_haptics_surface =
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
    ( hapi_surface.get() );
  open_haptics_surface->magnetic = magnetic->getValue();
}

void OpenHapticsSurface::UpdateMagnetic::
    onValueChange( const bool &b ) {
  OpenHapticsSurface *ohs =
    static_cast< OpenHapticsSurface * >( getOwner() );
  if( ohs->hapi_surface.get() ) {
    static_cast< HAPI::OpenHapticsRenderer::OpenHapticsSurface * >
      ( ohs->hapi_surface.get() )->snap_distance = b;
  }
}

#endif
