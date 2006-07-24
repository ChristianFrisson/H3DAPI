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
/// \file H3DVideoTextureNode.cpp
/// \brief CPP file for H3DVideoTextureNode.
///
//
//////////////////////////////////////////////////////////////////////////////


#include "H3DVideoTextureNode.h"

using namespace H3D;

H3DNodeDatabase H3DVideoTextureNode::database( 
        "H3DVideoTextureNode", 
        NULL,
        typeid( H3DVideoTextureNode ),
        &X3DTexture2DNode::database 
        );

namespace H3DVideoVideoTextureNodeInternals {
  FIELDDB_ELEMENT( H3DVideoTextureNode, 
                   scaleFrameToFillTexture, INPUT_OUTPUT );
}

void H3DVideoTextureNode::render() {
  if( !decoder.get() ) return;
  // assuming 24 bit RGB image.
  unsigned int required_frame_bytes = 
    decoder->getFrameWidth() * decoder->getFrameHeight() * 3;

  if( frame_bytes_allocated != required_frame_bytes ) {
    unsigned char *buffer = new unsigned char[ required_frame_bytes ];
    image->setValue( new PixelImage( decoder->getFrameWidth(),
                                     decoder->getFrameHeight(),
                                     1, 
                                     24,
                                     PixelImage::RGB,
                                     PixelImage::UNSIGNED,
                                     buffer ) );
    frame_bytes_allocated = required_frame_bytes;
    X3DTexture2DNode::render();
  } else {
    PixelImage *pi = dynamic_cast< PixelImage * >( image->getValue() );
    glBindTexture( GL_TEXTURE_2D, texture_id );
    enableTexturing();
    if( decoder->haveNewFrame() ) {
      decoder->getNewFrame( (unsigned char *)pi->getImageData() );
      pi->setWidth( decoder->getFrameWidth() );
      pi->setHeight( decoder->getFrameHeight() );

      unsigned int width = pi->width();
      unsigned int height = pi->height();
      bool free_image_data = false;
      void *image_data = pi->getImageData();
      
      if( !GLEW_ARB_texture_non_power_of_two && 
          scaleToPowerOfTwo->getValue() && 
          scaleFrameToFillTexture->getValue() ) {
        // check if any scaling is required and if so scale the image.
        bool needs_scaling = false;
        unsigned int new_width  = pi->width();
        unsigned int new_height = pi->height(); 
        
        if( !isPowerOfTwo( new_width ) ) {
        new_width = nextPowerOfTwo( new_width );
        needs_scaling = true;
        } 
        
        if( !isPowerOfTwo( new_height ) ) {
        new_height = nextPowerOfTwo( new_height );
        needs_scaling = true;
        } 
        
        if( needs_scaling ) {
          unsigned int bytes_per_pixel = pi->bitsPerPixel();
          bytes_per_pixel = 
            bytes_per_pixel % 8 ? 
            bytes_per_pixel / 8 : bytes_per_pixel - 8 + 1;
          
          void * new_data = malloc( H3DMax( new_width, 4u )*
                                    new_height*bytes_per_pixel );
          gluScaleImage( glPixelFormat( pi ), 
                         width,
                         height,
                         glPixelComponentType( pi ),
                         image_data,
                         new_width,
                         new_height,
                         glPixelComponentType( pi ),
                         new_data );
          width = new_width;
          height = new_height;
          
          free_image_data = true;
          image_data = new_data;
        }
      }
      glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 
                       width, height, 
                       GL_RGB,
                       GL_UNSIGNED_BYTE,
                       image_data );
      if( free_image_data ) free( image_data );
      
      enableTexturing();
    } 
  }
}



