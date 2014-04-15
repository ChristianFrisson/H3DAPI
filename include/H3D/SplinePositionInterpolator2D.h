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
/// \file SplinePositionInterpolator2D.h
/// \brief Header file for SplinePositionInterpolator2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPLINEPOSITIONINTERPOLATOR2D_H__
#define __SPLINEPOSITIONINTERPOLATOR2D_H__

#include <H3D/X3DInterpolatorNode.h>
#include <H3D/SFVec2f.h>
#include <H3D/MFVec2f.h>
#include <H3D/SFBool.h>
#include <H3DUtil/H3DMath.h>


namespace H3D {

  /// \ingroup X3DNodes
  /// \class SplinePositionInterpolator2D
  /// \brief The SplinePositionInterpolator2D node non-linearly interpolates 
  ///  among a list of 2D vectors to produce an SFVec2f value_changed event. 
  ///
  ///  The keyValue, keyVelocity, and key fields shall each have the same number of values.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SplinePositionInterpolator2D.x3d">SplinePositionInterpolator2D.x3d</a>
  ///     ( <a href="examples/SplinePositionInterpolator2D.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SplinePositionInterpolator2D.dot

  class H3DAPI_API SplinePositionInterpolator2D : public X3DInterpolatorNode {
  public:

    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// - routes_in[0] is the fraction_changed field
    /// - routes_in[1] is the key field
    /// - routes_in[2] is the keyValue field
    /// - routes_in[3] is the keyVelocity field
    struct H3DAPI_API SFValue : TypedField< SFVec2f, 
      Types< SFFloat, 
      MFFloat, 
      MFVec2f,
      MFVec2f> >{
        virtual void update();
    };
#ifdef __BORLANDC__
    friend struct SFValue;
#endif

    /// Construtor.
    SplinePositionInterpolator2D( 
      Inst< SFNode  >  _metadata      = 0,
      Inst< SFFloat >  _set_fraction  = 0,
      Inst< MFFloat >  _key           = 0,
      Inst< MFVec2f >  _keyValue      = 0,
      Inst< MFVec2f >  _keyVelocity   = 0,
      Inst< SFValue >  _value_changed = 0,
      Inst< SFBool  >  _closed    = 0,
      Inst< SFBool  >  _normalizeVelocity = 0);

    /// The values to interpolate between.
    /// 
    /// 
    /// <b>Access type:</b> inputOutput
    /// \dotfile SplinePositionInterpolator2D_keyValue.dot
    auto_ptr< MFVec2f > keyValue;
    
    /// KeyVelocity values, if left empty they are calculated using the keyValues.
    ///
    /// <b>Access type:</b> inputOutput
    ///
    /// \dotfile SplinePositionInterpolator2D_keyVelocity.dot
    auto_ptr< MFVec2f > keyVelocity;
   
    /// Specifies if the loop is Open or Closed.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> FALSE \n
    ///
    /// \dotfile SplinePositionInterpolator2D_closed.dot
    auto_ptr< SFBool > closed;
    

    /// Specifies if the Velocity should be normalized.
    ///
    /// <b>Default value:</b> FALSE \n
    /// <b>Access type:</b> inputOutput
    ///
    /// \dotfile SplinePositionInterpolator2D_normalizeVelocity.dot
    auto_ptr< SFBool > normalizeVelocity;
    

    /// The interpolated result value.
    /// 
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile SplinePositionInterpolator2D_value_changed.dot
    auto_ptr< SFValue > value_changed;


    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
