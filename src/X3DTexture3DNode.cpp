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
/// \file X3DTexture3DNode.cpp
/// \brief CPP file for X3DTexture3DNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTexture3DNode.h>

using namespace H3D;

H3DNodeDatabase X3DTexture3DNode::database( 
        "X3DTexture3DNode", 
        NULL,
        typeid( X3DTexture3DNode ),
        &X3DTextureNode::database 
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
  X3DTextureNode( _displayList, _metadata ),
  H3DImageObject( _image ),
  repeatS ( _repeatS  ),
  repeatT ( _repeatT  ),
  repeatR ( _repeatR  ),
  scaleToPowerOfTwo( _scaleToP2 ),
  textureProperties( _textureProp ),
  texture_id( 0 ) {

  type_name = "X3DTexture3DNode";

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
}


GLint X3DTexture3DNode::glInternalFormat( Image *i ) {
  TextureProperties *texture_properties = textureProperties->getValue();
  if( GLEW_ARB_texture_compression && 
      texture_properties &&
      texture_properties->textureCompression->getValue() != "DEFAULT" ) {
    switch( i->pixelType() ) {
    case Image::LUMINANCE: 
      return GL_COMPRESSED_LUMINANCE_ARB;
    case Image::LUMINANCE_ALPHA: 
      return GL_COMPRESSED_LUMINANCE_ALPHA_ARB;
    case Image::RGB:
    case Image::BGR:
      return GL_COMPRESSED_RGB_ARB;
    case Image::RGBA:
    case Image::BGRA:
      return GL_COMPRESSED_RGBA_ARB;
    default:
      return GL_RGBA;
    }
  } else {
    return X3DTextureNode::glInternalFormat( i );
  }
}

void X3DTexture3DNode::glTexImage( Image *i, GLenum texture_target,
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

  if( scale_to_power_of_two && !GLEW_ARB_texture_non_power_of_two ) {
    bool needs_scaling = false;
    unsigned int new_width  = i->width();
    unsigned int new_height = i->height(); 
    unsigned int new_depth = i->depth(); 

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
  if( texture_properties && texture_properties->generateMipMaps->getValue() ) {
#if( GLU_VERSION_1_3 )
    gluBuild3DMipmaps(  texture_target, 
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
      Console(3) << "Warning: Invalid borderWidth \"" << border_width 
                 << "\". Must be 0 or 1 (in " << getName()
                 << ")" << endl;
      border_width = 0;
    }
    
    // install the image as a 3d texture/
    glTexImage3D( texture_target, 
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

  if( free_image_data ) 
    free( image_data );
}

void X3DTexture3DNode::render()     {
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &texture_unit );
  Image * i = static_cast< Image * >(image->getValue());
  if( displayList->hasCausedEvent( image ) ) {
    // the image has changed so remove the old texture and install 
    // the new
    glDeleteTextures( 1, &texture_id );
    texture_id = 0;
    if( i ) {
      texture_id = renderImage( i, 
                                GL_TEXTURE_3D, 
                                scaleToPowerOfTwo->getValue() );
      enableTexturing();
    }
  } else {
    if ( texture_id ) {
      // same texture as last loop, so we just bind it.
      glBindTexture( GL_TEXTURE_3D, texture_id );
      enableTexturing();
    }     
  }
  
  if( i ) {
    renderTextureProperties();
  }
}

void X3DTexture3DNode::renderTextureProperties() {
  TextureProperties *texture_properties = textureProperties->getValue();
  if( texture_properties ) {
    // anisotropicDegree
    H3DFloat anisotropic = texture_properties->anisotropicDegree->getValue();
    if( anisotropic < 1 ) {
      Console(3) << "Warning: Invalid anisotropicDegree \"" << anisotropic 
                 << "\". Must be greater that 1.0 (in " << getName()
                 << ")" << endl;
        
    } else {
      if( GLEW_EXT_texture_filter_anisotropic ) {
        glTexParameterf(GL_TEXTURE_3D, 
                        GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic );
      }
    }
    // border color
    const RGBA &border_color = texture_properties->borderColor->getValue();
    GLfloat c[4];
    c[0] = border_color.r;
    c[1] = border_color.g;
    c[2] = border_color.b;
    c[3] = border_color.a;
    glTexParameterfv( GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, c );

    // boundary modes

    // S
    const string &s_mode = texture_properties->boundaryModeS->getValue();
    if( s_mode == "CLAMP" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    } else if( s_mode == "CLAMP_TO_EDGE" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    } else if( s_mode == "CLAMP_TO_BOUNDARY" ) {
      if( GLEW_ARB_texture_border_clamp ) {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, 
                         GL_CLAMP_TO_BORDER );
      } else {
        Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                   << "supported by your graphics card (in " << getName()
                   << ")" << endl;
      }
    } else if( s_mode == "MIRRORED_REPEAT" ) {
      if( GLEW_ARB_texture_mirrored_repeat ) {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, 
                         GL_MIRRORED_REPEAT_ARB );
      } else {
        Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                   << "supported by your graphics card (in " << getName()
                   << ")" << endl;
      }
    } else if( s_mode == "REPEAT" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    } else {
      Console(3) << "Warning: Invalid boundary mode \"" << s_mode 
                 << "\" in TextureProperties "
                 << " node for texture node(" << getName() << ")." << endl; 
    }

    // T
    const string &t_mode = texture_properties->boundaryModeT->getValue();
    if( s_mode == "CLAMP" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    } else if( s_mode == "CLAMP_TO_EDGE" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    } else if( s_mode == "CLAMP_TO_BOUNDARY" ) {
      if( GLEW_ARB_texture_border_clamp ) {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, 
                         GL_CLAMP_TO_BORDER );
      } else {
        Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                   << "supported by your graphics card (in " << getName()
                   << ")" << endl;
      }
    } else if( s_mode == "MIRRORED_REPEAT" ) {
      if( GLEW_ARB_texture_mirrored_repeat ) {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, 
                         GL_MIRRORED_REPEAT_ARB );
      } else {
        Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                   << "supported by your graphics card (in " << getName()
                   << ")" << endl;
      }
    } else if( s_mode == "REPEAT" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    } else {
      Console(3) << "Warning: Invalid boundary mode \"" << s_mode 
                 << "\" in TextureProperties "
                 << " node for texture node(" << getName() << ")." << endl; 
    }


    // R
    const string &r_mode = texture_properties->boundaryModeR->getValue();
    if( s_mode == "CLAMP" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );
    } else if( s_mode == "CLAMP_TO_EDGE" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    } else if( s_mode == "CLAMP_TO_BOUNDARY" ) {
      if( GLEW_ARB_texture_border_clamp ) {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, 
                         GL_CLAMP_TO_BORDER );
      } else {
        Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                   << "supported by your graphics card (in " << getName()
                   << ")" << endl;
      }
    } else if( s_mode == "MIRRORED_REPEAT" ) {
      if( GLEW_ARB_texture_mirrored_repeat ) {
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, 
                         GL_MIRRORED_REPEAT_ARB );
      } else {
        Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                   << "supported by your graphics card (in " << getName()
                   << ")" << endl;
      }
    } else if( s_mode == "REPEAT" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT );
    } else {
      Console(3) << "Warning: Invalid boundary mode \"" << s_mode 
                 << "\" in TextureProperties "
                 << " node for texture node(" << getName() << ")." << endl; 
    }

    // magnification filter
    const string &mag_filter = 
      texture_properties->magnificationFilter->getValue();

    if( mag_filter == "NEAREST_PIXEL" ||
        mag_filter == "FASTEST" )
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    else if( mag_filter == "AVG_PIXEL" ||
             mag_filter == "DEFAULT" ||
             mag_filter == "NICEST" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    } else  {
      Console(3) << "Warning: Invalid magnification filter \"" << mag_filter 
                 << "\" in TextureProperties "
                 << " node for texture node(" << getName() << ")." << endl; 
    }

    // minification filter
    const string &min_filter = 
      texture_properties->minificationFilter->getValue();

    if( min_filter == "NEAREST_PIXEL" ||
        min_filter == "FASTEST" )
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    else if( min_filter == "AVG_PIXEL" ||
             min_filter == "DEFAULT" ||
             min_filter == "NICEST" ) {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    } else if( min_filter == "AVG_PIXEL_AVG_MIPMAP") {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, 
                       GL_LINEAR_MIPMAP_LINEAR );
    } else if( min_filter == "AVG_PIXEL_NEAREST_MIPMAP") {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, 
                       GL_LINEAR_MIPMAP_NEAREST );
    } else if( min_filter == "NEAREST_PIXEL_AVG_MIPMAP") {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, 
                       GL_NEAREST_MIPMAP_LINEAR );
    } else if( min_filter == "NEAREST_PIXEL_NEAREST_MIPMAP") {
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, 
                       GL_NEAREST_MIPMAP_NEAREST );
    } else {
      Console(3) << "Warning: Invalid minification filter \"" << min_filter 
                 << "\" in TextureProperties "
                 << " node for texture node(" << getName() << ")." << endl; 
    }

    // priority
    H3DFloat priority = texture_properties->texturePriority->getValue();
    if( priority < 0 || priority > 1 ) {
      Console(3) << "Warning: Invalid texturePriority \"" << anisotropic 
                 << "\". Must be in range [0, 1] (in " << getName()
                 << ")" << endl;
        
    } else {
      glTexParameterf(GL_TEXTURE_3D, 
                      GL_TEXTURE_PRIORITY, priority );
    }

    if( GLEW_ARB_texture_compression ) {
      // compression
      const string &compression = 
        texture_properties->textureCompression->getValue();

      if( compression == "DEFAULT" ) {
        glHint( GL_TEXTURE_COMPRESSION_HINT_ARB, GL_DONT_CARE );
      } else if( compression == "FASTEST" ||
                 compression == "HIGH" ||
                 compression == "MEDIUM" ) {
        glHint( GL_TEXTURE_COMPRESSION_HINT_ARB, GL_FASTEST );
      } else if( compression == "NICEST" ||
                 compression == "LOW"  ) {
        glHint( GL_TEXTURE_COMPRESSION_HINT_ARB, GL_NICEST );
      } else {
        Console(3) << "Warning: Invalid textureCompression mode \"" 
                   << compression  << "\" in TextureProperties "
                   << " node for texture node(" << getName() << ")." << endl; 
      }
    }
  } else {
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
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  }
}


