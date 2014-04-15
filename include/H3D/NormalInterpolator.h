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
/// \file NormalInterpolator.h
/// \brief Header file for NormalInterpolator, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NORMALINTERPOLATOR_H__
#define __NORMALINTERPOLATOR_H__

#include <H3D/X3DInterpolatorNode.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class NormalInterpolator
  /// \brief The NormalInterpolator node interpolates among a list of normal
  /// vector sets specified by the keyValue field to produce an MFVec3f
  /// value_changed event. The output vector, value_changed, shall be a set
  /// of normalized vectors.
  ///
  /// Values in the keyValue field shall be of unit length. The number of
  /// normals in the keyValue field shall be an integer multiple of the
  /// number of key frames in the key field. That integer multiple defines
  /// how many normals will be contained in the value_changed events.
  ///
  /// Normal interpolation shall be performed on the surface of the unit
  /// sphere. That is, the output values for a linear interpolation from a
  /// point P on the unit sphere to a point Q also on the unit sphere shall
  /// lie along the shortest arc (on the unit sphere) connecting points P
  /// and Q. Also, equally spaced input fractions shall result in arcs of
  /// equal length. The results are undefined if P and Q are diagonally
  /// opposite. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NormalInterpolator.x3d">NormalInterpolator.x3d</a>
  ///     ( <a href="examples/NormalInterpolator.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NormalInterpolator.dot
  class H3DAPI_API NormalInterpolator : public X3DInterpolatorNode {
  public:
    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// routes_in[0] is the fraction_changed field
    /// routes_in[1] is the key field
    /// routes_in[2[ is the keyValue field
    struct H3DAPI_API MFValue : TypedField< MFVec3f, 
                                            Types< SFFloat, 
                                                   MFFloat, 
                                                   MFVec3f > >{
      virtual void update();
    };
#ifdef __BORLANDC__
    friend struct MFValue;
#endif

    /// Constructor.
    NormalInterpolator( Inst< SFNode  > _metadata      = 0,
                        Inst< SFFloat > _set_fraction  = 0,
                        Inst< MFFloat > _key           = 0,
                        Inst< MFVec3f > _keyValue      = 0,
                        Inst< MFValue > _value_changed = 0 );
    
    /// The values to interpolate between
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile NormalInterpolator_keyValue.dot
    auto_ptr< MFVec3f >  keyValue;

    /// The interpolated result value.
    /// 
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile NormalInterpolator_value_changed.dot
    auto_ptr< MFValue >  value_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
