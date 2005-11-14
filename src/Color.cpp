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
/// \file Color.cpp
/// \brief CPP file for Color, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Color.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Color::database( "Color", 
                                 &(newInstance<Color>),
                                 typeid( Color ),
                                 &X3DColorNode::database );

namespace ColorInternals {
  FIELDDB_ELEMENT( Color, color, INPUT_OUTPUT );
}


Color::Color( 
             Inst< MFColor>  _color,
             Inst< SFNode >  _metadata ) :
  X3DColorNode( _metadata ),
  color   ( _color    ) {
  type_name = "Color";
  database.initFields( this );

  color->route( propertyChanged );
}


/// Perform the OpenGL commands to render all verties as a vertex
/// array.
void Color::renderArray() {
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(3, GL_FLOAT, 0,
                  &(*color->begin()) );
}

/// Disable the array state enabled in renderArray().
void Color::disableArray() {
  glDisableClientState(GL_COLOR_ARRAY);
}
