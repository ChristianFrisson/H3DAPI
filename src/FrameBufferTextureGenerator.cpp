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
/// \file FrameBufferTextureGenerator.cpp
/// \brief CPP file for FrameBufferTextureGenerator.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FrameBufferTextureGenerator.h>

#include <H3D/GeneratedTexture3D.h>

using namespace H3D;

H3DNodeDatabase FrameBufferTextureGenerator::database( 
        "FrameBufferTextureGenerator", 
        &newInstance< FrameBufferTextureGenerator >,
        typeid( FrameBufferTextureGenerator ),
        &X3DGroupingNode::database 
        );

namespace FrameBufferTextureGeneratorInternals {
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, generateColorTextures, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, generateDepthTexture, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, outputTextureType, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, depthTexture, OUTPUT_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, colorTextures, OUTPUT_ONLY );
}

FrameBufferTextureGenerator::~FrameBufferTextureGenerator() {
  if( fbo_initialized ) {
    glDeleteFramebuffersEXT( 1, &fbo_id );
    
     if( !generateDepthTexture->getValue() ) {
       glDeleteBuffers(1, &depth_id);      
     }
    
     glDeleteBuffers(1, &stencil_id);   
  }
}

FrameBufferTextureGenerator::FrameBufferTextureGenerator( Inst< AddChildren    > _addChildren,
                                                          Inst< RemoveChildren > _removeChildren,
                                                          Inst< MFChild        > _children,
                                                          Inst< SFNode         > _metadata,
                                                          Inst< SFBound        > _bound,
                                                          Inst< SFVec3f        > _bboxCenter,
                                                          Inst< SFVec3f        > _bboxSize,
                                                          Inst< MFString       > _generateColorTextures,
                                                          Inst< SFBool         > _generateDepthTexture,
							  Inst< MFGeneratedTextureNode > _colorTextures, 
							  Inst< SFGeneratedTextureNode > _depthTexture,
							  Inst< SFString         > _outputTextureType  ):
  X3DGroupingNode( _addChildren, _removeChildren, _children, _metadata, _bound, _bboxCenter, _bboxSize ),
  generateColorTextures( _generateColorTextures ),
  generateDepthTexture( _generateDepthTexture ),
  colorTextures( _colorTextures ),
  depthTexture( _depthTexture ),
  outputTextureType( _outputTextureType ),
  fbo_initialized( false ),
  buffers_width(-1),
  buffers_height(-1),
  buffers_depth( -1 ),
  last_resize_success( true ) {

  type_name = "FrameBufferTextureGenerator";
  database.initFields( this );

  generateDepthTexture->setValue( false );
  outputTextureType->setValue( "2D" );
  
  // turn off display list since we want to get new values of the width
  // and height each loop to see if they have changed.
  displayList->setCacheMode( H3DDisplayListObject::DisplayList::OFF );
}

void FrameBufferTextureGenerator::render()     { 
  if( !GLEW_EXT_framebuffer_object ) {
    Console(4) << "Warning: Frame Buffer Objects not supported by your graphics card "
	       << "(EXT_frame_buffer_object). FrameBufferTextureGenerator ndoes will "
	       << "not work." << endl;
  }

  string output_texture_type = outputTextureType->getValue();
  if( output_texture_type != "3D" && output_texture_type != "2D_ARRAY" ) {
    output_texture_type = "2D";
  }

  /// Check if we need to generate any textures.
  if( generateColorTextures->size() == 0 && (!generateDepthTexture->getValue() || output_texture_type == "3D" )  )
    return;

  /// Save current state.
  glPushAttrib( GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);

  /// Make sure all textures and buffers are initialized.
  if( !fbo_initialized ) initializeFBO();
  
  // get the width and height of the buffer in pixels.
  GLint viewport[4];
  glGetIntegerv( GL_VIEWPORT, viewport );
  unsigned int width  = viewport[2];
  unsigned int height = viewport[3];
  unsigned int depth  = output_texture_type == "2D" ? 1: std::max( (int)children->size(), 1 ); 

  // ensure that all buffers are always of the frame buffer size
  if( buffers_width != width || buffers_height != height || buffers_depth != depth ) {
    last_resize_success = resizeBuffers( width, height, depth );
  }
  
  // Don't do anything if buffer resize had an error.
  if( !last_resize_success ) {
    glPopAttrib();
    return;
  }

  // set up fbo
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

  // set up to render to all color buffers
  if( GLEW_ARB_draw_buffers ) {
    glDrawBuffers( H3DMax( (int)color_ids.size(), 1 ), draw_buffers.get() );
  } else {
    Console(4) << "Warning: Your graphics card does not support multiple "
	       << "render targets(ARB_draw_buffers). Only one color texture will"
	       << " have update to their values";
  }

  if( output_texture_type == "2D" ) {
    // 2D textures. Render all nodes in children field into the textures.

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // render scene.
    X3DGroupingNode::render();
  } else {
    // 3D texture. Each child in the children fields is rendered into a different
    // slice in the 3D texture.

    const NodeVector &c = children->getValue();
    
    for( unsigned int i = 0; i < c.size(); i++ ) {
      // set the render target to the correct slice for depth texture.
      if( output_texture_type == "2D_ARRAY" && generateDepthTexture->getValue() ) {
	glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, depth_id, 0, i );
      }

      // set the render target to the correct slice for color textures.
      for( unsigned int j = 0; j < color_ids.size(); j++ ) {
	glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + j,
				     color_ids[j], 0, i );   
      }

      // Clear buffers.
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      
      // render child
      if( c[i] ) {
	H3DDisplayListObject *tmp = dynamic_cast< H3DDisplayListObject* >( c[i]);
	if( tmp )
	  tmp->displayList->callList();
	else
	  c[i]->render();
      }
    }
  }
  
  // reset fbo
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  // reset previous state.
  glPopAttrib();

};

void FrameBufferTextureGenerator::initializeFBO() {
  if( !fbo_initialized ) {
     glGenFramebuffersEXT(1, &fbo_id); 

     bool generate_2d = true;
     const string &output_texture_type = outputTextureType->getValue();
    if( output_texture_type == "3D" || output_texture_type == "2D_ARRAY" ) {
      generate_2d = false;
    } else {
      if( output_texture_type != "2D" ) {
	Console(4) << "Warning: Invalid outputTextureType value: \"" << output_texture_type 
                      << "\". Valid values are \"2D\", \"2D_ARRAY\" and \"3D\". Using 2D instead(in FrameBufferTextureGenerator node). " << endl;
      }
    }

     // If we are to generate depth texture use that id otherwise
     // create a new render buffer.
     if( generateDepthTexture->getValue() ) {
       if( generate_2d ) {
	 GeneratedTexture *tex = new GeneratedTexture;
	 // make sure the texture id is initialized.
	 tex->ensureInitialized( GL_TEXTURE_2D );
	 depthTexture->setValue( tex, id );
	 depth_id = tex->getTextureId();
       } else {
	 if( output_texture_type == "2D_ARRAY" ) {
	   GeneratedTexture3D *tex = new GeneratedTexture3D;
	   // make sure the texture id is initialized.
	   tex->ensureInitialized( GL_TEXTURE_2D_ARRAY_EXT );
	   depthTexture->setValue( tex, id );
	   depth_id = tex->getTextureId();
	 } else if( output_texture_type == "3D" ) {
	   Console(4) << "Warning: 3D depth textures cannot be generated by FrameBufferTextureGenerator. OpenGL does not support it. Depth texture will be undefined" << endl;
	 }
       }
     } else {
       glGenBuffers(1, &depth_id);      
     }

     glGenBuffers(1, &stencil_id);   

     GLint max_draw_buffers, max_color_attachments;
     glGetIntegerv( GL_MAX_DRAW_BUFFERS, &max_draw_buffers );
     glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments );

     const vector< string > &color_texture_types = generateColorTextures->getValue();

     unsigned int nr_color_textures = color_texture_types.size();
     if( nr_color_textures > max_draw_buffers ||
	 nr_color_textures > max_color_attachments ) {
       nr_color_textures = H3DMin( max_draw_buffers, max_draw_buffers );
       Console(4) << "Warning: Too many color textures. Supported by your graphics card: "
		  << nr_color_textures << ". Tried to use: " << color_texture_types.size() 
		  << ". Additional textures will be ignored(in FrameBufferTextureGenerator). "
		  << endl;
     }

     // generate glDrawBuffers input array
     if( nr_color_textures == 0 ) {
       draw_buffers.reset( (GLenum *)malloc( sizeof( GLenum ) ) );
       draw_buffers.get()[0] = GL_NONE;
     } else {
       draw_buffers.reset( (GLenum *)malloc( color_texture_types.size() * sizeof( GLenum ) ) ); 
     }

     // initialize color textures.
     for( size_t i = 0; i < nr_color_textures; i++ ) {
       if( generate_2d ) {
	GeneratedTexture *tex = new GeneratedTexture;
	// make sure the texture id is initialized.
	tex->ensureInitialized( GL_TEXTURE_2D );
	colorTextures->push_back( tex, id );
        color_ids.push_back( tex->getTextureId() );
       } else {
	 GeneratedTexture3D *tex = new GeneratedTexture3D;
	 // make sure the texture id is initialized.
	 if( output_texture_type == "2D_ARRAY" ) {
	   tex->ensureInitialized( GL_TEXTURE_2D_ARRAY_EXT );
	 } else {
	   tex->ensureInitialized( GL_TEXTURE_3D );
	 }
	 colorTextures->push_back( tex, id );
	 color_ids.push_back( tex->getTextureId() );
       }
       draw_buffers.get()[i] = GL_COLOR_ATTACHMENT0_EXT+i;
     }

     fbo_initialized = true;
  }
}

bool FrameBufferTextureGenerator::resizeBuffers( H3DInt32 width, H3DInt32 height, H3DInt32 depth ) {

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);
    
    string output_texture_type = outputTextureType->getValue();

    GLenum texture_type = GL_TEXTURE_2D;
    if( output_texture_type == "3D" ) {
      texture_type = GL_TEXTURE_3D;
    } else if( output_texture_type == "2D_ARRAY" ) {
      texture_type = GL_TEXTURE_2D_ARRAY_EXT;
    }


    // set up depth buffer
    if( generateDepthTexture->getValue()  ) {
      if( texture_type != GL_TEXTURE_3D ) {
	glBindTexture( texture_type, depth_id );

	// filter needs to be something else than GL_MIPMAP_LINEAR that is default
	// since that is not supported by FBO.
	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	
	if(texture_type == GL_TEXTURE_2D ) {
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, 
		       GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
				    GL_TEXTURE_2D, depth_id, 0 );
	} else if( texture_type == GL_TEXTURE_2D_ARRAY_EXT ) {
	  glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, GL_DEPTH_COMPONENT24, width, height, depth, 0, 
		       GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	  glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, depth_id, 0, 0 );
	}
      }
    } else {
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_id);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24,
			       width, height); 
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
				   GL_RENDERBUFFER_EXT, depth_id);
    }
    
    // set up stencil buffer
    /*glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencil_id);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX8_EXT,
			     width, height); 
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
				 GL_RENDERBUFFER_EXT, stencil_id);
    */

    // set up color buffers
     const vector< string > &color_texture_types = generateColorTextures->getValue();
     for( size_t i = 0; i < color_texture_types.size(); i++ ) {
       glBindTexture( texture_type, color_ids[i] );
       // filter needs to be something else than GL_MIPMAP_LINEAR that is default
       // since that is not supported by FBO.
       glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

       GLint internal_format = GL_RGBA;

       if( color_texture_types[i] == "RGBA32F" ) { 
	 if( GLEW_ARB_texture_float ) {
	   internal_format = GL_RGBA32F;
	 } else {
	   Console(4) << "Warning: Your graphics card does not support floating point textures (ARB_texture_float). Using RGBA instead(in FrameBufferTextureGenerator node). " << endl;
	 }
       } else {
         if( color_texture_types[i] != "RGBA" ) {
           Console(4) << "Warning: Invalid generateColorTextures value: \"" << color_texture_types[i] 
                      << "\". Valid values are \"RGBA\" and \"RGBA32F\". Using RGBA instead(in FrameBufferTextureGenerator node). " << endl;
         }
       }

       if( texture_type == GL_TEXTURE_2D ) {
	 glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, 
		      GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      
	 glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i,
				   GL_TEXTURE_2D, color_ids[i], 0 );        
       } else if( texture_type == GL_TEXTURE_2D_ARRAY_EXT ) {
	 glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, internal_format, width, height, depth, 0, 
		      GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	 glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i,
				      color_ids[i], 0,0 );        
       } else if( texture_type == GL_TEXTURE_3D ) {
	 glTexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, 
		      GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	 glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i,
				   GL_TEXTURE_3D, color_ids[i], 0, 0 );        
       }
     }

     // No color textures, disable drawing/reading from color buffers.
     if( color_ids.size() == 0 ) {
       glDrawBuffer( GL_NONE );
       glReadBuffer( GL_NONE );
     }

     // check for errors
     GLenum fbo_err = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
     if( fbo_err != GL_FRAMEBUFFER_COMPLETE_EXT ) {
       Console(4) << "Warning: Frame Buffer Object error: ";
       switch(fbo_err) {
       case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT :
	 Console(4) << "Attachment not complete" << endl;
	 break;
       case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT :
	 Console(4) << "Wrong size of attachments" << endl;
	 break;
       case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT :
	 Console(4) << "Draw buffer err" << endl;
	 break;
       case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT :
	 Console(4) << "Color attachments have different formats" << endl;
	 break;
       case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT :
	 Console(4) << "No attachments" << endl;
	 break;
       case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	 Console(4) << "Unsupported" << endl;
	 break;
       default:
	 Console(4) << "Unkown error" << endl;
	 break;
       }
       Console(4) << " (in FrameBufferTextureGenerator node). " << endl;
       return false;
     }
     buffers_width = width;
     buffers_height = height;
     buffers_depth = depth;
     return true;
}


