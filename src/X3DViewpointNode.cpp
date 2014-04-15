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
/// \file X3DViewpointNode.cpp
/// \brief CPP file for X3DViewpointNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DViewpointNode.h>
#include <H3D/ViewpointGroup.h>
#include <H3D/H3DNavigation.h>

using namespace H3D;

list<X3DViewpointNode *> X3DViewpointNode::viewpoints;
bool X3DViewpointNode::viewpoints_changed;

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
  FIELDDB_ELEMENT( X3DViewpointNode, totalPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DViewpointNode, totalOrientation, OUTPUT_ONLY );

  inline bool intersectLinePlane( const Vec3f &from,
                                  const Vec3f &to,
                                  const Vec3f &plane_point,
                                  const Vec3f &plane_normal,
                                  Vec3f &p ) {
    Vec3f ab = to - from;
    H3DFloat denom = plane_normal * ab;
    if( H3DAbs( denom ) < Constants::f_epsilon ) {
      return false;
    }

    H3DFloat t = ( plane_point * plane_normal - plane_normal * from ) / denom;
    p = from + t * ab;

    return true;
  }
}

X3DViewpointNode::X3DViewpointNode( Inst< SFSetBind     > _set_bind,
                                    Inst< SFVec3f       > _centerOfRotation,
                                    Inst< SFString      > _description,
                                    Inst< SFBool        > _jump,
                                    Inst< SFNode        > _metadata,
                                    Inst< SFOrientation > _orientation,
                                    Inst< SFPosition    > _position,
                                    Inst< SFBool        > _retainUserOffsets,
                                    Inst< SFTime        > _bindTime,
                                    Inst< SFBool        > _isBound,
                                    Inst< SFMatrix4f    > _accForwardMatrix,
                                    Inst< SFMatrix4f    > _accInverseMatrix,
                                    Inst< SFSumVec3f    > _totalPosition,
                                    Inst< SFSumRotation > _totalOrientation ) :
  X3DBindableNode( "X3DViewpointNode", _set_bind, _metadata, 
                   _bindTime, _isBound ),
  centerOfRotation ( _centerOfRotation ),
  description      ( _description      ),
  jump             ( _jump             ),
  orientation      ( _orientation      ),
  position         ( _position         ),
  retainUserOffsets( _retainUserOffsets ),
  accForwardMatrix ( _accForwardMatrix ),
  accInverseMatrix ( _accInverseMatrix ),
  totalPosition    ( _totalPosition ),
  totalOrientation ( _totalOrientation ),
  relPos( new SFVec3f() ),
  relOrn( new SFRotation() ),
  in_scene_graph( false ),
  is_top_level( false ) {
  
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

  position->routeNoEvent( totalPosition, id );
  relPos->setName( "relPos" );
  relPos->route( totalPosition, id );

  orientation->routeNoEvent( totalOrientation, id );
  relOrn->setName( "relOrn" );
  relOrn->route( totalOrientation, id );

  viewpoints.push_back( this );
  viewpoints_changed = true;
}

void X3DViewpointNode::traverseSG( TraverseInfo &ti ) {
  accInverseMatrix->setValue( ti.getAccInverseMatrix(), id );
  accForwardMatrix->setValue( ti.getAccForwardMatrix(), id );
  ViewpointGroup * dummy;
  if ( !in_scene_graph ) in_scene_graph = true;
  if ( ti.getUserData("ViewpointGroup", (void **) &dummy) != 0 ) {
    // user data has not been set, this is a viewpoint that exists 
    // outside a ViewpointGroup
    is_top_level = true;
  }
  // set dirty status on first traversal
  static bool flag = true;
  if ( flag ) {
    viewpoints_changed = true;
    flag = false;
  }
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
        bool local_jump = H3DNavigation::force_jump || new_vp->jump->getValue();
        if( !local_jump ) {

          const Matrix4f &vp_acc_inv_mtx =
            new_vp->accInverseMatrix->getValue();
          const Matrix4f &old_vp_acc_frw_mtx = accForwardMatrix->getValue();

          new_vp->relPos->setValue( vp_acc_inv_mtx *
                                    ( old_vp_acc_frw_mtx *
                                      totalPosition->getValue() )
                                    - new_vp->position->getValue() );

          new_vp->relOrn->setValue( -new_vp->orientation->getValue() * 
            ( Rotation( vp_acc_inv_mtx.getScaleRotationPart() ) *
              ( Rotation( old_vp_acc_frw_mtx.getScaleRotationPart() ) *
                totalOrientation->getValue() ) ) );
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
    bool local_jump = H3DNavigation::force_jump || jump->getValue();
    if( local_jump ) {
      if( !retainUserOffsets->getValue() ) {
        relPos->setValue( Vec3f( 0, 0, 0 ) );
        relOrn->setValue( Rotation( 0, 0, 0, 0 ) );
      }
    }
    else {
      if( active ) {
        X3DViewpointNode * old_vp = static_cast< X3DViewpointNode * >(active);
        const Matrix4f &vp_acc_inv_mtx = accInverseMatrix->getValue();
        const Matrix4f &old_vp_acc_frw_mtx =
          old_vp->accForwardMatrix->getValue();

        relPos->setValue( vp_acc_inv_mtx *
                  ( old_vp_acc_frw_mtx *
                    old_vp->totalPosition->getValue() )
                    - position->getValue() );

        relOrn->setValue( -orientation->getValue() * 
        ( Rotation( vp_acc_inv_mtx.getScaleRotationPart() ) *
          ( Rotation(old_vp_acc_frw_mtx.getScaleRotationPart() ) *
          old_vp->totalOrientation->getValue() ) ) );
      }
    }
    X3DBindableNode::toStackTop();
  }
}

void X3DViewpointNode::rotateAround( Rotation rotation, bool collision,
                                     Vec3f center_of_rot ) {
  Vec3f vp_pos = position->getValue();
  Vec3f vp_full_pos = vp_pos + relPos->getValue();
  Rotation vp_orientation = orientation->getValue();
  Rotation vp_full_orientation = vp_orientation * relOrn->getValue();

  rotation = Rotation( vp_full_orientation * rotation.axis, rotation.angle );
  Vec3f new_pos = Matrix3f( rotation ) *
    ( vp_full_pos - center_of_rot ) +
    center_of_rot;

  relPos->setValue( new_pos - vp_pos );
  Rotation new_rotation = rotation * vp_full_orientation;
  relOrn->setValue( -vp_orientation * new_rotation );
}

void X3DViewpointNode::rotateAroundSelf( Rotation rotation ) {
  Rotation vp_orientation = orientation->getValue();
  Rotation vp_full_orientation = vp_orientation * relOrn->getValue();
  rotation = Rotation( vp_full_orientation * rotation.axis, rotation.angle );
  Rotation new_rotation = rotation * vp_full_orientation;
  relOrn->setValue( -vp_orientation * new_rotation );
}

void X3DViewpointNode::translate( Vec3f direction, bool collision,
                                  const vector< H3DFloat > &avatar_size,
                                  X3DChildNode * topNode ) {
  Vec3f vp_pos = position->getValue();
  Vec3f vp_full_pos = vp_pos + relPos->getValue();
  Rotation vp_full_orientation = orientation->getValue() * relOrn->getValue();
  const Matrix4f &acc_fr_mt = accForwardMatrix->getValue();
  direction = vp_full_orientation * direction;
  Vec3f new_pos = vp_full_pos + direction;
  H3DFloat min_dist_from_plane = 1e-5f;

  if( collision && !avatar_size.empty() ) {
    Vec3f global_from = acc_fr_mt * vp_full_pos;
    Vec3f global_to = acc_fr_mt * new_pos;
    Vec3f global_direction = global_to - global_from;
    H3DFloat max_length = global_direction.length();
    if( max_length > Constants::f_epsilon )
      global_direction = global_direction / max_length;
    int max_nr_iterations = 5;
    int counter = 0;
    // Iterate until no collision or max_nr_iterations
    while( counter < max_nr_iterations ) {
      NodeIntersectResult result;
      if( !topNode->movingSphereIntersect( avatar_size[0],
                                           global_from,
                                           global_to,
                                           result ) ) {
        // No collision, exit loop.
        global_from = global_to;
        break;
      }
      result.transformResult();

      int closest_index = -1;
      H3DFloat closest_t = 2;
      for( unsigned int i = 0; i < result.result.size(); ++i ) {
        if( result.result[i].t < closest_t ) {
          closest_index = i;
          closest_t = (H3DFloat)result.result[i].t;
        }
      }

      Vec3f point_to_use = Vec3f( result.result[closest_index].point );
      Vec3f normal_to_use = Vec3f( result.result[ closest_index ].normal );
      normal_to_use.normalize();

      // Desired movement is along the plane
      Vec3f movement;
      if( X3DViewpointNodeInternals::intersectLinePlane( global_from,
                                                         global_to,
                                                         point_to_use,
                                                         normal_to_use,
                                                         movement ) )
        movement = movement - point_to_use;
      else
        movement = global_direction;

      movement.normalizeSafe();
      movement = (global_direction * movement ) * movement;
      global_to = global_from + movement;

      // If the new location is to close to plane move
      // out from plane.
      HAPI::Vec3 closest, tmp;
      HAPI::Collision::Plane hit_plane( point_to_use, normal_to_use );
      hit_plane.closestPoint( global_to, closest, tmp, tmp );
      if( (Vec3f( closest ) - global_to).lengthSqr() <= avatar_size[0] ) {
        H3DFloat dist_to_move =
          avatar_size[0] - (Vec3f( closest ) - global_to ).length()
          + min_dist_from_plane;
        global_to = global_to + dist_to_move * normal_to_use;
        movement = global_to - global_from;
        H3DFloat mov_length = movement.lengthSqr();
        // Do not move further than the avatar originally should have done.
        if( mov_length > Constants::f_epsilon && 
            mov_length > max_length * max_length ) {
          movement = movement / H3DSqrt( mov_length );
          movement = movement * max_length;
          global_to = global_from + movement;
        }
      }
      ++counter;
    }
    relPos->setValue( acc_fr_mt.inverse() * global_from - vp_pos );
  } else
    relPos->setValue( new_pos - vp_pos );
}

// used when moving without collision of any kind.
void X3DViewpointNode::moveTo( Vec3f new_pos ) {
  relPos->setValue( new_pos - position->getValue() );
}
bool X3DViewpointNode::detectCollision( const vector< H3DFloat > &avatar_size,
                                        X3DChildNode * topNode ) {
  Vec3f vp_full_pos =
    accForwardMatrix->getValue() * ( totalPosition->getValue() );
  NodeIntersectResult result;
  return topNode->movingSphereIntersect( avatar_size[0],
                                         vp_full_pos,
                                         vp_full_pos,
                                         result );
}

void X3DViewpointNode::getProjectionDimensions( EyeMode eye_mode,
                                                H3DFloat width,
                                                H3DFloat height,
                                                H3DFloat clip_near,
                                                H3DFloat &top,
                                                H3DFloat &bottom,
                                                H3DFloat &right,
                                                H3DFloat &left,
                                                StereoInfo * stereo_info ) {
  windowFromfieldOfView( width,
                         height,
                         clip_near,
                         top, bottom, right, left );
  H3DFloat frustum_shift = 0;
  if( eye_mode == LEFT_EYE || eye_mode == RIGHT_EYE ) {
    if( !stereo_info )
      stereo_info = StereoInfo::getActive();
    
    H3DFloat interocular_distance = (H3DFloat) 0.06;
    H3DFloat focal_distance = (H3DFloat) 0.6;
    if( stereo_info ) {
      interocular_distance = stereo_info->interocularDistance->getValue();
      focal_distance = stereo_info->focalDistance->getValue();
    }
    
    frustum_shift = ( interocular_distance / 2 )
      * clip_near / focal_distance;
    if( eye_mode == RIGHT_EYE )
      frustum_shift = -frustum_shift;
  } 
  left += frustum_shift;
  right += frustum_shift;
}

void X3DViewpointNode::setupViewMatrix( EyeMode eye_mode,
                                        StereoInfo * stereo_info  ) {
  const Vec3f &vp_position = totalPosition->getValue();
  const Rotation &vp_orientation = totalOrientation->getValue();
  const Matrix4f &vp_inv_m = accInverseMatrix->getValue();  
  // OpenGL version of vp_inv_m
  GLfloat vp_inv_transform[] = { 
    vp_inv_m[0][0], vp_inv_m[1][0], vp_inv_m[2][0], 0,
    vp_inv_m[0][1], vp_inv_m[1][1], vp_inv_m[2][1], 0,
    vp_inv_m[0][2], vp_inv_m[1][2], vp_inv_m[2][2], 0,
    vp_inv_m[0][3], vp_inv_m[1][3], vp_inv_m[2][3], 1 };

  if( eye_mode != MONO ) {
    // if stereo mode, viewpoint varies depending on what eye 
    //is being rendered for.

    H3DFloat interocular_distance = 0.06f;
    Rotation head_tilt = Rotation(1,0,0,0 );
    if( !stereo_info ) stereo_info = StereoInfo::getActive();
    if( stereo_info ) {
      interocular_distance = stereo_info->interocularDistance->getValue();
      head_tilt = stereo_info->headTilt->getValue();
    }

    H3DFloat half_interocular_distance = interocular_distance / 2;    

    // the vector from the viewpoint center to the left eye.
    Vec3f left_eye = 
      head_tilt * Vec3f( -half_interocular_distance, 0, 0 ); 

    // move to the eye position
    if( eye_mode == LEFT_EYE ) {
      glTranslatef( -left_eye.x, -left_eye.y, -left_eye.z );
    } else if( eye_mode == RIGHT_EYE ) {
      glTranslatef( left_eye.x, left_eye.y, left_eye.z );
    }
  }

  // set up view matrix based on X3DViewpointNode field values.
  glRotatef( (H3DFloat) -(180/Constants::pi)*vp_orientation.angle, 
             vp_orientation.axis.x, 
             vp_orientation.axis.y,
             vp_orientation.axis.z );
  
  glTranslatef( -vp_position.x,
                -vp_position.y, 
                -vp_position.z );
  glMultMatrixf( vp_inv_transform );

}


Matrix4f X3DViewpointNode::getViewMatrix( EyeMode eye_mode,
                                          StereoInfo * stereo_info ) {
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  setupViewMatrix( eye_mode, stereo_info );
  GLfloat mv[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mv );
  H3D::Matrix4f m( mv[0], mv[4], mv[8], mv[12],
                   mv[1], mv[5], mv[9], mv[13],
                   mv[2], mv[6], mv[10], mv[14],
                   mv[3], mv[7], mv[11], mv[15] );
  glPopMatrix();
  return m;
}
