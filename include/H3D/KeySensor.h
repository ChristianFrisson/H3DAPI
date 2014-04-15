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
/// \file KeySensor.h
/// \brief Header file for KeySensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DKEYSENSOR_H__
#define __X3DKEYSENSOR_H__


#include <H3D/X3DKeyDeviceSensorNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFString.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class KeySensor
  /// \brief A KeySensor node generates events when the user presses keys
  /// on the keyboard.
  ///
  /// \anchor KeySensor_details
  /// A KeySensor node can be enabled or disabled by sending
  /// it an enabled event with a value of TRUE or FALSE. If the KeySensor
  /// node is disabled, it does not track keyboard input or send events.
  ///
  /// keyPress and keyRelease events are generated as keys which produce
  /// characters are pressed and released on the keyboard. The value of
  /// these events is a string of length 1 containing the single UTF-8
  /// character associated with the key pressed. The set of UTF-8 
  /// characters that can be generated will vary between different keyboards
  /// and different implementations.
  ///
  /// actionKeyPress and actionKeyRelease events are generated as 'action' 
  /// keys are pressed and released on the keyboard. The value of these
  /// events are:
  /// 
  /// <table border = 0 >
  /// <tr><td>F1-F12<td>1-12</tr>
  /// <tr><td>HOME<td>13</tr>
  /// <tr><td>END<td>14</tr>
  /// <tr><td>PGUP<td>15</tr>
  /// <tr><td>PGDN<td>16</tr>
  /// <tr><td>UP<td>17</tr>
  /// <tr><td>DOWN<td>18</tr>
  /// <tr><td>LEFT<td>19</tr>
  /// <tr><td>RIGHT<td>20</tr>
  /// </table>
  ///
  /// The following is unimplemented in H3DAPI for applications using
  /// GLUT with H3DAPI on non Windows systems (for example H3DLoad)
  /// since it is not supported by glut to get an event when a shift,
  /// control or alt key is pressed.
  /// shiftKey, controlKey, and altKey events are generated as the shift,
  /// alt and control keys on the keyboard are pressed and released. 
  /// Their value is TRUE when the key is pressed and FALSE when the key
  /// is released. 
  ///
  /// When a key is pressed, the KeySensor sends an isActive event with 
  /// value TRUE. Once the key is released, the KeySensor sends an isActive
  /// event with value FALSE.
  ///
  /// The KeySensor is not affected by its position in the transformation
  /// hierarchy.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/KeySensor.x3d">KeySensor.x3d</a>
  ///     ( <a href="examples/KeySensor.x3d.html">Source</a> )
  class H3DAPI_API KeySensor : public X3DKeyDeviceSensorNode {
  public:

    /// Constructor.
    KeySensor( Inst< SFBool>  _enabled  = 0,
               Inst< SFNode>  _metadata = 0,
               Inst<  SFBool>  _isActive = 0,
               Inst< SFInt32>  _actionKeyPress = 0,
               Inst< SFInt32>  _actionKeyRelease = 0,
               Inst<  SFBool>  _altKey = 0,
               Inst<  SFBool>  _controlKey = 0,
               Inst<  SFBool>  _shiftKey = 0,
               Inst<  SFString>  _keyPress = 0,
               Inst<  SFString>  _keyRelease = 0
               );

    /// Function called when key is pressed. 
    virtual void keyboardDown( int key, bool special_key );

    /// Function called when key is released. 
    virtual void keyboardUp( int key, bool special_key );

    /// The actionKeyPress fields contains an integer that specifies
    /// which action key was the last one to be pressed. Info on the
    /// values can be found in the 
    /// \ref KeySensor_details "detailed description".
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFInt32>    actionKeyPress;

    /// The actionKeyRelease fields contains an integer that specifies
    /// which action key was the last one to be released. Info on the
    /// values can be found in the 
    /// \ref KeySensor_details "detailed description".
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFInt32>    actionKeyRelease;

    /// Generates events when alt key is used.
    /// True if the alt key is pressed, false when released.
    /// The altKey field is not supported at this time by H3DAPI when
    /// using GLUT on non-Windows systems.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr<  SFBool>    altKey;

    /// Generates events when control key is used. True if the control key
    /// is pressed, false when released.
    /// The controlKey field is not supported at this time by H3DAPI when
    /// using GLUT on non-Windows systems.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr<  SFBool>    controlKey;

    /// Generates events when shift key is used. True if the shift key
    /// is pressed, false when released.
    /// The shiftKey field is not supported at this time by H3DAPI when
    /// using GLUT on non-Windows systems.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr<  SFBool>    shiftKey;

    /// The keyPress field events are generated as keys which produce
    /// characters are pressed. The field contains a string of length 1
    /// with the single UTF-8 character associated with the latest key
    /// that was pressed.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFString>  keyPress;

    /// The keyRelease field events are generated as keys which produce
    /// characters are released. The field contains a string of length 1
    /// with the single UTF-8 character associated with the latest key
    /// that was released.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFString>  keyRelease; 

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
