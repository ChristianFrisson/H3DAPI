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
/// \file MouseSensor.cpp
/// \brief CPP file for MouseSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MouseSensor.h>

using namespace H3D;

list< MouseSensor * > MouseSensor::instances = 
list< MouseSensor * >();
bool MouseSensor::invalid_instance_ptr = false;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MouseSensor::database( 
                                      "MouseSensor", 
                                      &(newInstance<MouseSensor>), 
                                      typeid( MouseSensor ),
                                      &X3DSensorNode::database );

namespace MouseSensorInternals {
  FIELDDB_ELEMENT( MouseSensor, metadata, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MouseSensor, position, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MouseSensor, leftButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MouseSensor, middleButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MouseSensor, rightButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MouseSensor, motion, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MouseSensor, scrollUp, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MouseSensor, scrollDown, OUTPUT_ONLY );
}


MouseSensor::MouseSensor( Inst< SFBool>  _enabled,
                          Inst< SFNode>  _metadata,
                          Inst<  SFBool>  _isActive,
                          Inst< SFVec2f>  _position,
                          Inst<  SFBool>  _leftButton,
                          Inst<  SFBool>  _middleButton,
                          Inst<  SFBool>  _rightButton,
                          Inst<  SFVec2f>  _motion,
                          Inst< SFBool > _scrollUp,
                          Inst< SFBool > _scrollDown ):
  X3DSensorNode( _enabled, _metadata, _isActive ),
  position( _position ),
  leftButton( _leftButton ),
  middleButton( _middleButton ),
  rightButton( _rightButton ),
  motion( _motion ),
  scrollUp( _scrollUp ),
  scrollDown( _scrollDown ) {
  
  type_name = "MouseSensor";
  database.initFields( this );
  
  position->setValue( Vec2f( 0, 0 ), id );
  leftButton->setValue( false, id );
  middleButton->setValue( false, id );
  rightButton->setValue( false, id );

  scrollUp->setValue( true, id );
  scrollDown->setValue( true, id );

  instances.push_back( this );
}

MouseSensor::~MouseSensor() {
  instances.remove( this );
  invalid_instance_ptr = true;
}

void MouseSensor::mouseMotionAction( int x, int y ) {
  if( enabled->getValue() ) {
    const Vec2f &last_pos = position->getValue();
    Vec2f new_pos = Vec2f( (H3DFloat) x, (H3DFloat) y );
    Vec2f diff = new_pos - last_pos;
    if( diff * diff > Constants::f_epsilon ) {
      position->setValue( new_pos, id );
      motion->setValue( diff, id );
    }
  }
}

void MouseSensor::mouseButtonAction( int button, int state ) {
  if( enabled->getValue() ) {
    switch( button ) {
    case LEFT_BUTTON:
      leftButton->setValue( state == DOWN, id );
      break;
    case MIDDLE_BUTTON:
      middleButton->setValue( state == DOWN, id );
      break;
    case RIGHT_BUTTON:
      rightButton->setValue( state == DOWN, id );
      break;
    }
  }
}

void MouseSensor::mouseWheelAction( int direction ) {
  if( enabled->getValue() ) {
    if( direction == FROM ) {
      scrollUp->setValue( true, id );
    } else if( direction == TOWARDS ) {
      scrollDown->setValue( true, id );
    }
  }
}

void MouseSensor::buttonCallback( int button, int state ) {
  invalid_instance_ptr = false;
  list< MouseSensor * >::iterator end_iterator = instances.end();
  for( list< MouseSensor * >::iterator i = instances.begin();
       i != end_iterator;
       ++i ) {
    (*i)->mouseButtonAction( button, state );
    if( invalid_instance_ptr ) {
      if( instances.empty() ) {
        break;
      }
      i = instances.begin();
      end_iterator = instances.end();
      invalid_instance_ptr = false;
    }
  }
}
    
void MouseSensor::motionCallback( int x, int y ) {
  for( list< MouseSensor * >::iterator i = instances.begin();
       i != instances.end();
       ++i ) {
    (*i)->mouseMotionAction( x, y );
  }
}

void MouseSensor::wheelCallback( int direction ) {
  for( list< MouseSensor * >::iterator i = instances.begin();
       i != instances.end();
       ++i ) {
    (*i)->mouseWheelAction( direction );
  }
}
