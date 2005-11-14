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
/// \file X3DBindableNode.h
/// \brief Header file for X3DBindableNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DBINDABLENODE_H__
#define __X3DBINDABLENODE_H__

#include <iostream>
#include <deque>
#include "X3DChildNode.h"
#include "FieldTemplates.h"

#ifdef _MSC_VER
// disable warnings about not finding the definition of the static
// stack member. It is included by instances of X3DBindableNode by
// including the X3DBindableNode.cpp file in the .cpp file of the class
// using it.
#pragma warning( disable: 4661 )
#endif

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DBindableNode
  /// \brief X3DBindableNode is the abstract base type for all bindable 
  /// children nodes, including Background, TextureBackground, Fog, 
  /// NavigationInfo, Viewpoint and DeviceInfo. 
  ///
  /// These nodes have the unique behaviour that only one of each type can be
  /// bound (i.e., affect the user's experience) at any instant in time. The 
  /// browser shall maintain an independent, separate stack for each type of
  /// bindable node. Each of these nodes includes a set_bind inputOnly field
  /// and an isBound outputOnly field. The set_bind inputOnly field is used to
  /// move a given node to and from its respective top of stack. A TRUE value
  /// sent to the set_bind inputOnly field moves the node to the top of the
  /// stack; sending a FALSE value removes it from the stack. The isBound
  /// event is output when a given node is:
  /// - moved to the top of the stack;
  /// - removed from the top of the stack;
  /// - pushed down from the top of the stack by another node being placed
  /// on top.
  ///
  /// That is, isBound events are sent when a given node becomes, or ceases 
  /// to be, the active node. The node at the top of the stack (the most
  /// recently bound node) is the active node for its type and is used by the
  /// browser to set the world state. If the stack is empty (i.e., either the
  /// X3D file has no bindable nodes for a given type or the stack has been
  /// popped until empty), the default field values for that node type are
  /// used to set world state. The results are undefined if a multiply 
  /// instanced (DEF/USE) bindable node is bound.
  ///
  /// The following rules describe the behaviour of the binding stack for a
  /// node of type <bindable node>:
  /// <ul>
  /// <li>
  /// During read, the first encountered <bindable node> is bound by pushing
  /// it to the top of the <bindable node> stack. Nodes contained within files
  /// referenced by Inline nodes, within the strings passed to the 
  /// X3D::createX3DFromString() method, or within X3D files passed to the
  /// X3D::createX3DFromURL() method are not candidates for the first 
  /// encountered <bindable node>. The first node within a locally defined 
  /// prototype instance is a valid candidate for the first encountered <
  /// bindable node>. The first encountered <bindable node> sends an isBound
  /// TRUE event.
  /// <li>
  /// When a set_bind TRUE event is received by a <bindable node>,
  /// <ol><li>
  /// If it is not on the top of the stack: the current top of stack node 
  /// sends an isBound FALSE event. The new node is moved to the top of the
  /// stack and becomes the currently bound <bindable node>. The new 
  /// <bindable node> (top of stack) sends an isBound TRUE event.
  /// <li>
  /// If the node is already at the top of the stack, this event has no effect.
  /// </ol>
  /// <li>
  /// When a set_bind FALSE event is received by a <bindable node> in the 
  /// stack, it is removed from the stack. If it was on the top of the stack,
  /// <ol><li>
  /// it sends an isBound FALSE event.
  /// <li>
  /// the next node in the stack becomes the currently bound <bindable node>
  /// (i.e., pop) and issues an isBound TRUE event. 
  /// </ol>
  /// <li>
  /// If a set_bind FALSE event is received by a node not in the stack, the
  /// event is ignored and isBound events are not sent.
  /// <li>
  /// When a node replaces another node at the top of the stack, the isBound
  /// TRUE and FALSE output events from the two nodes are sent simultaneously
  /// (i.e., with identical timestamps).
  /// <li>
  /// If a bound node is deleted, it behaves as if it received a set_bind
  /// FALSE event (see f above). 
  /// </ul>
  /// The results are undefined if a bindable node is bound and is the child
  /// of an LOD, Switch, or any node or prototype that disables its children. 
  template< class T >
  class X3DBindableNode : public X3DChildNode {
  public:
    
    
    struct SFSetBind : SFBool {
      virtual inline void update() {
        static_cast< X3DBindableNode * >( owner )->makeActive();
      }
    };

    
    /// Constructor.
    inline X3DBindableNode( 
                           Inst< SFSetBind > _set_bind = 0,
                           Inst< SFNode    > _metadata = 0,
                           Inst< SFTime    > _bindTime = 0,
                           Inst< SFBool    > _isBound  = 0 ) :
      X3DChildNode( _metadata ),
      set_bind( _set_bind ),
      bindTime( _bindTime ),
      isBound ( _isBound  ) {

      set_bind->setName( "set_bind");
      bindTime->setName( "bindTime" );
      isBound->setName( "isBound" );

      set_bind->setOwner( this );
      bindTime->setOwner( this );
      isBound->setOwner( this );
      
      set_bind->setAccessType( Field::INPUT_ONLY );
      bindTime->setAccessType( Field::OUTPUT_ONLY );
      isBound->setAccessType( Field::OUTPUT_ONLY );

      if ( stack.empty() ) {
        stack.push_front( static_cast< T * >( this ) );
        isBound->setValue( true, id );
      }
                 
    }

    /// Destructor.
    virtual inline ~X3DBindableNode() {
      if ( getActive() == this ) {
        stack.pop_front();
        T *active = getActive();
        if( active ) 
          active->isBound->setValue( true, id );
      } else {
        for( typename StackType::iterator i = stack.begin();
             i != stack.end(); i++ )
          if ( (*i) == this ) {
            stack.erase( i );
            i = stack.end();
          }
      }
    }

    /// Input field to bind or unbind the node.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// <b>Valid range:</b> TRUE or FALSE
    auto_ptr< SFSetBind >  set_bind;
    
    /// The bindTime field is the time at which the Viewpoint node
    /// is bound or unbound. 
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFTime    >  bindTime;

    /// Output field telling if this node is bound or not.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFBool    >  isBound;

    /// Returns the active bindable instance, i.e. the instance on the
    /// top of the bindable stack.
    static inline T * getActive() {
      if ( !stack.empty() ) 
        return stack.front();
      else
        return 0;
    }
    
    /// Make this instance the active node. 
    inline void makeActive() {
      T *active = X3DBindableNode<T>::getActive();
      if ( active != this ) {
        // remove this from the stack, if it was in the stack...
        for( typename StackType::iterator i = stack.begin();
             i != stack.end(); i++ )
          if ( (*i) == this ) {
            stack.erase( i );
            i = stack.end();
          }
        // and place it on the top of the stack
        stack.push_front( static_cast< T* >( this ) );
        isBound->setValue( true, id );
        /// TODO: FIX
        active->isBound->setValue( false );
      }
    }

  protected:
    typedef deque< T* > StackType;
    /// The bindable stack.
    static StackType stack;
  };
}

#endif
