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
/// \file X3DInterpolatorNode.h
/// \brief Header file for X3DInterpolatorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DINTERPOLATORNODE_H__
#define __X3DINTERPOLATORNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFFloat.h>
#include <H3D/MFFloat.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DInterpolatorNode
  /// \brief The abstract node X3DInterpolatorNode forms the basis for
  /// all types of interpolators specified in this clause.
  ///
  /// The key field contains the list of key times, which could appear as:
  ///
  ///    key [0 0.25 0.65 0.75 1]
  ///
  /// to indicate there are five key frames in this node. The keyValue field
  /// contains values for the target field, one complete set of values for
  /// each key. Interpolator nodes containing no keys in the key field shall
  /// not produce any events. However, an input event that replaces an empty
  /// key field with one that contains keys will cause the interpolator node
  /// to produce events the next time that a set_fraction event is
  /// received..
  ///
  /// The set_fraction inputOnly field receives an SFFloat event and causes
  /// the interpolator node function to evaluate, resulting in a
  /// value_changed output event of the specified type with the same
  /// timestamp as the set_fraction event.
  ///
  /// The contents of the keyValue and value_changed fields are dependent on
  /// the type of the node (e.g., the PositionInterpolator fields use
  /// MFVec3f values). Each value or set of values in the keyValue field
  /// corresponds in order to the parameter value in the key field.
  ///
  /// For interpolator nodes that produce a single value, results are
  /// undefined if the number of values in the key field is not the same as
  /// the number of values in the keyValue field.
  ///
  /// For Interpolator nodes that produce multiple values, the keyValue
  /// field is an nxm array of values, where n is the number of values in
  /// the key field and m is the number of values at each key frame. Each m
  /// values in the keyValue field correspond, in order, to a parameter
  /// value in the key field. Each value_changed event shall contain m
  /// interpolated values. Results are undefined if the number of values in
  /// the keyValue field is not a positive integer multiple of the number of
  /// values in the key field.
  ///
  /// If an X3DInterpolatorNode value_changed outputOnly field is read
  /// before it receives any inputs, keyValue[0] is returned if keyValue is
  /// not empty. If keyValue is empty (i.e., [ ]), the initial value for the
  /// respective field type is returned (EXAMPLE (0, 0, 0) for SFVec3f); see
  /// 5 Field type reference for initial event values.
  ///
  /// The location of an X3DInterpolatorNode in the transformation hierarchy
  /// has no effect on its operation. For example, if a parent of an
  /// interpolator node is a Switch node with whichChoice set to 1 (i.e.,
  /// ignore its children), the interpolator continues to operate as
  /// specified (receives and sends events).
  ///
  /// A typical simplified structure for a key frame animation
  /// implementation involves a TimeSensor, ROUTEs, and the target node.
  ///
  ///  Transform { 
  ///    Shape 
  ///       IndexedFaceSet { coordIndex='... 1 ... >
  ///           Coordinate DEF='Moved' point [ x y z, ... ] # t0Geometry
  ///       }                  
  ///    }
  ///  } 
  ///
  /// CoordinateInterpolator DEF='Mover'
  ///    key [t0 t1 t2 ]            #   list of key times, 0 to 1
  ///    keyValue ' x y z, ... '    #   one geometry per key time
  ///
  /// TimeSensor DEF='Timer' cycleInterval 5 loop TRUE 
  ///
  /// ROUTE Timer.fraction_changed TO Mover.set_value  
  /// ROUTE Mover.value_changed TO Moved.point 
  ///
  /// In typical operation, the key frame set_fraction event arrives from a
  /// TimeSensor to signal that the time value has advanced. This value varies
  /// from 0 to 1 depending upon where the TimeSensor is in its cycle time.
  /// For example, if the TimeSensor has a cycleTime of 10 seconds, and 5
  /// seconds has elapsed in its cycle, the set_fraction value will be 0.5.
  ///
  /// In this sample structure, the IndexedFaceSet contains a Coordinate field
  /// named Moved. This defines the time equals zero geometry for the node.
  /// The CoordinateInterpolator node named Mover contains the list of key
  /// frame times and the corresponding sets of coordinates in the keyValue
  /// field. When the set_fraction event arrives for key, the corresponding 
  /// interpolated keyValue is sent to the target Coordinate node for
  /// rendering.
  class H3DAPI_API X3DInterpolatorNode : public X3DChildNode {
  public:
    
    /// Constructor.
    X3DInterpolatorNode( Inst< SFNode  > _metadata     = 0,
                         Inst< SFFloat > _set_fraction = 0,
                         Inst< MFFloat > _key          = 0 );

    /// Utility function for Interpolator nodes to be able to find the index
    /// for the key pair to interpolate between for a given fraction value f. 
    /// The weighting of f between the key pair is set in w.
    int lookupKey( H3DFloat f, H3DFloat &w ) {
      vector<H3DFloat> keys = key->getValue();
      if( keys.size() == 0 ) return -1;

      if ( keys.size() == 1 || f <= keys[0] ) {
        w = 0;
        return 0;
      }
      if ( f >= keys[keys.size()-1] ) {
        w = 1;
        return (int)keys.size() - 2;
      }

      // we know here that f is larger than keys[0] so we know that we will
      // go into the if-statement at some point.
      for( size_t i = keys.size()-1; true; --i) {
        if ( f > keys[i] ) {
          w = (f-keys[i])/(keys[i+1]-keys[i]);
          if( w < 0 ) w = 0;
          if( w > 1 ) w = 1;
          return (int) i;
        }
      }
      return -1;  // something must have gone wrong to get here
    }

    /// The set_fraction inputOnly field receives an SFFloat event and causes
    /// the interpolator node function to evaluate, resulting in a
    /// value_changed output event of the specified type with the same
    /// timestamp as the set_fraction event.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< SFFloat > set_fraction;

    /// The key field contains the list of key times, which could appear as:
    /// key [0 0.25 0.65 0.75 1]
    /// to indicate there are five key frames in this node.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFFloat > key;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
