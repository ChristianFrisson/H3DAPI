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
/// \file X3DParticleEmitterNode.cpp
/// \brief CPP file for X3DParticleEmitterNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DParticleEmitterNode.h"
#include "Viewpoint.h"
#include "X3DTextureNode.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DParticleEmitterNode::database( 
                                   "X3DParticleEmitterNode", 
                                   NULL, 
                                   typeid( X3DParticleEmitterNode ),
                                   &X3DNode::database );

namespace X3DParticleEmitterNodeInternals {
  FIELDDB_ELEMENT( X3DParticleEmitterNode, speed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DParticleEmitterNode, variation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DParticleEmitterNode, mass, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DParticleEmitterNode, surfaceArea, INPUT_OUTPUT );
}

X3DParticleEmitterNode::X3DParticleEmitterNode( 
                      Inst< SFNode  > _metadata,
                      Inst< SFFloat > _speed,
                      Inst< SFFloat > _variation,
                      Inst< SFFloat > _mass,
                      Inst< SFFloat > _surfaceArea  ):
  X3DNode( _metadata ),
  speed( _speed ),
  variation( _variation ),
  mass( _mass ),
  surfaceArea( _surfaceArea ) {

  type_name = "X3DParticleEmitterNode";
  database.initFields( this );

  speed->setValue( 0 );
  variation->setValue( 0.25 );
  mass->setValue( 0 );
  surfaceArea->setValue( 0 );
}

void X3DParticleEmitterNode::Particle::render( 
               X3DColorNode *color_ramp,
               const vector< H3DFloat > color_key,
               TextureCoordinate *tex_coord,
               const vector< H3DFloat > tex_coord_key  ) {
  if( isDead() ) return;
  glEnable( GL_BLEND );
  glEnable( GL_ALPHA_TEST );
  glAlphaFunc( GL_NOTEQUAL, 0 );
  //glDisable( GL_DEPTH_TEST );
  RGBA color;
  bool have_color_value = false;
  if( color_ramp ) {
    H3DFloat weight;
    int key_index = lookupKey( time_lived, 
                               weight, color_key );
    if( key_index > 3 ) {
      key_index = lookupKey( time_lived, 
                             weight, color_key );
    }
    
    if( color_key.size() > 0 && key_index >= 0 ) {
      have_color_value = true;
      if (weight<=0) 
        color = color_ramp->getColor( 0 );
      else if (weight>=1)
        color = color_ramp->getColor( color_ramp->nrAvailableColors() - 1 );
      else {
        RGB rgb_color;
        RGBA a = color_ramp->getColor( key_index );
        RGBA b = color_ramp->getColor( key_index+1 );
        Vec3f hsv_a = RGB( a.r, a.g, a.b ).toHSV();
        Vec3f hsv_b = RGB( b.r, b.g, b.b ).toHSV();
        H3DFloat range = H3DAbs( hsv_b.x - hsv_a.x );
        if( range <= 3 ) { 
          rgb_color.fromHSV( (1-weight)*hsv_a + (weight)*hsv_b );
        } else {
          range = 6 - range;
          H3DFloat step = range * weight;
          H3DFloat h = hsv_a.x < hsv_b.x ? hsv_a.x - step: hsv_a.x + step;
          if( h < 0 ) h += 6;
          if( h > 6 ) h -= 6;
          rgb_color.fromHSV( Vec3f( h,
                                    (1-weight)*hsv_a.y + (weight)*hsv_b.y,
                                    (1-weight)*hsv_a.z + (weight)*hsv_b.z ) );
        }
        color = RGBA( rgb_color.r, rgb_color.g, rgb_color.b, 
                      (1-weight) * a.a + weight *b.a ) ;
      }
      
    }
  }
  glColor3f( 1, 1, 0 );        

  if( type == POINT ) {
    // Save the old state of GL_LIGHTING 
    GLboolean lighting_enabled;
    glGetBooleanv( GL_LIGHTING, &lighting_enabled );
    glDisable( GL_LIGHTING );

    // disable texturing
    X3DTextureNode *texture = X3DTextureNode::getActiveTexture();
    if( texture ) texture->disableTexturing();

    if( have_color_value ) {
      glColor4f( color.r, color.g, color.b, color.a ); 
    }
    
    glEnable( GL_BLEND );
    glBegin( GL_POINTS );
    glVertex3f( position.x, position.y, position.z );
    glEnd();
    if( lighting_enabled ) glEnable( GL_LIGHTING );
    if( texture ) texture->enableTexturing();
  } else if( type == LINE ) {
    glEnable( GL_COLOR_MATERIAL );
    // Save the old state of GL_LIGHTING 
    GLboolean lighting_enabled;
    glGetBooleanv( GL_LIGHTING, &lighting_enabled );
    glDisable( GL_LIGHTING );

    // disable texturing
    X3DTextureNode *texture = X3DTextureNode::getActiveTexture();
    if( texture ) texture->disableTexturing();

    glEnable( GL_BLEND );

    if( have_color_value ) {
      glColor4f( color.r, color.g, color.b, color.a ); 
    }
    
    Vec3f line_dir = velocity;
    line_dir.normalizeSafe();
    line_dir = line_dir * size.y /2;
    glBegin( GL_LINES );
    glVertex3f( position.x - line_dir.x, 
                position.y - line_dir.y,
                position.z - line_dir.z );
    glVertex3f( position.x + line_dir.x, 
                position.y + line_dir.y,
                position.z + line_dir.z );
    glEnd();
    if( lighting_enabled ) glEnable( GL_LIGHTING );
    if( texture ) texture->enableTexturing();
  } else if( type == QUAD || type == TRIANGLE ) {
    // TODO: fix color material
    glEnable( GL_COLOR_MATERIAL );
    Vec3f t = Vec3f( size.x / 2, size.y/2, 0 );
    Vec3f max_corner = position + t; 
    Vec3f min_corner = position - t;
    glNormal3f( 0, 0, 1 );
    if( have_color_value ) {
      glColor4f( color.r, color.g, color.b, color.a ); 
    }
    glBegin( GL_QUADS );
    glTexCoord2f( 1, 1 );
    glVertex3f  ( max_corner.x, max_corner.y, max_corner.z );
    glTexCoord2f( 0, 1 );
    glVertex3f  ( min_corner.x, max_corner.y, max_corner.z );
    glTexCoord2f( 0, 0 );
    glVertex3f  ( min_corner.x, min_corner.y, max_corner.z );
    glTexCoord2f( 1, 0 );
    glVertex3f  ( max_corner.x, min_corner.y, max_corner.z );
    glEnd();
  } else if( type == SPRITE ) {
    Viewpoint *vp = Viewpoint::getActive();
    Matrix4f vp_to_local = Matrix4f( 1, 0, 0, -position.x,
                                  0, 1, 0, -position.y,
                                  0, 0, 1, -position.z,
                                  0, 0, 0, 1 ) *
      global_to_local *
      vp->accForwardMatrix->getValue();
    
    Vec3f vp_position =  vp_to_local * vp->position->getValue();
    Vec3f vp_y_axis = 
      vp_to_local.getScaleRotationPart() * 
      (vp->orientation->getValue() *  Vec3f( 0, 1, 0 ) );
    vp_y_axis.normalizeSafe();
    Vec3f particle_to_viewer = vp_to_local.getRotationPart() * Vec3f( 0, 0, 1); //vp_position;// - position;
    Vec3f X = vp_y_axis % particle_to_viewer;
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    GLfloat m[] = { X.x, X.y, X.z, 0,
                    vp_y_axis.x, vp_y_axis.y, vp_y_axis.z, 0,
                    particle_to_viewer.x, particle_to_viewer.y, particle_to_viewer.z, 0,
                    0, 0, 0, 1 };

    Matrix3f r = Matrix3f( X.x, X.y, X.z,
                           vp_y_axis.x, vp_y_axis.y, vp_y_axis.z,
                           particle_to_viewer.x, particle_to_viewer.y, particle_to_viewer.z).transpose();
    Rotation rr( r );
    //glTranslatef( 0.2, 0, 0 );
    glTranslatef( position.x, position.y, position.z );
    glMultMatrixf( m );

    // TODO: fix color material
    glEnable( GL_COLOR_MATERIAL );
    Vec3f t = Vec3f( size.x / 2, size.y/2, 0 );
    Vec3f max_corner = t; 
    Vec3f min_corner = - t;
    glNormal3f( 0, 0, 1 );
    if( have_color_value ) {
      glColor4f( color.r, color.g, color.b, color.a ); 
    }
    glBegin( GL_QUADS );
    glTexCoord2f( 1, 1 );
    glVertex3f  ( max_corner.x, max_corner.y, max_corner.z );
    glTexCoord2f( 0, 1 );
    glVertex3f  ( min_corner.x, max_corner.y, max_corner.z );
    glTexCoord2f( 0, 0 );
    glVertex3f  ( min_corner.x, min_corner.y, max_corner.z );
    glTexCoord2f( 1, 0 );
    glVertex3f  ( max_corner.x, min_corner.y, max_corner.z );
    glEnd();
    glPopMatrix();
  }  else if( type == GEOMETRY ) {
    if( geometry.get() ) {
      glMatrixMode( GL_MODELVIEW );
      glPushMatrix();
      glTranslatef( position.x, position.y, position.z );
      geometry->displayList->callList();
      glPopMatrix();
    }
  }
  //glEnable( GL_DEPTH_TEST );
}


