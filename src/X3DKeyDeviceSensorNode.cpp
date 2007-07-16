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
/// \file X3DKeyDeviceSensorNode.cpp
/// \brief CPP file for X3DKeyDeviceSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <X3DKeyDeviceSensorNode.h>

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
