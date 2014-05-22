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
/// \file Coordinate.cpp
/// \brief CPP file for Coordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Coordinate.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Coordinate::database( "Coordinate", 
                                      &(newInstance<Coordinate>),
                                      typeid( Coordinate ),
                                      &X3DCoordinateNode::database );

namespace CoordinateInternals {
  FIELDDB_ELEMENT( Coordinate, point, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( Coordinate, isDynamic, INPUT_OUTPUT );
}

Coordinate::Coordinate( 
                       Inst< SFNode  >  _metadata,
                       Inst< MFVec3f > _point) :
  X3DCoordinateNode( _metadata ),
  point   ( _point    ) {

  type_name = "Coordinate";
  database.initFields( this );

  point->route( propertyChanged );
  point->route( vboFieldsUpToDate );
}

Coordinate::~Coordinate() {
}

// Perform the OpenGL commands to render all vertices as a vertex
// array.
void Coordinate::renderArray() {
  if( !point->empty() ) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0,
                    &(*point->begin()) );
  }
}

// Disable the array state enabled in renderArray().
void Coordinate::disableArray() {
  glDisableClientState(GL_VERTEX_ARRAY);
}

bool Coordinate::preRenderCheckFail ( ){
  return GLVertexAttributeObject::preRenderCheckFail ( ) ||
    point->empty ( );
}

void Coordinate::setAttributeData ( ){
  attrib_data =(GLvoid*) &(*point->begin ( ));
  attrib_size = point->size ( ) * 3 * sizeof(GLfloat);
}

void Coordinate::renderVBO ( ){
  glEnableClientState ( GL_VERTEX_ARRAY );
  if ( use_bindless )
  {
    glVertexFormatNV ( 3, GL_FLOAT, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    glBufferAddressRangeNV ( GL_VERTEX_ARRAY_ADDRESS_NV, 0, vbo_GPUaddr, attrib_size );
  } else
  {
    glVertexPointer ( 3, GL_FLOAT, 0, NULL );
  }
}

void Coordinate::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableClientState ( GL_VERTEX_ARRAY );
}
