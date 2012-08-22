//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2009, SenseGraphics AB
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
/// \file CylinderSensor.cpp
/// \brief CPP file for CylinderSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CylinderSensor.h>

using namespace H3D;

H3DNodeDatabase CylinderSensor::database( 
        "CylinderSensor", 
        &(newInstance< CylinderSensor > ),
        typeid( CylinderSensor ),
        &X3DDragSensorNode::database 
        );

namespace CylinderSensorInternals {
  FIELDDB_ELEMENT( CylinderSensor, diskAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CylinderSensor, maxAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CylinderSensor, minAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CylinderSensor, offset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CylinderSensor, rotation_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( CylinderSensor, axisRotation, INPUT_OUTPUT );
}

CylinderSensor::CylinderSensor(
                      Inst< SFBool >  _autoOffset,
                      Inst< SFString > _description,
                      Inst< SFFloat > _diskAngle,
                      Inst< SFBool >  _enabled,
                      Inst< CheckAngleLimits > _maxAngle,
                      Inst< SFNode >  _metadata,
                      Inst< CheckAngleLimits > _minAngle,
                      Inst< SFFloat > _offset,
                      Inst< SFBool >  _isActive,
                      Inst< SFBool > _isOver,
                      Inst< SFRotation >  _rotation_changed,
                      Inst< SFVec3f >  _trackPoint_changed,
                      Inst< SFRotation > _axisRotation ) :
  X3DDragSensorNode( _autoOffset, _description, _enabled,
                     _metadata, _isActive, _isOver, _trackPoint_changed ),
  diskAngle( _diskAngle ),
  maxAngle( _maxAngle ),
  minAngle( _minAngle ),
  offset( _offset ),
  rotation_changed( _rotation_changed ),
  axisRotation( _axisRotation ),
  new_cylinder( true ),
	prev_new_cylinder( true ),
  y_axis( Vec3f( 0, 1, 0 ) ),
  plane_d( 0.0f ) {

  type_name = "CylinderSensor";
  database.initFields( this );

  diskAngle->setValue( (H3DFloat)(Constants::pi / 12 ) );
  maxAngle->setValue( -1 );
  minAngle->setValue( 0 );
  offset->setValue( 0 );
  axisRotation->setValue( Rotation( 0, 0, 1, 0 ), id );
}

/// Destructor. 
CylinderSensor::~CylinderSensor() {
}

void CylinderSensor::onIsOver( IntersectionInfo *result,
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

int CylinderSensor::intersectSegmentCylinder( 
  Vec3f sa, Vec3f sb, float &t ) {
  Vec3f d = cb - ca, m = sa - ca, n = sb - sa;

  H3DFloat md = m * d;
  H3DFloat nd = n * d;
  H3DFloat dd = d * d;
  // Test if segment fully outside either endcap of cylinder
  if( md < 0.0f && md + nd < 0.0f )
    return 0; // Segment outside 'ca' side of cylinder
  if( md > dd && md + nd > dd )
    return 0; // Segment outside 'cb' side of cylinder
  H3DFloat nn = n * n;
  H3DFloat mn = m * n;
  H3DFloat a = dd * nn - nd * nd;
  H3DFloat k = m * m - radius * radius;
  H3DFloat c = dd * k - md * md;
  if( H3DAbs(a) < Constants::f_epsilon ) {
    // Segment runs parallel to cylinder axis
    if( c > 0.0f )
      return 0; // 'sa' and thus the segment lie outside the cylinder
    // Now known that segment intersects cylinder; figure out how it intersects
    if( md < 0.0f )
      t = -mn / nn; // Intersect segment against 'ca' endcap
    else if( md > dd )
      t = ( nd - mn ) / nn; // Intersect segment against 'cb' encap
    else t = 0.0f; // a lies inside cylinder
    return 1;
  }
  H3DFloat b = dd * mn - nd * md;
  H3DFloat discr = b * b - a * c;
  if( discr < 0.0f ) return 0; // No real roots; no intersection
  t = ( -b - H3DSqrt(discr) ) / a;
  // NOTE: This code is corrected using errata on
  // http://realtimecollisiondetection.net/books/rtcd/errata/
  // page. Parts of the fix is not completely verified, if the code below
  // cause trouble investigate this.
  H3DFloat t0 = t = (-b - H3DUtil::H3DSqrt( discr ) ) / a;
  if( md + t * nd < 0.0f ) {
    // Intersection outside cylinder on 'ca' side
    if( nd <= 0.0f ) return 0; // Segment pointing away from endcap
    t = -md / nd;
    // Keep intersection if Dot(S(t) - ca, S(t) - ca ) <= r^2
    return k + t * (2.0f * mn + t * nn ) <= 0.0f;
  } else if( md + t * nd > dd ) {
    // Intersection outside cylinder on 'cb' side
    if( nd >= 0.0f ) return 0; // Segment pointing away from endcap
    t = ( dd - md ) / nd;
    // Keep intersection if Dot(S(t) - cb, S(t) - cb ) < r^2
    return k + dd - 2.0f * md + t * (2.0f * ( mn - nd ) + t * nn ) <= 0.0f;
  }
  t = t0;
  // Intersection if segment intersects cylinder between the end-caps.
  return t >= 0.0f && t <= 1.0f;
}

int CylinderSensor::intersectLinePlane(
  Vec3f a, Vec3f b, float &t, Vec3f &q ) {
  // Compute the t value for the directed line ab intersecting the plane
  Vec3f ab = b - a;
  t = (plane_d - y_axis * a ) / ( y_axis * ab );

  q = a + t * ab;
  return 1;
}

void CylinderSensor::setDragOutputEvents( bool _enabled,
                                          const Vec3f &from,
                                          const Vec3f &to ) {
  if( _enabled ) {
    if( isActive->getValue() ) {
      Matrix4f axis_rotation_matrix = Matrix4f( axisRotation->getValue() );
      Matrix4f axis_rotation_matrix_inverse = axis_rotation_matrix.transformInverse();
      if( new_cylinder ) {
        // Initialize variabels needed for outputting events.
        send_warning_message = true;
        geometry_global_to_local_original = geometry_global_to_local;
        Vec3f bearing = geometry_global_to_local_original * from -
                        geometry_global_to_local_original * to;
        bearing.normalize();
        Vec3f new_y_axis = axis_rotation_matrix * y_axis;
        H3DFloat dot_product = new_y_axis * bearing;
        use_caps = 
          H3DAcos( H3DAbs( dot_product ) ) < diskAngle->getValue();

        // create a new invisible cylinder if a cylinder will be used.
        last_intersection = axis_rotation_matrix_inverse * geometry_intersection;
        last_intersection.y = 0.0f;
        if( !use_caps )
          radius = last_intersection.length();

        last_intersection.normalizeSafe();
        // According to section 20.4.1 of the X3D specification
        // trackPoint_changed and rotation_changed events should not be sent at
        // first activation:
        // "For each SUBSEQUENT position of the bearing, a rotation_changed
        // event is sent that equals the sum of the rotation about the +Y-axis
        // vector of the local sensor coordinate system (from the initial
        // intersection to the new intersection) plus the offset value.
        // trackPoint_changed events reflect the unclamped drag position on the
        // surface of this disk."
				prev_new_cylinder = new_cylinder;
        new_cylinder = false;
      } else {
        // Check intersection with cylinder or plane.
        H3DFloat t;
        Vec3f intersectionPoint;
        bool intersected = false;
        Matrix4f global_to_local = axis_rotation_matrix_inverse *
                                   geometry_global_to_local_original;
        if( use_caps ) {
          if( intersectLinePlane( global_to_local * from,
                                  global_to_local * to,
                                  t, intersectionPoint ) )
            intersected = true;
        } else {
          Vec3f nearPlaneTransformed = global_to_local * from;
          Vec3f farPlaneTransformed = global_to_local * to;

          // The cylinder is along the y_axis and we do want it to be long
          // enough to intersect the segment.
          H3DFloat theMaxValue = H3DMax( H3DAbs( nearPlaneTransformed.y ),
                                         H3DAbs( farPlaneTransformed.y ) );
          ca = Vec3f( 0.0f, -theMaxValue - 1.0f, 0.0f );
          cb = Vec3f( 0.0f, theMaxValue + 1.0f, 0.0f );
          // sometimes the nearplane and farplane is outside the radius of the
          // cylinder and in that case there will be no intersection. So
          // we move the nearplane and farplane to allow for correct
          // intersection. This of course only affects intersection with the
          // invisible cylinder.
          Vec3f tempSBSA = farPlaneTransformed - nearPlaneTransformed;
          tempSBSA.normalize();
          nearPlaneTransformed = nearPlaneTransformed - tempSBSA * 1.1*radius;
          farPlaneTransformed = farPlaneTransformed + tempSBSA * 1.1*radius;

          if( intersectSegmentCylinder( nearPlaneTransformed,
                                        farPlaneTransformed,
                                        t ) ) {
            intersectionPoint = nearPlaneTransformed
              + t * ( farPlaneTransformed - nearPlaneTransformed );
            intersected = true;
          }
        }

        // if intersection send events.
        if( intersected ) {
          send_warning_message = true;
          trackPoint_changed->setValue( axis_rotation_matrix *
                                        intersectionPoint, id );
          intersectionPoint.y = 0.0f;
          intersectionPoint.normalizeSafe();
          H3DFloat angle = 0;

          // need to clamp the calculated angle because sometimes the scalar
          // product return a value slightly above 1 or below -1 which means
          // that H3DAcos returns an undefined value.
          if( (last_intersection % intersectionPoint).y < 0.0f ) {
            angle = -H3DAcos( Clamp( last_intersection *
                                         intersectionPoint, -1, 1 ) );
          } else {
            angle = H3DAcos( Clamp( last_intersection * 
                                        intersectionPoint, -1, 1 ) );
          }

          last_intersection = intersectionPoint;

          H3DFloat min_angle = minAngle->getValue();
          H3DFloat max_angle = maxAngle->getValue();
          
          angle += rotation_changed->getValue().angle;
          if( min_angle <= max_angle )
            angle = Clamp( angle, min_angle, max_angle );

					if( prev_new_cylinder )
						rotation_changed->setValue( Rotation( axis_rotation_matrix * y_axis,
							                                    angle ) *
																				Rotation( y_axis, offset->getValue() ),
								                        id );
					else
						rotation_changed->setValue( Rotation( axis_rotation_matrix * y_axis,
							                                    angle ),
								                        id );
        } else {
          if( send_warning_message ) {
            // X3D specification states that in the case of no Cylinder
            // (or plane) intersection "browsers may interpret this in a
            // variety of ways" which means doing whatever feels natural.
            // H3DAPI resends last event.
            Console(3) << "Warning: No intersection with invisible "
                       << ( use_caps ? "plane" : "cylinder" )
                       << " in CylinderSensor node( "
                       << getName() 
                       << " ). Last event resent." << endl;
            send_warning_message = false;
          }
          trackPoint_changed->touch();
					if( prev_new_cylinder )
						rotation_changed->setValue( Rotation( y_axis, offset->getValue() ),
																				id );
					else
						rotation_changed->touch();
        }
				prev_new_cylinder = new_cylinder;
      }
    } else {
      // not active anymore
      if( !new_cylinder ) {
				prev_new_cylinder = new_cylinder;
        new_cylinder = true;
        if( autoOffset->getValue() )
          offset->setValue( rotation_changed->getValue().angle,
                                id );
      }
    }
  }
}
