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

    /// If true the information in all the other fields are valid.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> FALSE \n
    /// \dotfile GraphicsHardwareInfo_initialized.dot
    auto_ptr< SFBool > initialized;

    /// The company responsible for this GL implementation.
    /// This name does not change from release to release.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_vendor.dot
    auto_ptr< SFString > vendor;

    /// The name of the renderer. This name is typically specific to a
    /// particular configuration of a hardware platform. It does not change
    /// from release to release.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_renderer.dot
    auto_ptr< SFString > renderer;

    /// A version or release number.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_version.dot
    auto_ptr< SFString > version;

    /// A version or release number for the shading language.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_shadingLanguageVersion.dot
    auto_ptr< SFString > shadingLanguageVersion;

    /// A space-separated list of supported extensions to GL.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_extensions.dot
    auto_ptr< SFString > extensions;

    /// A rough estimate of the largest 3D texture that the GL can handle.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_max3dTextureSize.dot
    auto_ptr< SFInt32 > max3dTextureSize;

    /// A value indicating the maximum supported depth of the client attribute
    /// stack.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxClientAttribStackDepth.dot
    auto_ptr< SFInt32 > maxClientAttribStackDepth;

    /// The maximum supported depth of the attribute stack.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxAttribStackDepth.dot
    auto_ptr< SFInt32 > maxAttribStackDepth;

    /// The maximum number of application-defined clipping planes.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxClipPlanes.dot
    auto_ptr< SFInt32 > maxClipPlanes;

    /// The maximum supported depth of the color matrix stack.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxColorMatrixStackDepth.dot
    auto_ptr< SFInt32 > maxColorMatrixStackDepth;

    /// The maximum supported texture image units that can be used to access
    /// texture maps from the vertex shader and the fragment processor
    /// combined. If both the vertex shader and the fragment processing stage
    /// access the same texture image unit, then that counts as using two
    /// texture image units against this limit.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxCombinedTextureImageUnits.dot
    auto_ptr< SFInt32 > maxCombinedTextureImageUnits;
    
    /// The value gives a rough estimate of the largest cube-map texture that
    /// the GL can handle.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxCubeMapTextureSize.dot
    auto_ptr< SFInt32 > maxCubeMapTextureSize;
    
    /// The maximum number of simultaneous output colors allowed from a fragment
    /// shader using the gl_FragData built-in array.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxDrawBuffers.dot
    auto_ptr< SFInt32 > maxDrawBuffers;
    
    /// The recommended maximum number of vertex array indices.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxElementsIndices.dot
    auto_ptr< SFInt32 > maxElementsIndices;
    
    /// The recommended maximum number of vertex array vertices.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxElementsVertices.dot
    auto_ptr< SFInt32 > maxElementsVertices;
    
    /// The maximum equation order supported by 1D and 2D evaluators. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxEvalOrder.dot
    auto_ptr< SFInt32 > maxEvalOrder;
    
    /// The maximum number of individual floating-point, integer, or boolean
    /// values that can be held in uniform variable storage for a fragment
    /// shader.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxFragmentUniformComponents.dot
    auto_ptr< SFInt32 > maxFragmentUniformComponents;
    
    /// The maximum number of lights.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxLights.dot
    auto_ptr< SFInt32 > maxLights;
    
    /// The maximum recursion depth allowed during display-list traversal.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxListNesting.dot
    auto_ptr< SFInt32 > maxListNesting;
    
    /// The maximum supported depth of the modelview matrix stack.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxModelviewStackDepth.dot
    auto_ptr< SFInt32 > maxModelviewStackDepth;
    
    /// The maximum supported depth of the selection name stack.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxNameStackDepth.dot
    auto_ptr< SFInt32 > maxNameStackDepth;
    
    /// The maximum supported size of a glPixelMap lookup table.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxPixelMapTable.dot
    auto_ptr< SFInt32 > maxPixelMapTable;
    
    /// The maximum supported depth of the projection matrix stack. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxProjectionStackDepth.dot
    auto_ptr< SFInt32 > maxProjectionStackDepth;
    
    /// The maximum number of texture coordinate sets available to vertex and
    /// fragment shaders. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxTextureCoords.dot
    auto_ptr< SFInt32 > maxTextureCoords;
    
    /// The maximum supported texture image units that can be used to access
    /// texture maps from the fragment shader. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxTextureImageUnits.dot
    auto_ptr< SFInt32 > maxTextureImageUnits;
    
    /// The maximum, absolute value of the texture level-of-detail bias.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxTextureLODBias.dot
    auto_ptr< SFInt32 > maxTextureLODBias;
    
    /// The value gives a rough estimate of the largest texture that the GL can
    /// handle.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxTextureSize.dot
    auto_ptr< SFInt32 > maxTextureSize;
    
    /// The maximum supported depth of the texture matrix stack. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxTextureStackDepth.dot
    auto_ptr< SFInt32 > maxTextureStackDepth;
    
    /// A value indicating the number of conventional texture units
    /// supported. Each conventional texture unit includes both a texture
    /// coordinate set and a texture image unit. Conventional texture
    /// units may be used for fixed-function (non-shader) rendering.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxTextureUnits.dot
    auto_ptr< SFInt32 > maxTextureUnits;
    
    /// The maximum number of interpolators available for processing varying
    /// variables used by vertex and fragment shaders. This value represents
    /// the number of individual floating-point values that can be
    /// interpolated; varying variables declared as vectors, matrices,
    /// and arrays will all consume multiple interpolators.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxVaryingFloats.dot
    auto_ptr< SFInt32 > maxVaryingFloats;
    
    /// The maximum number of 4-component generic vertex attributes accessible to a vertex shader.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxVertexAttribs.dot
    auto_ptr< SFInt32 > maxVertexAttribs;
    
    /// The maximum supported texture image units that can be used to access
    /// texture maps from the vertex shader.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxVertexTextureImageUnits.dot
    auto_ptr< SFInt32 > maxVertexTextureImageUnits;
    
    /// The maximum number of individual floating-point, integer, or boolean
    /// values that can be held in uniform variable storage for a vertex
    /// shader.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxVertexUniformComponents.dot
    auto_ptr< SFInt32 > maxVertexUniformComponents;
    
    /// The maximum supported width of the viewport.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxViewportWidth.dot
    auto_ptr< SFInt32 > maxViewportWidth;
    
    /// The maximum supported height of the viewport.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile GraphicsHardwareInfo_maxViewportHeight.dot
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
