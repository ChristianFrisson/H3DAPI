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
/// \file ForceField.h
/// \brief Header file for ForceField
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FORCEFIELD_H__
#define __FORCEFIELD_H__

#include <H3D/H3DForceEffect.h> 
#include <HAPI/HapticForceField.h>
#include <H3D/SFVec3f.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class ForceField
  /// This effect adds a constant force to all haptics devices at all times.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ForceField.x3d">ForceField.x3d</a>
  ///     ( <a href="examples/ForceField.x3d.html">Source</a> )
  class H3DAPI_API ForceField: public H3DForceEffect {
  public:
    /// Constructor
    ForceField( Inst< SFVec3f > _force = 0,
                Inst< SFNode  > _metadata = 0,
                Inst <SFVec3f > _torque = 0,
                Inst< MFInt32 > _deviceIndex = 0 );

    /// Adds a HapticForceField effect to the TraverseInfo.
    virtual void traverseSG( TraverseInfo &ti );

    /// The force to render.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f > force;

    /// The torque to render.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f > torque;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
