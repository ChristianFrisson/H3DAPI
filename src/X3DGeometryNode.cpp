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
/// \file X3DGeometryNode.cpp
/// \brief CPP file for X3DGeometryNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DGeometryNode.h>
#include <H3D/H3DSurfaceNode.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GeometryBoundTreeOptions.h>
#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/HapticsOptions.h>
#include <H3D/DeviceInfo.h>
#include <H3D/DebugOptions.h>
#include <H3D/Scene.h>
#include <H3D/HapticsRenderers.h>
#include <H3D/ShadowGeometry.h>
#include <H3D/H3DRenderModeGroupNode.h>

#ifdef HAVE_OPENHAPTICS
#include <HAPI/HLDepthBufferShape.h>
#include <HAPI/HLFeedbackShape.h>
#endif

#include <HAPI/HAPIHapticShape.h>
#include <HAPI/FeedbackBufferCollector.h>
#include <HAPI/HapticTriangleSet.h>
#include <HAPI/HapticLineSet.h>
#include <HAPI/HapticPointSet.h>

using namespace H3D;

H3DNodeDatabase X3DGeometryNode::database( "X3DGeometryNode", 
                                           NULL,
                                           typeid( X3DGeometryNode ),
                                           &X3DChildNode::database );

namespace X3DGeometryNodeInternals {
  FIELDDB_ELEMENT( X3DGeometryNode, isTouched, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, force, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, contactPoint, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, contactNormal, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, options, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DGeometryNode, contactTexCoord, OUTPUT_ONLY );
}

X3DGeometryNode::X3DGeometryNode( 
                                 Inst< SFNode>  _metadata,
                                 Inst< SFBound > _bound,
                                 Inst< DisplayList > _displayList,
                                 Inst< MFBool > _isTouched,
                                 Inst< MFVec3f > _force,
                                 Inst< MFVec3f > _contactPoint,
                                 Inst< MFVec3f > _contactNormal,
                                 Inst< MFVec3f > _contactTexCoord,
                                 Inst< SFBoundTree > _boundTree ) :
  X3DChildNode( _metadata ),
  H3DBoundedObject( _bound ),
  H3DDisplayListObject( _displayList ),
  isTouched( _isTouched ),
  force( _force ),
  contactPoint( _contactPoint ),
  contactTexCoord( _contactTexCoord ),
  contactNormal( _contactNormal ),
  boundTree( _boundTree ),
  options( new MFOptionsNode ),
  shadow_volume( NULL ),
  use_culling( false ),
  allow_culling( true ),
  draw_debug_options( true ),
  cull_face( GL_BACK ),
  print_negative_scaling_warning( true ) {

  type_name = "X3DGeometryNode";
  
  displayList->setOwner( this );
  bound->setOwner( this );
  boundTree->setName( "boundTree" );
  boundTree->setOwner( this );
  database.initFields( this );

  displayList->route( boundTree );
}

void X3DGeometryNode::initialize() {
  X3DChildNode::initialize();
  // The introduction of a ref_count_lock_pointer is changed here
  // in order to not have to change the constructor definition of
  // every single node. The ref_count_lock_pointer is deleted by the
  // RefCountedClass destructor.
  ref_count_lock_pointer = new MutexLock();
}

H3DShadowObjectNode *X3DGeometryNode::getShadowObject() {
  if( !shadow_volume.get() ) {
    ShadowGeometry *shadow_geom = new ShadowGeometry;
    shadow_geom->geometry->setValue( this );
    // Unref called to get rid of circular reference. This will work since
    // the ShadowGeometry should never be deleted until the shadow_volume
    // is removed. Which means that if unref is called then ~X3DGeometryNode
    // will be called when it is time to remove the X3DGeometryNode from the
    // geometry field of this ShadowGeometry.
    unref();
    shadow_volume.reset( shadow_geom );
  }
  return static_cast< H3DShadowObjectNode * >( shadow_volume.get() );
}

int X3DGeometryNode::getHapticShapeId( unsigned int index ) {

  if( index >= haptic_shape_ids.size() ) {
    for( size_t i = haptic_shape_ids.size(); i <= index; ++i ) {
      int shape_id = HAPI::HAPIHapticShape::genShapeId();
      haptic_shape_ids.push_back( make_pair( shape_id, 
                                             make_pair( Matrix4f(),
                                                        -1.0 /* No previous time yet */ ) ) );
    }
  }
  return haptic_shape_ids[ index ].first;
}

X3DGeometryNode::~X3DGeometryNode() {
  if( shadow_volume.get() ) {
    // Take care of the circular reference case.
    // In order to not accidently call delete for this X3DGeometryNode again
    // we call ref twice before setting the geometry field of the contained
    // ShadowGeometry to null. This will make the reference counter 2 before
    // ShadowGeometry calls unref.
    manual_initialize = true;
    // ref_count = 0
    ref();
    // ref_count = 1
    ref();
    // ref_count = 2
    // Simply set the geometry field to null since this might not be the
    // last reference to the ShadowGeometry (although in practice I think it
    // always is).
    static_cast< ShadowGeometry * >(shadow_volume.get() )->geometry->
      setValue( NULL );
    // ref_count = 1, it is safe to destroy the X3DGeometryNode properly
    // delete was never called.
  }
}

#ifdef HAVE_OPENHAPTICS
HAPI::HAPIHapticShape *X3DGeometryNode::getOpenGLHapticShape(
                                                    unsigned int hd_index,
                                                    H3DSurfaceNode *_surface,
                                                    const Matrix4f &_transform,
                                                    int _nr_vertices ) {
  int type = -1;
  bool adaptive_viewport = true;
  bool camera_view = true;
  HAPI::Collision::FaceType touchable_face;
  
  if( usingCulling() ) {
    if( getCullFace() == GL_FRONT ) touchable_face = HAPI::Collision::BACK;
    else touchable_face = HAPI::Collision::FRONT;
  } else {
    touchable_face = HAPI::Collision::FRONT_AND_BACK;
  }

  OpenHapticsOptions *openhaptics_options = NULL;

  getOptionNode( openhaptics_options );

  if( !openhaptics_options ) {
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      default_settings->getOptionNode( openhaptics_options );
    }
  }

  if( openhaptics_options ) {
    const string &shape = openhaptics_options->GLShape->getValue();
    if( shape == "FEEDBACK_BUFFER" ) {
      type = 0;
    } else if( shape == "DEPTH_BUFFER" ) {
      type = 1;
    } else {
      Console(4) << "Warning: Invalid OpenHaptics GLShape type: "
                 << shape 
                 << ". Must be \"FEEDBACK_BUFFER\" or \"DEPTH_BUFFER\" "
                 << "(in \"" << getName() << "\")" << endl;
    }
    // todo: fix touchable_face
/*    const string &face = openhaptics_options->touchableFace->getValue();
    if( face == "FRONT" ) touchable_face = HL_FRONT;
    else if( face == "BACK" ) touchable_face = HL_BACK;
    else if( face == "FRONT_AND_BACK" ) touchable_face = HL_FRONT_AND_BACK;
    else if( face == "AS_GRAPHICS" ) {
      // default values are the same as graphics
    } else {
      Console(4) << "Warning: Invalid default OpenHaptics touchable face: "
                 << face 
                 << ". Must be \"FRONT\", \"BACK\" or \"FRONT_AND_BACK\" "
                 << "(in active OpenHapticsSettings node\")" << endl;
                 }*/
    
    adaptive_viewport = openhaptics_options->useAdaptiveViewport->getValue();
    camera_view = openhaptics_options->useHapticCameraView->getValue();
  }

  if( type == 1 ) {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HLDepthBufferShape is destructed.
    ref();
    return new HAPI::HLDepthBufferShape( this,
                                      _transform,
                                      _surface->getSurface( hd_index ),
                                      touchable_face,
                                      camera_view,
                                      adaptive_viewport,
                                      this,
                                      -1,
                                      &X3DGeometryNode::cleanUpFunction );
  } else {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HLFeedbackShape is destructed.
    ref();
    return new HAPI::HLFeedbackShape( this,
                                      _transform,
                                      _surface->getSurface( hd_index ),
                                      touchable_face,
                                      camera_view,
                                      adaptive_viewport,
                                      _nr_vertices,
                                      this,
                                      -1,
                                      &X3DGeometryNode::cleanUpFunction );
  }
}

#endif

void X3DGeometryNode::DisplayList::callList( bool build_list ) {
    
  X3DGeometryNode *geom = 
    static_cast< X3DGeometryNode * >( owner );
  GLboolean culling_enabled;
  glGetBooleanv( GL_CULL_FACE, &culling_enabled );

  GLint cull_face;
  glGetIntegerv( GL_CULL_FACE_MODE, &cull_face );

  if( geom->usingCulling() && geom->allowingCulling() ) {
    glEnable( GL_CULL_FACE );
  } else {
    glDisable( GL_CULL_FACE );
  }

  glCullFace( geom->getCullFace() );

  if( H3DRenderModeGroupNode::current_render_mode_group ) {
    // save current OpenGL state
    H3DRenderModeGroupNode::current_render_mode_group->saveCurrentState();
    // make the changes wanted to the state.
    H3DRenderModeGroupNode::current_render_mode_group->changeGeometryState();
    // We do not know what OpenGL states are changed  by the 
    // H3DRenderModeGroupNode, so we cannot have it cached in a display
    // list.
    breakCache();
  }

  BugWorkaroundDisplayList::callList( build_list );

  if( H3DRenderModeGroupNode::current_render_mode_group ) {
    H3DRenderModeGroupNode::current_render_mode_group->restoreState();
  }

  if( geom->draw_debug_options ) {
    H3DInt32 render_depth = -1;
    bool render_bound = false;
    DebugOptions *debug_options = NULL;
    geom->getOptionNode( debug_options );
    if( !debug_options ) {
      GlobalSettings *default_settings = GlobalSettings::getActive();
      if( default_settings ) {
        default_settings->getOptionNode( debug_options );
      }
    }
    
    if( debug_options ) {
      render_depth = debug_options->drawBoundTree->getValue();
      render_bound = debug_options->drawBound->getValue();
    }
    
    if( render_depth >= 0 ) {
      HAPI::Collision::BinaryBoundTree *tree = geom->boundTree->getValue();
      if( tree ) {
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        tree->renderBounds( render_depth );
        glPopMatrix();
      }
    }
  
    if( render_bound ) {
      Bound *b = geom->bound->getValue();
      if( b ) {
        b->render();
      }
    }
  }
  
  // restore previous values for culling
  if( culling_enabled ) glEnable( GL_CULL_FACE );
  else glDisable( GL_CULL_FACE );
  
  glCullFace( cull_face );

}

unsigned int X3DGeometryNode::DisplayList::cachingDelay(){
  // use local graphic option first, if local graphic option is null, check global one
  GraphicsOptions *options = NULL;
  X3DGeometryNode *geom = static_cast< X3DGeometryNode * >( getOwner() );
  geom->getOptionNode( options );
  if( options ) {// This geometry contains graphic option 
    return options->cachingDelay->getValue();
  }
  else {// check global graphic option instead
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings&&default_settings->optionNodesUpdated() ) {
      default_settings->getOptionNode( options );
      if( options ) {// update graphic options
        graphic_options_previous = options;
        //cache_delay_previous = options->cachingDelay->getValue();
        return options->cachingDelay->getValue();
      }else{
        // global setting change in last frame, but no graphic in it now
        graphic_options_previous = NULL;
        return cache_delay_default;
      }
    }
    else if( default_settings ) { 
      // global setting option node exist but not updated
      if( graphic_options_previous!=NULL ) {
        return graphic_options_previous->cachingDelay->getValue();
      }else{
        return cache_delay_default;
      }
    }else{
      // no global settings at all now
      graphic_options_previous = NULL;
      return cache_delay_default;
    }
  }
}

bool X3DGeometryNode::DisplayList::usingCaching(){
  X3DGeometryNode *geom = static_cast< X3DGeometryNode * >( getOwner() );


  if( cache_mode == ON ) return true;
  if( cache_mode == OFF ) return false;

  GraphicsOptions *options = NULL;

  geom->getOptionNode( options );
  if( !options ) {// no local option, try to use global setting
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings&&default_settings->optionNodesUpdated() ) {
      default_settings->getOptionNode( options );
      if( options ) { // global graphic option exist and need update
        graphic_options_previous = options;
        return options->useCaching->getValue();
      }else{
        // no graphic option in updated global setting
        graphic_options_previous = NULL;
        return true;
      }
    }else if( default_settings ) {// global setting exit but no option updated
      if( graphic_options_previous!=NULL ) {
        return graphic_options_previous->useCaching->getValue();
      }else{
        return true;
      }
    }else{// no global setting at all
      graphic_options_previous = NULL;
      return true;
    }
  }else{
    // local option exist, use local option
    return options->useCaching->getValue();
  }
}

/// The HAPIBoundTree constructs a 
void X3DGeometryNode::SFBoundTree::update() { 
  X3DGeometryNode *geometry = static_cast< X3DGeometryNode * >( getOwner() );
  vector< HAPI::Collision::Triangle > triangles;
  vector< HAPI::Collision::LineSegment > lines;
  vector< HAPI::Collision::Point > points;
  HAPI::FeedbackBufferCollector::collectPrimitives( geometry, 
                                                    Matrix4d( 1, 0, 0, 0,
                                                              0, 1, 0, 0,
                                                              0, 0, 1, 0,
                                                              0, 0, 0, 1 ),
                                                    triangles, 
                                                    lines, 
                                                    points );
  
  GeometryBoundTreeOptions *options = NULL;
  geometry->getOptionNode( options );
  if( !options ) {
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      default_settings->getOptionNode( options );
    }
  }

  if( options ) {
    const string &type = options->boundType->getValue();
    H3DInt32 max_triangles = options->maxTrianglesInLeaf->getValue();
    if( type == "AABB" ) {
      value = new HAPI::Collision::AABBTree( triangles,
                                          lines,
                                          points,
                                          max_triangles );
    } else if( type == "OBB" ) {
      value = new HAPI::Collision::OBBTree( triangles,
                                         lines,
                                         points,
                                         max_triangles );
    } else if( type == "SPHERE" ) {
      value = new HAPI::Collision::SphereBoundTree( triangles,
                                                 lines,
                                                 points,
                                                 max_triangles );
    } else {
      Console(4) << "Warning: Invalid boundType: "
                 << type
                 << ". Must be \"SPHERE\", \"OBB\" or \"AABB\" "
                 << "(in active GeometryBoundTreeOptions node for \" "
                 << geometry->getName() << "\" node). Using AABB instead." 
                 << endl;
      value = new HAPI::Collision::AABBTree( triangles,
                                          lines,
                                          points,
                                          max_triangles );
    }

  } else {
    value = new HAPI::Collision::AABBTree( triangles, lines, points );
  }

}

void X3DGeometryNode::traverseSG( TraverseInfo &ti ) {
  X3DChildNode::traverseSG( ti );
  
  if( H3DRenderModeGroupNode::current_render_mode_group ) {
    // We do not know what OpenGL states are changed  by the 
    // H3DRenderModeGroupNode, so we cannot have it cached in a display
    // list.
    displayList->breakCache();
  }

  if( ti.getCurrentSurface() ) {
    bool force_full_oh = false;
    // if we have an OpenHapticsOptions node, then set the OpenHaptics 
    // options in the HapticTriangleSet we just created.
    OpenHapticsOptions *openhaptics_options = NULL;
    getOptionNode( openhaptics_options );
    if( !openhaptics_options ) {
      GlobalSettings *default_settings = GlobalSettings::getActive();
      if( default_settings ) {
        default_settings->getOptionNode( openhaptics_options );
      }
    }
  
    if( openhaptics_options ) {
      force_full_oh = 
        openhaptics_options->forceFullGeometryRender->getValue();
    }

    const vector< H3DHapticsDevice * > &devices = ti.getHapticsDevices();
    for( unsigned int i = 0; i < devices.size(); ++i ) {
      
      if( !ti.hapticsEnabled( i ) || 
          !ti.getCurrentSurface() ||
          !ti.getCurrentSurface()->getSurface(i) ) continue; 

      bool tmp_force_full_oh = force_full_oh;
#ifdef HAVE_OPENHAPTICS
      if( tmp_force_full_oh && 
          !dynamic_cast< OpenHapticsRenderer * >
            ( devices[i]->hapticsRenderer->getValue() ) )
        tmp_force_full_oh = false;
#endif

      if( tmp_force_full_oh ) {
#ifdef HAVE_OPENHAPTICS
        ti.addHapticShape( i, getOpenGLHapticShape( i,
                                                    ti.getCurrentSurface(),
                                                    ti.getAccForwardMatrix(),
                                                    nrVertices() ) );
#endif
      } else {
        createAndAddHapticShapes( ti, devices[i], i, openhaptics_options );
      }
    }
  }
}

void X3DGeometryNode::createAndAddHapticShapes(
                                  TraverseInfo &ti,
                                  H3DHapticsDevice *hd,
                                  H3DInt32 hd_index,
                                  OpenHapticsOptions *openhaptics_options) {

  vector< HAPI::Collision::Triangle > tris;
  tris.reserve( 200 );
  vector< HAPI::Collision::LineSegment > lines;
  lines.reserve( 200 );
  vector< HAPI::Collision::Point > points;
  points.reserve( 200 );

  HapticsOptions *haptics_options = NULL;
  getOptionNode( haptics_options );

  H3DFloat radius = (H3DFloat) 0.015;
  H3DFloat lookahead_factor = 3;
  HAPI::Collision::FaceType touchable_face;
  bool use_bound_tree = true;
  string dynamic_mode = "TRANSFORM_CHANGED";

  if( usingCulling() ) {
    if( getCullFace() == GL_FRONT ) touchable_face = HAPI::Collision::BACK;
    else touchable_face = HAPI::Collision::FRONT;
  } else {
    touchable_face = HAPI::Collision::FRONT_AND_BACK;
  }

  if( !haptics_options ) {
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      default_settings->getOptionNode( haptics_options );
    }
  }

  if( haptics_options ) {
    const string &face = haptics_options->touchableFace->getValue();
    if( face == "FRONT" ) touchable_face = HAPI::Collision::FRONT;
    else if( face == "BACK" ) touchable_face = HAPI::Collision::BACK;
    else if( face == "FRONT_AND_BACK" ) 
      touchable_face = HAPI::Collision::FRONT_AND_BACK;
    else if( face == "AS_GRAPHICS" ) {
      // default values are the same as graphics
    } else {
      Console(4) << "Warning: Invalid default touchable face: "
        << face 
        << ". Must be \"FRONT\", \"BACK\" or \"FRONT_AND_BACK\" "
        << "(in active HapticsOptions node\" )" << endl;
    }

    radius = haptics_options->maxDistance->getValue();
    lookahead_factor = haptics_options->lookAheadFactor->getValue();
    use_bound_tree = haptics_options->useBoundTree->getValue();
    dynamic_mode = haptics_options->dynamicMode->getValue();
  } 

  Vec3f scale = ti.getAccInverseMatrix().getScalePart();
  Matrix4f to_local = ti.getAccInverseMatrix();
  unsigned int active_layer = ti.getCurrentLayer();
  const vector< Vec3f > &prev_proxy_pos = hd->getPreviousProxyPositions();
  if( active_layer < prev_proxy_pos.size() ) {
    Vec3f local_proxy = to_local *
      hd->proxyPositions->getValueByIndex( active_layer );
    Vec3f local_last_proxy = to_local * prev_proxy_pos[active_layer];
    Vec3f movement = local_proxy - local_last_proxy;

    if( use_bound_tree ) {
      if( radius < 0 ) {
        boundTree->getValue()->getAllPrimitives( tris, lines, points );
      } else {
        boundTree->getValue()->getPrimitivesIntersectedByMovingSphere(
          radius * H3DMax( scale.x, H3DMax( scale.y, scale.z ) ),
          local_proxy,
          local_proxy + movement * lookahead_factor,
          tris,
          lines,
          points );
      }
    } else {
      if( radius < 0 ) {
        HAPI::FeedbackBufferCollector::collectPrimitives(
                                    this,
                                    Matrix4d( 1, 0, 0, 0,
                                              0, 1, 0, 0,
                                              0, 0, 1, 0,
                                              0, 0, 0, 1 ),
                                    tris,
                                    lines,
                                    points );
      } else {
        int nr_values = nrFeedbackBufferValues();
        if( nr_values < 0 ) nr_values = 200000;
        bool done = false;
        H3DFloat d = 2 * radius;
        Vec3f full_movement = movement * lookahead_factor;
        Vec3f center = (local_proxy + local_proxy + full_movement)/2;
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();
        while( !done ) {
          HAPI::FeedbackBufferCollector::startCollecting( nr_values, 
                                  center, 
                                  full_movement + 
                                  Vec3f( d, d, d ) *  H3DMax( scale.x,
                                  H3DMax( scale.y, 
                                  scale.z ) ) );
          glRender();
          HAPI::FeedbackBufferCollector::ErrorType e = 
            HAPI::FeedbackBufferCollector::endCollecting( tris,
                                                          lines,
                                                          points );
          if( e != HAPI::FeedbackBufferCollector::
            NOT_ENOUGH_MEMORY_ALLOCATED  )
            done = true;
          else {
            if( nr_values == 0 ) nr_values = 200000; 
            else nr_values *= 2;
          }
        }
        glMatrixMode( GL_MODELVIEW );
        glPopMatrix();
      }
    }
  }

  if( print_negative_scaling_warning ) {
    Matrix3f m3 = ti.getAccForwardMatrix().getScaleRotationPart();
    if( ( m3.getRow( 0 ) % m3.getRow( 1 ) ) * m3.getRow(2) < 0 ) {
      Console(3) << "Warning: A parent transform node to the X3DGeometryNode "
                 << getName() << " contains a negative scaling coefficient. "
                 << " Haptics will most likely not be rendered correctly."
                 << endl;
      print_negative_scaling_warning = false;
    }
  }
  if( tris.size() > 0 )  {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HapticTriangleSet is destructed.
    ref();
    HAPI::HapticTriangleSet * tri_set = 
      new HAPI::HapticTriangleSet(  ti.getAccForwardMatrix(),
                                    tris ,
                                    ti.getCurrentSurface()->getSurface( hd_index ),
                                    HAPI::HapticTriangleSet::NOT_CONVEX,
                                    touchable_face,
                                    this,
                                    -1,
                                    &X3DGeometryNode::cleanUpFunction
                                    );

#ifdef HAVE_OPENHAPTICS
    if( openhaptics_options ) {
      HAPI::OpenHapticsRenderer::OpenHapticsOptions::ShapeType type;
      bool adaptive_viewport;
      bool camera_view;

      const string &shape = openhaptics_options->GLShape->getValue();
      if( shape == "FEEDBACK_BUFFER" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER;
      } else if( shape == "DEPTH_BUFFER" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::DEPTH_BUFFER;
      } else if( shape == "CUSTOM" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::CUSTOM;
      } else {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER;
        Console(4) << "Warning: Invalid OpenHaptics GLShape type: "
          << shape 
          << ". Must be \"FEEDBACK_BUFFER\", \"DEPTH_BUFFER\" "
          << "or \"CUSTOM\""
          << "(in \"" << getName() << "\")" << endl;
      }

      adaptive_viewport = 
        openhaptics_options->useAdaptiveViewport->getValue();
      camera_view = openhaptics_options->useHapticCameraView->getValue();
      tri_set->addRenderOption( 
        new HAPI::OpenHapticsRenderer::OpenHapticsOptions( type,
        adaptive_viewport,
        camera_view ) );
    }
#endif
 
    addDynamicInfoToShape( ti, dynamic_mode, tri_set );
  
    ti.addHapticShape( hd_index, tri_set );
  }
  
  if( lines.size() > 0 )  {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HapticLineSet is destructed.
    ref();
    HAPI::HapticLineSet * lin_set = 
      new HAPI::HapticLineSet( ti.getAccForwardMatrix(),
                               lines,
                               ti.getCurrentSurface()->getSurface( hd_index ),
                               touchable_face,
                               this,
                               -1,
                               &X3DGeometryNode::cleanUpFunction );


#ifdef HAVE_OPENHAPTICS
    if( openhaptics_options ) {
      HAPI::OpenHapticsRenderer::OpenHapticsOptions::ShapeType type;
      bool adaptive_viewport;
      bool camera_view;

      const string &shape = openhaptics_options->GLShape->getValue();
      if( shape == "FEEDBACK_BUFFER" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER;
      } else if( shape == "DEPTH_BUFFER" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::DEPTH_BUFFER;
      } else if( shape == "CUSTOM" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::CUSTOM;
      } else {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER;
        Console(4) << "Warning: Invalid OpenHaptics GLShape type: "
          << shape 
          << ". Must be \"FEEDBACK_BUFFER\", \"DEPTH_BUFFER\" "
          << "or \"CUSTOM\""
          << "(in \"" << getName() << "\")" << endl;
      }

      adaptive_viewport = 
        openhaptics_options->useAdaptiveViewport->getValue();
      camera_view = openhaptics_options->useHapticCameraView->getValue();
      lin_set->addRenderOption( 
        new HAPI::OpenHapticsRenderer::OpenHapticsOptions( type,
        adaptive_viewport,
        camera_view ) );
    }
#endif

    addDynamicInfoToShape( ti, dynamic_mode, lin_set );

    ti.addHapticShape( hd_index, lin_set );
  }

  if( points.size() > 0 )  {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HapticPointSet is destructed.
    ref();
    HAPI::HapticPointSet * pt_set = 
      new HAPI::HapticPointSet(  ti.getAccForwardMatrix(),
                                 points ,
                                 ti.getCurrentSurface()->getSurface( hd_index ),
                                 touchable_face,
                                 this,
                                 -1,
                                 &X3DGeometryNode::cleanUpFunction 
                                 );


#ifdef HAVE_OPENHAPTICS
    if( openhaptics_options ) {
      HAPI::OpenHapticsRenderer::OpenHapticsOptions::ShapeType type;
      bool adaptive_viewport;
      bool camera_view;

      const string &shape = openhaptics_options->GLShape->getValue();
      if( shape == "FEEDBACK_BUFFER" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER;
      } else if( shape == "DEPTH_BUFFER" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::DEPTH_BUFFER;
      } else if( shape == "CUSTOM" ) {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::CUSTOM;
      } else {
        type = HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER;
        Console(4) << "Warning: Invalid OpenHaptics GLShape type: "
          << shape 
          << ". Must be \"FEEDBACK_BUFFER\", \"DEPTH_BUFFER\" "
          << "or \"CUSTOM\""
          << "(in \"" << getName() << "\")" << endl;
      }

      adaptive_viewport = 
        openhaptics_options->useAdaptiveViewport->getValue();
      camera_view = openhaptics_options->useHapticCameraView->getValue();
      pt_set->addRenderOption( 
        new HAPI::OpenHapticsRenderer::OpenHapticsOptions( type,
        adaptive_viewport,
        camera_view ) );
    }
#endif

    addDynamicInfoToShape( ti, dynamic_mode, pt_set );

    ti.addHapticShape( hd_index, pt_set );
  }

}

bool X3DGeometryNode::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  if( result.detect_pt_device && !result.hasCurrentPointingDevice() ) {
    // If this function is called because we are detecting pointing device
    // sensors but there is no current one then there is no use in doing
    // line intersect on the actual triangles.
    return false;
  }
  Bound * the_bound = bound->getValue();
  if( !the_bound || the_bound->lineSegmentIntersect( from, to ) ) {
    IntersectionInfo temp_result;
    bool returnValue =
      boundTree->getValue()->lineIntersect( from, to, temp_result );
    if( returnValue ) {
      result.addResults( temp_result, this );
      result.addPtDevMap();
    }
    return returnValue;
  }
  return false;
}

void X3DGeometryNode::closestPoint( const Vec3f &p,
                                    NodeIntersectResult &result ) {
  Vec3d temp_closest_point, temp_normal, temp_tex_coord;
  boundTree->getValue()->closestPoint( p, temp_closest_point, 
                                       temp_normal, temp_tex_coord );
  IntersectionInfo temp_result;
  temp_result.point = temp_closest_point;
  temp_result.normal = temp_normal;
  temp_result.tex_coord = temp_tex_coord;
  result.addResults( temp_result, this );
}

bool X3DGeometryNode::movingSphereIntersect( H3DFloat radius,
                                             const Vec3f &from, 
                                             const Vec3f &to,
                                             NodeIntersectResult &result ) {
  Bound * the_bound = bound->getValue();
  if( !the_bound || the_bound->movingSphereIntersect( from, to, radius ) ) {
    IntersectionInfo temp_result;
    bool return_value =
      boundTree->getValue()->movingSphereIntersect( radius,
                                                    from,
                                                    to,
                                                    temp_result );
    if( return_value ) {
      result.addResults( temp_result, this );
    }
    return return_value;
  }
  return false;  
}

void X3DGeometryNode::addDynamicInfoToShape( TraverseInfo &ti,
                                             const string &dynamic_mode,
                                             HAPI::HAPIHapticShape *shape ) {
  unsigned int geom_count = ti.getGeometryCount( this );
  if( geom_count < haptic_shape_ids.size() ) {
    const Matrix4f &acc_frw = ti.getAccForwardMatrix();
    if( dynamic_mode != "NEVER" ) {
      // If this is the first time we have seen the shape, don't add dynamic info
      // because the previous transform will be incorrect. The last time is initialized
      // to -1, so check the time to determine if it is the first time.
      // Otherwise we get a kick on the first contact with the shape.
      if ( haptic_shape_ids[ geom_count ].second.second >= 0.0 ) {
        const Matrix4f &last_xf = haptic_shape_ids[ geom_count ].second.first;
        // calculate the time between this scenegraph loop and last
        H3DTime dt = (Scene::time->getValue() - haptic_shape_ids[ geom_count ].second.second );
        // calculate velocities
        Vec3f velocity = (acc_frw.getTranslationPart() - last_xf.getTranslationPart() ) / dt;
        Vec3f scale_velocity = (acc_frw.getScalePart() - last_xf.getScalePart() ) / dt;
        HAPI::Rotation angular_velocity( last_xf.inverse().getRotationPart() * acc_frw.getRotationPart() );
        angular_velocity.angle = angular_velocity.angle / dt;

        // set velocities on shape.
        shape->setVelocity( velocity );
        shape->setAngularVelocity( angular_velocity );
        shape->setGrowthRate( scale_velocity );
      
        // force the shape to be dynamic if specified in the HapticsOptions
        if( dynamic_mode == "ALWAYS" )
          shape->setForceDynamic( true );
        else  if( !(dynamic_mode == "TRANSFORM_CHANGED" ||
                    dynamic_mode == "ALWAYS" ) ) {
          Console(4) << "Warning: Invalid dynamic mode: "
                     << dynamic_mode 
                     << ". Must be \"ALWAYS\", \"NEVER\" or \"TRANSFORM_CHANGED\" "
                     << "(in active HapticsOptions node\" )" << endl;
        }
      }
    }

    haptic_shape_ids[ geom_count ].second = make_pair( acc_frw, 
                                                       Scene::time->getValue() );
  }
}

