//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Image.h
/// \brief Header file for Image, the base class for all images.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "H3DApi.h"
#include "H3DTypes.h"
#include "RefCountedClass.h"
#include <assert.h>

/// H3D API namespace
namespace H3D {
  /// Virtual base class for all images containing virtual functions that
  /// all Image classes must define.
  ///
  class H3DAPI_API Image: public RefCountedClass {
  public:
    /// Type that defines what format each pixel in the image is
    /// on.
    typedef enum {
      LUMINANCE, 
      LUMINANCE_ALPHA,
      RGB,
      RGBA,
      BGR,
      BGRA,
      VEC3
      // Color table bitmap??
    } PixelType;
        
    /// Type of each component in the pixel. 
    typedef enum {
      SIGNED,
      UNSIGNED,
      RATIONAL
    } PixelComponentType;

    /// Returns the width of the image in pixels.
    virtual unsigned int width() = 0;
    /// Returns the height of the image in pixels.
    virtual unsigned int height() = 0;
    /// Returns the depth of the image in pixels.
    virtual unsigned int depth() = 0;
    /// Returns the number of bits used for each pixel in the image.
    virtual unsigned int bitsPerPixel() = 0;
    /// Returns the PixelType of the image.
    virtual PixelType pixelType() = 0;
    /// Returns the PixelComponentType of the image.
    virtual PixelComponentType pixelComponentType() = 0;
    /// Returns the size of the pixel in x, y and z direction in metres.
    /// By default all pixels have the size Vec3f( 1, 1, 1 ).
    virtual Vec3f pixelSize() {
      return Vec3f( 1, 1, 1 );
    }

    /// Returns a pointer to the raw image data. The ownership of the 
    /// pointer is held by the Image class, if any memory allocations
    /// are made for the pointer the Image class is responsible for 
    /// deallocating that memory.
    virtual void *getImageData() = 0;


    virtual void getElement( void *value, int x = 0, int y = 0, int z = 0 ) {
      unsigned int byte_rem = bitsPerPixel() % 8;
      unsigned int bytes_per_pixel = bitsPerPixel() / 8;
      assert( byte_rem == 0 );

      if( byte_rem != 0 ) {
        bytes_per_pixel++;
      }
      
      unsigned char *data = (unsigned char *) getImageData();

      memcpy( value, 
              &data[ ( ( z * height() + y ) * width() + x ) * bytes_per_pixel ],
              bytes_per_pixel );
    }
    
    virtual void setElement( void *value, int x = 0, int y = 0, int z = 0 ) {
      unsigned int byte_rem = bitsPerPixel() % 8;
      unsigned int bytes_per_pixel = bitsPerPixel() / 8;
      assert( byte_rem == 0 );
      if( byte_rem != 0 )
        bytes_per_pixel++;

      unsigned char *data = (unsigned char *)getImageData();
      memcpy( &data[ ( ( z * height() + y ) * width() + x ) * bytes_per_pixel ],
              value, 
              bytes_per_pixel );
    }
  };
}

#endif


