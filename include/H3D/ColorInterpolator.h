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
/// \file ColorInterpolator.h
/// \brief Header file for ColorInterpolator, X3D 
/// scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COLORINTERPOLATOR_H__
#define __COLORINTERPOLATOR_H__


#include <H3D/X3DInterpolatorNode.h>
#include <H3D/SFColor.h>
#include <H3D/MFColor.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ColorInterpolator
  /// \brief This node interpolates among a list of
  /// MFColor key values to produce an SFColor (RGB) value_changed
  /// event.
  ///
  /// The number of colours in the keyValue field shall be equal to the
  /// number of key frames in the key field. The keyValue field and
  /// value_changed events are defined in RGB colour space. A linear
  /// interpolation using the value of set_fraction as input is performed in
  /// HSV space (see [FOLEY] for description of RGB and HSV colour
  /// spaces). The results are undefined when interpolating between two
  /// consecutive keys with complementary hues.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ColorInterpolator.x3d">ColorInterpolator.x3d</a>
  ///     ( <a href="examples/ColorInterpolator.x3d.html">Source</a> )
  /// 
  /// \par Internal routes:
  /// \dotfile ColorInterpolator.dot
  class H3DAPI_API ColorInterpolator : public X3DInterpolatorNode {
  public:
    
    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// routes_in[0] is the fraction_changed field
    /// routes_in[1] is the key field
    /// routes_in[2[ is the keyValue field
    struct H3DAPI_API SFValue : TypedField< SFColor, 
                                            Types< SFFloat, 
                                                   MFFloat, 
                                                   MFColor > >{
      /// Function to convert from RGB to HSV color space.
      Vec3f RGBToHSV( const RGB &rgb );

      /// Function to convert from HSV to RGB color space.
      RGB HSVToRGB( const Vec3f &rgb );
      
      virtual void update();
    };
#ifdef __BORLANDC__
    friend class SFValue;
#endif

    /// Constructor.
    ColorInterpolator( Inst< SFNode  > _metadata      = 0,
                       Inst< SFFloat > _set_fraction  = 0,
                       Inst< MFFloat > _key           = 0,
                       Inst< MFColor > _keyValue      = 0,
                       Inst< SFValue > _value_changed = 0 );

    /// The values to interpolate between
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile ColorInterpolator_keyValue.dot
    auto_ptr< MFColor >  keyValue;

    /// The interpolated result value.
    /// 
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile ColorInterpolator_value_changed.dot
    auto_ptr< SFValue > value_changed;    

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
