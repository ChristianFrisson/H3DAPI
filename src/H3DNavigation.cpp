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

#include <H3DNavigation.h>
#include <X3DGeometryNode.h>
#include <Scene.h>

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
  const Matrix4f &acc_fr_mt = vp->accForwardMatrix->getValue();
  Vec3f vp_full_pos = vp->getFullPos();
  Rotation vp_full_orientation = vp->getFullOrn();
  H3DTime current_time = Scene::time->getValue();
  H3DTime delta_time = current_time - last_time;
  last_time = current_time;

  if( navigation_type == "EXAMINE" || navigation_type == "ANY" ) {
    Vec3f translation_delta;
    Rotation rotation_value;
    Vec3f center_of_rot;
    bool use_center;
    if( H3DNavigationDevices::getMoveInfo(
        translation_delta, rotation_value, center_of_rot, use_center ) ) {
      if( use_center ) {
        vp->translate( translation_delta, false, avatar_size, topNode );
        vp->rotateAround( rotation_value, false,
                  vp->accInverseMatrix->getValue() * center_of_rot );
      }
      else {
        vp->rotateAround( rotation_value, false,
                          vp->centerOfRotation->getValue() );
      }
    }
    else if( detect_collision )
      vp->detectCollision( avatar_size, topNode );
  }
  else if( navigation_type == "WALK" ) {
    Vec3f translation_delta;
    Rotation rotation_value;
    Vec3f center_of_rot;
    bool use_center;
    if( H3DNavigationDevices::getMoveInfo(
          translation_delta, rotation_value, center_of_rot, use_center ) ) {
      vp->rotateAroundSelf( rotation_value );

      Vec3f scaling = vp->accForwardMatrix->getValue().getScalePart();
      if( H3DAbs( scaling.x - scaling.y ) < Constants::f_epsilon
        && H3DAbs( scaling.y - scaling.z ) < Constants::f_epsilon ) {
        vp->translate( translation_delta * speed * delta_time * scaling.x,
          true, avatar_size, topNode );
      }
      else {
        Console(3) << "Warning: Non-uniform scaling in the"
          << " active X3DViewpointNode ( "
          << vp->getName()
          << " ) nodes local coordinate system. Speed of "
          << "Avatar is undefined ";
      }
    }
    else if( detect_collision )
      vp->detectCollision( avatar_size, topNode );
  }
  else if( navigation_type == "FLY" ) {
    Vec3f translation_delta;
    Rotation rotation_value;
    Vec3f center_of_rot;
    bool use_center;
    if( H3DNavigationDevices::getMoveInfo(
          translation_delta, rotation_value, center_of_rot, use_center ) ) {
      vp->rotateAroundSelf( rotation_value );
      Vec3f scaling = vp->accForwardMatrix->getValue().getScalePart();
      if( H3DAbs( scaling.x - scaling.y ) < Constants::f_epsilon
        && H3DAbs( scaling.y - scaling.z ) < Constants::f_epsilon ) {
        vp->translate( translation_delta * speed * delta_time * scaling.x,
          true, avatar_size, topNode );
      }
      else {
        Console(3) << "Warning: Non-uniform scaling in the"
          << " active X3DViewpointNode ( "
          << vp->getName()
          << " ) nodes local coordinate system. Speed of "
          << "Avatar is undefined ";
      }
    }
    else if( detect_collision )
      vp->detectCollision( avatar_size, topNode );
  }
  else if( navigation_type == "LOOKAT" ) {
    Vec3f translation_delta;
    Rotation rotation_value;
    Vec3f center_of_rot;
    bool use_center;
    if( H3DNavigationDevices::getMoveInfo(
          translation_delta, rotation_value, center_of_rot, use_center ) ) {

      // TODO: maybe change this to allow for other devices to specify what
      // to look at ( for example by supply a point and direction ).
      GLint viewport[4];
      GLdouble mvmatrix[16], projmatrix[16];
      GLdouble wx, wy, wz;
      glGetIntegerv( GL_VIEWPORT, viewport );
      glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
      glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );

      Vec2f mouse_pos = Vec2f( translation_delta.x, translation_delta.y );
      mouse_pos.y = viewport[3] - mouse_pos.y - 1;
      gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
        0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
      Vec3f near_plane_pos =
        Vec3f( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );
      gluUnProject( (GLdouble) mouse_pos.x, (GLdouble) mouse_pos.y,
        1.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
      Vec3f far_plane_pos =
        Vec3f( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );

      vector< HAPI::Bounds::IntersectionInfo > result;
      vector< pair< Node *, H3DInt32 > > theNodes;
      Matrix4f temp_matrix;
      vector< Matrix4f > transform_matrices;
      if( topNode->lineIntersect( near_plane_pos, 
        far_plane_pos,
        result,
        theNodes,
        temp_matrix,
        transform_matrices ) ) {
          int closest = 0;
          if( theNodes.size() > 1 ) {
            H3DFloat closestDistance = 
              (H3DFloat)(result[closest].point - near_plane_pos).lengthSqr();
            for( unsigned int kl = 1; kl < theNodes.size(); kl++ ) {
              H3DFloat tempClose = 
                (H3DFloat)(result[kl].point - near_plane_pos).lengthSqr();
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
            dynamic_cast< H3DBoundedObject * >( theNodes[closest].first );
          if( the_bound_object ) {
            box_bound = dynamic_cast< BoxBound * >(
            the_bound_object->bound->getValue() );
          }
          const Matrix4f &vp_acc_inv_mtx = vp->accInverseMatrix->getValue();

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
          }
          else {
            // calculate the center and viewing distance
            // since we know nothing about the geometry except
            // the point of intersection this is just a crude guess
            approx_center = vp_acc_inv_mtx *
              ( transform_matrices[closest ] * approx_center );
            viewing_distance = (vp_acc_inv_mtx *( transform_matrices[closest]
            * result[closest].point ) ).length() * 2.0f;
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
