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
/// \file X3DKeyDeviceSensorNode.cpp
/// \brief CPP file for X3DKeyDeviceSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DKeyDeviceSensorNode.h"

using namespace H3D;

list< X3DKeyDeviceSensorNode * > X3DKeyDeviceSensorNode::instances = 
list< X3DKeyDeviceSensorNode * >();

/// Constructor. 
X3DKeyDeviceSensorNode::X3DKeyDeviceSensorNode( Inst< SFBool>  _enabled,
                                                Inst< SFNode>  _metadata,
                                                Inst<  SFBool>  _isActive ):
  X3DSensorNode( _enabled, _metadata, _isActive ) {
  instances.push_back( this );
}

/// Destructor. 
X3DKeyDeviceSensorNode::~X3DKeyDeviceSensorNode() {
  instances.remove( this );
}


void X3DKeyDeviceSensorNode::glutKeyboardDownCallback( unsigned char key, 
                                                   int x, int y ) {
  int mods = glutGetModifiers();
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->glutKeyboardDown( key, mods, false );
  }
}

void X3DKeyDeviceSensorNode::glutSpecialDownCallback( int key, 
                                                      int x, int y ) {
  int mods = glutGetModifiers();
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->glutKeyboardDown( key, mods, true );
  }
}

void X3DKeyDeviceSensorNode::glutKeyboardUpCallback( unsigned char key, 
                                                     int x, int y ) {
  int mods = glutGetModifiers();
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->glutKeyboardUp( key, mods, false );
  }
}

void X3DKeyDeviceSensorNode::glutSpecialUpCallback( int key, 
                                                    int x, int y ) {
  int mods = glutGetModifiers();
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->glutKeyboardUp( key, mods, true );
  }
}

void  X3DKeyDeviceSensorNode::keyboardDownCallback( unsigned char key ) {
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->keyboardDown( key, false );
  }
}

void  X3DKeyDeviceSensorNode::keyboardSpecialDownCallback( int key ) {
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->keyboardDown( key, true );
  }
}

void X3DKeyDeviceSensorNode::keyboardUpCallback( unsigned char key ) {
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->keyboardUp( key, false );
  }
}

void X3DKeyDeviceSensorNode::keyboardSpecialUpCallback( int key ) {
  for( list< X3DKeyDeviceSensorNode * >::iterator i = instances.begin();
       i != instances.end();
       i++ ) {
    (*i)->keyboardUp( key, true );
  }
}

#ifdef WIN32
void X3DKeyDeviceSensorNode::keyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // Evaluate Window Message
	switch (uMsg)
	{
    case WM_KEYDOWN:  
    case WM_SYSKEYDOWN:
      keyboardSpecialDownCallback( wParam );
    break;
    
    case WM_KEYUP:
    case WM_SYSKEYUP:
      keyboardSpecialUpCallback( wParam );
    break;

    // When using ToAscii in the function called by
    // keyboardSpecialUpCallback the dead char gets
    // translated into something a bit strange. This
    // causes a deadchar message to look different
    // when the buttons is pushed than when it is
    // released. If the application want to take care
    // of deadchars then it should only look on the
    // keyPress event.
    case WM_SYSDEADCHAR:
    case WM_DEADCHAR:
    case WM_SYSCHAR:
    case WM_CHAR:
      keyboardDownCallback( wParam );
    break;

    case WM_MENUCHAR:
      if( GetKeyState( VK_MENU ) >= 0 )
        keyboardDownCallback( wParam );
    break;

  }
}
#endif
