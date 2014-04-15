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
/// \file ShadowSphere.cpp
/// \brief CPP file for ShadowSphere
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShadowSphere.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShadowSphere::database( "ShadowSphere", 
                                     &(newInstance<ShadowSphere>), 
                                     typeid( ShadowSphere ),
                                     &H3DShadowObjectNode::database );

namespace ShadowSphereInternals {
  FIELDDB_ELEMENT( ShadowSphere, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowSphere, position, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowSphere, detailLevel, INPUT_OUTPUT );
}

ShadowSphere::ShadowSphere( Inst< SFNode>  _metadata,
                            Inst< SFFloat > _radius,
                            Inst< SFVec3f > _position,
                            Inst< SFInt32 > _detailLevel ) :
  H3DShadowObjectNode( _metadata ),
  radius( _radius ),
  position( _position ),
  detailLevel( _detailLevel ) {

  type_name = "ShadowSphere";
  database.initFields( this );

  radius->setValue( 1 );
  position->setValue( Vec3f( 0, 0, 0 ) );
  detailLevel->setValue( 120 );
}


void ShadowSphere::renderShadow( X3DLightNode *light, 
                                 bool render_caps,
                                 const Matrix4f &local_to_global ) {
  H3DFloat r = radius->getValue();
  Vec3f pos = position->getValue();
  
  int nr_faces = detailLevel->getValue();

  MatrixTransform *t = transform->getValue();

  Matrix4f m, m_inv;
  if(t) {
    m = local_to_global * t->matrix->getValue();
  } else {
    m = local_to_global;
  }
  m_inv = m.inverse();

  DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );     
  PointLight *point_light = dynamic_cast< PointLight * >( light );     
  Vec3f light_dir, light_pos;
  Rotation rot;
  
  if( dir_light ) {
    light_dir = dir_light->direction->getValue();
    Vec3f dir = m_inv.getRotationPart() * light_dir;
    dir.normalizeSafe();
    rot = Rotation( Vec3f( 0, 1, 0 ), dir );
  } else if( point_light ) {
    light_pos = m_inv * point_light->location->getValue();
    Vec3f dir = pos - light_pos;
    // if lightsource is inside sphere, we skip the shadow volume.
    if( dir * dir <= r * r ) return;
    H3DFloat d = dir.length();
    if( d > Constants::f_epsilon ) {
      dir.normalizeSafe();
      rot = Rotation( Vec3f( 0, 1, 0 ), dir );
    }
    light_pos = -rot * light_pos;
  } else {
    return;
  }
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  GLfloat mv[] = { 
    m[0][0], m[1][0], m[2][0], 0,
    m[0][1], m[1][1], m[2][1], 0,
    m[0][2], m[1][2], m[2][2], 0,
    m[0][3], m[1][3], m[2][3], 1 };

  glMultMatrixf( mv );

  glTranslatef( pos.x, pos.y, pos.z );
  glRotatef( (GLfloat)( rot.angle * 180 / Constants::pi ),
             rot.axis.x, rot.axis.y, rot.axis.z );
  
  // render side
  glBegin( GL_QUAD_STRIP );
  for( int i=0; i<=nr_faces; ++i ) {
    float ratio = (float) i / nr_faces;
    if( i == nr_faces ) {
      ratio = 0;
    }
    float angle =  (float)(ratio * (Constants::pi*2));
    
    
    float sina = sin( angle );
    float cosa = cos( angle );
    //glNormal3f( -sina, 0, -cosa );
    Vec3f v( -r * sina, 0, -r * cosa );
    
    Vec3f d;
    if( point_light ) d = v - light_pos;
    else d = Vec3f( 0, 1, 0 ); 
      // point at infinity 
    glVertex4f( d.x, d.y, d.z, 0 );
    glVertex4f( v.x, v.y, v.z, 1 );
  }
  glEnd();
  
  if ( render_caps ) {
    if( !dir_light ) {
      // render top at infinity. Do not have to do this for DirectionalLight
      // since then it is already capped since all point converge to the same point
      // at infinity
      glBegin( GL_POLYGON );
      for( int i = 0; i < nr_faces; ++i ) {
        float ratio = (float) i / nr_faces;
        float angle = (float)( ratio * (Constants::pi*2));
        float sina = sin( angle );
        float cosa = cos( angle );
        Vec3f v = Vec3f(-r * sina, 0, -r * cosa ) - light_pos; 
        glVertex4f( v.x, v.y, v.z, 0 );
      }
      glEnd();
    }
    // render bottom
    glBegin( GL_POLYGON );
    glNormal3f( 0, -1, 0 );
    for( int i = nr_faces; i >= 0; --i ) {
      float ratio = (float) i / nr_faces;
      float angle = (float)( ratio * (Constants::pi*2) );
      float sina = sin( angle );
      float cosa = cos( angle );
      glVertex3f( -r * sina, 0, -r * cosa );
    }
    glEnd();
  }
  glPopMatrix();
}

