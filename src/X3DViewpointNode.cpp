//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file X3DViewpointNode.cpp
/// \brief CPP file for X3DViewpointNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DViewpointNode.h>
#include <H3D/ViewpointGroup.h>
#include <H3D/NavigationInfo.h>

using namespace H3D;

list<X3DViewpointNode *> X3DViewpointNode::viewpoints;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DViewpointNode::database( 
                                    "X3DViewpointNode", 
                                    NULL,
                                    typeid( X3DViewpointNode ),
                                    &X3DBindableNode::database );

namespace X3DViewpointNodeInternals {
  FIELDDB_ELEMENT( X3DViewpointNode, centerOfRotation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DViewpointNode, description, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DViewpointNode, jump, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DViewpointNode, orientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DViewpointNode, position, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DViewpointNode, retainUserOffsets, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DViewpointNode, accForwardMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DViewpointNode, accInverseMatrix, OUTPUT_ONLY );
}



X3DViewpointNode::X3DViewpointNode( 
                     Inst< SFSetBind  > _set_bind,
                     Inst< SFVec3f    > _centerOfRotation,
                     Inst< SFString   > _description,
                     Inst< SFBool    >  _jump,
                     Inst< SFNode     > _metadata,
                     Inst< SFRotation > _orientation,
                     Inst< SFVec3f    > _position,
                     Inst< SFBool     > _retainUserOffsets,
                     Inst< SFTime     > _bindTime,
                     Inst< SFBool     > _isBound,
                     Inst< SFMatrix4f > _accForwardMatrix,
                     Inst< SFMatrix4f > _accInverseMatrix ) :
  X3DBindableNode( "X3DViewpointNode", _set_bind, _metadata, 
                   _bindTime, _isBound ),
  centerOfRotation( _centerOfRotation ),
  description     ( _description      ),
  jump            ( _jump             ),
  orientation     ( _orientation      ),
  position        ( _position         ),
  retainUserOffsets( _retainUserOffsets ),
  accForwardMatrix( _accForwardMatrix ),
  accInverseMatrix( _accInverseMatrix ) {
  
  type_name = "X3DViewpointNode";
  database.initFields( this );

  centerOfRotation->setValue( Vec3f( 0, 0, 0 ) );
  jump->setValue( true );
  orientation->setValue( Rotation( 0, 0, 1, 0 ) );
  position->setValue( Vec3f( 0, 0, 10 ) );
  retainUserOffsets->setValue( false );
  // need to give the id since the fields are output only.
  accForwardMatrix->setValue( Matrix4f(), id );
  accInverseMatrix->setValue( Matrix4f(), id );

  viewpoints.push_back( this );
}

void X3DViewpointNode::removeFromStack() {
  StackType &s =  stack[bindable_stack_name];
  if( s.size() > 0 ) {
    bool is_active = (s.front() == this);
    X3DBindableNode::removeFromStack();
    if( is_active ) {
      if( s.size() > 0 ) {
        X3DViewpointNode * new_vp =
          static_cast<X3DViewpointNode * >(s.front());
        bool local_jump =
          NavigationInfo::force_jump ? true : new_vp->jump->getValue();
        if( !local_jump ) {

          const Matrix4f &vp_acc_inv_mtx =
            new_vp->accInverseMatrix->getValue();
          const Matrix4f &old_vp_acc_frw_mtx = accForwardMatrix->getValue();

          new_vp->rel_pos = vp_acc_inv_mtx *
                            ( old_vp_acc_frw_mtx *
                              getFullPos() )
                            - new_vp->position->getValue();

          new_vp->rel_orn = -new_vp->orientation->getValue() * 
            ( Rotation( vp_acc_inv_mtx.getScaleRotationPart() ) *
              ( Rotation( old_vp_acc_frw_mtx.getScaleRotationPart() ) *
                getFullOrn() ) );
        }
      }
    }
  }
}

void X3DViewpointNode::toStackTop() {
  StackType &s =  stack[bindable_stack_name];
  X3DBindableNode *active = NULL;
  if( s.size() > 0 ) active = s.front();
  if ( active != this ) {
    bool local_jump = NavigationInfo::force_jump ? true : jump->getValue();
    if( local_jump ) {
      if( !retainUserOffsets->getValue() ) {
        rel_pos = Vec3f( 0, 0, 0 );
        rel_orn = Rotation( 0, 0, 0, 0 );
      }
    }
    else {
      if( active ) {
        X3DViewpointNode * old_vp = static_cast< X3DViewpointNode * >(active);
        const Matrix4f &vp_acc_inv_mtx = accInverseMatrix->getValue();
        const Matrix4f &old_vp_acc_frw_mtx =
          old_vp->accForwardMatrix->getValue();

        rel_pos = vp_acc_inv_mtx *
                  ( old_vp_acc_frw_mtx *
                    old_vp->getFullPos() )
                    - position->getValue();

        rel_orn = -orientation->getValue() * 
        ( Rotation( vp_acc_inv_mtx.getScaleRotationPart() ) *
          ( Rotation(old_vp_acc_frw_mtx.getScaleRotationPart() ) *
          old_vp->getFullOrn() ) );
      }
    }
    X3DBindableNode::toStackTop();
  }
}


X3DViewpointNode::ViewpointList X3DViewpointNode::getViewpointHierarchy() {
  list< ViewpointGroup *> groups;
  
  ViewpointList vps;

  for( ViewpointList::iterator i = viewpoints.begin();
       i != viewpoints.end(); i++ ) {
    if( ViewpointGroup *vg = dynamic_cast< ViewpointGroup * >( *i ) ) {
      groups.push_back( vg );
    }
  }

  if( groups.size() == 0 ) return viewpoints;
  for( ViewpointList::iterator i = viewpoints.begin();
       i != viewpoints.end(); i++ ) {
    bool standalone_vp = true;
    if( !dynamic_cast< ViewpointGroup * >( *i ) ) {
      for( list< ViewpointGroup * >::iterator g = groups.begin();
           g != groups.end(); g++ ) {
        if( (*g)->containsViewpoint( *i ) ) {
          standalone_vp = false;
          break;
        }
      }
    }
    if( standalone_vp ) vps.push_back( *i );
  }

  return vps;
}

void X3DViewpointNode::rotateAround( Rotation rotation, bool collision,
                                     Vec3f center_of_rot ) {
  Vec3f vp_pos = position->getValue();
  Vec3f vp_full_pos = vp_pos + rel_pos;
  Rotation vp_orientation = orientation->getValue();
  Rotation vp_full_orientation = vp_orientation * rel_orn;

  rotation = Rotation( vp_full_orientation * rotation.axis, rotation.angle );
  Vec3f new_pos = Matrix3f( rotation ) *
    ( vp_full_pos - center_of_rot ) +
    center_of_rot;
  bool no_collision = true;

  if( no_collision ) {
    rel_pos = new_pos - vp_pos;
    Rotation new_rotation = rotation * vp_full_orientation;
    rel_orn = -vp_orientation * new_rotation;
  }
}

void X3DViewpointNode::rotateAroundSelf( Rotation rotation ) {
  Rotation vp_orientation = orientation->getValue();
  Rotation vp_full_orientation = vp_orientation * rel_orn;
  rotation = Rotation( vp_full_orientation * rotation.axis, rotation.angle );
  Rotation new_rotation = rotation * vp_full_orientation;
  rel_orn = -vp_orientation * new_rotation;
}

void X3DViewpointNode::translate( Vec3f direction, bool collision,
                                  const vector< H3DFloat > &avatar_size,
                                  X3DChildNode * topNode ) {
  Vec3f vp_pos = position->getValue();
  Vec3f vp_full_pos = vp_pos + rel_pos;
  Rotation vp_full_orientation = orientation->getValue() * rel_orn;
  const Matrix4f &acc_fr_mt = accForwardMatrix->getValue();
  Vec3f scaling = acc_fr_mt.getScalePart();
  direction = vp_full_orientation * direction;
  //direction = direction * scaling.x;
  Vec3f new_pos = vp_full_pos + direction;
  bool no_collision = true;

  if( collision && !avatar_size.empty() && 
    topNode->movingSphereIntersect( avatar_size[0],
    acc_fr_mt * vp_full_pos,
    acc_fr_mt * new_pos ) )
    no_collision = false;

  if( no_collision )
    rel_pos = new_pos - vp_pos;
}

void X3DViewpointNode::moveTo( Vec3f new_pos ) {
  rel_pos = new_pos - position->getValue();
}
bool X3DViewpointNode::detectCollision( const vector< H3DFloat > &avatar_size,
                                        X3DChildNode * topNode ) {
  Vec3f vp_full_pos =
    accForwardMatrix->getValue() *  (position->getValue() + rel_pos );
  return topNode->movingSphereIntersect( avatar_size[0],
                                         vp_full_pos,
                                         vp_full_pos );
}

