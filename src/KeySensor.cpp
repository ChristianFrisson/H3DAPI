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
/// \file KeySensor.cpp
/// \brief CPP file for KeySensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/KeySensor.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase KeySensor::database( 
                                    "KeySensor", 
                                    &(newInstance<KeySensor>), 
                                    typeid( KeySensor ),
                                    &X3DKeyDeviceSensorNode::database );

namespace KeySensorInternals {
  FIELDDB_ELEMENT( KeySensor, actionKeyPress, OUTPUT_ONLY );
  FIELDDB_ELEMENT( KeySensor, actionKeyRelease, OUTPUT_ONLY );
  FIELDDB_ELEMENT( KeySensor, altKey, OUTPUT_ONLY );
  FIELDDB_ELEMENT( KeySensor, controlKey, OUTPUT_ONLY );
  FIELDDB_ELEMENT( KeySensor, keyPress, OUTPUT_ONLY );
  FIELDDB_ELEMENT( KeySensor, keyRelease, OUTPUT_ONLY );
  FIELDDB_ELEMENT( KeySensor, shiftKey, OUTPUT_ONLY );
}

KeySensor::KeySensor( Inst< SFBool   > _enabled,
                      Inst< SFNode   > _metadata,
                      Inst< SFBool   > _isActive,
                      Inst< SFInt32  > _actionKeyPress,
                      Inst< SFInt32  > _actionKeyRelease,
                      Inst< SFBool   > _altKey,
                      Inst< SFBool   > _controlKey,
                      Inst< SFBool   > _shiftKey,
                      Inst< SFString > _keyPress,
                      Inst< SFString > _keyRelease ):

  X3DKeyDeviceSensorNode( _enabled, _metadata, _isActive ),
  actionKeyPress( _actionKeyPress ),
  actionKeyRelease( _actionKeyRelease ),
  altKey( _altKey ),
  controlKey( _controlKey ),
  shiftKey( _shiftKey ),
  keyPress( _keyPress ),
  keyRelease( _keyRelease ) {

  type_name = "KeySensor";
  database.initFields( this );
}

void KeySensor::keyboardDown( int key, bool special_key ) {
  if( enabled->getValue() ) {
    isActive->setValue( true, id );
    if( special_key ) {
      switch( key ) {
        case ALT: altKey->setValue( true, id ); break;
        case CONTROL: controlKey->setValue( true, id ); break;
        case SHIFT: shiftKey->setValue( true, id ); break;
        default: {
          actionKeyPress->setValue( key, id );
        }
      }
    } else {
      keyPress->setValue( string( 1, key ), id );
    }
  }
}

void KeySensor::keyboardUp( int key, bool special_key ) {
  if( enabled->getValue() ) {
    isActive->setValue( false, id );
    if( special_key ) {
      switch( key ) {
        case ALT: altKey->setValue( false, id ); break;
        case CONTROL: controlKey->setValue( false, id ); break;
        case SHIFT: shiftKey->setValue( false, id ); break;
        default: {
          actionKeyRelease->setValue( key, id );
        }
      }
    } else {
      keyRelease->setValue( string( 1, key ), id  );
    }
  }
}
