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
/// \file H3DNavigation.cpp
/// \brief CPP file for H3DNavigation, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DNavigation.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/Scene.h>
#include <H3D/NavigationInfo.h>

using namespace H3D;

list< H3DNavigation * > H3DNavigation::h3d_navigations;
bool H3DNavigation::force_jump = false;

H3DNavigation::H3DNavigation() : haptic_device_nav( NULL ),
                                 sws_navigation( NULL ),
                                 linear_interpolate( false ),
                                 old_vp( NULL ),
                                 last_time( 0 ) {
  mouse_nav.reset( new MouseNavigation( this ) );
  keyboard_nav.reset( new KeyboardNavigation( this ) );

  h3d_navigations.push_back( this );
}

 bool H3DNavigation::isUniformScaling( const Vec3f &scaling ) {
   return 
     H3DAbs( scaling.x - scaling.y ) < 0.00001 && 
     H3DAbs( scaling.y - scaling.z ) < 0.00001;
 }

void H3DNavigation::doNavigation(
                                string navigation_type, X3DViewpointNode * vp,
                                X3DChildNode *topNode, bool detect_collision,
                                const vector< H3DFloat > &avatar_size, 
                                H3DFloat speed,
                                const vector< string > &transition_type,
                                H3DTime transition_time ) {
  Vec3f vp_pos = vp->position->getValue();
  Vec3f vp_full_pos = vp_pos + vp->relPos->getValue();
  Rotation vp_orientation = vp->orientation->getValue();
  Rotation vp_full_orientation = vp_orientation * vp->relOrn->getValue();
  H3DTime current_time = Scene::time->getValue();
  if( old_vp.get() && old_vp.get() != vp ) {
    // if the viewpoint is switched when a transition is going on
    // reset the old viewpoint and calculate the new transition from
    // current position and viewpoint.
    if( linear_interpolate ) {
      if( !old_vp.get()->retainUserOffsets->getValue() ) {
        old_vp.get()->relPos->setValue( goal_position );
        old_vp.get()->relOrn->setValue( goal_orientation );
      }
      linear_interpolate = false;
    }

    if( vp->jump->getValue() || force_jump ) {
      force_jump = false;

      string transition = getTransitionType( transition_type );

      if( transition == "TELEPORT" ) {
        // Instant movement.
        NavigationInfo *nav_info = NavigationInfo::getActive();
        if( nav_info ) {
          nav_info->setTransitionComplete( true );
        }
      } else {
        // For all other types than teleport, use LINEAR movement.
        linear_interpolate = true;
        const Matrix4f &vp_acc_inv_mtx = vp->accInverseMatrix->getValue();
        const Matrix4f &old_vp_acc_frw_mtx = 
          old_vp->accForwardMatrix->getValue();

        start_orientation = -vp_orientation * 
          ( (Rotation)vp_acc_inv_mtx.getScaleRotationPart() *
          ( (Rotation)old_vp_acc_frw_mtx.getScaleRotationPart()
          * old_vp_orientation ) );

        goal_orientation = vp->relOrn->getValue();

        start_position = vp_acc_inv_mtx * 
          (old_vp_acc_frw_mtx * old_vp_pos) - vp_pos;
        goal_position = vp->relPos->getValue();
        move_direction = goal_position - start_position;
        start_time = current_time;
      }
    }
    old_vp.reset( vp );
  }
  else if( !old_vp.get() ) {
    old_vp.reset( vp );
    old_vp_pos = old_vp->totalPosition->getValue();
    old_vp_orientation = old_vp->totalOrientation->getValue();
  }

  // When a transition takes place navigationinfo negates external
  // use of setValue of the position field of a viewpoint
  if( linear_interpolate ) {
    H3DTime elapsed_time = current_time - start_time;
    H3DTime total_time = transition_time;
    if( elapsed_time < total_time ) {
      H3DDouble interpolation = elapsed_time / total_time;
      vp->relPos->setValue( start_position +
        move_direction * interpolation );
      vp->relOrn->setValue( start_orientation.slerp( goal_orientation,
        (H3DFloat)interpolation ) );
    } else {
      linear_interpolate = false;
      NavigationInfo *nav_info = NavigationInfo::getActive();
      if( nav_info ) {
        nav_info->setTransitionComplete( true );
      }
      vp->relPos->setValue( goal_position );
      vp_full_pos = vp_pos + goal_position;
      vp->relOrn->setValue( goal_orientation );
      vp_full_orientation = vp_orientation * goal_orientation;
    }
  }

  if( !linear_interpolate ) {

    Rotation vp_full_orientation = vp->totalOrientation->getValue();
    H3DTime delta_time = current_time - last_time;
    last_time = current_time;

    if( navigation_type == "EXAMINE" || navigation_type == "ANY" ) {
      H3DNavigationDevices::MoveInfo move_info;
      if( H3DNavigationDevices::getMoveInfo( move_info, this ) ) {
        if( move_info.zoom ) {
          Vec3f scaling = vp->accForwardMatrix->getValue().getScalePart();
  
          vector< H3DFloat > temp_avatar_size( avatar_size );
          for( unsigned int i = 0; i < temp_avatar_size.size(); ++i ) {
             temp_avatar_size[i] *= scaling.x;
          }
          Vec3f direction = vp->centerOfRotation->getValue() - 
                            vp->totalPosition->getValue();
          H3DFloat max_movement = direction.length();
          if( max_movement < Constants::f_epsilon ) {
            // Should never come here unless user manually aligns the
            // position of the viewpoint with the center of rotation.
            // Just choose a direction to zoom out from.
            direction = Vec3f( 1, 0, 0 );
            max_movement = 1;
          }
          direction = direction / max_movement;
          bool move_towards = true;
          if( move_info.translation_sum * Vec3f( 0, 0, -1 ) < 0 ) {
            direction = -direction;
            move_towards = false;
          }
          direction = -vp_full_orientation * direction;
          direction = move_info.translation_sum.length() * direction *
                          speed * scaling.x;
          if( move_towards ) {
             H3DFloat dist_from_center = 1e-5f;
             max_movement = max_movement > dist_from_center ?
                            max_movement - dist_from_center : 0;
             if( direction.length() < max_movement )
                  vp->translate( direction, false, temp_avatar_size, topNode );
             else {
               direction.normalize();
               vp->translate( direction * max_movement,
                              false, temp_avatar_size, topNode );
             }
           } else {
              vp->translate( direction, false, temp_avatar_size, topNode );
           }
           
           if( !isUniformScaling( scaling ) ) {
              Console(3) << "Warning: Non-uniform scaling in the"
                         << " active X3DViewpointNode ( "
                         << vp->getName()
                         << " ) nodes local coordinate system. Speed and avatar size of "
                         << "Avatar is undefined " << endl;
           }
        } else if( move_info.use_center_sum ) {
          vp->translate( move_info.translation_sum,
                         false, avatar_size, topNode );
          vp->rotateAround( move_info.rotation_sum, false,
                            vp->accInverseMatrix->getValue() *
                            move_info.center_of_rot_sum );
        }
        else {
          vp->rotateAround( move_info.rotation_sum, false,
                            vp->centerOfRotation->getValue() );
        }
      }
      else if( detect_collision )
        vp->detectCollision( avatar_size, topNode );
    } else if( navigation_type == "WALK" || navigation_type == "FLY" ) {
      Vec3f scaling = vp->accForwardMatrix->getValue().getScalePart();
      
      vector< H3DFloat > temp_avatar_size( avatar_size );
      for( unsigned int i = 0; i < temp_avatar_size.size(); ++i ) {
        temp_avatar_size[i] *= scaling.x;
      }
      H3DNavigationDevices::MoveInfo move_info;
      if( H3DNavigationDevices::getMoveInfo( move_info, this ) ) {
        vp->rotateAroundSelf( move_info.rotation_sum );
        vp->translate( move_info.translation_sum * speed *
                       delta_time * scaling.x,
                       detect_collision, temp_avatar_size, topNode );
      }
      else if( detect_collision ) {
        vp->detectCollision( temp_avatar_size, topNode );
      }

      if( !isUniformScaling( scaling ) ) {
         Console(3) << "Warning: Non-uniform scaling in the"
                    << " active X3DViewpointNode ( "
                    << vp->getName()
                    << " ) nodes local coordinate system. Speed and avatar size of "
                    << "Avatar is undefined " << endl;
      }     
    } else if( navigation_type == "LOOKAT" ) {
      H3DNavigationDevices::MoveInfo move_info;
      if( H3DNavigationDevices::getMoveInfo( move_info, this ) ) {

        // TODO: maybe change this to allow for other devices to specify what
        // to look at ( for example by supply a point and direction ).
        GLint viewport[4];
        GLdouble mvmatrix[16], projmatrix[16];
        GLdouble wx, wy, wz;
        glGetIntegerv( GL_VIEWPORT, viewport );
        glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
        glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );

        Vec2f mouse_pos = Vec2f( move_info.translation_sum.x,
                                 move_info.translation_sum.y );
        mouse_pos.y = viewport[3] - mouse_pos.y - 1;
        // Project to 0, 0.5 and 1.0 and use the values to check if
        // an infinite clip_far value in a Viewpoint node caused the
        // unproject to project to the wrong place (behind the near_plane)
        // if it did then choose a large vector in the other direction.
        gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
          0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
        Vec3f near_plane_pos( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );
        gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
          0.5, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
        Vec3f middle_plane_pos( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );
        gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
          1.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
        Vec3f far_plane_pos( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );
        Vec3f near_middle = middle_plane_pos - near_plane_pos;
        Vec3f near_far = far_plane_pos - near_plane_pos;
        if( near_middle * near_far < 0 ) {
          // Infinite far plane caused problems. Choose a large vector in the
          // other direction. Note that this is not an optimal solution, the best
          // solution in this case would be to use a rayIntersect function in
          // updateX3DPointingDeviceSensors. In this case we choose the point
          // halfway to infinity, hopefully that should be enough.
          near_middle.normalizeSafe();
          far_plane_pos = near_plane_pos + near_far.length() * near_middle;
        }

        Node::LineIntersectResult result( true, false );
        if( topNode->lineIntersect( near_plane_pos,
                                    far_plane_pos,
                                    result ) ) {
          int closest = 0;
          result.transformResult();
          if( result.theNodes.size() > 1 ) {
            H3DFloat closestDistance = 
              (H3DFloat)( result.result[closest].point
                          - near_plane_pos ).lengthSqr();
            for( unsigned int kl = 1; kl < result.theNodes.size(); ++kl ) {
              H3DFloat tempClose = 
                (H3DFloat)( result.result[kl].point -
                            near_plane_pos).lengthSqr();
              if( tempClose < closestDistance ) {
                closestDistance = tempClose;
                closest = kl;
              }
            }
          }

          Vec3f approx_center;
          H3DFloat viewing_distance;
          BoxBound *box_bound = 0;
          H3DBoundedObject * the_bound_object =
            dynamic_cast< H3DBoundedObject * >
              (result.theNodes[closest] );
          if( the_bound_object ) {
            box_bound = dynamic_cast< BoxBound * >(
              the_bound_object->bound->getValue() );
          }
          const Matrix4f &vp_acc_inv_mtx = vp->accInverseMatrix->getValue();
          const vector< Matrix4f > transform_matrices =
            result.getGeometryTransforms();

          if( box_bound ) {
            approx_center = vp_acc_inv_mtx *
              ( transform_matrices[closest ] * box_bound->center->getValue() );
            Vec3f acc_inv_scale = vp_acc_inv_mtx.getScalePart();
            Vec3f geom_scale = transform_matrices[closest].getScalePart();
            Vec3f size = H3DMax( H3DMax( acc_inv_scale.x, acc_inv_scale.y ),
                                 acc_inv_scale.z ) *
                         ( H3DMax( H3DMax( geom_scale.x, geom_scale.y ),
                                   geom_scale.z ) *
                         box_bound->size->getValue() );

            viewing_distance = 2 * size.length();
          } else {
            // calculate the center and viewing distance
            // since we know nothing about the geometry except
            // the point of intersection this is just a crude guess
            approx_center = vp_acc_inv_mtx *
              ( transform_matrices[closest ] * approx_center );
            viewing_distance = (vp_acc_inv_mtx *( transform_matrices[closest] *
              (Vec3f)result.result[closest].point ) ).length() * 2.0f;
          }
          Vec3f backward = vp_full_orientation * Vec3f( 0, 0, 1 );
          vp->centerOfRotation->setValue( approx_center );

          string transition = getTransitionType( transition_type );
          if( transition == "TELEPORT" ) {
            // Instant movement.
            vp->moveTo( approx_center + viewing_distance * backward );
          } else {
            // For all other types than teleport, use LINEAR movement.
            linear_interpolate = true;
            start_orientation = vp->relOrn->getValue();
            goal_orientation = start_orientation;

            start_position = vp->relPos->getValue();
            goal_position = ( approx_center + viewing_distance * backward ) -
                            vp->position->getValue();
            move_direction = goal_position - start_position;
            start_time = current_time;
          }

          // No check is done to make sure that the selected object is not
          // obscured by other objects or that the new position is in a place
          // where fly and walk would detect collision.
        }
      }
      else if( detect_collision )
        vp->detectCollision( avatar_size, topNode );
    }
    else if( navigation_type == "NONE" ) {
      if( detect_collision )
        vp->detectCollision( avatar_size, topNode );
    }
  }

  old_vp_pos = vp_full_pos;
  old_vp_orientation = vp_full_orientation;
  H3DNavigationDevices::setNavTypeForAll( navigation_type, this );
}

void H3DNavigation::disableDevice( int device ) {
  for( list< H3DNavigation * >::iterator i = h3d_navigations.begin();
       i != h3d_navigations.end(); ++i ) {
    switch( device ) {
      case ALL: {
        if( (*i)->mouse_nav.get() )
          (*i)->mouse_nav->disableDevice();
        (*i)->keyboard_nav.reset( NULL );
        (*i)->haptic_device_nav.reset( NULL );
        (*i)->sws_navigation.reset( NULL );
        break;
      }
      case MOUSE: {
        if( (*i)->mouse_nav.get() )
          (*i)->mouse_nav->disableDevice();
        break;
      }
      case KEYBOARD: {
        (*i)->keyboard_nav.reset( NULL );
        break;
      }
      case HAPTICSDEVICE: {
        (*i)->haptic_device_nav.reset( NULL );
        break;
      }
      case SWS: {
        (*i)->sws_navigation.reset( NULL );
        break;
      }
      default: {}
    }
  }
}

void H3DNavigation::enableDevice( int device ) {
  for( list< H3DNavigation * >::iterator i = h3d_navigations.begin();
       i != h3d_navigations.end(); ++i ) {
    switch( device ) {
      case ALL: {
        if( (*i)->mouse_nav.get() ) {
          (*i)->mouse_nav->enableDevice();
        }
        else {
          (*i)->mouse_nav.reset( new MouseNavigation( (*i) ) );
        }
        if( !(*i)->keyboard_nav.get() )
          (*i)->keyboard_nav.reset( new KeyboardNavigation( (*i) ) );
        if( !(*i)->haptic_device_nav.get() )
          (*i)->haptic_device_nav.reset( new HapticDeviceNavigation( (*i) ) );
        if( !(*i)->sws_navigation.get() )
          (*i)->sws_navigation.reset( new SWSNavigation( (*i) ) );
        break;
      }
      case MOUSE: {
        if( (*i)->mouse_nav.get() ) {
          (*i)->mouse_nav->enableDevice();
        }
        else {
          (*i)->mouse_nav.reset( new MouseNavigation( (*i) ) );
        }
        break;
      }
      case KEYBOARD: {
        if( !(*i)->keyboard_nav.get() )
          (*i)->keyboard_nav.reset( new KeyboardNavigation( (*i) ) );
        break;
      }
      case HAPTICSDEVICE: {
        if( !(*i)->haptic_device_nav.get() )
          (*i)->haptic_device_nav.reset( new HapticDeviceNavigation( (*i) ) );
        break;
      }
      case SWS: {
        if( !(*i)->sws_navigation.get() )
          (*i)->sws_navigation.reset( new SWSNavigation( (*i) ) );
        break;
      }
      default: {}
    }
  }
}

bool H3DNavigation::isEnabled( int device ) {
  for( list< H3DNavigation * >::iterator i = h3d_navigations.begin();
       i != h3d_navigations.end(); ++i ) {
    switch( device ) {
      case ALL: {
        if( !(*i)->mouse_nav.get() || !(*i)->mouse_nav->isEnabled() ) {
           return false;
        }
        if( !(*i)->keyboard_nav.get() )
          return false;
        if( !(*i)->haptic_device_nav.get() )
          return false;
        if( !(*i)->sws_navigation.get() )
          return false;
        return true;
      }
      case MOUSE: {
        if( !(*i)->mouse_nav.get() || !(*i)->mouse_nav->isEnabled() ) {
           return false;
        }
        return true;
      }
      case KEYBOARD: {
        if( (*i)->keyboard_nav.get() )
          return true;
        return false;
      }
      case HAPTICSDEVICE: {
        if( (*i)->haptic_device_nav.get() )
          return true;
        return false;
      }
      case SWS: {
        if( (*i)->sws_navigation.get() )
          return true;
        return false;
      }
      default: { return false; }
    }
  }
  return false;
}

X3DViewpointNode *
H3DNavigation::viewpointToUse( X3DViewpointNode *potential_vp,
                               int nav_index ) {
  if( nav_index >= 0 && nav_index < (int)h3d_navigations.size() ) {
    list< H3DNavigation * >::iterator i = h3d_navigations.begin();
    for( int j = 0; j < nav_index; ++j ) {
      ++i;
    }
    return (*i)->viewpointToUse( potential_vp );
  }
  return NULL;
}

string H3DNavigation::getTransitionType(
  const vector< string > &transition_types ) {
  for( unsigned int i = 0; i < transition_types.size(); ++i ) {
    if( transition_types[i] == "LINEAR" ||
        transition_types[i] == "TELEPORT" ||
        transition_types[i] == "ANIMATE" )
    return transition_types[i];
  }
  return string( "LINEAR" );
}

