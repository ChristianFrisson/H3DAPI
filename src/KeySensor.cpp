//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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

#include "KeySensor.h"

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

void KeySensor::glutKeyboardDown( int stroke, 
                                  int modifiers, 
                                  bool special_key ) {
  if( enabled->getValue() ) {
    isActive->setValue( true, id );
    if( special_key ) {
      switch( stroke ) {
      case GLUT_KEY_F1: actionKeyPress->setValue( F1, id ); break;
      case GLUT_KEY_F2: actionKeyPress->setValue( F2, id  ); break;
      case GLUT_KEY_F3: actionKeyPress->setValue( F3, id  ); break;
      case GLUT_KEY_F4: actionKeyPress->setValue( F4, id  ); break;
      case GLUT_KEY_F5: actionKeyPress->setValue( F5, id  ); break;
      case GLUT_KEY_F6: actionKeyPress->setValue( F6, id  ); break;
      case GLUT_KEY_F7: actionKeyPress->setValue( F7, id  ); break;
      case GLUT_KEY_F8: actionKeyPress->setValue( F8, id  ); break;
      case GLUT_KEY_F9: actionKeyPress->setValue( F9, id  ); break;
      case GLUT_KEY_F10: actionKeyPress->setValue( F10, id  ); break;
      case GLUT_KEY_F11: actionKeyPress->setValue( F11, id  ); break;
      case GLUT_KEY_F12: actionKeyPress->setValue( F12, id  ); break;
      case GLUT_KEY_PAGE_UP: actionKeyPress->setValue( PGUP, id  ); break;
      case GLUT_KEY_PAGE_DOWN: actionKeyPress->setValue( PGDN, id  ); break;
      case GLUT_KEY_UP: actionKeyPress->setValue( UP, id  ); break;
      case GLUT_KEY_DOWN: actionKeyPress->setValue( DOWN, id  ); break;
      case GLUT_KEY_LEFT: actionKeyPress->setValue( LEFT, id  ); break;
      case GLUT_KEY_RIGHT: actionKeyPress->setValue( RIGHT, id  ); break;
        
      default: {}  
      }
    } else {
      keyPress->setValue( string( 1, stroke ), id  );
    }
  }
}

void KeySensor::glutKeyboardUp( int stroke, 
                                int modifiers, 
                                bool special_key ) {
  if( enabled->getValue() ) {
    isActive->setValue( false, id );
    if( special_key ) {
      switch( stroke ) {
      case GLUT_KEY_F1: actionKeyRelease->setValue( F1, id ); break;
      case GLUT_KEY_F2: actionKeyRelease->setValue( F2, id  ); break;
      case GLUT_KEY_F3: actionKeyRelease->setValue( F3, id  ); break;
      case GLUT_KEY_F4: actionKeyRelease->setValue( F4, id  ); break;
      case GLUT_KEY_F5: actionKeyRelease->setValue( F5, id  ); break;
      case GLUT_KEY_F6: actionKeyRelease->setValue( F6, id  ); break;
      case GLUT_KEY_F7: actionKeyRelease->setValue( F7, id  ); break;
      case GLUT_KEY_F8: actionKeyRelease->setValue( F8, id  ); break;
      case GLUT_KEY_F9: actionKeyRelease->setValue( F9, id  ); break;
      case GLUT_KEY_F10: actionKeyRelease->setValue( F10, id  ); break;
      case GLUT_KEY_F11: actionKeyRelease->setValue( F11, id  ); break;
      case GLUT_KEY_F12: actionKeyRelease->setValue( F12, id  ); break;
      case GLUT_KEY_PAGE_UP: actionKeyRelease->setValue( PGUP, id  ); break;
      case GLUT_KEY_PAGE_DOWN: actionKeyRelease->setValue( PGDN, id  ); break;
      case GLUT_KEY_UP: actionKeyRelease->setValue( UP, id  ); break;
      case GLUT_KEY_DOWN: actionKeyRelease->setValue( DOWN, id  ); break;
      case GLUT_KEY_LEFT: actionKeyRelease->setValue( LEFT, id  ); break;
      case GLUT_KEY_RIGHT: actionKeyRelease->setValue( RIGHT, id  ); break;
        
      default: {}  
      }
    } else {
      keyRelease->setValue( string( 1, stroke ), id  );
    }
  }
}

void KeySensor::keyboardDown( int key, bool special_key ) {
  if( enabled->getValue() ) {
    isActive->setValue( true, id );
#ifdef WIN32
    if( special_key ) {
      switch( key ) {
        case VK_F1: actionKeyPress->setValue( F1, id ); break;
        case VK_F2: actionKeyPress->setValue( F2, id ); break;
        case VK_F3: actionKeyPress->setValue( F3, id ); break;
        case VK_F4: actionKeyPress->setValue( F4, id ); break;
        case VK_F5: actionKeyPress->setValue( F5, id ); break;
        case VK_F6: actionKeyPress->setValue( F6, id ); break;
        case VK_F7: actionKeyPress->setValue( F7, id ); break;
        case VK_F8: actionKeyPress->setValue( F8, id ); break;
        case VK_F9: actionKeyPress->setValue( F9, id ); break;
        case VK_F10: actionKeyPress->setValue( F10, id ); break;
        case VK_F11: actionKeyPress->setValue( F11, id ); break;
        case VK_F12: actionKeyPress->setValue( F12, id ); break;
        case VK_HOME: actionKeyPress->setValue( HOME, id ); break;
        case VK_END: actionKeyPress->setValue( END, id ); break;
        case VK_PRIOR: actionKeyPress->setValue( PGUP, id ); break;
        case VK_NEXT: actionKeyPress->setValue( PGDN, id ); break;
        case VK_UP: actionKeyPress->setValue( UP, id ); break;
        case VK_DOWN: actionKeyPress->setValue( DOWN, id ); break;
        case VK_LEFT: actionKeyPress->setValue( LEFT, id ); break;
        case VK_RIGHT: actionKeyPress->setValue( RIGHT, id ); break;
        case VK_SHIFT: shiftKey->setValue( true, id ); break;
        case VK_MENU: altKey->setValue( true, id ); break;
        case VK_CONTROL: controlKey->setValue( true, id ); break;
        case VK_DELETE: keyPress->setValue( string( 1, key ), id ); break;
        default: {}
      }
    } else {
      keyPress->setValue( string( 1, key ), id );
    }
#endif
  }
}

void KeySensor::keyboardUp( int key, bool special_key ) {
  if( enabled->getValue() ) {
    isActive->setValue( false, id );
#ifdef WIN32
    if( special_key ) {
      switch( key ) {
        case VK_F1: actionKeyRelease->setValue( F1, id ); break;
        case VK_F2: actionKeyRelease->setValue( F2, id ); break;
        case VK_F3: actionKeyRelease->setValue( F3, id ); break;
        case VK_F4: actionKeyRelease->setValue( F4, id ); break;
        case VK_F5: actionKeyRelease->setValue( F5, id ); break;
        case VK_F6: actionKeyRelease->setValue( F6, id ); break;
        case VK_F7: actionKeyRelease->setValue( F7, id ); break;
        case VK_F8: actionKeyRelease->setValue( F8, id ); break;
        case VK_F9: actionKeyRelease->setValue( F9, id ); break;
        case VK_F10: actionKeyRelease->setValue( F10, id ); break;
        case VK_F11: actionKeyRelease->setValue( F11, id ); break;
        case VK_F12: actionKeyRelease->setValue( F12, id ); break;
        case VK_HOME: actionKeyRelease->setValue( HOME, id ); break;
        case VK_END: actionKeyRelease->setValue( END, id ); break;
        case VK_PRIOR: actionKeyRelease->setValue( PGUP, id ); break;
        case VK_NEXT: actionKeyRelease->setValue( PGDN, id ); break;
        case VK_UP: actionKeyRelease->setValue( UP, id ); break;
        case VK_DOWN: actionKeyRelease->setValue( DOWN, id ); break;
        case VK_LEFT: actionKeyRelease->setValue( LEFT, id ); break;
        case VK_RIGHT: actionKeyRelease->setValue( RIGHT, id ); break;
        case VK_SHIFT: shiftKey->setValue( false, id ); break;
        case VK_MENU: altKey->setValue( false, id ); break;
        case VK_CONTROL: controlKey->setValue( false, id ); break;
        default: {
          BYTE state[ 256 ];
          WORD code[ 2 ];

          GetKeyboardState( state );

          if( ToAscii( key, 0, state, code, 0 ) == 1 )
            key=code[ 0 ];

          keyRelease->setValue( string(1, key ), id );
        }
      }
    } else {
      keyRelease->setValue( string( 1, key ), id );
    }
#endif
  }
}
