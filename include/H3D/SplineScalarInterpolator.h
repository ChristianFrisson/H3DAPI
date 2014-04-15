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
/// \file SplineScalarInterpolator.h
/// \brief Header file for SplineScalarInterpolator, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPLINESCALARINTERPOLATOR_H__
#define __SPLINESCALARINTORPOLATOR_H__

#include <H3D/X3DInterpolatorNode.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFBool.h>
#include <H3DUtil/H3DMath.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class SplineScalarInterpolator
  /// \brief The SplineScalarInterpolator node non-linearly interpolates 
  ///  among a list of floats to produce an SFFloat value_changed event.
  ///
  /// The keyValue, keyVelocity, and key fields shall each have the same
  /// number of values.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SplineScalarInterpolator.x3d">SplineScalarInterpolator.x3d</a>
  ///     ( <a href="examples/SplineScalarInterpolator.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SplineScalarInterpolator.dot

  class H3DAPI_API SplineScalarInterpolator : public X3DInterpolatorNode {
  public:
    
    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// - routes_in[0] is the fraction_changed field
    /// - routes_in[1] is the key field
    /// - routes_in[2] is the keyValue field
    /// - routes_in[3] is the keyVelocity field
    struct H3DAPI_API SFValue : TypedField< SFFloat, 
                                            Types< SFFloat, 
                                                   MFFloat, 
                                                   MFFloat,
                                                   MFFloat> >{
      virtual void update();
    };
#ifdef __BORLANDC__
    friend struct SFValue;
#endif

    /// Construtor.
    SplineScalarInterpolator( Inst< SFNode  >  _metadata      = 0,
                        Inst< SFFloat >  _set_fraction  = 0,
                        Inst< MFFloat >  _key           = 0,
                        Inst< MFFloat >  _keyValue      = 0,
                        Inst< MFFloat >  _keyVelocity   = 0,
                        Inst< SFValue >  _value_changed = 0,
                        Inst< SFBool  >  _closed    = 0,
                        Inst< SFBool  >  _normalizeVelocity = 0);

    /// keyValues are the values to interpolate between
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile SplineScalarInterpolator_keyValue.dot
    auto_ptr< MFFloat > keyValue;

    /// KeyVelocity, if left empty they are calculated using the keyValues.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile SplineScalarInterpolator_keyVelocity.dot
    auto_ptr< MFFloat > keyVelocity;

    /// Closed is a boolean that specifies if the loop 
    /// is open or closed
    /// 
    /// <b>Default value:</b> SFBool = FALSE \n
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile SplineScalarInterpolator_closed.dot
    auto_ptr< SFBool > closed;

    /// NormalizeVelocity is a boolean that specifies 
    /// if the keyVelocities should be normalized
    /// 
    /// <b>Default value:</b> SFBool = FALSE \n
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile SplineScalarInterpolator_normalizeVelocity.dot
    auto_ptr< SFBool > normalizeVelocity;

    /// The interpolated result value.
    /// 
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile SplineScalarInterpolator_value_changed.dot
    auto_ptr< SFValue > value_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
