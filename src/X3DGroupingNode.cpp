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
/// \file X3DGroupingNode.cpp
/// \brief CPP file for X3DGroupingNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DGroupingNode.h>
#include <H3D/H3DRenderStateObject.h>
#include <H3D/MatrixTransform.h>
#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/ClipPlane.h>

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

X3DGroupingNode::X3DGroupingNode( Inst< AddChildren    > _addChildren,
                                  Inst< RemoveChildren > _removeChildren,
                                  Inst< MFChild        > _children,
                                  Inst< SFNode         > _metadata,
                                  Inst< SFBound        > _bound,
                                  Inst< SFVec3f        > _bboxCenter,
                                  Inst< SFVec3f        > _bboxSize ) :
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
  // skip rendering if none of the children are transparent(in multi-pass
  // transparency mode) and the render pass is a pass to render transparent
  // objects.
  if( X3DShapeNode::geometry_render_mode != X3DShapeNode::SOLID && 
      X3DShapeNode::geometry_render_mode != X3DShapeNode::ALL && 
      !children_multi_pass_transparency ) {
    return;
  }

  X3DChildNode::render();
  
  for( MFNode::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->enableGraphicsState();
    }
  }

  // not using iterators since they can become invalid if the 
  // traversal changes the children field while iterating.
  const NodeVector &c = children->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DDisplayListObject *tmp = dynamic_cast< H3DDisplayListObject* >( c[i]);
      if( tmp )
        tmp->displayList->callList();
      else
        c[i]->render();
    }
  }

  for( MFNode::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->disableGraphicsState();
    }
  }

};

void X3DGroupingNode::traverseSG( TraverseInfo &ti ) {
  // save previous state of multi-pass transparency and reset
  // to false in order to be able to identify if any of the children
  // nodes sets it.
  bool previous_multi_pass  = ti.getMultiPassTransparency();
  ti.setMultiPassTransparency( false );

  for( MFNode::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->enableHapticsState( ti );
    }
  }

  // not using iterators since they can become invalid if the 
  // traversal changes the children field while iterating.
  const NodeVector &c = children->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] )
      c[i]->traverseSG( ti );
  }

  for( MFNode::const_reverse_iterator i = children->rbegin();
       i != children->rend(); ++i ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->disableHapticsState( ti );
    }
  }

  children_multi_pass_transparency = ti.getMultiPassTransparency();
  ti.setMultiPassTransparency( previous_multi_pass || children_multi_pass_transparency ); 
}

bool X3DGroupingNode::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  bool intersect = false;
  Bound * the_bound = bound->getValue();

  bool add_pt_devices = result.detect_pt_device && !pt_dev_sensors.empty();
  if( add_pt_devices ) {
    LineIntersectResult::PointingDeviceResultStruct temp_ptd_struct;
    temp_ptd_struct.global_to_local = result.getCurrentTransform().inverse();
    for( unsigned int i = 0; i < pt_dev_sensors.size(); ++i ) {
      if( pt_dev_sensors[i]->enabled->getValue() ) {
        temp_ptd_struct.x3dptd.push_back( pt_dev_sensors[i] );
      }
    }

    if( temp_ptd_struct.x3dptd.empty() )
      add_pt_devices = false;
    else
      result.pushCurrentPtDevice( temp_ptd_struct );
  }
  if( !the_bound ||
      the_bound->lineSegmentIntersect( from, to ) ) {
    Vec3f local_to = to;
    Vec3f local_from = from;
    bool below_one_plane = false;
    const NodeVector &children_nodes = children->getValue();
    for( unsigned int i = 0; i < children_nodes.size(); ++i ) {
      ClipPlane * cp = dynamic_cast< ClipPlane * >(children_nodes[i]);
      if( cp ) {
        if( !cp->truncateLine( local_from, local_to,
                               local_from, local_to ) ) {
          below_one_plane = true;
          break;
        }
      }
    }

    if( !below_one_plane ) {
      for( unsigned int i = 0; i < children_nodes.size(); ++i ) {
        if( children_nodes[i] &&
            children_nodes[i]->lineIntersect( local_from,
                                              local_to,
                                              result ) ) {
            intersect = true;
        }
      }
    }
  }

  if( add_pt_devices ) {
    result.popCurrentPtDevice();
  }
  return intersect;
}

void X3DGroupingNode::closestPoint( const Vec3f &p,
                                    NodeIntersectResult &result ) {
  const NodeVector &children_nodes = children->getValue();
  for( unsigned int i = 0; i < children_nodes.size(); ++i ) {
    if( children_nodes[i] )
      children_nodes[i]->closestPoint( p, result );
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

    // If we have a X3DPointingDeviceSensorNode add it to a separate
    // vector.
    X3DPointingDeviceSensorNode * pdsn = 
      dynamic_cast< X3DPointingDeviceSensorNode * >( n );
    if( pdsn ) {
      o->pt_dev_sensors.push_back( pdsn );
      o->pt_dev_sens_index[ pdsn ] = -1;
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
          t->transformedBound->unroute( o->bound );
        } else {
          bo->bound->unroute( o->bound );
        }
      }
    }

    // Remove eventual X3DPointingDeviceSensorNodes from the
    // separate vector.
    if( !o->pt_dev_sensors.empty() ) {
      X3DPointingDeviceSensorNode * pdsn = 
        dynamic_cast< X3DPointingDeviceSensorNode * >( n );
      if( pdsn ) {
        vector< X3DPointingDeviceSensorNode * >::iterator i;
        bool found = false;
        for( i = o->pt_dev_sensors.begin();
          i < o->pt_dev_sensors.end(); ++i ) {
            if( *i == n ) {
              found = true;
              break;
            }
        }
        if( found ) {
          o->pt_dev_sensors.erase( i );
          o->pt_dev_sens_index.erase( pdsn );
        }
      }
    }
  }
  MFChildBase::onRemove( n );
}


bool X3DGroupingNode::movingSphereIntersect( H3DFloat radius,
                                             const Vec3f &from, 
                                             const Vec3f &to,
                                             NodeIntersectResult &result ) {
  Bound * the_bound = bound->getValue();
  if( !the_bound || the_bound->movingSphereIntersect( from, to, radius ) ) {
    const NodeVector &children_nodes = children->getValue();
    bool hit = false;
    for( unsigned int i = 0; i < children_nodes.size(); ++i ) {
      if( children_nodes[i] &&
          children_nodes[i]->movingSphereIntersect( radius, from, to, result ))
        hit = true;
    }
    return hit;
  }
  return false;
}

void X3DGroupingNode::AddChildren::onAdd( Node *n ) {
  TypedMFNode< X3DChildNode >::onAdd( n );

  X3DGroupingNode *group_node = static_cast< X3DGroupingNode * >(getOwner());
  const NodeVector &c = group_node->children->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( n == c[i] )
      return;
  }

  group_node->children->push_back( n, group_node->id );
}

void X3DGroupingNode::RemoveChildren::onAdd( Node *n ) {
  TypedMFNode< X3DChildNode >::onAdd( n );
  X3DGroupingNode *group_node = static_cast< X3DGroupingNode * >(getOwner());
  group_node->children->erase( n, group_node->id );
}

