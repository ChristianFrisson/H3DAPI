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
  FIELDDB_ELEMENT ( TextureCoordinate4D, isDynamic, INPUT_OUTPUT );
}


TextureCoordinate4D::TextureCoordinate4D( 
                                     Inst< SFNode >  _metadata,
                                     Inst< MFVec4f>  _point ) :
  X3DTextureCoordinateNode( _metadata ),
  point( _point ) {

  type_name = "TextureCoordinate4D";
  database.initFields( this );
  point->route( propertyChanged );
  point->route( vboFieldsUpToDate );
}

TextureCoordinate4D::~TextureCoordinate4D() {
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

bool TextureCoordinate4D::preRenderCheckFail ( ){
  return GLVertexAttributeObject::preRenderCheckFail ( ) ||
    point->empty ( );
}

void TextureCoordinate4D::setAttributeData ( ){
  attrib_data = (GLvoid*)&(*point->begin ( ));
  attrib_size = point->size ( ) * 4 * sizeof(GLfloat);
}

void TextureCoordinate4D::renderVBO ( ){
  glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
  if ( use_bindless )
  {
    glTexCoordFormatNV ( 4, GL_FLOAT, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    glBufferAddressRangeNV ( GL_TEXTURE_COORD_ARRAY_ADDRESS_NV, 0, vbo_GPUaddr, attrib_size );
  } else{
    glTexCoordPointer ( 4, GL_FLOAT, 0, NULL );
  }
}

void TextureCoordinate4D::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
}