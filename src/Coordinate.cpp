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
/// \file Coordinate.cpp
/// \brief CPP file for Coordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Coordinate.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Coordinate::database( "Coordinate", 
                                      &(newInstance<Coordinate>),
                                      typeid( Coordinate ),
                                      &X3DCoordinateNode::database );

namespace CoordinateInternals {
  FIELDDB_ELEMENT( Coordinate, point, INPUT_OUTPUT );
}

Coordinate::Coordinate( 
                       Inst< SFNode >  _metadata,
                       Inst< MFVec3f > _point ) :
  X3DCoordinateNode( _metadata ),
  point   ( _point    ) {

  type_name = "Coordinate";
  database.initFields( this );

  point->route( propertyChanged );
}


/// Perform the OpenGL commands to render all verties as a vertex
/// array.
void Coordinate::renderArray() {
  if( !point->empty() ) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0,
                    &(*point->begin()) );
  }
}

/// Disable the array state enabled in renderAttay().
void Coordinate::disableArray() {
  glDisableClientState(GL_VERTEX_ARRAY);
}
