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
/// \file FrameBufferTextureGenerator.cpp
/// \brief CPP file for FrameBufferTextureGenerator.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FrameBufferTextureGenerator.h>
#include <H3D/GeneratedTexture3D.h>
#include <H3D/X3DShapeNode.h>

using namespace H3D;

std::set< FrameBufferTextureGenerator*> FrameBufferTextureGenerator::fbo_nodes;

H3DNodeDatabase FrameBufferTextureGenerator::database( 
                                                      "FrameBufferTextureGenerator", 
                                                      &newInstance< FrameBufferTextureGenerator >,
                                                      typeid( FrameBufferTextureGenerator ),
                                                      &X3DGroupingNode::database 
                                                       );

namespace FrameBufferTextureGeneratorInternals {
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, generateColorTextures, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, generateDepthTexture, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, depthBufferType, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, outputTextureType, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, samples, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, update, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, depthTexture, OUTPUT_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, colorTextures, OUTPUT_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, viewpoint, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, depthTextureProperties, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, colorTextureProperties, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FrameBufferTextureGenerator, background, INPUT_OUTPUT );
}

FrameBufferTextureGenerator::~FrameBufferTextureGenerator() {
  if( fbo_initialized ) {
    glDeleteFramebuffersEXT( 1, &fbo_id );
    glDeleteFramebuffersEXT( 1, &multi_samples_fbo_id );
    
    if( !generateDepthTexture->getValue() ) {
      glDeleteRenderbuffersEXT(1, &depth_id);      
      glDeleteRenderbuffersEXT(1, &multi_samples_depth_id);      
    }
     
    glDeleteRenderbuffersEXT(1, &stencil_id);   

    for( unsigned int i = 0; i<multi_samples_color_ids.size(); i++ ) {
      glDeleteRenderbuffersEXT(1, &multi_samples_color_ids[i] );
    } 
  }
  fbo_nodes.erase( this );
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
                                                          Inst< MFTexturePropertiesNode > _colorTextureProperties,
                                                          Inst< SFTexturePropertiesNode > _depthTextureProperties,
                                                          Inst< MFGeneratedTextureNode > _colorTextures, 
                                                          Inst< SFGeneratedTextureNode > _depthTexture,
                                                          Inst< SFString         > _depthBufferType,
                                                          Inst< SFString         > _outputTextureType,
                                                          Inst< SFInt32        > _samples,
                                                          Inst< SFString       > _update,
                                                          Inst< SFViewpointNode > _viewpoint,
                                                          Inst< SFBackgroundNode > _background,
                                                          Inst< SFInt32         > _width,
                                                          Inst< SFInt32         > _height ):
  X3DGroupingNode( _addChildren, _removeChildren, _children, _metadata, _bound, _bboxCenter, _bboxSize ),
  generateColorTextures( _generateColorTextures ),
  generateDepthTexture( _generateDepthTexture ),
  colorTextureProperties( _colorTextureProperties ),
  depthTextureProperties( _depthTextureProperties ),
  colorTextures( _colorTextures ),
  depthTexture( _depthTexture ),
  depthBufferType( _depthBufferType ),
  outputTextureType( _outputTextureType ),
  samples( _samples ),
  update( _update ),
  viewpoint( _viewpoint ),
  background ( _background ),
  width( _width ),
  height( _height ),
  fbo_initialized( false ),
  buffers_width(-1),
  buffers_height(-1),
  buffers_depth( -1 ),
  last_resize_success( true ),
  nr_samples( 0 ),
  render_func( NULL ),
  render_func_data( NULL ),
  always_use_existing_viewport( false ) {

  type_name = "FrameBufferTextureGenerator";
  database.initFields( this );

  generateDepthTexture->setValue( false );
  outputTextureType->addValidValue( "2D" );
  outputTextureType->addValidValue( "2D_RECTANGLE" );
  outputTextureType->addValidValue( "3D" );
  outputTextureType->addValidValue( "2D_ARRAY" );
  outputTextureType->setValue( "2D" );
  samples->setValue( 0 );
  width->setValue( -1 );
  height->setValue( -1 );

  depthBufferType->addValidValue( "DEPTH" );
  depthBufferType->addValidValue( "DEPTH16" );
  depthBufferType->addValidValue( "DEPTH24" );
  depthBufferType->addValidValue( "DEPTH32" );
  depthBufferType->addValidValue( "DEPTH32F" );
  depthBufferType->addValidValue( "DEPTH_STENCIL" );
  depthBufferType->addValidValue( "DEPTH24_STENCIL8" );
  depthBufferType->setValue( "DEPTH" );
  
  update->addValidValue( "NONE" );
  update->addValidValue( "NEXT_FRAME_ONLY" );
  update->addValidValue( "ALWAYS" );
  update->setValue( "ALWAYS" );

  // turn off display list since we want to get new values of the width
  // and height each loop to see if they have changed.
  displayList->setCacheMode( H3DDisplayListObject::DisplayList::OFF );

  fbo_nodes.insert( this );
}

#ifdef H3D_WINDOWS
#undef max
#endif
void FrameBufferTextureGenerator::render()     { 

  // Only render the texture once regardless of multi pass
  // rendering state.
  if ( X3DShapeNode::geometry_render_mode != X3DShapeNode::ALL &&
       X3DShapeNode::geometry_render_mode != X3DShapeNode::SOLID ) {
    return;
  }

  if( !GLEW_EXT_framebuffer_object ) {
    Console(4) << "Warning: Frame Buffer Objects not supported by your graphics card "
               << "(EXT_frame_buffer_object). FrameBufferTextureGenerator nodes will "
               << "not work." << endl;
    return;
  }

  string output_texture_type = outputTextureType->getValue();
  if( output_texture_type == "2D_ARRAY" && !GLEW_EXT_texture_array) {
    Console(4) << "Warning: Texture arrays not supported by your graphics card "
               << "(EXT_texture_array). FrameBufferTextureGenerator nodes with \"2D_ARRAY\" will "
               << "not work." << endl;
    return;
  } else if( output_texture_type == "2D_RECTANGLE" && !GLEW_ARB_texture_rectangle) {
    Console(4) << "Warning: Texture rectangles not supported by your graphics card "
               << "(ARB_texture_rectangle). FrameBufferTextureGenerator nodes with \"2D_RECTANGLE\" will "
               << "not work." << endl;
    return;
  }

  if( output_texture_type != "3D" && 
      output_texture_type != "2D_ARRAY" &&
      output_texture_type != "2D_RECTANGLE" ) {
    output_texture_type = "2D";
  }

  /// Check if we need to generate any textures.
  if( generateColorTextures->size() == 0 && (!generateDepthTexture->getValue() || output_texture_type == "3D" )  )
    return;
  
  const string &update_string = update->getValue();

  if( update_string == "NEXT_FRAME_ONLY" ) {
    update->setValue( "NONE" );
  } else if( update_string == "ALWAYS" ) {
    // just continue
  } else if( update_string == "NONE" ) {
    if( fbo_initialized ) return;
  } else {
    Console(3) << "Warning: Invalid value for \"update\" field in \""
               << getName() << "\" node (\"" << update_string
               << "\"). Must be one of \"NONE\", \"NEXT_FRAME_ONLY\"" 
               << " or \"ALWAYS\". Using \"ALWAYS\" instead." << endl;
  }

  GLint previous_fbo_id;
  glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &previous_fbo_id );
  /// Save current state.
  glPushAttrib( GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT | GL_VIEWPORT_BIT );

  /// Make sure all textures and buffers are initialized.
  if( !fbo_initialized ) initializeFBO();
  
  // get the width and height of the buffer in pixels.
  int current_width  = width->getValue();
  int current_height = height->getValue();

  if( current_height == -1 || current_width == -1 ) {
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    if( current_width == -1 )  current_width  = viewport[2];
    if( current_height == -1 ) current_height = viewport[3];
  } 

  bool using_stencil_buffer = (depthBufferType->getValue() == "DEPTH24_STENCIL8" ||
                               depthBufferType->getValue() == "DEPTH_STENCIL" );

  if( !always_use_existing_viewport ) {
    // Set viewport to span entire frame buffer 
    glViewport( 0 , 0, current_width, current_height );
  }

  unsigned int current_depth  = output_texture_type == "2D" || output_texture_type == "2D_RECTANGLE" ? 1: std::max( (int)children->size(), 1 );

  // ensure that all buffers are always of the frame buffer size
  if( buffers_width != current_width || buffers_height != current_height || buffers_depth != current_depth ) {
    last_resize_success = resizeBuffers( current_width, current_height, current_depth );
  }
  
  // Don't do anything if buffer resize had an error.
  if( !last_resize_success ) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previous_fbo_id);
    glPopAttrib();
    return;
  }

  if( nr_samples > 0 ) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, multi_samples_fbo_id);
  } else {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);
  }

  // set up to render to all color buffers
  if( GLEW_ARB_draw_buffers ) {
    glDrawBuffers( H3DMax( (int)color_ids.size(), 1 ), draw_buffers.get() );
  } else {
    Console(4) << "Warning: Your graphics card does not support multiple "
               << "render targets(ARB_draw_buffers). Only one color texture will"
               << " have update to their values";
  }

  // if a viewpoint has been specified use that instead of what has already 
  // been set up
  X3DViewpointNode* vp = viewpoint->getValue();
  if( vp ) {
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    vp->setupViewMatrix( X3DViewpointNode::MONO );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    vp->setupProjection( X3DViewpointNode::MONO,
                         (H3DFloat) current_width,
                         (H3DFloat) current_height,
                         (H3DFloat) 0.01, (H3DFloat) -1 ); // TODO: calculate near and far plane
  }

  if( output_texture_type == "2D" || output_texture_type == "2D_RECTANGLE" ) {
    // 2D textures. Render all nodes in children field into the textures.
   
    // render scene.
    if( render_func ) {
      render_func( this, -1, render_func_data );
    } else {
      // Get background and set clear color
      X3DBackgroundNode* bg = background->getValue();
      X3DViewpointNode* bgVP= vp ? vp : X3DViewpointNode::getActive();
      if ( bg && bgVP ) {
        RGBA clear_color = bg->glClearColor();
        glClearColor( clear_color.r, clear_color.g, clear_color.b, clear_color.a );
      }

      // Clear buffers.
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
      
      // Render background
      if ( bg && bgVP ) {
        const Vec3f &vp_position = bgVP->totalPosition->getValue();
        const Rotation &vp_orientation = bgVP->totalOrientation->getValue();
        const Matrix4f &vp_inv_m = bgVP->accInverseMatrix->getValue();
        Rotation vp_inv_rot = Rotation(vp_inv_m.getRotationPart());

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glRotatef( (H3DFloat) -(180/Constants::pi)*vp_orientation.angle, 
                    vp_orientation.axis.x, 
                    vp_orientation.axis.y,
                    vp_orientation.axis.z );
        glRotatef( (H3DFloat) (180/Constants::pi)*vp_inv_rot.angle, 
                    vp_inv_rot.axis.x, vp_inv_rot.axis.y, vp_inv_rot.axis.z );
        glDepthMask( GL_FALSE );
        bg->renderBackground();
        glDepthMask( GL_TRUE );
        glPopMatrix();
      }

      X3DShapeNode::GeometryRenderMode  m= X3DShapeNode::geometry_render_mode;

      if( children_multi_pass_transparency ) {
        X3DShapeNode::geometry_render_mode = X3DShapeNode::SOLID;
        X3DGroupingNode::render();
    
        X3DShapeNode::geometry_render_mode = X3DShapeNode::TRANSPARENT_BACK; 
        X3DGroupingNode::render();
    
        X3DShapeNode::geometry_render_mode = X3DShapeNode::TRANSPARENT_FRONT; 
        X3DGroupingNode::render();
        X3DShapeNode::geometry_render_mode = X3DShapeNode::ALL; 
      } else {
        X3DShapeNode::geometry_render_mode = X3DShapeNode::ALL; 
        X3DGroupingNode::render();
      }
      
      X3DShapeNode::geometry_render_mode= m;
    }
    
    // blit multi sample render buffer to output textures if using multi sampling.
    if( nr_samples > 0 ) {
      glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, multi_samples_fbo_id );
      glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, fbo_id );
      
      // blit multi sample buffers to textures.
      if( generateDepthTexture->getValue() ) {
        glBlitFramebufferEXT(0, 0, current_width, current_height, 0, 0, current_width, current_height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
      }
      
      for( unsigned int i = 0; i < color_ids.size(); i++ ) {
        glReadBuffer( GL_COLOR_ATTACHMENT0_EXT + i );
        glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT + i );
        glBlitFramebufferEXT(0, 0, current_width, current_height, 0, 0, current_width, current_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
      }
    }

  } else {
    // 3D texture. Each child in the children fields is rendered into a different
    // slice in the 3D texture.

    const NodeVector &c = children->getValue();
    
    for( unsigned int i = 0; i < c.size(); i++ ) {
      
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

      // set the render target to the correct slice for depth texture.
      if( output_texture_type == "2D_ARRAY" && generateDepthTexture->getValue() ) {
        glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, depth_id, 0, i );
        if( using_stencil_buffer )
          glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, depth_id, 0, i );
      }

      // set the render target to the correct slice for color textures.
      for( unsigned int j = 0; j < color_ids.size(); j++ ) {
        glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + j,
                                     color_ids[j], 0, i );   
      }

      if( nr_samples > 0 ) {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, multi_samples_fbo_id);
      } 
     
      if( !checkFBOCompleteness() ) {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previous_fbo_id);
        glPopAttrib();        
        return;
      }

      // render child
      if( render_func ) {
        render_func( this, i, render_func_data );
      } else {
        // Clear buffers.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

        if( c[i] ) {
          H3DDisplayListObject *tmp = dynamic_cast< H3DDisplayListObject* >( c[i]);
          if( tmp )
            tmp->displayList->callList();
          else
            c[i]->render();
        }
      }

      // blit multi sample render buffer to output textures if using multi sampling.
      if( nr_samples > 0 ) {
        glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, multi_samples_fbo_id );
        glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, fbo_id );
        
        // blit multi sample buffers to textures.
        if( generateDepthTexture->getValue() ) {
          glBlitFramebufferEXT(0, 0, current_width, current_height, 0, 0, current_width, current_height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
        }
        
        for( unsigned int i = 0; i < color_ids.size(); i++ ) {
          glReadBuffer( GL_COLOR_ATTACHMENT0_EXT + i );
          glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT + i );
          glBlitFramebufferEXT(0, 0, current_width, current_height, 0, 0, current_width, current_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
      }
    }
  }

  if( vp ) {
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
  }

  // reset fbo
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, previous_fbo_id);

  // reset previous state.
  glPopAttrib();

};

void FrameBufferTextureGenerator::initializeFBO() {
  if( !fbo_initialized ) {
    glGenFramebuffersEXT(1, &fbo_id); 
    glGenFramebuffersEXT(1, &multi_samples_fbo_id); 

    bool generate_2d = true;
    const string &output_texture_type = outputTextureType->getValue();
    if( output_texture_type == "3D" || output_texture_type == "2D_ARRAY" ) {
      generate_2d = false;
    } else {
      if( output_texture_type != "2D" && output_texture_type != "2D_RECTANGLE" ) {
        Console(4) << "Warning: Invalid outputTextureType value: \"" << output_texture_type 
                   << "\". Valid values are \"2D\", \"2D_RECTANGLE\", \"2D_ARRAY\" and \"3D\". Using 2D instead(in FrameBufferTextureGenerator node). " << endl;
      }
    }

    // If we are to generate depth texture use that id otherwise
    // create a new render buffer.
    if( generateDepthTexture->getValue() ) {
      if( generate_2d ) {
        GeneratedTexture *tex = new GeneratedTexture;
        // make sure the texture id is initialized.
        if( output_texture_type == "2D_RECTANGLE" ) {
          tex->ensureInitialized( GL_TEXTURE_RECTANGLE_ARB );
        } else {
          tex->ensureInitialized( GL_TEXTURE_2D );
        }
        depthTextureProperties->route( tex->textureProperties );
        depthTexture->setValue( tex, id );
        depth_id = tex->getTextureId();
      } else {
        if( output_texture_type == "2D_ARRAY" ) {
          GeneratedTexture3D *tex = new GeneratedTexture3D;
          // make sure the texture id is initialized.
          tex->ensureInitialized( GL_TEXTURE_2D_ARRAY_EXT );
          depthTextureProperties->route( tex->textureProperties );
          depthTexture->setValue( tex, id );
          depth_id = tex->getTextureId();
        } else if( output_texture_type == "3D" ) {
          Console(4) << "Warning: 3D depth textures cannot be generated by FrameBufferTextureGenerator. OpenGL does not support it. Depth texture will be undefined" << endl;
        }
      }
    } else {
      glGenRenderbuffersEXT(1, &depth_id);      
    }

    glGenRenderbuffersEXT(1, &stencil_id);   
    glGenRenderbuffersEXT(1, &multi_samples_depth_id);

    GLint max_draw_buffers, max_color_attachments;
    glGetIntegerv( GL_MAX_DRAW_BUFFERS_ARB, &max_draw_buffers );
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &max_color_attachments );

    const vector< string > &color_texture_types = generateColorTextures->getValue();

    size_t nr_color_textures = color_texture_types.size();
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

    TextureProperties *tp = NULL;

    // initialize color textures.
    for( size_t i = 0; i < nr_color_textures; i++ ) {
      if( i < colorTextureProperties->size() ) {
        tp = colorTextureProperties->getValueByIndex( i );
      }

      if( generate_2d ) {
        GeneratedTexture *tex = new GeneratedTexture;
        // make sure the texture id is initialized.
        if( output_texture_type == "2D_RECTANGLE" ) {
          tex->ensureInitialized( GL_TEXTURE_RECTANGLE_ARB );
        } else {
          tex->ensureInitialized( GL_TEXTURE_2D );
        }
        tex->textureProperties->setValue( tp );
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
        tex->textureProperties->setValue( tp );
        colorTextures->push_back( tex, id );
        color_ids.push_back( tex->getTextureId() );
      }
       
      GLuint ms_id;
      glGenRenderbuffersEXT( 1, &ms_id );
      multi_samples_color_ids.push_back( ms_id );
      draw_buffers.get()[i] = (GLenum) (GL_COLOR_ATTACHMENT0_EXT+i);
    }

    fbo_initialized = true;
  }
}

bool FrameBufferTextureGenerator::resizeBuffers( H3DInt32 width, H3DInt32 height, H3DInt32 depth ) {

  string output_texture_type = outputTextureType->getValue();
  const vector< string > &color_texture_types = generateColorTextures->getValue();
  bool using_stencil_buffer = (depthBufferType->getValue() == "DEPTH24_STENCIL8" ||
                               depthBufferType->getValue() == "DEPTH_STENCIL");
     
  GLenum texture_type = GL_TEXTURE_2D;
  if( output_texture_type == "2D_RECTANGLE" ) {
    texture_type = GL_TEXTURE_RECTANGLE_ARB;
  } else if( output_texture_type == "3D" ) {
    texture_type = GL_TEXTURE_3D;
  } else if( output_texture_type == "2D_ARRAY" ) {
    texture_type = GL_TEXTURE_2D_ARRAY_EXT;
  }

  // determine nr sample points.
  nr_samples = 0;
  if( !GLEW_EXT_framebuffer_blit || !GLEW_EXT_framebuffer_multisample ) {
    Console(4) << "Warning: Your graphics card does not support multi-sample draw "
               << "buffers(EXT_framebuffer_multisample). Multi-sampling will be off"
               << " (in FrameBufferTextureGenerator)." << endl;
  } else {
    GLint max_nr_samples;
    glGetIntegerv( GL_MAX_SAMPLES_EXT, &max_nr_samples );
    H3DInt32 s = samples->getValue();
    if( s > max_nr_samples ) {
      Console(4) << "Warning: Unsupported nr of multi-samples: " << s 
                 << ". Your graphics draw supports a maximum of " << max_nr_samples 
                 << " (in FrameBufferTextureGenerator)." << endl;
      nr_samples = max_nr_samples;
    } else {
      nr_samples = s;
    }
  }

  GLenum depth_internal_format = stringToInternalDepthFormat( depthBufferType->getValue() );
  GLenum depth_format = stringToDepthFormat( depthBufferType->getValue() );
  GLenum depth_type = stringToDepthType( depthBufferType->getValue() );

  if( nr_samples > 0 ) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, multi_samples_fbo_id);

    // create multi sample render buffers
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multi_samples_depth_id );
    glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, nr_samples, depth_internal_format, width, height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, multi_samples_depth_id);
    if( using_stencil_buffer )
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, multi_samples_depth_id);

    for( unsigned int i = 0; i<multi_samples_color_ids.size(); i++ ) {
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, multi_samples_color_ids[i] );
      glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, nr_samples, 
                                          stringToInternalFormat( color_texture_types[i]) ,
                                          width, height);
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, 
                                   GL_RENDERBUFFER_EXT, multi_samples_color_ids[i]);
    }
  
    if( !checkFBOCompleteness() ) return false;

  } 

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

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
        
      if( texture_type == GL_TEXTURE_2D_ARRAY_EXT ) {
        glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, depth_internal_format, width, height, depth, 0, 
                     depth_format, depth_type, NULL);
        
        glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, depth_id, 0, 0 );
        if( using_stencil_buffer )
          glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,depth_id, 0, 0 );
      } else {
        glTexImage2D( texture_type, 0, depth_internal_format, width, height, 0, 
                      depth_format, depth_type, NULL);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                  texture_type, depth_id, 0 );
        
        if( using_stencil_buffer )
          glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
                                    texture_type, depth_id, 0);
      } 
    }
  } else {
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_id);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depth_internal_format,
                             width, height); 
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                 GL_RENDERBUFFER_EXT, depth_id);
    if( using_stencil_buffer )
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
                                   GL_RENDERBUFFER_EXT, depth_id);
  }
    
  // set up stencil buffer
  // NOTE: seems like separate stencil buffers are not supported by current hardware.
  // Instead they use a special format and packs the depth and stencil buffer into
  // the same buffer. So for now the only supported way to have a stencil buffer
  // is to use the DEPTH_STENCIL depthBufferType.
  /*glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencil_id);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX8_EXT,
    width, height); 
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
    GL_RENDERBUFFER_EXT, stencil_id);
  */

  // set up color buffers
  for( size_t i = 0; i < color_texture_types.size(); i++ ) {
    glBindTexture( texture_type, color_ids[i] );
    // filter needs to be something else than GL_MIPMAP_LINEAR that is default
    // since that is not supported by FBO.
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

    GLint internal_format = stringToInternalFormat( color_texture_types[i] );

    if( texture_type == GL_TEXTURE_2D ) {
      glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, 
                   GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, (GLenum)( GL_COLOR_ATTACHMENT0_EXT + i ),
                                GL_TEXTURE_2D, color_ids[i], 0 );        
    } else if( texture_type == GL_TEXTURE_RECTANGLE_ARB ) {
      glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, internal_format, width, height, 0, 
                   GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, (GLenum)( GL_COLOR_ATTACHMENT0_EXT + i ),
                                GL_TEXTURE_RECTANGLE_ARB, color_ids[i], 0 );        
    } else if( texture_type == GL_TEXTURE_2D_ARRAY_EXT ) {
      glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, internal_format, width, height, depth, 0, 
                   GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, (GLenum)( GL_COLOR_ATTACHMENT0_EXT + i ),
                                   color_ids[i], 0,0 );        
    } else if( texture_type == GL_TEXTURE_3D ) {
      glTexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, 
                   GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, (GLenum)( GL_COLOR_ATTACHMENT0_EXT + i ),
                                GL_TEXTURE_3D, color_ids[i], 0, 0 );        
    }
  }

  // No color textures, disable drawing/reading from color buffers.
  if( color_ids.size() == 0 ) {
    glDrawBuffer( GL_NONE );
    glReadBuffer( GL_NONE );
  }

  if( !checkFBOCompleteness() ) return false;
     
  buffers_width = width;
  buffers_height = height;
  buffers_depth = depth;
  return true;
}

bool FrameBufferTextureGenerator::checkFBOCompleteness() {
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
  return true;
}

GLenum FrameBufferTextureGenerator::stringToInternalFormat( const string &s ) {

  GLint internal_format = GL_RGBA;

  if( s == "RGBA32F" ) { 
    if( GLEW_ARB_texture_float ) {
      internal_format = GL_RGBA32F_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support floating point textures (ARB_texture_float). Using RGBA instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "RGBA16F" ) { 
    if( GLEW_ARB_texture_float ) {
      internal_format = GL_RGBA16F_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support floating point textures (ARB_texture_float). Using RGBA instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "RGB32F" ) { 
    if( GLEW_ARB_texture_float ) {
      internal_format = GL_RGB32F_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support floating point textures (ARB_texture_float). Using RGB instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "RGB16F" ) { 
    if( GLEW_ARB_texture_float ) {
      internal_format = GL_RGB16F_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support floating point textures (ARB_texture_float). Using RGB instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "RGB" ) { 
    internal_format = GL_RGB;
  } else {
    if( s != "RGBA" ) {
      Console(4) << "Warning: Invalid generateColorTextures value: \"" << s 
                 << "\". Using \"RGBA\" instead(in FrameBufferTextureGenerator node). " << endl;
    }
  }
  
  return internal_format;
}

GLenum FrameBufferTextureGenerator::stringToDepthFormat( const string &s ) {
  GLenum format = GL_DEPTH_COMPONENT;
  if( (s == "DEPTH_STENCIL" ||
       s == "DEPTH24_STENCIL8" ) &&
      GLEW_EXT_packed_depth_stencil ) {
    format = GL_DEPTH_STENCIL_EXT;
  }
  return format;
}

GLenum FrameBufferTextureGenerator::stringToDepthType( const string &s ) {
  GLenum type = GL_FLOAT;
  if( (s == "DEPTH_STENCIL" ||
       s == "DEPTH24_STENCIL8" ) &&
      GLEW_EXT_packed_depth_stencil ) {
    type = GL_UNSIGNED_INT_24_8_EXT;
  }
  return type;
}

GLenum FrameBufferTextureGenerator::stringToInternalDepthFormat( const string &s ) {

  GLenum internal_format = GL_DEPTH_COMPONENT;

  if( s == "DEPTH16" ) { 
    if( GLEW_ARB_depth_texture ) {
      internal_format = GL_DEPTH_COMPONENT16_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support depth textures (ARB_depth_texture). Using DEPTH instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "DEPTH24" ) { 
    if( GLEW_ARB_depth_texture ) {
      internal_format = GL_DEPTH_COMPONENT24_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support depth textures (ARB_depth_texture). Using DEPTH instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "DEPTH32" ) { 
    if( GLEW_ARB_depth_texture ) {
      internal_format = GL_DEPTH_COMPONENT32_ARB;
    } else {
      Console(4) << "Warning: Your graphics card does not support depth textures (ARB_depth_texture). Using DEPTH instead(in FrameBufferTextureGenerator node). " << endl;
    }
  } else if( s == "DEPTH32F" ) { 
    if( GLEW_ARB_depth_buffer_float ) {
      internal_format = GL_DEPTH_COMPONENT32F;
    } else {
      Console(4) << "Warning: Your graphics card does not support floating point depth textures (ARB_depth_buffer_float). Using DEPTH instead(in FrameBufferTextureGenerator node). " << endl; 
    }
  } else if( s == "DEPTH24_STENCIL8" ) { 
    if( GLEW_EXT_packed_depth_stencil ) {
      internal_format = GL_DEPTH24_STENCIL8_EXT;
    } else {
      Console(4) << "Warning: Your graphics card does not support packed depth stencil buffers(EXT_packed_depth_stencil). Using DEPTH instead(in FrameBufferTextureGenerator node). " << endl; 
    }
  } else if( s == "DEPTH_STENCIL" ) { 
    if( GLEW_EXT_packed_depth_stencil ) {
      internal_format = GL_DEPTH_STENCIL_EXT;
    } else {
      Console(4) << "Warning: Your graphics card does not support packed depth stencil buffers(EXT_packed_depth_stencil). Using DEPTH instead(in FrameBufferTextureGenerator node). " << endl; 
    }
  } else {
    if( s != "DEPTH" ) {
      Console(4) << "Warning: Invalid depthBufferType value: \"" << s 
                 << "\". Using \"DEPTH\" instead(in FrameBufferTextureGenerator node). " << endl;
    }
  }
  
  return internal_format;
}


void FrameBufferTextureGenerator::setRenderCallback( RenderCallbackFunc func, 
                                                     void *args ) {
  render_func = func;
  render_func_data = args;
}
