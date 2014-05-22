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
/// \file TextureCoordinate.cpp
/// \brief CPP file for TextureCoordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TextureCoordinate.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureCoordinate::database( 
                                      "TextureCoordinate", 
                                      &(newInstance<TextureCoordinate>), 
                                      typeid( TextureCoordinate ),
                                      &X3DTextureCoordinateNode::database );

namespace TextureCoordinateInternals {
  FIELDDB_ELEMENT( TextureCoordinate, point, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( TextureCoordinate, isDynamic, INPUT_OUTPUT );
}


TextureCoordinate::TextureCoordinate( 
                                     Inst< SFNode >  _metadata,
                                     Inst< MFVec2f>  _point ) :
  X3DTextureCoordinateNode( _metadata ),
  point( _point ) {

  type_name = "TextureCoordinate";
  database.initFields( this );
  point->route( propertyChanged );
  point->route( vboFieldsUpToDate );
}

TextureCoordinate::~TextureCoordinate() {
}

void TextureCoordinate::render( int index ) {
  const Vec2f &p = point->getValueByIndex( index );
  glTexCoord2f( p.x, p.y );
}

void TextureCoordinate::renderForTextureUnit( int index,
                                              unsigned int texture_unit ) {
  const Vec2f &p = point->getValueByIndex( index );
  glMultiTexCoord2f( GL_TEXTURE0_ARB + texture_unit, p.x, p.y );
}

void TextureCoordinate::renderArray() {
  if( !point->empty() ) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0,
                    &(*point->begin()) );
  }
}

// Disable the array state enabled in renderArray().
void TextureCoordinate::disableArray() {
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool TextureCoordinate::preRenderCheckFail ( ){
  return GLVertexAttributeObject::preRenderCheckFail ( ) ||
    point->empty ( );
}

void TextureCoordinate::setAttributeData ( ){
  attrib_data = (GLvoid*)&(*point->begin ( ));
  attrib_size = point->size ( ) * 2 * sizeof(GLfloat);
}

void TextureCoordinate::renderVBO ( ){
  glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
  if ( use_bindless )
  {
    glTexCoordFormatNV ( 2, GL_FLOAT, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    GLint active_texutre_unit;
    glGetIntegerv(GL_CLIENT_ACTIVE_TEXTURE_ARB, &active_texutre_unit );
    glBufferAddressRangeNV ( GL_TEXTURE_COORD_ARRAY_ADDRESS_NV, active_texutre_unit-GL_TEXTURE0_ARB, vbo_GPUaddr, attrib_size );
  } else{
    glTexCoordPointer ( 2, GL_FLOAT, 0, NULL );
  }
  
}

void TextureCoordinate::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
}
