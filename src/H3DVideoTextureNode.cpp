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
/// \file H3DVideoTextureNode.cpp
/// \brief CPP file for H3DVideoTextureNode.
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/H3DVideoTextureNode.h>

using namespace H3D;

H3DNodeDatabase H3DVideoTextureNode::database( 
        "H3DVideoTextureNode", 
        NULL,
        typeid( H3DVideoTextureNode ),
        &X3DTexture2DNode::database 
        );

namespace H3DVideoVideoTextureNodeInternals {
}

void H3DVideoTextureNode::render() {
  if( !decoder.get() ) return;
  // assuming 24 bit RGB image.
  unsigned int required_frame_bytes = 
    decoder->getFrameSize();

  if( required_frame_bytes == 0 ) return;

  if( frame_bytes_allocated != required_frame_bytes ) {
    unsigned char *buffer = new unsigned char[ required_frame_bytes ];
    PixelImage *pi = new PixelImage( decoder->getFrameWidth(),
                                     decoder->getFrameHeight(),
                                     1, 
                                     decoder->getFrameBitsPerPixel(),
                                     decoder->getFramePixelType(),
                                     decoder->getFramePixelComponentType(),
                                     buffer ) ;
    pi->setByteAlignment( decoder->getFrameByteAlignment() ) ;
    image->setValue( pi );

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
      glPixelStorei( GL_UNPACK_ALIGNMENT, pi->byteAlignment() );
      glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 
                       width, height, 
                       glPixelFormat( pi ),
                       glPixelComponentType( pi ),
                       image_data );
      if( free_image_data ) free( image_data );
      
      enableTexturing();
    } 
  }
}



