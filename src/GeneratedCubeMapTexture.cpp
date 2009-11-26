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
/// \file GeneratedCubeMapTexture.cpp
/// \brief CPP file for GeneratedCubeMapTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeneratedCubeMapTexture.h>
#include <H3D/X3DBackgroundNode.h>
#include <H3D/DeviceInfo.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GeneratedCubeMapTexture::database( 
                           "GeneratedCubeMapTexture", 
                           &(newInstance< GeneratedCubeMapTexture > ), 
                           typeid( GeneratedCubeMapTexture ),
                           &X3DEnvironmentTextureNode::database );

namespace GeneratedCubeMapTextureInternals {
  FIELDDB_ELEMENT( GeneratedCubeMapTexture, size, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( GeneratedCubeMapTexture, update, INPUT_OUTPUT );
}

GeneratedCubeMapTexture::GeneratedCubeMapTexture( 
                                          Inst< DisplayList > _displayList,
                                          Inst< SFNode      > _metadata,
                                          Inst< SFString    > _update,
                                          Inst< SFInt32     > _size ) :
  X3DEnvironmentTextureNode( _displayList, _metadata ),
  H3DMultiPassRenderObject(),
  update( _update ),
  size( _size ),
  textures_initialized( false ),
  generating_textures( false ) {
    
  type_name = "GeneratedCubeMapTexture";
  database.initFields( this );

  size->setValue( 128 );
  update->addValidValue( "NONE" );
  update->addValidValue( "NEXT_FRAME_ONLY" );
  update->addValidValue( "ALWAYS" );
  update->setValue( "NONE" );
}

GeneratedCubeMapTexture::~GeneratedCubeMapTexture() {
  if( textures_initialized && GLEW_EXT_framebuffer_object ) {
    glDeleteRenderbuffersEXT( 1, &rbo_id );
    glDeleteFramebuffersEXT( 1, &fbo_id );
  }
}

void GeneratedCubeMapTexture::initializeTextures() {
  H3DInt32 dim = size->getValue();

  if( !cube_map_id ) {
    glGenTextures( 1, &cube_map_id );
  }
  
  glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, cube_map_id );


  if( GLEW_EXT_framebuffer_object ) {
    // create a framebuffer object
    glGenFramebuffersEXT(1, &fbo_id);
    
    // create a renderbuffer object to store depth info
    glGenRenderbuffersEXT(1, &rbo_id);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbo_id);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                             dim, dim );
  }

  if( !textures_initialized ) {
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, dim, dim, 
                  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, dim, dim, 
                  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, dim, dim, 
                  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, dim, dim, 
                  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, dim, dim, 
                  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, dim, dim, 
                  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, 
                     GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T,
                     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, 
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, 
                    GL_LINEAR);
	  textures_initialized = true;
  }
}

void GeneratedCubeMapTexture::render() {
  if( !GLEW_ARB_texture_cube_map ) {
    Console(4) << "Warning: ARB_texture_cube_map extension not supported "
               << "by your graphics card. ComposedCubeMapTexture node cannot "
               << "be used." << endl; 
  } else {
    if( !textures_initialized )
      initializeTextures();
    enableTexturing();
  }
}

void GeneratedCubeMapTexture::enableTexturing() {
  glEnable( GL_TEXTURE_CUBE_MAP_ARB );
}

void GeneratedCubeMapTexture::disableTexturing() {
  glDisable( GL_TEXTURE_CUBE_MAP_ARB );
}

void GeneratedCubeMapTexture::traverseSG( TraverseInfo &ti ) {
  X3DEnvironmentTextureNode::traverseSG( ti );
  H3DMultiPassRenderObject::traverseSG( ti );
  local_to_global = ti.getAccForwardMatrix();
}

void GeneratedCubeMapTexture::updateCubeMapTextures( X3DChildNode *n,
                                                     X3DViewpointNode *vp ) {
  if( !textures_initialized )
    initializeTextures();

  generating_textures = true;
  displayList->touch();
  glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, cube_map_id );

  if( GLEW_EXT_framebuffer_object ) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);
  }

  updateCubeMap( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vec3f( 0, 0, 0 ), 
                 Vec3f( 0, 0, 1 ), Vec3f( 0, -1, 0 ), n, vp ); 
  updateCubeMap( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vec3f( 0, 0, 0 ), 
                 Vec3f( 0, 0, -1 ), Vec3f( 0, -1, 0 ), n, vp ); 
  updateCubeMap( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vec3f( 0, 0, 0 ), 
                 Vec3f( 0, 1, 0 ), Vec3f( 0, 0, 1 ), n, vp ); 
  updateCubeMap( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vec3f( 0, 0, 0 ), 
                 Vec3f( 0, -1, 0 ), Vec3f( 0, 0, -1 ), n, vp ); 
  updateCubeMap( GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vec3f( 0, 0, 0 ), 
                   Vec3f( 1, 0, 0 ), Vec3f( 0, -1, 0 ), n, vp ); 
  updateCubeMap( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vec3f( 0, 0, 0 ), 
                 Vec3f( -1, 0, 0 ), Vec3f( 0, -1, 0 ), n, vp ); 
  generating_textures = false;

  if( GLEW_EXT_framebuffer_object ) {
    // switch back to window-system-provided framebuffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }

  displayList->touch();
}

void GeneratedCubeMapTexture::updateCubeMap( GLuint texture_target,
                                             const Vec3f & camera_position,
                                             const Vec3f & camera_dir,
                                             const Vec3f & camera_up,
                                             X3DChildNode *n,
                                             X3DViewpointNode *vp ) {

  // Use frame buffer object if supported, otherwise we just use the normal 
  // frame buffer and copy from that. In that case problems will occur if
  // the frame buffer size is smaller than the texture size, since we
  // will then copy rubish into the texture.
  if( GLEW_EXT_framebuffer_object ) {
    // attach the texture to FBO color attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              texture_target, cube_map_id, 0);
    
    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                 GL_RENDERBUFFER_EXT, rbo_id );

  
    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
      Console( 4)  << "GeneratedCubeMapTexture error: Error setting up frame "
                   << "buffer object" << endl;
      return;
    }
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrontFace( GL_CCW );

  // TODO: calculate the clip_near and clip_far values from the bounding
  // box of n and of the stylus of the haptics devices.
  H3DFloat clip_near = 0.01f;  // near viewing plane at 1cm
  H3DFloat clip_far  = 10.f; // far viewing plane at 10m
  X3DBackgroundNode *background = X3DBackgroundNode::getActive();
  gluPerspective( 90, 1.0, clip_near, clip_far);

  glViewport( 0, 0, size->getValue(), size->getValue() );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if( !GLEW_EXT_framebuffer_object ) {
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
  }

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  Rotation vp_orientation = vp->totalOrientation->getValue();
  Rotation vp_rot = 
    (Rotation)vp->accForwardMatrix->getValue().getRotationPart();

  // rotate the camera to be aligned to the viewpoint
  Vec3f c_dir =  
    vp_rot* ( vp_orientation * camera_dir );

  Vec3f c_up =  
    vp_rot * (vp_orientation * camera_up );

  Vec3f c_pos = local_to_global * Vec3f( 0, 0, 0 );

  Vec3f c_ref = c_pos + c_dir;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt( 0.0, 0.0, 0.0, 
             c_dir.x, c_dir.y, c_dir.z, 
             c_up.x, c_up.y, c_up.z );

  // Rotate the headlight in the direction of the viewpoint.
  glPushAttrib( GL_LIGHTING_BIT );
  Vec3f dir( 0, 0, -1 );
  dir = vp_rot * (vp_orientation * dir );
  dir = -dir;
  GLfloat dir_v[] = { dir.x, dir.y, dir.z, 0 };
  glLightfv( GL_LIGHT0, GL_POSITION, dir_v );

  // Render the background.
  if( background ) {
    glDepthMask( GL_FALSE );
    background->renderBackground();
    glDepthMask( GL_TRUE );
  }

  glLoadIdentity();
  gluLookAt( c_pos.x, c_pos.y, c_pos.z, 
             c_ref.x, c_ref.y, c_ref.z, 
             c_up.x, c_up.y, c_up.z );

	// Render the stylus of all haptics devices.
  DeviceInfo *di = DeviceInfo::getActive();
  if( di ) {
    di->renderStyli();
  }

  // Render the scene.
  H3DDisplayListObject *dlo = 
    dynamic_cast< H3DDisplayListObject * >( n );
  if( dlo )  dlo->displayList->callList();
  else n->render();

  glPopAttrib();



  // Copy the rendered scene to the texture if not using
  // frame buffer objects
  if( !GLEW_EXT_framebuffer_object ) {
    glCopyTexSubImage2D( texture_target, 0, 0, 0, 0, 0, 
                         size->getValue(), size->getValue() );
  }
}


/// Updates the cube map textures of all instances of 
/// GeneratedCubeMapTexture. The update field will be checked to 
/// see if an update is required. 
void GeneratedCubeMapTexture::renderPreViewpoint( X3DChildNode *n,
                                                  X3DViewpointNode *vp ) {
  const string &update_string = update->getValue();
  if( update_string == "NEXT_FRAME_ONLY" ) {
    updateCubeMapTextures( n, vp );
    update->setValue( "NONE" );
  } else if( update_string == "ALWAYS" ) {
    updateCubeMapTextures( n, vp );
  } else if( update_string == "NONE" ) {
    if(  getTextureId() == 0 ) 
      updateCubeMapTextures( n, vp );
  } else {
    Console(3) << "Warning: Invalid value for \"update\" field in \""
               << getName() << "\" node (\"" << update_string
               << "\"). Must be one of \"NONE\", \"NEXT_FRAME_ONLY\"" 
               << " or \"ALWAYS\"" << endl;
  }
}


