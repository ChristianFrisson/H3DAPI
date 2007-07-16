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
/// \file TextureProperties.cpp
/// \brief CPP file for TextureProperties, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <TextureProperties.h>

using namespace H3D;

H3DNodeDatabase TextureProperties::database( 
        "TextureProperties", 
        &newInstance< TextureProperties>,
        typeid( TextureProperties ),
        &X3DNode::database 
        );

namespace TexturePropertiesInternals {
  FIELDDB_ELEMENT( TextureProperties, anisotropicDegree, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, borderColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, borderWidth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, boundaryModeS, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, boundaryModeT, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, boundaryModeR, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, magnificationFilter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, minificationFilter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureCompression, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, texturePriority, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, generateMipMaps, INPUT_OUTPUT );
}

TextureProperties::TextureProperties( 
                       Inst< SFNode  > _metadata             ,
                       Inst< SFFloat  > _anisotropicDegree   ,
                       Inst< SFColorRGBA > _borderColor      ,
                       Inst< SFInt32  > _borderWidth         ,
                       Inst< SFString > _boundaryModeS       ,
                       Inst< SFString > _boundaryModeT       ,
                       Inst< SFString > _boundaryModeR       ,
                       Inst< SFString  > _magnificationFilter,
                       Inst< SFString  > _minificationFilter ,
                       Inst< SFString > _textureCompression  ,
                       Inst< SFFloat > _texturePriority       ,
                       Inst< SFBool  > _generateMipMaps      ):
  X3DNode( _metadata ),
  anisotropicDegree ( _anisotropicDegree  ),
  borderColor ( _borderColor  ),
  borderWidth( _borderWidth ),
  boundaryModeS( _boundaryModeS ),
  boundaryModeT( _boundaryModeT ),
  boundaryModeR( _boundaryModeR ),
  magnificationFilter( _magnificationFilter ),
  minificationFilter( _minificationFilter ),
  textureCompression( _textureCompression ),
  texturePriority( _texturePriority ),
  generateMipMaps( _generateMipMaps ),
  propertyChanged( new Field ) {

  type_name = "TextureProperties";

  database.initFields( this );

  anisotropicDegree->setValue( 1.0f );
  borderColor->setValue( RGBA( 0, 0, 0, 0 ) );
  borderWidth->setValue( 0 );
  boundaryModeS->setValue( "REPEAT" );
  boundaryModeT->setValue( "REPEAT" );
  boundaryModeR->setValue( "REPEAT" );
  magnificationFilter->setValue( "FASTEST" );
  minificationFilter->setValue( "FASTEST" );
  textureCompression->setValue( "FASTEST" );
  texturePriority->setValue( 1.0f );
  generateMipMaps->setValue( false );

  anisotropicDegree->route( propertyChanged );
  borderColor->route( propertyChanged );
  borderWidth->route( propertyChanged );
  boundaryModeS->route( propertyChanged );
  boundaryModeT->route( propertyChanged );
  boundaryModeR->route( propertyChanged );
  magnificationFilter->route( propertyChanged );
  minificationFilter->route( propertyChanged );
  textureCompression->route( propertyChanged );
  texturePriority->route( propertyChanged );
  generateMipMaps->route( propertyChanged );
}
