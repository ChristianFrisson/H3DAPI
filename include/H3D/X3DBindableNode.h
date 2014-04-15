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
/// \file X3DBindableNode.h
/// \brief Header file for X3DBindableNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DBINDABLENODE_H__
#define __X3DBINDABLENODE_H__

#include <iostream>
#include <deque>
#include <H3D/X3DChildNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFTime.h>

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
  /// node of type X3DBindableNode:
  /// <ul>
  /// <li>
  /// During read, the first encountered X3DBindableNode is bound by pushing
  /// it to the top of the X3DBindableNode stack. The first encountered
  /// X3DBindableNode sends an isBound TRUE event.
  /// <li>
  /// When a set_bind TRUE event is received by a X3DBindableNode,
  /// <ol><li>
  /// If it is not on the top of the stack: the current top of stack node 
  /// sends an isBound FALSE event. The new node is moved to the top of the
  /// stack and becomes the currently bound X3DBindableNode. The new 
  /// X3DBindableNode (top of stack) sends an isBound TRUE event.
  /// <li>
  /// If the node is already at the top of the stack, this event has no effect.
  /// </ol>
  /// <li>
  /// When a set_bind FALSE event is received by a X3DBindableNode in the 
  /// stack, it is removed from the stack. If it was on the top of the stack,
  /// <ol><li>
  /// it sends an isBound FALSE event.
  /// <li>
  /// the next node in the stack becomes the currently bound X3DBindableNode
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
  class H3DAPI_API X3DBindableNode : public X3DChildNode {
  public:
    typedef std::deque< X3DBindableNode* > StackType;
    typedef map< string, StackType> StackMapType;

    /// The SFSetBind field calls toStackTop() and removeFromStack() on the 
    /// X3DBindableNode it is in depending on the value that it is set to.
    struct H3DAPI_API SFSetBind : public AutoUpdate< SFBool > {
      virtual inline void update() {
        SFBool::update();
        if( value )
          static_cast< X3DBindableNode * >( owner )->toStackTop();
        else 
          static_cast< X3DBindableNode * >( owner )->removeFromStack();
      }
      
      /// Set the value of the field.
      inline virtual void setValue( const bool &v, int id = 0 ) {
        SFBool::setValue( v, id );
        if( value )
          static_cast< X3DBindableNode * >( owner )->toStackTop();
        else 
          static_cast< X3DBindableNode * >( owner )->removeFromStack();
      }
    };

    
    /// Constructor.
    X3DBindableNode( const string &_bindable_stack_name,
                     Inst< SFSetBind > _set_bind = 0,
                     Inst< SFNode    > _metadata = 0,
                     Inst< SFTime    > _bindTime = 0,
                     Inst< SFBool    > _isBound  = 0 );

    /// Destructor.
    virtual inline ~X3DBindableNode() {
      removeFromStack();
    }

    /// Returns the active bindable instance, i.e. the instance on the
    /// top of the bindable stack.
    static X3DBindableNode * getActive( const string &bindable_stack_name ) {
      if ( !stack[bindable_stack_name].empty() ) 
        return stack[bindable_stack_name].front();
      else
        return 0;
    }

    /// Get the stack for a given name
    static const StackType &getStack( const string &bindable_stack_name );

    /// Return the map of all bindable nodes available.
    static const StackMapType &getStackMap();

    /// Initialize this bindable stack, toStackTop is done.
    virtual void initialize();

    /// Move this instance to the stack top. 
    virtual void toStackTop();

    /// Remove the bindable node from the stack.
    virtual void removeFromStack();

    /// Is this node at the top of the stack
    inline bool isStackTop() {
      const StackType &s = getStack( bindable_stack_name );
      if( s.size() > 0 ) return this == s.front();
      else return false;
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
    
    /// The H3DNodedatabase for this node
    static H3DNodeDatabase database;

  protected:
    string bindable_stack_name;
    /// The bindable stack.
    static StackMapType stack;
  };
}

#endif
