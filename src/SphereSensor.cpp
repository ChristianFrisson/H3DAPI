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
/// \file SphereSensor.cpp
/// \brief CPP file for SphereSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SphereSensor.h>

using namespace H3D;

H3DNodeDatabase SphereSensor::database( 
        "SphereSensor", 
        &(newInstance< SphereSensor > ),
        typeid( SphereSensor ),
        &X3DDragSensorNode::database 
        );

namespace SphereSensorInternals {
  FIELDDB_ELEMENT( SphereSensor, offset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SphereSensor, rotation_changed, OUTPUT_ONLY );
}

SphereSensor::SphereSensor(
                      Inst< SFBool >  _autoOffset,
                      Inst< SFString > _description,
                      Inst< SFBool >  _enabled,
                      Inst< SFNode >  _metadata,
                      Inst< SFRotation > _offset,
                      Inst< SFBool >  _isActive,
                      Inst< SFBool > _isOver,
                      Inst< SFRotation >  _rotation_changed,
                      Inst< SFVec3f >  _trackPoint_changed ) :
  X3DDragSensorNode( _autoOffset, _description, _enabled,
                     _metadata, _isActive, _isOver, _trackPoint_changed ),
  offset( _offset ),
  rotation_changed( _rotation_changed ),
  new_radius( true ),
  prev_new_radius( true ),
  center( Vec3f( 0, 0, 0 ) ) {

  type_name = "SphereSensor";
  database.initFields( this );

  offset->setValue( Rotation( 0, 1, 0, 0 ) );
}

void SphereSensor::onIsOver( IntersectionInfo *result,
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

int SphereSensor::intersectSegmentSphere( 
  Vec3f a1, Vec3f a2, H3DFloat & t, Vec3f &q ) {
    Vec3f a1a2 = a2 - a1;
    H3DFloat length = a1a2.length();
    Vec3f d = a1a2 / length;
    Vec3f m = a1 - center;
    H3DFloat b = m * d;
    H3DFloat c = m * m - radius * radius;

    // Exit if r's origin outside sp (c > 0) and r pointing away from s (b > 0)
    if( c > 0.0f && b > 0.0f) return 0;
    float discr = b*b - c;
    // A negative discriminant corresponds to ray missing sphere
    if( discr < 0.0f ) return 0;
    // Segment now found to intersect sphere,
    // compute smallest t value of intersection
    t = -b - H3DSqrt( discr );
    // if t > length intersection is outside segment
    if( t > length ) return 0;
    // If t is negative, segment started inside sphere so clamp to to zero.
    if( t < 0.0f ) t = 0.0f;
    q = a1 + t * d;

    return 1;
}

void SphereSensor::setDragOutputEvents( bool _enabled,
                                        const Vec3f &from,
                                        const Vec3f &to ) {
  if( _enabled ) {
    if( isActive->getValue() ) {
      if( new_radius ) {
        // Initialize variables used when active.
        send_warning_message = true;
        last_intersection = geometry_intersection;
        active_global_to_local_matrix = geometry_global_to_local;
        prev_new_radius = new_radius;
        new_radius = false;
        radius = last_intersection.length();
        // I interpret the text in 20.4.3 in the X3D specification in the same
        // was as 20.4.1 and 20.4.2 when to send the events. Those should only
        // be sent for subsequent operations, not at activation.
        last_intersection.normalize();
      } else {
        // Calculate intersection and send events.
        H3DFloat t;
        Vec3f intersectionPoint;
        if( intersectSegmentSphere( 
                                 active_global_to_local_matrix * from,
                                 active_global_to_local_matrix * to,
                                 t, intersectionPoint ) ) {
          send_warning_message = true;
          trackPoint_changed->setValue( intersectionPoint, id );
          intersectionPoint.normalize();
          rotation_changed->setValue( Rotation( last_intersection,
                                                    intersectionPoint) *
                                          offset->getValue(), id );
        } else {
          if( send_warning_message ) {
            // X3D specification states that in the case of no sphere
            // intersection "browsers may interpret this in a variety of ways"
            // which means doing whatever feels natural.
            // H3DAPI resends last event.
            Console(3) << "Warning: No intersection with invisible sphere"
                       << " in SphereSensor node( "
                       << getName() 
                       << " ). Last event resent." << endl;
            send_warning_message = false;
          }
          trackPoint_changed->touch();
          if( prev_new_radius )
             rotation_changed->setValue( offset->getValue(), id );
          else
            rotation_changed->touch();
        }
        prev_new_radius = new_radius;
      }
    } else {
      // Reset variables when isActive is set to false after being true.
      if( !new_radius ) {
        prev_new_radius = new_radius;
        new_radius = true;
        if( autoOffset->getValue() )
          offset->setValue( rotation_changed->getValue(), id );
      }
    }
  }
}
