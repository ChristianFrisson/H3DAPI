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
/// \file X3DTexture2DNode.cpp
/// \brief CPP file for X3DTexture2DNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DTexture2DNode.h"

using namespace H3D;

H3DNodeDatabase X3DTexture2DNode::database( 
        "X3DTexture2DNode", 
        NULL,
        typeid( X3DTexture2DNode ),
        &X3DTextureNode::database 
        );

namespace X3DTexture2DNodeInternals {
  FIELDDB_ELEMENT( X3DTexture2DNode, repeatS, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture2DNode, repeatT, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture2DNode, scaleToPowerOfTwo, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture2DNode, interpolate, INPUT_OUTPUT );
}

X3DTexture2DNode::X3DTexture2DNode( 
                                   Inst< DisplayList > _displayList,
                                   Inst< SFNode  > _metadata,
                                   Inst< SFBool  > _repeatS,
                                   Inst< SFBool  > _repeatT,
                                   Inst< SFBool  > _scaleToP2,
                                   Inst< SFImage > _image,
                                   Inst< SFBool  > _interpolate ) :
  X3DTextureNode( _displayList, _metadata ),
  H3DImageObject( _image ),
  repeatS ( _repeatS  ),
  repeatT ( _repeatT  ),
  scaleToPowerOfTwo( _scaleToP2 ),
  interpolate( _interpolate ),
  texture_id( 0 ) {

  type_name = "X3DTexture2DNode";

  database.initFields( this );
  image->setName( "image" );
  image->setOwner( this );

  repeatS->setValue( true );
  repeatT->setValue( true );
  scaleToPowerOfTwo->setValue( true );
  interpolate->setValue( true );

  image->route( displayList );
  repeatS->route( displayList );
  repeatT->route( displayList );
  scaleToPowerOfTwo->route( displayList );
  interpolate->route( displayList );
}

void X3DTexture2DNode::glTexImage( Image *i, GLenum texture_target,
                                   bool scale_to_power_of_two ) {
  // the image data to render
  void *image_data = i->getImageData();
  // the width of the data of the image_data pointer
  unsigned int width = i->width();
  // the height of the data of the image_data pointer
  unsigned int height = i->height();
  // flag to determine if the image_data pointer should be deallocated
  // at the end of the function or not.
  bool free_image_data = false;

  // if width < 4 we have to pad the data in order for it to be 4 byte 
  // aligned as required by the glTexImage2D function.
  if( width < 4 ) {
    image_data = padTo4ByteAlignment( image_data, width, height, 1,
                                      i->bitsPerPixel() );
    free_image_data = true;
  }

  if( scale_to_power_of_two && !GLEW_ARB_texture_non_power_of_two ) {
    // check if any scaling is required and if so scale the image.
    bool needs_scaling = false;
    unsigned int new_width  = i->width();
    unsigned int new_height = i->height(); 

    if( !isPowerOfTwo( new_width ) ) {
      new_width = nextPowerOfTwo( new_width );
      needs_scaling = true;
    } 

    if( !isPowerOfTwo( new_height ) ) {
      new_height = nextPowerOfTwo( new_height );
      needs_scaling = true;
    } 
    if( needs_scaling ) {
      unsigned int bytes_per_pixel = i->bitsPerPixel();
      bytes_per_pixel = 
        bytes_per_pixel % 8 ? 
        bytes_per_pixel / 8 : bytes_per_pixel - 8 + 1;
      
      void * new_data = malloc( H3DMax( new_width, 4u )*
                                new_height*bytes_per_pixel );
      gluScaleImage( glPixelFormat( i ), 
                     width,
                     height,
                     glPixelComponentType( i ),
                     image_data,
                     new_width,
                     new_height,
                     glPixelComponentType( i ),
                     new_data );
      width = new_width;
      height = new_height;

      // remove the old image data if the image was padded before.
      if( free_image_data ) {
        free( image_data );
      } else {
        free_image_data = true;
      }
      image_data = new_data;
    }
  }

  // install the image as a 2d texture/
  glTexImage2D( texture_target, 
                0, // mipmap level
                glInternalFormat( i ),
                width,
                height,
                0, // border
                glPixelFormat( i ),
                glPixelComponentType( i ),
                image_data );

  // free the temporary image data if necessary.
  if( free_image_data ) 
    free( image_data );
}

void X3DTexture2DNode::render()     {
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &texture_unit );
  Image * i = static_cast< Image * >(image->getValue());
  if( displayList->hasCausedEvent( image ) ) {
     // the image has changed so remove the old texture and install 
    // the new
    glDeleteTextures( 1, &texture_id );
    texture_id = 0;
    if( i ) {
      texture_id = renderImage( i, 
                                GL_TEXTURE_2D, 
                                scaleToPowerOfTwo->getValue() );
      enableTexturing();
    }
  } else {
    if ( texture_id ) {
      // same texture as last loop, so we just bind it.
      glBindTexture(  GL_TEXTURE_2D, texture_id );
      enableTexturing();
    }     
  }


  if( i ) {
    // set up texture parameters 
    if ( repeatS->getValue() )
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    else
      glTexParameteri( GL_TEXTURE_2D, 
                       GL_TEXTURE_WRAP_S, 
                       GL_CLAMP_TO_EDGE );
    if ( repeatT->getValue() )
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    else
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
                       GL_CLAMP_TO_EDGE );
    if( interpolate->getValue() ) {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      
    } else {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }
  }
};

