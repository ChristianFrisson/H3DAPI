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
/// \file PositionInterpolator.h
/// \brief Header file for PositionInterpolator, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __POSITIONINTERPOLATOR_H__
#define __POSITIONINTERPOLATOR_H__

#include <H3D/X3DInterpolatorNode.h>
#include <H3D/SFVec3f.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PositionInterpolator
  /// \brief The PositionInterpolator node linearly interpolates among a 
  /// list of 3D vectors to produce an SFVec3f value_changed event. 
  ///
  /// The keyValue field shall contain exactly as many values as in the key
  /// field. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PositionInterpolator.x3d">PositionInterpolator.x3d</a>
  ///     ( <a href="examples/PositionInterpolator.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile PositionInterpolator.dot
  class H3DAPI_API PositionInterpolator : public X3DInterpolatorNode {
  public:

    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// routes_in[0] is the fraction_changed field
    /// routes_in[1] is the key field
    /// routes_in[2[ is the keyValue field
    struct H3DAPI_API SFValue : TypedField< SFVec3f, 
                                            Types< SFFloat, 
                                                   MFFloat, 
                                                   MFVec3f > >{
      virtual void update();
    };
#ifdef __BORLANDC__
    friend struct SFValue;
#endif
    
    /// Constructor.
    PositionInterpolator( Inst< SFNode  > _metadata      = 0,
                          Inst< SFFloat > _set_fraction  = 0,
                          Inst< MFFloat > _key           = 0,
                          Inst< MFVec3f > _keyValue      = 0,
                          Inst< SFValue > _value_changed = 0 );

    /// The values to interpolate between.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile PositionInterpolator_keyValue.dot
    auto_ptr< MFVec3f >  keyValue;

    /// The linearly interpolated result value.
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile PositionInterpolator_value_changed.dot
    auto_ptr< SFValue >  value_changed;

    /// The H3DNodeDatabase for the node.
    static H3DNodeDatabase database;
  };
}

#endif
