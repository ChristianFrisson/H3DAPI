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
/// \file GraphicsHardwareInfo.h
/// \brief Header file for GraphicsHardwareInfo.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GRAPHICSHARDWAREINFO_H__
#define __GRAPHICSHARDWAREINFO_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class GraphicsHardwareInfo
  /// The GraphicsHardwareInfo class provides information about the graphics
  /// hardware the program is being run on and its capabilities.
  ///
  /// One can either use the GraphicsHardwareInfo node to get the info or
  /// use the static function GraphicsHardwareInfo::getInfo directly.
  /// 
  /// The values are the same as can be get by the glGet function. See
  /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glGet.xml">OpenGL reference documentation</a> for that.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GraphicsHardwareInfo.x3d">GraphicsHardwareInfo.x3d</a>
  ///     ( <a href="examples/GraphicsHardwareInfo.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/GraphicsHardwareInfo1.x3d">GraphicsHardwareInfo1.x3d</a>
  ///     ( <a href="examples/GraphicsHardwareInfo1.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile GraphicsHardwareInfo.dot
  class H3DAPI_API GraphicsHardwareInfo : public X3DChildNode {
  public:

    /// Constructor.
    GraphicsHardwareInfo( Inst< SFNode  > _metadata = 0,
			  Inst< SFBool  > _initialized = 0,
			  Inst< SFString > _vendor = 0,
			  Inst< SFString > _renderer = 0,
			  Inst< SFString > _version = 0,
			  Inst< SFString > _shadingLanguageVersion = 0,
			  Inst< SFString > _extensions = 0,
			  Inst< SFInt32 > _max3dTextureSize = 0,
			  Inst< SFInt32 > _maxClientAttribStackDepth = 0,
			  Inst< SFInt32 > _maxAttribStackDepth = 0,
			  Inst< SFInt32 > _maxClipPlanes = 0,
			  Inst< SFInt32 > _maxColorMatrixStackDepth = 0,
			  Inst< SFInt32 > _maxCombinedTextureImageUnits = 0,
			  Inst< SFInt32 > _maxCubeMapTextureSize = 0,
			  Inst< SFInt32 > _maxDrawBuffers = 0,
			  Inst< SFInt32 > _maxElementsIndices = 0,
			  Inst< SFInt32 > _maxElementsVertices = 0,
			  Inst< SFInt32 > _maxEvalOrder = 0,
			  Inst< SFInt32 > _maxFragmentUniformComponents = 0,
			  Inst< SFInt32 > _maxLights = 0,
			  Inst< SFInt32 > _maxListNesting = 0,
			  Inst< SFInt32 > _maxModelviewStackDepth = 0,
			  Inst< SFInt32 > _maxNameStackDepth = 0,
			  Inst< SFInt32 > _maxPixelMapTable = 0,
			  Inst< SFInt32 > _maxProjectionStackDepth = 0,
			  Inst< SFInt32 > _maxTextureCoords = 0,
			  Inst< SFInt32 > _maxTextureImageUnits = 0,
			  Inst< SFInt32 > _maxTextureLODBias = 0,
			  Inst< SFInt32 > _maxTextureSize = 0,
			  Inst< SFInt32 > _maxTextureStackDepth = 0,
			  Inst< SFInt32 > _maxTextureUnits = 0,
			  Inst< SFInt32 > _maxVaryingFloats = 0,
			  Inst< SFInt32 > _maxVertexAttribs = 0,
			  Inst< SFInt32 > _maxVertexTextureImageUnits = 0,
			  Inst< SFInt32 > _maxVertexUniformComponents = 0,
			  Inst< SFInt32 > _maxViewportWidth = 0,
			  Inst< SFInt32 > _maxViewportHeight = 0 );
        
    /// Destructor.
    virtual ~GraphicsHardwareInfo() {}

    /// The Info class holds information about the current graphics hardware.
    struct Info {
      string vendor;
      string renderer;
      string version;
      string shading_language_version;
      string extensions;
      H3DInt32 max_3d_texture_size;
      H3DInt32 max_client_attrib_stack_depth;
      H3DInt32 max_attrib_stack_depth;
      H3DInt32 max_clip_planes;
      H3DInt32 max_color_matrix_stack_depth;
      H3DInt32 max_combined_texture_image_units;
      H3DInt32 max_cube_map_texture_size;
      H3DInt32 max_draw_buffers;
      H3DInt32 max_elements_indices;
      H3DInt32 max_elements_vertices;
      H3DInt32 max_eval_order;
      H3DInt32 max_fragment_uniform_components;
      H3DInt32 max_lights;
      H3DInt32 max_list_nesting;
      H3DInt32 max_modelview_stack_depth;
      H3DInt32 max_name_stack_depth;
      H3DInt32 max_pixel_map_table;
      H3DInt32 max_projection_stack_depth;
      H3DInt32 max_texture_coords;
      H3DInt32 max_texture_image_units;
      H3DInt32 max_texture_lod_bias;
      H3DInt32 max_texture_size;
      H3DInt32 max_texture_stack_depth;
      H3DInt32 max_texture_units;
      H3DInt32 max_varying_floats;
      H3DInt32 max_vertex_attribs;
      H3DInt32 max_vertex_texture_image_units;
      H3DInt32 max_vertex_uniform_components;
      H3DInt32 max_viewport_width;
      H3DInt32 max_viewport_height;
      
    };

    /// Inititalize the node. Sets the fields of the node.
    virtual void initialize();

    /// Function to call to initialize the static info struct
    /// with the values from the current graphics hardware. Can only
    /// be called when a valid current OpenGL context exits. 
    static bool initializeInfo();

    /// Returns true if the static info member has been initialized or
    /// not.
    static bool infoIsInitialized() {
      return info_initialized;
    }

    /// Returns info about the current graphics hardware. Only valid 
    /// if infoIsInitialized is true.
    static const Info &getInfo() {
      return info;
    }

    auto_ptr< SFBool > initialized;
    auto_ptr< SFString > vendor;
    auto_ptr< SFString > renderer;
    auto_ptr< SFString > version;
    auto_ptr< SFString > shadingLanguageVersion;
    auto_ptr< SFString > extensions;


    auto_ptr< SFInt32 > max3dTextureSize;
    auto_ptr< SFInt32 > maxClientAttribStackDepth;
    auto_ptr< SFInt32 > maxAttribStackDepth;
    auto_ptr< SFInt32 > maxClipPlanes;
    auto_ptr< SFInt32 > maxColorMatrixStackDepth;
    auto_ptr< SFInt32 > maxCombinedTextureImageUnits;
    auto_ptr< SFInt32 > maxCubeMapTextureSize;
    auto_ptr< SFInt32 > maxDrawBuffers;
    auto_ptr< SFInt32 > maxElementsIndices;
    auto_ptr< SFInt32 > maxElementsVertices;
    auto_ptr< SFInt32 > maxEvalOrder;
    auto_ptr< SFInt32 > maxFragmentUniformComponents;
    auto_ptr< SFInt32 > maxLights;
    auto_ptr< SFInt32 > maxListNesting;
    auto_ptr< SFInt32 > maxModelviewStackDepth;
    auto_ptr< SFInt32 > maxNameStackDepth;
    auto_ptr< SFInt32 > maxPixelMapTable;
    auto_ptr< SFInt32 > maxProjectionStackDepth;
    auto_ptr< SFInt32 > maxTextureCoords;
    auto_ptr< SFInt32 > maxTextureImageUnits;
    auto_ptr< SFInt32 > maxTextureLODBias;
    auto_ptr< SFInt32 > maxTextureSize;
    auto_ptr< SFInt32 > maxTextureStackDepth;
    auto_ptr< SFInt32 > maxTextureUnits;
    auto_ptr< SFInt32 > maxVaryingFloats;
    auto_ptr< SFInt32 > maxVertexAttribs;
    auto_ptr< SFInt32 > maxVertexTextureImageUnits;
    auto_ptr< SFInt32 > maxVertexUniformComponents;
    auto_ptr< SFInt32 > maxViewportWidth;
    auto_ptr< SFInt32 > maxViewportHeight;


    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Flag indicating if the info member has been initialized or not.
    static bool info_initialized;

    /// The graphics card info for the current system.
    static Info info;

  };
}

#endif
