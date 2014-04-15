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
/// \file X3DDragSensorNode.cpp
/// \brief CPP file for X3DDragSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DDragSensorNode.h>

using namespace H3D;

H3DNodeDatabase X3DDragSensorNode::database( 
        "X3DDragSensorNode", 
        NULL,
        typeid( X3DDragSensorNode ),
        &X3DPointingDeviceSensorNode::database 
        );

namespace X3DDragSensorNodeInternals {
  FIELDDB_ELEMENT( X3DDragSensorNode, autoOffset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DDragSensorNode, trackPoint_changed, OUTPUT_ONLY );
}

X3DDragSensorNode::X3DDragSensorNode(
                              Inst< SFBool >  _autoOffset,
                              Inst< SFString > _description,
                              Inst< SFBool >  _enabled,
                              Inst< SFNode >  _metadata,
                              Inst< SFBool >  _isActive,
                              Inst< SFBool >  _isOver,
                              Inst< SFVec3f >  _trackPoint_changed ) :
  X3DPointingDeviceSensorNode( _description, _enabled,
                               _metadata, _isActive, _isOver ),
  autoOffset ( _autoOffset  ),
  trackPoint_changed( _trackPoint_changed ),
  send_warning_message( true ) {

  type_name = "X3DDragSensorNode";
  database.initFields( this );

  autoOffset->setValue( true );
}

H3DFloat X3DDragSensorNode::Clamp(
  H3DFloat n, H3DFloat min, H3DFloat max ) {
  if( n < min ) return min;
  if( n > max ) return max;
  return n;
}
