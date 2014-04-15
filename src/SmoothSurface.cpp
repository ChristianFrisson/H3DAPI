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
H3DNodeDatabase SmoothSurface::database( "SmoothSurface", 
                                         &(newInstance<SmoothSurface>), 
                                         typeid( SmoothSurface ),
                                         &H3DStiffnessSurfaceNode::database );

/// Constructor.
SmoothSurface::SmoothSurface( Inst< UpdateStiffness > _stiffness,
                              Inst< UpdateDamping   > _damping,
                              Inst< SFBool          > _useRelativeValues ):
  H3DStiffnessSurfaceNode( _stiffness, _damping, _useRelativeValues ) {
  type_name = "SmoothSurface";
  database.initFields( this );
}

void SmoothSurface::initialize() {
  H3DStiffnessSurfaceNode::initialize();
  hapi_surface.reset(
    new HAPI::FrictionSurface( stiffness->getValue(),
                               damping->getValue(),
                               0, 0,
                               useRelativeValues->getValue() ) );
}

