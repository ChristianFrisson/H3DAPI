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
/// \file H3DNavigation.cpp
/// \brief CPP file for H3DNavigation, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DNavigation.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/Scene.h>

using namespace H3D;

H3DNavigation * H3DNavigation::instance = 0;

H3DNavigation::H3DNavigation() : mouse_nav( new MouseNavigation() ),
                                 keyboard_nav( new KeyboardNavigation() ),
                                 haptic_device_nav( 0 ),
                                 sws_navigation( 0 ) {
}

void H3DNavigation::doNavigation(
                                string navigation_type, X3DViewpointNode * vp,
                                X3DChildNode *topNode, bool detect_collision,
                                const vector< H3DFloat > &avatar_size, 
                                H3DFloat speed ) {
  if( instance ) {
    instance->navigate( navigation_type, vp, topNode,
      detect_collision, avatar_size, speed );
  }
  else {
    instance = new H3DNavigation();
    instance->last_time = Scene::time->getValue();
    instance->navigate( navigation_type, vp, topNode,
      detect_collision, avatar_size, speed );
  }
}

void H3DNavigation::destroy() {
  if( instance )
  {
    delete instance;
    instance = 0;
  }
}

void H3DNavigation::navigate( string navigation_type, X3DViewpointNode * vp,
                              X3DChildNode *topNode, bool detect_collision, 
                              const vector< H3DFloat > &avatar_size, 
                              H3DFloat speed ) {
  Rotation vp_full_orientation = vp->totalOrientation->getValue();
  H3DTime current_time = Scene::time->getValue();
  H3DTime delta_time = current_time - last_time;
  last_time = current_time;

  if( navigation_type == "EXAMINE" || navigation_type == "ANY" ) {
    H3DNavigationDevices::MoveInfo move_info;
    if( H3DNavigationDevices::getMoveInfo( move_info ) ) {
      if( move_info.zoom == true ) {
        Vec3f scaling = vp->accForwardMatrix->getValue().getScalePart();
        if( H3DAbs( scaling.x - scaling.y ) < Constants::f_epsilon
          && H3DAbs( scaling.y - scaling.z ) < Constants::f_epsilon ) {
            vector< H3DFloat > temp_avatar_size( avatar_size );
            for( unsigned int i = 0; i < temp_avatar_size.size(); i++ ) {
              temp_avatar_size[i] *= scaling.x;
            }
            Vec3f direction = vp->centerOfRotation->getValue() - 
                              vp->totalPosition->getValue();
            direction.normalize();
            if( move_info.translation_sum * Vec3f( 0, 0, -1 ) < 0 )
              direction = -direction;
            direction = -vp_full_orientation * direction;
            vp->translate( move_info.translation_sum.length() * direction *
                           speed *
                           delta_time * scaling.x,
              false, temp_avatar_size, topNode );
        } else {
          Console(3) << "Warning: Non-uniform scaling in the"
            << " active X3DViewpointNode ( "
            << vp->getName()
            << " ) nodes local coordinate system. Speed and avatar size of "
            << "Avatar is undefined ";
        }
      } else if( move_info.use_center_sum ) {
        vp->translate( move_info.translation_sum,
                       false, avatar_size, topNode );
        vp->rotateAround( move_info.rotation_sum, false,
                  vp->accInverseMatrix->getValue() * move_info.center_of_rot_sum );
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
    if( H3DAbs( scaling.x - scaling.y ) < Constants::f_epsilon
        && H3DAbs( scaling.y - scaling.z ) < Constants::f_epsilon ) {
      vector< H3DFloat > temp_avatar_size( avatar_size );
      for( unsigned int i = 0; i < temp_avatar_size.size(); i++ ) {
        temp_avatar_size[i] *= scaling.x;
      }
      H3DNavigationDevices::MoveInfo move_info;
      if( H3DNavigationDevices::getMoveInfo( move_info ) ) {
        vp->rotateAroundSelf( move_info.rotation_sum );
        vp->translate( move_info.translation_sum * speed *
                        delta_time * scaling.x,
                       detect_collision, temp_avatar_size, topNode );
      }
      else if( detect_collision )
        vp->detectCollision( temp_avatar_size, topNode );
    } else {
        Console(3) << "Warning: Non-uniform scaling in the"
          << " active X3DViewpointNode ( "
          << vp->getName()
          << " ) nodes local coordinate system. Speed and avatar size of "
          << "Avatar is undefined ";
    }
  } else if( navigation_type == "LOOKAT" ) {
    H3DNavigationDevices::MoveInfo move_info;
    if( H3DNavigationDevices::getMoveInfo( move_info ) ) {

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
      gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
        0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
      Vec3f near_plane_pos =
        Vec3f( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );
      gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
        1.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
      Vec3f far_plane_pos =
        Vec3f( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );

      Node::LineIntersectResult result( true, true );
      if( topNode->lineIntersect( near_plane_pos, 
        far_plane_pos,
        result ) ) {
          int closest = 0;
          result.transformResult();
          if( result.theNodes.size() > 1 ) {
            H3DFloat closestDistance = 
              (H3DFloat)( result.result[closest].point
                          - near_plane_pos ).lengthSqr();
            for( unsigned int kl = 1; kl < result.theNodes.size(); kl++ ) {
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
            Vec3f size = H3DMax( H3DMax( H3DUtil::H3DAbs( acc_inv_scale.x ),
                                         H3DUtil::H3DAbs( acc_inv_scale.y ) ),
                                  H3DUtil::H3DAbs( acc_inv_scale.z ) ) *
              ( H3DMax( H3DMax( H3DUtil::H3DAbs( geom_scale.x ),
                                H3DUtil::H3DAbs( geom_scale.y ) ),
                        H3DUtil::H3DAbs( geom_scale.z ) ) *
              box_bound->size->getValue() );

            viewing_distance = 2 * size.length();
          }
          else {
            // calculate the center and viewing distance
            // since we know nothing about the geometry except
            // the point of intersection this is just a crude guess
            approx_center = vp_acc_inv_mtx *
              ( transform_matrices[closest ] * approx_center );
            viewing_distance = (vp_acc_inv_mtx *( transform_matrices[closest]
            * (Vec3f)result.result[closest].point ) ).length() * 2.0f;
          }
          Vec3f backward = vp_full_orientation * Vec3f( 0, 0, 1 );
          vp->centerOfRotation->setValue( approx_center );

          vp->moveTo( approx_center + viewing_distance * backward );

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

void H3DNavigation::disableDevice( int device ) {
  if( instance )
  {
    switch( device ) {
      case ALL: {
        if( instance->mouse_nav.get() )
          instance->mouse_nav->disableDevice();
        instance->keyboard_nav.reset(0);
        instance->haptic_device_nav.reset(0);
        instance->sws_navigation.reset( 0 );
        break;
      }
      case MOUSE: {
        if( instance->mouse_nav.get() )
          instance->mouse_nav->disableDevice();
        break;
      }
      case KEYBOARD: {
        instance->keyboard_nav.reset( 0 );
        break;
      }
      case HAPTICSDEVICE: {
        instance->haptic_device_nav.reset( 0 );
        break;
      }
      case SWS: {
        instance->sws_navigation.reset( 0 );
        break;
      }
      default: {}
    }
  }
}

void H3DNavigation::enableDevice( int device ) {
  if( !instance ) {
    instance = new H3DNavigation();
    instance->last_time = Scene::time->getValue();
  }
  if( instance )
  {
    switch( device ) {
      case ALL: {
        if( instance->mouse_nav.get() ) {
          instance->mouse_nav->enableDevice();
        }
        else {
          instance->mouse_nav.reset( new MouseNavigation() );
        }
        if( !instance->keyboard_nav.get() )
          instance->keyboard_nav.reset( new KeyboardNavigation() );
        if( !instance->haptic_device_nav.get() )
          instance->haptic_device_nav.reset( new HapticDeviceNavigation() );
        if( !instance->sws_navigation.get() )
          instance->sws_navigation.reset( new SWSNavigation() );
        break;
      }
      case MOUSE: {
        if( instance->mouse_nav.get() ) {
          instance->mouse_nav->enableDevice();
        }
        else {
          instance->mouse_nav.reset( new MouseNavigation() );
        }
        break;
      }
      case KEYBOARD: {
        if( !instance->keyboard_nav.get() )
          instance->keyboard_nav.reset( new KeyboardNavigation() );
        break;
      }
      case HAPTICSDEVICE: {
        if( !instance->haptic_device_nav.get() )
          instance->haptic_device_nav.reset( new HapticDeviceNavigation() );
        break;
      }
      case SWS: {
        if( !instance->sws_navigation.get() )
          instance->sws_navigation.reset( new SWSNavigation() );
        break;
      }
      default: {}
    }
  }
}

bool H3DNavigation::isEnabled( int device ) {
  if( instance )
  {
    switch( device ) {
      case ALL: {
        if( !instance->mouse_nav.get() || !instance->mouse_nav->isEnabled() ) {
           return false;
        }
        if( !instance->keyboard_nav.get() )
          return false;
        if( !instance->haptic_device_nav.get() )
          return false;
        if( !instance->sws_navigation.get() )
          return false;
        return true;
      }
      case MOUSE: {
        if( !instance->mouse_nav.get() || !instance->mouse_nav->isEnabled() ) {
           return false;
        }
        return true;
      }
      case KEYBOARD: {
        if( instance->keyboard_nav.get() )
          return true;
        return false;
      }
      case HAPTICSDEVICE: {
        if( instance->haptic_device_nav.get() )
          return true;
        return false;
      }
      case SWS: {
        if( instance->sws_navigation.get() )
          return true;
        return false;
      }
      default: { return false; }
    }
  }
  return false;
}
