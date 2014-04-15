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
/// \file EaseInEaseOut.h
/// \brief Header file for EaseInEaseOut, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __EASEINEASEOUT_H__
#define __EASEINEASEOUT_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFFloat.h>
#include <H3D/MFFloat.h>
#include <H3D/MFVec2f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class EaseInEaseOut
  /// \brief The EaseInEaseOut node supports controlled gradual 
  /// transitions by specifying modifications for TimeSensor node 
  /// fractions. The EaseInEaseOut node receives set_fraction field
  /// event. It uses the values of the key field and the easeIneaseOut
  /// field to modify that fraction which is then issued as a 
  /// modifiedFraction_changed event.
  ///
  /// The easeInEaseOut field shall have the same size as the key
  /// field, and each Vec2f value of the easeInEaseOut field 
  /// corresponds to the key value with the same index. The first 
  /// component of each easeInEaseOut field Vec2f value corresponds 
  /// to the easeIn feature as the interpolator approaches the key, 
  /// and the second component of each easeInEaseOut field Vec2f 
  /// value corresponds to the easeOut feature as the interpolator 
  /// progresses away from the key.
  ///
  /// The values of the easeInEaseOut field range from zero to one. 
  /// At zero, there is no modification of the fraction.
  ///
  /// The scope of the easeOut effect on the local fraction is equal 
  /// to the easeOut value.
  ///
  /// \par Internal routes:
  /// \dotfile EaseInEaseOut.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/EaseInEaseOut.x3d">EaseInEaseOut.x3d</a>
  ///     ( <a href="examples/EaseInEaseOut.x3d.html">Source</a> )
  class H3DAPI_API EaseInEaseOut : public X3DChildNode {
  public:

    /// Thrown when number of field values in key and easeInEaseOut
    /// are not equal
    H3D_API_EXCEPTION( UnequalFieldValues );

    /// Class for modified fraction_changed field type
    ///
    /// routes_in[0] is the set_fraction field
    /// routes_in[1] is the key field
    /// routes_in[2] is the easeInEaseOutField
    class H3DAPI_API SFFraction : public TypedField< SFFloat, 
                              Types<SFFloat, MFFloat, MFVec2f> > {
    protected:
      virtual void update();
    };

    /// Constructor.
    EaseInEaseOut( Inst< SFFloat    > _set_fraction    = 0,
                   Inst< MFVec2f    > _easeInEaseOut = 0,
                   Inst< MFFloat    > _key = 0,
                   Inst< SFNode     > _metadata = 0,
                   Inst< SFFraction > _modifiedFraction_changed = 0 );

    /// Incoming fraction field values routed from TimeSensor node.
    /// 
    /// <b>Access type:</b> inputOnly
    /// 
    /// \dotfile EaseInEaseOut_set_fraction.dot
    auto_ptr< SFFloat > set_fraction;

    /// The proportion of time at which ease-in and -out effect should
    /// take place for the corresponding key value. The first component 
    /// of this Vec2f field value corresponds to the easeIn feature as 
    /// the interpolator approaches the key. The second component of 
    /// corresponds to the easeOut feature as the interpolator progresses 
    /// away from the key. The range of value is 0.0 to 1.0. The number 
    /// of field values must be same as that of the key field.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> []
    /// 
    /// \dotfile EaseInEaseOut_easeInEaseOut.dot
    auto_ptr< MFVec2f > easeInEaseOut;

    /// The keypoints for interpolation.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> []
    /// 
    /// \dotfile EaseInEaseOut_key.dot
    auto_ptr< MFFloat > key;

    /// Output fraction as modified from specified key and 
    /// easeInEaseOut values.
    /// 
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile EaseInEaseOut_modifiedFraction_changed.dot
    auto_ptr< SFFloat > modifiedFraction_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Utility function to find the index for the key pair 
    /// for a given fraction value f. The proportion of distance
    /// between the key pair at f is set in p.
    int lookupKey( H3DFloat f, H3DFloat &p );

    /// Check that easeInEaseOut and key have the same number of
    /// field values, and print error message if they are not equal.
    void checkKeyEaseInEaseOut();
  };
}

#endif
