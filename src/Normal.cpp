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
/// \file Normal.cpp
/// \brief CPP file for Normal, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Normal.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Normal::database( 
                                 "Normal", 
                                 &(newInstance<Normal>), 
                                 typeid( Normal ),
                                 &X3DNormalNode::database );

namespace NormalInternals {
  FIELDDB_ELEMENT( Normal, vector, INPUT_OUTPUT );
}


Normal::Normal( 
               Inst< SFNode >  _metadata,
               Inst< MFVec3f>  _vector ) :
  X3DNormalNode( _metadata ),
  vector  ( _vector   ) {

  type_name = "Normal";
  database.initFields( this );

  vector->route( propertyChanged );

}

/// Perform the OpenGL commands to render all verties as a vertex
/// array.
void Normal::renderArray() {
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer( GL_FLOAT, 0,
                   &(*vector->begin()) );
}

/// Disable the array state enabled in renderArray().
void Normal::disableArray() {
  glDisableClientState(GL_NORMAL_ARRAY);
}
