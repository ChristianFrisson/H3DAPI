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
/// \file PointLight.cpp
/// \brief CPP file for PointLight, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PointLight.h>
#include <H3D/ShadowCaster.h>

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
                       Inst< SFBool >  _global,
                       Inst< SFFloat > _radius ) :
  X3DLightNode( _metadata,_ambientIntensity, _color, _global,
                _intensity, _on ),
  attenuation     ( _attenuation      ),
  location        ( _location         ),
  radius          ( _radius           ) {

  type_name = "PointLight";
  database.initFields( this );

  attenuation->setValue( Vec3f( 1, 0, 0 ) );
  location->setValue( Vec3f( 0, 0, 0 ) );
  global->setValue( true );
  radius->setValue( 100 );

  attenuation->route( displayList );
  location->route( displayList );
  radius->route( displayList );
}


X3DLightNode::GLLightInfo PointLight::getGLLightInfo() {
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
  
  light.diffuse = RGBA( diffuse, 1 );
  light.specular = RGBA( diffuse, 1 );
  light.ambient = RGBA( ambient, 1 );

  return light;
}

void PointLight::traverseSG( TraverseInfo &ti ) {
  X3DLightNode::traverseSG( ti );
  if( shadows->getValue() ) {
    ShadowCaster *shadow_caster = NULL;
    if( !ti.getUserData( "ShadowCaster",  (void **)&shadow_caster) ) {
      PointLight *light = new PointLight;
      light->location->setValue( ti.getAccForwardMatrix() * location->getValue() );
      shadow_caster->light->push_back( light );
    }
  }
}
