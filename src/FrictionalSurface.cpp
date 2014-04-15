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
/// \file FrictionalSurface.cpp
/// \brief cpp file for FrictionalSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DApi.h>
#include <H3D/FrictionalSurface.h>
#include <HAPI/FrictionSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FrictionalSurface::database( 
                                         "FrictionalSurface",
                                         &(newInstance<FrictionalSurface>),
                                         typeid( FrictionalSurface ),
                                         &H3DFrictionalSurfaceNode::database );

FrictionalSurface::FrictionalSurface( 
        Inst< UpdateStiffness > _stiffness,
        Inst< UpdateDamping > _damping,
        Inst< UpdateStaticFriction > _staticFriction,
        Inst< UpdateDynamicFriction > _dynamicFriction,
        Inst< SFBool          > _useRelativeValues ):
  H3DFrictionalSurfaceNode( _stiffness,
                        _damping,
                        _staticFriction,
                        _dynamicFriction,
                        _useRelativeValues ) {
  type_name = "FrictionalSurface";
  database.initFields( this );
}

void FrictionalSurface::initialize() {
  H3DFrictionalSurfaceNode::initialize();
  hapi_surface.reset(
    new HAPI::FrictionSurface( stiffness->getValue(),
                               damping->getValue(),
                               staticFriction->getValue(),
                               dynamicFriction->getValue(),
                               useRelativeValues->getValue() ) );
}

