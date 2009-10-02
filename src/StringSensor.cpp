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
/// \file StringSensor.cpp
/// \brief CPP file for StringSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/StringSensor.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase StringSensor::database( 
                                       "StringSensor", 
                                       &(newInstance<StringSensor>), 
                                       typeid( StringSensor ),
                                       &X3DKeyDeviceSensorNode::database );

namespace StringSensorInternals {
  FIELDDB_ELEMENT( StringSensor, deletionAllowed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( StringSensor, enteredText, OUTPUT_ONLY );
  FIELDDB_ELEMENT( StringSensor, finalText, OUTPUT_ONLY );

}

StringSensor::StringSensor( Inst< SFBool  >  _deletionAllowed,
                           Inst< SFBool  >  _enabled,
                           Inst< SFNode  >  _metadata,
                           Inst< SFString>  _enteredText,
                           Inst< SFString>  _finalText, 
                           Inst< SFBool  >  _isActive):

X3DKeyDeviceSensorNode( _enabled, _metadata, _isActive ),
deletionAllowed ( _deletionAllowed),
enteredText (_enteredText),
finalText (_finalText){
  type_name = "StringSensor";
  database.initFields( this );
  deletionAllowed->setValue( true );
}

void StringSensor::keyboardDown( int key, bool special_key) {
  if( enabled->getValue() ) {
    if( !isActive->getValue() ) isActive->setValue( true, id );
    string enteredKey;
    if(key == 8 && deletionAllowed->getValue()){/* backspace for Windows & Unix */
      string enteredString = enteredText->getValue();
      int strSize = (int)enteredString.size();
      if(strSize>0){
        enteredString.resize(strSize-1);
        enteredText->setValue(enteredString, id);
      }
    }
    else if(key == 13){ /* enter key for windows & Unix */
      finalText->setValue(enteredText->getValue(), id);
      enteredText->setValue("", id);
      isActive->setValue(false, id);
    }

    else if( !special_key){ 
      enteredKey = string(1,key );
      enteredText->setValue((enteredText->getValue() + enteredKey), id);
    }
  } else {
    // isactive false??
  }
}


