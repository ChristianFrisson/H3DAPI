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
  set_SphereEvents( new Set_SphereEvents ) {

  type_name = "SphereSensor";
  database.initFields( this );

  offset->setValue( Rotation( 0, 1, 0, 0 ) );

  set_SphereEvents->setOwner( this );
  mouse_sensor->position->routeNoEvent( set_SphereEvents );
  isActive->routeNoEvent( set_SphereEvents );
}

void SphereSensor::onIsOver( bool newValue,
                           IntersectionInfo &result,
                           int pt_id ) {
  if( is_enabled && ( isActive->getValue() || number_of_active == 0 ) ) {
    X3DPointingDeviceSensorNode::onIsOver( newValue,
                                           result,
                                           pt_id );
    if( newValue ) {
      intersection_matrix = pt_matrices[pt_id];
      original_intersection = intersection_matrix * Vec3f( result.point );
    }
  }
}

int SphereSensor::Set_SphereEvents::intersectSegmentSphere( 
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

void SphereSensor::Set_SphereEvents::update() {
  SFBool::update();
  SphereSensor *ss = 
    static_cast< SphereSensor * >( getOwner() );
  if( ss->enabled->getValue() ) {
    bool isActive = static_cast< SFBool * >(routes_in[1])->getValue();
    if( isActive ) {
      if( new_radius ) {
        original_intersection = ss->original_intersection;
        original_transform_matrix = ss->intersection_matrix;
        new_radius = false;
        radius = original_intersection.length();
        ss->trackPoint_changed->setValue( original_intersection, ss->id );
        ss->rotation_changed->setValue( Rotation( 0, 1, 0, 0 ) * 
                                        ss->offset->getValue(), ss->id );
        original_intersection.normalize();
      }
      else {
        H3DFloat t;
        Vec3f intersectionPoint;
        if( intersectSegmentSphere( 
                                 original_transform_matrix * near_plane_pos,
                                 original_transform_matrix * far_plane_pos,
                                 t, intersectionPoint ) ) {
          ss->trackPoint_changed->setValue( intersectionPoint, ss->id );
          intersectionPoint.normalize();
          ss->rotation_changed->setValue( Rotation( original_intersection,
                                                    intersectionPoint) *
                                          ss->offset->getValue(), ss->id );
        }
        else {
          // X3D specification states that in the case of no sphere
          // intersection "browsers may interpret this in a variety of ways"
          // which means doing whatever feels natural.
          // H3DAPI resends last event.
          Console(3) << "Warning: No intersection with invisible sphere"
                     << " in SphereSensor node( "
				             << ss->getName() 
                     << " ). Last event resent." << endl;
          ss->trackPoint_changed->touch();
          ss->rotation_changed->touch();
        }
      }
    }
    else {
      if( !new_radius ) {
        new_radius = true;
        if( ss->autoOffset->getValue() )
          ss->offset->setValue( ss->rotation_changed->getValue(), ss->id );
      }
    }
  }
}
