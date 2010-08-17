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
/// \file TextureCoordinate4D.cpp
/// \brief CPP file for TextureCoordinate4D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TextureCoordinate4D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureCoordinate4D::database( 
                                "TextureCoordinate4D", 
                                &(newInstance<TextureCoordinate4D>), 
                                typeid( TextureCoordinate4D ),
                                &X3DTextureCoordinateNode::database );

namespace TextureCoordinate4DInternals {
  FIELDDB_ELEMENT( TextureCoordinate4D, point, INPUT_OUTPUT );
}


TextureCoordinate4D::TextureCoordinate4D( 
                                     Inst< SFNode >  _metadata,
                                     Inst< MFVec4f>  _point ) :
  X3DTextureCoordinateNode( _metadata ),
  point( _point ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "TextureCoordinate4D";
  database.initFields( this );
  point->route( propertyChanged );
  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  point->route( vboFieldsUpToDate );
}

TextureCoordinate4D::~TextureCoordinate4D() {
  // Delete buffer if it was allocated.
  if( GLEW_ARB_vertex_buffer_object && vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

void TextureCoordinate4D::render( int index ) {
  const Vec4f &p = point->getValueByIndex( index );
  glTexCoord4f( p.x, p.y, p.z, p.w );
}

void TextureCoordinate4D::renderForTextureUnit( int index,
                                                unsigned int texture_unit ) {
  const Vec4f &p = point->getValueByIndex( index );
  glMultiTexCoord4f( GL_TEXTURE0_ARB + texture_unit, p.x, p.y, p.z, p.w );
}

void TextureCoordinate4D::renderArray() {
  if( !point->empty() ) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, 0,
                      &(*point->begin()) );
  }
}

// Disable the array state enabled in renderArray().
void TextureCoordinate4D::disableArray() {
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// Perform the OpenGL commands to render all vertices as a vertex
// buffer object.
void TextureCoordinate4D::renderVertexBufferObject() {
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
                       point->size() * 4 * sizeof(GLfloat),
                       &(*point->begin()), GL_STATIC_DRAW_ARB );
    } else {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
    }
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, 0, NULL );
  }
}

// Disable the array state enabled in renderVertexBufferObject().
void TextureCoordinate4D::disableVertexBufferObject() {
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

