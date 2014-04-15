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
/// \file SpaceballSensor.cpp
/// \brief CPP file for SpaceballSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SpaceballSensor.h>

using namespace H3D;

list< SpaceballSensor * > SpaceballSensor::instances = 
list< SpaceballSensor * >();

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SpaceballSensor::database( 
                                      "SpaceballSensor", 
                                      &(newInstance<SpaceballSensor>), 
                                      typeid( SpaceballSensor ),
                                      &X3DSensorNode::database );

namespace SpaceballSensorInternals {
  FIELDDB_ELEMENT( SpaceballSensor, metadata, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpaceballSensor, motion, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SpaceballSensor, rotation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SpaceballSensor, buttons, OUTPUT_ONLY );
}


SpaceballSensor::SpaceballSensor( Inst< SFBool  > _enabled,
                                  Inst< SFNode  > _metadata,
                                  Inst< SFBool  > _isActive,
                                  Inst< SFVec3f > _motion,
                                  Inst< SFVec3f > _rotation,
                                  Inst< MFBool  > _buttons ):
  X3DSensorNode( _enabled, _metadata, _isActive ),
  motion( _motion ),
  rotation( _rotation ),
  buttons( _buttons ) {
  
  type_name = "SpaceballSensor";
  database.initFields( this );
  
  motion->setValue( Vec3f( 0, 0, 0 ), id );
  rotation->setValue( Vec3f( 0, 0, 0 ), id );
  buttons->resize( 30, 0, id );
  instances.push_back( this );
}

SpaceballSensor::~SpaceballSensor() {
  instances.remove( this );
}


void SpaceballSensor::addGLUTMotionAction( int x, int y, int z ) { 
  
  motion->setValue( Vec3f( (H3DFloat)x, (H3DFloat)y, (H3DFloat)z ), id );
  
}

void SpaceballSensor::addGLUTRotateAction( int x, int y, int z ) { 
  Vec3f new_rot = Vec3f( (H3DFloat) x, (H3DFloat) y, (H3DFloat) z );
  rotation->setValue( new_rot, id );
}

void SpaceballSensor::addGLUTButtonAction( int button, int state ) { 
  if ( button > (int)buttons->size() ) {
    buttons->resize( button, false, id );
  }
  buttons->setValue( button-1, state != 0, id );
}


void SpaceballSensor::glutMotionCallback( int x, int y, int z ) {
  for( list< SpaceballSensor * >::iterator i = instances.begin();
       i != instances.end();
       ++i ) {
    (*i)->addGLUTMotionAction( x, y, z );
  }
}

void SpaceballSensor::glutRotateCallback( int x, int y, int z ) {
  for( list< SpaceballSensor * >::iterator i = instances.begin();
       i != instances.end();
       ++i ) {
    (*i)->addGLUTRotateAction( x, y, z );
  }
}

void SpaceballSensor::glutButtonCallback( int button, int state ) {
  for( list< SpaceballSensor * >::iterator i = instances.begin();
       i != instances.end();
       ++i ) {
    (*i)->addGLUTButtonAction( button, state );
  }
}
