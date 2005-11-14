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
/// \file X3DKeyDeviceSensorNode.h
/// \brief Header file for X3DKeyDeviceSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DKEYDEVICESENSORNODE_H__
#define __X3DKEYDEVICESENSORNODE_H__

#include "X3DSensorNode.h"
#include <list>
#include "GL/glew.h"
#include "GL/glut.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DKeyDeviceSensorNode
  /// This abstract node type is the base type for all sensor node types
  /// which operate using key devices.
  /// 
  class H3DAPI_API X3DKeyDeviceSensorNode : public X3DSensorNode {
  public:
    
    /// Constructor. 
    X3DKeyDeviceSensorNode( Inst< SFBool > _enabled  = 0,
                            Inst< SFNode > _metadata = 0,
                            Inst< SFBool > _isActive = 0 );
    /// Destructor. 
    ~X3DKeyDeviceSensorNode();

    /// Virtual function called when a key is pressed.
    /// \param key The value of the key pressed, either ascii character
    /// or function key value depending on special_key argument. 
    /// See KeyDevice.cpp for example usage.
    /// \param modifiers State of modifier keys (alt, shift, ctrl)
    /// \param special_key true if function key is pressed, false otherwise
    virtual void glutKeyboardUp( int key, 
                                 int modifiers, 
                                 bool special_key ) = 0;

    /// Virtual function called when a key is released.
    /// \param key he value of the key pressed, either ascii character
    /// or function key value depending on special_key argument. 
    /// See KeyDevice.cpp for example usage.
    /// \param modifiers State of modifier keys (alt, shift, ctrl)
    /// \param special_key true if function key is pressed, false otherwise
    virtual void glutKeyboardDown( int key, 
                                   int modifiers, 
                                   bool special_key ) = 0;

    /// GLUT callback function for glutKeyboardFunc.
    static void glutKeyboardDownCallback( unsigned char key, 
                                          int x, int y );
    
    /// GLUT callback function for glutSpecialFunc.
    static void glutSpecialDownCallback( int key, 
                                         int x, int y );

    /// GLUT callback function for glutKeyboardUpFunc.
    static void glutKeyboardUpCallback( unsigned char key, 
                                          int x, int y );
    
    /// GLUT callback function for glutSpecialUpFunc.
    static void glutSpecialUpCallback( int key, 
                                       int x, int y );

  private:
    /// The instances of X3DKeyDeviceSensorNode that has been created.
    static list< X3DKeyDeviceSensorNode * > instances;  
  };
}

#endif
