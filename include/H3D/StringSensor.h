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
/// \file StringSensor.h
/// \brief Header file for StringSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSTRINGSENSOR_H__
#define __X3DSTRINGSENSOR_H__


#include <H3D/X3DKeyDeviceSensorNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFString.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class StringSensor
  /// \brief A StringSensor node generates events as the user presses
  ///  keys on the keyboard. A StringSensor node can be enabled or 
  ///  disabled by sending it an enabled event with a value of TRUE 
  ///  or FALSE. If the StringSensor node is disabled, it does not 
  ///  track keyboard input or send events.
  ///
  ///  enteredText events are generated as keys which produce characters
  ///  are pressed on the keyboard. The value of this event is the UTF-8
  ///  string entered including the latest character struck. The set of 
  ///  UTF-8 characters that can be generated will vary between different
  ///  keyboards and different implementations. 
  /// 
  ///  If a deletionAllowed has value TRUE, the previously entered character
  ///  in the enteredText is removed when the backspace key is entered.
  ///  If deletionAllowed has value FALSE, characters 
  ///  may only be added to the string; deletion of characters shall not 
  ///  be allowed. Should the browser-recognized value for deleting the 
  ///  preceding character is entered, it shall be ignored.
  ///
  ///  The finalText event is generated whenever the browser-recognized
  ///  value for terminating a string is entered; in H3D this is the enter key.
  ///  When this recognition occurs, the finalText field generates an event 
  ///  with value equal to that of enteredText. After the finalText field 
  ///  event has been generated, the enteredText field is set to the empty
  ///  string but no event is generated.
  ///
  ///  When the user begins typing, the StringSensor sends an isActive
  ///  event with value TRUE. When the string is terminated, the StringSensor
  ///  sends an isActive event with value FALSE.
  ///
  ///  The StringSensor is not affected by its position in the
  ///  transformation hierarchy.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/StringSensor.x3d">StringSensor.x3d</a>
  ///     ( <a href="examples/StringSensor.x3d.html">Source</a> )
  class H3DAPI_API StringSensor : public X3DKeyDeviceSensorNode {
  public:

    /// Constructor.
    StringSensor( Inst< SFBool  >  _deletionAllowed   = 0,
                  Inst< SFBool  >  _enabled  = 0,
                  Inst< SFNode  >  _metadata = 0,
                  Inst< SFString>  _enteredText =0,
                  Inst< SFString>  _finalText   =0, 
                  Inst< SFBool  >  _isActive = 0
                  );

    /// Function called when key is pressed. 
    virtual void keyboardDown( int key, bool special_key);
    
    /// Function called when key is released. 
    virtual void keyboardUp( int key, bool special_key ){
    }

    ///  If a deletionAllowed has value TRUE, the previously entered character
    ///  in the enteredText is removed when the backspace key is entered.
    ///  
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> SFBool = TRUE \n
    /// \dotfile StringSensor_deletionAllowed.dot
    auto_ptr< SFBool>  deletionAllowed;

    /// enteredText events are generated as keys which produce characters
    /// are pressed on the keyboard. 
    ///
    /// <b>Access type:</b> outputOnly
    /// \dotfile StringSensor_enteredText.dot
    auto_ptr< SFString>  enteredText;
 
    /// The finalText event is generated whenever the browser-recognized
    /// value for terminating a string is entered. 
    ///
    /// <b>Access type:</b> outputOnly
    /// \dotfile StringSensor_finalText.dot
    auto_ptr< SFString>  finalText; 

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
