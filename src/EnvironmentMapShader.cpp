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
/// \file EnvironmentMapShader.cpp
/// \brief CPP file for EnvironmentMapShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/EnvironmentMapShader.h>
#include <H3D/ShaderPart.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase EnvironmentMapShader::database( 
                                   "EnvironmentMapShader", 
                                   &(newInstance<EnvironmentMapShader>), 
                                   typeid( EnvironmentMapShader ),
                                   &H3DGeneratedFragmentShaderNode::database );

namespace EnvironmentMapShaderInternals {
  FIELDDB_ELEMENT( EnvironmentMapShader, environmentMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( EnvironmentMapShader, fresnel, INPUT_OUTPUT );
  FIELDDB_ELEMENT( EnvironmentMapShader, type, INPUT_OUTPUT );
}

EnvironmentMapShader::EnvironmentMapShader( Inst< DisplayList  > _displayList,
                                            Inst< SFNode       > _metadata,
                                            Inst< SFBool       > _isSelected ,
                                            Inst< SFBool       > _isValid,
                                            Inst< SFBool       > _activate,
                                            Inst< SFString     > _language,
                                            Inst< MFShaderPart > _parts,
                                            Inst< SFBool       > _suppressUniformWarnings,
                                            Inst< MFString > _fragmentShaderString,
                                            Inst< MFString > _vertexShaderString,
                                            Inst< SFEnvironmentTextureNode > _environmentMap,
                                            Inst< SFFloat      > _fresnel,
                                            Inst <SFString     > _type ) :
  H3DGeneratedFragmentShaderNode( _displayList, _metadata, _isSelected, 
                                  _isValid, _activate, _language, _parts, 
                                  _suppressUniformWarnings, _fragmentShaderString,
                                  _vertexShaderString ),

  environmentMap( _environmentMap ),
  fresnel( _fresnel ),
  type( _type ) {
  type_name = "EnvironmentMapShader";
  database.initFields( this );

  fresnel->setValue( 0 );
  type->addValidValue( "GLOBAL_SPACE" );
  type->addValidValue( "VIEW_SPACE" );
  type->setValue( "GLOBAL_SPACE" );

  environmentMap->route( displayList, id );
  fresnel->route( displayList, id );
  type->route( displayList, id );

  fresnel->route( rebuildShader, id );
  type->route( rebuildShader, id );
}

/// Get the varying variables used by the shader generator.
void EnvironmentMapShader::getVaryingVariables( vector< VaryingVariable > &varyings ) {
  if( environmentMap->getValue() ) {
    varyings.push_back( VaryingVariable( uniqueShaderName( "normal_local" ),
                                         "vec3",
                                         uniqueShaderName( "normal_local" ) + " = gl_Normal;" ) );
    varyings.push_back( VaryingVariable( uniqueShaderName( "vertex_local" ),
                                         "vec3",
                                         uniqueShaderName( "vertex_local" ) + " = gl_Vertex.xyz;" ) );
    varyings.push_back( VaryingVariable( uniqueShaderName( "eye_pos_local" ),
                                         "vec3",
                                         "vec4 camera_local = gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0); \n"
                                         "camera_local.xyz /= camera_local.w; \n" + 
                                         uniqueShaderName( "eye_pos_local" ) + " = camera_local.xyz;\n" ) );
  }
}


string EnvironmentMapShader::getFragmentShaderString() {
    
  stringstream s;
  
  if( environmentMap->getValue() ) {
    s << "    vec3 normal_local = normalize(" << uniqueShaderName( "normal_local" ) << ");" << endl;
    s << "    vec3 eye_pos_local = " << uniqueShaderName( "eye_pos_local" ) << ";" << endl;
    s << "    vec3 vertex_local = " << uniqueShaderName( "vertex_local" ) << ";" << endl;
    s << "    vec3 eye_vec_local = normalize(vertex_local-eye_pos_local); " << endl;    
    s << "    vec3 reflection_vector_local = reflect( eye_vec_local, normal_local); " << endl;

    const string &t = type->getValue();
    if( t == "VIEW_SPACE" ) {
      s << "    vec3 tex_coord = gl_NormalMatrix * reflection_vector_local; " << endl;
    } else {
      if( t != "GLOBAL_SPACE" ) {
        Console(4) << "Invalid type: \"" << t << "\" in EnvironmentMapShader. Using \"GLOBAL_SPACE\" instead. " << endl;
      }
      s << "    vec3 tex_coord = reflection_vector_local; " << endl;
    }
    s << "    vec4 env_color = textureCube( " << uniqueShaderName( "environment_map" ) 
      << ", tex_coord  ); " << endl;

    H3DFloat r = fresnel->getValue();
    if( r > (H3DFloat) 0.0 ) {
      // calculate fresnel term
      s << "    float R0 = 1.0 - " << uniqueShaderName( "fresnel" ) << ";" << endl;
      s << "    float fresnel = R0 + (1.0-R0) * pow(1.0-abs(dot(-eye_vec_local, normal_local)), 5.0); " << endl;
      s << "    generated_color = env_color * fresnel + generated_color * (1.0-fresnel ); " << endl;
    } else {
      // full reflectivity
      s << "   generated_color = env_color; " << endl;
    }
  }
  return s.str();
}


string EnvironmentMapShader::addUniformFields( ComposedShader *shader ) {
  // add dynamic fields for uniform variables 
  stringstream s;
  
  if( environmentMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "environment_map" ), 
                                     "samplerCube",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( environmentMap ) );
    if( fresnel->getValue() > 0.0 ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "fresnel" ), 
                                       "float",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( fresnel ) );
    }
    
  }
  
  return s.str();
}

