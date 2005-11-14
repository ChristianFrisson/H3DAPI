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
#include "HLShape.h"
#include "HapticShape.h"

using namespace H3D;

H3DNodeDatabase X3DGeometryNode::database( "X3DGeometryNode", 
                                           NULL,
                                           typeid( X3DGeometryNode ) );

namespace X3DGeometryNodeInternals {
  FIELDDB_ELEMENT( X3DGeometryNode, isTouched, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, force, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, contactPoint, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DGeometryNode, contactNormal, OUTPUT_ONLY );
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
  contactNormal( _contactNormal ) {

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
  X3DGeometryNode *geometry = static_cast< X3DGeometryNode * >( userdata );
  HLdouble n[3], p[3];
  hlCacheGetDoublev( cache, 
                     HL_PROXY_POSITION,
                     p );
  hlCacheGetDoublev( cache, 
                     HL_PROXY_TOUCH_NORMAL,
                     n );



  // TODO: fix it for several devices.
  vector< Vec3f > v;
  vector< Vec3f > fv;
  if( HLShape::getHLShape( object ) ) {
    const Matrix4f &m = dynamic_cast< HapticShape * >
      (HLShape::getHLShape( object ))->transform.inverse();
    v.push_back( m * Vec3f( (H3DFloat)p[0],
                            (H3DFloat)p[1],
                            (H3DFloat)p[2] ) );
    geometry->contactPoint->setValue( v, geometry->id );

    v[0] = m.getScaleRotationPart() * Vec3f( (H3DFloat)n[0],
                                             (H3DFloat)n[1],
                                             (H3DFloat)n[2] );
    v[0].normalizeSafe();
    geometry->contactNormal->setValue( v, geometry->id );
    
    Vec3f f;
    HLdouble hlforce[3];
    hlGetShapeDoublev( object, HL_REACTION_FORCE, hlforce );
    f =  m.getScaleRotationPart() * Vec3f( (H3DFloat)hlforce[0],
                                           (H3DFloat)hlforce[1],
                                           (H3DFloat)hlforce[2] );
    fv.push_back( f );
    geometry->force->setValue( fv, geometry->id );
    //cerr << "X3D Force = " << f.x << ", " << f.y
    //     << ", " << f.z << endl;

  }

}

void HLCALLBACK X3DGeometryNode::touchCallback( HLenum event,
                                                HLuint object,
                                                HLenum thread,
                                                HLcache *cache,
                                                void *userdata ) {
  X3DGeometryNode *geometry = static_cast< X3DGeometryNode * >( userdata );
  // make sure contactPoint and contactNormal vectors are set 
  // before isTouched is set to avoid errors if routed to AutoUpdate
  // fields.
  X3DGeometryNode::motionCallback( event, object, thread, cache, userdata );
  vector< bool > v;
  v.push_back( true );
  geometry->isTouched->setValue( v, geometry->id );
}

void HLCALLBACK X3DGeometryNode::untouchCallback( HLenum event,
                                                  HLuint object,
                                                  HLenum thread,
                                                  HLcache *cache,
                                                  void *userdata ) {
  X3DGeometryNode *geometry = static_cast< X3DGeometryNode * >( userdata );
  vector< bool > v;
  v.push_back( false );
  geometry->isTouched->setValue( v, geometry->id );
  vector< Vec3f > fv;
  fv.push_back( Vec3f( 0, 0, 0 ) );
  geometry->force->setValue( fv, geometry->id );
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
      hlEventd(  HL_EVENT_MOTION_LINEAR_TOLERANCE, 0 );
      HLuint hl_shape_id = hlGenShapes(1);
      shape_ids.push_back( hl_shape_id );
      hlAddEventCallback( HL_EVENT_MOTION, 
                          hl_shape_id,
                          HL_CLIENT_THREAD,
                          &motionCallback,
                          this );
      hlAddEventCallback( HL_EVENT_TOUCH, 
                          hl_shape_id,
                          HL_CLIENT_THREAD,
                          &touchCallback,
                          this );
      hlAddEventCallback( HL_EVENT_UNTOUCH, 
                          hl_shape_id,
                          HL_CLIENT_THREAD,
                          &untouchCallback,
                          this );
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
#endif
