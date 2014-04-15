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
/// \file X3DTextureNode.cpp
/// \brief CPP file for X3DTextureNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTextureNode.h>
#include <H3D/X3D.h>
#include <H3D/FrameBufferTextureGenerator.h>
#include <H3D/Appearance.h>
#include <assert.h>

#include <H3DUtil/LoadImageFunctions.h>

namespace {
  /// X3D string containing frame buffer object used to save texture to file
  const std::string save_to_url_x3d= 
    "<FrameBufferTextureGenerator DEF='GENERATOR' outputTextureType='2D' generateColorTextures='RGBA' update='NONE'>"
    "  <Shape>"
    "    <Appearance DEF='APP' />"
    "    <FullscreenRectangle zValue='0.99' screenAligned='true' /> "
    "  </Shape>"
    "</FrameBufferTextureGenerator>";
}

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DTextureNode::database( "X3DTextureNode", 
                                          NULL, 
                                          typeid( X3DTextureNode ),
                                          &(X3DAppearanceChildNode::database) );

namespace X3DTextureNodeInternals {
  FIELDDB_ELEMENT( X3DTextureNode, saveToUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTextureNode, saveSuccess, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DTextureNode, saveHeight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTextureNode, saveWidth, INPUT_OUTPUT );
}

H3D_API_EXCEPTION( UnsupportedPixelComponentType );
H3D_VALUE_EXCEPTION( Image::PixelType, UnsupportedPixelType );

bool X3DTextureNode::load_images_in_separate_thread = true;
X3DTextureNode *X3DTextureNode::active_texture = NULL;

GLint X3DTextureNode::glInternalFormat( Image *i ) {
  switch( i->pixelType() ) {
  case Image::LUMINANCE: 
    if( GLEW_ARB_texture_float && 
        i->pixelComponentType() == Image::RATIONAL ) {
      if( i->bitsPerPixel() <= 16 ) return GL_LUMINANCE16F_ARB;
      else return GL_LUMINANCE32F_ARB;
    } else {
      switch( i->bitsPerPixel() ) {
      case 4:  return GL_LUMINANCE4;
      case 8:  return GL_LUMINANCE8;
      case 12: return GL_LUMINANCE12;
      case 16: return GL_LUMINANCE16;
      default: return GL_LUMINANCE;
      }
    }
  case Image::LUMINANCE_ALPHA:
    if( GLEW_ARB_texture_float && 
        i->pixelComponentType() == Image::RATIONAL ) {
      if( i->bitsPerPixel() <= 32 ) return GL_LUMINANCE_ALPHA16F_ARB;
      else return GL_LUMINANCE_ALPHA32F_ARB;
    } else {
      switch( i->bitsPerPixel() ) {
      case 8:  return GL_LUMINANCE4_ALPHA4;
      case 16: return GL_LUMINANCE8_ALPHA8;
      case 32: return GL_LUMINANCE16_ALPHA16;
      default: return GL_LUMINANCE_ALPHA;
      }
    }
  case Image::RGB:
  case Image::BGR:
    if( GLEW_ARB_texture_float && 
        i->pixelComponentType() == Image::RATIONAL ) {
      if( i->bitsPerPixel() <= 48 ) return GL_RGB16F_ARB;
      else return GL_RGB32F_ARB;
    } else {
      switch( i->bitsPerPixel() ) {
      case 12: return GL_RGB4;
      case 15: return GL_RGB5;
      case 24: return GL_RGB8;
      case 30: return GL_RGB10;
      case 36: return GL_RGB12;
      case 48: return GL_RGB16;
      default: return GL_RGB;
      }
    }
  case Image::RGBA:
  case Image::BGRA:
    if( GLEW_ARB_texture_float && 
        i->pixelComponentType() == Image::RATIONAL ) {
      if( i->bitsPerPixel() <= 64 ) return GL_RGBA16F_ARB;
      else return GL_RGBA32F_ARB;
    } else {
      switch( i->bitsPerPixel() ) {
      case 16: return GL_RGBA4;
      case 32: return GL_RGBA8;
      case 48: return GL_RGBA12;
      case 64: return GL_RGBA16;
      default: return GL_RGBA;
      }
    }
  default:
    throw UnsupportedPixelType( i->pixelType() );
  }
    
}

GLenum X3DTextureNode::glPixelFormat( Image *i ) {
  switch( i->pixelType() ) {
  case Image::LUMINANCE:       return GL_LUMINANCE;
  case Image::LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;
  case Image::RGB:             return GL_RGB;
  case Image::BGR:             return GL_BGR;
  case Image::RGBA:            return GL_RGBA;
  case Image::BGRA:            return GL_BGRA;
  default: throw UnsupportedPixelType( i->pixelType() ); 
  }
}

GLenum X3DTextureNode::glPixelComponentType( Image *i ) {
  switch( i->pixelType() ) {
  case Image::LUMINANCE: 
    switch( i->pixelComponentType() ) {
    case Image::UNSIGNED: 
      switch( i->bitsPerPixel() ) {
      case 8:  return GL_UNSIGNED_BYTE;
      case 16: return GL_UNSIGNED_SHORT;
      case 32: return GL_UNSIGNED_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit UNSIGNED LUMINANCE value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION ); 
      }
    case Image::SIGNED:
      switch( i->bitsPerPixel() ) {
      case 8:  return GL_BYTE;
      case 16: return GL_SHORT;
      case 32: return GL_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit SIGNED LUMINANCE value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION ); 
      }
    case Image::RATIONAL:
      switch( i->bitsPerPixel() ) {
      case 32: return GL_FLOAT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit RATIONAL LUMINANCE value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION ); 
      }
    }
  case Image::LUMINANCE_ALPHA:
    switch( i->pixelComponentType() ) {
    case Image::UNSIGNED: 
      switch( i->bitsPerPixel() ) {
      case 16:  return GL_UNSIGNED_BYTE;
      case 32: return GL_UNSIGNED_SHORT;
      case 64: return GL_UNSIGNED_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit UNSIGNED LUMINANCE_ALPHA value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );
      }
    case Image::SIGNED:
      switch( i->bitsPerPixel() ) {
      case 16:  return GL_BYTE;
      case 32: return GL_SHORT;
      case 64: return GL_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit SIGNED LUMINANCE_ALPHA value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );
      }
    case Image::RATIONAL:
      switch( i->bitsPerPixel() ) {
      case 64: return GL_FLOAT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit RATIONAL LUMINANCE_ALPHA value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );

      }
    }
  case Image::RGB:
  case Image::BGR:
    switch( i->pixelComponentType() ) {
    case Image::UNSIGNED: 
      switch( i->bitsPerPixel() ) {
      case 24:  return GL_UNSIGNED_BYTE;
      case 48: return GL_UNSIGNED_SHORT;
      case 96: return GL_UNSIGNED_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit UNSIGNED RGB value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );
      }
    case Image::SIGNED:
      switch( i->bitsPerPixel() ) {
      case 24:  return GL_BYTE;
      case 48: return GL_SHORT;
      case 96: return GL_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit SIGNED RGB value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );

      }
    case Image::RATIONAL:
      switch( i->bitsPerPixel() ) {
      case 96: return GL_FLOAT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit RATIONAL RGB value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );

      }
    }       

  case Image::RGBA:
  case Image::BGRA:
    switch( i->pixelComponentType() ) {
    case Image::UNSIGNED: 
      switch( i->bitsPerPixel() ) {
      case 32:  return GL_UNSIGNED_BYTE;
      case 64:  return GL_UNSIGNED_SHORT;
      case 128: return GL_UNSIGNED_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit UNSIGNED RGBA value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );
      }
    case Image::SIGNED:
      switch( i->bitsPerPixel() ) {
      case 32:  return GL_BYTE;
      case 64: return GL_SHORT;
      case 128: return GL_INT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit SIGNED RGBA value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );
      }
    case Image::RATIONAL:
      switch( i->bitsPerPixel() ) {
      case 128: return GL_FLOAT;
      default: 
        stringstream s;
        s << "Cannot find OpenGL type for "
          << i->bitsPerPixel() 
          << " bit RATIONAL RGBA value.";
        throw UnsupportedPixelComponentType( s.str(), H3D_FULL_LOCATION );
      }
    }
  default:
    throw UnsupportedPixelType( i->pixelType() );
  }

}

X3DTextureNode::X3DTextureNode( 
                               Inst< DisplayList > _displayList,
                               Inst< SFNode>  _metadata,
                               Inst< UpdateSaveToURL > _saveToUrl,
                               Inst< SFBool > _saveSuccess,
                               Inst< SFInt32 > _saveHeight,
                               Inst< SFInt32 > _saveWidth ):
  X3DAppearanceChildNode( _displayList, _metadata ),
  saveToUrl ( _saveToUrl ),
  saveSuccess ( _saveSuccess ),
  saveHeight ( _saveHeight ),
  saveWidth ( _saveWidth ) {

  type_name = "X3DTextureNode";
  database.initFields( this );

  saveSuccess->setValue ( false, id );
  saveHeight->setValue ( -1 );
  saveWidth->setValue ( -1 );
}

GLuint X3DTextureNode::renderImage( Image *image, 
                                    GLenum texture_target, 
                                    bool scale_to_power_of_two ) {
  if( image ) {
    GLuint texture_id;
    glGenTextures( 1, &texture_id );
    glBindTexture( texture_target, texture_id );

    while( glGetError() != GL_NO_ERROR )
      ;
    glTexImage( image, texture_target, scale_to_power_of_two );

    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {

      stringstream msg;

      GLint border_width;
      glGetTexLevelParameteriv( texture_target, 0, GL_TEXTURE_BORDER, &border_width );
      
      string m;
      if( border_width == 0 ) {
        m = "(expecting power of 2)";
      } else {
        m = "(must be 2^k + 2*border_width for some integer k)(border_width=1)";
      }

      H3DInt32 total_border = border_width * 2;

      // check that the dimensions of the image is a power of 2.
      bool invalid_dimensions = false;
      if( !isPowerOfTwo( image->width() - total_border ) ) {
        msg <<" Width " <<image->width() 
            << m;
        invalid_dimensions = true;
      }
      if( !isPowerOfTwo( image->height() - total_border ) ) {
        msg <<" Height " <<image->height() 
            << m;
        invalid_dimensions = true;
      }
      if( !isPowerOfTwo( image->depth() - total_border ) ) {
        msg <<" Depth " <<image->depth() 
            <<m ;
        invalid_dimensions = true;
      }
      if( invalid_dimensions ) {
        msg << " in " << name;
        throw InvalidTextureDimensions( msg.str(),
                                        H3D_FULL_LOCATION );
      }
      msg << (char*) gluErrorString( error ) << " in " << name;
      throw OpenGLTextureError( (char*) gluErrorString( error ),
                                H3D_FULL_LOCATION );
    } 

    return texture_id;
  } else { 
    return 0;
  }

}

std::pair<H3DInt32,H3DInt32> X3DTextureNode::getDefaultSaveDimensions () {
  return std::pair<H3DInt32,H3DInt32> ( 512, 512 );
}

Image* X3DTextureNode::renderToImage ( H3DInt32 _width, H3DInt32 _height ) {
  X3D::DEFNodes dn;
  AutoRef<FrameBufferTextureGenerator> fbo ( 
    static_cast<FrameBufferTextureGenerator*>(X3D::createX3DNodeFromString ( save_to_url_x3d, &dn ).get() ) );

  // Set texture save dimensions
  fbo->height->setValue ( _height );
  fbo->width->setValue  ( _width );

  Appearance* app= NULL;
  dn.getNode ( "APP", app );

  app->texture->setValue ( this );
  fbo->update->setValue ( "SPECIFIED_FRAMES_ONLY" );
  fbo->framesBeforeStop->setValue ( 10 );
  fbo->displayList->callList();

  // Check that FrameBufferTextureGenerator succeeded.
  if( fbo->isFBOInitialized() ) {
    // frame buffer size, width was set earlier in the code.
    int buffer_width = fbo->width->getValue();
    int buffer_height = fbo->height->getValue();
    int bpp = 32;

    // Create container for image data, then bind buffer and read from it.
    Image* image= new PixelImage ( buffer_width, buffer_height, 1, bpp, Image::BGRA, Image::UNSIGNED );

    // Save current FBO
    GLint previous_fbo_id;
    glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &previous_fbo_id );

    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo->getFBOId() );
    glReadPixels( 0, 0, buffer_width, buffer_height, GL_BGRA, GL_UNSIGNED_BYTE, image->getImageData() );

    // Restore previous FBO
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, previous_fbo_id );

    return image;
  }

  return NULL;
}

void X3DTextureNode::UpdateSaveToURL::onNewValue( const std::string &v ) {
  X3DTextureNode* node= static_cast<X3DTextureNode*>(getOwner());

  if ( v.empty() ) return;

#ifdef HAVE_FREEIMAGE
  // Set texture save dimensions
  std::pair<H3DInt32,H3DInt32> default_size= node->getDefaultSaveDimensions ();

  AutoRef<Image> image ( node->renderToImage (
     node->saveWidth->getValue()  == -1 ? default_size.first  : node->saveWidth->getValue(),
     node->saveHeight->getValue() == -1 ? default_size.second : node->saveHeight->getValue()) );

  if ( image.get() ) {
    node->saveSuccess->setValue ( H3DUtil::saveFreeImagePNG ( v, *image ), node->id );
  }
#else // HAVE_FREEIMAGE

  Console(4) << "Warning: Could not save texture to file! Compiled without the required FreeImage library." << endl;
  node->saveSuccess->setValue ( false, node->id );

#endif // HAVE_FREEIMAGE
}
