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
/// \file FreeImageImage.h
/// \brief Image class encapsulating a FreeImage bitmap.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FREEIMAGEIMAGE_H__
#define __FREEIMAGEIMAGE_H__

#include "H3DApi.h"
#include "Image.h"
#include "Exception.h"

#ifdef HAVE_FREEIMAGE
#include <FreeImage.h>

namespace H3D {
  /// Thrown when the FIBITMAP * we encapsulate has a color type we 
  /// don't support.
  H3D_VALUE_EXCEPTION( FREE_IMAGE_COLOR_TYPE, 
                       UnsupportedFreeImageColorType ); 
  /// Thrown when the FIBITMAP * we encapsulate has a image type we 
  /// don't support.
  H3D_VALUE_EXCEPTION( FREE_IMAGE_TYPE, UnsupportedFreeImageImageType ); 

  /// This Image class is an encapsulation of a FreeImage FIBITMAP *,
  /// providing all nesessary Image functions to use it in e.g. ImageTexture
  /// 
  class H3DAPI_API FreeImageImage: public Image {
  public:
    /// Constructor. 
    /// \param _bitmap The FreeImage bitmap we want to encapsulate.
    ///
    FreeImageImage( FIBITMAP *_bitmap ):
      bitmap( _bitmap ){}

    /// Returns the width of the image in pixels.
    virtual unsigned int width() {
      return FreeImage_GetWidth( bitmap );
    }

    /// Returns the height of the image in pixels.
    virtual unsigned int height() {
      return FreeImage_GetHeight( bitmap );
    }

    /// Returns the depth of the image in pixels.
    virtual unsigned int depth() {
      return 1;
    }

    /// Returns the number of bits used for each pixel in the image.
    virtual unsigned int bitsPerPixel() {
      return FreeImage_GetBPP( bitmap );
    }

    /// Returns the PixelType of the image.
    virtual PixelType pixelType() {
      FREE_IMAGE_COLOR_TYPE t = FreeImage_GetColorType( bitmap );
      switch( t ) {
      case FIC_MINISBLACK: 
      case FIC_MINISWHITE: return LUMINANCE;
#ifdef FREEIMAGE_BIGENDIAN
      case FIC_RGB: return RGB;
      case FIC_RGBALPHA: return RGBA;
#else
      case FIC_RGB: return BGR;
      case FIC_RGBALPHA: return BGRA;
#endif
      default: 
        throw UnsupportedFreeImageColorType( t, "", H3D_FULL_LOCATION );
      }
    }
        
    /// Returns the PixelComponentType of the image.
    virtual PixelComponentType pixelComponentType() {
      FREE_IMAGE_TYPE t = FreeImage_GetImageType( bitmap );
      switch( t ) {
      case FIT_BITMAP:
      case FIT_UINT16: 
      case FIT_UINT32: return UNSIGNED;
      case FIT_INT16: 
      case FIT_INT32: return SIGNED;
      case FIT_DOUBLE:
      case FIT_FLOAT: return RATIONAL;
      default: 
        throw UnsupportedFreeImageImageType( t, "", H3D_FULL_LOCATION );
      }
    }
        
    /// Returns a pointer to the raw image data. 
    virtual void *getImageData() {
      return FreeImage_GetBits( bitmap );
    }

  protected:
    FIBITMAP *bitmap;
  };

    
}

#endif // HAVE_FREEIMAGE
#endif


