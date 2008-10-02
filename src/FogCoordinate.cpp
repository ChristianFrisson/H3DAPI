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
/// \file FogCoordinate.cpp
/// \brief CPP file for FogCoordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FogCoordinate.h>
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FogCoordinate::database( 
                                   "FogCoordinate", 
				   &(newInstance<FogCoordinate>), 
                                   typeid( FogCoordinate ),
                                   &X3DGeometricPropertyNode::database );

namespace FogCoordinateInternals {
  FIELDDB_ELEMENT( FogCoordinate, depth, INPUT_OUTPUT );
}

FogCoordinate::FogCoordinate(   Inst< MFFloat  > _depth,
                                Inst< SFNode   > _metadata ):
  X3DGeometricPropertyNode( _metadata ),
  depth( _depth ) {

  type_name = "FogCoordinate";
  database.initFields( this );
  depth->route( propertyChanged );
}

// Perform the OpenGL commands to render fog until 
// the given index.
void FogCoordinate::render( int depth_index ) {
  const GLfloat d = depth->getValueByIndex( depth_index );
  glFogCoordfEXT( d);
}

// Perform the OpenGL commands to render the fog for
// the whole fog array.
void FogCoordinate::renderArray() {
  if( !depth->empty() ) {

  //  float test_array [] = {5, 10, 15, 20, 25, 30 },
    glEnableClientState(GL_FOG_COORD_ARRAY);
    glFogCoordPointerEXT(GL_FLOAT, 0,
                     &(*depth->begin()) );
   
  
    // *depth->begin() gives the first value of the MFFloat depth
    // &(*depth->begin()) gives a pointer to the adress of the 
    //   first value in depth
  }

  //void glFogCoordPointer( GLenum type, GLsizei stride, GLvoid *pointer)
  //
  //type - Specifies the data type of each fog coordinate. Symbolic 
  //constants GL_FLOAT, or GL_DOUBLE are accepted. The initial value is
  //GL_FLOAT.
  //
  //stride - Specifies the byte offset between consecutive fog coordinates.
  //If stride is 0, the array elements are understood to be tightly packed. 
  //The initial value is 0. 
  //
  //pointer - Specifies a pointer to the first coordinate of the first 
  //fog coordinate in the array. The initial value is 0. 
  }

void FogCoordinate::disableArray() {
  glDisableClientState(GL_FOG_COORD_ARRAY);
}

