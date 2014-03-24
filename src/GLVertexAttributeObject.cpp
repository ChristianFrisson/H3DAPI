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
/// \file GLVertexBufferObject.cpp
/// \brief cpp file for GLVertexAttributeObject class.
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GLVertexAttributeObject.h>

using namespace H3D;

GLVertexAttributeObject::GLVertexAttributeObject ( VERTEXATTRIBUTE::VERTEXATTRIBUTETYPE type) :
vboFieldsUpToDate ( new Field ),
isDynamic(new SFBool),
vbo_id ( NULL ),
attrib_type( type ),
attrib_size( 0 ),
attrib_data( NULL ){
  vboFieldsUpToDate->setName ( "vboFieldsUpToDate" );
  isDynamic->route ( vboFieldsUpToDate );
  isDynamic->setValue ( false );
}

void GLVertexAttributeObject::updateVertexBufferObject ( ){

  setAttributeData ( );
  if ( !GLEW_ARB_vertex_program ) return;
  if ( !vboFieldsUpToDate->isUpToDate() ){
    vboFieldsUpToDate->upToDate ( );
    if ( !vbo_id ){
      vbo_id = new GLuint;
      glGenBuffersARB ( 1, vbo_id );
    }
    glBindBuffer ( GL_ARRAY_BUFFER, *vbo_id );
    if ( isDynamic->getValue() )
    {
      glBufferData ( GL_ARRAY_BUFFER, attrib_size, NULL, GL_STREAM_DRAW );
      glBufferData ( GL_ARRAY_BUFFER, attrib_size, attrib_data, GL_STREAM_DRAW );
    } else
    {
      glBufferData ( GL_ARRAY_BUFFER, attrib_size, attrib_data, GL_STATIC_DRAW );
    }
  } else
  {
    glBindBuffer ( GL_ARRAY_BUFFER, *vbo_id );
  }
}

void GLVertexAttributeObject::renderVertexBufferObject ( ){

  updateVertexBufferObject ( );
  renderVBO ( );

}

void GLVertexAttributeObject::disableVertexBufferObject ( ){
  disableVBO ( );
}


