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
/// \file ShadowCasterShaders.cpp
/// \brief CPP file for ShadowCasterShaders
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShadowCasterShaders.h>
#include <H3D/ComposedShader.h>

using namespace H3D;

namespace H3D {
namespace ShadowCasterShaders {
const char *vertex_shader_passthrough_string = 
  "glsl:\n"
  "uniform mat4 modelMatrix; \n"
  "void main() {\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * modelMatrix * gl_Vertex;\n"
  "\n"
  "} \n";

const char *vertex_shader_noprojection_string = 
  "glsl:\n"
  "uniform mat4 modelMatrix; \n"
  "void main() {\n"
  "  gl_Position = gl_ModelViewMatrix * modelMatrix * gl_Vertex;\n"
  "\n"
  "} \n";

// send in world space coordinate from vertex shader
const char* vertex_shader_local_string = 
  "glsl:\n"
  "uniform mat4 modelMatrix; \n"
  "void main() {\n"
  "  gl_Position = modelMatrix * gl_Vertex;\n"
  "\n"
  "} \n";




const char *fragment_shader_string = 
  "glsl:\n"
  "void main() {\n"
  "  gl_FragColor = vec4( vec3(gl_LightSource[0].position), 1.0 ); \n"
  "} \n";
const char *shader_extension_string = 
  "#version 120\n"
  "#extension GL_EXT_geometry_shader4 : enable\n";
const char *shader_extension_string_430 = 
  "#version 430 compatibility\n"
  "#extension GL_EXT_geometry_shader4 : enable\n";

const char *geometry_shader_function_cpu = 
  "                                                                                    \n"
  "float stereo_dx_mv = matrixViewShift;                                               \n"
  "float stereo_dx_mvp = gl_ProjectionMatrix[0][0] * stereo_dx_mv;                     \n"
  "vec4 mvz = vec4(gl_ModelViewMatrix[0].z, gl_ModelViewMatrix[1].z,                   \n"
  "gl_ModelViewMatrix[2].z, gl_ModelViewMatrix[3].z);                                  \n"
  "void main(){                                                                        \n"
  "  int nr_vertices = gl_in.length();                                                  \n"
  "  int nr_triangle = nr_vertices/3;                                                  \n"
  "  for(int k = 0; k<2; ++k){                                                         \n"
  "    int eye_index = k+1;                                                            \n"
  "    int offset_sign = k *(-2)+1;                                                    \n"
  "    for(int i = 0; i<nr_triangle; i++){                                             \n"
  "      vec4 p0 = gl_in[i*3+0].gl_Position;                                           \n"
  "      vec4 p1 = gl_in[i*3+1].gl_Position;                                           \n"
  "      vec4 p2 = gl_in[i*3+2].gl_Position;                                           \n"
  "      gl_Position = gl_ModelViewProjectionMatrix*p0;                                \n"
  "      if(p0.w==0){                                                                  \n"
  "        gl_Position.x += dot(mvz,p0)*matrixProjShift*offset_sign;                   \n"
  "      }else{                                                                        \n"
  "        gl_Position.x += (stereo_dx_mvp + dot(mvz,p0)*matrixProjShift)*offset_sign; \n"
  "      }                                                                             \n"
  "      gl_ViewportIndex = eye_index;                                                 \n"
  "      EmitVertex();                                                                 \n"
  "                                                                                    \n"
  "      gl_Position = gl_ModelViewProjectionMatrix*p1;                                \n"
  "      if(p1.w==0){                                                                  \n"
  "        gl_Position.x += dot(mvz,p1)*matrixProjShift*offset_sign;                   \n"
  "      }else{                                                                        \n"
  "        gl_Position.x += (stereo_dx_mvp + dot(mvz,p1)*matrixProjShift)*offset_sign; \n"
  "      }                                                                             \n"
  "      gl_ViewportIndex = eye_index;                                                 \n"
  "      EmitVertex();                                                                 \n"
  "                                                                                    \n"
  "      gl_Position = gl_ModelViewProjectionMatrix*p2;                                \n"
  "      if(p2.w==0){                                                                  \n"
  "        gl_Position.x += dot(mvz,p2)*matrixProjShift*offset_sign;                   \n"
  "      }else{                                                                        \n"
  "        gl_Position.x += (stereo_dx_mvp + dot(mvz,p2)*matrixProjShift)*offset_sign; \n"
  "      }                                                                             \n"
  "      gl_ViewportIndex = eye_index;                                                 \n"
  "      EmitVertex();                                                                 \n"
  "      EndPrimitive();                                                               \n"
  "    }                                                                               \n"
  "  }                                                                                 \n"
  "}                                                                                   \n";

const char *geometry_shader_functions_string_sps = 
  "\n"
  "  uniform mat4 modelMatrix; \n"
  "  float facing_epsilon = -0.00001f;\n"
  "\n"
  "  vec4 p0= gl_in[0].gl_Position;                                    \n"
  "  vec4 p1= gl_in[1].gl_Position;                                    \n"
  "  vec4 p2= gl_in[2].gl_Position;                                    \n"
  "  vec4 p3= gl_in[3].gl_Position;                                    \n"
  "  vec4 p4= gl_in[4].gl_Position;                                    \n"
  "  vec4 p5= gl_in[5].gl_Position;                                    \n"
  "                                                                    \n"
  "  float stereo_dx_mv = matrixViewShift;                             \n"
  "  mat4 mvMatrix = gl_ModelViewMatrix*modelMatrix; \n"
  "  float stereo_dx_mvp = gl_ProjectionMatrix[0][0] * stereo_dx_mv;   \n"
  "  vec4 mvz = vec4(gl_ModelViewMatrix[0].z, gl_ModelViewMatrix[1].z, \n"
  "    gl_ModelViewMatrix[2].z, gl_ModelViewMatrix[3].z);              \n"
  "  vec4 mv_p0 = gl_ModelViewMatrix*p0;                               \n"
  "  vec4 mvp_p0 = gl_ProjectionMatrix*mv_p0;                          \n"
  "  float stereo_dx0 = stereo_dx_mvp + dot(mvz,p0) * matrixProjShift; \n"
  "  vec4 mvp_p1 = gl_ModelViewProjectionMatrix*p1;                    \n"
  "  float stereo_dx1 = stereo_dx_mvp + dot(mvz,p1) * matrixProjShift; \n"
  "  vec4 mv_p2 = gl_ModelViewMatrix*p2;                               \n"
  "  vec4 mvp_p2 = gl_ProjectionMatrix*mv_p2;                          \n"
  "  float stereo_dx2 = stereo_dx_mvp + dot(mvz,p2) * matrixProjShift; \n"
  "  vec4 mvp_p3 = gl_ModelViewProjectionMatrix*p3;                    \n"
  "  float stereo_dx3 = stereo_dx_mvp + dot(mvz,p3) * matrixProjShift; \n"
  "  vec4 mv_p4 = gl_ModelViewMatrix*p4;                               \n"
  "  vec4 mvp_p4 = gl_ProjectionMatrix*mv_p4;                          \n"
  "  float stereo_dx4 = stereo_dx_mvp + dot(mvz,p4) * matrixProjShift; \n"
  "  vec4 mvp_p5 = gl_ModelViewProjectionMatrix*p5;                    \n"
  "  float stereo_dx5 = stereo_dx_mvp + dot(mvz,p5) * matrixProjShift; \n"
  "// returns true if pos is on the same side of the triangle as the normal is pointing\n"
  "bool triangleFacingPos(vec3 v0, vec3 v1, vec3 v2, vec3 pos) {\n"
  "  // Normalizing is required in order to avoid some incorrectly calculated shadow triangles.\n"
  "  vec3 e0 = normalize( v1 - v0 );\n"
  "  vec3 e1 = normalize( v2 - v0 );\n"
  "  vec3 normal = cross(e1, e0);\n"
  "  return dot(normal, normalize( pos-v0 )) <= facing_epsilon;\n"
  "}\n"
  "// returns true if dir points in the opposite direction of the triangle normal\n"
  "bool triangleFacingDir(vec3 v0, vec3 v1, vec3 v2, vec3 dir) {\n"
  "  // Normalizing is required in order to avoid some incorrectly calculated shadow triangles. Yes it slows things down a bit, but otherwise shadows can actually be missing.\n"
  "  vec3 e0 = normalize( v1 - v0 );\n"
  "  vec3 e1 = normalize( v2 - v0 );\n"
  "  vec3 normal = normalize( cross(e1, e0) );\n"
  "  return dot(normal, normalize(-dir)) <= facing_epsilon;\n"
  "}\n"
  "// given a silhouette edge(v0, v1) and a point light position a quad is drawn \n"
  "// from the edge to its projection at infinity\n"
  "// all input in normalized device coordinates.\n"
  "void edgeToInfinityGeometryPointLight( vec3 v0, vec3 v1, vec3 light_pos, int offset_sign, int eye_index ) {\n"
  "  vec3 dir_v0 = v0 - light_pos;\n"
  "  vec3 dir_v1 = v1 - light_pos;\n"
  "  \n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4( v1, 1.0 );\n"
  "  gl_Position.x += (stereo_dx_mvp + dot(mvz,vec4( v1, 1.0 ))*matrixProjShift)*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4( v0, 1.0 );\n"
  "  gl_Position.x += (stereo_dx_mvp + dot(mvz,vec4( v0, 1.0 ))*matrixProjShift)*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  // for directions, the offset is different, because offset for direction will \n"
  "  // not be affected by the eye view matrix offset\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4( dir_v1, 0.0 );\n"
  "  gl_Position.x += (matrixProjShift*dot(mvz, vec4(dir_v1,0.0)))*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4( dir_v0, 0.0 );\n"
  "  gl_Position.x += (matrixProjShift*dot(mvz, vec4(dir_v0,0.0)))*offset_sign;\n"
  "  EmitVertex();\n"
  "  EndPrimitive();\n"
  "}\n"
  "\n"
  "// given a silhouette edge(v0, v1) and a directional light direction a quad is drawn \n"
  "// from the edge to its projection at infinity.\n"
  "// All input in view coordinates\n"
  "void edgeToInfinityGeometryDirLight( vec3 v0, vec3 v1, vec3 light_dir, int offset_sign, int eye_index ) {\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4(v1, 1.0);\n"
  "  gl_Position.x+= (stereo_dx_mvp + dot(mvz,vec4( v1, 1.0 ))*matrixProjShift)*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4(v0, 1.0 );\n"
  "  gl_Position.x+= (stereo_dx_mvp + dot(mvz,vec4( v0, 1.0 ))*matrixProjShift)*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4( light_dir, 0.0 );\n"
  "  // for directions, the offset is different, because offset for direction will \n"
  "  // not be affected by the eye view matrix offset\n"
  "  gl_Position.x+= (matrixProjShift*dot(mvz,vec4( light_dir, 0.0 )))*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ModelViewProjectionMatrix * vec4( light_dir, 0.0 );\n"
  "  gl_Position.x+= (matrixProjShift*dot(mvz,vec4( light_dir, 0.0 )))*offset_sign;\n"
  "  gl_ViewportIndex = eye_index;\n"
  "  EmitVertex();\n"
  "  EndPrimitive();\n"
  "}\n"
  "\n"
  "// light_pos_view is light position in local space coordinates\n"
  "void mainPointLight( vec4 light_pos_local, bool draw_caps ) {\n"
  "  vec3 light_pos = light_pos_local.xyz;\n"
  "  // return if triangle is degenerated in view space                \n"
  "  // view space shifting will not affect whether the triangle       \n"
  "  //is degenerated                                                  \n"
  "  if(mv_p0 ==mv_p2||mv_p2==mv_p4||mv_p4==mv_p0 ) return;            \n"
  "  \n"
  "  vec4 mv_light_pos = gl_ModelViewMatrix*light_pos_local;           \n"
  "  for (int i = 0; i<2; ++i){\n"
  "  int eye_index = i+1;\n"
  "  int offset_sign = int(i*(-2)+1);\n"
  "  if( triangleFacingPos( p0.xyz, \n"
  "                         p2.xyz, \n"
  "                         p4.xyz,\n"
  "                         light_pos ) ) {\n"
  "    // triangle facing light\n"
  "\n"
  "    if( draw_caps ) {\n"
  "      // render the triangle itself\n"
  "      gl_Position = mvp_p0;\n"
  "      gl_Position.x += stereo_dx0*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      gl_Position = mvp_p2;\n"
  "      gl_Position.x += stereo_dx2*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      gl_Position = mvp_p4;\n"
  "      gl_Position.x += stereo_dx4*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "\n"
  "    // if a silhouette edge, render edge to infinity.\n"
  "\n"
  "    // edge 0 silhouette edge\n"
  "    if( p0 == p1 || \n"
  "        !triangleFacingPos( p0.xyz, \n"
  "                            p1.xyz, \n"
  "                            p2.xyz,\n"
  "                            light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( p0.xyz, \n"
  "                                        p2.xyz, \n"
  "                                        light_pos, \n"
  "                                        offset_sign,\n"
  "                                        eye_index );\n"
  "    }\n"
  "\n"
  "    // edge 1 silhouette edge\n"
  "    if( p2 == p3 || \n"
  "       !triangleFacingPos( p2.xyz, \n"
  "                           p3.xyz, \n"
  "                           p4.xyz,\n"
  "                           light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( p2.xyz, \n"
  "                                        p4.xyz, \n"
  "                                        light_pos,\n"
  "                                        offset_sign,\n"
  "                                        eye_index );\n"
  "\n"
  "    }\n"
  "\n"
  "    // edge 2 silhouette edge\n"
  "    if( p4 == p5 || \n"
  "        !triangleFacingPos( p4.xyz, \n"
  "                            p5.xyz, \n"
  "                            p0.xyz,\n"
  "                            light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( p4.xyz, \n"
  "                                        p0.xyz, \n"
  "                                        light_pos, \n"
  "                                        offset_sign,\n"
  "                                        eye_index );\n"
  "\n"
  "    }\n"
  "  } else {\n"
  "    if( draw_caps ) {\n"
  "      // triangle facing away from light.\n"
  "      vec3 dir_v0 = gl_PositionIn[0].xyz - light_pos;\n"
  "      vec3 dir_v1 = gl_PositionIn[2].xyz - light_pos;\n"
  "      vec3 dir_v2 = gl_PositionIn[4].xyz - light_pos;\n"
  "      \n"
  "      gl_Position = gl_ModelViewProjectionMatrix * vec4( dir_v0, 0.0 );\n"
  "      //gl_Position.x += (stereo_dx_mvp + dot(mvz,vec4( dir_v0, 0.0 ))*matrixProjShift)*offset_sign;\n"
  "      gl_Position.x += ( dot(mvz,vec4( dir_v0, 0.0 ))*matrixProjShift)*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ModelViewProjectionMatrix * vec4( dir_v1, 0.0 );\n"
  "      // for directions, the offset is different, because offset for direction will \n"
  "      // not be affected by the eye view space offset\n"
  "      gl_Position.x += ( dot(mvz,vec4( dir_v1, 0.0 ))*matrixProjShift)*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ModelViewProjectionMatrix * vec4( dir_v2, 0.0 );\n"
  "      gl_Position.x += ( dot(mvz,vec4( dir_v2, 0.0 ))*matrixProjShift)*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "  }\n"
  "  }\n"
  "}\n"
  "\n"
  "\n"
  "// light_dir is in local coordinates\n"
  "void mainDirectionalLight( vec3 light_dir, bool draw_caps ) {\n"
  "\n"
  "  // return if triangle is degenerated in view space                \n"
  "  // view space shifting will not affect whether the triangle       \n"
  "  //is degenerated                                                  \n"
  "  if(mv_p0 ==mv_p2||mv_p2==mv_p4||mv_p4==mv_p0 ) return;            \n"
  "\n"
  "  for (int i = 0; i<2; ++i){\n"
  "  int eye_index = i+1;\n"
  "  int offset_sign = int(i*(-2)+1);\n"
  "  if( triangleFacingDir( p0.xyz, p2.xyz, p4.xyz, light_dir ) ) {\n"
  "\n"
  "    if( draw_caps ) {\n"
  "      // render the triangle itself\n"
  "      gl_Position = mvp_p0;\n"
  "      gl_Position.x += stereo_dx0*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      gl_Position = mvp_p2;\n"
  "      gl_Position.x += stereo_dx2*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      gl_Position = mvp_p4;\n"
  "      gl_Position.x += stereo_dx4*offset_sign;\n"
  "      gl_ViewportIndex = eye_index;\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "   \n"
  "    // edge 0 silhouette edge\n"
  "    if( p0 == p1 || !triangleFacingDir( p0.xyz, p1.xyz, p2.xyz, light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p0.xyz, p2.xyz, light_dir, offset_sign,eye_index );\n"
  "    }\n"
  "\n"
  "    // edge 1 silhouette edge\n"
  "    if( p2 == p3 || !triangleFacingDir( p2.xyz, p3.xyz, p4.xyz,light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p2.xyz, p4.xyz,light_dir, offset_sign,eye_index );\n"
  "    }\n"
  "\n"
  "    // edge 2 silhouette edge\n"
  "    if( p4 == p5 || !triangleFacingDir( p4.xyz, p5.xyz, p0.xyz, light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p4.xyz, p0.xyz, light_dir, offset_sign,eye_index );\n"
  "    }\n"
  "  }\n"
  "  }\n"
  "}\n";
  ;

const char *geometry_shader_functions_string = 
  "\n"
  "// returns true if the incoming triangle does not have an area \n"
  "bool isTriangleDegenerate() { \n"
  "  return( gl_PositionIn[0] == gl_PositionIn[2] || \n" 
  "          gl_PositionIn[2] == gl_PositionIn[4] || \n"
  "          gl_PositionIn[4] == gl_PositionIn[0] ); \n"
  "} \n"
  "\n"
  "float facing_epsilon = -0.00001f;\n"
  "\n"
  "// returns true if pos is on the same side of the triangle as the normal is pointing\n"
  "bool triangleFacingPos(vec3 v0, vec3 v1, vec3 v2, vec3 pos) {\n"
  "  // Normalizing is required in order to avoid some incorrectly calculated shadow triangles.\n"
  "  vec3 e0 = normalize( v1 - v0 );\n"
  "  vec3 e1 = normalize( v2 - v0 );\n"
  "  vec3 normal = cross(e1, e0);\n"
  "  return dot(normal, normalize( pos-v0 )) <= facing_epsilon;\n"
  "}\n"
  "\n"
  "// returns true if dir points in the opposite direction of the triangle normal\n"
  "bool triangleFacingDir(vec3 v0, vec3 v1, vec3 v2, vec3 dir) {\n"
  "  // Normalizing is required in order to avoid some incorrectly calculated shadow triangles. Yes it slows things down a bit, but otherwise shadows can actually be missing.\n"
  "  vec3 e0 = normalize( v1 - v0 );\n"
  "  vec3 e1 = normalize( v2 - v0 );\n"
  "  vec3 normal = normalize( cross(e1, e0) );\n"
  "  return dot(normal, normalize(-dir)) <= facing_epsilon;\n"
  "}\n"
  "\n"
  "// given a silhouette edge(v0, v1) and a point light position a quad is drawn \n"
  "// from the edge to its projection at infinity\n"
  "// all input in normalized device coordinates.\n"
  "void edgeToInfinityGeometryPointLight( vec3 v0, vec3 v1, vec3 light_pos ) {\n"
  "  vec3 dir_v0 = v0 - light_pos;\n"
  "  vec3 dir_v1 = v1 - light_pos;\n"
  "  \n"
  "  gl_Position = gl_ProjectionMatrix * vec4( v1, 1.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( v0, 1.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( dir_v1, 0.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( dir_v0, 0.0 );\n"
  "  EmitVertex();\n"
  "  EndPrimitive();\n"
  "}\n"
  "\n"
  "// given a silhouette edge(v0, v1) and a directional light direction a quad is drawn \n"
  "// from the edge to its projection at infinity.\n"
  "// All input in view coordinates\n"
  "void edgeToInfinityGeometryDirLight( vec3 v0, vec3 v1, vec3 light_dir ) {\n"
  "  gl_Position = gl_ProjectionMatrix * vec4(v1, 1.0);\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4(v0, 1.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( light_dir, 0.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( light_dir, 0.0 );\n"
  "  EmitVertex();\n"
  "  EndPrimitive();\n"
  "}\n"
  "\n"
  "// light_pos_view is light position in view coordinates\n"
  "void mainPointLight( vec4 light_pos_view, bool draw_caps ) {\n"
  "  vec3 light_pos = light_pos_view.xyz;\n"
  "  \n"
  "  if( triangleFacingPos( gl_PositionIn[0].xyz, \n"
  "                         gl_PositionIn[2].xyz, \n"
  "                         gl_PositionIn[4].xyz,\n"
  "                         light_pos ) ) {\n"
  "    // triangle facing light\n"
  "\n"
  "    if( draw_caps ) {\n"
  "      // render the triangle itself\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[0];\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[2];\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[4];\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "\n"
  "    // if a silhouette edge, render edge to infinity.\n"
  "\n"
  "    // edge 0 silhouette edge\n"
  "    if( gl_PositionIn[0] == gl_PositionIn[1] || \n"
  "        !triangleFacingPos( gl_PositionIn[0].xyz, \n"
  "                            gl_PositionIn[1].xyz, \n"
  "                            gl_PositionIn[2].xyz,\n"
  "                            light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( gl_PositionIn[0].xyz, \n"
  "                                        gl_PositionIn[2].xyz, \n"
  "                                        light_pos );\n"
  "    }\n"
  "\n"
  "    // edge 1 silhouette edge\n"
  "    if( gl_PositionIn[2] == gl_PositionIn[3] || \n"
  "       !triangleFacingPos( gl_PositionIn[2].xyz, \n"
  "                           gl_PositionIn[3].xyz, \n"
  "                           gl_PositionIn[4].xyz,\n"
  "                           light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( gl_PositionIn[2].xyz, \n"
  "                                        gl_PositionIn[4].xyz, \n"
  "                                        light_pos );\n"
  "\n"
  "    }\n"
  "\n"
  "    // edge 2 silhouette edge\n"
  "    if( gl_PositionIn[4] == gl_PositionIn[5] || \n"
  "        !triangleFacingPos( gl_PositionIn[4].xyz, \n"
  "                            gl_PositionIn[5].xyz, \n"
  "                            gl_PositionIn[0].xyz,\n"
  "                            light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( gl_PositionIn[4].xyz, \n"
  "                                        gl_PositionIn[0].xyz, \n"
  "                                        light_pos );\n"
  "\n"
  "    }\n"
  "  } else {\n"
  "    if( draw_caps ) {\n"
  "      // triangle facing away from light.\n"
  "      vec3 dir_v0 = gl_PositionIn[0].xyz - light_pos;\n"
  "      vec3 dir_v1 = gl_PositionIn[2].xyz - light_pos;\n"
  "      vec3 dir_v2 = gl_PositionIn[4].xyz - light_pos;\n"
  "      \n"
  "      gl_Position = gl_ProjectionMatrix * vec4( dir_v0, 0.0 );\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * vec4( dir_v1, 0.0 );\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * vec4( dir_v2, 0.0 );\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "  }\n"
  "}\n"
  "\n"
  "\n"
  "// light_dir is in view coordinates\n"
  "void mainDirectionalLight( vec3 light_dir, bool draw_caps ) {\n"
  "\n"
  "  // transforming to view space\n"
  "  vec3 p0 = gl_PositionIn[0].xyz;\n"
  "  vec3 p1 = gl_PositionIn[1].xyz;\n"
  "  vec3 p2 = gl_PositionIn[2].xyz;\n"
  "  vec3 p3 = gl_PositionIn[3].xyz;\n"
  "  vec3 p4 = gl_PositionIn[4].xyz;\n"
  "  vec3 p5 = gl_PositionIn[5].xyz;\n"
  "\n"
  "  // if a silhouette edge, render edge to infinity.\n"
  "  if( triangleFacingDir( p0, p2, p4, light_dir ) ) {\n"
  "\n"
  "    if( draw_caps ) {\n"
  "      // render the triangle itself(only when drawing caps)\n"
  "      // no back cap needed for directional lights since all points \n"
  "      // converge to the same point at infinity.\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[0];\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix *  gl_PositionIn[2];\n"
  "      EmitVertex();\n"
  "      gl_Position =  gl_ProjectionMatrix * gl_PositionIn[4];\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "   \n"
  "    // edge 0 silhouette edge\n"
  "    if( p0 == p1 || !triangleFacingDir( p0, p1, p2, light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p0, p2, light_dir );\n"
  "    }\n"
  "\n"
  "    // edge 1 silhouette edge\n"
  "    if( p2 == p3 || !triangleFacingDir( p2, p3, p4,light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p2, p4,light_dir );\n"
  "    }\n"
  "\n"
  "    // edge 2 silhouette edge\n"
  "    if( p4 == p5 || !triangleFacingDir( p4, p5, p0, light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p4, p0, light_dir );\n"
  "    }\n"
  "  }\n"
  "}\n";



  string getPointLightGeometryShaderString( bool draw_caps, bool single_pass_stereo=false, 
    float matrixViewShift = 0, float matrixProjShift = 0  ) {
    stringstream s;
    
    s << "glsl:" << endl;
    if( single_pass_stereo ) {
      s << shader_extension_string_430<<endl;
      s << "const float matrixProjShift = "<< matrixProjShift <<";"<< endl;
      s << "const float matrixViewShift = "<< matrixViewShift <<";"<<endl;
      s << geometry_shader_functions_string_sps <<endl;
      s << "uniform vec3 lightPosition; " << endl;
      s << "vec4 lightP = vec4( lightPosition, 1.0 );"<<endl;
    }else{
      s << shader_extension_string<<endl;
      s << geometry_shader_functions_string << endl;
      s << "uniform vec3 lightPosition; " << endl;
      s << "vec4 lightP = gl_ModelViewMatrix * vec4( lightPosition, 1.0 );"<<endl;
    }
    
    s << "void main() { " << endl;
    if (!single_pass_stereo){
      s << "  if( isTriangleDegenerate() ) return; " << endl;
    }
    if( draw_caps ) {
      s << "  mainPointLight( lightP, true ); " << endl; 
    } else {
      s << "  mainPointLight( lightP, false ); " << endl; 
    }
    s << "}" << endl;
    return s.str();
  }

  string getDirLightGeometryShaderString( bool draw_caps, bool single_pass_stereo=false, 
    float matrixViewShift = 0, float matrixProjShift = 0) {
    stringstream s;
    s << "glsl:" << endl;
    if( single_pass_stereo ) {
      s << shader_extension_string_430 <<endl;
      s << "const float matrixProjShift = "<< matrixProjShift <<";"<< endl;
      s << "const float matrixViewShift = "<< matrixViewShift <<";"<<endl;
      s << geometry_shader_functions_string_sps <<endl;
      s << "uniform vec3 lightDirection; " << endl;
      s << "vec4 lightDir = vec4( lightDirection, 0.0 ); "<<endl;
    }else{
      s << shader_extension_string <<endl;
      s << geometry_shader_functions_string << endl;
      s << "uniform vec3 lightDirection; " << endl;
      s << "vec4 lightDir = gl_ModelViewMatrix * vec4( lightDirection, 0.0 ); "<<endl;
    }
    s << "uniform bool drawCaps; " << endl;
    s << "void main() { " << endl;
    if( !single_pass_stereo ) {
      s << "  if( isTriangleDegenerate() ) return; " << endl;
    }
    if( draw_caps ) {
      s << "  mainDirectionalLight( lightDir.xyz, true ); " << endl;
    } else {
      s << "  mainDirectionalLight( lightDir.xyz, false ); " << endl;
    }
    s << "}" << endl;
    return s.str();
  }

  string getVertexShaderString( bool cpu_shadows, bool draw_caps, bool dir_light, bool single_pass_stereo ) {
    if( cpu_shadows ) {
      if (single_pass_stereo){
        return vertex_shader_local_string;
      }else{
        return vertex_shader_passthrough_string;
      }
    } else {
      if( single_pass_stereo ) {
        return vertex_shader_local_string;
      }else{
        return vertex_shader_noprojection_string;
      }
    }
  }

  string getFragmentShaderString( bool cpu_shadows, bool draw_caps, bool dir_light, bool single_pass_stereo ) {
    return fragment_shader_string;
  }

  string getGeometryShaderString( bool cpu_shadows, bool draw_caps, bool dir_light, bool single_pass_stereo, float matrixViewShift, float matrixProjShift ) {
    if( !cpu_shadows ) {
      if( dir_light ) return getDirLightGeometryShaderString( draw_caps, single_pass_stereo, matrixViewShift, matrixProjShift );
      else return getPointLightGeometryShaderString( draw_caps, single_pass_stereo, matrixViewShift, matrixProjShift );
    } else {
      stringstream s;
      s << 
        "glsl:\n"
        "#version 430 compatibility                                                          \n"
        "  layout(triangles) in;                                                             \n"
        "layout(triangle_strip, max_vertices = 255) out;                                      \n";
      s <<"const float matrixProjShift = " << matrixProjShift<<";"<<endl;
      s <<"const float matrixViewShift = " << matrixViewShift<<";"<<endl;
      s << geometry_shader_function_cpu;
      return s.str();
    }
  }
  
  // the current shader node
  AutoRef< ComposedShader > shader_node;

  // the setup of the current shader node
  bool current_cpu_shadows, current_draw_caps, current_dir_light, current_single_pass_stereo;

  float current_projShift, current_viewShift;

  GLint matrix_uniform, light_uniform;
  
  string current_vertex_shader, current_geometry_shader, current_fragment_shader;

}
}


void H3D::ShadowCasterShaders::shaderInit( bool cpu_shadows, bool draw_caps, 
  bool dir_light, bool single_pass_stereo, float matrixViewShift, float matrixProjShift )
{
  float diff_threshold = 0.00001;
  if( !shader_node.get() ||
      current_cpu_shadows != cpu_shadows ||
      current_draw_caps != draw_caps ||
      current_dir_light != dir_light ||
      current_single_pass_stereo != single_pass_stereo ||
      ( current_single_pass_stereo == true&&
        (
        H3DAbs(current_projShift - matrixProjShift) > diff_threshold ||
        H3DAbs(current_viewShift - matrixViewShift) > diff_threshold
        )
      )
    ) {

    // regenerate shader
    shader_node.reset( new ComposedShader );

    if( !cpu_shadows ) {
      shader_node->geometryInputType->setValue( "TRIANGLES_ADJACENCY" );
      shader_node->geometryOutputType->setValue( "TRIANGLE_STRIP" );
    
      if( dir_light ) {
        // add field for uniform variable lightDirection.
        Field *lightDir = new SFVec3f;
        shader_node->addField( "lightDirection", Field::INPUT_OUTPUT, lightDir );
      } else {
        // add field for uniform variable lightPosition.
        Field *lightPos = new SFVec3f;
        shader_node->addField( "lightPosition", Field::INPUT_OUTPUT, lightPos );
      }
    
    
    }

    Field *model_matrix = new SFMatrix4f;
    shader_node->addField( "modelMatrix", 
                                Field::INPUT_OUTPUT, model_matrix );

    // vertex shader
    ShaderPart *vertex_shader = new ShaderPart;
    vertex_shader->type->setValue( "VERTEX" );
    current_vertex_shader = getVertexShaderString( cpu_shadows, draw_caps, dir_light, single_pass_stereo );
    vertex_shader->url->push_back( current_vertex_shader );
    shader_node->parts->push_back( vertex_shader );

    
    // geometry shader
    if( !cpu_shadows ) {
      ShaderPart *geom_shader = new ShaderPart;
      geom_shader->type->setValue( "GEOMETRY" );
      current_geometry_shader = getGeometryShaderString( cpu_shadows, draw_caps, dir_light, single_pass_stereo, matrixViewShift, matrixProjShift );
      geom_shader->url->push_back( current_geometry_shader );
      shader_node->parts->push_back( geom_shader );
    }else{
      if (single_pass_stereo){
        ShaderPart *geom_shader = new ShaderPart;
        geom_shader->type->setValue( "GEOMETRY" );
        current_geometry_shader = getGeometryShaderString( cpu_shadows, draw_caps, dir_light, single_pass_stereo, matrixViewShift, matrixProjShift );
        geom_shader->url->push_back( current_geometry_shader );
      }
    }

    // fragment shader
    ShaderPart *fragment_shader = new ShaderPart;
    fragment_shader->type->setValue( "FRAGMENT" );
    current_fragment_shader = getFragmentShaderString( cpu_shadows, draw_caps, dir_light, single_pass_stereo );
    fragment_shader->url->push_back( current_fragment_shader );
    shader_node->parts->push_back( fragment_shader );  

    current_cpu_shadows = cpu_shadows;
    current_draw_caps = draw_caps;
    current_dir_light = dir_light;
    current_single_pass_stereo = single_pass_stereo;
    current_projShift = matrixProjShift;
    current_viewShift = matrixViewShift;
  }

  shader_node->preRender();
  shader_node->displayList ->callList();

  ShadowCasterShaders::matrix_uniform =  glGetUniformLocationARB( shader_node->getProgramHandle(), "modelMatrix" );
  
  if( !cpu_shadows ) {
    if( dir_light ) {
      ShadowCasterShaders::light_uniform =  glGetUniformLocationARB( shader_node->getProgramHandle(), "lightDirection" );
    } else {
      ShadowCasterShaders::light_uniform =  glGetUniformLocationARB( shader_node->getProgramHandle(), "lightPosition" );
    }
  }
}

void ShadowCasterShaders::shaderClean() {
  // disable shaders
  shader_node->postRender();
}

bool ShadowCasterShaders::setTransformMatrix( const Matrix4f&m ) {
  if( ShadowCasterShaders::matrix_uniform != -1 ) {
    glUniformMatrix4fvARB( ShadowCasterShaders::matrix_uniform, 1, true, m[0] );
  }
  return ShadowCasterShaders::matrix_uniform != -1;
}

bool ShadowCasterShaders::setDirectionalLightDir( const Vec3f &v ) {
  if( ShadowCasterShaders::light_uniform != -1 ) {
    glUniform3fARB( ShadowCasterShaders::light_uniform, (GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z );
  }
  return ShadowCasterShaders::light_uniform != -1;
}

bool ShadowCasterShaders::setPointLightPosition( const Vec3f &p ) {
  if( ShadowCasterShaders::light_uniform != -1 ) {
    glUniform3fARB( ShadowCasterShaders::light_uniform, (GLfloat)p.x, (GLfloat)p.y, (GLfloat)p.z );
  }
  return ShadowCasterShaders::light_uniform != -1;
}

void ShadowCasterShaders::shaderToggle( bool on ) {
  if( shader_node.get() ) {
    if( on ) {
      glUseProgramObjectARB( shader_node->getProgramHandle() ); 
    } else {
      glUseProgramObjectARB( 0 );
    }
  }
}
