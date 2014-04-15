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
/// \file X3DTouchSensorNode.cpp
/// \brief CPP file for X3DTouchSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTouchSensorNode.h>

using namespace H3D;

H3DNodeDatabase X3DTouchSensorNode::database( 
        "X3DTouchSensorNode", 
        NULL,
        typeid( X3DTouchSensorNode ),
        &X3DPointingDeviceSensorNode::database 
        );

namespace X3DTouchSensorNodeInternals {
  FIELDDB_ELEMENT( X3DTouchSensorNode, touchTime, OUTPUT_ONLY );
}

X3DTouchSensorNode::X3DTouchSensorNode( 
                              Inst< SFString > _description,
                              Inst< SFBool >  _enabled,
                              Inst< SFNode >  _metadata,
                              Inst< SFBool >  _isActive,
                              Inst< SFBool >  _isOver,
                              Inst< SFTime > _touchTime ) :
  X3DPointingDeviceSensorNode( _description, _enabled, 
                               _metadata, _isActive, _isOver ),
  touchTime ( _touchTime  ),
  setTouchTime( new SetTouchTime ) {

  type_name = "X3DTouchSensorNode";
  database.initFields( this );
  
  setTouchTime->setValue( true );
  setTouchTime->setOwner( this );
  setTouchTime->setName( "setTouchTime" );
  isActive->route( setTouchTime );
}
