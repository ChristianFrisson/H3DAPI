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
/// \file X3DGeometryNode.cpp
/// \brief CPP file for X3DGeometryNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DGeometryNode.h"
#include "GlobalSettings.h"

#ifdef USE_HAPTICS
#include "OpenHapticsOptions.h"
#include "HLShape.h"
#include "HapticShape.h"
#include "DeviceInfo.h"
#include "HLDepthBufferShape.h"
#include "HLFeedbackShape.h"
#endif

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
}

X3DGeometryNode::X3DGeometryNode( 
                                 Inst< SFNode>  _metadata,
                                 Inst< SFBound > _bound,
                                 Inst< DisplayList > _displayList,
                                 Inst< MFBool > _isTouched,
                                 Inst< MFVec3f > _force,
                                 Inst< MFVec3f > _contactPoint,
                                 Inst< MFVec3f > _contactNormal) :
  X3DChildNode( _metadata ),
  H3DBoundedObject( _bound ),
  H3DDisplayListObject( _displayList ),
  isTouched( _isTouched ),
  force( _force ),
  contactPoint( _contactPoint ),
  contactNormal( _contactNormal ),
  options( new MFOptionsNode ),
  use_back_face_culling( false ),
  allow_back_face_culling( true ) {

  type_name = "X3DGeometryNode";
  
  displayList->setOwner( this );
  bound->setOwner( this );
  database.initFields( this );
}


#ifdef HAVE_OPENHAPTICS
void HLCALLBACK X3DGeometryNode::motionCallback( HLenum event,
                                                 HLuint object,
                                                 HLenum thread,
                                                 HLcache *cache,
                                                 void *userdata ) {
  CallbackData *cb_data = static_cast< CallbackData * >( userdata ); 
  int device_index = cb_data->device_index;
  X3DGeometryNode *geometry = cb_data->geometry;
  HLdouble n[3], p[3];
  hlCacheGetDoublev( cache, 
                     HL_PROXY_POSITION,
                     p );
  hlCacheGetDoublev( cache, 
                     HL_PROXY_TOUCH_NORMAL,
                     n );

  if( HLShape::getHLShape( object ) ) {
    const Matrix4f &m = dynamic_cast< HapticShape * >
      (HLShape::getHLShape( object ))->transform.inverse();

    if( device_index > (int)geometry->contactPoint->size() -1 )
      geometry->contactPoint->resize( device_index + 1, Vec3f( 0, 0, 0 ), geometry->id );
    if( device_index > (int)geometry->contactNormal->size() -1 )
      geometry->contactNormal->resize( device_index + 1, Vec3f( 1, 0, 0 ), geometry->id );
    if( device_index > (int)geometry->force->size() -1 )
      geometry->force->resize( device_index + 1, Vec3f( 0, 0, 0 ), geometry->id );

    Vec3f cp = m * Vec3f( (H3DFloat)p[0],
                         (H3DFloat)p[1],
                         (H3DFloat)p[2] );

    geometry->contactPoint->setValue( device_index, cp, geometry->id );

    Vec3f cn = m.getScaleRotationPart() * Vec3f( (H3DFloat)n[0],
                                                 (H3DFloat)n[1],
                                                 (H3DFloat)n[2] );
    cn.normalizeSafe();
    geometry->contactNormal->setValue( device_index, cn, geometry->id );
    
    Vec3f f;
    HLdouble hlforce[3];
    hlGetShapeDoublev( object, HL_REACTION_FORCE, hlforce );
    f =  m.getScaleRotationPart() * Vec3f( (H3DFloat)hlforce[0],
                                           (H3DFloat)hlforce[1],
                                           (H3DFloat)hlforce[2] );
    geometry->force->setValue( device_index, f, geometry->id );
  }

}

void HLCALLBACK X3DGeometryNode::touchCallback( HLenum event,
                                                HLuint object,
                                                HLenum thread,
                                                HLcache *cache,
                                                void *userdata ) {
  CallbackData *cb_data = static_cast< CallbackData * >( userdata ); 
  int device_index = cb_data->device_index;
  X3DGeometryNode *geometry = cb_data->geometry;
  // make sure contactPoint and contactNormal vectors are set 
  // before isTouched is set to avoid errors if routed to AutoUpdate
  // fields.
  X3DGeometryNode::motionCallback( event, object, thread, cache, userdata );

  if( device_index > (int)geometry->isTouched->size() -1 )
    geometry->isTouched->resize( device_index + 1, false, geometry->id );
  
  geometry->isTouched->setValue( device_index, 
                                 true,  
                                 geometry->id );  
}

void HLCALLBACK X3DGeometryNode::untouchCallback( HLenum event,
                                                  HLuint object,
                                                  HLenum thread,
                                                  HLcache *cache,
                                                  void *userdata ) {
  CallbackData *cb_data = static_cast< CallbackData * >( userdata ); 
  int device_index = cb_data->device_index;
  X3DGeometryNode *geometry = cb_data->geometry;
  if( device_index != -1 ) {
    if( device_index > (int)geometry->isTouched->size() -1 )
      geometry->isTouched->resize( device_index + 1, false, geometry->id );
    if( device_index > (int)geometry->force->size() -1 )
      geometry->force->resize( device_index + 1, Vec3f( 0, 0, 0 ), geometry->id );
    geometry->isTouched->setValue( device_index, 
                                   false,  
                                   geometry->id );  
    geometry->force->setValue( device_index, 
                               Vec3f( 0, 0, 0 ), 
                               geometry->id );
  }
}

HLuint X3DGeometryNode::getHLShapeId( HLHapticsDevice *hd,
                                      unsigned int index ) {
  ShapeIdMap::iterator i = hl_shape_ids.find( hd );
  if( i == hl_shape_ids.end() ) {
    i =  hl_shape_ids.insert( i, std::make_pair( hd, vector< HDuint >() ) );
  }
  vector< HLuint > &shape_ids = (*i).second;  
  if( index >= shape_ids.size() ) {
    for( size_t i = shape_ids.size();
         i <= index;
         i++ ) {
      // find the index of the haptics device
      DeviceInfo *di = DeviceInfo::getActive();
      int device_index = -1;
      if( di ) {
        const NodeVector &devices = di->device->getValue();
        for( unsigned int i = 0; i < devices.size(); i++ ) {
          if( (Node *)devices[i] == (Node *)hd )
            device_index = i;
        }
      }
      assert( device_index != -1 );

      CallbackData *cb_data = new CallbackData( this, device_index );
      callback_data.push_back( cb_data );
      hlEventd(  HL_EVENT_MOTION_LINEAR_TOLERANCE, 0 );
      HLuint hl_shape_id = hlGenShapes(1);
      shape_ids.push_back( hl_shape_id );
      hlAddEventCallback( HL_EVENT_MOTION, 
                          hl_shape_id,
                          HL_CLIENT_THREAD,
                          &motionCallback,
                          cb_data );
      hlAddEventCallback( HL_EVENT_TOUCH, 
                          hl_shape_id,
                          HL_CLIENT_THREAD,
                          &touchCallback,
                          cb_data );
      hlAddEventCallback( HL_EVENT_UNTOUCH, 
                          hl_shape_id,
                          HL_CLIENT_THREAD,
                          &untouchCallback,
                          cb_data );
    }
  }
  return shape_ids[index];
}

X3DGeometryNode::~X3DGeometryNode() {
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
  }
}

HapticShape *X3DGeometryNode::getOpenGLHapticShape( H3DSurfaceNode *_surface,
                                                    const Matrix4f &_transform,
                                                    HLint _nr_vertices ) {
  int type = -1;
  bool adaptive_viewport = true;
  bool camera_view = true;
  HLenum touchable_face = HL_FRONT_AND_BACK;

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
    
    const string &face = openhaptics_options->touchableFace->getValue();
    if( face == "FRONT" ) touchable_face = HL_FRONT;
    else if( face == "BACK" ) touchable_face = HL_BACK;
    else if( face == "FRONT_AND_BACK" ) touchable_face = HL_FRONT_AND_BACK;
    else if( face == "AS_GRAPHICS" ) {
      if( usingBackFaceCulling() ) touchable_face = HL_FRONT;
      else touchable_face = HL_FRONT_AND_BACK;
    } else {
      Console(4) << "Warning: Invalid default OpenHaptics touchable face: "
                 << face 
                 << ". Must be \"FRONT\", \"BACK\" or \"FRONT_AND_BACK\" "
                 << "(in active OpenHapticsSettings node\")" << endl;
    }

    adaptive_viewport = openhaptics_options->useAdaptiveViewport->getValue();
    camera_view = openhaptics_options->useHapticCameraView->getValue();
  }

  if( type == 1 ) {
    return new HLDepthBufferShape( this,
                                   _surface,
                                   _transform,
                                   touchable_face );
  } else {
    return new HLFeedbackShape( this,
                                _surface,
                                _transform,
                                _nr_vertices,
                                touchable_face );
  }
}

#endif

void X3DGeometryNode::DisplayList::callList( bool build_list ) {
    
  X3DGeometryNode *geom = 
    static_cast< X3DGeometryNode * >( owner );
    
  GLboolean culling_enabled;
  glGetBooleanv( GL_CULL_FACE, &culling_enabled );

  if( geom->usingBackFaceCulling() && geom->allowingBackFaceCulling() ) {
    glEnable( GL_CULL_FACE );
  } else {
    glDisable( GL_CULL_FACE );
  }

  BugWorkaroundDisplayList::callList( build_list );

  if( culling_enabled ) glEnable( GL_CULL_FACE );
  else glDisable( GL_CULL_FACE );
  
  }
