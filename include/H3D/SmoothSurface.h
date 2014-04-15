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
/// \file SmoothSurface.h
/// \brief Header file for SmoothSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SMOOTHSURFACE_H__
#define __SMOOTHSURFACE_H__

#include <H3D/H3DStiffnessSurfaceNode.h>
#include <H3D/SFFloat.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class SmoothSurface
  /// A SmoothSurface has no friction at all. Only the stiffness and damping
  /// can be set. If the field useRelativeValues is true then
  /// stiffness is specified as a value between 0 and 1 where 1 is the
  /// maximum stiffness the device can handle. Otherwise the stiffness is
  /// in N/m.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Surfaces.x3d">Surfaces.x3d</a>
  ///     ( <a href="examples/Surfaces.x3d.html">Source</a> )
  class H3DAPI_API SmoothSurface: public H3DStiffnessSurfaceNode {
  public:

    /// Constructor.
    SmoothSurface( Inst< UpdateStiffness > _stiffness   = 0,
                   Inst< UpdateDamping   > _damping     = 0,
                   Inst< SFBool          > _useRelativeValues = 0 );

    void initialize();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
