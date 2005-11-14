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
/// \file SpotLight.cpp
/// \brief CPP file for SpotLight, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "SpotLight.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SpotLight::database( 
                                    "SpotLight", 
                                    &(newInstance<SpotLight>), 
                                    typeid( SpotLight ),
                                    &X3DLightNode::database );

namespace SpotLightInternals {
  FIELDDB_ELEMENT( SpotLight, attenuation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpotLight, beamWidth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpotLight, cutOffAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpotLight, direction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpotLight, location, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SpotLight, radius, INPUT_OUTPUT );
}


SpotLight::SpotLight( 
                     Inst< SFNode  > _metadata,
                     Inst< SFFloat > _ambientIntensity,
                     Inst< SFColor > _color,
                     Inst< SFFloat > _intensity,
                     Inst< SFBool  > _on,
                     Inst< SFVec3f > _attenuation,
                     Inst< SFFloat > _beamWidth,
                     Inst< SFFloat > _cutOffAngle,
                     Inst< SFVec3f > _direction,
                     Inst< SFVec3f > _location,
                     Inst< SFFloat > _radius ) :
  X3DLightNode( _metadata,_ambientIntensity, _color, 
                _intensity, _on ),
  attenuation     ( _attenuation      ),
  beamWidth       ( _beamWidth        ),
  cutOffAngle     ( _cutOffAngle      ),
  direction       ( _direction        ),
  location        ( _location         ),
  radius          ( _radius           ) {

  type_name = "SpotLight";
  database.initFields( this );

  attenuation->setValue( Vec3f( 1, 0, 0 ) );
  beamWidth->setValue((H3DFloat)(Constants::pi/2));
  cutOffAngle->setValue((H3DFloat)(Constants::pi/4));
  direction->setValue( Vec3f( 0, 0, -1 ) );
  location->setValue( Vec3f( 0, 0, 0 ) );
  radius->setValue( 100 );

  direction->route( displayList );
  attenuation->route( displayList );
  beamWidth->route( displayList );
  cutOffAngle->route( displayList );
  location->route( displayList );
  radius->route( displayList );

  type_name = "SpotLight";
}

void SpotLight::enableGraphicsState() {
  X3DLightNode::enableGraphicsState();
  if( light_index + 1 <= (GLuint)max_lights ) {
    
    // location
    Vec3f l = location->getValue();
    GLfloat loc_v[] = { l.x, l.y, l.z, 1 };
    glLightfv( GL_LIGHT0+light_index, GL_POSITION, loc_v );
    
    // set attenuation values
    Vec3f att = attenuation->getValue();
    glLightf(GL_LIGHT0+light_index, GL_CONSTANT_ATTENUATION, att.x );
    glLightf(GL_LIGHT0+light_index, GL_LINEAR_ATTENUATION, att.y );
    glLightf(GL_LIGHT0+light_index, GL_QUADRATIC_ATTENUATION, att.z );
    
    // direction
    Vec3f d = direction->getValue();  
    GLfloat dir_v[] = { d.x, d.y, d.z };
    glLightfv( GL_LIGHT0+light_index, GL_SPOT_DIRECTION, dir_v );
    
    H3DFloat angle = cutOffAngle->getValue();
    H3DFloat beam = beamWidth->getValue();
    glLightf( GL_LIGHT0+light_index, GL_SPOT_CUTOFF, 
              (GLfloat)(angle * 180.0f / Constants::pi ));
    // The exponential dropoff is not right/spec compliant...
    glLightf( GL_LIGHT0+light_index, GL_SPOT_EXPONENT, 
              beam < angle ? 1.0f : 0.0f );
  }
}


