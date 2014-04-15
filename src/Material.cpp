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
/// \file Material.cpp
/// \brief CPP file for Material, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Material.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Material::database( 
                                   "Material", 
                                   &(newInstance<Material>), 
                                   typeid( Material ),
                                   &X3DMaterialNode::database );

namespace MaterialInternals {
  FIELDDB_ELEMENT( Material, ambientIntensity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Material, diffuseColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Material, emissiveColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Material, shininess, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Material, specularColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Material, transparency, INPUT_OUTPUT );
}

Material::Material( 
                   Inst< DisplayList > _displayList,
                   Inst< SFFloat > _ambientIntensity,
                   Inst< SFColor > _diffuseColor,
                   Inst< SFColor > _emissiveColor,
                   Inst< SFNode  > _metadata,
                   Inst< SFFloat > _shininess,
                   Inst< SFColor > _specularColor,
                   Inst< SFFloat > _transparency ) :
  X3DMaterialNode( _displayList, _metadata ),
  ambientIntensity( _ambientIntensity ),
  diffuseColor    ( _diffuseColor     ),
  emissiveColor   ( _emissiveColor    ),
  shininess       ( _shininess        ),
  specularColor   ( _specularColor    ),
  transparency    ( _transparency     ) {

  type_name = "Material";
  database.initFields( this );

  ambientIntensity->setValue( 0.2f );
  diffuseColor->setValue( RGB( 0.8f, 0.8f, 0.8f ) );
  emissiveColor->setValue( RGB( 0, 0, 0 ) );
  shininess->setValue( 0.2f );
  specularColor->setValue( RGB( 0, 0, 0 ) );
  transparency->setValue( 0.f );

  ambientIntensity->route( displayList );
  diffuseColor->route( displayList );
  emissiveColor->route( displayList );
  shininess->route( displayList );
  specularColor->route( displayList );
  transparency->route( displayList );
}


void Material::render()  { 
  X3DMaterialNode::render();  

  GLfloat material[4];
  
  H3DFloat t = transparency->getValue();
  material[3] = 1 - t;
  if( t > 0 ) {
    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
    
  // Ambient
  float intensity = ambientIntensity->getValue(); 
  RGB d = diffuseColor->getValue();
  material[0] = d.r * intensity; 
  material[1] = d.g * intensity; 
  material[2] = d.b * intensity;
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, material );
  
  // Specular 
  RGB s = specularColor->getValue(); 
  material[0] = s.r; 
  material[1] = s.g; 
  material[2] = s.b; 
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, material );

  // Shininess
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, H3DMin( shininess->getValue()*128, 128.0f ) );
  
  // set the emissive color.
  RGB e = emissiveColor->getValue();
  material[0] = e.r;
  material[1] = e.g;
  material[2] = e.b;
  glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, material );

  // Diffuse
  material[0] = d.r;
  material[1] = d.g;
  material[2] = d.b;

  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material );

};
