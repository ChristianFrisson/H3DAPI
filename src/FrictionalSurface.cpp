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
/// \file FrictionalSurface.cpp
/// \brief cpp file for FrictionalSurface
///
//
//////////////////////////////////////////////////////////////////////////////

#include "FrictionalSurface.h"

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
  
void FrictionalSurface::hlRender( HLHapticsDevice *hd ) {
#ifdef HAVE_OPENHAPTICS
  SmoothSurface::hlRender( hd );
  hlMaterialf(HL_FRONT_AND_BACK, 
              HL_DYNAMIC_FRICTION, 
              dynamicFriction->getValue() );
  hlMaterialf(HL_FRONT_AND_BACK, 
              HL_STATIC_FRICTION, 
              staticFriction->getValue() );
#endif
}

FrictionalSurface::FrictionalSurface( Inst< SFFloat >  _stiffness,
                                      Inst< SFFloat >  _damping,
                                      Inst< SFFloat >  _staticFriction,
                                      Inst< SFFloat >  _dynamicFriction ):
  SmoothSurface( _stiffness, _damping ),
  staticFriction( _staticFriction ),
  dynamicFriction( _dynamicFriction ) {
  type_name = "FrictionalSurface";
  database.initFields( this );
  
  staticFriction->setValue( 0.1f );
  dynamicFriction->setValue( 0.4f );
}
