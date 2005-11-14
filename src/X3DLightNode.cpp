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
/// \file X3DLightNode.cpp
/// \brief CPP file for X3DLightNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DLightNode.h"

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
}

GLuint X3DLightNode::global_light_index = 0;

X3DLightNode::X3DLightNode( 
                           Inst< SFNode >  _metadata,
                           Inst< SFFloat>  _ambientIntensity,
                           Inst< SFColor>  _color,
                           Inst< SFFloat>  _intensity,
                           Inst< SFBool >  _on ) :
  X3DChildNode( _metadata ),
  ambientIntensity( _ambientIntensity ),
  color           ( _color            ),
  intensity       ( _intensity        ),
  on              ( _on               ),
  max_lights( -1 ) {

  type_name = "X3DLightNode";
  database.initFields( this );

  displayList->setOwner( this );

  ambientIntensity->setValue( 0 );
  color->setValue( RGB( 1,1,1 ) );
  intensity->setValue( 1 );
  on->setValue( true );

  ambientIntensity->route( displayList );
  color->route( displayList );
  intensity->route( displayList );
  on->route( displayList );

  light_index = ++global_light_index;
}

void X3DLightNode::enableGraphicsState() { 
  if( max_lights == -1 ) {
    glGetIntegerv( GL_MAX_LIGHTS, &max_lights );
    if( light_index + 1 > (GLuint)max_lights )
      cerr << "Warning: Maximum number of lightsources (" << max_lights 
           << ") exceeded. Light will be ignored (" << getName() 
           << ")" << endl;
  }
  
  if( light_index + 1 <= (GLuint)max_lights ) {
    if ( on->getValue() ) {
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
  }
};

void X3DLightNode::disableGraphicsState() { 
  if( light_index + 1 <= (GLuint)max_lights ) {
    glDisable(GL_LIGHT0+light_index);
  }
};

