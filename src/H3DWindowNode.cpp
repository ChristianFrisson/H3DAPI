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
/// \file H3DWindowNode.cpp
/// \brief CPP file for H3DWindowNode.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "AutoPtrVector.h"
#include "H3DWindowNode.h"
#include "Viewpoint.h"
#include "TimeStamp.h"
#include "Bound.h"
#include "H3DBoundedObject.h"
#include "DeviceInfo.h"
#include "H3DDisplayListObject.h"
#include "Exception.h"
#include "X3DBackgroundNode.h"
#include "MatrixTransform.h"
#include "NavigationInfo.h"
#include "StereoInfo.h"
#include "GeneratedCubeMapTexture.h"
#include "Fog.h"

#include <GL/glew.h>

using namespace H3D;


/////////////////////////////////////////////////////////////////////////////
//
// Initialize static members
//

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DWindowNode::database( "H3DWindowNode", 
                                         NULL, 
                                         typeid( H3DWindowNode ) );

namespace H3DWindowNodeInternals {
  FIELDDB_ELEMENT( H3DWindowNode, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DWindowNode, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DWindowNode, fullscreen, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DWindowNode, mirrored, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DWindowNode, renderMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DWindowNode, viewpoint, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DWindowNode, time, INPUT_OUTPUT );
}


bool H3DWindowNode::GLEW_init = false;
set< H3DWindowNode* > H3DWindowNode::windows;

H3DWindowNode::H3DWindowNode( 
                   Inst< SFInt32     > _width,
                   Inst< SFInt32     > _height,
                   Inst< SFBool      > _fullscreen,
                   Inst< SFBool      > _mirrored,
                   Inst< RenderMode  > _renderMode, 
                   Inst< SFViewpoint > _viewpoint,
                   Inst< SFTime      > _time     ) :
#ifdef WIN32
  rendering_context( NULL ),
#endif
  width     ( _width      ),
  height    ( _height     ),
  fullscreen( _fullscreen ),
  mirrored  ( _mirrored   ),
  renderMode( _renderMode ),
  viewpoint( _viewpoint ),
  time    ( _time ),
  last_render_child( NULL ),
  window_id( 0 ),
  rebuild_stencil_mask( false ),
  stencil_mask( NULL ),
  stencil_mask_height( 0 ),
  stencil_mask_width( 0 ) {
  
  type_name = "H3DWindowNode";
  database.initFields( this );

  width->setValue( 800 );
  height->setValue( 600 );
  fullscreen->setValue( false );
  mirrored->setValue( false );
  renderMode->setValue( "MONO" );
  time->setValue( TimeStamp::now() );

  windows.insert( this );
}

H3DWindowNode::~H3DWindowNode() {
  if( stencil_mask )
    free( stencil_mask );
  windows.erase( this );
}

void H3DWindowNode::shareRenderingContext( H3DWindowNode *w ) {
#ifdef WIN32
  BOOL res = wglShareLists( rendering_context, w->getRenderingContext() );
  if( !res ) {
    TCHAR buffer[80]; 
    DWORD dw = GetLastError(); 
    FormatMessage(
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  dw,
                  0,
                  buffer,
                  80, NULL );
    throw Exception::H3DAPIException( buffer,
                                      H3D_FULL_LOCATION );
  }
#else
  throw Exception::H3DAPIException( "shareRenderingContext not implemented", 
                         H3D_FULL_LOCATION );
#endif
}

void H3DWindowNode::initialize() {
  initWindowHandler();
  initWindow();

  if( !GLEW_init ) {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      stringstream s;
      s << "Glew init error: " << glewGetErrorString( err ) << ends;
      throw Exception::H3DAPIException( s.str() );
    }
    GLEW_init = true;
  }
    
#ifdef WIN32
  rendering_context = wglGetCurrentContext();
  if( renderMode->getRenderMode() == RenderMode::QUAD_BUFFERED_STEREO ) {
    // make sure that we got the required pixel format for stereo.
    HDC hdc = wglGetCurrentDC();
    int pixel_format = GetPixelFormat( hdc );
    PIXELFORMATDESCRIPTOR  pfd; 
    DescribePixelFormat(hdc, pixel_format,  
                        sizeof(PIXELFORMATDESCRIPTOR), &pfd); 
    if( !(pfd.dwFlags & PFD_STEREO) ) {
      Console(4) << "Warning: Stereo pixel format not supported by your "
                 << "graphics card. Quad buffered stereo cannot be used. "
                 << "Using \"MONO\" instead. " <<endl;
      renderMode->setValue( "MONO", id );
    }
  }
#endif    

  for( set< H3DWindowNode * >::iterator i = windows.begin();
       i != windows.end();
       i++ ) {
    if( (*i)!=this && (*i)->isInitialized() )
      shareRenderingContext( *i );
  }
  
  // configure OpenGL context for rendering.
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  glDepthMask( GL_TRUE );
  glEnable( GL_LIGHTING );
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
  GLfloat no_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, no_ambient);
  Node::initialize();
}

bool H3DWindowNode::calculateFarAndNearPlane( H3DFloat &clip_far,
                                         H3DFloat &clip_near,
                                         X3DChildNode *child_to_render,
                                         Viewpoint *vp,
                                         bool include_styli ) {

  // calculate the far and near clipping planes from the union of the 
  // bounding box of the child_to_render node and the styli of the 
  // haptics devices.

  bool success = true;
  vector< Bound * > bounds;
  AutoPtrVector< Bound > bounds_to_remove;
  
  if( include_styli ) {
    // get the Bounds of the styli of the H3DHapticsDevices.
    DeviceInfo *di = DeviceInfo::getActive();
    if( di ) {
      for( DeviceInfo::MFDevice::const_iterator i = di->device->begin();
           i != di->device->end();
           i++ ) {
        H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
        H3DBoundedObject *stylus_bound = 
          dynamic_cast< H3DBoundedObject * >( hd->stylus->getValue() );
        MatrixTransform *mt = 
          dynamic_cast< MatrixTransform * >( hd->stylus->getValue() );
        Bound *b = NULL;
        if( mt ) b = mt->transformedBound->getValue();
        else if( stylus_bound ) b = stylus_bound->bound->getValue();

        if( b ) {
          BoxBound *bb = dynamic_cast< BoxBound * >( b );
          if( bb ) {
            const Vec3f &pos = hd->weightedProxyPosition->getValue();
            //const Rotation &rot = hd->trackerOrientation->getValue();
            TransformedBoxBound *tbb = new TransformedBoxBound();
            tbb->boxBound->setValue( bb );
            Matrix4f m = hd->trackerOrientation->getValue();
            m[0][3] = pos.x;
            m[1][3] = pos.y;
            m[2][3] = pos.z;
            tbb->matrix->setValue( m );
            bounds.push_back( tbb );
            bounds_to_remove.push_back( tbb );
          } else {
            success = false;
          }
        }
      }
    }
  }
    
  // add the child node bound.
  H3DBoundedObject *bound_object = 
    dynamic_cast< H3DBoundedObject * >( child_to_render );
  MatrixTransform *transform = 
    dynamic_cast< MatrixTransform * >( child_to_render );
  Bound *b = NULL;
  if( transform ) b = transform->transformedBound->getValue();
  else if( bound_object ) b = bound_object->bound->getValue();
  
  if( b ){
    BoxBound *bb = dynamic_cast< BoxBound * >( b );
    if( bb ) {
      bounds.push_back( bb );
    } else {
      success = false;
    }
  }
  
  // if no bounded objects return
  if( bounds.size() == 0 || !success )
    return false;
  
  // calculate the total bound.
  auto_ptr< Bound > total_bound( Bound::boundUnion( bounds.begin(), 
                                                    bounds.end() ) );
    
  BoxBound *bound =  dynamic_cast< BoxBound * >( total_bound.get() );
  if( bound ) {
    Vec3f vp_position = vp->position->getValue();
    Rotation vp_orientation = vp->orientation->getValue();
    const Matrix4f &vp_frw_m = vp->accForwardMatrix->getValue();

    const Vec3f &bb_center = bound->center->getValue();
    const Vec3f &half_bb_size   = bound->size->getValue() / 2.0;
    // get the lookat vector in global coordinates.
    Vec3f vp_lookat = 
      (Matrix3f)vp_frw_m.getScaleRotationPart() * 
      (Matrix3f)vp_orientation * 
      Vec3f( 0, 0, -1 );
    vp_lookat.normalize();
    // get the position in global coordinates.
    Vec3f vp_pos = vp_frw_m * vp_position;
    
    // find the maximum and minimum distances from the viewpoint position
    // in the direction of the lookat vector to the corners of the 
    // bounding box.
    Vec3f trf_bb = bb_center + half_bb_size;
    Vec3f llc_bb = bb_center - half_bb_size;
    H3DFloat min_d, max_d;
    
    // +++
    Vec3f point = trf_bb;
    Vec3f v = point - vp_pos;
    min_d = max_d = vp_lookat * v;
    
    // ++-
    point.z = llc_bb.z;
    v = point - vp_pos;
    H3DFloat d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;
    
    // +--
    point.y = llc_bb.y;
    v = point - vp_pos;
    d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;
    
    // +-+
    point.z = trf_bb.z;
    v = point - vp_pos;
    d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;
    
    // --+
    point.x = llc_bb.x;
    v = point - vp_pos;
    d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;
    
    // -++
    point.y = trf_bb.y;
    v = point - vp_pos;
    d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;
    
    // -+-
    point.z = llc_bb.z;
    v = point - vp_pos;
    d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;
    
    // ---
    point.y = llc_bb.y;
    v = point - vp_pos;
    d = vp_lookat * v;
    if( d < min_d ) min_d = d;
    if( d > max_d ) max_d = d;

    // add an epsilon value to make sure nothing is clipped away
    // unintentially.
    clip_near = min_d - 1e-4f;
    clip_far = max_d + 1e-4f;

    // make sure the clip planes are in front of the camera. Depth
    // buffer precision is affected by the values of clip_far and clip_near.
    // The greater the radion clip_far/clip_near the less the precision will
    // be. So we do not want to set clip_near too close to 0. Therefore
    // we make sure that the ratio is at most 1000.
    if( clip_far <= 0 ) clip_far = 2*Constants::f_epsilon;
    if( clip_near <= clip_far * 0.001f ) clip_near = clip_far * 0.001f;
    
    return success;
  }
  return false;
}

void renderStyli() {
  // Render the stylus of each H3DHapticsDevice.
  DeviceInfo *di = DeviceInfo::getActive();
  if( di ) {
    di->renderStyli();
  }
}

void H3DWindowNode::render( X3DChildNode *child_to_render ) {
  if( !child_to_render ) return;
  
  if ( !isInitialized() )
    initialize();

  // make this the active window
  makeWindowActive();

  // set fullscreen mode
  setFullscreen( fullscreen->getValue() );

  H3DDisplayListObject *dlo = 
    dynamic_cast< H3DDisplayListObject * >( child_to_render );

  glPushAttrib( GL_ENABLE_BIT );

  X3DBackgroundNode *background = X3DBackgroundNode::getActive();
  Fog *fog = Fog::getActive();
  RGB clear_color = RGB( 0, 0, 0 );
  if( background ) clear_color = background->glClearColor();
  if( fog ) fog->renderFog(); 
  glClearColor( clear_color.r, clear_color.g, clear_color.b, 1 );
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  
  glEnable( GL_DEPTH_TEST ); 
  glEnable( GL_LIGHTING );
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
  GLfloat no_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, no_ambient);
  

  // enable headlight
  NavigationInfo *nav_info = NavigationInfo::getActive();
  if( nav_info && !nav_info->headlight->getValue() )
    glDisable( GL_LIGHT0 );
  else 
    glEnable(GL_LIGHT0);

    AutoRef< Viewpoint > vp_ref;
  // get the viewpoint. If the H3DWindowNode viewpoint field is set use that
  // otherwise use the stack top of the Viewpoint bindable stack.
  Viewpoint *vp = static_cast< Viewpoint * >( viewpoint->getValue() );
  if( !vp ) 
    vp = Viewpoint::getActive();
  if ( ! vp ) {
    vp = new Viewpoint;
    vp_ref.reset( vp );
  }

  RenderMode::Mode stereo_mode = renderMode->getRenderMode();

  Vec3f vp_position = vp->position->getValue();
  Rotation vp_orientation = vp->orientation->getValue();
  const Matrix4f &vp_inv_m = vp->accInverseMatrix->getValue();
  Rotation vp_inv_rot = (Rotation)vp_inv_m.getRotationPart();
  //const Matrix4f &vp_frw_m = vp->accForwardMatrix->getValue();
  GLfloat vp_inv_transform[] = { 
    vp_inv_m[0][0], vp_inv_m[1][0], vp_inv_m[2][0], 0,
    vp_inv_m[0][1], vp_inv_m[1][1], vp_inv_m[2][1], 0,
    vp_inv_m[0][2], vp_inv_m[1][2], vp_inv_m[2][2], 0,
    vp_inv_m[0][3], vp_inv_m[1][3], vp_inv_m[2][3], 1 };


  GeneratedCubeMapTexture::updateAllCubeMapTextures( child_to_render, vp );

  glViewport( 0, 0, width->getValue(), height->getValue() );

  if( rebuild_stencil_mask ) {
    // build up stencil buffer
    int w = width->getValue();
    // width needs to be a power of 2 because of how glDrawPixels work.
    // The extra values will be ignored.
    w += 4 - (w  % 4  );
    int h = height->getValue();
    if( stencil_mask ) free( stencil_mask );
    stencil_mask = (unsigned char *) malloc( w*h );
    stencil_mask_width = w;
    stencil_mask_height = h;

    if( stereo_mode == RenderMode::HORIZONTAL_INTERLACED ) {
      for( int i = 0; i < h; i++ )
        for( int j = 0; j < w; j++ )
          stencil_mask[i*w+j]=(i+1)%2;
    } else {
      for( int i = 0; i < h; i++ )
        for( int j = 0; j < w; j++ )
          stencil_mask[i*w+j]=(j+1)%2;
    }
    rebuild_stencil_mask = false;
  }

  H3DFloat fov_h, fov_v;
  H3DFloat lwidth = (H3DFloat) width->getValue();
  H3DFloat lheight = (H3DFloat) height->getValue();
  H3DFloat field_of_view = vp->fieldOfView->getValue();
  H3DFloat aspect_ratio = lwidth / lheight;
  
  // calculate the horizontal and vertical field of view components
  // as defined by the X3D spec for Viewpoint.
  if ( field_of_view > Constants::pi )
    field_of_view = (H3DFloat) Constants::pi;
  if ( lwidth < lheight ) {
    // width is smallest
    fov_h = field_of_view;
    fov_v = 2 * atan( ( lheight * tan(fov_h/2) ) / lwidth );
    if ( fov_v > Constants::pi ) {
      fov_v = (H3DFloat) Constants::pi;
      fov_h = 2 * atan( ( lwidth / lheight ) * tan(fov_v/2) );        
    }
  } else {
    fov_v = field_of_view;
    fov_h = 2 * atan( ( lwidth / lheight ) * tan(fov_v/2) );
    if ( fov_h > Constants::pi ) {
      fov_h = field_of_view;
      fov_v = 2 * atan( ( lheight * tan(fov_h/2) ) / lwidth );        
    }
  }
  
  H3DFloat clip_near = 0.01f;  // near viewing plane at 1cm
  H3DFloat clip_far  = 10.f; // far viewing plane at 10m

  // calculate the far and near clipping planes from the union of the 
  // bounding box of the child_to_render node and the styli of the 
  // haptics devices.
  calculateFarAndNearPlane( clip_far, clip_near, 
                            child_to_render, vp, true );
  
  if( nav_info && nav_info->visibilityLimit->getValue() != 0 )
    clip_far = nav_info->visibilityLimit->getValue();

  if( background ) {
    if( clip_near > 0.01f ) clip_near = 0.01f;
    if( clip_far < 0.051f ) clip_far = 0.1f;
  }

  H3DFloat interocular_distance = 0.06f;
  H3DFloat focal_distance = 0.6f;

  StereoInfo *stereo_info = StereoInfo::getActive();
  if( stereo_info ) {
    interocular_distance = stereo_info->interocularDistance->getValue();
    focal_distance = stereo_info->focalDistance->getValue();
  }

  H3DFloat half_interocular_distance = interocular_distance / 2;

  if( renderMode->isStereoMode() ) {
    // make sure the focal plane is between the near and far 
    // clipping planes.
    if( focal_distance <= clip_near ) {
      clip_near = focal_distance - 0.01f;
    }
    if( focal_distance >= clip_far ) {
      clip_far = focal_distance + 0.01f;
    }

    // The stereo rendering is made using the parallel axis asymmetric frustum 
    // perspective projection method, which means that the viewpoint for each 
    // eye remain parallell and an the asymmetric view frustum is set up using 
    // glFrustum.

    // LEFT EYE
    H3DFloat frustum_shift = 
      half_interocular_distance * clip_near / focal_distance; 

    H3DFloat top    = clip_near * tan(fov_v/2);
    H3DFloat bottom = -top;

    H3DFloat right  = aspect_ratio * top;
    H3DFloat left   = -right;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( mirrored->getValue() ) {
      glScalef( 1, -1, 1 );
      glFrontFace( GL_CW );
    } else {
      glFrontFace( GL_CCW );
    }
    
    glFrustum( left + frustum_shift,
               right + frustum_shift,
               bottom, top, 
               clip_near, clip_far );
    
    glDrawBuffer(GL_BACK_LEFT);
    
    if( stereo_mode == RenderMode::RED_BLUE_STEREO ||
        stereo_mode == RenderMode::RED_CYAN_STEREO )
      // render only the red component
      glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    else if( stereo_mode == RenderMode::VERTICAL_INTERLACED ||
             stereo_mode == RenderMode::HORIZONTAL_INTERLACED ||
             stereo_mode == RenderMode::VERTICAL_INTERLACED_GREEN_SHIFT ) {
      // TODO: Currently we redraw the stencil mask each loop, becuase of
      // the stencil mask can be cleared when running on a Sharp laptop
      // from somewhere outside H3D API. When this does not happen anymore
      // we can initialize the stencil buffer once.
      if( stencil_mask )
        glDrawPixels( stencil_mask_width, stencil_mask_height, 
                      GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencil_mask );
      // render only every second line
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL,1,1);
      glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    } else if( stereo_mode == RenderMode::HORIZONTAL_SPLIT ) {
      glViewport( 0, height->getValue() / 2, 
                  width->getValue(), height->getValue() / 2 );
    } else if( stereo_mode == RenderMode::VERTICAL_SPLIT ) {
      glViewport( 0, 0, width->getValue() / 2, height->getValue() );
    }

    // clear the buffers before rendering
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glRotatef( (H3DFloat) -(180/Constants::pi)*vp_orientation.angle, 
               vp_orientation.axis.x, 
               vp_orientation.axis.y,
               vp_orientation.axis.z );
    glRotatef( (H3DFloat) (180/Constants::pi)*vp_inv_rot.angle, 
               vp_inv_rot.axis.x, vp_inv_rot.axis.y, vp_inv_rot.axis.z );
    if( background ) {
      glDepthMask( GL_FALSE );
      background->renderBackground();
      glDepthMask( GL_TRUE );
    }
    glPopMatrix();
    glTranslatef( half_interocular_distance, 0, 0 );
    glRotatef( (H3DFloat) -(180/Constants::pi)*vp_orientation.angle, 
               vp_orientation.axis.x, 
               vp_orientation.axis.y,
               vp_orientation.axis.z );
 
    glTranslatef( -vp_position.x,
                  -vp_position.y, 
                  -vp_position.z );
    glMultMatrixf( vp_inv_transform );
    
    renderStyli();
    // render the scene
    if( dlo )  dlo->displayList->callList();
    else child_to_render->render();

    // RIGHT EYE
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if( mirrored->getValue() ) {
      glScalef( 1, -1, 1 );
      glFrontFace( GL_CW );
    } else {
      glFrontFace( GL_CCW );
    }
    glFrustum( left - frustum_shift,
               right - frustum_shift,
               bottom, top, 
               clip_near, clip_far );
    
    if( stereo_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
      glDrawBuffer(GL_BACK_RIGHT);
      // clear the buffers before rendering
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    } else if( stereo_mode == RenderMode::RED_BLUE_STEREO ) {
      glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE);
    } else if( stereo_mode == RenderMode::RED_CYAN_STEREO ) {
      glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE);
    } else if( stereo_mode == RenderMode::VERTICAL_INTERLACED ||
               stereo_mode == RenderMode::HORIZONTAL_INTERLACED ||
               stereo_mode == RenderMode::VERTICAL_INTERLACED_GREEN_SHIFT ) {
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_NOTEQUAL,1,1);
      glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    } if( stereo_mode == RenderMode::HORIZONTAL_SPLIT ) {
      glViewport( 0, 0, 
                  width->getValue(), height->getValue() / 2 );
    } else if( stereo_mode == RenderMode::VERTICAL_SPLIT ) {
      glViewport( width->getValue() / 2, 0, 
                  width->getValue() / 2, height->getValue() );
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glRotatef( (H3DFloat)-(180/Constants::pi)*vp_orientation.angle, 
               vp_orientation.axis.x, 
               vp_orientation.axis.y,
               vp_orientation.axis.z );
    glRotatef( (H3DFloat)(180/Constants::pi)*vp_inv_rot.angle, 
               vp_inv_rot.axis.x, vp_inv_rot.axis.y, vp_inv_rot.axis.z );
    if( background ) {
      glDepthMask( GL_FALSE );
      background->renderBackground();
      glDepthMask( GL_TRUE );
    }
    glPopMatrix();


    glTranslatef( -half_interocular_distance, 0, 0 );
    glRotatef( (H3DFloat) -(180/Constants::pi)*vp_orientation.angle, 
               vp_orientation.axis.x, 
               vp_orientation.axis.y,
               vp_orientation.axis.z );
      
    glTranslatef( -vp_position.x,
                  -vp_position.y, 
                  -vp_position.z );
    glMultMatrixf( vp_inv_transform );

    renderStyli();    
    if( dlo )  dlo->displayList->callList();
    else child_to_render->render();

    if( stereo_mode == RenderMode::RED_BLUE_STEREO ||
        stereo_mode == RenderMode::RED_CYAN_STEREO )
      glDisable( GL_BLEND );
    else if( stereo_mode == RenderMode::VERTICAL_INTERLACED ||
             stereo_mode == RenderMode::HORIZONTAL_INTERLACED ) 
      glDisable( GL_STENCIL_TEST );
    else if( stereo_mode == RenderMode::VERTICAL_INTERLACED_GREEN_SHIFT ) {
      glDisable( GL_STENCIL_TEST );
      	// do green shift
      glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_POLYGON_BIT | 
                   GL_POLYGON_STIPPLE_BIT | GL_VIEWPORT_BIT | 
                   GL_TRANSFORM_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
      glDisable(GL_POLYGON_STIPPLE);
      glDisable(GL_LINE_STIPPLE);
      glDisable(GL_DITHER);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_1D);
      glDisable(GL_POLYGON_SMOOTH);
      glDisable(GL_NORMALIZE);
      glDisable(GL_STENCIL_TEST);
      glDisable(GL_SCISSOR_TEST);
      glDisable(GL_ALPHA_TEST);
      glPixelTransferi(GL_MAP_COLOR,0);
      glPixelTransferi(GL_MAP_STENCIL,0);
      glPixelZoom(1.000000,1.000000);
      glViewport(0,0,width->getValue(),height->getValue());
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glOrtho(0.000000,width->getValue(),0.000000,
              height->getValue(),1.000000,100.000000);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glTranslated(0.000000,0.000000,-10.000000);
      glColorMask(0,1,0,0);
      glRasterPos2f(1.000000,0.000000);
      glCopyPixels(0,0,width->getValue(),height->getValue(),GL_COLOR);
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      glPopAttrib();
    }
    swapBuffers();
  } else {
    // MONO
    H3DFloat top    = clip_near * tan(fov_v/2);
    H3DFloat bottom = -top;
    H3DFloat right  = aspect_ratio * top;
    H3DFloat left   = -right;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if( mirrored->getValue() ) {
      glScalef( 1, -1, 1 );
      glFrontFace( GL_CW );
    } else {
      glFrontFace( GL_CCW );
    }
    glFrustum(left,right,bottom,top,clip_near,clip_far);
    
    glMatrixMode(GL_MODELVIEW);
    glDrawBuffer(GL_BACK);
    glLoadIdentity();
    
    glRotatef( (H3DFloat) -(180/Constants::pi)*vp_orientation.angle, 
               vp_orientation.axis.x, 
               vp_orientation.axis.y,
               vp_orientation.axis.z );

    // clear the buffers before rendering
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    glRotatef( (H3DFloat) (180/Constants::pi)*vp_inv_rot.angle, 
               vp_inv_rot.axis.x, vp_inv_rot.axis.y, vp_inv_rot.axis.z );

    if( background ) {
      glDepthMask( GL_FALSE );
      background->renderBackground();
      glDepthMask( GL_TRUE );
    } 
    glPopMatrix();

    glTranslatef( -vp_position.x, -vp_position.y, -vp_position.z );
    glMultMatrixf( vp_inv_transform );
    
    renderStyli();

    if( dlo )  dlo->displayList->callList();
    else child_to_render->render();
    swapBuffers();
  }
  glPopAttrib();
}

void H3DWindowNode::reshape( int w, int h ) {
  width->setValue( w );
  height->setValue( h );
  RenderMode::Mode stereo_mode = renderMode->getRenderMode();
  if( stereo_mode == RenderMode::VERTICAL_INTERLACED ||
      stereo_mode == RenderMode::HORIZONTAL_INTERLACED ||
      stereo_mode == RenderMode::VERTICAL_INTERLACED_GREEN_SHIFT ) {
    rebuild_stencil_mask = true;
  }
}


void H3DWindowNode::display() {
  if( last_render_child ) 
    render( last_render_child );
}

H3DWindowNode::RenderMode::Mode H3DWindowNode::RenderMode::getRenderMode() {
  upToDate();
  if( value == "MONO" )
    return MONO;
  else if( value == "QUAD_BUFFERED_STEREO" )
    return QUAD_BUFFERED_STEREO;
  else if( value == "VERTICAL_SPLIT" )
    return VERTICAL_SPLIT;
  else if( value == "HORIZONTAL_SPLIT" )
    return HORIZONTAL_SPLIT;
  else if( value == "VERTICAL_INTERLACED" )
    return VERTICAL_INTERLACED;
  else if( value == "HORIZONTAL_INTERLACED" )
    return HORIZONTAL_INTERLACED;
  else if( value == "RED_BLUE_STEREO" )
    return RED_BLUE_STEREO;
  else if( value == "RED_CYAN_STEREO" )
    return RED_CYAN_STEREO;
  else if( value == "VERTICAL_INTERLACED_GREEN_SHIFT" )
    return VERTICAL_INTERLACED_GREEN_SHIFT;  
  else {
    stringstream s;
    s << "Must be one of MONO, QUAD_BUFFERED_STEREO, HORIZONTAL_INTERLACED, "
      << "VERTICAL_INTERLACED, VERTICAL_INTERLACED_GREEN_SHIFT, "
      << "VERTICAL_SPLIT, HORIZONTAL_SPLIT, "
      << "RED_CYAN_STEREO or RED_BLUE_STEREO. " << ends;
    throw InvalidRenderMode( value, 
                             s.str(),
                             H3D_FULL_LOCATION );
  }
}
