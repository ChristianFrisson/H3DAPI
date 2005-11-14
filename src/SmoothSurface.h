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
/// \file SmoothSurface.h
/// \brief Header file for SmoothSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SMOOTHSURFACE_H__
#define __SMOOTHSURFACE_H__

#include "H3DSurfaceNode.h"
#include "HLSurface.h"
#include "FieldTemplates.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class SmoothSurface
  /// A SmoothSurface has no friction at all. Only the stiffness of the surface
  /// can be specified. The stiffness is specified as a value between 0 and 1
  /// where 1 is the maximum stiffness the device can handle.
  ///
  class H3DAPI_API SmoothSurface: public H3DSurfaceNode, public HLSurface {
  public:

    /// Constructor.
    SmoothSurface( Inst< SFFloat >  _stiffness = 0,
                   Inst< SFFloat >  _damping   = 0 );
  
    /// Renders the surface using hlMaterialf calls
    virtual void hlRender( HLHapticsDevice *hd );

    /// The stiffness of the surface. Should be a value between 0 and 1
    /// where 1 is the maximum stiffness the haptics device can handle.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.5 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< SFFloat > stiffness;

    /// The velocity based damping of the surface. Should be a value between
    /// 0 and 1 where 1 is the maximum damping the haptics device can handle.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< SFFloat > damping;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
