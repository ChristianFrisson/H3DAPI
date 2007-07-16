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
/// \file DirectionalLight.cpp
/// \brief CPP file for DirectionalLight, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <DirectionalLight.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DirectionalLight::database( "DirectionalLight", 
                                            &(newInstance<DirectionalLight>), 
                                            typeid( DirectionalLight ),
                                            &X3DLightNode::database );

namespace DirectionalLightInternals {
  FIELDDB_ELEMENT( DirectionalLight, direction, INPUT_OUTPUT );
}

DirectionalLight::DirectionalLight( 
                                   Inst< SFNode >  _metadata,
                                   Inst< SFFloat>  _ambientIntensity,
                                   Inst< SFColor>  _color,
                                   Inst< SFFloat>  _intensity,
                                   Inst< SFBool >  _on,
                                   Inst< SFVec3f>  _direction ) :
  X3DLightNode( _metadata,_ambientIntensity, _color, _intensity, _on ),
  direction( _direction ) {

  type_name = "DirectionalLight";
  database.initFields( this );

  direction->setValue( Vec3f( 0, 0, -1 ) );
  
  ambientIntensity->route( displayList );
  intensity->route( displayList );
  direction->route( displayList );
  color->route( displayList );
  on->route( displayList );
}

void DirectionalLight::enableGraphicsState() {
  X3DLightNode::enableGraphicsState();
  if( light_index + 1 <= (GLuint) max_lights ) {
    Vec3f p = direction->getValue();
    p = -1.0 * p;
    GLfloat dir[] = { p.x, p.y, p.z, 0 };
    glLightfv( GL_LIGHT0+light_index, GL_POSITION, dir );
  }
}


