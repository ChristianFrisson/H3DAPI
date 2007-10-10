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
/// \file RawImageLoader.cpp
/// \brief CPP file for RawImageLoader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RawImageLoader.h>
#include <fstream>

#ifdef HAVE_ZLIB
# include <zlib.h>
#endif

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
  pixelType->setValue( "RGB" );
  pixelComponentType->setValue( "UNSIGNED" );
  bitsPerPixel->setValue( 24 );
  pixelSize->setValue( Vec3f( 0, 0, 0 ) );
}

Image *RawImageLoader::loadImage(  const string &url ) {
  const string &pixel_type_string = pixelType->getValue();
  const string &pixel_component_type_string = pixelComponentType->getValue();
  
  Image::PixelType pixel_type;
  if( pixel_type_string == "LUMINANCE" )
    pixel_type = Image::LUMINANCE;
  else if( pixel_type_string == "LUMINANCE_ALPHA" )
    pixel_type = Image::LUMINANCE_ALPHA;
  else if( pixel_type_string == "RGB" ) 
    pixel_type = Image::RGB;
  else if( pixel_type_string == "RGBA" ) 
    pixel_type = Image::RGBA;
  else if( pixel_type_string == "BGR" ) 
    pixel_type = Image::BGR;
  else if( pixel_type_string == "BGRA" ) 
    pixel_type = Image::BGRA;
  else if( pixel_type_string == "VEC3" ) 
    pixel_type = Image::VEC3;
  else {
    Console(3) << "Warning: Invalid pixelType value \"" << pixel_type_string
               << "\" in  RawImageLoader. " << endl;
	  return NULL;
  }

  Image::PixelComponentType pixel_component_type;
  if( pixel_component_type_string == "SIGNED" ) 
    pixel_component_type = Image::SIGNED; 
  else if( pixel_component_type_string == "UNSIGNED" )
    pixel_component_type = Image::UNSIGNED;
  else if( pixel_component_type_string == "RATIONAL" )
    pixel_component_type = Image::RATIONAL;
  else {
    Console(3) << "Warning: Invalid pixelComponentType value \"" 
               << pixel_component_type_string
               << "\" in  RawImageLoader. " << endl;
	  return NULL;
  }
    
  unsigned int expected_size = 
    width->getValue() * height->getValue() * depth->getValue() * 
    bitsPerPixel->getValue() / 8;

  unsigned char * data = new unsigned char[expected_size];
  
  ifstream is( url.c_str(), ios::in | ios::binary );
  if( !is.good() ) {
    return NULL;
  }
  
  is.read( (char *)data, expected_size );
  int actual_size = is.gcount();
  is.close();
  
#ifdef HAVE_ZLIB
  
  if( actual_size < expected_size ){
    
    unsigned char * data2 = new unsigned char[expected_size];
    unsigned long int uncompressed_size = expected_size;
    
    int err;
    z_stream strm = {
      data, actual_size, 0,
      data2, expected_size, 0
    };
    
    err = inflateInit2(&strm,47);
    
    if( err == Z_MEM_ERROR ){
      Console(3) << "Warning: zlib memory error." << endl;
      delete[] data, data2;
      return NULL;
    }
    if( err == Z_VERSION_ERROR ){
      Console(3) << "Warning: zlib version error." << endl;
      delete[] data, data2;
      return NULL;
    }
    
    err = inflate(&strm,Z_FINISH);
    
    if( err == Z_DATA_ERROR ){
      Console(3) << "Warning: zlib unrecognizable data error." << endl;
      delete[] data, data2;
      return NULL;
    }
    if( err == Z_STREAM_ERROR ){
      Console(3) << "Warning: zlib stream error." << endl;
      delete[] data, data2;
      return NULL;
    }
    if( err == Z_BUF_ERROR ){
      Console(3) << "Warning: zlib out of memory error." << endl;
      delete[] data, data2;
      return NULL;
    }
    
    err = inflateEnd(&strm);
    
    if( err == Z_STREAM_ERROR ){
      Console(3) << "Warning: zlib stream error." << endl;
      delete[] data, data2;
      return NULL;
    }
    
    Console(2) << "Inflated compressed raw file." << endl;
    delete[] data;
    data = data2;
  }
  
#endif
  
  return new PixelImage( width->getValue(),
                         height->getValue(),
                         depth->getValue(),
                         bitsPerPixel->getValue(),
                         pixel_type,
                         pixel_component_type,
                         data,
                         false,
                         pixelSize->getValue() );
}

