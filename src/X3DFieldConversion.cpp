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
/// \file X3DFieldConversion.cpp
/// \brief This file contains functions for convertion from a string to an 
/// value of an X3D field type.
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/X3DFieldConversion.h>

using namespace H3D;
using namespace X3D;

PixelImage *X3D::X3DStringTo3DImage( const string &x3d_string ) {
  typedef Convert::X3DFieldConversionError ConversionError;
  const char *s = x3d_string.c_str();
  const char *t1;
    
  s = Convert::skipWhitespaces( s );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. Number components not specified." ); 
  }
  int nr_components = Convert::getValue<int>( s, t1 );
  s = Convert::skipWhitespaces( t1 );
    
  Image::PixelType pixel_type;
  unsigned int bits_per_pixel;

  switch( nr_components ) {
  case 1: 
    pixel_type = Image::LUMINANCE;
    bits_per_pixel = 8;
    break;
  case 2: 
    pixel_type = Image::LUMINANCE_ALPHA;
    bits_per_pixel = 16;
    break;
  case 3: 
    pixel_type = Image::RGB;
    bits_per_pixel = 24;
    break;
  case 4: 
    pixel_type = Image::RGBA;
    bits_per_pixel = 32;
    break;
  default:
    stringstream s;
    s << "SFImage. Component value is "
      << nr_components << ". Must be 1, 2, 3 or 4. ";
    throw ConversionError( s.str() );
  }

  s = Convert::skipWhitespaces( t1 );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. No width value specified." ); 
  }
  int width = Convert::getValue<int>( s, t1 );
  s = Convert::skipWhitespaces( t1 );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. No height value specified." ); 
  }
  int height = Convert::getValue<int>( s, t1 );

  s = Convert::skipWhitespaces( t1 );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. No depth value specified." ); 
  }
  int depth = Convert::getValue<int>( s, t1 );
  s = Convert::skipWhitespaces( t1 );

  unsigned char *data =  Convert::readImageData( s, 
                                                 width,
                                                 height,
                                                 depth,
                                                 nr_components );
  return new PixelImage( width,
                         height, 
                         depth,
                         bits_per_pixel,
                         pixel_type,
                         Image::UNSIGNED,
                         data, false );
}

PixelImage *X3D::X3DStringTo2DImage( const string &x3d_string ) {
  typedef Convert::X3DFieldConversionError ConversionError;
  const char *s = x3d_string.c_str();
  const char *t1;
    
  s = Convert::skipWhitespaces( s );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. No width value specified." ); 
  }
  int width = Convert::getValue<int>( s, t1 );
  s = Convert::skipWhitespaces( t1 );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. No height value specified." ); 
  }
  int height = Convert::getValue<int>( s, t1 );
  s = Convert::skipWhitespaces( t1 );
  if( s[0] == '\0' ) {
    throw ConversionError( "SFImage. Number components not specified." ); 
  }
  int nr_components = Convert::getValue<int>( s, t1 );
  s = Convert::skipWhitespaces( t1 );
    
  Image::PixelType pixel_type;
  unsigned int bits_per_pixel;

  switch( nr_components ) {
  case 1: 
    pixel_type = Image::LUMINANCE;
    bits_per_pixel = 8;
    break;
  case 2: 
    pixel_type = Image::LUMINANCE_ALPHA;
    bits_per_pixel = 16;
    break;
  case 3: 
    pixel_type = Image::RGB;
    bits_per_pixel = 24;
    break;
  case 4: 
    pixel_type = Image::RGBA;
    bits_per_pixel = 32;
    break;
  default:
    stringstream s;
    s << "SFImage. Component value is "
      << nr_components << ". Must be 1, 2, 3 or 4. ";
    throw ConversionError( s.str() );
  }

  unsigned char *data =  Convert::readImageData( s, 
                                                 width,
                                                 height,
                                                 1,
                                                 nr_components );
  return new PixelImage( width,
                         height, 
                         1,
                         bits_per_pixel,
                         pixel_type,
                         Image::UNSIGNED,
                         data, false );
}




unsigned char * X3D::Convert::readImageData( const char *s, 
                                             int width,
                                             int height,
                                             int depth,
                                             int nr_components ) {
  const char *t1 = Convert::skipWhitespaces( s ); 
  const char *t2;

  int nr_pixels = width * height * depth;

  unsigned char *data = new unsigned char[ nr_pixels * nr_components ];
  int x = 0, y = 0, z = 0;
  for( z = 0; z < depth; ++z )
    for( y = 0; y < height; ++y )
      for( x = 0; x < width; ++x ) {
        // pixel_index is the index in the data vector where
        // the current pixel starts
        unsigned int pixel_index = 
          ( ( z * height  + y )* width + x ) * nr_components; 

        if( t1[0] == '\0' ) break;
        int pixel;  
        try {
          pixel = getValue< int >( t1, t2 );
        } catch( const X3DFieldConversionError & ) {
          delete data;
          stringstream s;
          s << "SFImage. Con't convert pixel ("
            << x << ", " << y << ", " << z << ") to int";
          throw X3DFieldConversionError( s.str() );
        } 
        t1 = Convert::skipWhitespaces( t2 );  
        switch( nr_components ) {
        case 1: {
          // Make sure the value is between 0-255
          if( pixel & 0xFFFFFF00 ) {
            delete data;
            stringstream s;
            s << "SFImage. Single component pixel value of pixel ("
              << x << ", " << y << ", " << z << ") is " 
              << pixel << ". Pixel component "
              << "values must be between 0 and 255. ";
            throw X3DFieldConversionError( s.str() );
          }
          unsigned char c = pixel & 0x000000FF;
          data[ pixel_index ] = c;
          break;
        }
        case 2: {
          // Make sure the value is between 0x0000 and 0xFFFF
          if( pixel & 0xFFFF0000 ) {
            delete data;
            stringstream s;
            s << "SFImage. Two component pixel value of pixel ("
              << x << ", " << y << ", " << z << ") is " 
              << pixel << ". Pixel component "
              << "values must be between 0x0000 and 0xFFFF.";
            throw X3DFieldConversionError( s.str() );
          }
          unsigned char c = ( pixel & 0x0000FF00 ) >> 8;
          data[ pixel_index ] = c;
          c = pixel  & 0x000000FF;
          data[ pixel_index + 1 ] = c;
          break;
        }
        case 3: {
          // Make sure the value is between 0x000000 and 0xFFFFFF
          if( pixel & 0xFF000000 ) {
            delete data;
            stringstream s;
            s << "SFImage. Three component pixel value of pixel "
              << x << ", " << y << ", " << z << ") is " 
              << pixel << ". Pixel component "
              << "values must be between 0x000000 and 0xFFFFFF.";
            throw X3DFieldConversionError( s.str() );
          }
          unsigned char c = ( pixel & 0x00FF0000 ) >> 16;
          data[ pixel_index ] =  c;
          c = (pixel & 0x0000FF00) >> 8;
          data[ pixel_index + 1 ] =  c;
          c = pixel  & 0x000000FF;
          data[ pixel_index + 2 ] =  c;
          break;
        }
        case 4: {
          unsigned char c = ( pixel & 0xFF000000 ) >> 24;
          data[ pixel_index ] =  c;
          c = ( pixel & 0x00FF0000 ) >> 16;
          data[ pixel_index + 1 ] =  c;
          c = ( pixel & 0x0000FF00 ) >> 8;
          data[ pixel_index + 2 ] =  c;
          c = pixel  & 0x000000FF;
          data[ pixel_index + 3 ] =  c;
          break;
        }
        default: 
          delete data;
          stringstream s;
          s << "SFImage. Component value is "
            << nr_components << ". Must be 1, 2, 3 or 4. ";
          throw X3DFieldConversionError( s.str() );
        }           
        
      }
  
  int pixels_processed = x * y * z;
  if( pixels_processed != nr_pixels ) {
    delete data;
    stringstream s;
    s << "SFImage.  Only " << pixels_processed << " pixels. " 
      << "Expecting " << nr_pixels << " pixels.";
    throw X3DFieldConversionError( s.str() );
  } else if( t1[0] != '\0' ) {
    delete data;
    stringstream s;
    s << "SFImage.  More than " << nr_pixels << " pixels. " 
      << "Expecting " << nr_pixels << " pixels.";
    throw X3DFieldConversionError( s.str() );
  }
  return data;
}
