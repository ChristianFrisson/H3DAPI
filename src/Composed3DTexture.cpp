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
/// \file Composed3DTexture.cpp
/// \brief CPP file for Composed3DTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Composed3DTexture.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Composed3DTexture::database( 
                                       "Composed3DTexture", 
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
                           Inst< SFBool      > _interpolate,
                           Inst< MFTexture2DNode > _texture ) :
  X3DTexture3DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _repeatR, _scaleToP2, _image, _interpolate ),
  texture( _texture ) {
  type_name = "Composed3DTexture";
  database.initFields( this );

  texture->route( displayList );
}

void Composed3DTexture::glTexImage( Image *image, GLenum texture_target,
                                    bool scale_to_power_of_two ) {
  unsigned int width  = 1;
  unsigned int height = 1;
  Image::PixelType pixel_type = Image::LUMINANCE;
  unsigned int bits_per_pixel = 1;

  // find the maximum width, height and bit depth of all the textures. Also
  // find the pixel type that has most components. These will be used as
  // internal format for the 3d texture we are going to render.
  for( unsigned int i = 0; i < texture->size(); i++ ) {
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
  
  if( scale_to_power_of_two  ) {
    if( !isPowerOfTwo( width ) ) width = nextPowerOfTwo( width );
    if( !isPowerOfTwo( height ) ) height = nextPowerOfTwo( height );
  }
  
  // dummy image to use in calls to function like glInternalFormat()
  PixelImage pi( width, height, (unsigned int)texture->size(), 
                 bits_per_pixel, pixel_type, Image::UNSIGNED, 
                 NULL ); 
    
  // allocate texture memory for the entire texture. 
  glTexImage3D( texture_target, 
                0, // mipmap level
                glInternalFormat( &pi ),
                width,
                height,
                texture->size(),
                0, // border
                glPixelFormat( &pi ),
                glPixelComponentType( &pi ),
                NULL );

  // for each 2d texture, scale it to proper size and transfer it to texture
  // memory
  for( unsigned int i = 0; i < texture->size(); i++ ) {
    X3DTexture2DNode *t = texture->getValueByIndex( i );
    if(t) {
      Image *tex_image = t->image->getValue();
      if( tex_image ) {
        
        void *image_data = tex_image->getImageData();
        bool free_image_data = false;

        // if width < 4 we have to pad the data in order for it to be 4 byte 
        // aligned as required by the glTexImage2D function.
        if( tex_image->width() < 4 ) {
          image_data = padTo4ByteAlignment( image_data, 
                                            tex_image->width(),
                                            tex_image->height(), 
                                            1, 
                                            tex_image->bitsPerPixel() );
          free_image_data = true;
        }
        
        if( width != tex_image->width() ||
            height != tex_image->height() ) {
          // the image is not the same size as the final 3d texture
          // so we need to scale it
          unsigned int bytes_per_pixel = tex_image->bitsPerPixel();
          bytes_per_pixel = 
            bytes_per_pixel % 8 ? 
            bytes_per_pixel / 8 : bytes_per_pixel - 8 + 1;
      
          void * new_data = malloc( H3DMax( width, 4u )*
                                    height*bytes_per_pixel );
          gluScaleImage( glPixelFormat( tex_image ), 
                         tex_image->width(),
                         tex_image->height(),
                         glPixelComponentType( tex_image ),
                         image_data,
                         width,
                         height,
                         glPixelComponentType( tex_image ),
                         new_data );
          if( free_image_data ) {
            free( image_data );
          } else {
            free_image_data = true;
          }
          image_data = new_data;
        }

        // transfer to texture memory as a subtexture
        glTexSubImage3D( texture_target, 
                      0, // mipmap level
                      0,// xoffset
                      0,// yoffset
                      i,// zoffset
                      width,
                      height,
                      1,
                      glPixelFormat( tex_image ),
                      glPixelComponentType( tex_image ),
                      image_data );
      }
    }
  }
  /// If an RGB, BGR, RGBA or BGRA texture the texture values should not be
  /// modulated with diffuseColor according to the X3D spec. So we set the 
  /// diffuse color to 1 1 1 in order to show the texture values as they are.
  /// The alpha value should be the one from material if 3 component texture
  /// and the one from the texture if 4-component texture.
  if( pixel_type == Image::RGB ||
      pixel_type == Image::BGR ) {
    GLfloat material[] = { 1,1,1,1 };
    GLfloat current_material[4];
    glGetMaterialfv( GL_FRONT, GL_DIFFUSE, current_material );
    material[3] = current_material[3];
    glMaterialfv( GL_FRONT, GL_DIFFUSE, material );
    glGetMaterialfv( GL_BACK, GL_DIFFUSE, current_material );
    material[3] = current_material[3];
    glMaterialfv( GL_BACK, GL_DIFFUSE, material );
  } else if ( pixel_type == Image::RGBA ||
              pixel_type == Image::BGRA ) {
    GLfloat material[] = {1,1,1,1};
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material );
  }
}

void Composed3DTexture::render()     {
  
  if( displayList->hasCausedEvent( texture ) ) {
     // the image has changed so remove the old texture and install 
    // the new
    glDeleteTextures( 1, &texture_id );
    texture_id = 0;
    if( texture->size() > 0 ) {
      glGenTextures( 1, &texture_id );
      glBindTexture( GL_TEXTURE_3D, texture_id );
      
      while( glGetError() != GL_NO_ERROR )
        ;
      
      glTexImage( NULL, GL_TEXTURE_3D, scaleToPowerOfTwo->getValue() );
      
      GLenum error = glGetError();
      if( error != GL_NO_ERROR ) {
        stringstream msg;
        msg << (char*) gluErrorString( error ) << " in " << name << ends;
        throw OpenGLTextureError( (char*) gluErrorString( error ),
                                  H3D_FULL_LOCATION );
      }
      enableTexturing();
    }
  } else {
    if ( texture_id ) {
      // same texture as last loop, so we just bind it.
      glBindTexture( GL_TEXTURE_3D, texture_id );
      enableTexturing();
    }     
  }
  
  // set up texture parameters 
  if ( repeatS->getValue() )
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  else
    glTexParameteri( GL_TEXTURE_3D, 
                     GL_TEXTURE_WRAP_S, 
                     GL_CLAMP_TO_EDGE );
  if ( repeatT->getValue() )
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  else
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, 
                     GL_CLAMP_TO_EDGE );
  if ( repeatR->getValue() )
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT );
  else
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, 
                     GL_CLAMP_TO_EDGE );
   if( interpolate->getValue() ) {
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  } else {
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  }
}


