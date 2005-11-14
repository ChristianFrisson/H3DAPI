
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
/// \file FrictionalSurface.h
/// \brief Header file for FrictionalSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FRICTIONALSURFACE_H__
#define __FRICTIONALSURFACE_H__

#include "SmoothSurface.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class FrictionalSurface
  /// Surface with friction.
  class H3DAPI_API FrictionalSurface: public SmoothSurface {
  public:

    /// Constructor.
    FrictionalSurface( Inst< SFFloat >  _stiffness = 0,
                       Inst< SFFloat >  _damping   = 0,
                       Inst< SFFloat >  _staticFriction = 0,
                       Inst< SFFloat >  _dynamicFriction = 0 );
  
    /// Renders the surface using hlMaterialf calls
    virtual void hlRender( HLHapticsDevice *hd );

    /// The friction that is experienced upon initial movement when resting on 
    /// the surface.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.1 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< SFFloat > staticFriction;

    /// The friction that is experienced when moving along the surface 
    /// the surface.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.4 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< SFFloat > dynamicFriction;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
