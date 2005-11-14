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
/// \file PointLight.cpp
/// \brief CPP file for PointLight, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "PointLight.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PointLight::database( 
                                     "PointLight", 
                                     &(newInstance<PointLight>), 
                                     typeid( PointLight ),
                                     &X3DLightNode::database );

namespace PointLightInternals {
  FIELDDB_ELEMENT( PointLight, attenuation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointLight, location, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointLight, radius, INPUT_OUTPUT );
}


PointLight::PointLight( 
                       Inst< SFNode  > _metadata,
                       Inst< SFFloat > _ambientIntensity,
                       Inst< SFColor > _color,
                       Inst< SFFloat > _intensity,
                       Inst< SFBool  > _on,
                       Inst< SFVec3f > _attenuation,
                       Inst< SFVec3f > _location,
                       Inst< SFFloat > _radius ) :
  X3DLightNode( _metadata,_ambientIntensity, _color, _intensity, _on ),
  attenuation     ( _attenuation      ),
  location        ( _location         ),
  radius          ( _radius           ) {

  type_name = "PointLight";
  database.initFields( this );

  attenuation->setValue( Vec3f( 1, 0, 0 ) );
  location->setValue( Vec3f( 0, 0, 0 ) );
  radius->setValue( 100 );

  attenuation->route( displayList );
  location->route( displayList );
  radius->route( displayList );
}

void PointLight::enableGraphicsState() {
  X3DLightNode::enableGraphicsState();
  if( light_index + 1 <= (GLuint)max_lights ) {
    
    Vec3f p = location->getValue();
    GLfloat pos_v[] = { p.x, p.y, p.z, 1 };
    glLightfv( GL_LIGHT0+light_index, GL_POSITION, pos_v );
    
    // set attenuation values
    Vec3f att = attenuation->getValue();
    glLightf(GL_LIGHT0+light_index, GL_CONSTANT_ATTENUATION, att.x );
    glLightf(GL_LIGHT0+light_index, GL_LINEAR_ATTENUATION, att.y );
    glLightf(GL_LIGHT0+light_index, GL_QUADRATIC_ATTENUATION, att.z );
  }
}
