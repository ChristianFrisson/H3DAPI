//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file Composed3DTexture.cpp
/// \brief CPP file for Composed3DTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Composed3DTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Composed3DTexture::database( 
                                       "Composed3DTexture", 
                                       "ComposedTexture3D",
                                       &(newInstance<Composed3DTexture>), 
                                       typeid( Composed3DTexture ),
                                       &X3DTexture3DNode::database );

namespace Composed3DTextureInternals {
  FIELDDB_ELEMENT( Composed3DTexture, texture, INPUT_OUTPUT );
}


Composed3DTexture::Composed3DTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFNode      >  _metadata,
                           Inst< SFBool      >  _repeatS,
                           Inst< SFBool      >  _repeatT,
                           Inst< SFBool      >  _repeatR,
                           Inst< SFBool      >  _scaleToP2,
                           Inst< SFImage     > _image,
                           Inst< MFTexture2DNode > _texture,
                           Inst< SFTextureProperties > _textureProperties ) :
  X3DTexture3DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _repeatR, _scaleToP2, _image, _textureProperties ),
  texture( _texture ) {
  type_name = "Composed3DTexture";
  database.initFields( this );

  texture->route( image );
}

void Composed3DTexture::SFImage::update() {
  MFTexture2DNode *texture = static_cast< MFTexture2DNode * >(routes_in[0]);

  unsigned int width  = 1;
  unsigned int height = 1;
  unsigned int depth = (unsigned int)texture->size();
  Vec3f pixel_size = Vec3f(1,1,1);
  Image::PixelType pixel_type = Image::LUMINANCE;
  unsigned int bits_per_pixel = 1;

  if( texture->size() == 0 ) {
    value = NULL;
    return;
  }

  bool have_all_images = true;

  // check that all the texture nodes has images loaded. 
  for( unsigned int i = 0; i < texture->size(); ++i ) {
    X3DTexture2DNode *t = texture->getValueByIndex( i );
    if( !t || !t->image->getValue() ) {
      have_all_images = false;
      // we do not break here since we want all images to be updated
    }
  }
  
  if( !have_all_images ) {
    value = NULL;
    return;
  }

  // find the maximum width, height and bit depth of all the textures. Also
  // find the pixel type that has most components. These will be used as
  // internal format for the 3d texture we are going to render.
  for( unsigned int i = 0; i < texture->size(); ++i ) {
    X3DTexture2DNode *t = texture->getValueByIndex( i );
    if(t) {
      Image *tex_image = t->image->getValue();
      if( tex_image ) {
        switch( tex_image->pixelType() ) {
        case Image::LUMINANCE: break;
        case Image::LUMINANCE_ALPHA: {
          if( pixel_type == Image::LUMINANCE ) pixel_type = Image::LUMINANCE_ALPHA;
          break;
        }
        case Image::RGB:
        case Image::BGR: {
          if( pixel_type == Image::LUMINANCE ||
              pixel_type == Image::LUMINANCE_ALPHA ) pixel_type = Image::RGB; 
          break;
        }
        default: pixel_type = Image::RGBA;
        }
        if( tex_image->width() > width ) width = tex_image->width();
        if( tex_image->height() > height ) height = tex_image->height();
        if( tex_image->bitsPerPixel() > bits_per_pixel ) 
          bits_per_pixel = tex_image->bitsPerPixel();
      }
    }
  }

  unsigned bytes_per_pixel = 
        bits_per_pixel % 8 == 0 ? 
        bits_per_pixel / 8 : bits_per_pixel / 8 + 1;

  unsigned char *data = 
    new unsigned char[ width * height * depth * bytes_per_pixel ];

  // for each 2d texture, scale it to proper size and transfer it to texture
  // memory
  for( unsigned int i = 0; i < texture->size(); ++i ) {
    X3DTexture2DNode *t = texture->getValueByIndex( i );
    if(t) {
      Image *tex_image = t->image->getValue();
      if( tex_image ) {
        
        void *image_data = tex_image->getImageData();
        bool free_image_data = false;

        if( width != tex_image->width() ||
            height != tex_image->height() ) {
          // the image is not the same size as the final 3d texture
          // so we need to scale it
          unsigned int bytes_per_pixel = tex_image->bitsPerPixel();
          bytes_per_pixel = 
            bytes_per_pixel % 8 == 0 ? 
            bytes_per_pixel / 8 : bytes_per_pixel / 8 + 1;
      
	        unsigned int new_size = width*height*bytes_per_pixel;
          void * new_data = malloc( new_size );
          gluScaleImage( t->glPixelFormat( tex_image ), 
                         tex_image->width(),
                         tex_image->height(),
                         t->glPixelComponentType( tex_image ),
                         image_data,
                         width,
                         height,
                         t->glPixelComponentType( tex_image ),
                         new_data );
          if( free_image_data ) {
            free( image_data );
          } else {
            free_image_data = true;
          }
          image_data = new_data;
       } else {
         if(pixel_size.x == 1 ) pixel_size = tex_image->pixelSize();
       }
	     memcpy( data + width * height * i * bytes_per_pixel, 
		           image_data, 
		           width * height * bytes_per_pixel );

      }
    }
  }

  value = new PixelImage( width, height, depth, 
			                    bits_per_pixel, pixel_type, Image::UNSIGNED, 
			                    data, false, pixel_size );
}

