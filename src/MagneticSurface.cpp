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
/// \file MagneticSurface.cpp
/// \brief cpp file for MagneticSurface
///
//
//////////////////////////////////////////////////////////////////////////////

#include "MagneticSurface.h"
#include "HLObject.h"
#include "HLHapticsDevice.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MagneticSurface::database( 
                                        "MagneticSurface", 
                                        &(newInstance<MagneticSurface>), 
                                        typeid( MagneticSurface ),
                                        &FrictionalSurface::database );

namespace MagneticSurfaceInternals {
  FIELDDB_ELEMENT( MagneticSurface, snapDistance, INPUT_OUTPUT );
}


void MagneticSurface::hlRender( HLHapticsDevice *hd ) {
#ifdef HAVE_OPENHAPTICS
  FrictionalSurface::hlRender( hd );
  hlTouchModel( HL_CONSTRAINT );
  // convert to millimeters
  hlTouchModelf( HL_SNAP_DISTANCE, 1000 * snapDistance->getValue() );
#endif
}

/// Constructor.
MagneticSurface::MagneticSurface( Inst< SFFloat >  _stiffness,
                                  Inst< SFFloat >  _damping,
                                  Inst< SFFloat >  _staticFriction,
                                  Inst< SFFloat >  _dynamicFriction,
                                  Inst< SFFloat > _snapDistance ):
  FrictionalSurface( _stiffness, _damping, 
                     _staticFriction, _dynamicFriction ),
  snapDistance( _snapDistance ) {
  type_name = "MagneticSurface";
  database.initFields( this );
  
  snapDistance->setValue( (H3DFloat)0.01 );
}
