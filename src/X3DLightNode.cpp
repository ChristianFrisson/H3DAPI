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
/// \file X3DLightNode.cpp
/// \brief CPP file for X3DLightNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DLightNode.h>

using namespace H3D;

H3DNodeDatabase X3DLightNode::database( 
        "X3DLightNode", 
        NULL,
        typeid( X3DLightNode ),
        &X3DChildNode::database 
        );

namespace X3DLightNodeInternals {
  FIELDDB_ELEMENT( X3DLightNode, ambientIntensity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DLightNode, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DLightNode, intensity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DLightNode, on, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DLightNode, global, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DLightNode, shadows, INPUT_OUTPUT );

  inline void fillGLArray( const RGBA &c, GLfloat *gl_c ) {
    gl_c[0] = c.r;
    gl_c[1] = c.g;
    gl_c[2] = c.b;
    gl_c[3] = c.a;
  } 

  inline void fillGLArray( const Vec4f &v, GLfloat *gl_v ) {
    gl_v[0] = v.x;
    gl_v[1] = v.y;
    gl_v[2] = v.z;
    gl_v[3] = v.w;
  }

    inline void fillGLArray( const Vec3f &v, GLfloat *gl_v ) {
    gl_v[0] = v.x;
    gl_v[1] = v.y;
    gl_v[2] = v.z;
  }
}

using X3DLightNodeInternals::fillGLArray;

GLint X3DLightNode::global_light_index = -1;
GLint X3DLightNode::max_lights = -1;

X3DLightNode::X3DLightNode( 
                           Inst< SFNode >  _metadata,
                           Inst< SFFloat>  _ambientIntensity,
                           Inst< SFColor>  _color,
                           Inst< SFBool >  _global,
                           Inst< SFFloat>  _intensity,
                           Inst< SFBool >  _on,
                           Inst< SFBool >  _shadows ) :
  X3DChildNode( _metadata ),
  ambientIntensity( _ambientIntensity ),
  color           ( _color            ),
  global          ( _global           ),
  intensity       ( _intensity        ),
  on              ( _on               ),
  shadows         ( _shadows          ),
  last_ti_ptr( 0 ),
  act_global( false ) {

  type_name = "X3DLightNode";
  database.initFields( this );

  displayList->setOwner( this );

  ambientIntensity->setValue( 0 );
  color->setValue( RGB( 1,1,1 ) );
  global->setValue( false );
  intensity->setValue( 1 );
  on->setValue( true );
  shadows->setValue( true );

  ambientIntensity->route( displayList );
  color->route( displayList );
  intensity->route( displayList );
  on->route( displayList );
}

void X3DLightNode::enableGraphicsState() {
  if ( ( act_global && graphics_state_counter < traverse_sg_counter ) ||
       ( !global->getValue() && on->getValue() ) ) {

    light_index = getLightIndex( getName() );
    had_light_index.push_back( true );

    if( light_index < (GLuint)max_lights ) {
      GLLightInfo gl_light = getGLLightInfo();
      glPushAttrib( GL_LIGHTING_BIT );
      glEnable( GL_LIGHT0 +light_index );
      
      GLfloat col_d[4], col_a[4], col_s[4];
      fillGLArray( gl_light.diffuse, col_d );
      fillGLArray( gl_light.ambient, col_a );
      fillGLArray( gl_light.specular, col_s );

      glLightfv( GL_LIGHT0+light_index, GL_DIFFUSE, col_d );
      glLightfv( GL_LIGHT0+light_index, GL_SPECULAR, col_s );
      glLightfv( GL_LIGHT0+light_index, GL_AMBIENT, col_a );
      
      GLfloat pos[4];
      fillGLArray( gl_light.position, pos );
      glLightfv( GL_LIGHT0+light_index, GL_POSITION, pos );      

      if( !gl_light.isDirectionalLight() ) {
        glLightf(GL_LIGHT0+light_index, GL_CONSTANT_ATTENUATION, gl_light.constantAttenuation );
        glLightf(GL_LIGHT0+light_index, GL_LINEAR_ATTENUATION, gl_light.linearAttenuation );
        glLightf(GL_LIGHT0+light_index, GL_QUADRATIC_ATTENUATION, gl_light.quadraticAttenuation );
      } 

      if( gl_light.isSpotLight() ) {
        GLfloat dir_v[3];
        fillGLArray( gl_light.spotDirection, dir_v );
        glLightfv( GL_LIGHT0+light_index, GL_SPOT_DIRECTION, dir_v );
  glLightf( GL_LIGHT0+light_index, GL_SPOT_CUTOFF, gl_light.spotCutoff );
        glLightf( GL_LIGHT0+light_index, GL_SPOT_EXPONENT, gl_light.spotExponent );
      }
      // RGB col = color->getValue();
      // H3DFloat ai = ambientIntensity->getValue();
      // H3DFloat i = intensity->getValue();
      // cola[0] = col.r * ai;
      // cola[1] = col.g * ai;
      // cola[2] = col.b * ai;
      // glLightfv( GL_LIGHT0+light_index, GL_AMBIENT, cola );
    }
  }  else {
    had_light_index.push_back( false );
  }
  ++graphics_state_counter;
};

void X3DLightNode::disableGraphicsState() {
  --graphics_state_counter;
  if( had_light_index.back() ) {
    if( global_light_index + 1 <= max_lights ) {
      glPopAttrib();
    }
    --global_light_index;
  }
  had_light_index.pop_back();
  displayList->breakCache();
};


void X3DLightNode::traverseSG( TraverseInfo &ti ) {
  if( last_ti_ptr != &ti ) {
    last_ti_ptr = &ti;
    traverse_sg_counter = 0;
    graphics_state_counter = 0;
    global_light_transforms.clear();
    had_light_index.clear();
    act_global = false;
  }

  if( global->getValue() && on->getValue() ) {    
    global_light_transforms.push_back( ti.getAccForwardMatrix() );
    ++traverse_sg_counter;
    act_global = true;
  }
  else
    act_global = false;
}

GLuint X3DLightNode::getLightIndex( string name_for_error ) {
  if( max_lights == -1 ) {
    glGetIntegerv( GL_MAX_LIGHTS, &max_lights );
  }
  ++global_light_index;
    
  return (GLuint)global_light_index;
}

/// Add light to TraverseInfo.
void X3DLightNode::enableHapticsState( TraverseInfo &ti ) {
  // only add lights that are on.
  if( on->getValue() )
    ti.addActiveLightNode( this, ti.getAccForwardMatrix() );
}

/// Remove light from TraverseInfo
void X3DLightNode::disableHapticsState( TraverseInfo &ti ) {
  // only remove non-global lights, globals are still active.
  // not checking for on since it might have changed from enableHapticsState
  // and then we want to remove it anyway.
  if( !global->getValue() )
    ti.removeActiveLightNode( this, ti.getAccForwardMatrix() );
}
