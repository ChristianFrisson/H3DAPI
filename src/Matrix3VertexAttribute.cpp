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
/// \file Matrix3VertexAttribute.cpp
/// \brief CPP file for Matrix3VertexAttribute, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Matrix3VertexAttribute.h>
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Matrix3VertexAttribute::database( 
                                   "Matrix3VertexAttribute", 
                                   &(newInstance<Matrix3VertexAttribute>), 
                                   typeid( Matrix3VertexAttribute ),
                                   &X3DVertexAttributeNode::database );

namespace Matrix3VertexAttributeInternals {
  FIELDDB_ELEMENT( Matrix3VertexAttribute, value, INPUT_OUTPUT );
}

Matrix3VertexAttribute::Matrix3VertexAttribute( Inst< SFNode     > _metadata,
                                                Inst< SFString   > _name,
                                                Inst< MFMatrix3f > _value ):
  X3DVertexAttributeNode( _metadata, _name ),
  value( _value ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  value->route(propertyChanged);
  value->route(vboFieldsUpToDate);

  type_name = "Matrix3VertexAttribute";
  database.initFields( this );
}

Matrix3VertexAttribute::~Matrix3VertexAttribute() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

// Perform the OpenGL commands to set the vertex attribute
// with the given index.
void Matrix3VertexAttribute::render( int value_index ) {
  if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
    GLfloat v0, v1, v2;
    const Matrix3f &m = value->getValueByIndex( value_index );
    v0 = m[0][0]; v1 = m[1][0]; v2 = m[2][0];
    glVertexAttrib3fARB( attrib_index, v0, v1, v2 );
    v0 = m[0][1]; v1 = m[1][1]; v2 = m[2][1];
    glVertexAttrib3fARB( attrib_index + 1, v0, v1, v2 );
    v0 = m[0][2]; v1 = m[1][2]; v2 = m[2][2];
    glVertexAttrib3fARB( attrib_index + 2, v0, v1, v2 );
    // TODO: bind name
    //glBindAttribLocationARB( 
  }
}

// Perform the OpenGL commands to set the vertex attributes
// as a an vertex attribute array.
void Matrix3VertexAttribute::renderArray() {
  if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
    glEnableVertexAttribArrayARB( attrib_index );
    GLfloat *data = new GLfloat[ 9 * value->size() ];
    for( unsigned int i = 0; i < value->size(); i++ ) {
      const Matrix3f &m = value->getValueByIndex( i );
      data[ i*9   ] = m[0][0];
      data[ i*9+1 ] = m[1][0];
      data[ i*9+2 ] = m[2][0];
      data[ i*9+3 ] = m[0][1];
      data[ i*9+4 ] = m[1][1];
      data[ i*9+4 ] = m[2][1];
      data[ i*9+5 ] = m[0][2];
      data[ i*9+6 ] = m[1][2];
      data[ i*9+8 ] = m[2][2];
    }
    glVertexAttribPointerARB( attrib_index,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            data );
    delete[] data;
    // TODO: bind name
  }
}

// Disable the array state enabled in renderArray().
void Matrix3VertexAttribute::disableArray() {
  if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
    glDisableVertexAttribArrayARB( attrib_index );
  }
}

// Perform the OpenGL commands to render all values as a vertex
// buffer object.
void Matrix3VertexAttribute::renderVertexBufferObject() {
  if( !value->empty() ) {
    if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
      if( !vboFieldsUpToDate->isUpToDate() ) {
        // Only transfer data when it has been modified.
        vboFieldsUpToDate->upToDate();
        if( !vbo_id ) {
          vbo_id = new GLuint;
          glGenBuffersARB( 1, vbo_id );
        }
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
        GLfloat *data = new GLfloat[ 9 * value->size() ];
        for( unsigned int i = 0; i < value->size(); i++ ) {
          const Matrix3f &m = value->getValueByIndex( i );
          data[ i*9   ] = m[0][0];
          data[ i*9+1 ] = m[1][0];
          data[ i*9+2 ] = m[2][0];
          data[ i*9+3 ] = m[0][1];
          data[ i*9+4 ] = m[1][1];
          data[ i*9+4 ] = m[2][1];
          data[ i*9+5 ] = m[0][2];
          data[ i*9+6 ] = m[1][2];
          data[ i*9+8 ] = m[2][2];
        }
        glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                         value->size() * 9 * sizeof(GLfloat),
                         data, GL_STATIC_DRAW_ARB );
        delete[] data;
      } else {
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
      }
      glEnableVertexAttribArrayARB( attrib_index );
      glVertexAttribPointerARB( attrib_index,
              3,
              GL_FLOAT,
              GL_FALSE,
              0,
              0 );
    }
  }
}

// Disable the array state enabled in renderVertexBufferObject().
void Matrix3VertexAttribute::disableVertexBufferObject() {
  if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
    glDisableVertexAttribArrayARB( attrib_index );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
  }
}

