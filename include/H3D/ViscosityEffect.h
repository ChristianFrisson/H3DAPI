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
/// \file ViscosityEffect.h
/// \brief Header file for ViscosityEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VISCOSITYEFFECT_H__
#define __VISCOSITYEFFECT_H__

#include <H3D/H3DForceEffect.h> 
#include <H3D/SFVec3f.h>
#include <H3D/SFBool.h>
#include <H3D/SFFloat.h>
#include <H3D/SFInt32.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class ViscosityEffect
  /// ViscosityEffect is a localized haptic effect
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ViscosityEffect.x3d">ViscosityEffect.x3d</a>
  ///     ( <a href="examples/ViscosityEffect.x3d.html">Source</a> )
  class H3DAPI_API ViscosityEffect: public H3DForceEffect {
  public:
    /// Constructor
    ViscosityEffect( Inst< SFFloat > _radius      = 0,
                     Inst< SFFloat > _viscosity   = 0,
                     Inst< SFBool  > _enabled     = 0,
                     Inst< MFInt32 > _deviceIndex = 0,
                     Inst< SFNode  >  _metadata   = 0,
                     Inst< SFFloat > _dampingFactor = 0 );

    /// Adds a forceeffect created from viscosity and radius.
    virtual void traverseSG( TraverseInfo &ti );


    /// The radius of the sphere moving through the fluid
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.0025 \n
    auto_ptr< SFFloat > radius;

    /// The viscosity of the fluid.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.01 \n
    auto_ptr< SFFloat > viscosity;

    /// Enables or disables the force calculations.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > enabled;

    ///  dampingFactor
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.5 \n
    auto_ptr< SFFloat > dampingFactor;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
