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
/// \file PhongShader.cpp
/// \brief CPP file for PhongShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PhongShader.h>
#include <H3D/ShaderPart.h>
#include <H3D/NavigationInfo.h>

#include <H3D/DirectionalLight.h>
#include <H3D/PointLight.h>
#include <H3D/SpotLight.h>

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
  FIELDDB_ELEMENT( PhongShader, specularColorRamp, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, specularColorRampMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, diffuseColorRamp, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, diffuseColorRampMode, INPUT_OUTPUT );
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
  FIELDDB_ELEMENT( PhongShader, backSpecularColorRamp, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backSpecularColorRampMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backDiffuseColorRamp, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backDiffuseColorRampMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backFresnel, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, backModulateMaps, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PhongShader, separateBackColor, INPUT_OUTPUT );

  FIELDDB_ELEMENT( PhongShader, model, INPUT_OUTPUT );

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
                          Inst< SFTexture2DNode > _specularColorRamp,
                          Inst< SFString        > _specularColorRampMode,
                          Inst< SFTexture2DNode > _diffuseColorRamp,
                          Inst< SFString        > _diffuseColorRampMode,
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
                          Inst< SFTexture2DNode > _backSpecularColorRamp,
                          Inst< SFString        > _backSpecularColorRampMode,
                          Inst< SFTexture2DNode > _backDiffuseColorRamp,
                          Inst< SFString        > _backDiffuseColorRampMode,
                          Inst< SFFloat         > _backFresnel,
                          Inst< SFBool          > _backModulateMaps,
                          Inst< SFBool          > _separateBackColor,
                          Inst< SFString        > _model  ) :
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
  diffuseColorRamp( _diffuseColorRamp ),
  diffuseColorRampMode( _diffuseColorRampMode ),
  specularColorRamp( _specularColorRamp ),
  specularColorRampMode( _specularColorRampMode ),
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
  backDiffuseColorRamp( _backDiffuseColorRamp ),
  backDiffuseColorRampMode( _backDiffuseColorRampMode ),
  backSpecularColorRamp( _backSpecularColorRamp ),
  backSpecularColorRampMode( _backSpecularColorRampMode ),
  backFresnel( _backFresnel ),
  backModulateMaps( _backModulateMaps ),
  separateBackColor( _separateBackColor ),
  current_nr_lightsources( 0 ),
  model( _model ) {

  type_name = "PhongShader";
  database.initFields( this );

  model->addValidValue( "BLINN-PHONG" );
  model->addValidValue( "PHONG" );
  model->addValidValue( "BLINN-PHONG-EYEATINF" );
  model->setValue( "BLINN-PHONG" );

  normalMapCoordSpace->addValidValue( "OBJECT" );
  normalMapCoordSpace->addValidValue( "TANGENT" );
  normalMapCoordSpace->setValue( "OBJECT" );
  normalMapMatrix->setValue( Matrix4f( 2, 0, 0, -1,
                                       0, 2, 0, -1,
                                       0, 0, 2, -1,
                                       0, 0, 0, 1 ) );
  specularColorRampMode->addValidValue( "LIGHT" );
  specularColorRampMode->addValidValue( "CAMERA" );
  specularColorRampMode->setValue( "LIGHT" );
  diffuseColorRampMode->addValidValue( "LIGHT" );
  diffuseColorRampMode->addValidValue( "CAMERA" );
  diffuseColorRampMode->setValue( "LIGHT" );

  backSpecularColorRampMode->addValidValue( "LIGHT" );
  backSpecularColorRampMode->addValidValue( "CAMERA" );
  backSpecularColorRampMode->setValue( "LIGHT" );
  backDiffuseColorRampMode->addValidValue( "LIGHT" );
  backDiffuseColorRampMode->addValidValue( "CAMERA" );
  backDiffuseColorRampMode->setValue( "LIGHT" );


  backNormalMapCoordSpace->addValidValue( "OBJECT" );
  backNormalMapCoordSpace->addValidValue( "TANGENT" );
  backNormalMapCoordSpace->setValue( "OBJECT" );

  backNormalMapMatrix->setValue( Matrix4f( 2, 0, 0, -1,
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
  specularColorRamp->route( displayList, id );
  specularColorRampMode->route( displayList, id );
  diffuseColorRamp->route( displayList, id );
  diffuseColorRampMode->route( displayList, id );
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
  backSpecularColorRamp->route( displayList, id );
  backSpecularColorRampMode->route( displayList, id );
  backDiffuseColorRamp->route( displayList, id );
  backDiffuseColorRampMode->route( displayList, id );
  backNormalMapMatrix->route( displayList, id );
  backModulateMaps->route( displayList, id );
  backFresnel->route( displayList, id );
  separateBackColor->route( displayList, id );
  model->route( displayList, id );

  ambientMap->route( rebuildShader, id );
  diffuseMap->route( rebuildShader, id );
  emissionMap->route( rebuildShader, id );
  normalMap->route( rebuildShader, id );
  normalMapCoordSpace->route( rebuildShader, id );
  specularMap->route( rebuildShader, id );
  glossMap->route( rebuildShader, id );
  diffuseColorRamp->route( rebuildShader, id );
  diffuseColorRampMode->route( rebuildShader, id );
  specularColorRamp->route( rebuildShader, id );
  specularColorRampMode->route( rebuildShader, id );
  normalMapMatrix->route( rebuildShader, id );
  fresnel->route( rebuildShader, id );
  modulateMaps->route( rebuildShader, id );
  model->route( rebuildShader, id );

  backAmbientMap->route( rebuildShader, id );
  backDiffuseMap->route( rebuildShader, id );
  backEmissionMap->route( rebuildShader, id );
  backNormalMap->route( rebuildShader, id );
  backNormalMapCoordSpace->route( rebuildShader, id );
  backSpecularMap->route( rebuildShader, id );
  backGlossMap->route( rebuildShader, id );
  backDiffuseColorRamp->route( rebuildShader, id );
  backDiffuseColorRampMode->route( rebuildShader, id );
  backSpecularColorRamp->route( rebuildShader, id );
  backSpecularColorRampMode->route( rebuildShader, id );
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


/// Get a string with GLSL function definitions to be used by
/// the generated shader.
string PhongShader::getFunctionShaderString() {
  stringstream s;

  const string &shader_model = model->getValue();
  
  s << "float " << uniqueShaderName( "getSpecularMultiplier") << "( gl_LightSourceParameters light, vec3 light_dir, vec3 normal, vec3 vertex, float shininess ) { " << endl;
  if( shader_model == "PHONG" ) {
    s << "   vec3 eye_dir = normalize( -vertex );" << endl;
    s << "   vec3 R = reflect(-light_dir, normal);" << endl; 
    s << "   float pf = pow(max(dot(R,eye_dir), 0.0), shininess);" << endl;
    s << "   return pf; " << endl;
  } else {
    if( shader_model != "BLINN-PHONG" && shader_model != "BLINN-PHONG-EYEATINF" ) {
      Console(4) << "Warning: Unsupported value \" " << shader_model << "\" for model field in PhongShader. Must be \"BLINN-PHONG\", \"BLINN-PHONG-EYEATINF\" or \"PHONG\". " << endl; 
    }
    
    if( shader_model == "BLINN-PHONG-EYEATINF" ) {
      s << "   vec3 eye_dir = vec3( 0, 0, 1 ); " << endl;
    } else {
      s << "   vec3 eye_dir = normalize( -vertex );" << endl;
    }
    /*if( shader_model == "GLSL_HALFVECTOR" ) {
      s << "  vec3 H = normalize(light.halfVector.xyz);" << endl;
      } else {*/
    s << "   vec3 H = normalize( eye_dir + light_dir ); " << endl; 
    s << "  return pow(max(dot(normal,H), 0.0), shininess); " << endl;
  }
  s << "}  " << endl;


  s << "float " << uniqueShaderName("calculateAttenuation") << 
    "(in gl_LightSourceParameters light, in float dist) {\n"
    "  return(1.0 / max(light.constantAttenuation +\n"
    "                   light.linearAttenuation * dist +\n"
    "                   light.quadraticAttenuation * dist * dist, 1.0 ));\n"
    "}\n"
    "\n"
    "// Returns the color generated by one directional light using the Phong lighting model.\n"
    "// i is the index of the light\n" <<
    "vec4 " << uniqueShaderName("directionalLightPhong") << "(in gl_LightSourceParameters light, \n" <<
    "                           in vec3 normal, \n"
    "                           in vec3 vertex, \n"
    "                           in float material_shininess,\n"
    "                           in vec4 material_ambient, \n"
    "                           in vec4 material_diffuse, \n"
    "                           in vec4 material_specular) {\n"
    "  \n"
    "  vec4 ambient  = vec4( 0.0 );\n"
    "  vec4 specular = vec4( 0.0 );\n"
    "  vec4 diffuse  = vec4( 0.0 );\n"
    "\n"
    "  vec3 light_dir = normalize(light.position.xyz);\n"
    "   \n"
    "  // cos angle between N and L\n"
    "  float nDotL = dot(normal, light_dir);\n"
    "   \n"
    "  if (nDotL > 0.0) {   \n"
    "    float pf = " << uniqueShaderName("getSpecularMultiplier") << "( light, light_dir, normal, vertex, material_shininess ); \n" 
    "    diffuse  += material_diffuse * light.diffuse  * nDotL;\n"
    "    specular += material_specular * light.specular * pf;\n"
    "  }\n"
    "  ambient  += material_ambient * light.ambient;\n"
    "\n"
    "  return ambient + specular + diffuse;\n"
    "}\n"
    "\n"
    "// Returns the color generated by one point light using the Phong lighting model.\n"
    "// i is the index of the light\n"
    "vec4 " << uniqueShaderName("pointLightPhong") << "(in gl_LightSourceParameters light, \n" <<
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
    "  vec3 D = light.position.xyz - vertex;\n"
    "  vec3 L = normalize(D);\n"
    "  float dist = length(D);\n"
    "  float attenuation = " << uniqueShaderName("calculateAttenuation") << "(light, dist);\n" <<
    "  float nDotL = dot(normal,L);\n"
    "  \n"
    "  if (nDotL > 0.0) {   \n"
    "    float pf =  " << uniqueShaderName("getSpecularMultiplier") << "( light, L, normal, vertex, material_shininess ); \n" <<
    "    diffuse  += material_diffuse * light.diffuse  * attenuation * nDotL;\n"
    "    specular += material_specular * light.specular * attenuation * pf;\n"
    "  }\n"
    "  ambient += material_ambient * light.ambient * attenuation;\n"
    "\n"
    "  return ambient + specular + diffuse;\n"
    "}\n"
    "// Returns the color generated by one point light using the Phong lighting model.\n"
    "// i is the index of the light\n"
    "vec4 " << uniqueShaderName("pointLightPhongX3D") << "(in gl_LightSourceParameters light, \n" <<
    "                        in vec3 normal, \n"
    "                        in vec3 vertex, \n"
    "                        in float material_shininess,\n"
    "                        in vec4 material_ambient, \n"
    "                        in vec4 material_diffuse, \n"
    "                        in vec4 material_specular, \n"
    "                        in float radius ) {\n"
    "  vec4 ambient  = vec4( 0.0 );\n"
    "  vec4 specular = vec4( 0.0 );\n"
    "  vec4 diffuse  = vec4( 0.0 );\n"
    "\n"
    "  vec3 D = light.position.xyz - vertex;\n"
    "  float dist = length(D);\n"
    "  if( dist > radius ) return vec4( 0.0, 0.0, 0.0, 1.0 ); \n"
    "  float attenuation = " << uniqueShaderName("calculateAttenuation") << "(light, dist);\n" <<
    "  vec3 L = normalize(D);\n"
    "  float nDotL = dot(normal,L);\n"
    "  \n"
    "  if (nDotL > 0.0) {   \n" 
    "    float pf = " << uniqueShaderName("getSpecularMultiplier" ) << "( light, L, normal, vertex, material_shininess ); \n"
    "    diffuse  += material_diffuse * light.diffuse  * attenuation * nDotL;\n"
    "    specular += material_specular * light.specular * attenuation * pf;\n"
    "  }\n"
    "  ambient += material_ambient * light.ambient * attenuation;\n"
    "\n"
    "  return ambient + specular + diffuse;\n"
    "}\n"
    "\n"
    "// Returns the color generated by one spot light using the Phong lighting model.\n"
    "// i is the index of the light\n"
    "vec4 " << uniqueShaderName("spotLightPhong") << "( in gl_LightSourceParameters light, \n" <<
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
    "  vec3 D = light.position.xyz - vertex;\n"
    "  vec3 L = normalize(D);\n"
    "  \n"
    "  float dist = length(D);\n"
    "  float attenuation = " << uniqueShaderName("calculateAttenuation") << "(light, dist);\n" <<
    "\n"
    "  float nDotL = dot(normal,L);\n"
    "\n"
    "  if (nDotL > 0.0) {   \n"
    "    float spotEffect = dot(normalize(light.spotDirection), -L);\n"
    "    \n"
    "    if (spotEffect > light.spotCosCutoff) {\n"
    "      attenuation *=  pow(spotEffect, light.spotExponent);\n"
    "      float pf = " << uniqueShaderName("getSpecularMultiplier") << "( light, L, normal, vertex, material_shininess ); \n" 
    "      \n"
    "      diffuse  += material_diffuse * light.diffuse  * attenuation * nDotL;\n"
    "      specular += material_specular * light.specular * attenuation * pf;\n"
    "    }\n"
    "  }\n"
    "  ambient  += material_ambient * light.ambient * attenuation;\n"
    "  return ambient + specular + diffuse;\n"
    "}\n"
    "\n"
    "// Returns the color generated by one spot light using the Phong lighting model.\n"
    "// i is the index of the light\n"
    "vec4 " << uniqueShaderName("spotLightPhongX3D") << "( in gl_LightSourceParameters light,\n"
    "                        in vec3 normal, \n"
    "                        in vec3 vertex, \n"
    "                        in float material_shininess,\n"
    "                        in vec4 material_ambient, \n"
    "                        in vec4 material_diffuse, \n"
    "                        in vec4 material_specular, \n"
    "                        in float radius, \n"
    "                        in float beam_width ) {\n"
    "  vec4 ambient  = vec4( 0.0 );\n"
    "  vec4 specular = vec4( 0.0 );\n"
    "  vec4 diffuse  = vec4( 0.0 );\n"
    "\n"
    "  vec3 D = light.position.xyz - vertex;\n"
    "  \n"
    "  float dist = length(D);\n"
    "  if( dist > radius ) return vec4( 0.0, 0.0, 0.0, 1.0 ); \n"
    "  vec3 L = normalize(D);\n"
    "  float attenuation = " << uniqueShaderName("calculateAttenuation") << "(light, dist);\n"
    "\n"
    "  float nDotL = dot(normal,L);\n"
    "\n"
    "  if (nDotL > 0.0) {   \n"
    "    float spotAngle = acos(dot(normalize(light.spotDirection), -L));\n"
    "    float spotCutOffAngle = 3.1415 * light.spotCutoff / 180.0;\n"
    "    \n"
    "    float multiplier = 0.0;\n"
    "    if ( spotAngle >= spotCutOffAngle ) {\n"
    "        multiplier = 0.0;\n"
    "    } else if (spotAngle <= beam_width) {\n"
    "        multiplier = 1.0;\n"
    "    } else {\n"
    "        multiplier = (spotAngle - spotCutOffAngle) / (beam_width - spotCutOffAngle);\n"
    "    }\n"
    "    \n" 
    "    if (multiplier > 0.0 ) {\n"
    "      attenuation *= multiplier;\n"
    "      float pf = " << uniqueShaderName("getSpecularMultiplier") << "( light, L, normal, vertex, material_shininess ); \n" 
    "      \n"
    "      diffuse  += material_diffuse * light.diffuse  * attenuation * nDotL;\n"
    "      specular += material_specular * light.specular * attenuation * pf;\n"
    "    }\n"
    "  }\n"
    "  ambient  += material_ambient * light.ambient * attenuation;\n"
    "  return ambient + specular + diffuse;\n"
    "}\n"
    "\n"
    "\n"
    "// Returns the color generated by one light using the Phong lighting model.\n"
    "// It will choose one of directionalLightPhong, pointLightPhong or \n"
    "// spotLightPhong depending on what kind of light it is.\n"
    "// i is the index of the light\n"
    "vec4 " << uniqueShaderName("lightPhong") << "(in gl_LightSourceParameters light, \n"
    "                in vec3 normal, \n"
    "                in vec3 vertex, \n"
    "                in float shininess,\n"
    "                in vec4 ambient, \n"
    "                in vec4 diffuse, \n"
    "                in vec4 specular) {\n"
    "  if (light.position.w == 0.0)\n"
    "    return " << uniqueShaderName("directionalLightPhong") << "(light, normal, vertex, shininess, ambient, diffuse, specular);\n"
    "  else if (light.spotCutoff == 180.0)\n"
    "    return " << uniqueShaderName("pointLightPhong") << "(light, normal, vertex, shininess, ambient, diffuse, specular);\n"
    "  else\n"
    "    return " << uniqueShaderName("spotLightPhong" ) << "(light, normal, vertex, shininess, ambient, diffuse, specular);\n"
    "}\n";

  return s.str();
}

void PhongShader::getAttributes( vector< Attribute > &attributes ) {
  if( normalMap->getValue() && normalMapCoordSpace->getValue() == "TANGENT" ) {
    bool have_binormal = false;
    bool have_tangent = false;
    for( unsigned int i = 0; i < attributes.size(); ++i ) {
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

  s << "    vec3 orig_normal = normalize( normal ); " << endl;
  s << "    vec3 view_dir = normalize(-vertex);" << endl;

  // add fresnel term if fresnel is used
  if( fresnel->getValue() > 0 || (separateBackColor->getValue()  && backFresnel->getValue() > 0 ) ){
    s << "    float fresnel = pow(1.0-abs(dot(normalize(-vertex), normal)), 5.0);" << endl;
  }

  s << "    vec4 diffuse_color, specular_color, ambient_color, emission_color; " << endl;
  s << "    float shininess; " << endl;
  s << "    vec4 back_diffuse_color, back_specular_color, back_ambient_color, back_emission_color; " << endl;
  s << "    float back_shininess; " << endl;
  s << "    vec3 N, back_N; " << endl;
  s << getBaseColors( "emission_color", "shininess", "ambient_color", "diffuse_color", "specular_color", "N",
                      "back_emission_color", "back_shininess", "back_ambient_color", "back_diffuse_color", "back_specular_color", "back_N" );


  // saturate specular color based on fresnel effect
  if( fresnel->getValue() > 0 ) {
    s << "    float f = " << uniqueShaderName( "fresnel" ) << ";" << endl;
    s << "    specular_color *= 1-f + f * fresnel; " << endl;
  }

 
  bool use_camera_specular_color_ramp = 
    specularColorRamp->getValue() && specularColorRampMode->getValue() == "CAMERA"; 
  
  bool use_camera_diffuse_color_ramp = 
    diffuseColorRamp->getValue() && diffuseColorRampMode->getValue() == "CAMERA"; 

  // color ramps
  if( use_camera_specular_color_ramp || use_camera_diffuse_color_ramp ) {
    // the lookup 
    s << "    float ramp_coord = dot( N, view_dir ); " << endl;
  }

  if( use_camera_specular_color_ramp ) {
    s << "    vec4 specular_color_ramp = texture2D( " << uniqueShaderName( "specular_color_ramp" ) << ", vec2( ramp_coord, 0.5 ) );" << endl;
    s << "    specular_color = specular_color * specular_color_ramp; " << endl;
  }

  if( use_camera_diffuse_color_ramp ) {
    s << "    vec4 diffuse_color_ramp = texture2D( " << uniqueShaderName( "diffuse_color_ramp" ) << ", vec2( ramp_coord, 0.5 ) ) ;" << endl;
    s << "    diffuse_color = diffuse_color * diffuse_color_ramp; " << endl;
  }
  
  // back colors
 
  if( separateBackColor->getValue() ) {
    // saturate specular color based on fresnel effect
    if( backFresnel->getValue() > 0 ) {
      s << "    float bf = " << uniqueShaderName( "backFresnel" ) << ";" << endl;
      s << "    back_specular_color *= 1-bf + bf * fresnel; " << endl;
    }
    
    bool use_camera_back_specular_color_ramp = 
      backSpecularColorRamp->getValue() && backSpecularColorRampMode->getValue() == "CAMERA"; 
  
    bool use_camera_back_diffuse_color_ramp = 
      backDiffuseColorRamp->getValue() && backDiffuseColorRampMode->getValue() == "CAMERA"; 
    
    if( use_camera_back_specular_color_ramp || use_camera_back_diffuse_color_ramp ) {
      // the lookup 
      s << "    float back_ramp_coord = dot( back_N, view_dir ); " << endl;
    }

    if( use_camera_back_specular_color_ramp ) {
      s << "    vec4 back_specular_color_ramp = texture2D( " << uniqueShaderName( "back_specular_color_ramp" ) << ", vec2( back_ramp_coord, 0.5 ) );" << endl;
      s << "    back_specular_color = back_specular_color * back_specular_color_ramp; " << endl;
    }

    if( use_camera_back_diffuse_color_ramp ) {
      s << "    vec4 back_diffuse_color_ramp = texture2D( " << uniqueShaderName( "back_diffuse_color_ramp" ) << ", vec2( back_ramp_coord, 0.5 ) );" << endl;
      s << "    back_diffuse_color = back_diffuse_color * back_diffuse_color_ramp; " << endl;
    }

  }

  s <<
    "  vec4 final_color = vec4( 0.0, 0.0, 0.0, 1.0 );\n"
    "  bool is_back_face = (dot(orig_normal, view_dir) < 0.0);\n"
    "  if( is_back_face ) { " << endl;

  unsigned int light_index = 0;

  // add headlight if enabled
  bool have_headlight = current_nr_lightsources > current_light_nodes.size();
    
  for( unsigned int i = 0; i < current_nr_lightsources; ++i ) { 
    X3DLightNode *light_node = NULL;
    if( have_headlight ) {
      if( i > 0 ) light_node = current_light_nodes[i-1].getLight();
    } else {
      light_node = current_light_nodes[i].getLight();
    }

    s << "      { " << endl;
    // create the variable "light" with all light parameters.
    s << setupLight( light_node, i ) << endl;
    bool use_back_specular_color_ramp = 
      (separateBackColor->getValue() && backSpecularColorRamp->getValue() && backSpecularColorRampMode->getValue() == "LIGHT" ) ||
      (!separateBackColor->getValue() && specularColorRamp->getValue() && specularColorRampMode->getValue() == "LIGHT");

    bool use_back_diffuse_color_ramp = 
      (separateBackColor->getValue() && backDiffuseColorRamp->getValue() && backDiffuseColorRampMode->getValue() == "LIGHT" )||
      (!separateBackColor->getValue() && diffuseColorRamp->getValue() && diffuseColorRampMode->getValue() == "LIGHT" );

    if( use_back_specular_color_ramp || use_back_diffuse_color_ramp ) {
      if( dynamic_cast< DirectionalLight * >( light_node ) ) {
        s << "      vec3 light_dir = -normalize(light.position.xyz);" << endl;
      } else {
        s << "    vec3 D = light.position.xyz - vertex; " << endl;
        s << "    vec3 light_dir = normalize(D);" << endl;
      }
    }

    if( use_back_specular_color_ramp ) {
      s << "      float ramp_coord_specular = " << uniqueShaderName( "getSpecularMultiplier" ) <<  "( light, light_dir, back_N, vertex, back_shininess ); " << endl;
      if( separateBackColor->getValue() ) {
        s << "      vec4 back_specular_color_ramp = texture2D( " << uniqueShaderName( "back_specular_color_ramp" ) << ", vec2( ramp_coord_specular, 0.5 ) );" << endl;
      } else {
        s << "      vec4 back_specular_color_ramp = texture2D( " << uniqueShaderName( "specular_color_ramp" ) << ", vec2( ramp_coord_specular, 0.5 ) );" << endl;
      } 
      s << "      vec4 back_specular_color_i = back_specular_color * back_specular_color_ramp;" << endl;
    } else {
      s << "      vec4 back_specular_color_i = back_specular_color;" << endl;
    }
  
    if( use_back_diffuse_color_ramp ) {
    
      s << "      float ramp_coord_diffuse = dot(back_N, light_dir);" << endl;
      if( separateBackColor->getValue() ) {
        s << "      vec4 back_diffuse_color_ramp = texture2D( " << uniqueShaderName( "back_diffuse_color_ramp" ) << ", vec2( ramp_coord_diffuse, 0.5 ) );" << endl;
      } else {
        s << "      vec4 back_diffuse_color_ramp = texture2D( " << uniqueShaderName( "diffuse_color_ramp" ) << ", vec2( ramp_coord_diffuse, 0.5 ) );" << endl;
      }
      s << "      vec4 back_diffuse_color_i = back_diffuse_color * back_diffuse_color_ramp;" << endl;
    } else {
      s << "      vec4 back_diffuse_color_i = back_diffuse_color;" << endl;
    }
    
    
    s << getLightContribution( light_node, i, "back_N", "vertex", "back_shininess", "back_ambient_color", "back_diffuse_color_i", "back_specular_color_i" ) << endl;
    s << "    } " << endl;
  }

  s << "    final_color += back_emission_color;\n"
    "    final_color.a = back_diffuse_color.a;\n"
    "  } else { \n";

  for( unsigned int i = 0; i < current_nr_lightsources; ++i ) { 
    X3DLightNode *light_node = NULL;
    if( have_headlight ) {
      if( i > 0 ) light_node = current_light_nodes[i-1].getLight();
    } else {
      light_node = current_light_nodes[i].getLight();
    }

    s << "      { " << endl;
    // create the variable "light" with all light parameters.
    s << setupLight( light_node, i ) << endl;

    bool use_specular_color_ramp = specularColorRamp->getValue() && specularColorRampMode->getValue() == "LIGHT";
    bool use_diffuse_color_ramp = diffuseColorRamp->getValue() && diffuseColorRampMode->getValue() == "LIGHT";

    if( use_diffuse_color_ramp || use_specular_color_ramp ) {
      if( dynamic_cast< DirectionalLight * >( light_node ) ) {
        s << "      vec3 light_dir = normalize(light.position.xyz);" << endl;
      } else {
        s << "    vec3 D = light.position.xyz - vertex; " << endl;
        s << "    vec3 light_dir = normalize(D);" << endl;
      }
    }

    if( use_specular_color_ramp ) {
      s << "      float ramp_coord = " << uniqueShaderName( "getSpecularMultiplier " ) << "( light, light_dir, N, vertex, shininess ); " << endl; 
      s << "      vec4 specular_color_ramp = texture2D( " << uniqueShaderName( "specular_color_ramp" ) << ", vec2( ramp_coord, 0.5 ) );" << endl;
      s << "      vec4 specular_color_i = specular_color * specular_color_ramp;" << endl;
    } else {
      s << "      vec4 specular_color_i = specular_color;" << endl;
    }

    if( use_diffuse_color_ramp ) {
      s << "      float ramp_coord = dot(N, light_dir);" << endl;
      s << "      vec4 diffuse_color_ramp = texture2D( " << uniqueShaderName( "diffuse_color_ramp" ) << ", vec2( ramp_coord, 0.5 ) );" << endl;
      s << "      vec4 diffuse_color_i = diffuse_color * diffuse_color_ramp;" << endl;
    } else {
      s << "      vec4 diffuse_color_i = diffuse_color;" << endl;
    }
    
    s << getLightContribution( light_node, i, "N", "vertex", "shininess", "ambient_color", "diffuse_color_i", "specular_color_i" ) << endl;
    s << "    } " << endl;
  }
  s << 
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

  if( specularColorRamp->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "specular_color_ramp" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( specularColorRamp ) );  
  }

  if( diffuseColorRamp->getValue() ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "diffuse_color_ramp" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( diffuseColorRamp ) );  
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

    if( backSpecularColorRamp->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_specular_color_ramp" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backSpecularColorRamp ) );  
    }

    if( backDiffuseColorRamp->getValue() ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "back_diffuse_color_ramp" ), 
                                       "sampler2D",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backDiffuseColorRamp ) );  
    }

    if( backFresnel->getValue() > 0 ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueShaderName( "backFresnel" ), 
                                       "float",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( backFresnel ) );  
    }

  }

  NavigationInfo *ni = NavigationInfo::getActive();
  int index_offset = 0;

  // add headlight if enabled
  bool have_headlight = current_nr_lightsources > current_light_nodes.size();
  if( have_headlight ) {
    ++index_offset;
  }
    
  // add light uniforms
  for( unsigned int i = 0; i < current_light_nodes.size(); ++i ) {
    s << addUniformFieldsForLight( shader, current_light_nodes[i].getLight(), i+index_offset );
  }

  return s.str();
}

string PhongShader::addUniformFieldsForLight( ComposedShader *shader,
                                              X3DLightNode *light,
                                              int gl_index ) {
  stringstream s;
  SpotLight *spot_light = dynamic_cast< SpotLight * >( light );
  PointLight *point_light = dynamic_cast< PointLight * >( light );
  DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );
  
  if( spot_light ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueLightFieldName( "beamWidth", spot_light ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( spot_light->beamWidth ) );  
    s << addUniformToFragmentShader( shader,
                                     uniqueLightFieldName( "radius", spot_light ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( spot_light->radius ) );  
  }
  
  if( point_light ) {
    s << addUniformToFragmentShader( shader,
                                     uniqueLightFieldName( "radius", point_light ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( point_light->radius ) );  
  }
  
  GLint max_lights;
  glGetIntegerv( GL_MAX_LIGHTS, &max_lights );

  if( (int) gl_index >= max_lights ) {   
    stringstream t_name;
    t_name << uniqueLightFieldName( "transform", light ) << gl_index;
    s << addUniformToFragmentShader( shader,
                                     t_name.str(), 
                                     "mat4",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( current_light_node_transforms[gl_index - max_lights] ) ); 

    s << addUniformToFragmentShader( shader,
                                     uniqueLightFieldName( "color", light ), 
                                     "vec3",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( light->color ) ); 
    
    s << addUniformToFragmentShader( shader,
                                     uniqueLightFieldName( "ambientIntensity", light ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( light->ambientIntensity ) ); 
    
    s << addUniformToFragmentShader( shader,
                                     uniqueLightFieldName( "intensity", light ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( light->intensity ) ); 
    

    if( dir_light ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueLightFieldName( "direction", dir_light ), 
                                       "vec3",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( dir_light->direction ) ); 
      
      
    } else if( point_light ) {
      s << addUniformToFragmentShader( shader,
                                       uniqueLightFieldName( "location", point_light ), 
                                       "vec3",
                                       H3D::Field::INPUT_OUTPUT,
                                       copyAndRouteField( point_light->location ) ); 
      s <<  addUniformToFragmentShader( shader,
                                        uniqueLightFieldName( "attenuation", point_light ), 
                                        "vec3",
                                        H3D::Field::INPUT_OUTPUT,
                                        copyAndRouteField( point_light->attenuation ) ); 
    } else if( spot_light ) {   
      s <<  addUniformToFragmentShader( shader,
                                        uniqueLightFieldName( "location", spot_light ), 
                                        "vec3",
                                        H3D::Field::INPUT_OUTPUT,
                                        copyAndRouteField( spot_light->location ) ); 
      s <<  addUniformToFragmentShader( shader,
                                        uniqueLightFieldName( "direction", spot_light ), 
                                        "vec3",
                                        H3D::Field::INPUT_OUTPUT,
                                        copyAndRouteField( spot_light->direction ) ); 
      s <<  addUniformToFragmentShader( shader,
                                        uniqueLightFieldName( "cutOffAngle", spot_light ), 
                                        "float",
                                        H3D::Field::INPUT_OUTPUT,
                                        copyAndRouteField( spot_light->cutOffAngle ) ); 
      s <<  addUniformToFragmentShader( shader,
                                        uniqueLightFieldName( "beamWidth", spot_light ), 
                                        "float",
                                        H3D::Field::INPUT_OUTPUT,
                                        copyAndRouteField( spot_light->beamWidth ) ); 
      s <<  addUniformToFragmentShader( shader,
                                        uniqueLightFieldName( "attenuation", spot_light ), 
                                        "vec3",
                                        H3D::Field::INPUT_OUTPUT,
                                        copyAndRouteField( spot_light->attenuation ) ); 
      
    }
  }
  return s.str();
}

void PhongShader::traverseSG( TraverseInfo &ti ) {
  ComposedShader::traverseSG( ti );

  static bool requires_tangents;

  requires_tangents = 
    (normalMap->getValue() && normalMapCoordSpace->getValue() == "TANGENT" ) || 
    (backNormalMap->getValue() && backNormalMapCoordSpace->getValue() == "TANGENT" );

   if( requires_tangents ) {
     ti.setUserData( "shaderRequiresTangents", &requires_tangents );
   }

  // the shaderRequiresTangents entry is set to false in 
  // X3DShapeNode::traverseSG in order for it to only be active for one
  // geometry.

   NavigationInfo *ni = NavigationInfo::getActive();
   bool headlight = !ni || ni->headlight->getValue();
   size_t nr_active_lights = 0;
   if( headlight ) ++nr_active_lights;

   TraverseInfo::LightVector light_nodes = ti.getActiveLightNodes();

   // order lights in order of its index in OpenGL. 
   // Lights are given indices in the order: headlight, local lights, global lights.
   TraverseInfo::LightVector ordered_light_nodes;
  
   // the OpenGL light index for the current light.
   int gl_index = 0;
  
   // add global lights
   for( unsigned int i = 0; i < light_nodes.size(); ++i ) {
     if( light_nodes[i].getLight()->global->getValue() ) {
       ordered_light_nodes.push_back( light_nodes[i] );
       ++gl_index;
     }
   }

   // add local lights
   for( unsigned int i = 0; i < light_nodes.size(); ++i ) {
     if( !light_nodes[i].getLight()->global->getValue() ) {
       ordered_light_nodes.push_back( light_nodes[i] );
       ++gl_index;
     }
   }

   nr_active_lights += ordered_light_nodes.size();

   // check if number of light sources have changed.
   bool lights_have_changed = nr_active_lights != current_nr_lightsources;

   // check if the light source nodes have changed
   if( !lights_have_changed ) {
     for( unsigned int i = 0; i < ordered_light_nodes.size(); ++i ) {
       if( ordered_light_nodes[i].getLight() != current_light_nodes[i].getLight() ) {
         lights_have_changed = true;
         break;
       }
     }
   }
   
   current_nr_lightsources = (unsigned int) nr_active_lights;
   current_light_nodes = ordered_light_nodes;

   GLint max_lights = 1;
   glGetIntegerv( GL_MAX_LIGHTS, &max_lights );

   // make sure we have the right number of transforms, i.e. one for
   // each light more than maximum number of supported light sources
   // by OpenGL implementation.
   if( lights_have_changed ) {
     current_light_node_transforms.clear();
     for( unsigned int i = max_lights; i < current_nr_lightsources; ++i ) {
       current_light_node_transforms.push_back( new SFMatrix4f );
     }
   } 

   // set the transforms for the light sources with indices over
   // max index. These are needed to transform light parameters
   // to view space. The transform is from local light space to 
   // local model space.
   // This transform will be multiplied by gl_ModelViewMatrix in shader
   // to get the light to view space.
   for( int i = 0; i < (int) current_light_nodes.size() - max_lights; ++i ) {
     current_light_node_transforms[i]->setValue( ti.getAccInverseMatrix() * current_light_nodes[i+max_lights].getLightTransform() );
   }

   if( lights_have_changed ) {
     // nr of lights have changed, touch a field to rebuild shader
     ambientMap->touch();
   }

}

string PhongShader::setupLight( X3DLightNode *light,
                                unsigned int index ) {
  stringstream s;
  GLint max_lights;
  glGetIntegerv( GL_MAX_LIGHTS, &max_lights );

  SpotLight *spot_light = dynamic_cast< SpotLight * >( light );
  DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );
  PointLight *point_light = dynamic_cast< PointLight * >( light );

  if( index < (unsigned int)max_lights ) {
    s << "      gl_LightSourceParameters light = gl_LightSource[" << index << "];" << endl;
  } else {
    // we are using more lightsources than there are support for in fixed function
    // OpenGL so we need to build the entire light parameter structure ourselves.
    s << "      gl_LightSourceParameters light; " << endl;
    s << "      light.diffuse.rgb = " << uniqueLightFieldName( "intensity", light ) << " * " << uniqueLightFieldName( "color", light ) << ";" << endl;
    s << "      light.diffuse.a = 1.0; "<< endl;
    s << "      light.specular = light.diffuse; " << endl;
    s << "      light.ambient.rgb = " << uniqueLightFieldName( "ambientIntensity", light ) << " * " << uniqueLightFieldName( "color", light ) << ";" << endl;
    s << "      light.ambient.a = 1.0; " << endl;
    s << "      mat4 light_transform = gl_ModelViewMatrix * " << uniqueLightFieldName("transform", light) << index << ";" << endl;

    if( dir_light ) {
      s << "      light.position = light_transform * vec4( -" << uniqueLightFieldName( "direction", light ) <<  ", 0.0 ); " << endl;
      s << "      light.spotCutoff = 180.0; " << endl;
      s << "      light.spotCosCutoff = -1.0; " << endl;
      s << "      vec3 light_direction = normalize(light.position.xyz);" << endl;
      s << "      light.halfVector = vec4( normalize( light_direction + vec3( 0.0, 0.0, 1.0 ) ), 0.0 ); " << endl;
    } else if( point_light || spot_light ) {
      s << "      light.position = light_transform * vec4( " << uniqueLightFieldName( "location", light ) <<  ", 1.0 ); " << endl;
      s << "      vec3 attenuation = " << uniqueLightFieldName( "attenuation", light ) << "; " << endl;
      s << "      light.constantAttenuation = attenuation.x; " << endl;
      s << "      light.linearAttenuation = attenuation.y; " << endl;
      s << "      light.quadraticAttenuation = attenuation.z; " << endl;
      s << "      vec3 light_direction =  normalize( light.position.xyz - vertex );" << endl;
      s << "      light.halfVector = vec4( normalize( light_direction + vec3( 0.0, 0.0, 1.0 ) ), 0.0 ); " << endl;
      if( spot_light ) {
        s << "      light.spotDirection = (light_transform * vec4( " << uniqueLightFieldName( "direction", light ) <<  ", 0.0)).xyz; " << endl;
        s << "      light.spotCutoff = " << uniqueLightFieldName( "cutOffAngle", light ) <<  "*180.0 / 3.1415; " << endl;
        s << "      light.spotCosCutoff = cos( light.spotCosCutoff ); " << endl;
        if( spot_light->beamWidth->getValue() < spot_light->cutOffAngle->getValue() ) {
          s << "      light.spotExponent = 1.0; " << endl;
        } else {
          s << "      light.spotExponent = 0.0; " << endl;
        }
      } else {
        s << "      light.spotCutoff = 180.0; " << endl;
        s << "      light.spotCosCutoff = -1.0; " << endl;
      }
    }
  }
  return s.str();
}

string PhongShader::getLightContribution( X3DLightNode *light,
                                          unsigned int index, 
                                          string normal, 
                                          string vertex, 
                                          string shininess, 
                                          string ambient_color,
                                          string diffuse_color,
                                          string specular_color ) {

  stringstream s;

  SpotLight *spot_light = dynamic_cast< SpotLight * >( light );
  DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );
  PointLight *point_light = dynamic_cast< PointLight * >( light );

  // directional light
  if( !light || dir_light) {
    // if light is NULL then it is the headlight
    s << "      final_color += " << uniqueShaderName( "directionalLightPhong" ) << "( light, " << normal << ", "  << vertex << ", "<< shininess << ", " <<  ambient_color << ", " << diffuse_color << ", " << specular_color << ");" << endl;
  } else {
    // point light
    if( point_light ) {
      s << "      final_color += " << uniqueShaderName( "pointLightPhongX3D" ) << "( light, " << normal << ", " << vertex << ", " << shininess << ", " <<  ambient_color << ", " << diffuse_color << ", " << specular_color << ", " << uniqueLightFieldName( "radius", point_light ) << ");" << endl;
    } else {
      // spot light
      if( spot_light ) {
        s << "      final_color += " << uniqueShaderName( "spotLightPhongX3D" ) << "( light, " << normal << ", " << vertex << ", " << shininess << ", " <<  ambient_color << ", " << diffuse_color << ", " << specular_color << ", " << uniqueLightFieldName( "radius", spot_light ) <<", " << uniqueLightFieldName( "beamWidth", spot_light ) << ");" << endl;
      } else {
        // default
        s << "      final_color += " << uniqueShaderName( "lightPhong" ) << "( light, " << normal << ", " << vertex << ", " << shininess << ", " <<  ambient_color << ", " << diffuse_color << ", " << specular_color << ");" << endl;
      }
    }
  }

  return s.str();
}


string PhongShader::uniqueLightFieldName( const string &field_name,
                                          X3DLightNode *light) {
  stringstream name;
  name << field_name << (unsigned long)light;
  return uniqueShaderName( name.str() );
}


string PhongShader::getBaseDiffuseColor( const string &diffuse_color, 
                                         const string &back_diffuse_color ) {
  stringstream s;
  bool modulate = modulateMaps->getValue();
  bool back_modulate = backModulateMaps->getValue();

  // diffuse_color
  if( diffuseMap->getValue() ) {
    if( modulate ) {
      s << "   " << diffuse_color << " = gl_FrontMaterial.diffuse * texture2D( "
        << uniqueShaderName( "diffuse_map" ) 
        << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    " << diffuse_color << " = texture2D( " << uniqueShaderName( "diffuse_map" ) << ", gl_TexCoord[0].st );" << endl;
      // if the texture contains alpha use the texture alpha, otherwise use the 
      // one from the material diffuse color which is set by the transparency 
      // field in a Material node.
      Image *image = diffuseMap->getValue()->image->getValue();
      if( image && 
          image->pixelType() != Image::LUMINANCE_ALPHA &&
          image->pixelType() != Image::RGBA &&
          image->pixelType() != Image::BGRA) {
        s << "    " << diffuse_color << ".a = gl_FrontMaterial.diffuse.a;" << endl;
      }
    }
  } else {
    s << "    " << diffuse_color << " = gl_FrontMaterial.diffuse; " << endl;
  }

  // back_diffuse_color
  if( separateBackColor->getValue() ) {
    if( backDiffuseMap->getValue() ) {
      if( back_modulate ) {
        s << "    " << back_diffuse_color << " = gl_BackMaterial.diffuse * texture2D( " << uniqueShaderName( "back_diffuse_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    " << back_diffuse_color << " = texture2D( " << uniqueShaderName( "back_diffuse_map" ) << ", gl_TexCoord[0].st );" << endl;
        // if the texture contains alpha use the texture alpha, otherwise use the 
        // one from the material diffuse color which is set by the transparency 
        // field in a Material node.
        Image *image = backDiffuseMap->getValue()->image->getValue();
        if( image && 
            image->pixelType() != Image::LUMINANCE_ALPHA &&
            image->pixelType() != Image::RGBA &&
            image->pixelType() != Image::BGRA) {
          s << "    " << back_diffuse_color << ".a = gl_BackMaterial.diffuse.a;" << endl;
        }
      }
    } else {
      s << "    " << back_diffuse_color << " = gl_BackMaterial.diffuse; " << endl;
    }
  } else {
    s << "    " << back_diffuse_color << " = " << diffuse_color << "; " << endl;
  }
  
  return s.str();
}

string PhongShader::getBaseEmissionColor( const string &emission_color, 
                                          const string &back_emission_color ) {
  stringstream s; 
  bool modulate = modulateMaps->getValue();
  bool back_modulate = backModulateMaps->getValue();

  // emission_color
  if( emissionMap->getValue() ) {
    if( modulate ) {
      s << "    " << emission_color << " = gl_FrontMaterial.emission * texture2D( "
        << uniqueShaderName( "emission_map" ) << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    " << emission_color << " = texture2D( " << uniqueShaderName( "emission_map" ) << ", gl_TexCoord[0].st );" << endl;
    }
  } else {
    s << "    " << emission_color << " = gl_FrontMaterial.emission; " << endl;
  }


  // back_emission_color
  if( separateBackColor->getValue() ) {
    if( backEmissionMap->getValue() ) {
      if( back_modulate ) {
        s << "    " << back_emission_color << " = gl_BackMaterial.emission * texture2D( " << uniqueShaderName( "back_emission_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    " << back_emission_color << " = texture2D( " << uniqueShaderName( "back_emission_map" ) << ", gl_TexCoord[0].st );" << endl;
      }
    } else {
      s << "    " << back_emission_color << " = gl_BackMaterial.emission; " << endl;
    }
  } else {
    s << "    " << back_emission_color << " = " << emission_color << "; " << endl;
  }

  return s.str();
}

string PhongShader::getBaseAmbientColor( const string &ambient_color, 
                                         const string &back_ambient_color ) {
  stringstream s;
  bool modulate = modulateMaps->getValue();
  bool back_modulate = backModulateMaps->getValue();

  // ambient color
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
      s << "    " << ambient_color << "= intensity * texture2D( " << uniqueShaderName( "ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    " << ambient_color << "= texture2D( " << uniqueShaderName( "ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
    }
  } else {
    s << "    " << ambient_color << " = gl_FrontMaterial.ambient; " << endl;
  }
  
  // back_ambient_color
  if( separateBackColor->getValue() ) {
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
        s << "    " << back_ambient_color << " = back_intensity * texture2D( " << uniqueShaderName( "back_ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    " << back_ambient_color << " = texture2D( " << uniqueShaderName( "back_ambient_map" ) << ", gl_TexCoord[0].st );" << endl;
      }
    } else {
      s << "    " << back_ambient_color << " = gl_BackMaterial.ambient; " << endl;
    }
  } else {
    s << "    " << back_ambient_color << " = " << ambient_color << "; " << endl;
  }
  return s.str();
}

string PhongShader::getBaseSpecularColor( const string &specular_color, 
                                          const string &back_specular_color ) {
  stringstream s;
  bool modulate = modulateMaps->getValue();
  bool back_modulate = backModulateMaps->getValue();

  // specular_color
  if( specularMap->getValue() ) {
    if( modulate ) {
      s << "    " << specular_color << " = gl_FrontMaterial.specular * texture2D( " << uniqueShaderName( "specular_map" ) << ", gl_TexCoord[0].st );" << endl;
    } else {
      s << "    " << specular_color << " = texture2D( " << uniqueShaderName( "specular_map" ) << ", gl_TexCoord[0].st );" << endl;
    }
  } else {
    s << "    " << specular_color << " = gl_FrontMaterial.specular; " << endl;
  } 

  // back_specular_color 
  if( separateBackColor->getValue() ) {
    if( backSpecularMap->getValue() ) {
      if( back_modulate ) {
        s << "    " << back_specular_color << " = gl_BackMaterial.specular * texture2D( " << uniqueShaderName( "back_specular_map" ) << ", gl_TexCoord[0].st );" << endl;
      } else {
        s << "    " << back_specular_color << " = texture2D( " << uniqueShaderName( "back_specular_map" ) << ", gl_TexCoord[0].st );" << endl;
      }
    } else {
      s << "    " << back_specular_color << " = gl_BackMaterial.specular; " << endl;
    }
  } else {
    s << "    " << back_specular_color << " = " << specular_color << "; " << endl;
  }
  return s.str();
}
    
string PhongShader::getBaseShininess( const string &shininess, 
                                      const string &back_shininess ) {
  stringstream s;
  bool modulate = modulateMaps->getValue();
  bool back_modulate = backModulateMaps->getValue();

  // shininess
  if( glossMap->getValue() ) {
    if( modulate ) {
      s << "    " << shininess << " = gl_FrontMaterial.shininess * texture2D( " << uniqueShaderName( "gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
    } else {
      s << "    " << shininess << " = texture2D( " << uniqueShaderName( "gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
    }
  } else {
    s << "    " << shininess << " = gl_FrontMaterial.shininess;" << endl;
  }

  // back_shininess
  if( separateBackColor->getValue() ) {
    if( backGlossMap->getValue() ) {
      if( back_modulate ) {
        s << "    " << back_shininess << " = gl_BackMaterial.shininess * texture2D( " << uniqueShaderName( "back_gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
      } else {
        s << "    " << back_shininess << " = texture2D( " << uniqueShaderName( "back_gloss_map" ) << ", gl_TexCoord[0].st ).r;" << endl;
      }
    } else {
      s << "    " << back_shininess << " = gl_BackMaterial.shininess;" << endl;
    }    
  } else {
    s << "    " << back_shininess << " = " << shininess << ";" << endl;
  } 
  
  return s.str();
}

string PhongShader::getBaseNormal( const string &normal, 
                                   const string &back_normal ) {
  stringstream s;

  // normal
  if( normalMap->getValue() ) {

    string coord_space = normalMapCoordSpace->getValue();
    if( coord_space != "OBJECT" && coord_space != "TANGENT" ) {
      Console(4) << "Invalid normalMapCoordSpace value in PhongShader node: \"" 
                 << coord_space << "\". Using \"OBJECT\" instead." << endl;
      coord_space = "OBJECT";
    }

    s << "    mat4 normal_map_matrix = " << uniqueShaderName( "normal_map_matrix" ) << ";" << endl;

    s << "    vec3 N_tmp = texture2D( " << uniqueShaderName( "normal_map" ) << ", gl_TexCoord[0].st ).xyz;" << endl;
    
    // texture values [0-1] -> object space normal [-1,1]
    s << "    N_tmp = vec3( normal_map_matrix * vec4( N_tmp, 1.0 ) );" << endl;
    
    if( coord_space == "TANGENT" ) {
      // from tangent to global space
      s << "    mat3 tangent_space_matrix = mat3( " << uniqueShaderName("tangent_axis") <<", " << uniqueShaderName("binormal_axis") << ", orig_normal ); " << endl;
      s << "    N_tmp = tangent_space_matrix * N_tmp; " << endl;
    } else {
      // from object to global space
      s << "    N_tmp = vec3( gl_ModelViewMatrix * vec4(N_tmp,0.0) );" << endl;
    }
    s << "    N_tmp = normalize( N_tmp ); " << endl;
  } else {
    s << "    vec3 N_tmp = orig_normal;\n" << endl;
  }
  s << "     " << normal << " = N_tmp; " << endl;

  // back_normal 
  if( separateBackColor->getValue() ) {
    if( backNormalMap->getValue() ) {
      s << "    mat4 back_normal_map_matrix = " << uniqueShaderName( "back_normal_map_matrix" ) << ";" << endl;

      s << "    vec3 back_N_tmp = texture2D( " << uniqueShaderName( "back_normal_map" ) << ", gl_TexCoord[0].st ).xyz;" << endl;
      
      // texture values [0-1] -> object space normal [-1,1]
      s << "    back_N_tmp = vec3( back_normal_map_matrix * vec4( back_N_tmp, 1.0 ) );" << endl;
      
      // to global space
      s <<      "back_N_tmp = vec3( gl_ModelViewMatrix * vec4(back_N_tmp,0.0) );" << endl;
      s <<      "back_N_tmp = -normalize( back_N_tmp ); " << endl;
    } else {
      s << "  vec3 back_N_tmp = -orig_normal;\n" << endl;
    }
    s << "   " << back_normal << " = back_N_tmp; " << endl;

  } else {
    // separateBackColor is false, just use the front colors as back colors.
    s << "  " << back_normal << " = -" << normal << "; " << endl;
  }

  return s.str();
}

string PhongShader::getBaseColors( const string &emission_color,
                                   const string &shininess, 
                                   const string &ambient_color,
                                   const string &diffuse_color,
                                   const string &specular_color,
                                   const string &normal,
                                   const string &back_emission_color,
                                   const string &back_shininess, 
                                   const string &back_ambient_color,
                                   const string &back_diffuse_color,
                                   const string &back_specular_color,
                                   const string &back_normal  ) {
  return "{\n" +
    getBaseNormal( normal, back_normal ) + 
    getBaseDiffuseColor( diffuse_color, back_diffuse_color ) + 
    getBaseSpecularColor( specular_color, back_specular_color ) + 
    getBaseAmbientColor( ambient_color, back_ambient_color ) +
    getBaseShininess( shininess, back_shininess ) +
    getBaseEmissionColor( emission_color, back_emission_color ) +
    "} \n" ;
}
