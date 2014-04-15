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
/// \file TwoSidedMaterial.cpp
/// \brief CPP file for TwoSidedMaterial, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TwoSidedMaterial.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TwoSidedMaterial::database( 
                                   "TwoSidedMaterial", 
                                   &(newInstance<TwoSidedMaterial>), 
                                   typeid( TwoSidedMaterial ),
                                   &X3DMaterialNode::database );

namespace TwoSidedMaterialInternals {
  FIELDDB_ELEMENT( TwoSidedMaterial, ambientIntensity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, diffuseColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, emissiveColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, shininess, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, specularColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, transparency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, backAmbientIntensity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, backDiffuseColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, backEmissiveColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, backShininess, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, backSpecularColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, backTransparency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TwoSidedMaterial, separateBackColor, INPUT_OUTPUT );
}

TwoSidedMaterial::TwoSidedMaterial( 
                   Inst< DisplayList > _displayList,
                   Inst< SFFloat > _ambientIntensity,
                   Inst< SFColor > _diffuseColor,
                   Inst< SFColor > _emissiveColor,
                   Inst< SFNode  > _metadata,
                   Inst< SFFloat > _shininess,
                   Inst< SFColor > _specularColor,
                   Inst< SFFloat > _transparency,
                   Inst< SFFloat >  _backAmbientIntensity,
                   Inst< SFColor >  _backDiffuseColor    ,
                   Inst< SFColor >  _backEmissiveColor   ,
                   Inst< SFFloat >  _backShininess       ,
                   Inst< SFColor >  _backSpecularColor   ,
                   Inst< SFFloat >  _backTransparency    ,
                   Inst< SFBool  > _separateBackColor     ) :
  X3DMaterialNode( _displayList, _metadata ),
  ambientIntensity( _ambientIntensity ),
  diffuseColor    ( _diffuseColor     ),
  emissiveColor   ( _emissiveColor    ),
  shininess       ( _shininess        ),
  specularColor   ( _specularColor    ),
  transparency    ( _transparency     ),
  backAmbientIntensity( _backAmbientIntensity ),
  backDiffuseColor    ( _backDiffuseColor     ),
  backEmissiveColor   ( _backEmissiveColor    ),
  backShininess       ( _backShininess        ),
  backSpecularColor   ( _backSpecularColor    ),
  backTransparency    ( _backTransparency     ),
  separateBackColor   ( _separateBackColor ) {

  type_name = "TwoSidedMaterial";
  database.initFields( this );

  ambientIntensity->setValue( 0.2f );
  diffuseColor->setValue( RGB( 0.8f, 0.8f, 0.8f ) );
  emissiveColor->setValue( RGB( 0, 0, 0 ) );
  shininess->setValue( 0.2f );
  specularColor->setValue( RGB( 0, 0, 0 ) );
  transparency->setValue( 0.f );

  backAmbientIntensity->setValue( 0.2f );
  backDiffuseColor->setValue( RGB( 0.8f, 0.8f, 0.8f ) );
  backEmissiveColor->setValue( RGB( 0, 0, 0 ) );
  backShininess->setValue( 0.2f );
  backSpecularColor->setValue( RGB( 0, 0, 0 ) );
  backTransparency->setValue( 0.f );
  separateBackColor->setValue( false );
  
  ambientIntensity->route( displayList );
  diffuseColor->route( displayList );
  emissiveColor->route( displayList );
  shininess->route( displayList );
  specularColor->route( displayList );
  transparency->route( displayList );

  backAmbientIntensity->route( displayList );
  backDiffuseColor->route( displayList );
  backEmissiveColor->route( displayList );
  backShininess->route( displayList );
  backSpecularColor->route( displayList );
  backTransparency->route( displayList );
  separateBackColor->route( displayList );
}


void TwoSidedMaterial::render()  { 
  X3DMaterialNode::render();  

  GLfloat material[4];
  
  H3DFloat t = transparency->getValue();
  material[3] = 1 - t;
  if( isTransparent() ) {
    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  GLenum front_material_face = 
    separateBackColor->getValue() ? GL_FRONT : GL_FRONT_AND_BACK;

  // Ambient
  float intensity = ambientIntensity->getValue(); 
  RGB d = diffuseColor->getValue();
  material[0] = d.r * intensity; 
  material[1] = d.g * intensity; 
  material[2] = d.b * intensity;
  glMaterialfv( front_material_face, GL_AMBIENT, material );
  
  // Specular 
  RGB s = specularColor->getValue(); 
  material[0] = s.r; 
  material[1] = s.g; 
  material[2] = s.b; 
  glMaterialfv( front_material_face, GL_SPECULAR, material );

  // Shininess
  glMaterialf( front_material_face, GL_SHININESS, H3DMin( shininess->getValue()*128, 128.0f ) );
  
  // set the emmisive color.
  RGB e = emissiveColor->getValue();
  material[0] = e.r;
  material[1] = e.g;
  material[2] = e.b;
  glMaterialfv( front_material_face, GL_EMISSION, material );

  // Diffuse
  material[0] = d.r;
  material[1] = d.g;
  material[2] = d.b;

  glMaterialfv( front_material_face, GL_DIFFUSE, material );

  if( separateBackColor->getValue() ) {
    H3DFloat t = backTransparency->getValue();
    material[3] = 1 - t;    

    // Ambient
    intensity = backAmbientIntensity->getValue(); 
    RGB d = backDiffuseColor->getValue();
    material[0] = d.r * intensity; 
    material[1] = d.g * intensity; 
    material[2] = d.b * intensity;
    glMaterialfv( GL_BACK, GL_AMBIENT, material );
  
    // Specular 
    s = backSpecularColor->getValue(); 
    material[0] = s.r; 
    material[1] = s.g; 
    material[2] = s.b; 
    glMaterialfv( GL_BACK, GL_SPECULAR, material );

    // Shininess
    glMaterialf( GL_BACK, GL_SHININESS, 
                 H3DMax( backShininess->getValue()*128, 128.0f ) );
  
    // set the emmisive color.
    e = backEmissiveColor->getValue();
    material[0] = e.r;
    material[1] = e.g;
    material[2] = e.b;
    glMaterialfv( GL_BACK, GL_EMISSION, material );

    // Diffuse
    material[0] = d.r;
    material[1] = d.g;
    material[2] = d.b;

    glMaterialfv( GL_BACK, GL_DIFFUSE, material );
  }
};
