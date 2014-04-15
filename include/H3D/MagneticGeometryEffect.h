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
/// \file MagneticGeometryEffect.h
/// \brief Header file for MagneticGeometryEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MAGNETICGEOMETRYEFFECT_H__
#define __MAGNETICGEOMETRYEFFECT_H__

#include <H3D/H3DForceEffect.h> 
#include <H3D/SFVec3f.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>
#include <H3D/SFFloat.h>
#include <H3D/X3DGeometryNode.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class MagneticGeometryEffect
  /// MagneticGeometryEffect is a localized haptic effect that tries to
  /// constrain the haptic device to the surface of the given geometry.
  /// The effect is comparable to MagneticSurface. The given geometry
  /// is not rendered visually in the scene.
  /// The force is calculated as follows:
  /// force = (closest_point_to_surface - device_position) * springConstant
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MagneticGeometryEffect.x3d">MagneticGeometryEffect.x3d</a>
  ///     ( <a href="examples/MagneticGeometryEffect.x3d.html">Source</a> )
  class H3DAPI_API MagneticGeometryEffect: public H3DForceEffect {
  public:

    typedef TypedSFNode< X3DGeometryNode > SFGeometry;

    /// Constructor
    MagneticGeometryEffect(
                     Inst< SFBool  > _enabled     = 0,
                     Inst< SFNode  >  _metadata   = 0,
                     Inst< SFFloat > _startDistance = 0,
                     Inst< SFFloat > _escapeDistance = 0,
                     Inst< SFBool  > _active = 0,
                     Inst< SFFloat > _springConstant = 0,
                     Inst< SFGeometry > _geometry = 0,
                     Inst< MFInt32 > _deviceIndex = 0 );

    /// Adds a forceeffect.
    virtual void traverseSG( TraverseInfo &ti );

    /// Enables or disables the force calculations.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > enabled;

    /// When the haptics device comes within this distance of the geometry
    /// the constraint is activated.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.01 \n
    auto_ptr< SFFloat > startDistance;

    /// If the effect is active, the effect will be deactivated if the 
    /// haptics device moves outside this distance from the surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.01 \n
    auto_ptr< SFFloat > escapeDistance;

    /// Tells whether the effect is active or not.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool > active;

    /// The spring constant of the spring. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 300 \n
    auto_ptr< SFFloat > springConstant;

    /// The geometry whose surface to follow.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< SFGeometry > geometry;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    vector< bool > force_active;
  };
}

#endif
