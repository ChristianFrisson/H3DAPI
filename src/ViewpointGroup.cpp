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
/// \file ViewpointGroup.cpp
/// \brief CPP file for ViewpointGroup, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ViewpointGroup.h>
#include <H3D/X3DViewpointNode.h>

using namespace H3D;

list< ViewpointGroup * > ViewpointGroup::viewpoint_groups;
bool ViewpointGroup::viewpoint_groups_changed;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ViewpointGroup::database( 
                                    "ViewpointGroup", 
                                    &(newInstance<ViewpointGroup>), 
                                    typeid( ViewpointGroup ),
                                    &X3DChildNode::database );

namespace ViewpointGroupInternals {
  FIELDDB_ELEMENT( ViewpointGroup, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, children, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, description, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, displayed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, retainUserOffsets, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, size, INPUT_OUTPUT );
}

ViewpointGroup::ViewpointGroup(
                    Inst< SFVec3f  >  _center,
                    Inst< MFChild  >  _children,
                    Inst< SFString >  _description,
                    Inst< SFBool   >  _displayed,
                    Inst< SFNode   >  _metadata,
                    Inst< SFBool   >  _retainUserOffsets,
                    Inst< SFVec3f  >  _size ) :
  X3DChildNode( _metadata ),
  center( _center ),
  children( _children ),
  description( _description ),
  displayed( _displayed ),
  retainUserOffsets( _retainUserOffsets ),
  size( _size ),
  in_scene_graph( false ),
  is_top_level( false ) {
  
  type_name = "ViewpointGroup";
  database.initFields( this );

  description->setValue( "" );
  displayed->setValue( true );
  center->setValue( Vec3f( 0, 0, 0 ) );
  retainUserOffsets->setValue( false );
  size->setValue( Vec3f( 0, 0, 0 ) );

  viewpoint_groups.push_back( this );
  viewpoint_groups_changed = true;
  /*
  X3DViewpointNode * vp = getActive();
  if( vp ) {
    last_position = vp->totalPosition->getValue();
  }
  */
}

void ViewpointGroup::traverseSG( TraverseInfo &ti ) {
  ViewpointGroup * dummy;
  if ( !in_scene_graph ) in_scene_graph = true;
  if ( ti.getUserData("ViewpointGroup", (void **) &dummy) != 0 ) {
    // user data has not been set, this is a top level ViewpointGroup
    is_top_level = true;
    ti.setUserData( "ViewpointGroup", this );
  }

  if( displayed->getValue() ) {
    Vec3f the_center = center->getValue();
    if( the_center.lengthSqr() <= Constants::f_epsilon ) {
      display_in_list = true;
    }
    else {
      X3DViewpointNode * vp = X3DViewpointNode::getActive();
      if( vp ) {
        Vec3f vp_full_position = vp->totalPosition->getValue();
        Vec3f box_size_half = size->getValue() / 2;
        Vec3f box_min = the_center - box_size_half;
        Vec3f box_max = the_center + box_size_half;
        if( vp_full_position.x >= box_min.x && vp_full_position.x <= box_max.x
          && vp_full_position.y >= box_min.y && vp_full_position.y <= box_max.y
          && vp_full_position.z >= box_min.z && vp_full_position.z <= box_max.z
          ) {
            display_in_list = true;
        }
        else
          display_in_list = false;
      }
    }
  }
  else {
    display_in_list = false;
  }

  // set dirty status on first traversal
  static bool flag = true;
  if ( flag ) {
    viewpoint_groups_changed = true;
    flag = false;
  }

  for( MFChild::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    (*i)->traverseSG( ti );
  }
  ti.deleteUserData( "ViewpointGroup" );
}

bool ViewpointGroup::containsViewpoint( X3DViewpointNode *vp ) const {
  for( MFChild::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    if( ViewpointGroup *g = dynamic_cast< ViewpointGroup * >( *i ) ) {
      if( g->containsViewpoint( vp ) ) return true;
    } else {
      if( (*i) == vp ) return true;
    }
  }
  return false;
}

list< Node * > ViewpointGroup::getChildrenAsList() {
  list< Node * > list;
  for( MFChild::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    list.push_back( *i );
  }
  return list;
}

void ViewpointGroup::MFChild::onAdd( Node * n ) {
  // Check if node is of type X3DViewpointNode or ViewpointGroup
  if ( !dynamic_cast< X3DViewpointNode * >(n) && 
      !dynamic_cast< ViewpointGroup * >(n) ) {
    Node *pi = getPrototypeNode( n );
    if( !dynamic_cast< X3DViewpointNode * >(pi) &&
        !dynamic_cast< ViewpointGroup * >(pi) ) {
      stringstream s;
      s << "Expecting " << typeid( X3DViewpointNode ).name();
      s << " or " << typeid( ViewpointGroup ).name();
      throw InvalidNodeType( n->getTypeName(),
                            s.str(),
                            H3D_FULL_LOCATION );
    }
  }
  MFNode::onAdd( n );
}
