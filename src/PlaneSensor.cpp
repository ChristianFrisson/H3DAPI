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
/// \file PlaneSensor.cpp
/// \brief CPP file for PlaneSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PlaneSensor.h>

using namespace H3D;

H3DNodeDatabase PlaneSensor::database( 
        "PlaneSensor", 
        &(newInstance< PlaneSensor > ),
        typeid( PlaneSensor ),
        &X3DDragSensorNode::database 
        );

namespace PlaneSensorInternals {
  FIELDDB_ELEMENT( PlaneSensor, maxPosition, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaneSensor, minPosition, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaneSensor, offset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaneSensor, translation_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PlaneSensor, axisRotation, INPUT_OUTPUT );
}

PlaneSensor::PlaneSensor( Inst< SFBool >  _autoOffset,
                          Inst< SFString > _description,
                          Inst< SFBool >  _enabled,
                          Inst< SFVec2f > _maxPosition,
                          Inst< SFNode >  _metadata,
                          Inst< SFVec2f > _minPosition,
                          Inst< SFOffset > _offset,
                          Inst< SFBool >  _isActive,
                          Inst< SFBool > _isOver,
                          Inst< SFVec3f >  _trackPoint_changed,
                          Inst< SFVec3f >  _translation_changed,
                          Inst< SFRotation > _axisRotation ) :
  X3DDragSensorNode( _autoOffset, _description, _enabled,
                     _metadata, _isActive, _isOver, _trackPoint_changed ),
  maxPosition ( _maxPosition  ),
  minPosition( _minPosition ),
  offset( _offset ),
  translation_changed( _translation_changed ),
  axisRotation( _axisRotation ),
  new_plane( true ),
  plane_normal( Vec3f( 0, 0, 1) ) {

  type_name = "PlaneSensor";
  database.initFields( this );

  maxPosition->setValue( Vec2f( -1, -1 ) );
  minPosition->setValue( Vec2f( 0, 0 ) );
  offset->setValue( Vec3f( 0, 0, 0 ) );
  axisRotation->setValue( Rotation( 0, 0, 1, 0 ), id );
}

// Destructor. 
PlaneSensor::~PlaneSensor() {
}

void PlaneSensor::onIsOver( IntersectionInfo *result,
                            Matrix4f *global_to_local ) {
  if( is_enabled && ( isActive->getValue() || number_of_active == 0 ) ) {
    X3DPointingDeviceSensorNode::onIsOver( result,
                                           global_to_local );
    if( new_value ) {
      geometry_global_to_local = *global_to_local;
      geometry_intersection =
        geometry_global_to_local * Vec3f( result->point );
    }
  }
}

int PlaneSensor::intersectLinePlane(
  Vec3f a, Vec3f b, float &t, Vec3f &q ) {
  // Compute the t value for the directed line ab intersecting the plane
  Vec3f ab = b - a;
  H3DFloat s =  plane_normal * ab;
  H3DFloat plane_normal_a = plane_normal * a;
  if( H3DUtil::H3DAbs( s ) < Constants::f_epsilon ) {
    H3DFloat t2 = plane_normal_a - plane_d;
    if( H3DUtil::H3DAbs( t2 ) < Constants::f_epsilon ) {
      t = 0;
      q = a;
      return 1;
    }
    return 0;
  } else {
    t = ( plane_d - plane_normal_a ) / s;
    q = a + t * ab;
    if ( t >= 0 && t <= 1 ) {
      return 1;
    } else {
      // Intersection not in view frustum: 
      // t < 0: pointer may be "over the horizon" of the local sensor
      // coordinate system's Z-plane or below its intersection with the near
      // clipping plane. Thus the intersection point lies in front of the near
      // clipping plane
      // t > 1: intersection point is behind far clipping plane
      return 0;
    }
  }
}

void PlaneSensor::setDragOutputEvents( bool _enabled,
                                       const Vec3f &from,
                                       const Vec3f &to ) {
  if( _enabled ) {
    if( isActive->getValue() ) {
      Matrix4f axis_rotation_matrix = Matrix4f( axisRotation->getValue() );
      Matrix4f axis_rotation_matrix_inverse = axis_rotation_matrix.transformInverse();
      if( new_plane ) {
        // Initialize variables used when active.
        send_warning_message = true;
        last_intersection = axis_rotation_matrix_inverse * geometry_intersection;
        active_global_to_local_matrix = geometry_global_to_local;
        new_plane = false;
        plane_d = plane_normal * last_intersection;
        // According to section 20.4.2 of the X3D specification
        // translation_changed and trackPoint_changed events should not be sent
        // at first activation:
        // "For each subsequent movement of the bearing, a translation_changed
        // event is output which corresponds to the sum of the relative
        // translation from the original intersection point to the intersection
        // point of the new bearing in the plane plus the offset value. The
        // sign of the translation is defined by the Z=0 plane of the local
        // sensor coordinate system. trackPoint_changed events reflect the
        // unclamped drag position on the surface of this plane."
      } else {
        // Calculate intersection and send events.
        H3DFloat t;
        Vec3f intersectionPoint;
        Matrix4f global_to_local =
          axis_rotation_matrix_inverse * active_global_to_local_matrix;
        if( intersectLinePlane(
              global_to_local * from,
              global_to_local * to,
              t, intersectionPoint ) ) {
          send_warning_message = true;
          Vec3f trans_changed = intersectionPoint - last_intersection
                                + axis_rotation_matrix_inverse * offset->getValue();
          Vec2f min_position = minPosition->getValue();
          Vec2f max_position = maxPosition->getValue();
          if( min_position.x <= max_position.x ) {
            trans_changed.x = Clamp( trans_changed.x,
                                     min_position.x,
                                     max_position.x );
          }
          if( min_position.y <= max_position.y ) {
            trans_changed.y = Clamp( trans_changed.y,
                                     min_position.y,
                                     max_position.y );
          }
          trans_changed.z = 0.0f;
          // transform from local sensor coordinate to local coordinate system
          trackPoint_changed->setValue( axis_rotation_matrix * intersectionPoint, id );
          translation_changed->setValue( axis_rotation_matrix * trans_changed , id );
        } else {
          if( send_warning_message ) {
            // X3D specification states that in the case of no plane
            // intersection "browsers may interpret this in a variety of ways"
            // which means doing whatever feels natural.
            // H3DAPI resends last event.
            Console(3) << "Outside the plane due to near- and farplane" <<
                    " clipping or other reason, last event resent." << endl;
            send_warning_message = false;
          }
          trackPoint_changed->touch();
          translation_changed->touch();
        }
      }
    } else {
      // Reset variables when not active anymore.
      if( !new_plane ) {
        new_plane = true;
        if( autoOffset->getValue() )
          offset->setValue( translation_changed->getValue(), id );
      }
    }
  }
}
