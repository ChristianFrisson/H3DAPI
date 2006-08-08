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
/// \file SmoothSurface.cpp
/// \brief cpp file for SmoothSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DApi.h"
#ifdef USE_HAPTICS
#include "SmoothSurface.h"
#include "HLObject.h"
#include "HLHapticsDevice.h"

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
}


void SmoothSurface::hlRender( HLHapticsDevice *hd ) {
#ifdef HAVE_OPENHAPTICS
  hlMakeCurrent( hd->getHapticContext() );
  hlTouchModel( HL_CONTACT );
  hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, stiffness->getValue() );
  hlMaterialf(HL_FRONT_AND_BACK, HL_DAMPING, damping->getValue() );
  hlMaterialf(HL_FRONT_AND_BACK, HL_DYNAMIC_FRICTION, 0 );
  hlMaterialf(HL_FRONT_AND_BACK, HL_STATIC_FRICTION, 0 );

#endif
}

/// Constructor.
SmoothSurface::SmoothSurface( Inst< SFFloat >  _stiffness,
                              Inst< SFFloat >  _damping ):
  stiffness( _stiffness ),
  damping( _damping ) {
  type_name = "SmoothSurface";
  database.initFields( this );
  
  stiffness->setValue( 0.5 );
  damping->setValue( 0 );
}
#endif
