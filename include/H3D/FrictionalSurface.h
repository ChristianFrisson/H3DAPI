
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
/// \file FrictionalSurface.h
/// \brief Header file for FrictionalSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FRICTIONALSURFACE_H__
#define __FRICTIONALSURFACE_H__

#include <H3D/H3DFrictionalSurfaceNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class FrictionalSurface
  /// Surface with friction.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Surfaces.x3d">Surfaces.x3d</a>
  ///     ( <a href="examples/Surfaces.x3d.html">Source</a> )
  class H3DAPI_API FrictionalSurface: public H3DFrictionalSurfaceNode {
  public:

    /// Constructor.
    FrictionalSurface( Inst< UpdateStiffness >  _stiffness = 0,
                       Inst< UpdateDamping >  _damping   = 0,
                       Inst< UpdateStaticFriction >  _staticFriction = 0,
                       Inst< UpdateDynamicFriction >  _dynamicFriction = 0,
                       Inst< SFBool          > _useRelativeValues = 0 );

    void initialize();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
