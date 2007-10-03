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
/// \file X3DGeometryNode.cpp
/// \brief CPP file for X3DGeometryNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <X3DGeometryNode.h>
#include <H3DSurfaceNode.h>
#include <GlobalSettings.h>
#include <GeometryBoundTreeOptions.h>

#include <HapticsOptions.h>
#include <HAPIHapticShape.h>
#include <DeviceInfo.h>
#include <HapticTriangleSet.h>
#include <HLDepthBufferShape.h>
#include <HLFeedbackShape.h>

#include <DebugOptions.h>

#include <FeedbackBufferCollector.h>

#include <X3DPointingDeviceSensorNode.h>
#include <HapticLineSet.h>
#include <HapticPointSet.h>

using namespace H3D;

H3DNodeDatabase X3DGeometryNode::database( "X3DGeometryNode", 
                                           NULL,
                                           typeid( X3DGeometryNode ) );

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
  use_culling( false ),
  allow_culling( true ),
  draw_debug_options( true ),
  cull_face( GL_BACK ),
  last_ti_ptr( 0 ),
  current_geom_id( -1 ) {

  type_name = "X3DGeometryNode";
  
  displayList->setOwner( this );
  bound->setOwner( this );
  boundTree->setName( "boundTree" );
  boundTree->setOwner( this );
  database.initFields( this );

  displayList->route( boundTree );
}

void X3DGeometryNode::initialize() {
  // The introduction of a ref_count_lock_pointer is changed here
  // in order to not have to change the constructor definition of
  // every single node.
  use_ref_count_lock = true;
  // deleted by RefCountedClass destructor.
  ref_count_lock_pointer = new MutexLock();
}

int X3DGeometryNode::getHapticShapeId( unsigned int index ) {

  if( index >= haptic_shape_ids.size() ) {
    for( size_t i = haptic_shape_ids.size(); i <= index; i++ ) {
      int shape_id = HAPI::HAPIHapticShape::genShapeId();
      haptic_shape_ids.push_back( shape_id );
    }
  }
  return haptic_shape_ids[ index ];
}

#ifdef HAVE_OPENHAPTICS
X3DGeometryNode::~X3DGeometryNode() {/*
  for( ShapeIdMap::iterator entry = hl_shape_ids.begin();
       entry != hl_shape_ids.end();
       entry++ )
  for( vector< HLuint>::iterator i = (*entry).second.begin();
       i != (*entry).second.end();
       i++ ) {
    HLuint hl_shape_id = *i; 
    hlDeleteShapes( hl_shape_id, 1 );
    hlRemoveEventCallback( HL_EVENT_MOTION, 
                           hl_shape_id,
                           HL_CLIENT_THREAD,
                           &motionCallback );
    hlRemoveEventCallback( HL_EVENT_TOUCH, 
                           hl_shape_id,
                           HL_CLIENT_THREAD,
                           &touchCallback );
    hlRemoveEventCallback( HL_EVENT_UNTOUCH, 
                           hl_shape_id,
                           HL_CLIENT_THREAD,
                           &untouchCallback );
  }*/
}

HAPI::HAPIHapticShape *X3DGeometryNode::getOpenGLHapticShape(
                                                    H3DSurfaceNode *_surface,
                                                    const Matrix4f &_transform,
                                                    HLint _nr_vertices ) {
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
		                                     this,
                                         _surface->getSurface(),
                                         _transform,
                                         &X3DGeometryNode::cleanUpFunction,
                                         touchable_face,
                                         camera_view,
                                         adaptive_viewport );
  } else {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HLFeedbackShape is destructed.
    ref();
    return new HAPI::HLFeedbackShape( this,
		                                  this,
                                      _surface->getSurface(),
                                      _transform,
                                      &X3DGeometryNode::cleanUpFunction,
                                      _nr_vertices,
                                      touchable_face,
                                      camera_view );
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

  BugWorkaroundDisplayList::callList( build_list );

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
        glScalef( 1e-3f, 1e-3f, 1e-3f ); 
        tree->render( render_depth );
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


/// The HAPIBoundTree constructs a 
void X3DGeometryNode::SFBoundTree::update() { 
  X3DGeometryNode *geometry = static_cast< X3DGeometryNode * >( getOwner() );
  vector< HAPI::Collision::Triangle > triangles;
  vector< HAPI::Collision::LineSegment > lines;
  vector< HAPI::Collision::Point > points;
  HAPI::FeedbackBufferCollector::collectPrimitives( geometry, 
                                                    Matrix4f( 1e3f, 0, 0, 0,
                                                              0, 1e3f, 0, 0,
                                                              0, 0, 1e3f, 0,
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

  // if there exist a X3DPointingDeviceSensor add this node to its
  // geometry vector.
  if( !ti.current_pt_dev_sensors.empty() ) {
    if( last_ti_ptr != &ti ) {
      current_geom_id = -1;
      last_ti_ptr = &ti;
    }
    current_geom_id++;
    for( unsigned int i = 0; i < ti.current_pt_dev_sensors.size(); i++ ) {
      ti.current_pt_dev_sensors[i]->addGeometryNode( this, current_geom_id );
    }
  }
  
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
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
    
    if( force_full_oh ) {
#ifdef HAVE_OPENHAPTICS
      ti.addHapticShapeToAll( getOpenGLHapticShape( ti.getCurrentSurface(),
                                                    ti.getAccForwardMatrix(),
                                                    nrVertices() ) );
#endif
    } else {
      const vector< H3DHapticsDevice * > &devices = ti.getHapticsDevices();
    
      for( unsigned int i = 0; i < devices.size(); i++ ) {

        createAndAddHapticShapes( ti, devices[i], i, openhaptics_options );
        
        //displayList->breakCache();        
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
  } 

  Vec3f scale = ti.getAccInverseMatrix().getScalePart();
  Matrix4f to_local = ti.getAccInverseMatrix();
  if( use_bound_tree )
    to_local = Matrix4f( 1e3, 0, 0, 0,
                         0, 1e3, 0, 0,
                         0, 0, 1e3, 0,
                         0, 0, 0, 1 ) * to_local;
  Vec3f local_proxy =  to_local * hd->proxyPosition->getValue();
  Vec3f local_last_proxy = to_local * hd->getPreviousProxyPosition();
  Vec3f movement = local_proxy - local_last_proxy;

  if( use_bound_tree ) {
    if( radius < 0 ) {
      boundTree->getValue()->getAllPrimitives( tris, lines, points );
    } else {
      boundTree->getValue()->getPrimitivesIntersectedByMovingSphere(
        radius * 1e3 * H3DMax( scale.x, H3DMax( scale.y, scale.z ) ),
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
                                  Matrix4f( 1e3f, 0, 0, 0,
                                            0, 1e3f, 0, 0,
                                            0, 0, 1e3f, 0,
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
      glScalef( 1e3f, 1e3f, 1e3f );
      while( !done ) {
        HAPI::FeedbackBufferCollector::startCollecting( nr_values, 
                                center * 1e3f, 
                                (full_movement + 
                                Vec3f( d, d, d ) *  H3DMax( scale.x,
                                H3DMax( scale.y, 
                                scale.z ) ) 
                                * 1e3f  ) );
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

  if( tris.size() > 0 )  {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HapticTriangleSet is destructed.
    ref();
    HAPI::HapticTriangleSet * tri_set = 
      new HAPI::HapticTriangleSet( tris ,
      this,
      ti.getCurrentSurface()->getSurface(),
      Matrix4f( 1e3, 0, 0, 0,
                0, 1e3, 0, 0,
                0, 0, 1e3, 0,
                0, 0, 0, 1 ) *
      (ti.getAccForwardMatrix() *
      Matrix4f( 1e-3, 0, 0, 0,
                0, 1e-3, 0, 0,
                0, 0, 1e-3, 0,
                0, 0, 0, 1 )),
      &X3DGeometryNode::cleanUpFunction,
      -1,
      touchable_face );


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

    ti.addHapticShape( hd_index, tri_set );
  }

  if( lines.size() > 0 )  {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HapticLineSet is destructed.
    ref();
    HAPI::HapticLineSet * lin_set = 
      new HAPI::HapticLineSet( lines ,
      this,
      ti.getCurrentSurface()->getSurface(),
      Matrix4f( 1e3, 0, 0, 0,
                0, 1e3, 0, 0,
                0, 0, 1e3, 0,
                0, 0, 0, 1 ) *
      (ti.getAccForwardMatrix() *
      Matrix4f( 1e-3, 0, 0, 0,
                0, 1e-3, 0, 0,
                0, 0, 1e-3, 0,
                0, 0, 0, 1 )),
      &X3DGeometryNode::cleanUpFunction,
      -1,
      touchable_face );


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

    ti.addHapticShape( hd_index, lin_set );
  }

  if( points.size() > 0 )  {
    // Increase ref-count to have cleanupfunction decrease
    // it when the HapticPointSet is destructed.
    ref();
    HAPI::HapticPointSet * pt_set = 
      new HAPI::HapticPointSet( points ,
      this,
      ti.getCurrentSurface()->getSurface(),
      Matrix4f( 1e3, 0, 0, 0,
                0, 1e3, 0, 0,
                0, 0, 1e3, 0,
                0, 0, 0, 1 ) *
      (ti.getAccForwardMatrix() *
      Matrix4f( 1e-3, 0, 0, 0,
                0, 1e-3, 0, 0,
                0, 0, 1e-3, 0,
                0, 0, 0, 1 )),
      &X3DGeometryNode::cleanUpFunction,
      -1,
      touchable_face);


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

    ti.addHapticShape( hd_index, pt_set );
  }

}

bool X3DGeometryNode::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  vector< IntersectionInfo > &result,
                  vector< pair< Node *, H3DInt32 > > &theNodes,
                  const Matrix4f &current_matrix,
                  vector< Matrix4f > &geometry_transforms,
                  bool pt_device_affect ) {
  if( pt_device_affect )
    current_geom_id++;
  IntersectionInfo tempresult;
  bool returnValue =
    boundTree->getValue()->lineIntersect( 1000*from, 1000*to, tempresult );
  if( returnValue ) {
    tempresult.point = tempresult.point / 1000;
    tempresult.normal = tempresult.normal / 1000;
    result.push_back( tempresult );
    theNodes.push_back( make_pair( this, current_geom_id ) );
    geometry_transforms.push_back( current_matrix );
  }
  return returnValue;
}

void X3DGeometryNode::closestPoint(
                  const Vec3f &p,
                  vector< Vec3f > &closest_point,
                  vector< Vec3f > &normal,
                  vector< Vec3f > &tex_coord ) {
  Vec3d temp_closest_point, temp_normal, temp_tex_coord;
  boundTree->getValue()->closestPoint( p * 1000, temp_closest_point, 
                                       temp_normal, temp_tex_coord );
  closest_point.push_back( (Vec3f)temp_closest_point / 1000 );
  normal.push_back( (Vec3f)temp_normal / 1000 );
  tex_coord.push_back( (Vec3f)temp_tex_coord );
}


void X3DGeometryNode::resetNodeDefUseId() {
  current_geom_id = -1;
}

bool X3DGeometryNode::movingSphereIntersect( H3DFloat radius,
                                             const Vec3f &from, 
                                             const Vec3f &to ) {
  return boundTree->getValue()->movingSphereIntersect( radius * 1000.0f,
                                                       from * 1000.0f,
                                                       to * 1000.0f );
}

void X3DGeometryNode::incrNodeDefUseId( bool pt_device_affect ) {
  if( pt_device_affect )
    current_geom_id++;
}
