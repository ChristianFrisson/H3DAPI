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
/// \file PlaneSensor.cpp
/// \brief CPP file for PlaneSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <PlaneSensor.h>

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
}

PlaneSensor::PlaneSensor(
                              Inst< SFBool >  _autoOffset,
                      Inst< SFString > _description,
                      Inst< SFBool >  _enabled,
                      Inst< SFVec2f > _maxPosition,
                      Inst< SFNode >  _metadata,
                      Inst< SFVec2f > _minPosition,
                      Inst< SFVec3f > _offset,
                      Inst< SFBool >  _isActive,
                      Inst< SFBool > _isOver,
                      Inst< SFVec3f >  _trackPoint_changed,
                      Inst< SFVec3f >  _translation_changed ) :
  X3DDragSensorNode( _autoOffset, _description, _enabled,
                     _metadata, _isActive, _isOver, _trackPoint_changed ),
  maxPosition ( _maxPosition  ),
  minPosition( _minPosition ),
  offset( _offset ),
  translation_changed( _translation_changed ),
  set_PlaneEvents( new Set_PlaneEvents ) {

  type_name = "PlaneSensor";
  database.initFields( this );

  maxPosition->setValue( Vec2f( -1, -1 ) );
  minPosition->setValue( Vec2f( 0, 0 ) );
  offset->setValue( Vec3f( 0, 0, 0 ) );

  set_PlaneEvents->setValue( true );
  set_PlaneEvents->setOwner( this );
  mouse_sensor->position->routeNoEvent( set_PlaneEvents );
  isActive->routeNoEvent( set_PlaneEvents );
}

/// Destructor. 
PlaneSensor::~PlaneSensor() {
}

void PlaneSensor::onIsOver( bool newValue,
                            IntersectionInfo &result,
                            int pt_id ) {
  if( is_enabled && ( isActive->getValue() || number_of_active == 0 ) ) {
    X3DPointingDeviceSensorNode::onIsOver( newValue,
                                           result,
                                           pt_id );
    if( newValue ) {
      intersection_matrix = pt_matrices[pt_id];
      intersection_point = intersection_matrix * Vec3f( result.point );
    }
  }
}

int PlaneSensor::Set_PlaneEvents::intersectLinePlane(
  Vec3f a, Vec3f b, float &t, Vec3f &q ) {
  // Compute the t value for the directed line ab intersecting the plane
  Vec3f ab = b - a;
  t = (planeD - planeNormal * a ) / ( planeNormal * ab );

  q = a + t * ab;
  return 1;
}

void PlaneSensor::Set_PlaneEvents::update() {
  SFBool::update();
  PlaneSensor *ps = 
    static_cast< PlaneSensor * >( getOwner() );

  if( ps->enabled->getValue() ) {
    bool isActive = static_cast< SFBool * >(routes_in[1])->getValue();
    if( isActive ) {
      if( newPlane ) {
        originalIntersection = ps->intersection_point;
        active_matrix = ps->intersection_matrix;
        newPlane = false;
        planeD = planeNormal * originalIntersection;
        ps->trackPoint_changed->setValue( originalIntersection, ps->id );
        ps->translation_changed->setValue( Vec3f( 0, 0, 0 ) + 
                                           ps->offset->getValue(), ps->id );
      }
      else {
        H3DFloat t;
        Vec3f intersectionPoint;
        if( intersectLinePlane( active_matrix * near_plane_pos,
                                   active_matrix * far_plane_pos, t,
                                   intersectionPoint ) ) {
          Vec3f translation_changed = intersectionPoint - originalIntersection
                                      + ps->offset->getValue();
          Vec2f minPosition = ps->minPosition->getValue();
          Vec2f maxPosition = ps->maxPosition->getValue();
          if( minPosition.x <= maxPosition.x ) {
            translation_changed.x = ps->Clamp( translation_changed.x,
                                           minPosition.x,
                                           maxPosition.x );
          }
          if( minPosition.y <= maxPosition.y ) {
            translation_changed.y = ps->Clamp( translation_changed.y,
                                           minPosition.y,
                                           maxPosition.y );
          }
          ps->trackPoint_changed->setValue( intersectionPoint, ps->id );
          ps->translation_changed->setValue( translation_changed , ps->id );
        }
        else {
          // X3D specification states that in the case of no plane intersection
          // "browsers may interpret this in a variety of ways"
          // which means doing whatever feels natural.
          // H3DAPI resends last event.
          cerr << "Outside the plane due to near- and farplane" <<
                  " clipping or other reason, last event resent." << endl;
          ps->trackPoint_changed->touch();
          ps->translation_changed->touch();
        }
      }
    }
    else {
      if( !newPlane ) {
        newPlane = true;
        if( ps->autoOffset->getValue() )
          ps->offset->setValue( ps->translation_changed->getValue(), ps->id );
      }
    }
  }
}
