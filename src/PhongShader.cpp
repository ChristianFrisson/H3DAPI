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
/// \file PhongShader.cpp
/// \brief CPP file for PhongShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PhongShader.h>
#include <H3D/ShaderPart.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PhongShader::database( 
                                   "PhongShader", 
                                   &(newInstance<PhongShader>), 
                                   typeid( PhongShader ),
                                   &H3DGeneratedFragmentShaderNode::database );

namespace PhongShaderInternals {
  FIELDDB_ELEMENT( PhongShader, normalMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, normalMapCoordSpace, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, normalMapMatrix, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, ambientMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, diffuseMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, emissionMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, specularMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, glossMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, fresnel, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, modulateMaps, INPUT_OUTPUT );

  FIELDDB_ELEMENT( PhongShader, backNormalMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backNormalMapCoordSpace, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backNormalMapMatrix, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backAmbientMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backDiffuseMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backEmissionMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backSpecularMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backGlossMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backFresnel, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backModulateMaps, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, separateBackColor, INPUT_OUTPUT );
}

PhongShader::PhongShader( Inst< DisplayList  > _displayList,
                          Inst< SFNode       > _metadata,
                          Inst< SFBool       > _isSelected ,
                          Inst< SFBool       > _isValid,
                          Inst< SFBool       > _activate,
                          Inst< SFString     > _language,
                          Inst< MFShaderPart > _parts,
                          Inst< SFBool       > _suppressUniformWarnings,
                          Inst< MFString > _fragmentShaderString,
                          Inst< MFString > _vertexShaderString,
                          Inst< SFTexture2DNode > _ambientMap,
                          Inst< SFTexture2DNode > _diffuseMap,
                          Inst< SFTexture2DNode > _emissionMap,
                          Inst< SFTexture2DNode > _normalMap,
                          Inst< SFString        > _normalMapCoordSpace,
                          Inst< SFMatrix4f        > _normalMapMatrix,
                          Inst< SFTexture2DNode > _specularMap,
                          Inst< SFTexture2DNode > _glossMap,
                          Inst< SFFloat         > _fresnel,
                          Inst< SFBool          > _modulateMaps,
                          Inst< SFTexture2DNode > _backAmbientMap ,
                          Inst< SFTexture2DNode > _backDiffuseMap ,
                          Inst< SFTexture2DNode > _backEmissionMap ,
                          Inst< SFTexture2DNode > _backNormalMap  ,
                          Inst< SFString        > _backNormalMapCoordSpace,
                          Inst< SFMatrix4f      > _backNormalMapMatrix,
                          Inst< SFTexture2DNode > _backSpecularMap,
                          Inst< SFTexture2DNode > _backGlossMap   ,
                          Inst< SFFloat         > _backFresnel,
                          Inst< SFBool          > _backModulateMaps,
                          Inst< SFBool          > _separateBackColor ) :
  H3DGeneratedFragmentShaderNode( _displayList, _metadata, _isSelected, 
                                  _isValid, _activate, _language, _parts, 
                                  _suppressUniformWarnings, _fragmentShaderString,
                                  _vertexShaderString ),

  ambientMap( _ambientMap ),
  diffuseMap( _diffuseMap ),
  emissionMap( _emissionMap ),
  normalMap( _normalMap ),
  normalMapCoordSpace( _normalMapCoordSpace ),
  normalMapMatrix( _normalMapMatrix ),
  specularMap( _specularMap ),
  glossMap( _glossMap ),
  fresnel( _fresnel ),
  modulateMaps( _modulateMaps ),
  backAmbientMap( _backAmbientMap ),
  backDiffuseMap( _backDiffuseMap ),
  backEmissionMap( _backEmissionMap ),
  backNormalMap( _backNormalMap ),
  backNormalMapCoordSpace( _backNormalMapCoordSpace ),
  backNormalMapMatrix( _backNormalMapMatrix ),
  backSpecularMap( _backSpecularMap ),
  backGlossMap( _backGlossMap ),
  backFresnel( _backFresnel ),
  backModulateMaps( _backModulateMaps ),
  separateBackColor( _separateBackColor ) {

  type_name = "PhongShader";
  database.initFields( this );

  normalMapCoordSpace->addValidValue( "OBJECT" );
  normalMapCoordSpace->addValidValue( "TANGENT" );
  normalMapCoordSpace->setValue( "OBJECT" );
  
  normalMapMatrix->setValue( Matrix4f( 2, 0, 0, -1,
                                       0, 2, 0, -1,
                                       0, 0, 2, -1,
                                       0, 0, 0, 1 ) );

  ambientMap->route( displayList, id );
  diffuseMap->route( displayList, id );
  emissionMap->route( displayList, id );
  normalMap->route( displayList, id );
  normalMapCoordSpace->route( displayList, id );
  specularMap->route( displayList, id );
  glossMap->route( displayList, id );
  normalMapMatrix->route( displayList, id );
  modulateMaps->route( displayList, id );
  fresnel->route( displayList, id );

  backAmbientMap->route( displayList, id );
  backDiffuseMap->route( displayList, id );
  backEmissionMap->route( displayList, id );
  backNormalMap->route( displayList, id );
  backNormalMapCoordSpace->route( displayList, id );
  backSpecularMap->route( displayList, id );
  backGlossMap->route( displayList, id );
  backNormalMapMatrix->route( displayList, id );
  backModulateMaps->route( displayList, id );
  backFresnel->route( displayList, id );
  separateBackColor->route( displayList, id );

  ambientMap->route( rebuildShader, id );
  diffuseMap->route( rebuildShader, id );
  emissionMap->route( rebuildShader, id );
  normalMap->route( rebuildShader, id );
  normalMapCoordSpace->route( rebuildShader, id );
  specularMap->route( rebuildShader, id );
  glossMap->route( rebuildShader, id );
  normalMapMatrix->route( rebuildShader );
  fresnel->route( rebuildShader, id );
  modulateMaps->route( rebuildShader );

  backAmbientMap->route( rebuildShader, id );
  backDiffuseMap->route( rebuildShader, id );
  backEmissionMap->route( rebuildShader, id );
  backNormalMap->route( rebuildShader, id );
  backNormalMapCoordSpace->route( rebuildShader, id );
  backSpecularMap->route( rebuildShader, id );
  backGlossMap->route( rebuildShader, id );
  backNormalMapMatrix->route( rebuildShader );
  backFresnel->route( rebuildShader, id );
  backModulateMaps->route( rebuildShader );
  separateBackColor->route( rebuildShader );

  modulateMaps->setValue( false );
  backModulateMaps->setValue( false );
  separateBackColor->setValue( false );
  fresnel->setValue( 0 );
  backFresnel->setValue( 0 );
}


string fragment_shader_functions = 

"float calculateAttenuation(in int i, in float dist) {\n"
"  return(1.0 / (gl_LightSource[i].constantAttenuation +\n"
"                gl_LightSource[i].linearAttenuation * dist +\n"
"                gl_LightSource[i].quadraticAttenuation * dist * dist));\n"
"}\n"
"\n"
"// Returns the color generated by one directional light using the Phong lighting model.\n"
"// i is the index of the light\n"
"vec4 directionalLightPhong(in int i, \n"
"                           in vec3 normal, \n"
"                           in float material_shininess,\n"
"                           in vec4 material_ambient, \n"
"                           in vec4 material_diffuse, \n"
"                           in vec4 material_specular) {\n"
"  \n"
"  vec4 ambient  = vec4( 0.0 );\n"
"  vec4 specular = vec4( 0.0 );\n"
"  vec4 diffuse  = vec4( 0.0 );\n"
"\n"
"  vec3 light_dir = normalize(gl_LightSource[i].position.xyz);\n"
"   \n"
"  // cos angle between N and L\n"
"  float nDotL = dot(normal, light_dir);\n"
"   \n"
"  if (nDotL > 0.0) {   \n"
"    vec3 H = gl_LightSource[i].halfVector.xyz;\n"
"    float pf = pow(max(dot(normal,H), 0.0), material_shininess);\n"
"    diffuse  += material_diffuse * gl_LightSource[i].diffuse  * nDotL;\n"
"    specular += material_specular * gl_LightSource[i].specular * pf;\n"
"  }\n"
"  ambient  += material_ambient * gl_LightSource[i].ambient;\n"
"\n"
"  return ambient + specular + diffuse;\n"
"}\n"
"\n"
"// Returns the color generated by one point light using the Phong lighting model.\n"
"// i is the index of the light\n"
"vec4 pointLightPhong(in int i, \n"
"                     in vec3 normal, \n"
"                     in vec3 vertex, \n"
"                     in float material_shininess,\n"
"                     in vec4 material_ambient, \n"
"                     in vec4 material_diffuse, \n"
"                     in vec4 material_specular) {\n"
"  vec4 ambient  = vec4( 0.0 );\n"
"  vec4 specular = vec4( 0.0 );\n"
"  vec4 diffuse  = vec4( 0.0 );\n"
"\n"
"  vec3 D = gl_LightSource[i].position.xyz - vertex;\n"
"  vec3 L = normalize(D);\n"
"  float dist = length(D);\n"
"  float attenuation = calculateAttenuation(i, dist);\n"
"  float nDotL = dot(normal,L);\n"
"  \n"
"  if (nDotL > 0.0) {   \n"
"    vec3 E = normalize(-vertex);\n"
"    vec3 R = reflect(-L, normal);\n"
"    float pf = pow(max(dot(R,E), 0.0), material_shininess);\n"
"    diffuse  += material_diffuse * gl_LightSource[i].diffuse  * attenuation * nDotL;\n"
"    specular += material_specular * gl_LightSource[i].specular * attenuation * pf;\n"
"  }\n"
"  ambient += material_ambient * gl_LightSource[i].ambient * attenuation;\n"
"\n"
"  return ambient + specular + diffuse;\n"
"}\n"
"\n"
"\n"
"// Returns the color generated by one spot light using the Phong lighting model.\n"
"// i is the index of the light\n"
"vec4 spotLightPhong( in int i, \n"
"                     in vec3 normal, \n"
"                     in vec3 vertex, \n"
"                     in float material_shininess,\n"
"                     in vec4 material_ambient, \n"
"                     in vec4 material_diffuse, \n"
"                     in vec4 material_specular ) {\n"
"  vec4 ambient  = vec4( 0.0 );\n"
"  vec4 specular = vec4( 0.0 );\n"
"  vec4 diffuse  = vec4( 0.0 );\n"
"\n"
"  vec3 D = gl_LightSource[i].position.xyz - vertex;\n"
"  vec3 L = normalize(D);\n"
"  \n"
"  float dist = length(D);\n"
"  float attenuation = calculateAttenuation(i, dist);\n"
"\n"
"  float nDotL = dot(normal,L);\n"
"\n"
"  if (nDotL > 0.0) {   \n"
"    float spotEffect = dot(normalize(gl_LightSource[i].spotDirection), -L);\n"
"    \n"
"    if (spotEffect > gl_LightSource[i].spotCosCutoff) {\n"
"      attenuation *=  pow(spotEffect, gl_LightSource[i].spotExponent);\n"
"      \n"
"      vec3 E = normalize(-vertex);\n"
"      vec3 R = reflect(-L, normal);\n"
"      \n"
"      float pf = pow(max(dot(R,E), 0.0), material_shininess);\n"
"      \n"
"      diffuse  += material_diffuse * gl_LightSource[i].diffuse  * attenuation * nDotL;\n"
"      specular += material_specular * gl_LightSource[i].specular * attenuation * pf;\n"
"    }\n"
"  }\n"
"  ambient  += material_ambient * gl_LightSource[i].ambient * attenuation;\n"
"  return ambient + specular + diffuse;\n"
"}\n"
"\n"
"\n"
"// Returns the color generated by one light using the Phong lighting model.\n"
"// It will choose one of directionalLightPhong, pointLightPhong or \n"
"// spotLightPhong depending on what kind of light it is.\n"
"// i is the index of the light\n"
"vec4 lightPhong(in int i, \n"
"                in vec3 normal, \n"
"                in vec3 vertex, \n"
"                in float shininess,\n"
"                in vec4 ambient, \n"
"                in vec4 diffuse, \n"
"                in vec4 specular) {\n"
"  if (gl_LightSource[i].position.w == 0.0)\n"
"    return directionalLightPhong(i, normal, shininess, ambient, diffuse, specular);\n"
"  else if (gl_LightSource[i].spotCutoff == 180.0)\n"
"    return pointLightPhong(i, normal, vertex, shininess, ambient, diffuse, specular);\n"
"  else\n"
"    return spotLightPhong(i, normal, vertex, shininess, ambient, diffuse, specular);\n"
  "}\n";


/// Get a string with GLSL function definitions to be used by
/// the generated shader.
string PhongShader::getFunctionShaderString() {
  return fragment_shader_functions;
}

void PhongShader::getAttributes( vector< Attribute > &attributes ) {
  if( normalMap->getValue() && normalMapCoordSpace->getValue() == "TANGENT" ) {
    bool have_binormal = false;
    bool have_tangent = false;
    for( unsigned int i = 0; i < attributes.size(); i++ ) {
      if( attributes[i].name == "binormal" ) have_binormal = true;
      else if( attributes[i].name == "tangent" ) have_tangent = true;
    }
    if( !have_binormal )
      attributes.push_back( Attribute( "binormal", "vec3" ) );

    if( !have_tangent )
      attributes.push_back( Attribute( "tangent", "vec3" ) );
  }
}

/// Get the varying variables used by the shader generator.
void PhongShader::getVaryingVariables( vector< VaryingVariable > &varyings ) {
  varyings.push_back( VaryingVariable( uniqueShaderName( "normal" ),
                                       "vec3",
                                       uniqueShaderName( "normal" ) + " = gl_NormalMatrix * gl_Normal;" ) );
  varyings.push_back( VaryingVariable( uniqueShaderName( "vertex" ),
                                       "vec3",
                                        uniqueShaderName( "vertex" ) + " = vec3( gl_ClipVertex );" ) );

  if( normalMap->getValue() && normalMapCoordSpace->getValue() == "TANGENT" ) {
    varyings.push_back( VaryingVariable( uniqueShaderName( "binormal_axis" ),
                                         "vec3",
                                         uniqueShaderName( "binormal_axis" ) + " = gl_NormalMatrix * binormal;" ) );
    varyings.push_back( VaryingVariable( uniqueShaderName( "tangent_axis" ),
                                         "vec3",
                                         uniqueShaderName( "tangent_axis" ) + " = gl_NormalMatrix * tangent;" ) );
  }
}


string PhongShader::getFragmentShaderString() {
  stringstream s;
  
  s << "    vec3 normal = " << uniqueShaderName( "normal" ) << ";" << endl;
  s << "    vec3 vertex = " << uniqueShaderName( "vertex" ) << ";" << endl;

  s << "    vec3 orig_normal = normalize( normal ); \n" << endl;

  bool modulate = modulateMaps->getValue();
  bool back_modulate = backModulateMaps->getValue();

  // add fresnel term if fresnel is used
  if( fresnel->getValue() > 0 || (separateBackColor->getValue()  && backFresnel->getValue() > 0 ) ){
    s << "    float fresnel = pow(1.0-abs(dot(normalize(-vertex), normal)), 5.0);" << endl;
  }

  if( diffuseMap->getValue() ) {
    if( modulate ) {
      s << "    vec4 diffuse_color = gl_FrontMaterial.diffuse * texture2D( "
        << uniqueShaderName( "diffuse_map" ) 
        << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    vec4 diffuse_color = texture2D( " << uniqueShaderName( "diffuse_map" ) << ", gl_TexCoord[0].st );" << endl;
      // if the texture contains alpha use the texture alpha, otherwise use the 
      // one from the material diffuse color which is set by the transparency 
      // field in a Material node.
      Image *image = diffuseMap->getValue()->image->getValue();
      if( image && 
          image->pixelType() != Image::LUMINANCE_ALPHA &&
          image->pixelType() != Image::RGBA &&
          image->pixelType() != Image::BGRA) {
        s << "    diffuse_color.a = gl_FrontMaterial.diffuse.a;" << endl;
      }
    }
  } else {
    s << "    vec4 diffuse_color = gl_FrontMaterial.diffuse; " << endl;
  }

  if( emissionMap->getValue() ) {
    if( modulate ) {
      s << "    vec4 emission_color = gl_FrontMaterial.emission * texture2D( "
        << uniqueShaderName( "emission_map" ) << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    vec4 emission_color = texture2D( " << uniqueShaderName( "emission_map" ) << ", gl_TexCoord[0].st );" << endl;
    }
  } else {
    s << "    vec4 emission_color = gl_FrontMaterial.emission; " << endl;
  }

  if( ambientMap->getValue() ) {
    if( modulate ) {
      // in Material ambient color is ambientIntensity * diffuseColor. We modulate with the ambientIntensity
      // value
      s << "    float intensity = 0.0; " << endl;
      s << "    if( gl_FrontMaterial.diffuse.r != 0.0 ) { " << endl;
      s << "       intensity = gl_FrontMaterial.ambient.r / gl_FrontMaterial.diffuse.r;" << endl;
      s << "    } else if( gl_FrontMaterial.diffuse.g != 0.0 ) { " << endl;
      s << "       intensity = gl_FrontMaterial.ambient.g / gl_FrontMaterial.diffuse.g;" << endl;
      s << "    } else if( gl_FrontMaterial.diffuse.b != 0.0 ) { " << endl;
      s << "       intensity = gl_FrontMaterial.ambient.b / gl_FrontMaterial.diffuse.b;" << endl;
      s << "    } " << endl;
      s << "    vec4 ambient_color = intensity * texture2D( " << uniqueShaderName( "ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    vec4 ambient_color = texture2D( " << uniqueShaderName( "ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
    }
  } else {
    s << "    vec4 ambient_color = gl_FrontMaterial.ambient; " << endl;
  }

  if( specularMap->getValue() ) {
    if( modulate ) {
      s << "    vec4 specular_color = gl_FrontMaterial.specular * texture2D( " << uniqueShaderName( "specular_map" ) << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    vec4 specular_color = texture2D( " << uniqueShaderName( "specular_map" ) << ", gl_TexCoord[0].st );" << endl;
    }
  } else {
    s << "    vec4 specular_color = gl_FrontMaterial.specular; " << endl;
  }

  // saturate specular color based on fresnel effect
  if( fresnel->getValue() > 0 ) {
    s << "    float f = " << uniqueShaderName( "fresnel" ) << ";" << endl;
    s << "    specular_color *= 1-f + f * fresnel; " << endl;
  }

  if( glossMap->getValue() ) {
    if( modulate ) {
      s << "    float shininess = gl_FrontMaterial.shininess * texture2D( " << uniqueShaderName( "gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
    } else {
      s << "    float shininess = texture2D( " << uniqueShaderName( "gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
    }
  } else {
    s << "    float shininess = gl_FrontMaterial.shininess;" << endl;
  }

  if( normalMap->getValue() ) {

    string coord_space = normalMapCoordSpace->getValue();
    if( coord_space != "OBJECT" && coord_space != "TANGENT" ) {
      Console(4) << "Invalid normalMapCoordSpace value in PhongShader node: \"" 
                 << coord_space << "\". Using \"OBJECT\" instead." << endl;
      coord_space == "OBJECT";
    }

    s << "    mat4 normal_map_matrix = " << uniqueShaderName( "normal_map_matrix" ) << ";" << endl;

    s << "    vec3 N = texture2D( " << uniqueShaderName( "normal_map" ) << ", gl_TexCoord[0].st ).xyz;" << endl;
    
    // texture values [0-1] -> object space normal [-1,1]
    s << "    N = vec3( normal_map_matrix * vec4( N, 1.0 ) );" << endl;
    
    if( coord_space == "TANGENT" ) {
      // from tangent to global space
      s << "    mat3 tangent_space_matrix = mat3( " << uniqueShaderName("tangent_axis") <<", " << uniqueShaderName("binormal_axis") << ", orig_normal ); " << endl;
      s << "    N = tangent_space_matrix * N; " << endl;
    } else {
      // from object to global space
      s << "    N = vec3( gl_ModelViewMatrix * vec4(N,0.0) );" << endl;
    }
    s << "    N = normalize( N ); " << endl;
  } else {
    s << "    vec3 N = orig_normal;\n" << endl;
  }

  // back colors
  if( separateBackColor->getValue() ) {
    if( backDiffuseMap->getValue() ) {
      if( back_modulate ) {
        s << "    vec4 back_diffuse_color = gl_BackMaterial.diffuse * texture2D( " << uniqueShaderName( "back_diffuse_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    vec4 back_diffuse_color = texture2D( " << uniqueShaderName( "back_diffuse_map" ) << ", gl_TexCoord[0].st );" << endl;
        // if the texture contains alpha use the texture alpha, otherwise use the 
        // one from the material diffuse color which is set by the transparency 
        // field in a Material node.
        Image *image = backDiffuseMap->getValue()->image->getValue();
        if( image && 
            image->pixelType() != Image::LUMINANCE_ALPHA &&
            image->pixelType() != Image::RGBA &&
            image->pixelType() != Image::BGRA) {
          s << "    diffuse_color.a = gl_BackMaterial.diffuse.a;" << endl;
        }
      }
    } else {
      s << "    vec4 back_diffuse_color = gl_BackMaterial.diffuse; " << endl;
    }
    
    if( backEmissionMap->getValue() ) {
      if( back_modulate ) {
        s << "    vec4 back_emission_color = gl_BackMaterial.emission * texture2D( " << uniqueShaderName( "back_emission_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    vec4 back_emission_color = texture2D( " << uniqueShaderName( "back_emission_map" ) << ", gl_TexCoord[0].st );" << endl;
      }
    } else {
      s << "    vec4 back_emission_color = gl_BackMaterial.emission; " << endl;
    }
    
    if( backAmbientMap->getValue() ) {
      if( back_modulate ) {
        // in Material ambient color is ambientIntensity * diffuseColor. We modulate with the ambientIntensity
        // value
        s << "    float back_intensity = 0.0; " << endl;
        s << "    if( gl_BackMaterial.diffuse.r != 0.0 ) { " << endl;
        s << "       back_intensity = gl_BackMaterial.ambient.r / gl_BackMaterial.diffuse.r;" << endl;
        s << "    } else if( gl_BackMaterial.diffuse.g != 0.0 ) { " << endl;
        s << "       back_intensity = gl_BackMaterial.ambient.g / gl_BackMaterial.diffuse.g;" << endl;
        s << "    } else if( gl_BackMaterial.diffuse.b != 0.0 ) { " << endl;
        s << "       back_intensity = gl_BackMaterial.ambient.b / gl_BackMaterial.diffuse.b;" << endl;
        s << "    } " << endl;
        s << "    vec4 back_ambient_color = back_intensity * texture2D( " << uniqueShaderName( "back_ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    vec4 back_ambient_color = texture2D( " << uniqueShaderName( "back_ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
      }
    } else {
      s << "    vec4 back_ambient_color = gl_BackMaterial.ambient; " << endl;
    }
    
    if( backSpecularMap->getValue() ) {
      if( back_modulate ) {
        s << "    vec4 back_specular_color = gl_BackMaterial.specular * texture2D( " << uniqueShaderName( "back_specular_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    vec4 back_specular_color = texture2D( " << uniqueShaderName( "back_specular_map" ) << ", gl_TexCoord[0].st );" << endl;
      }
    } else {
      s << "    vec4 back_specular_color = gl_BackMaterial.specular; " << endl;
    }

    // saturate specular color based on fresnel effect
    if( backFresnel->getValue() > 0 ) {
      s << "    float bf = " << uniqueShaderName( "backFresnel" ) << ";" << endl;
      s << "    back_specular_color *= 1-bf + bf * fresnel; " << endl;
    }
    
    if( backGlossMap->getValue() ) {
      if( back_modulate ) {
        s << "    float back_shininess = gl_BackMaterial.shininess * texture2D( " << uniqueShaderName( "back_gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
      } else {
        s << "    float back_shininess = texture2D( " << uniqueShaderName( "back_gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
      }
    } else {
      s << "    float back_shininess = gl_BackMaterial.shininess;" << endl;
    }
    
    if( backNormalMap->getValue() ) {
      s << "    mat4 back_normal_map_matrix = " << uniqueShaderName( "back_normal_map_matrix" ) << ";" << endl;

      s << "    vec3 back_N = texture2D( " << uniqueShaderName( "back_normal_map" ) << ", gl_TexCoord[0].st ).xyz;" << endl;
      
      // texture values [0-1] -> object space normal [-1,1]
      s << "    back_N = vec3( back_normal_map_matrix * vec4( back_N, 1.0 ) );" << endl;
      
      // to global space
      s <<      "back_N = vec3( gl_ModelViewMatrix * vec4(back_N,0.0) );" << endl;
      s <<      "back_N = -normalize( back_N ); " << endl;
    } else {
      s << "  vec3 back_N = -orig_normal;\n" << endl;
    }
  } else {
    // separateBackColor is false, just use the front colors as back colors.
    s << "  vec4 back_ambient_color = ambient_color;\n" << endl;
    s << "  vec4 back_diffuse_color = diffuse_color;\n" << endl;
    s << "  vec4 back_specular_color = specular_color;\n" << endl;
    s << "  vec4 back_emission_color = emission_color;\n" << endl;
    s << "  float back_shininess = shininess;\n" << endl;
    s << "  vec3 back_N = -N;\n" << endl;
  }

  // TODO: support several lights
  s <<
    "  vec4 final_color = vec4( 0.0, 0.0, 0.0, 1.0 );\n"
    "  vec3 view_dir = normalize(-vertex);\n"
    "  bool is_back_face = (dot(orig_normal, view_dir) < 0.0);\n"
    "  if( is_back_face ) { \n"
    "    final_color += lightPhong( 0, back_N, vertex, back_shininess, back_ambient_color, back_diffuse_color, back_specular_color );\n"
    "    final_color += back_emission_color;\n"
    "    final_color.a = back_diffuse_color.a;\n"
    "  } else { \n"
    "    final_color += lightPhong( 0, N, vertex, shininess, ambient_color, diffuse_color, specular_color );\n"
    "    final_color += emission_color;\n"
    "    final_color.a = diffuse_color.a;\n"
    "  } \n" 
    "  generated_color = final_color;\n";
  //Console(4)<< s.str() << endl;
  return s.str();
}


string PhongShader::addUniformFields( ComposedShader *shader ) {
  // add dynamic fields for uniform variables 
  stringstream s;
  
  if( ambientMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "ambient_map" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( ambientMap ) );
  }
  
  if( diffuseMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "diffuse_map" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( diffuseMap ) );
  }

  if( emissionMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "emission_map" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( emissionMap ) );
  }
  
  if( specularMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "specular_map" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( specularMap ) );  
  }

  if( normalMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "normal_map" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( normalMap ) );  
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "normal_map_matrix" ), 
                                     "mat4",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( normalMapMatrix ) );  
  }

  if( glossMap->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "gloss_map" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( glossMap ) );  
  }

  if( fresnel->getValue() > 0 ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "fresnel" ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( fresnel ) );  
  }

  if( separateBackColor->getValue() ) {
    if( backAmbientMap->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_ambient_map" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backAmbientMap ) );
    }
    
    if( backDiffuseMap->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_diffuse_map" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backDiffuseMap ) );
    }
    
    if( backEmissionMap->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_emission_map" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backEmissionMap ) );
    }
  
    if( backSpecularMap->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_specular_map" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backSpecularMap ) );  
    }

    if( backNormalMap->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_normal_map" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backNormalMap ) );  
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_normal_map_matrix" ), 
                                       "mat4",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backNormalMapMatrix ) );  
    }

    if( backGlossMap->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_gloss_map" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backGlossMap ) );  
    }

    if( backFresnel->getValue() > 0 ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "backFresnel" ), 
                                       "float",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backFresnel ) );  
    }

  }

  return s.str();
}

void PhongShader::traverseSG( TraverseInfo &ti ) {
  ComposedShader::traverseSG( ti );

  static bool requires_tangents;

  ti.setUserData( "shaderRequiresTangents", &requires_tangents );

  requires_tangents = 
    (normalMap->getValue() && normalMapCoordSpace->getValue() == "TANGENT" ) || 
    (backNormalMap->getValue() && backNormalMapCoordSpace->getValue() == "TANGENT" );
  
  // the shaderRequiresTangents entry is set to false in 
  // X3DShapeNode::traverseSG in order for it to only be active for one
  // geometry.

}
