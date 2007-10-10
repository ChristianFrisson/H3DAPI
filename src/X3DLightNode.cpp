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
}

GLint X3DLightNode::global_light_index = -1;
GLint X3DLightNode::max_lights = -1;

X3DLightNode::X3DLightNode( 
                           Inst< SFNode >  _metadata,
                           Inst< SFFloat>  _ambientIntensity,
                           Inst< SFColor>  _color,
                           Inst< SFBool >  _global,
                           Inst< SFFloat>  _intensity,
                           Inst< SFBool >  _on ) :
  X3DChildNode( _metadata ),
  ambientIntensity( _ambientIntensity ),
  color           ( _color            ),
  global          ( _global           ),
  intensity       ( _intensity        ),
  on              ( _on               ),
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

  ambientIntensity->route( displayList );
  color->route( displayList );
  intensity->route( displayList );
  on->route( displayList );
}

void X3DLightNode::enableGraphicsState() {
      light_index = getLightIndex( getName() );
      had_light_index.push_back( true );

      if( light_index + 1 <= (GLuint)max_lights ) {
        glPushAttrib( GL_LIGHTING_BIT );
        glEnable( GL_LIGHT0 +light_index );

        RGB col = color->getValue();
        H3DFloat ai = ambientIntensity->getValue();
        H3DFloat i = intensity->getValue();

        GLfloat cola[] = { col.r *i, col.g*i, col.b*i, 1 };
        glLightfv( GL_LIGHT0+light_index, GL_DIFFUSE, cola );
        glLightfv( GL_LIGHT0+light_index, GL_SPECULAR, cola );

        cola[0] = col.r * ai;
        cola[1] = col.g * ai;
        cola[2] = col.b * ai;
        glLightfv( GL_LIGHT0+light_index, GL_AMBIENT, cola );
      }
};

void X3DLightNode::disableGraphicsState() {
  graphics_state_counter--;
  if( had_light_index.back() ) {
    if( global_light_index + 1 <= max_lights ) {
      glPopAttrib();
    }
    global_light_index--;
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
    ti.x3dlightnode_vector.push_back( this );
    global_light_transforms.push_back( ti.getAccForwardMatrix() );
    traverse_sg_counter++;
    act_global = true;
  }
  else
    act_global = false;
}

GLuint X3DLightNode::getLightIndex( string name_for_error ) {
  if( max_lights == -1 ) {
    glGetIntegerv( GL_MAX_LIGHTS, &max_lights );
  }
  global_light_index++;
  if( global_light_index + 1 > max_lights )
      Console(4) << "Warning: Maximum number of lightsources (" << max_lights
                 << ") exceeded. Light will be ignored (" << name_for_error
                 << ")" << endl;
  
  return (GLuint)global_light_index;
}
