//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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

#include <H3D/Normal.h>

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
  vector  ( _vector ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "Normal";
  database.initFields( this );

  vector->route( propertyChanged );
  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  vector->route( vboFieldsUpToDate );
}

Normal::~Normal() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

// Perform the OpenGL commands to render all vertices as a vertex
// array.
void Normal::renderArray() {
  if( !vector->empty() ) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer( GL_FLOAT, 0,
                   &(*vector->begin()) );
  }
}

// Disable the array state enabled in renderArray().
void Normal::disableArray() {
  glDisableClientState(GL_NORMAL_ARRAY);
}

// Perform the OpenGL commands to render all vertices as a vertex
// buffer object.
void Normal::renderVertexBufferObject() {
  if( !vector->empty() ) {
    if( !vboFieldsUpToDate->isUpToDate() ) {
      // Only transfer data when it has been modified.
      vboFieldsUpToDate->upToDate();
      if( !vbo_id ) {
        vbo_id = new GLuint;
        glGenBuffersARB( 1, vbo_id );
      }
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                       vector->size() * 3 * sizeof(GLfloat),
                       &(*vector->begin()), GL_STATIC_DRAW_ARB );
    } else {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
    }
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, NULL );
  }
}

// Disable the array state enabled in renderVertexBufferObject().
void Normal::disableVertexBufferObject() {
  glDisableClientState(GL_NORMAL_ARRAY);
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}


