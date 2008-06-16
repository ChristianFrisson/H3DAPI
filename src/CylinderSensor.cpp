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
}

CylinderSensor::CylinderSensor(
                      Inst< SFBool >  _autoOffset,
                      Inst< SFString > _description,
                      Inst< SFFloat > _diskAngle,
                      Inst< SFBool >  _enabled,
                      Inst< Check_Angle_Limits > _maxAngle,
                      Inst< SFNode >  _metadata,
                      Inst< Check_Angle_Limits > _minAngle,
                      Inst< SFFloat > _offset,
                      Inst< SFBool >  _isActive,
                      Inst< SFBool > _isOver,
                      Inst< SFRotation >  _rotation_changed,
                      Inst< SFVec3f >  _trackPoint_changed ) :
  X3DDragSensorNode( _autoOffset, _description, _enabled,
                     _metadata, _isActive, _isOver, _trackPoint_changed ),
  diskAngle( _diskAngle ),
  maxAngle( _maxAngle ),
  minAngle( _minAngle ),
  offset( _offset ),
  rotation_changed( _rotation_changed ),
  set_CylinderEvents( new Set_CylinderEvents ) {

  type_name = "CylinderSensor";
  database.initFields( this );

  diskAngle->setValue( (H3DFloat)(Constants::pi / 12 ) );
  maxAngle->setValue( -1 );
  minAngle->setValue( 0 );
  offset->setValue( 0 );

  set_CylinderEvents->setOwner( this );
  set_CylinderEvents->setName( "set_CylinderEvents" );
  mouse_sensor->position->routeNoEvent( set_CylinderEvents );
  isActive->routeNoEvent( set_CylinderEvents );
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
      intersection_matrix = *global_to_local;
      intersection_point = intersection_matrix * Vec3f( result->point );
    }
  }
}

int CylinderSensor::Set_CylinderEvents::intersectSegmentCylinder( 
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
  if( t < 0.0f || t > 1.0f ) return 0; // Intersection lies outside segment
  if( md + t * nd < 0.0f ) {
    // Intersection outside cylinder on 'ca' side
    if( nd <= 0.0f ) return 0; // Segment pointing away from endcap
    t = -md / nd;
    // Keep intersection if Dot(S(t) - ca, S(t) - ca ) <= r^2
    return k + 2 * t * ( mn + t * nn ) <= 0.0f;
  } else if( md + t * nd > dd ) {
    // Intersection outside cylinder on 'cb' side
    if( nd >= 0.0f ) return 0; // Segment pointing away from endcap
    t = ( dd - md ) / nd;
    // Keep intersection if Dot(S(t) - cb, S(t) - cb ) < r^2
    return k + dd - 2 * md + t * (2 * ( mn - nd ) + t * nn ) <= 0.0f;
  }
  // Segment intersect cylinder between the endcaps; t is correct
  return 1;
}

int CylinderSensor::Set_CylinderEvents::intersectLinePlane(
  Vec3f a, Vec3f b, float &t, Vec3f &q ) {
  // Compute the t value for the directed line ab intersecting the plane
  Vec3f ab = b - a;
  t = (plane_d - y_axis * a ) / ( y_axis * ab );

  q = a + t * ab;
  return 1;
}

void CylinderSensor::Set_CylinderEvents::update() {
  SFBool::update();
  CylinderSensor *cs = 
    static_cast< CylinderSensor * >( getOwner() );
  if( cs->enabled->getValue() ) {
    bool isActive = static_cast< SFBool * >(routes_in[1])->getValue();
    if( isActive ) {
      if( new_cylinder ) {
        original_transform_matrix = cs->intersection_matrix;
        Vec3f bearing = original_transform_matrix * far_plane_pos -
                        original_transform_matrix * near_plane_pos;
        bearing.normalize();
        H3DFloat dot_product = y_axis * bearing;
        use_caps = 
          H3DAcos( H3DAbs( dot_product ) ) < cs->diskAngle->getValue();

        // create a new invisible cylinder if a cylinder will be used.
        original_intersection = cs->intersection_point;
        original_intersection.y = 0.0f;
        if( !use_caps )
          radius = original_intersection.length();

        original_intersection.normalizeSafe();
        cs->trackPoint_changed->setValue( original_intersection, cs->id );
        cs->rotation_changed->setValue( Rotation( y_axis, 
                                                  cs->offset->getValue() ),
                                        cs->id );
        new_cylinder = false;
      }
      else {
        // Check intersection with cylinder or plane.
        H3DFloat t;
        Vec3f intersectionPoint;
        bool intersected = false;
        if( use_caps && 
            intersectLinePlane( 
              original_transform_matrix * near_plane_pos,
              original_transform_matrix * far_plane_pos,
              t, intersectionPoint ) ) {
          intersected = true;
        }
        else {
          Vec3f nearPlaneTransformed = original_transform_matrix * 
                                       near_plane_pos;
          Vec3f farPlaneTransformed = original_transform_matrix *
                                      far_plane_pos;

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
          cs->trackPoint_changed->setValue( intersectionPoint, cs->id );
          intersectionPoint.y = 0.0f;
          intersectionPoint.normalizeSafe();
          H3DFloat angle = 0;

          // need to clamp the calculated angle because sometimes the scalar
          // product return a value slightly above 1 or below -1 which means
          // that H3DAcos returns an undefined value.
          if( (original_intersection % intersectionPoint).y < 0.0f ) {
            angle = -H3DAcos( cs->Clamp( original_intersection *
                                         intersectionPoint, -1, 1 ) );
          }
          else {
            angle = H3DAcos( cs->Clamp( original_intersection * 
                                        intersectionPoint, -1, 1 ) );
          }

          original_intersection = intersectionPoint;

          H3DFloat minAngle = cs->minAngle->getValue();
          H3DFloat maxAngle = cs->maxAngle->getValue();
          
          angle += cs->rotation_changed->getValue().angle;
          if( minAngle <= maxAngle )
            angle = cs->Clamp( angle, minAngle, maxAngle );

          cs->rotation_changed->setValue( Rotation( y_axis, angle ),
                                          cs->id );
        } else {
          // X3D specification states that in the case of no Cylinder(or plane)
          // intersection "browsers may interpret this in a variety of ways"
          // which means doing whatever feels natural.
          // H3DAPI resends last event.
          Console(3) << "Warning: No intersection with invisible cylinder"
                     << " in CylinderSensor node( "
                     << cs->getName() 
                     << " ). Last event resent." << endl;
          cs->trackPoint_changed->touch();
          cs->rotation_changed->touch();
        }
      }
    }
    else {
      // not active anymore
      if( !new_cylinder ) {
        new_cylinder = true;
        if( cs->autoOffset->getValue() )
          cs->offset->setValue( cs->rotation_changed->getValue().angle,
                                cs->id );
      }
    }
  }
}
