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
}

CoordinateDouble::CoordinateDouble( 
                            Inst< SFNode  > _metadata,
                            Inst< MFVec3d > _point ) :
  X3DCoordinateNode( _metadata ),
  point( _point ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "CoordinateDouble";
  database.initFields( this );

  point->route( propertyChanged );
  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  point->route( vboFieldsUpToDate );
}

CoordinateDouble::~CoordinateDouble() {
  // Delete buffer if it was allocated.
  if( GLEW_ARB_vertex_buffer_object && vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
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

// Perform the OpenGL commands to render all vertices as a vertex
// buffer object.
void CoordinateDouble::renderVertexBufferObject() {
  if( !point->empty() ) {
    if( !vboFieldsUpToDate->isUpToDate() ) {
      // Only transfer data when it has been modified.
      vboFieldsUpToDate->upToDate();
      if( !vbo_id ) {
        vbo_id = new GLuint;
        glGenBuffersARB( 1, vbo_id );
      }
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                       point->size() * 3 * sizeof(GLdouble),
                       &(*point->begin()), GL_STATIC_DRAW_ARB );
    } else {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, NULL );
  }
}

// Disable the array state enabled in renderVertexBufferObject().
void CoordinateDouble::disableVertexBufferObject() {
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

