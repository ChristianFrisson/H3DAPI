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
/// \file TextureCoordinate3D.cpp
/// \brief CPP file for TextureCoordinate3D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TextureCoordinate3D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureCoordinate3D::database( 
                                "TextureCoordinate3D", 
                                &(newInstance<TextureCoordinate3D>), 
                                typeid( TextureCoordinate3D ),
                                &X3DTextureCoordinateNode::database );

namespace TextureCoordinate3DInternals {
  FIELDDB_ELEMENT( TextureCoordinate3D, point, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( TextureCoordinate3D, isDynamic, INPUT_OUTPUT );
}


TextureCoordinate3D::TextureCoordinate3D( 
                                     Inst< SFNode >  _metadata,
                                     Inst< MFVec3f>  _point ) :
  X3DTextureCoordinateNode( _metadata ),
  point( _point ) {

  type_name = "TextureCoordinate3D";
  database.initFields( this );
  point->route( propertyChanged );
  point->route( vboFieldsUpToDate );
}

TextureCoordinate3D::~TextureCoordinate3D() {
}

void TextureCoordinate3D::render( int index ) {
  const Vec3f &p = point->getValueByIndex( index );
  glTexCoord3f( p.x, p.y, p.z );
}

void TextureCoordinate3D::renderForTextureUnit( int index,
                                              unsigned int texture_unit ) {
  const Vec3f &p = point->getValueByIndex( index );
  glMultiTexCoord3f( GL_TEXTURE0_ARB + texture_unit, p.x, p.y, p.z );
}

void TextureCoordinate3D::renderArray() {
  if( !point->empty() ) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(3, GL_FLOAT, 0,
                    &(*point->begin()) );
  }
}

// Disable the array state enabled in renderArray().
void TextureCoordinate3D::disableArray() {
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool TextureCoordinate3D::preRenderCheckFail ( ){
  return GLVertexAttributeObject::preRenderCheckFail ( ) ||
    point->empty ( );
}

void TextureCoordinate3D::setAttributeData ( ){
  attrib_data = (GLvoid*)&(*point->begin ( ));
  attrib_size = point->size ( ) * 3 * sizeof(GLfloat);
}

void TextureCoordinate3D::renderVBO ( ){
  glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
  if ( use_bindless )
  {
    glTexCoordFormatNV ( 3, GL_FLOAT, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    glBufferAddressRangeNV ( GL_TEXTURE_COORD_ARRAY_ADDRESS_NV, 0, vbo_GPUaddr, attrib_size );
  } else{
    glTexCoordPointer ( 3, GL_FLOAT, 0, NULL );
  }
}

void TextureCoordinate3D::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
}