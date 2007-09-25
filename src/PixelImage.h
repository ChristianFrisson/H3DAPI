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
/// \file PixelImage.h
/// \brief Header file for the PixelImage class.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PIXELIMAGE_H__
#define __PIXELIMAGE_H__

#include <H3DApi.h>
#include <H3DTypes.h>
#include <Image.h>
#include <vector>

namespace H3D {

  class H3DAPI_API PixelImage: public Image {
  public:
    /// Constructor. 
    ///
    PixelImage( unsigned int _width,
                unsigned int _height,
                unsigned int _depth,
                unsigned int _bits_per_pixel,
                PixelType _pixel_type,
                PixelComponentType _pixel_component_type,
                unsigned char *data,
                bool copy_data = false,
                const Vec3f &_pixel_size = Vec3f( 0, 0, 0 ) );

    /// Constructor.
    /// A new PixelImage with the given dimensions is created by 
    /// resampling the given image. Trilinear interpolation is used
    /// to get new pixel values.
    PixelImage( Image *image,
                unsigned int new_width,
                unsigned int new_height,
                unsigned int new_depth );

    ~PixelImage() {
      if( image_data )
        delete[] image_data;
    }

    /// Returns the width of the image in pixels.
    virtual unsigned int width() {
      return w;
    }

    /// Returns the height of the image in pixels.
    virtual unsigned int height() {
      return h;
    }

    /// Returns the depth of the image in pixels.
    virtual unsigned int depth() {
      return d;
    }

    /// Returns the size of the pixel in x, y and z direction in metres.
    virtual Vec3f pixelSize() {
      return pixel_size;
    }

    /// Returns the number of bits used for each pixel in the image.
    virtual unsigned int bitsPerPixel() {
      return bits_per_pixel;
    }

    /// Returns the PixelType of the image.
    virtual PixelType pixelType() {
      return pixel_type;
    }
        
    /// Returns the PixelComponentType of the image.
    virtual PixelComponentType pixelComponentType() {
      return pixel_component_type;
    }
        
    /// Returns a pointer to the raw image data. 
    virtual void *getImageData() {
      return image_data;
    }

    /// Set the height of the image in pixels.
    virtual void setHeight( unsigned int height ) {
      h = height;
    }

    /// Set the width of the image in pixels.
    virtual void setWidth( unsigned int width ) {
      w = width;
    }

    /// Set the depth of the image in pixels.
    virtual void setDepth( unsigned int depth ) {
      d = depth;
    }


    /// Set the size of the pixel in x, y and z direction in metres.
    virtual void setPixelSize( const Vec3f &s ) {
      pixel_size = s;
    }

    /// Set the number of bits used for each pixel in the image.
    virtual void setbitsPerPixel( unsigned int b ) {
      bits_per_pixel = b;
    }

    /// Set the PixelType of the image.
    virtual void setPixelType( const PixelType &pt) {
      pixel_type = pt;
    }
        
    /// Set the PixelComponentType of the image.
    virtual void setPixelComponentType( const PixelComponentType &pct ) {
      pixel_component_type = pct;
    }
        
    /// Set a pointer to the raw image data. 
    virtual void setImageData( unsigned char * data, bool copy_data = false ) {
      if( image_data ) delete image_data;
      if( copy_data ) {
        unsigned int size = (w * h * d * bits_per_pixel)/8;
        image_data = new unsigned char[ size ];
        memcpy( image_data, data, size );
      } else {
        image_data = data;
      }
    }

  protected:
    unsigned int w, h, d;
    unsigned int bits_per_pixel;
    PixelType pixel_type;
    PixelComponentType pixel_component_type;
    Vec3f pixel_size;
    unsigned char *image_data;
  };

    
}

#endif


