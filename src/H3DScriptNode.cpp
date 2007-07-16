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
/// \file H3DScriptNode.cpp
/// \brief cpp file for H3DScriptNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3DScriptNode.h>

using namespace H3D;

H3DNodeDatabase H3DScriptNode::database( 
        "H3DScriptNode", 
        NULL,
        typeid( H3DScriptNode ),
        &X3DChildNode::database 
        );

namespace H3DScriptNodeInternals {
  FIELDDB_ELEMENT( H3DScriptNode, url, INITIALIZE_ONLY );
}

H3DScriptNode::H3DScriptNode( Inst< MFString > _url ) : 
  X3DUrlObject( _url ) {
  type_name = "H3DScriptNode";
  database.initFields( this );
}



