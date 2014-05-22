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
/// \file FloatVertexAttribute.cpp
/// \brief CPP file for FloatVertexAttribute, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FloatVertexAttribute.h>
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FloatVertexAttribute::database( 
                                   "FloatVertexAttribute", 
                                   &(newInstance<FloatVertexAttribute>),
                                   typeid( FloatVertexAttribute ),
                                   &X3DVertexAttributeNode::database );

namespace FloatVertexAttributeInternals {
  FIELDDB_ELEMENT( FloatVertexAttribute, value, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FloatVertexAttribute, numComponents, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( FloatVertexAttribute, isDynamic, INPUT_OUTPUT );
}

FloatVertexAttribute::FloatVertexAttribute( Inst< SFNode   > _metadata,
                                            Inst< SFString > _name,
                                            Inst< MFFloat  > _value,
                                            Inst< SFInt32  > _numComponents):
  X3DVertexAttributeNode( _metadata, _name ),
  value( _value ),
  numComponents( _numComponents ){

  type_name = "FloatVertexAttribute";
  database.initFields( this );

  value->route(propertyChanged);
  numComponents->route(propertyChanged);

  value->route( vboFieldsUpToDate );
  numComponents->route( vboFieldsUpToDate );

  numComponents->setValue( 4 );
}

FloatVertexAttribute::~FloatVertexAttribute() {
}

// Perform the OpenGL commands to set the vertex attribute
// with the given index.
void FloatVertexAttribute::render( int value_index ) {
  if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
    GLfloat v0, v1, v2, v3;
    H3DInt32 nr_components = numComponents->getValue();
    int index = value_index * nr_components;
    v0 = value->getValueByIndex( index );
    v1 = nr_components > 1 ? 
      (GLfloat) value->getValueByIndex( index + 1 ) : 0;
    v2 = nr_components > 2 ? 
      (GLfloat) value->getValueByIndex( index + 2 ) : 0;
    v3 = nr_components > 3 ? 
      (GLfloat) value->getValueByIndex( index + 3 ) : 1;
    glVertexAttrib4fARB( attrib_index, v0, v1, v2, v3 );
  }
}

// Perform the OpenGL commands to set the vertex attributes
// as a an vertex attribute array.
void FloatVertexAttribute::renderArray() {
  if( !value->empty() ) {
    if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
      glEnableVertexAttribArrayARB( attrib_index );
      glVertexAttribPointerARB( attrib_index,
        numComponents->getValue(),
        GL_FLOAT,
        GL_FALSE,
        0,
        &(*value->begin() ) );
    }
  }
}

// Disable the array state enabled in renderArray().
void FloatVertexAttribute::disableArray() {
  if( GLEW_ARB_vertex_program && attrib_index >= 0 ) {
    glDisableVertexAttribArrayARB( attrib_index );
  }
}

bool FloatVertexAttribute::preRenderCheckFail ( ){
  return GLVertexAttributeObject::preRenderCheckFail ( ) ||
    value->empty ( ) || attrib_index < 0;
}

void FloatVertexAttribute::setAttributeData ( ){
  attrib_data = (GLvoid*)&(*value->begin ( ));
  attrib_size = value->size ( )*sizeof(GLfloat);
}

void FloatVertexAttribute::renderVBO ( ){
  glEnableVertexAttribArrayARB ( attrib_index );
  if ( use_bindless )
  {
    glVertexAttribFormatNV ( attrib_index, numComponents->getValue(), GL_FLOAT, GL_FALSE, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    glBufferAddressRangeNV ( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, attrib_index, vbo_GPUaddr, attrib_size );
  } else{
    glVertexAttribPointerARB ( attrib_index,
      numComponents->getValue ( ),
      GL_FLOAT,
      GL_FALSE,
      0,
      0 );
  }
}

void FloatVertexAttribute::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableVertexAttribArrayARB ( attrib_index );
}