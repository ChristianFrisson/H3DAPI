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
/// \file OrientationInterpolator.h
/// \brief Header file for OrientationInterpolator, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ORIENTATIONINTERPOLATOR_H__
#define __ORIENTATIONINTERPOLATOR_H__

#include <H3D/X3DInterpolatorNode.h>
#include <H3D/SFRotation.h>
#include <H3D/MFRotation.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class OrientationInterpolator
  /// \brief The OrientationInterpolator node interpolates among a list of
  /// rotation values specified in the keyValue field to produce an SFRotation
  /// value_changed event.
  ///
  /// These rotations are absolute in object space and therefore are not
  /// cumulative. The keyValue field shall contain exactly as many rotations
  /// as there are key frames in the key field.
  /// 
  /// An orientation represents the final position of an object after a
  /// rotation has been applied. An OrientationInterpolator interpolates
  /// between two orientations by computing the shortest path on the unit
  /// sphere between the two orientations. The interpolation is linear in
  /// arc length along this path. The results are undefined if the two
  /// orientations are diagonally opposite.
  ///
  /// If two consecutive keyValue values exist such that the arc length
  /// between them is greater than pi, the interpolation will take place on
  /// the arc complement. For example, the interpolation between the
  /// orientations
  ///
  ///   (0, 1, 0, 0) and (0, 1, 0, 5.0)
  ///
  /// is equivalent to the rotation between the orientations
  ///
  /// (0, 1, 0, 2*pi) and (0, 1, 0, 5.0).
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/OrientationInterpolator.x3d">OrientationInterpolator.x3d</a>
  ///     ( <a href="examples/OrientationInterpolator.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile OrientationInterpolator.dot
  class H3DAPI_API OrientationInterpolator : public X3DInterpolatorNode {
  public:
    
    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// routes_in[0] is the fraction_changed field
    /// routes_in[1] is the key field
    /// routes_in[2[ is the keyValue field
    struct H3DAPI_API SFValue : TypedField< SFRotation, 
                                            Types< SFFloat, 
                                                   MFFloat,
                                                   MFRotation > >{
      virtual void update();
    };
#ifdef __BORLANDC__
    friend struct SFValue;
#endif

    /// Constructor.
    OrientationInterpolator( Inst< SFNode     > _metadata      = 0,
                             Inst< SFFloat    > _set_fraction  = 0,
                             Inst< MFFloat    > _key           = 0,
                             Inst< MFRotation > _keyValue      = 0,
                             Inst< SFValue    > _value_changed = 0 );

    /// The values to interpolate between.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile OrientationInterpolator_keyValue.dot
    auto_ptr< MFRotation >  keyValue;
    
    /// The resulting interpolated value.
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile OrientationInterpolator_value_changed.dot
    auto_ptr<  SFValue >  value_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
