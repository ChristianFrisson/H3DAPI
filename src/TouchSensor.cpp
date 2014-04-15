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
/// \file TouchSensor.cpp
/// \brief CPP file for TouchSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TouchSensor.h>

using namespace H3D;


H3DNodeDatabase TouchSensor::database( 
        "TouchSensor", 
        &(newInstance< TouchSensor > ),
        typeid( TouchSensor ),
        &X3DTouchSensorNode::database 
        );

namespace TouchSensorInternals {
  FIELDDB_ELEMENT( TouchSensor, hitNormal_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TouchSensor, hitPoint_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TouchSensor, hitTexCoord_changed, OUTPUT_ONLY );
}

TouchSensor::TouchSensor( 
                         Inst< SFString > _description,
                         Inst< SFBool >  _enabled,
                         Inst< SFNode >  _metadata,
                         Inst< SFVec3f > _hitNormal_changed,
                         Inst< SFVec3f > _hitPoint_changed,
                         Inst< SFVec2f > _hitTexCoord_changed,
                         Inst< SFBool >  _isActive,
                         Inst< SFBool >  _isOver,
                         Inst< SFTime > _touchTime ) :
  X3DTouchSensorNode( _description, _enabled,
                      _metadata, _isActive, _isOver, _touchTime ),
  hitNormal_changed( _hitNormal_changed ),
  hitPoint_changed( _hitPoint_changed ),
  hitTexCoord_changed( _hitTexCoord_changed ) {

  type_name = "TouchSensor";
  database.initFields( this );
}

void TouchSensor::onIsOver( IntersectionInfo *result,
                            Matrix4f *global_to_local ) {
  if( is_enabled && ( isActive->getValue() || number_of_active == 0 ) ) {
    X3DPointingDeviceSensorNode::onIsOver( result,
                                           global_to_local );
    if( new_value ) {
      Vec3f newNormalPoint = *global_to_local
      * Vec3f( result->point + result->normal );
      Vec3f newPoint = *global_to_local * Vec3f( result->point );
      newNormalPoint = newNormalPoint - newPoint;
      newNormalPoint.normalize();

      hitPoint_changed->setValue( Vec3f( newPoint ), id );
      hitNormal_changed->setValue( Vec3f( newNormalPoint ), id );
      hitTexCoord_changed->setValue( Vec2f( (H3DFloat)result->tex_coord.x,
        (H3DFloat)result->tex_coord.y ),
        id );
    }
  }
}
