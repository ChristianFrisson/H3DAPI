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
/// \file RawImageLoader.cpp
/// \brief CPP file for RawImageLoader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RawImageLoader.h>
#include <H3DUtil/LoadImageFunctions.h>

using namespace H3D;

H3DNodeDatabase RawImageLoader::database( 
        "RawImageLoader", 
        &newInstance< RawImageLoader >,
        typeid( RawImageLoader ),
        &H3DImageLoaderNode::database 
        );

namespace RawImageLoaderInternals {
  FIELDDB_ELEMENT( RawImageLoader, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RawImageLoader, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RawImageLoader, depth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RawImageLoader, pixelType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RawImageLoader, pixelComponentType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RawImageLoader, bitsPerPixel, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RawImageLoader, pixelSize, INPUT_OUTPUT );
}

RawImageLoader::RawImageLoader( Inst< SFInt32  > _width,
                                Inst< SFInt32  > _height,
                                Inst< SFInt32  > _depth,
                                Inst< SFString > _pixelType,
                                Inst< SFString > _pixelComponentType,
                                Inst< SFInt32  > _bitsPerPixel,
                                Inst< SFVec3f  > _pixelSize ) :
  width( _width ),
  height( _height ),
  depth( _depth ),
  pixelType( _pixelType ),
  pixelComponentType( _pixelComponentType ),
  bitsPerPixel( _bitsPerPixel ),
  pixelSize( _pixelSize ) {

  type_name = "RawImageLoader";
  database.initFields( this );

  width->setValue( 1 );
  height->setValue( 1 );
  depth->setValue( 1 );
  pixelType->addValidValue( "LUMINANCE" );
  pixelType->addValidValue( "LUMINANCE_ALPHA" );
  pixelType->addValidValue( "RGB" );
  pixelType->addValidValue( "RGBA" );
  pixelType->addValidValue( "BGR" );
  pixelType->addValidValue( "BGRA" );
  pixelType->addValidValue( "VEC3" );
  pixelType->setValue( "RGB" );
  pixelComponentType->addValidValue( "UNSIGNED" );
  pixelComponentType->addValidValue( "SIGNED" );
  pixelComponentType->addValidValue( "RATIONAL" );
  pixelComponentType->setValue( "UNSIGNED" );
  bitsPerPixel->setValue( 24 );
  pixelSize->setValue( Vec3f( 0, 0, 0 ) );
}

Image *RawImageLoader::loadImage( const string &url ) {
  RawImageInfo raw_image_info( width->getValue(),
                               height->getValue(),
                               depth->getValue(),
                               pixelType->getValue(),
                               pixelComponentType->getValue(),
                               bitsPerPixel->getValue(),
                               pixelSize->getValue() );
  return loadRawImage( url, raw_image_info );
}

