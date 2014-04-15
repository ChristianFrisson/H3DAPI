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
/// \file X3DSequencerNode.h
/// \brief Header file for X3DSequencerNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSEQUENCERNODE_H__
#define __X3DSEQUENCERNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFFloat.h>
#include <H3D/MFFloat.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DSequencerNode
  /// \brief This abstract node type is the base node type from which all 
  /// Sequencers are derived.
  ///
  /// The specified X3D sequencer nodes are designed for discrete events 
  /// along a timeline. Each of these nodes defines a piecewise-linear 
  /// function, f(t), on the interval (-infinity, +infinity).
  /// The piecewise-linear function is defined by n values of t, called key,
  /// and the n corresponding values of f(t), called keyValue. The keys shall
  /// be monotonically non-decreasing, otherwise the results are undefined.
  /// The keys are not restricted to any interval.
  ///
  /// Each of these nodes evaluates f(t) given any value of t 
  /// (via the fraction field) as follows:
  /// Let the n keys t0, t1, t2, ..., tn-1 partition the domain 
  /// (-infinity, +infinity) into the n+1 subintervals given by 
  /// (-infinity, t0), [t0, t1), [t1, t2), ... , [tn-1, +infinity).
  /// Also, let the n values v0, v1, v2, ..., vn-1 be the values of f(t) at
  /// the associated key values. The discrete value sequencing 
  /// function, f(t), is defined to be: 
  ///
  /// f(t) = vn, if tn <= t < tn-1
  ///      = v0, if t <= t0,
  ///      = vn-1, if t >= tn-1

  class H3DAPI_API X3DSequencerNode : public X3DChildNode {
  public:
    
    /// Each value in the keyValue field corresponds in order to the parameter
    /// value in the key field.
    template< class MType > class H3DAPI_API KeyValues : public MType {};

    /// ValueChanged is a specialized class used to evaluate and set the 
    /// value_changed field depending on the input to the class.
    ///
    /// routes_in[0] is the next field.
    /// routes_in[1] is the previous field.
    /// routes_in[2] is the set_fraction field.
    template< class TheType, class KeyValuesIn, class SequencerClass >
      class H3DAPI_API ValueChanged
      : public AutoUpdate< TypedField< TheType, 
                                       Types< SFBool, 
                                              SFBool, 
                                              SFFloat > > > {
    public:
      ValueChanged() { currentPosition = 0; fractionInitialized = false; }
      bool fractionInitialized;
      // The X3D specification never tells from where the next and previous
      // should start. Therefore I choose to start at the beginning
      // by default.
      H3DInt32 currentPosition;
    protected:
      // evaluate the value using set_fraction
      inline  typename TheType::value_type evaluateValueChanged(
        const typename KeyValuesIn::vector_type &key_value, 
        const vector< H3DFloat > &keys, 
        const H3DFloat &setFraction) {
        
        if( setFraction <= keys.front() )
          return key_value.front();
        else if( setFraction >= keys.back() )
          return key_value.back();
        else {
          int i;
          for( i = 0; 
            i < ( H3DInt32 )key_value.size() - 1 &&
            !( setFraction >= keys[i] && setFraction < keys[ i + 1 ] );
          ++i );
          return key_value[i];
        }
      }
      virtual void update() {
        if( TheType::routes_in[0] == TheType::event.ptr ||
            TheType::routes_in[1] == TheType::event.ptr ||
            TheType::routes_in[2] == TheType::event.ptr ) {
        
          SequencerClass * sn = 
            static_cast< SequencerClass * >( TheType::getOwner() );

          bool notMonotonically = false;

          const typename KeyValuesIn::vector_type &key_value = 
            sn->keyValue->getValue();

          const vector< H3DFloat > &keys = sn->key->getValue();

          if( keys.empty() ) {
            Console(3) << "Warning: The key array is empty in " <<
              "X3DSequencerNode ( " << sn->getName() << 
              " ). Node will not be used. " << endl;
            return;
          }

          if( key_value.empty() ) {
            Console(3) << "Warning: The keyValue array is empty in " <<
              "X3DSequencerNode ( " << sn->getName() << 
              " ). Node will not be used. " << endl;
            return;
          }

          if( keys.size() != key_value.size() ) {
            Console(3) << "Warning: The key and keyValue arrays mismatch in " <<
              "X3DSequencerNode ( " << sn->getName() << 
              " ). Node will not be used. " << endl;
            return;
          }

          // The keys shall be monotonically non-decreasing,
          // otherwise the results are undefined.
          for( int i = 0; i < (int)keys.size() - 1; ++i ) {
            if( keys[i] > keys[ i + 1 ] )
              notMonotonically = true;
          }

          if( notMonotonically ) {
            Console(3) << "Warning: The key array is not monotonically " <<
              "non-decreasing in X3DSequencerNode ( " << sn->getName() << 
              " ). Node will not be used. " << endl;
            return;
          }

          // next value
          if( TheType::routes_in[0] == TheType::event.ptr ) {
            ++currentPosition;
            if( currentPosition > ( H3DInt32 )key_value.size() - 1 )
              currentPosition = 0;

            TheType::value = key_value[ currentPosition ];
          }
          // previous value
          else if( TheType::routes_in[1] == TheType::event.ptr ) {
            --currentPosition;
            if( currentPosition < 0 )
              currentPosition = (H3DInt32) key_value.size() - 1;

            TheType::value = key_value[ currentPosition ];
          }
          // if set_fraction is used or anything else changes.
          else if( TheType::routes_in[2] == TheType::event.ptr ) {
            fractionInitialized = true;
            const H3DFloat &setFraction = 
              static_cast< SFFloat * >( TheType::routes_in[2] )->getValue( sn -> id );
            TheType::value = evaluateValueChanged(key_value, keys, setFraction);
          }
        }
      }
    };
#ifdef __BORLANDC__
    template< class TheType, class KeyValuesIn > friend class ValueChanged;
#endif

    /// Constructor.
    X3DSequencerNode(  Inst< SFNode           > _metadata      = 0,
                       Inst< SFBool           > _next          = 0,
                       Inst< SFBool           > _previous       = 0,
                       Inst< SFFloat          > _set_fraction  = 0,
                       Inst< MFFloat          > _key           = 0 );

    /// Receipt of next event triggers next output value in keyValue array,
    /// next goes to initial element after last.
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// <b>Default value:</b> - \n
    auto_ptr< SFBool >  next;

    /// Receipt of previous event triggers previous output value in keyValue
    /// array, previous goes to last element after first.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// <b>Default value:</b> - \n
    auto_ptr< SFBool >  previous;

    /// The set_fraction inputOnly field receives an SFFloat event and causes 
    /// the sequencing function to evaluate, resulting in a value_changed 
    /// output event with the same timestamp as the set_fraction event.
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// <b>Default value:</b> - \n
    auto_ptr< SFFloat >  set_fraction;

    /// Each value in the keyValue field corresponds in order to the parameter
    /// value in the key field. The keys shall be monotonically non-decreasing,
    /// otherwise the results are undefined. The keys are not restricted to 
    /// any interval. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    auto_ptr< MFFloat >  key;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
