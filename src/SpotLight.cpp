//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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

#include <H3D/SpotLight.h>

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
                     Inst< SFBool >  _global,
                     Inst< SFBool  > _on,
                     Inst< SFVec3f > _attenuation,
                     Inst< SFFloat > _beamWidth,
                     Inst< SFFloat > _cutOffAngle,
                     Inst< SFVec3f > _direction,
                     Inst< SFVec3f > _location,
                     Inst< SFFloat > _radius ) :
  X3DLightNode( _metadata,_ambientIntensity, _color, _global,
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
  global->setValue( true );

  direction->route( displayList );
  attenuation->route( displayList );
  beamWidth->route( displayList );
  cutOffAngle->route( displayList );
  location->route( displayList );
  radius->route( displayList );

  type_name = "SpotLight";
}

X3DLightNode::GLLightInfo SpotLight::getGLLightInfo() {
  GLLightInfo light;

  Vec3f p = location->getValue();
  if( act_global ) {
    p = global_light_transforms[ graphics_state_counter ] * p;
  }
  light.position = Vec4f( p, 1 );

  // set attenuation values
  Vec3f att = attenuation->getValue();
  light.constantAttenuation = att.x;
  light.linearAttenuation = att.y;
  light.quadraticAttenuation = att.z;

  RGB col = color->getValue();
  H3DFloat ai = ambientIntensity->getValue();
  H3DFloat i = intensity->getValue();
  
  RGB ambient = col * ai;
  RGB diffuse = col * i;


  Vec3f d = direction->getValue();  
  if( act_global ) {
    d = global_light_transforms[ graphics_state_counter ]
      .getScaleRotationPart() * d;
  }
  // set spotlight specific values
  light.spotDirection = d;

  H3DFloat angle = cutOffAngle->getValue();
  H3DFloat beam = beamWidth->getValue();

  light.spotCutoff = (GLfloat)(angle * 180.0f / Constants::pi );
  // The exponential dropoff is not right/spec compliant...
  light.spotExponent = beam < angle ? 1.0f : 0.0f;
  
  light.diffuse = RGBA( diffuse, 1 );
  light.specular = RGBA( diffuse, 1 );
  light.ambient = RGBA( ambient, 1 );

  return light;
}



