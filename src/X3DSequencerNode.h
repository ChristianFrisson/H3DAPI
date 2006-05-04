//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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

#include "X3DChildNode.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "MFFloat.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class X3DSequencerNode
  /// \brief This abstract node type is the base node type from which all 
	/// Sequencers are derived.
	/// 
  /// \par Internal routes:
  /// \dotfile X3DSequencerNode.dot 

	class X3DSequencerNode : public X3DChildNode {
  public:
		
		/// Each value in the keyValue field corresponds in order to the parameter
		/// value in the key field.
		template< class MType > class KeyValues1 : public MType {};

		template< class TheType, class KeyValues > class ValueChanged
			: public TypedField < typename TheType, 
														Types< SFBool, 
															SFBool, 
															SFFloat, 
															MFFloat, 
															typename KeyValues > > {
		public:
			ValueChanged() { nextItem = 0; previousItem = -50; }
			H3DInt32 nextItem;
			H3DInt32 previousItem;
		protected:
			virtual void update() {
				X3DSequencerNode * sN = 
          static_cast< X3DSequencerNode * >( getOwner() );

				bool notMonotonically = false;

				const typename KeyValues::vector_type &key_value = 
					static_cast< typename KeyValues * >( routes_in[4] )->getValue();

				const vector< H3DFloat > &keys = 
					static_cast< MFFloat * >( routes_in[3] )->getValue();

				if( keys.size() != key_value.size() ) {
					Console(3) << "Warning: The key and keyValue arrays mismatch in " <<
						"X3DSequencerNode ( " << getName() << 
						"). Node will not be used. " << endl;
					return;
				}

				if( keys.empty() ) {
					Console(3) << "Warning: The key array is empty in " <<
						"X3DSequencerNode ( " << getName() << 
						"). Node will not be used. " << endl;
					return;
				}

				if( key_value.empty() ) {
					Console(3) << "Warning: The keyValue array is empty in " <<
						"X3DSequencerNode ( " << getName() << 
						"). Node will not be used. " << endl;
					return;
				}

				/// The keys shall be monotonically non-decreasing,
				/// otherwise the results are undefined.
				for( int i = 0; i < (int)keys.size() - 1; i++ ) {
					if( keys[i] > keys[ i + 1 ] )
						notMonotonically = true;
				}

				if( notMonotonically ) {
					Console(3) << "Warning: The key array is not monotonically " <<
						"non-decreasing in X3DSequencerNode ( " << getName() << 
						"). Node will not be used. " << endl;
					return;
				}
					
				if( routes_in[0] == event.ptr ) {
					nextItem++;
					if( nextItem > ( H3DInt32 )key_value.size() - 1 )
						nextItem = 0;

					value = key_value[ nextItem ];
				}
				else if( routes_in[1] == event.ptr ) {
					previousItem--;
					if( previousItem == -51 )
						previousItem = key_value.size() - 2;
					if( previousItem < 0 || previousItem > ( H3DInt32 )key_value.size() - 1 )
						previousItem = key_value.size() - 1;

					value = key_value[ previousItem ];
				}
				else if( routes_in[2] == event.ptr || routes_in[3] == event.ptr || routes_in[4] == event.ptr ) {
					const H3DFloat setFraction = 
						static_cast< SFFloat * >( routes_in[2] )->getValue( sN -> id );
					// evaluate the value using set_fraction
					if( setFraction <= keys.front() )
						value = key_value.front();
					else if( setFraction >= keys.back() )
						value = key_value.back();
					else {
						int i;
						for( i = 0; 
							i < ( H3DInt32 )key_value.size() - 1 &&
							!( setFraction >= keys[i] && setFraction < keys[ i + 1 ] );
						i++ );
						value = key_value[i];
					}
				}
			}
		};

    /// Constructor.
    X3DSequencerNode(	Inst< SFNode           > _metadata			= 0,
											Inst< SFBool	         > _next    			= 0,
											Inst< SFBool           > _previous 			= 0,
											Inst< SFFloat          > _set_fraction	= 0,
											Inst< MFFloat          > _key     			= 0 );
							 

		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile X3DSequencerNode_next.dot 
    auto_ptr< SFBool >  next;

		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile X3DSequencerNode_previous.dot 
    auto_ptr< SFBool >  previous;

		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile X3DSequencerNode_set_fraction.dot 
    auto_ptr< SFFloat >  set_fraction;

		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile X3DSequencerNode_key.dot 
    auto_ptr< MFFloat >  key;

		/// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
	};
}

#endif
