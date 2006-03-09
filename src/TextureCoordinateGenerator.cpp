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
/// \file TextureCoordinateGenerator.cpp
/// \brief CPP file for TextureCoordinateGenerator, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "TextureCoordinateGenerator.h"
#include "GL/glew.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TextureCoordinateGenerator::database( 
                        "TextureCoordinateGenerator", 
                        &(newInstance<TextureCoordinateGenerator>), 
                        typeid( TextureCoordinateGenerator ),
                        &X3DTextureCoordinateNode::database );

namespace TextureCoordinateGeneratorInternals {
  FIELDDB_ELEMENT( TextureCoordinateGenerator, mode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureCoordinateGenerator, parameter, INPUT_OUTPUT );
}


TextureCoordinateGenerator::TextureCoordinateGenerator( 
                                     Inst< SFNode   > _metadata,
                                     Inst< SFString > _mode,
                                     Inst< MFFloat  > _parameter ) :
  X3DTextureCoordinateNode( _metadata ),
  mode( _mode ),
  parameter( _parameter ) {

  type_name = "TextureCoordinateGenerator";
  database.initFields( this );
  mode->route( propertyChanged );
  parameter->route( propertyChanged );
  
  mode->setValue( "GL_SPHERE" );
}

void TextureCoordinateGenerator::render( int index ) {
  throw Exception::H3DAPIException("Should not be called", 
                                   H3D_FULL_LOCATION ); 
}

void TextureCoordinateGenerator::renderForTextureUnit( int index,
                                  unsigned int texture_unit ) {
  throw Exception::H3DAPIException("Should not be called", 
                                   H3D_FULL_LOCATION ); 
}

void TextureCoordinateGenerator::renderArray() {
  throw Exception::H3DAPIException("Should not be called", 
                                   H3D_FULL_LOCATION ); 
}


void TextureCoordinateGenerator::renderArrayForTextureUnit( 
                                  unsigned int texture_unit ) {
  throw Exception::H3DAPIException("Should not be called", 
                                   H3D_FULL_LOCATION ); 
}

/// Disable the array state enabled in renderAttay().
void TextureCoordinateGenerator::disableArray() {
  throw Exception::H3DAPIException("Should not be called", 
                                   H3D_FULL_LOCATION ); 
}

void TextureCoordinateGenerator::disableArrayForTextureUnit( unsigned int texture_unit ) {
  throw Exception::H3DAPIException("Should not be called", 
                                   H3D_FULL_LOCATION ); 
}

void TextureCoordinateGenerator::startTexGen() {  
  
  const string &gen_mode = mode->getValue();
  const vector< H3DFloat > &params = parameter->getValue();

  if( gen_mode == "GL_SPHERE" ) {
    glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
    glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
  } else if( gen_mode == "MATRIX" ) {
    if( params.size() < 12 ) {
      stringstream s;
      s << "\"MATRIX\" texture coord generation requires 12 values "
        << "in the parameter field. " << params.size() << " given (in "
        << getName() << " node)" << ends;
      throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
    }
    H3DFloat sparams[4] = {params[0],params[1],params[2],params[3]};
    H3DFloat tparams[4] = {params[4],params[5],params[6],params[7]};
    H3DFloat rparams[4] = {params[8],params[9],params[10],params[11]};
    glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
    glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
    glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
    glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
    glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
    glTexGenfv( GL_R, GL_OBJECT_PLANE, rparams );
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glEnable( GL_TEXTURE_GEN_R );
  } else if( gen_mode == "CAMERASPACEPOSITION" ||
             gen_mode == "COORD-EYE" ) {
    H3DFloat sparams[4] = {1,0,0,0};
    H3DFloat tparams[4] = {0,1,0,0};
    H3DFloat rparams[4] = {0,0,1,0};
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    glTexGenfv( GL_S, GL_EYE_PLANE, sparams );
    glTexGenfv( GL_T, GL_EYE_PLANE, tparams );
    glTexGenfv( GL_R, GL_EYE_PLANE, rparams );
    glPopMatrix();
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glEnable( GL_TEXTURE_GEN_R );
  } else if( gen_mode == "COORD" ) {
    H3DFloat sparams[4] = {1,0,0,0};
    H3DFloat tparams[4] = {0,1,0,0};
    H3DFloat rparams[4] = {0,0,1,0}; 
    glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
    glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
    glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
    glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
    glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
    glTexGenfv( GL_R, GL_OBJECT_PLANE, rparams );
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glEnable( GL_TEXTURE_GEN_R );
  } else if( gen_mode == "CAMERASPACEREFLECTIONVECTOR" ) {
    if( !GLEW_ARB_texture_cube_map ) {
      Console(4) << "Warning: ARB_texture_cube_map extension not supported "
                 << "by your graphics card. \"CAMERASPACEREFLECTIONVECTOR\" "
                 << "mode in \"" << getName() << "\" node cannot be used." 
                 << endl;
    }
    glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB );
    glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB );
    glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB );
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glEnable( GL_TEXTURE_GEN_R );
  } else if( gen_mode == "CAMERASPACENORMAL" ) {
    if( !GLEW_ARB_texture_cube_map ) {
      Console(4) << "Warning: ARB_texture_cube_map extension not supported "
                 << "by your graphics card. \"CAMERASPACENORMAL\" "
                 << "mode in \"" << getName() << "\" node cannot be used." 
                 << endl;
    }
    glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB );
    glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB );
    glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB );
    glEnable( GL_TEXTURE_GEN_S );
    glEnable( GL_TEXTURE_GEN_T );
    glEnable( GL_TEXTURE_GEN_R );
  } else {
    stringstream s;
    s << "Unsupported mode \"" << gen_mode << "\" (in "
        << getName() << " node)" << ends;
      throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
  }


}

void TextureCoordinateGenerator::stopTexGen() {
  glDisable( GL_TEXTURE_GEN_S );
  glDisable( GL_TEXTURE_GEN_T );
  glDisable( GL_TEXTURE_GEN_R );
}
