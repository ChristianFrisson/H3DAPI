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
/// \file GraphicsHardwareInfo.cpp
/// \brief CPP file for GraphicsHardwareInfo.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GraphicsHardwareInfo.h>
#include <GL/glew.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GraphicsHardwareInfo::database( "GraphicsHardwareInfo", 
                                                &(newInstance<GraphicsHardwareInfo>), 
                                                typeid( GraphicsHardwareInfo ),
                                                &X3DChildNode::database );

bool GraphicsHardwareInfo::info_initialized = false;
GraphicsHardwareInfo::Info GraphicsHardwareInfo::info;

namespace GraphicsHardwareInfoInternals {
  FIELDDB_ELEMENT( GraphicsHardwareInfo, initialized, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, vendor, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, renderer, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, version, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, shadingLanguageVersion, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, extensions, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, max3dTextureSize, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxClientAttribStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxAttribStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxClipPlanes, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxColorMatrixStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxCombinedTextureImageUnits, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxCubeMapTextureSize, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxDrawBuffers, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxElementsIndices, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxElementsVertices, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxEvalOrder, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxFragmentUniformComponents, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxLights, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxListNesting, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxModelviewStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxNameStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxPixelMapTable, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxProjectionStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxTextureCoords, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxTextureImageUnits, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxTextureLODBias, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxTextureSize, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxTextureStackDepth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxTextureUnits, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxVaryingFloats, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxVertexAttribs, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxVertexTextureImageUnits, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxVertexUniformComponents, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxViewportWidth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GraphicsHardwareInfo, maxViewportHeight, OUTPUT_ONLY );
}


GraphicsHardwareInfo::GraphicsHardwareInfo( Inst< SFNode      > _metadata,
                                            Inst< SFBool  > _initialized,
                                            Inst< SFString > _vendor,
                                            Inst< SFString > _renderer,
                                            Inst< SFString > _version,
                                            Inst< SFString > _shadingLanguageVersion,
                                            Inst< SFString > _extensions,
                                            Inst< SFInt32 > _max3dTextureSize,
                                            Inst< SFInt32 > _maxClientAttribStackDepth,
                                            Inst< SFInt32 > _maxAttribStackDepth,
                                            Inst< SFInt32 > _maxClipPlanes,
                                            Inst< SFInt32 > _maxColorMatrixStackDepth,
                                            Inst< SFInt32 > _maxCombinedTextureImageUnits,
                                            Inst< SFInt32 > _maxCubeMapTextureSize,
                                            Inst< SFInt32 > _maxDrawBuffers,
                                            Inst< SFInt32 > _maxElementsIndices,
                                            Inst< SFInt32 > _maxElementsVertices,
                                            Inst< SFInt32 > _maxEvalOrder,
                                            Inst< SFInt32 > _maxFragmentUniformComponents,
                                            Inst< SFInt32 > _maxLights,
                                            Inst< SFInt32 > _maxListNesting,
                                            Inst< SFInt32 > _maxModelviewStackDepth,
                                            Inst< SFInt32 > _maxNameStackDepth,
                                            Inst< SFInt32 > _maxPixelMapTable,
                                            Inst< SFInt32 > _maxProjectionStackDepth,
                                            Inst< SFInt32 > _maxTextureCoords,
                                            Inst< SFInt32 > _maxTextureImageUnits,
                                            Inst< SFInt32 > _maxTextureLODBias,
                                            Inst< SFInt32 > _maxTextureSize,
                                            Inst< SFInt32 > _maxTextureStackDepth,
                                            Inst< SFInt32 > _maxTextureUnits,
                                            Inst< SFInt32 > _maxVaryingFloats,
                                            Inst< SFInt32 > _maxVertexAttribs,
                                            Inst< SFInt32 > _maxVertexTextureImageUnits,
                                            Inst< SFInt32 > _maxVertexUniformComponents,
                                            Inst< SFInt32 > _maxViewportWidth,
                                            Inst< SFInt32 > _maxViewportHeight):
  X3DChildNode( _metadata ),
  initialized( _initialized ),
  vendor( _vendor ),
  renderer( _renderer ),
  version( _version ),
  shadingLanguageVersion( _shadingLanguageVersion ),
  extensions( _extensions ),
  max3dTextureSize( _max3dTextureSize ),
  maxClientAttribStackDepth( _maxClientAttribStackDepth ),
  maxAttribStackDepth( _maxAttribStackDepth ),
  maxClipPlanes( _maxClipPlanes ),
  maxColorMatrixStackDepth( _maxColorMatrixStackDepth ),
  maxCombinedTextureImageUnits( _maxCombinedTextureImageUnits ),
  maxCubeMapTextureSize( _maxCubeMapTextureSize ),
  maxDrawBuffers( _maxDrawBuffers),
  maxElementsIndices( _maxElementsIndices ),
  maxElementsVertices( _maxElementsVertices ),
  maxEvalOrder( _maxEvalOrder ),
  maxFragmentUniformComponents( _maxFragmentUniformComponents ),
  maxLights( _maxLights ),
  maxListNesting( _maxListNesting ),
  maxModelviewStackDepth( _maxModelviewStackDepth ),
  maxNameStackDepth( _maxNameStackDepth ),
  maxPixelMapTable( _maxPixelMapTable ),
  maxProjectionStackDepth( _maxProjectionStackDepth ),
  maxTextureCoords( _maxTextureCoords ),
  maxTextureImageUnits( _maxTextureImageUnits ),
  maxTextureLODBias( _maxTextureLODBias ),
  maxTextureSize( _maxTextureSize ),
  maxTextureStackDepth(_maxTextureStackDepth ),
  maxTextureUnits( _maxTextureUnits ),
  maxVaryingFloats( _maxVaryingFloats ),
  maxVertexAttribs( _maxVertexAttribs ),
  maxVertexTextureImageUnits( _maxVertexTextureImageUnits ),
  maxVertexUniformComponents( _maxVertexUniformComponents ),
  maxViewportWidth( _maxViewportWidth ),
  maxViewportHeight( _maxViewportHeight ){

  type_name = "GraphicsHardwareInfo";
  database.initFields( this );

  initialized->setValue( false, id );
}

string glGetStlString( GLenum name ) {
  string return_string = "";
  const GLubyte * v = glGetString( name );
  if( v == 0 ) {
    glGetError();
    // The error is not important. It is GL_INVALID_ENUM.
    // We just want to clear the error stack.
  } else
    return_string = (const char *)v;
  return return_string;
}

bool GraphicsHardwareInfo::initializeInfo() {
  
  info.vendor = glGetStlString( GL_VENDOR );
  info.renderer = glGetStlString( GL_RENDERER );
  info.version = glGetStlString( GL_VERSION );
  info.shading_language_version = glGetStlString( GL_SHADING_LANGUAGE_VERSION );
  info.extensions = glGetStlString( GL_EXTENSIONS );

  GLint v;
  glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &v );
  info.max_3d_texture_size = v;
  glGetIntegerv( GL_MAX_ATTRIB_STACK_DEPTH, &v );
  info.max_attrib_stack_depth = v;
  glGetIntegerv( GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &v );
  info.max_client_attrib_stack_depth = v;
  glGetIntegerv( GL_MAX_CLIP_PLANES, &v );
  info.max_clip_planes = v;
  glGetIntegerv( GL_MAX_COLOR_MATRIX_STACK_DEPTH, &v );
  info.max_color_matrix_stack_depth = v;
  glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &v );
  info.max_combined_texture_image_units = v;
  glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, &v );
  info.max_cube_map_texture_size = v;
  glGetIntegerv( GL_MAX_DRAW_BUFFERS, &v );
  info.max_draw_buffers = v;
  glGetIntegerv( GL_MAX_ELEMENTS_INDICES, &v );
  info.max_elements_indices = v;
  glGetIntegerv( GL_MAX_ELEMENTS_VERTICES, &v );
  info.max_elements_vertices = v;
  glGetIntegerv( GL_MAX_EVAL_ORDER, &v );
  info.max_eval_order = v;
  glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &v );
  info.max_fragment_uniform_components = v;
  glGetIntegerv( GL_MAX_LIGHTS, &v );
  info.max_lights = v;
  glGetIntegerv( GL_MAX_LIST_NESTING, &v );
  info.max_list_nesting = v;
  glGetIntegerv( GL_MAX_MODELVIEW_STACK_DEPTH, &v );
  info.max_modelview_stack_depth = v;
  glGetIntegerv( GL_MAX_NAME_STACK_DEPTH, &v );
  info.max_name_stack_depth = v;
  glGetIntegerv( GL_MAX_PIXEL_MAP_TABLE, &v );
  info.max_pixel_map_table = v;
  glGetIntegerv( GL_MAX_PROJECTION_STACK_DEPTH, &v );
  info.max_projection_stack_depth = v;
  glGetIntegerv( GL_MAX_TEXTURE_COORDS, &v );
  info.max_texture_coords = v;
  glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &v );
  info.max_texture_image_units = v;
  glGetIntegerv( GL_MAX_TEXTURE_LOD_BIAS, &v );
  info.max_texture_lod_bias = v;
  glGetIntegerv( GL_MAX_TEXTURE_SIZE, &v );
  info.max_texture_size = v;
  glGetIntegerv( GL_MAX_TEXTURE_STACK_DEPTH, &v );
  info.max_texture_stack_depth = v;
  glGetIntegerv( GL_MAX_TEXTURE_UNITS, &v );
  info.max_texture_units = v;
#ifdef GLEW_ARB_shader_image_load_store
  if (GLEW_ARB_shader_image_load_store){
    glGetIntegerv(GL_MAX_IMAGE_UNITS, &v);
    info.max_image_units = v;
  }
#endif // GLEW_ARB_shader_image_load_store

  glGetIntegerv( GL_MAX_VARYING_FLOATS, &v );
  info.max_varying_floats = v;
  glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &v );
  info.max_vertex_attribs = v;
  glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &v );
  info.max_vertex_texture_image_units = v;
  glGetIntegerv( GL_MAX_VERTEX_UNIFORM_COMPONENTS, &v );
  info.max_vertex_uniform_components = v;
#ifdef GLEW_ARB_framebuffer_object
  if( GLEW_ARB_framebuffer_object ) {
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &v);
    info.max_color_attachments = v;
    glGetIntegerv( GL_MAX_SAMPLES, &v );
    info.max_samples = v;
  }
#endif
  GLint vp[2];
  glGetIntegerv( GL_MAX_VIEWPORT_DIMS, vp );
  info.max_viewport_width = vp[0];
  info.max_viewport_height = vp[1];

  info.support_ARB_texture_float = false;
#ifdef GLEW_ARB_texture_float
  if( GLEW_ARB_texture_float ) {
    info.support_ARB_texture_float = true;
  }
#endif
  info.support_ARB_depth_buffer_float = false;
#ifdef GLEW_ARB_depth_buffer_float
  if( GLEW_ARB_depth_buffer_float ) {
    info.support_ARB_depth_buffer_float = true;
  }
#endif
  info.support_EXT_packed_depth_stencil = false;
#ifdef GLEW_EXT_packed_depth_stencil
  if( GLEW_EXT_packed_depth_stencil ) {
    info.support_EXT_packed_depth_stencil = true;
  }
#endif

  info_initialized = true;

  return true;
}

void GraphicsHardwareInfo::initialize() {
  initialized->setValue( infoIsInitialized(), id );
  if( infoIsInitialized() ) {
    vendor->setValue( info.vendor, id );
    renderer->setValue( info.renderer, id );
    version->setValue(info.version, id );
    shadingLanguageVersion->setValue( info.shading_language_version, id );
    extensions->setValue( info.extensions, id );

    max3dTextureSize->setValue( info.max_3d_texture_size, id );
    maxClientAttribStackDepth->setValue( info.max_client_attrib_stack_depth, id );
    maxAttribStackDepth->setValue( info.max_attrib_stack_depth, id );
    maxClipPlanes->setValue( info.max_clip_planes, id );
    maxColorMatrixStackDepth->setValue( info.max_color_matrix_stack_depth, id );
    maxCombinedTextureImageUnits->setValue( info.max_combined_texture_image_units, id );
    maxCubeMapTextureSize->setValue( info.max_cube_map_texture_size, id );
    maxDrawBuffers->setValue( info.max_draw_buffers, id );
    maxElementsIndices->setValue( info.max_elements_indices, id );
    maxElementsVertices->setValue( info.max_elements_vertices, id );
    maxEvalOrder->setValue( info.max_eval_order, id );
    maxFragmentUniformComponents->setValue( info.max_fragment_uniform_components, id );
    maxLights->setValue( info.max_lights, id );
    maxListNesting->setValue( info.max_list_nesting, id );
    maxModelviewStackDepth->setValue( info.max_modelview_stack_depth, id );
    maxNameStackDepth->setValue( info.max_name_stack_depth, id );
    maxPixelMapTable->setValue( info.max_pixel_map_table, id );
    maxProjectionStackDepth->setValue( info.max_projection_stack_depth, id );
    maxTextureCoords->setValue( info.max_texture_coords, id );
    maxTextureImageUnits->setValue( info.max_texture_image_units, id );
    maxTextureLODBias->setValue( info.max_texture_lod_bias, id );
    maxTextureSize->setValue( info.max_texture_size, id );
    maxTextureStackDepth->setValue( info.max_texture_stack_depth, id );
    maxTextureUnits->setValue( info.max_texture_units, id );
    maxVaryingFloats->setValue( info.max_varying_floats, id );
    maxVertexAttribs->setValue( info.max_vertex_attribs, id );
    maxVertexTextureImageUnits->setValue( info.max_vertex_texture_image_units, id );
    maxVertexUniformComponents->setValue( info.max_vertex_uniform_components, id );
    maxViewportWidth->setValue( info.max_viewport_width, id );
    maxViewportHeight->setValue( info.max_viewport_height, id );
  }
}




