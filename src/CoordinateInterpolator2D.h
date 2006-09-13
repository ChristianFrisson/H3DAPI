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
/// \file CoordinateInterpolator2D.h
/// \brief Header file for CoordinateInterpolator2D, 
///  X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COORDINATEINTERPOLATOR2D_H__
#define __COORDINATEINTERPOLATOR2D_H__

#include "X3DInterpolatorNode.h"
#include "MFVec2f.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class CoordinateInterpolator2D
  /// \brief This node linearly interpolates among a
  /// list of MFVec2f values to produce an MFVec2f value_changed
  /// event.
  ///
  /// The number of coordinates in the keyValue field shall be an integer
  /// multiple of the number of key frames in the key field. That integer
  /// multiple defines how many coordinates will be contained in the
  /// value_changed events. 
  ///
  /// \par Internal routes:
  /// \dotfile CoordinateInterpolator2D.dot
  class H3DAPI_API CoordinateInterpolator2D : public X3DInterpolatorNode {
  public:

    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// routes_in[0] is the fraction_changed field
    /// routes_in[1] is the key field
    /// routes_in[2[ is the keyValue field    
    struct MFValue : TypedField< MFVec2f, 
    Types< SFFloat, MFFloat, MFVec2f > > {
      virtual void update();
    };
#ifdef __BORLANDC__
    friend struct MFValue;
#endif

    /// Construtor.
    CoordinateInterpolator2D( Inst< SFNode  > _metadata      = 0,
                              Inst< SFFloat > _set_fraction  = 0,
                              Inst< MFFloat > _key           = 0,
                              Inst< MFVec2f > _keyValue      = 0,
                              Inst< MFValue > _value_changed = 0 );


    /// The values to interpolate between.
    auto_ptr< MFVec2f >  keyValue;

    /// The linearly interpolated result value.
    auto_ptr<  MFVec2f >  value_changed;

    /// The H3DNodeDatabase for the node.
    static H3DNodeDatabase database;
  };
}

#endif
