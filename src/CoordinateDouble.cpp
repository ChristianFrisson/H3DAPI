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
/// \file CoordinateDouble.cpp
/// \brief CPP file for CoordinateDouble, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CoordinateDouble.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system
H3DNodeDatabase CoordinateDouble::database( "CoordinateDouble",
                                            &(newInstance<CoordinateDouble>),
                                            typeid( CoordinateDouble ),
                                            &X3DCoordinateNode::database );

namespace CoordinateDoubleInternals {
  FIELDDB_ELEMENT( CoordinateDouble, point, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( CoordinateDouble, isDynamic, INPUT_OUTPUT );
}

CoordinateDouble::CoordinateDouble( 
                            Inst< SFNode  > _metadata,
                            Inst< MFVec3d > _point ) :
  X3DCoordinateNode( _metadata ),
  point( _point ) {

  type_name = "CoordinateDouble";
  database.initFields( this );

  point->route( propertyChanged );
  point->route( vboFieldsUpToDate );
}

CoordinateDouble::~CoordinateDouble() {
}

// Perform the OpenGL commands to render all vertices as a vertex
/// array.
void CoordinateDouble::renderArray() {
  if ( !point->empty() ) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer( 3, GL_DOUBLE, 0,
                     &(*point->begin()) );

  }
}

// Disable the array state enabled in renderArray().
void CoordinateDouble::disableArray() {
  glDisableClientState(GL_VERTEX_ARRAY);
}

bool CoordinateDouble::preRenderCheckFail ( )  {
  return GLVertexAttributeObject::preRenderCheckFail ( ) ||
    point->empty ( );
}

void CoordinateDouble::setAttributeData ( ){
  attrib_data = (GLvoid*)&(*point->begin ( ));
  attrib_size = point->size ( ) * 3 * sizeof(GLdouble);
}

void CoordinateDouble::renderVBO ( ){
  glEnableClientState ( GL_VERTEX_ARRAY );
  if ( use_bindless )
  {
    glVertexFormatNV ( 3, GL_DOUBLE, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    glBufferAddressRangeNV ( GL_VERTEX_ARRAY_ADDRESS_NV, 0, vbo_GPUaddr, attrib_size );
  } else
  {
    glVertexPointer ( 3, GL_DOUBLE, 0, NULL );
  }
}

void CoordinateDouble::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableClientState ( GL_VERTEX_ARRAY );
}
