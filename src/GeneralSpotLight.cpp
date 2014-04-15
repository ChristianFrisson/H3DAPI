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
/// \file GeneralSpotLight.cpp
/// \brief CPP file for GeneralSpotLight, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeneralSpotLight.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GeneralSpotLight::database( 
                                    "GeneralSpotLight", 
                                    &(newInstance<GeneralSpotLight>), 
                                    typeid( GeneralSpotLight ),
                                    &SpotLight::database );

namespace GeneralSpotLightInternals {
  FIELDDB_ELEMENT( GeneralSpotLight, ambientColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GeneralSpotLight, specularColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GeneralSpotLight, specularIntensity, INPUT_OUTPUT );
}


GeneralSpotLight::GeneralSpotLight( 
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
                     Inst< SFFloat > _radius,
                     Inst< SFColor>  _ambientColor,
                     Inst< SFColor>  _specularColor,
                     Inst< SFFloat > _specularIntensity  ) :
  SpotLight( _metadata,_ambientIntensity, _color, _intensity,
                _global, _on, _attenuation, _beamWidth,
    _cutOffAngle, _direction, _location, _radius ),
  ambientColor    ( _ambientColor     ),
  specularColor   ( _specularColor    ),
  specularIntensity( _specularIntensity ) {

  type_name = "GeneralSpotLight";
  database.initFields( this );

  ambientColor->setValue( RGB( 1, 1, 1 ) );
  specularColor->setValue( RGB( 1, 1, 1 ) );
  specularIntensity->setValue( 1 );

  ambientColor->route( displayList );
  specularColor->route( displayList );
  specularIntensity->route( displayList );

}

X3DLightNode::GLLightInfo GeneralSpotLight::getGLLightInfo() {
  
  GLLightInfo light = SpotLight::getGLLightInfo();

  RGB ac = ambientColor->getValue();
  RGB sc = specularColor->getValue();
  H3DFloat ai = ambientIntensity->getValue();
  H3DFloat si = intensity->getValue();
 
  light.specular = RGBA( sc*si, 1 );
  light.ambient = RGBA( ac*ai, 1 );

  return light;
}



