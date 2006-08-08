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
/// \file X3DGroupingNode.cpp
/// \brief CPP file for X3DGroupingNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DGroupingNode.h"
#include "H3DRenderStateObject.h"
#include "MatrixTransform.h"

using namespace H3D;

H3DNodeDatabase X3DGroupingNode::database( 
        "X3DGroupingNode", 
        NULL,
        typeid( X3DGroupingNode ),
        &X3DChildNode::database 
        );

namespace X3DGroupingNodeInternals {
  FIELDDB_ELEMENT( X3DGroupingNode, addChildren, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DGroupingNode, removeChildren, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DGroupingNode, children, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DGroupingNode, bboxCenter, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( X3DGroupingNode, bboxSize, INITIALIZE_ONLY );
}

X3DGroupingNode::X3DGroupingNode( Inst< MFChild >  _addChildren,
                                  Inst< MFChild >  _removeChildren,
                                  Inst< MFChild >  _children,
                                  Inst< SFNode  >  _metadata,
                                  Inst< SFBound >  _bound,
                                  Inst< SFVec3f >  _bboxCenter,
                                  Inst< SFVec3f >  _bboxSize ) :
  X3DChildNode( _metadata ),
  X3DBoundedObject( _bound, _bboxCenter, _bboxSize ),
  use_union_bound( false ),
  addChildren   ( _addChildren    ),
  removeChildren( _removeChildren ),
  children      ( _children       ) {

  type_name = "X3DGroupingNode";
  database.initFields( this );

  displayList->setOwner( this );
  bound->setOwner( this );

  bound->setValue( new EmptyBound );
  children->route( displayList );
}

void X3DGroupingNode::render()     { 
  X3DChildNode::render();
  
  for( MFNode::const_iterator i = children->begin();
       i != children->end(); i++ ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->enableGraphicsState();
    }
  }
  
  //cerr << "X3DGroupingNode::render() children" << endl;
  for( MFNode::const_iterator i = children->begin();
       i != children->end(); i++ ) {
    H3DDisplayListObject *c = dynamic_cast< H3DDisplayListObject* >( *i );
    if ( c ) {
      c->displayList->callList();
    } else {
      //if(*i)
        (*i)->render();
    }
  }

  for( MFNode::const_iterator i = children->begin();
       i != children->end(); i++ ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->disableGraphicsState();
    }
  }

};

void X3DGroupingNode::traverseSG( TraverseInfo &ti ) {
  for( MFNode::const_iterator i = children->begin();
       i != children->end(); i++ ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->enableHapticsState( ti );
    }
  }
  // not using iterators since they can become invalid if the 
  // traversal changes the children field while iterating.
  const NodeVector &c = children->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] )
      c[i]->traverseSG( ti );
  }

  for( MFNode::const_reverse_iterator i = children->rbegin();
       i != children->rend(); i++ ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->disableHapticsState( ti );
    }
  }
}

void X3DGroupingNode::SFBound::update() {
  value = Bound::SFBoundUnion( routes_in.begin(),
                               routes_in.end() );
}

void X3DGroupingNode::MFChild::onAdd( Node *n ) {
  MFChildBase::onAdd( n );
  X3DChildNode *c = static_cast< X3DChildNode* >( n );
  X3DGroupingNode *o = static_cast< X3DGroupingNode* >( owner );
  if ( c ) {
    //c->displayList->route( o->displayList );
    if( o->use_union_bound ) {
      H3DBoundedObject *bo = 
        dynamic_cast< H3DBoundedObject * >( n );
      if( bo ) {
        MatrixTransform *t = dynamic_cast< MatrixTransform *>( n );
        if( t ) {
          t->transformedBound->route( o->bound );
        } else {
          bo->bound->route( o->bound );
        }
      }
    }
  }
}

void X3DGroupingNode::MFChild::onRemove( Node *n ) {
  X3DChildNode *c = static_cast< X3DChildNode* >( n );
  X3DGroupingNode *o = static_cast< X3DGroupingNode* >( owner );
  if ( c ) {
    //c->displayList->unroute( o->displayList );
    if( o->use_union_bound ) {
      H3DBoundedObject *bo = 
        dynamic_cast< H3DBoundedObject * >( n );
      if( bo ) {
        MatrixTransform *t = dynamic_cast< MatrixTransform *>( n );
        if( t ) {
          t->transformedBound->route( o->bound );
        } else {
          bo->bound->unroute( o->bound );
        }
      }
    }
  }
  MFChildBase::onRemove( n );
}
