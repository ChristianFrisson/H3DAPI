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
/// \file PlanarReflector.cpp
/// \brief CPP file for PlanarReflector, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PlanarReflector.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/Scene.h>

using namespace H3D;

H3DNodeDatabase PlanarReflector::database( 
        "PlanarReflector", 
        &(newInstance< PlanarReflector >),
        typeid( PlanarReflector ),
        &X3DChildNode::database 
        );

namespace PlanarReflectorInternals {
  FIELDDB_ELEMENT( PlanarReflector, geometry, INPUT_OUTPUT );
}

list< PlanarReflector * > PlanarReflector::instances;

PlanarReflector::PlanarReflector( 
                           Inst< SFNode           > _metadata,
                           Inst< SFBound          > _bound,
                           Inst< SFVec3f          > _bboxCenter,
                           Inst< SFVec3f          > _bboxSize,
                           Inst< DisplayList      > _displayList,
                           Inst< SFGeometryNode   > _geometry
) :
  X3DChildNode( _metadata ),
  X3DBoundedObject( _bound ),
  H3DDisplayListObject( _displayList ),
  geometry  ( _geometry   ),
  use_geometry_bound( false ) {

  type_name = "PlanarReflector";
  database.initFields( this );

  displayList->setOwner( this );
  bound->setOwner( this );
  
  Scene::time->route( displayList );
  geometry->route( displayList );

  instances.push_back( this );
}


void PlanarReflector::update ( X3DChildNode *n,
                               X3DViewpointNode *vp,
                               const Matrix4f &vp_transform ) {
  glMatrixMode( GL_MODELVIEW );

  // draw mirror geometry into stencil buffer
  glPushMatrix();
  GLfloat t[16] = { local_to_global[0][0], local_to_global[1][0], local_to_global[2][0], local_to_global[3][0],
  local_to_global[0][1], local_to_global[1][1], local_to_global[2][1], local_to_global[3][1],
  local_to_global[0][2], local_to_global[1][2], local_to_global[2][2], local_to_global[3][2],
  local_to_global[0][3], local_to_global[1][3], local_to_global[2][3], local_to_global[3][3] };
  glMultMatrixf( t );
  
  glClear( GL_STENCIL_BUFFER_BIT );

  // write 1 into all values in the stencil buffer used when rendering the geomtery
  glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
  glStencilFunc( GL_ALWAYS, 1, 1 );
  glEnable( GL_STENCIL_TEST );
  // don't write to frame buffer or depth buffer
  glColorMask( 0, 0, 0, 0 );
  glDisable( GL_DEPTH_TEST );
  
  // draw mirror geometry
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->displayList->callList();
  
  // reset to frite to frame and depth buffer again
  glColorMask( 1, 1, 1, 1 );
  glEnable( GL_DEPTH_TEST );

  glPopMatrix();

  // set up test to only render the parts where the mirror geometry was rendered
  glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
  glStencilFunc( GL_EQUAL, 1, 1 );
  glClear( GL_DEPTH_BUFFER_BIT );

  glPushMatrix();
  
  // calculate reflection matrix
  Matrix4f full_vp = vp_transform * local_to_global;
  Vec3f P = full_vp * Vec3f( 0, 0, 0 );
  Vec3f V = full_vp.getScaleRotationPart() * Vec3f( 0, 0, 1 );
  Matrix4f m( 1 - 2*V.x*V.x, -2*V.x*V.y, -2*V.x*V.z, 2*(P*V)*V.x,
              -2*V.x*V.y, 1 - 2*V.y*V.y, -2*V.y*V.z, 2*(P*V)*V.y,
              -2*V.x*V.z, -2*V.y*V.z, 1 - 2*V.z*V.z, 2*(P*V)*V.z,
              0, 0, 0, 1 );

  // the reflection matrix
  GLfloat rm[16] = { m[0][0], m[1][0], m[2][0], m[3][0],
  m[0][1], m[1][1], m[2][1], m[3][1],
  m[0][2], m[1][2], m[2][2], m[3][2],
  m[0][3], m[1][3], m[2][3], m[3][3] };

  // the transformation used for viewpoint
  GLfloat vp_m[16] = { vp_transform[0][0], vp_transform[1][0], vp_transform[2][0], vp_transform[3][0],
  vp_transform[0][1], vp_transform[1][1], vp_transform[2][1], vp_transform[3][1],
  vp_transform[0][2], vp_transform[1][2], vp_transform[2][2], vp_transform[3][2],
  vp_transform[0][3], vp_transform[1][3], vp_transform[2][3], vp_transform[3][3] };

  // set up clip plane to clip everything behind the mirror
  glMultMatrixf( t );
  int plane_index = 0;
  Vec3f point( 0, 0, 0 );
  Vec3f normal( 0, 0, -1 );
  GLdouble e[] = {  normal.x, normal.y, normal.z, 
                 -normal.x*point.x - normal.y*point.y - normal.z*point.z };
  glClipPlane( GL_CLIP_PLANE0 + plane_index, e );
  glEnable( GL_CLIP_PLANE0 + plane_index );
  
  // draw scene
  glLoadMatrixf( rm );
  glMultMatrixf( vp_m );
  
  // reflection matrix contains a negative scaling so we need to change front face.
  glFrontFace( GL_CW );
  n->render();
  glDisable( GL_CLIP_PLANE0 + plane_index );
  glPopMatrix();
  glDisable( GL_STENCIL_TEST );

  // draw mirror geometry
  glClear( GL_DEPTH_BUFFER_BIT );
  glPushMatrix();
  glMultMatrixf( t );
  glEnable( GL_BLEND );
  glDisable( GL_LIGHTING );
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f( 0.6, 0.6, 0.6, 0.1 );
  if( g ) g->render();
  glEnable( GL_LIGHTING );
  glPopMatrix();
    /*
  glPushMatrix();
  GLfloat t[16] = { local_to_global[0][0], local_to_global[1][0], local_to_global[2][0], local_to_global[3][0],
  local_to_global[0][1], local_to_global[1][1], local_to_global[2][1], local_to_global[3][1],
  local_to_global[0][2], local_to_global[1][2], local_to_global[2][2], local_to_global[3][2],
  local_to_global[0][3], local_to_global[1][3], local_to_global[2][3], m[3][3] };
  glMultMatrixf( t );
  
  glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
  glStencilFunc( GL_ALWAYS, 1, 1 );
  glEnable( GL_STENCIL_TEST );

  glColorMask( 0, 0, 0, 0 );
  
  // blending maybe?
  //  render();
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->displayList->callList();
  
  glColorMask( 1, 1, 1, 1 );

  glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
  glStencilFunc( GL_ALWAYS, 1, 1 );
 // glClearColor( 0, 1, 0, 1 );
  //glClear( GL_COLOR_BUFFER_BIT );


    glClear( GL_DEPTH_BUFFER_BIT );
   if( g ) g->displayList->callList();
  glDisable( GL_STENCIL_TEST );
//   X3DGeometryNode *g = geometry->getValue();

 
 //if( g ) g->displayList->callList();

  glPopMatrix();
  */
  
}

void PlanarReflector::traverseSG( TraverseInfo &ti ) {
  local_to_global = ti.getAccForwardMatrix();
  Node *g = geometry->getValue();
  if ( g ) g->traverseSG( ti );
  X3DChildNode::traverseSG( ti );
}

bool PlanarReflector::lineIntersect(
                           const Vec3f &from,
                           const Vec3f &to,
                           vector< IntersectionInfo > &result,
                           vector< pair< Node *, H3DInt32 > > &theNodes,
                           const Matrix4f &current_matrix,
                           vector< Matrix4f > &geometry_transforms,
                           bool pt_device_affect ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) return g->lineIntersect( from, to, result,
                            theNodes,
                            current_matrix,
                            geometry_transforms,
                            pt_device_affect ); 
  else return false; 
}

void PlanarReflector::closestPoint(
                  const Vec3f &p,
                  vector< Vec3f > &closest_point,
                  vector< Vec3f > &normal,
                  vector< Vec3f > &tex_coord ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->closestPoint( p, closest_point, normal, tex_coord );
}

bool PlanarReflector::movingSphereIntersect( H3DFloat radius,
                                          const Vec3f &from, 
                                          const Vec3f &to ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) return g->movingSphereIntersect( radius, from, to );
  else return false;
}

void PlanarReflector::resetNodeDefUseId() {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->resetNodeDefUseId();
}

void PlanarReflector::incrNodeDefUseId( bool pt_device_affect ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->incrNodeDefUseId( pt_device_affect );
}
