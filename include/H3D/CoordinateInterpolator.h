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
/// \file CoordinateInterpolator.h
/// \brief UNIMPLEMENTED: Header file for CoordinateInterpolator, 
///  X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COORDINATEINTERPOLATOR_H__
#define __COORDINATEINTERPOLATOR_H__

#include <H3D/X3DInterpolatorNode.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class CoordinateInterpolator
  /// \brief This node linearly interpolates among a
  /// list of MFVec3f values to produce an MFVec3f value_changed
  /// event.
  ///
  /// The number of coordinates in the keyValue field shall be an integer
  /// multiple of the number of key frames in the key field. That integer
  /// multiple defines how many coordinates will be contained in the
  /// value_changed events. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/CoordinateInterpolator.x3d">CoordinateInterpolator.x3d</a>
  ///     ( <a href="examples/CoordinateInterpolator.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile CoordinateInterpolator.dot
  class H3DAPI_API CoordinateInterpolator : public X3DInterpolatorNode {
  public:

    /// The SFValue field updates the interpolated value from the fields
    /// routed to it.
    ///
    /// routes_in[0] is the fraction_changed field
    /// routes_in[1] is the key field
    /// routes_in[2[ is the keyValue field    
    struct MFValue : TypedField< MFVec3f, Types< SFFloat, MFFloat, MFVec3f > > {
      virtual void update() {
        CoordinateInterpolator *interpolator = 
            static_cast<CoordinateInterpolator*>( getOwner() );
        H3DFloat fraction = static_cast<SFFloat*>(routes_in[0])->getValue(interpolator->id);
        int key_size = static_cast<MFFloat*>(routes_in[1])->size();
        H3DFloat weight;
        int key_index = static_cast<CoordinateInterpolator*>(owner)->lookupKey( fraction, weight );
        vector< Vec3f > key_values = static_cast<MFVec3f*>(routes_in[2])->getValue();
        int value_size = 0;
        if( key_size != 0 )
          value_size = (int) key_values.size() / key_size;
        value.resize( value_size );

  if ( key_index >= 0 && 
       (key_index + 2)* value_size - 1 < (int)key_values.size() ) {
          if (weight<=0) 
            for (int x = 0; x < value_size; ++x )
              value[x] = key_values[ key_index*value_size + x ];
          else if (weight>=1)
            for (int x = 0; x < value_size; ++x )
              value[x] = key_values[ (key_index+1)*value_size + x];
          else { // else, interpolate linearly
            for (int x = 0; x < value_size; ++x ) {
              Vec3f a = key_values[ key_index*value_size + x ];
              Vec3f b = key_values[ (key_index+1)*value_size + x  ];
              value[ x ] = (1-weight)*a + (weight)*b;
            }
          }
        }
      }
    };
#ifdef __BORLANDC__
    friend struct MFValue;
#endif
    
    /// Constructor.
    CoordinateInterpolator( Inst< SFNode  >  _metadata      = 0,
                            Inst< SFFloat >  _set_fraction  = 0,
                            Inst< MFFloat >  _key           = 0,
                            Inst< MFVec3f >  _keyValue      = 0,
                            Inst< MFValue >  _value_changed = 0 );

    /// The values to interpolate between.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// \dotfile CoordinateInterpolator_keyValue.dot
    auto_ptr< MFVec3f > keyValue;

    /// The linearly interpolated result value.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile CoordinateInterpolator_value_changed.dot
    auto_ptr< MFValue > value_changed;

    /// The H3DNodeDatabase for the node.
    static H3DNodeDatabase database;
  };
}

#endif
