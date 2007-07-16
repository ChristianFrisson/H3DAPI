//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file X3DFogObject.h
/// \brief Header file for X3DFogObject, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DFOGOBJECT_H__
#define __X3DFOGOBJECT_H__

#include <SFColor.h>
#include <SFString.h>
#include <SFFloat.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \class X3DFogObject
  /// \brief X3DFogObject is the abstract type that describes a node that 
  /// influences the lighting equation through the use of fog semantics. 
  class H3DAPI_API X3DFogObject {
  public:

    /// Constructor.
    X3DFogObject( Inst< SFColor  > _color           = 0,
                  Inst< SFString > _fogType         = 0,
                  Inst< SFFloat  > _visibilityRange = 0 );

    /// The color of the fog.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 1 1 \n
    auto_ptr< SFColor >  color;

    /// The fogType field controls how much of the fog colour is blended
    /// with the object as a function of distance. If fogType is "LINEAR",
    /// the amount of blending is a linear function of the distance, resulting
    /// in a depth cueing effect. If fogType is "EXPONENTIAL," an exponential
    /// increase in blending is used, resulting in a more natural fog
    /// appearance.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "LINEAR" \n
    /// <b>Valid values:</b> "LINEAR" | "EXPONENTIAL" \n
    auto_ptr< SFString >  fogType;

    /// The visibilityRange specifies the distance in metres (in the local
    /// coordinate system) at which objects are totally obscured by the fog. 
    /// Objects located outside the visibilityRange from the viewer are drawn
    /// with a constant colour of color. Objects very close to the viewer are
    /// blended very little with the fog color. A visibilityRange of 0.0
    /// disables the fog.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFFloat >  visibilityRange;
  };
}

#endif
