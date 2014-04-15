
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
/// \file X3DBindableNode.cpp
/// \brief CPP file for X3DBindableNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DBindableNode.h>

using namespace H3D;

X3DBindableNode::StackMapType X3DBindableNode::stack;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DBindableNode::database( 
            "X3DBindableNode", 
            NULL, 
            typeid( X3DBindableNode ),
            &X3DChildNode::database );

namespace X3DBindableNodeInternals {
  FIELDDB_ELEMENT( X3DBindableNode, set_bind, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DBindableNode, isBound, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DBindableNode, bindTime, OUTPUT_ONLY );
}


/// Constructor.
X3DBindableNode::X3DBindableNode( const string &_bindable_stack_name,
                                  Inst< SFSetBind > _set_bind,
                                  Inst< SFNode    > _metadata,
                                  Inst< SFTime    > _bindTime,
                                  Inst< SFBool    > _isBound ) :
  X3DChildNode( _metadata ),
  set_bind( _set_bind ),
  bindTime( _bindTime ),
  isBound ( _isBound  ),
  bindable_stack_name( _bindable_stack_name ) {
  
  type_name = "X3DBindableNode";
  database.initFields( this );
}

void X3DBindableNode::initialize() {
  X3DChildNode::initialize();
  StackType &s = stack[bindable_stack_name];
  if ( s.size() == 0 ) {
    toStackTop();
  }
}

void X3DBindableNode::removeFromStack() {
  StackType &s =  stack[bindable_stack_name];
  if( s.size() > 0 ) {
    bool is_active = (s.front() == this);
    
    if ( is_active ) {
      s.pop_front();
      isBound->setValue( false, id );
      if( !s.empty() ) {
        X3DBindableNode *new_top = s.front();
        new_top->isBound->setValue( true, new_top->id );
        new_top->bindTime->setValue( TimeStamp(), new_top->id );
      }
    } else {
      for( StackType::iterator i = s.begin();
           i != s.end(); ++i ) {
        if ( (*i) == this ) {
          s.erase( i );
          isBound->setValue( false, id );
          break;
        }
      }
    }
  }
}

void X3DBindableNode::toStackTop() {
  StackType &s =  stack[bindable_stack_name];
  X3DBindableNode *active = NULL;
  if( s.size() > 0 ) active = s.front();
  if ( active != this ) {
    // remove this from the stack, if it was in the stack...
    for( StackType::iterator i = s.begin();
         i != s.end(); ++i ) {
      if ( (*i) == this ) {
        s.erase( i );
        break;
      }
    }
    
    // and place it on the top of the stack
    s.push_front( this );
    isBound->setValue( true, id );
    bindTime->setValue( TimeStamp(), id );
    if( active )
      active->isBound->setValue( false, active->id );
  }
}


 /// Return the map of all bindable nodes available.
const X3DBindableNode::StackMapType &X3DBindableNode::getStackMap() {
  return stack;
}

const X3DBindableNode::StackType &X3DBindableNode::getStack
( const string &bindable_stack_name ) {
  return stack[ bindable_stack_name ];
}
