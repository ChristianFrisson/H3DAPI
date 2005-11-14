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
/// \file ColorRGBA.cpp
/// \brief CPP file for ColorRGBA, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ColorRGBA.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ColorRGBA::database( "ColorRGBA", 
                                     &(newInstance<ColorRGBA>),
                                     typeid( ColorRGBA ),
                                     &X3DColorNode::database );

namespace ColorRGBAInternals {
  FIELDDB_ELEMENT( ColorRGBA, color, INPUT_OUTPUT );
}


ColorRGBA::ColorRGBA( 
                     Inst< MFColorRGBA>  _color,
                     Inst< SFNode     >  _metadata ) :
  X3DColorNode( _metadata ),
  color   ( _color    ) {

  type_name = "ColorRGBA";
  database.initFields( this );

  color->route( propertyChanged );
}

/// Perform the OpenGL commands to render all verties as a vertex
/// array.
void ColorRGBA::renderArray() {
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, 0,
                  &(*color->begin()) );
}

/// Disable the array state enabled in renderArray().
void ColorRGBA::disableArray() {
  glDisableClientState(GL_COLOR_ARRAY);
}

