//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file FrameBufferTextureGenerator.h
/// \brief Header file for FrameBufferTextureGenerator.
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FRAMEBUFFERTEXTUREGENERATOR_H__
#define __FRAMEBUFFERTEXTUREGENERATOR_H__

#include <H3D/X3DGroupingNode.h>
#include <H3D/MFString.h>
#include <H3D/GeneratedTexture.h>
#include <H3D/X3DViewpointNode.h>
#include <H3D/X3DBackgroundNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class FrameBufferTextureGenerator
  /// \brief The FrameBufferTextureGenerator node provides a mean to render a sub-scene 
  /// into a texture instead of the normal frame buffer. 
  /// 
  /// The children field contains the nodes to render into the textures.
  /// 
  /// Two kinds of textures can be generated: depth textures and color textures.
  /// Depth textures are the depth buffer as a texture while the color textures
  /// are the frame buffer as a texture. Only one depth texture can be generated
  /// per FrameBufferTextureGenerator, but multiple color textures is available.
  /// 
  /// The generateColorTextures field defines the color buffer textures to generate
  /// and their type. 
  /// For each texture to generate the type of the texture needs to be specified. 
  /// The available types are:
  /// "RGBA" - normal color texture with alpha channel.
  /// "RGB" - normal color texture without alpha channel.
  /// "RGBA32F" - floating point number texture with 32 bit float for each component.
  /// "RGB32F"  - floating point number texture with 32 bit float for each component.
  /// "RGBA16F" - floating point number texture with 16 bit float for each component.
  /// "RGB16F"  - floating point number texture with 16 bit float for each component.
  ///
  /// The output textures will be output into the colorTextures field.
  ///
  /// By default output is rendered into all color textures. This can be changed
  /// using a RenderTargetSelectGroup in any sub-scene in the children field.
  /// The textures in colorTextures correspond directly to multiple render targets
  /// in OpenGL so that a glsl fragment shader can write to the colorTextures[0] 
  /// by gl_FragData[0], the second texture by gl_FragData[1] and so on.
  /// 
  /// The generateDepthTexture determines if a depth texture should be generated
  /// or not. If true the depthTexture field will contain a texture containing the 
  /// depth buffer.
  ///
  /// The depthBufferType field determines the precision and type of the depth 
  /// buffer.
  /// 
  /// "DEPTH"  - default depth buffer type for the graphics card.
  /// "DEPTH16" - 16 bit depth buffer.
  /// "DEPTH24" - 24 bit depth buffer.
  /// "DEPTH32" - 32 bit depth buffer.
  /// "DEPTH32F" - 32 bit floating point depth buffer.
  ///
  ///
  /// The outputTextureType field determines the output type of the generated
  /// textures. Valid values: 
  /// 
  /// "2D"       - normal 2D texture(glsl type sampler2D)
  /// "2D_RECTANGLE" - 2D rectangle texture(glsl type sampler2DRect)
  /// "3D"       - normal 3D texture. 3D depth textures not supported. (glsl type sampler3D)
  /// "2D_ARRAY" - 2D texture array. Similar to a 3D texture but without interpolation
  /// between depth layers. Can only be used by shaders and not used directly for texturing.
  /// glsl type sampler2DArray
  ///
  /// The samples field determines how many samples to use for each pixel value in the
  /// textures. A value of 0 means no multi-sampling, values > 0 means multi-sampling
  /// with the specified number of sample points. Using multiple sample points reduces
  /// aliasing artifacts.
  /// 
  /// The update field allows the user to request a regeneration of the 
  /// texture. Setting this field to "ALWAYS" will make the texture be
  /// rendered every frame. A value of "NONE" will stop rendering so that
  /// no further updates are performed even if the contained scene graph 
  /// changes. When the value is set to "NEXT_FRAME_ONLY", it is an 
  /// instruction to render the texture at the end of this frame, and then
  /// not to render again. What this means is that the update frame indicator
  /// is set to this frame, and at the start of the next frame, the update
  /// value will be automatically set back to "NONE" to indicate that the 
  /// rendering has taken place already. Since this is a field change value,
  /// it will automatically generate an output event that may be routed.
  ///
  /// The viewpoint field can be used to define a separatue X3DViewpointNode
  /// to use when rendering the scene. If NULL, the current active viewpoint is used.
  ///
  /// The width and height field is the size of the output textures in pixels. 
  /// If set to -1 the current default frame buffer width or height is used.
  ///
  /// The colorTextureProperties contain the TextureProperties to apply to 
  /// each generated color texture. They are applied to
  /// the textures in the same order as colorTextures in sequence. If there are more
  /// colorTextures than colorTextureProperties the last colorTextureProperty is used
  /// for all remaining textures. If no TextureProperties are assigned the
  /// default texture values are used.
  ///
  /// The depthTextureProperties contains TextureProperties to apply to the 
  /// generated depth texture. If no TextureProperties are assigned the
  /// default texture values are used.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator.x3d">FrameBufferTextureGenerator.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator_shader.x3d">FrameBufferTextureGenerator_shader.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator_shader.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator_texturearray.x3d">FrameBufferTextureGenerator_texturearray.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator_texturearray.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator_fullscreen.x3d">FrameBufferTextureGenerator_fullscreen.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator_fullscreen.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FrameBufferTextureGenerator.dot
  class H3DAPI_API FrameBufferTextureGenerator : public X3DGroupingNode {
  public:

    typedef TypedSFNode< TextureProperties > SFTexturePropertiesNode;
    typedef TypedMFNode< TextureProperties > MFTexturePropertiesNode;
    typedef TypedSFNode< X3DViewpointNode > SFViewpointNode;
    typedef TypedSFNode< X3DBackgroundNode > SFBackgroundNode;
    typedef TypedMFNode< H3DSingleTextureNode > MFGeneratedTextureNode;
    typedef TypedSFNode< H3DSingleTextureNode > SFGeneratedTextureNode;
    typedef void (*RenderCallbackFunc)( FrameBufferTextureGenerator *, int i, void * );
    
    /// Constructor.
    FrameBufferTextureGenerator( Inst< AddChildren    > _addChildren     = 0,
                                 Inst< RemoveChildren > _removeChildren  = 0,
                                 Inst< MFChild        > _children        = 0,
                                 Inst< SFNode         > _metadata        = 0,
                                 Inst< SFBound        > _bound           = 0,
                                 Inst< SFVec3f        > _bboxCenter      = 0,
                                 Inst< SFVec3f        > _bboxSize        = 0, 
                                 Inst< MFString       > _generateColorTextures = 0,
                                 Inst< SFBool         > _generateDepthTexture  = 0,
                                 Inst< MFTexturePropertiesNode > _colorTextureProperties = 0,
                                 Inst< SFTexturePropertiesNode > _depthTextureProperties = 0,
                                 Inst< MFGeneratedTextureNode > _colorTextures = 0, 
                                 Inst< SFGeneratedTextureNode > _depthTexture  = 0,
                                 Inst< SFString         > _depthBufferType = 0,
                                 Inst< SFString         > _outputTextureType = 0,
                                 Inst< SFInt32          > _samples   = 0,
                                 Inst< SFString         > _update    = 0,
                                 Inst< SFViewpointNode  > _viewpoint = 0,
                                 Inst< SFBackgroundNode > _background = 0,
                                 Inst< SFInt32          > _width     = 0,
                                 Inst< SFInt32          > _height    = 0 );
        
    /// Destructor.
    virtual ~FrameBufferTextureGenerator();

    /// Performes the OpenGL calls needed for generating the textures.
    virtual void render();

    /// Set a callback function that will override the normal rendering of the children
    /// field into the textures and replace it with any OpenGL calls of your own.
    /// The callback function is of the form:
    /// void f( FrameBufferTextureGenerator *g, int i, void *args )
    /// where g is the FrameBufferTextureGenerator being rendered, i is -1 if 2D 
    /// textures are rendered and the index of the slice being rendered into if
    /// 3D textures. When rendering 3d textures the depth of the 3D texture is
    /// still the size of the children field so make sure to resize it to the appropriate
    /// size. args is user defined input the function.
    /// 
    /// \param func The callback function to use.
    /// \param args User specific data which is sent to the callback function when called.
    void setRenderCallback( RenderCallbackFunc func, void *args = NULL ); 

    /// Defines the color buffer textures to generate and their type. 
    /// For each texture to generate the type of the texture needs to be specified. 
    /// The available types are:
    /// "RGBA" - normal color texture with alpha channel.
    /// "RGB" - normal color texture without alpha channel.
    /// "RGBA32F" - floating point number texture with 32 bit float for each component.
    /// "RGB32F"  - floating point number texture with 32 bit float for each component.
    /// "RGBA16F" - floating point number texture with 16 bit float for each component.
    /// "RGB16F"  - floating point number texture with 16 bit float for each component.
    ///
    /// The output textures will be output into the colorTextures field.
    ///
    /// By default output is rendered into all color textures. This can be changed
    /// using a RenderTargetSelectGroup in any sub-scene in the children field.
    /// The textures correspond directly to multiple render targets in OpenGL so that
    /// a glsl fragment shader can write to the colorTextures[0] by gl_FragData[0],
    /// the second texture by gl_FragData[1] and so on.
    ///
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> []
    /// <b>Valid values:</b> "RGBA", "RGB", "RGBA32F", "RGB32F", "RGBA16F", "RGB16F"
    auto_ptr< MFString > generateColorTextures;

    /// Determines if a depth texture should be generated or not. If true the depthTexture
    /// field will contain a texture containing the depth buffer.
    /// 
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> false
    auto_ptr< SFBool > generateDepthTexture;

    /// The TextureProperties to apply to each generated color texture. They are applied to
    /// the textures in the same order as colorTextures in sequence. If there are more
    /// colorTextures than colorTextureProperties the last colorTextureProperty is used
    /// for all remaining textures.
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< MFTexturePropertiesNode > colorTextureProperties;

    /// The TextureProperties to apply to the generated depth texture.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> NULL
    auto_ptr< SFTexturePropertiesNode > depthTextureProperties;

    /// The texture nodes generated from color buffers specified in generateColorTextures.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFGeneratedTextureNode > colorTextures;

    /// The texture node generated from depth buffers if specified in generateDepthTexture.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFGeneratedTextureNode > depthTexture;
    
    /// The depthBufferType field determines the precision and type of the depth 
    /// buffer.
    /// 
    /// "DEPTH"  - default depth buffer type for the graphics card.
    /// "DEPTH16" - 16 bit depth buffer.
    /// "DEPTH24" - 24 bit depth buffer.
    /// "DEPTH32" - 32 bit depth buffer.
    /// "DEPTH32F" - 32 bit floating point depth buffer.
    /// "DEPTH_STENCIL" - default combined depth and stencil buffer type.
    /// "DEPTH24_STENCIL8" - 24 bit depth buffer, 8 bit stencil buffer.
    ///
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> "DEPTH"
    /// <b>Valid values:</b> "DEPTH", "DEPTH16", "DEPTH24", "DEPTH32", "DEPTH32F", "DEPTH_STENCIL", "DEPTH24_STENCIL8"
    auto_ptr< SFString > depthBufferType;

    /// The outputTextureType field determines the output type of the generated
    /// textures. Valid values: 
    /// 
    /// "2D"       - normal 2D texture(glsl type sampler2D)
    /// "2D_RECTANGLE" - 2D rectangle texture(glsl type sampler2DRect)
    /// "3D"       - normal 3D texture. 3D depth textures not supported. (glsl type sampler3D)
    /// "2D_ARRAY" - 2D texture array. Similar to a 3D texture but without interpolation
    /// between depth layers. Can only be used by shaders and not used directly for texturing.
    /// glsl type sampler2DArray
    ///
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> "2D"
    /// <b>Valid values:</b> "2D", "2D_RECTANGLE", "3D", "2D_ARRAY"
    auto_ptr< SFString > outputTextureType;
    
    /// The samples field determines how many samples to use for each pixel value in the
    /// textures. A value of 0 means no multi-sampling, values > 0 means multi-sampling
    /// with the specified number of sample points. Using multiple sample points reduces
    /// aliasing artifacts.
    /// 
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> 0
    auto_ptr< SFInt32 > samples;

    /// The update field allows the user to request a regeneration of the 
    /// texture. Setting this field to "ALWAYS" will make the texture be
    /// rendered every frame. A value of "NONE" will stop rendering so that
    /// no further updates are performed even if the contained scene graph 
    /// changes. When the value is set to "NEXT_FRAME_ONLY", it is an 
    /// instruction to render the texture at the end of this frame, and then
    /// not to render again. What this means is that the update frame indicator
    /// is set to this frame, and at the start of the next frame, the update
    /// value will be automatically set back to "NONE" to indicate that the 
    /// rendering has taken place already. Since this is a field change value,
    /// it will automatically generate an output event that may be routed.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "ALWAYS"
    /// <b>Valid values:</b> "NONE", "ALWAYS", "NEXT_FRAME_ONLY"
    auto_ptr< SFString > update;

    /// The X3DViewpointNode to use when rendering the scene. If NULL,
    /// the current active viewpoint is used.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> NULL
    auto_ptr< SFViewpointNode > viewpoint;

    /// The X3DBackgroundNode to use when rendering the scene. If NULL,
    /// then no background is rendered.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> NULL
    auto_ptr< SFBackgroundNode > background;

    /// The width of the output textures in pixels. If set to -1 the
    /// current default frame buffer width is used.
    ///
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> inputOutput
    auto_ptr< SFInt32 > width;

    /// The height of the output textures in pixels. If set to -1 the
    /// current default frame buffer height is used.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> -1
    auto_ptr< SFInt32 > height;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
    /// The ensureInitialized function makes sure that the output textures
    /// of the FrameBufferTextureGenerator are initialized and all required
    /// frame buffer objects. Can only be called when a OpenGL context is 
    /// active.
    inline void ensureInitialized() {
      if( !fbo_initialized ) initializeFBO();
    }

    /// Returns true if the fbo used by the node has been initialized, 
    inline bool isFBOInitialized() {
      return fbo_initialized;
    }

    /// Returns the OpenGL fbo id used by the FrameBufferTextureGenerator.
    /// Only valid if isFBOInitialized is true/
    inline GLuint getFBOId() {
      return fbo_id;
    }

    /// If set to true the currently set up viewport when the render() function 
    /// is called will be used for the rendering. If false, the viewport will be
    /// set to fill the entire frame buffer.
    inline void setAlwaysUseExistingViewport( bool use ) {
      always_use_existing_viewport = use;
    }

    /// Returns the current value of alwaysUseExistingViewpoint. See 
    /// setAlwaysUseExistingViewport for details.
    inline bool alwaysUseExistingViewpoint() {
      return always_use_existing_viewport;
    }

    /// The FrameBufferTextureGenerator that are currently in use in the
    /// application. 
    static std::set< FrameBufferTextureGenerator * > fbo_nodes;

  protected:

    /// Converts a string to a OpenGL internal texture format.
    GLenum stringToInternalFormat( const string &format_string );

    /// Converts a string to a OpenGL internal depth texture format.
    GLenum stringToInternalDepthFormat( const string &s );

    /// Converts a string to a OpenGL depth texture format.
    GLenum stringToDepthFormat( const string &s );

    /// Converts a string to a OpenGL depth type format.
    GLenum stringToDepthType( const string &s );

    /// Checks the currently bound fbo for completeness and prints a error message
    /// if something is wrong. True is returned if fbo complete. 
    bool checkFBOCompleteness();

    /// Initialize all output textures and buffers needed for the node.
    void initializeFBO();

    /// Help function that is called when the main frame buffer size has changed and
    /// it will resize all output textures to match the size of the frame buffer.
    /// \param w The new width of the textures(in pixels).
    /// \param h The new height of the textures(in pixels).
    /// \param d The new depth of the textures(in pixels). Only relevant
    /// when output texture type is "2D_ARRAY" or "3D".
    /// \return true on success, false on error.
    bool resizeBuffers( H3DInt32 w, H3DInt32 h, H3DInt32 d );

    /// Flag used to determine if initializeFBO has been called or not.
    bool fbo_initialized;

    /// The OpenGL texture ids for all textures generated in the colorTextures field.
    vector<GLuint> color_ids;

    /// The OpenGL multi samples render buffer for depth buffer when using 
    /// multi sample rendering.
    GLuint multi_samples_depth_id;
    
    /// The OpenGL multi samples render buffer for color buffers when 
    /// using multi sample rendering.
    vector<GLuint> multi_samples_color_ids;
    
    /// The OpenGL fbo used for multi sample rendering.
    GLuint multi_samples_fbo_id;

    /// The current width(in pixels) of output textures and buffers.
    H3DInt32 buffers_width;

    /// The current width(in pixels) of output textures and buffers.
    H3DInt32 buffers_height;

    /// The current depth(in pixels) of output textures and buffers.
    /// Only relevant when rendering into 3d texture or 2d texture
    /// array.
    H3DInt32 buffers_depth;

    /// The id of the frame buffer object used to generate textures.
    GLuint fbo_id;

    /// The id of the depth buffer used with the fbo. This is a texture id if 
    /// generateDepthTexture is true, and a render buffer id otherwise.
    GLuint depth_id;

    /// The id of the render buffer used as stencil buffer with the fbo.
    GLuint stencil_id;

    /// Array containing [GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT .. ]
    /// with one entry for each texture in colorTextures. Used with glDrawBuffers.
    auto_ptr< GLenum > draw_buffers;

    /// True if the last call to resizeBuffers from render() was true.
    bool last_resize_success;

    /// The number of multisamples currently used for rendering.
    int nr_samples;

    /// The render callback function, if any.
    RenderCallbackFunc render_func;

    /// The specified callback function user data, if any.
    void *render_func_data;

    /// If set to true the currently set up viewport when the render() function 
    /// is called will be used for the rendering. If false, the viewport will be
    /// set to fill the entire frame buffer.
    bool always_use_existing_viewport;
  };
}

#endif
