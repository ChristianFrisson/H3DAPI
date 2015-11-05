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
/// \file X3DTexture3DNode.cpp
/// \brief CPP file for X3DTexture3DNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTexture3DNode.h>
#include <H3DUtil/Image.h>
#include <H3D/GlobalSettings.h>

using namespace H3D;

H3DNodeDatabase X3DTexture3DNode::database( 
        "X3DTexture3DNode", 
        NULL,
        typeid( X3DTexture3DNode ),
        &H3DSingleTextureNode::database 
        );

namespace X3DTexture3DNodeInternals {
  FIELDDB_ELEMENT( X3DTexture3DNode, repeatS, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture3DNode, repeatT, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture3DNode, repeatR, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture3DNode, scaleToPowerOfTwo, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTexture3DNode, textureProperties, INPUT_OUTPUT );
}

X3DTexture3DNode::X3DTexture3DNode( 
                                   Inst< DisplayList > _displayList,
                                   Inst< SFNode  > _metadata,
                                   Inst< SFBool  > _repeatS,
                                   Inst< SFBool  > _repeatT,
                                   Inst< SFBool  > _repeatR,
                                   Inst< SFBool  > _scaleToP2,
                                   Inst< SFImage > _image,
                                   Inst< SFTextureProperties > _textureProp ) :
  H3DSingleTextureNode( _displayList, _metadata ),
  H3DImageObject( _image ),
  repeatS ( _repeatS  ),
  repeatT ( _repeatT  ),
  repeatR ( _repeatR  ),
  scaleToPowerOfTwo( _scaleToP2 ),
  textureProperties( _textureProp ),
  imageNeedsUpdate( new Field ),
  updateTextureProperties( new UpdateTextureProperties ){

  type_name = "X3DTexture3DNode";
  
  texture_unit = GL_TEXTURE0_ARB ;
  texture_target = GL_TEXTURE_3D ;

  database.initFields( this );
  image->setName( "image" );
  image->setOwner( this );

  repeatS->setValue( false );
  repeatT->setValue( false );
  repeatR->setValue( false );
  scaleToPowerOfTwo->setValue( true );

  image->route( displayList );
  repeatS->route( displayList );
  repeatT->route( displayList );
  repeatR->route( displayList );
  scaleToPowerOfTwo->route( displayList );
  textureProperties->route( displayList );

  imageNeedsUpdate->setName( "ImageNeedsUpdate" );
  imageNeedsUpdate->setOwner( this );
  image->route( imageNeedsUpdate );

  updateTextureProperties->setName( "UpdateTextureProperties" );
  updateTextureProperties->setOwner(this);
  textureProperties->route(updateTextureProperties);
}

string X3DTexture3DNode::SFImage::getValueAsString( const string& separator) {
  // Slow but convenient. is only used for JS engine

  stringstream ss;
  Image* img = getValue();
  if( img ) {
    ss << img->width() << separator << img->height() << separator << img->depth()
       << separator << (int)img->pixelType() + 1;
    for(unsigned int ix = 0; ix < img->width(); ++ix) {
      for(unsigned int iy = 0; iy < img->height(); ++iy) {
        for(unsigned int iz = 0; iz < img->depth(); ++iz) {
          RGBA color = img->getPixel( ix, iy, iz );
          unsigned char bytes_per_pixel = img->pixelType() + 1;
          unsigned char* data = new unsigned char[ bytes_per_pixel ];
          img->RGBAToImageValue(color, (void*) data);
          int intval = 0;
          for (int i = 0; i < bytes_per_pixel; ++i) {
            intval = intval << 8;
            intval = intval | data[i];
          }
          delete [] data;
          ss<< separator << intval;
        }
      }
    }
  } else {
    ss << 0 << separator << 0
     << separator << 0 << separator << 0;
  }
  return ss.str();
}

GLint X3DTexture3DNode::glInternalFormat( Image *i ) {
  TextureProperties *texture_properties = textureProperties->getValue();
  GLint tex_prop_format;
  if( texture_properties && texture_properties->glInternalFormat( i, tex_prop_format ) ) {
    return tex_prop_format;
  } else {
    return H3DSingleTextureNode::glInternalFormat( i );
  }
}

void X3DTexture3DNode::glTexImage( Image *i, GLenum _texture_target,
                                   bool scale_to_power_of_two ) {
  // the image data to render
  void *image_data = i->getImageData();
  unsigned int width = i->width();
  unsigned int height = i->height();
  unsigned int depth = i->depth();

  auto_ptr< PixelImage > pi;

  // flag to determine if the image_data pointer should be deallocated
  // at the end of the function or not.
  bool free_image_data = false;

  // Get global max texture dimension
  H3DInt32 max_dimension= -1;
  GlobalSettings*  gs = GlobalSettings::getActive();
  if( gs ) {
    GraphicsOptions* go = NULL;
    gs->getOptionNode( go );
    if ( go ) {
      max_dimension= go->maxTextureDimension->getValue();
    }
  }

  if( scale_to_power_of_two && !GLEW_ARB_texture_non_power_of_two || max_dimension > 0 ) {
    bool needs_scaling = false;
    unsigned int new_width  = i->width();
    unsigned int new_height = i->height(); 
    unsigned int new_depth = i->depth(); 

    // Enforce global maximum texture dimension
    if ( max_dimension > 0 && new_width > max_dimension ) {
      new_width= max_dimension;
      needs_scaling= true;
    }
    if ( max_dimension > 0 && new_height > max_dimension ) {
      new_height= max_dimension;
      needs_scaling= true;
    }
    if ( max_dimension > 0 && new_depth > max_dimension ) {
      new_depth= max_dimension;
      needs_scaling= true;
    }

    if( !isPowerOfTwo( new_width ) ) {
      new_width = nextPowerOfTwo( new_width );
      needs_scaling = true;
    } 
    if( !isPowerOfTwo( new_height ) ) {
      new_height = nextPowerOfTwo( new_height );
      needs_scaling = true;
    } 

    if( !isPowerOfTwo( new_depth ) ) {
      new_depth = nextPowerOfTwo( new_depth );
      needs_scaling = true;
    } 

    if( needs_scaling ) {
      pi.reset( new PixelImage( i, new_width, new_height, new_depth ) );
      width = new_width;
      height = new_height;
      depth = new_depth;
      image_data = pi->getImageData();
    }
  }

  TextureProperties *texture_properties = textureProperties->getValue();
  
  GLint byte_alignment;
  glGetIntegerv( GL_UNPACK_ALIGNMENT, &byte_alignment );
  glPixelStorei( GL_UNPACK_ALIGNMENT, i->byteAlignment() );

  if( texture_properties ) {
    glPushAttrib( GL_PIXEL_MODE_BIT );
    const Vec4f &scale = texture_properties->textureTransferScale->getValue();
    glPixelTransferf( GL_RED_SCALE, scale.x );
    glPixelTransferf( GL_BLUE_SCALE, scale.y );
    glPixelTransferf( GL_GREEN_SCALE, scale.z );
    glPixelTransferf( GL_ALPHA_SCALE, scale.w );

    const Vec4f &bias = texture_properties->textureTransferBias->getValue();
    glPixelTransferf( GL_RED_BIAS, bias.x );
    glPixelTransferf( GL_BLUE_BIAS, bias.y );
    glPixelTransferf( GL_GREEN_BIAS, bias.z );
    glPixelTransferf( GL_ALPHA_BIAS, bias.w );
  }

  if( texture_properties && texture_properties->generateMipMaps->getValue() ) {
#if( GLU_VERSION_1_3 )
    gluBuild3DMipmaps(  _texture_target, 
                        glInternalFormat( i ),
                        width,
                        height,
                        depth,
                        glPixelFormat( i ),
                        glPixelComponentType( i ),
                        image_data );
#endif
  } else {
    H3DInt32 border_width = 
      texture_properties ? 
      texture_properties->borderWidth->getValue() : 0;
    
    if( border_width < 0 || border_width > 1 ) {
      Console(LogLevel::Warning) << "Warning: Invalid borderWidth \"" << border_width 
                 << "\". Must be 0 or 1 (in " << getName()
                 << ")" << endl;
      border_width = 0;
    }
    
    // install the image as a 3d texture/
    glTexImage3D( _texture_target, 
                  0, // mipmap level
                  glInternalFormat( i ),
                  width,
                  height,
                  depth,
                  border_width, // border
                  glPixelFormat( i ),
                  glPixelComponentType( i ),
                  image_data );
  }

  glPixelStorei( GL_UNPACK_ALIGNMENT, byte_alignment );

  // restore scale and bias pixel transfer components
  if( texture_properties ) glPopAttrib();

  if( free_image_data ) 
    free( image_data );
}

void X3DTexture3DNode::render()     {
  updateTextureProperties->upToDate();
  Image * i = static_cast< Image * >(image->getValue());
  if( displayList->hasCausedEvent( image ) ) {

    if( !image->imageChanged() || texture_id == 0 ) {
      // the image has changed so remove the old texture and install 
      // the new
      glDeleteTextures( 1, &texture_id );
      texture_id = 0;
      if( i ) {
        texture_id = renderImage( i, 
                                  texture_target, 
                                  scaleToPowerOfTwo->getValue() );
      } 
    } else {
      glBindTexture(  texture_target, texture_id );
      renderSubImage( i, texture_target, 
                      image->xOffset(), 
                      image->yOffset(),
                      image->zOffset(),
                      image->changedWidth(), 
                      image->changedHeight(),
                      image->changedDepth() );
    }
    enableTexturing();
  } else {
    if ( texture_id ) {
      // same texture as last loop, so we just bind it.
      glBindTexture( texture_target, texture_id );
      enableTexturing();
    }     
  }
  
  if( i ) {
    renderTextureProperties();
  }
  imageNeedsUpdate->upToDate();
}

void X3DTexture3DNode::renderTextureProperties() {
  // If the texture is resident (see bindless textures), then we can no longer change the texture properties
  // and attempting to do so will result in an error state
  if ( getTextureHandle() != 0 ) return;

  TextureProperties *texture_properties = textureProperties->getValue();
  if( texture_properties ) {
    texture_properties->renderTextureProperties( texture_target );
  } else {
    // set up texture parameters 
    if ( repeatS->getValue() )
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT );
    else
      glTexParameteri( texture_target, 
                       GL_TEXTURE_WRAP_S, 
                       GL_CLAMP_TO_EDGE );
    if ( repeatT->getValue() )
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    else
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, 
                       GL_CLAMP_TO_EDGE );
    if ( repeatR->getValue() )
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT );
    else
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, 
                       GL_CLAMP_TO_EDGE );
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  }
}

void X3DTexture3DNode::renderSubImage( Image *_image, 
                                       GLenum _texture_target, 
                                       int x_offset, 
                                       int y_offset, 
                                       int z_offset,
                                       int width, 
                                       int height,
                                       int depth ) {
  // todo: is there a way to do this without copying data?
  unsigned char *image_data = (unsigned char *) _image->getImageData();

  unsigned int bytes_per_pixel = _image->bitsPerPixel() / 8;

  unsigned char *modified_data = 
    new unsigned char[ width * height * depth * bytes_per_pixel ]; 
  for( unsigned int z = 0; z < (unsigned int)depth; ++z ) {
    for( unsigned int y = 0; y < (unsigned int)height; ++y ) {
      memcpy( modified_data + (z * height + y ) * width * bytes_per_pixel, 
              image_data + 
              ( ((z + z_offset) * _image->height() + (y + y_offset ) ) * 
                _image->width() + x_offset) * bytes_per_pixel,
              width * bytes_per_pixel );
    }
  }

  GLint byte_alignment;
  glGetIntegerv( GL_UNPACK_ALIGNMENT, &byte_alignment );
  glPixelStorei( GL_UNPACK_ALIGNMENT, _image->byteAlignment() );

  TextureProperties *texture_properties = textureProperties->getValue();
  if( texture_properties ) {
    glPushAttrib( GL_PIXEL_MODE_BIT );
    const Vec4f &scale = texture_properties->textureTransferScale->getValue();
    glPixelTransferf( GL_RED_SCALE, scale.x );
    glPixelTransferf( GL_BLUE_SCALE, scale.y );
    glPixelTransferf( GL_GREEN_SCALE, scale.z );
    glPixelTransferf( GL_ALPHA_SCALE, scale.w );

    const Vec4f &bias = texture_properties->textureTransferBias->getValue();
    glPixelTransferf( GL_RED_BIAS, bias.x );
    glPixelTransferf( GL_BLUE_BIAS, bias.y );
    glPixelTransferf( GL_GREEN_BIAS, bias.z );
    glPixelTransferf( GL_ALPHA_BIAS, bias.w );
  }

  glTexSubImage3D( _texture_target, 0, 
                   x_offset, y_offset, z_offset, 
                   width, height, depth, 
                   glPixelFormat( _image ),
                   glPixelComponentType( _image ), 
                   modified_data );

  if( texture_properties ) glPopAttrib();

  delete [] modified_data;
}

void X3DTexture3DNode::enableTexturing() {
 // texture 2d arrays cannot be enabled, only be used with shaders.
  if( texture_target != GL_TEXTURE_2D_ARRAY_EXT&&texture_target!=GL_TEXTURE_2D_MULTISAMPLE_ARRAY ) 
    glEnable( texture_target );
  Image * i = static_cast< Image * >(image->getValue());
  if( i ) {
    Image::PixelType pixel_type = i->pixelType();
    if( pixel_type == Image::LUMINANCE_ALPHA ||
      pixel_type == Image::RGBA ||
      pixel_type == Image::BGRA ) {
        glEnable( GL_BLEND );
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
  }
}
      
/// Virtual function for making all OpenGL calls that are needed to
/// disable texturing for the texture.
///
void X3DTexture3DNode::disableTexturing() {
 // texture 2d arrays cannot be enabled, only be used with shaders.
  if( texture_target != GL_TEXTURE_2D_ARRAY_EXT ) 
    glDisable( texture_target );
  Image * i = static_cast< Image * >(image->getValue());
  if( i ) {
    Image::PixelType pixel_type = i->pixelType();
    if( pixel_type == Image::LUMINANCE_ALPHA ||
      pixel_type == Image::RGBA ||
      pixel_type == Image::BGRA ) {
        glDisable( GL_BLEND );
    }
  }
}

void X3DTexture3DNode::UpdateTextureProperties::update(){
  X3DTexture3DNode* t = static_cast<X3DTexture3DNode*>(getOwner());
  TextureProperties *texture_properties = t->textureProperties->getValue();
  if( texture_properties ) {
    string target_type = texture_properties->textureType->getValue();
    if( target_type == "2D_ARRAY" ) {t->setTextureTarget(GL_TEXTURE_2D_ARRAY_EXT);} 
    else if( target_type == "2D_RECTANGLE" ) {
      Console(LogLevel::Warning) << "Warning: Invalid textureType \"2D_RECTANGLE\" in TextureProperties for "
        << "X3DTexture2DNode. \"2D_RECTANGLE\" can only be used for 2D textures" << endl;
    } else if( target_type != "NORMAL" ) {
      Console(LogLevel::Warning) << "Warning: Invalid textureType: \"" << target_type << "\" in TextureProperties for "
        << "X3DTexture3DNode. " << endl;
    }
  }
}

GLuint64 X3DTexture3DNode::getTextureHandle() {
  // When the image is used as a bindless texture in a shader
  // then it is possible that the image is never rendered. So we take
  // this opportunity to ensure that the image is upToDate since otherwise
  // changes to the image will not be handled.
  image->upToDate();
  return H3DSingleTextureNode::getTextureHandle();
}

std::pair<H3DInt32,H3DInt32> X3DTexture3DNode::getDefaultSaveDimensions () {
  Image* im= image->getValue();
  if ( im ) {
    return std::pair<H3DInt32,H3DInt32> ( im->width(), im->height() );
  } else {
    return X3DTextureNode::getDefaultSaveDimensions();
  }
}

GLenum X3DTexture3DNode::glPixelFormat( Image *i ) {
  TextureProperties *texture_properties = textureProperties->getValue();
  GLenum tex_prop_format;
  if( texture_properties && texture_properties->glPixelFormat( i, tex_prop_format ) ) {
    return tex_prop_format;
  } else {
    return H3DSingleTextureNode::glPixelFormat( i );
  }
}
