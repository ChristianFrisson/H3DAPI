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
/// \file X3DEnvironmentalSensorNode.cpp
/// \brief CPP file for X3DEnvironmentalSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DEnvironmentalSensorNode.h>

using namespace H3D;

H3DNodeDatabase X3DEnvironmentalSensorNode::database( 
        "X3DEnvironmentalSensorNode", 
        NULL,
        typeid( X3DEnvironmentalSensorNode ),
        &X3DSensorNode::database 
        );

namespace X3DSensorEnvironmentalNodeInternals {
  FIELDDB_ELEMENT( X3DEnvironmentalSensorNode, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DEnvironmentalSensorNode, size, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DEnvironmentalSensorNode, enterTime, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DEnvironmentalSensorNode, exitTime, OUTPUT_ONLY );

}

X3DEnvironmentalSensorNode::X3DEnvironmentalSensorNode(  
                Inst< SFNode > _metadata,
                Inst< SFVec3f > _center ,
                Inst< SFBool > _enabled ,
                Inst< SFVec3f > _size ,
                Inst< SFTime > _enterTime ,
                Inst< SFTime > _exitTime ,
                Inst< SFBool > _isActive ) :          

                X3DSensorNode( _enabled, _metadata, _isActive),
                center( _center),
                size( _size ),
                enterTime( _enterTime ),
                exitTime( _exitTime ){

  type_name = "X3DEnvironmentalSensorNode";
  database.initFields( this );

  center->setValue( Vec3f( 0, 0, 0) );
  size->setValue( Vec3f( 0, 0, 0) );

  enterTime->setValue( 0.0, id );
  exitTime->setValue( 0.0, id );
  
}


