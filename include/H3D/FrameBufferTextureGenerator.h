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
#include <H3D/NavigationInfo.h>
#include <H3D/X3DBackgroundNode.h>
#include <H3D/ShadowCaster.h>
#include <H3D/TypedField.h>
#include <H3D/FieldTemplates.h>

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
  /// The viewpoint field can be used to define a separate X3DViewpointNode
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
  /// The depthBufferStorage is a option to set what is going to be used
  /// to initial the depth buffer. By default it is set to be LOCAL
  /// The available types are:
  /// 
  /// "LOCAL"         - clear the local depth buffer
  /// "DEFAULT_COPY"  - copy depth buffer from back buffer
  /// "FBO_COPY"      - copy depth buffer from external specified FBO
  /// "FBO_SHARE"     - share depth buffer with external specified FBO
  /// 
  /// The externalFBODepthBuffer is a filed used to specify the FBO to be used 
  /// to provide depth buffer. It will only be used if the depthBufferStorage
  /// is set to be FBO_COPY or FBO_SHARE
  /// 
  /// The colorBufferStorages contains the information about how to initialize 
  /// every single color buffer. By default it is NULL and every needed color
  /// buffer will use zeroed color buffer to start
  /// The available types are:
  /// "LOCAL"   - clear the corresponding color buffer
  /// "DEFAULT_COPY"  - copy the corresponding color buffer from default color buffer
  /// "FBO_COPY_x"    - copy the corresponding color buffer from x-th color buffer 
  /// of external specified FBO
  /// "FBO_SHARE_x"   - share the corresponding color buffer with the x-th color buffer
  /// of external specified FBO
  /// 
  /// The externalFBOColorBuffers contains all the external FBOs which will be
  /// used and only be used while one or more value in colorBufferStorages is
  /// FBO_COPY_x or FBO_SHARE_x
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
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator_external_fbo.x3d">FrameBufferTextureGenerator_external_fbo.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator_external_fbo.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FrameBufferTextureGenerator.dot
  class H3DAPI_API FrameBufferTextureGenerator : public X3DGroupingNode {
  public:

    typedef TypedSFNode< TextureProperties > SFTexturePropertiesNode;
    typedef TypedMFNode< TextureProperties > MFTexturePropertiesNode;
    typedef TypedSFNode< X3DViewpointNode > SFViewpointNode;
    typedef TypedSFNode< NavigationInfo > SFNavigationInfo;
    typedef TypedSFNode< X3DBackgroundNode > SFBackgroundNode;
    typedef TypedMFNode< H3DSingleTextureNode > MFGeneratedTextureNode;
    typedef TypedSFNode< H3DSingleTextureNode > SFGeneratedTextureNode;
    typedef void (*RenderCallbackFunc)( FrameBufferTextureGenerator *, int i, void * );
    typedef TypedSFNode< FrameBufferTextureGenerator > SFFrameBufferTextureGeneratorNode;
    typedef TypedMFNode< FrameBufferTextureGenerator > MFFrameBufferTextureGeneratorNode;

    class H3DAPI_API UpdateMode : public AutoUpdate < OnNewValueSField < SFString > > {
        virtual void onNewValue( const std::string& new_value );
    };

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
                                 Inst< SFGeneratedTextureNode > _colorTexture  = 0,
                                 Inst< SFGeneratedTextureNode > _depthTexture  = 0,
                                 Inst< SFString         > _depthBufferType = 0,
                                 Inst< SFString         > _outputTextureType = 0,
                                 Inst< SFInt32          > _samples   = 0,
                                 Inst< UpdateMode       > _update    = 0,
                                 Inst< SFInt32          > _framesBeforeStop = 0,
                                 Inst< SFViewpointNode  > _viewpoint = 0,
                                 Inst< SFNavigationInfo > _navigationInfo = 0,
                                 Inst< SFBackgroundNode > _background = 0,
                                 Inst< SFInt32          > _width     = 0,
                                 Inst< SFInt32          > _height    = 0,
                                 Inst< SFInt32          > _widthInUse = 0,
                                 Inst< SFInt32          > _heightInUse = 0,
                                 Inst< SFBool           > _useStereo = 0,
                                 Inst< SFString         > _depthBufferStorage = 0,
                                 Inst< SFFrameBufferTextureGeneratorNode > _externalFBODepthBuffer = 0,
                                 Inst< MFString         > _colorBufferStorages = 0,
                                 Inst< MFFrameBufferTextureGeneratorNode > _externalFBOColorBuffers = 0,
                                 Inst< SFBool           > _useNavigation = 0 );
        
    /// Destructor.
    virtual ~FrameBufferTextureGenerator();

    /// Performes the OpenGL calls needed for generating the textures.
    virtual void render();

    /// Traverse the scenegraph.
    virtual void traverseSG( TraverseInfo &ti );

    /// Initialize the FrameBufferTextureGenerator
    virtual void initialize();

    /// Returns true if the FBO used has a stencil buffer.
    bool haveStencilBuffer();

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

    /// Defines what should be used as the depth buffer for current FBO before rendering
    /// the sub-scene children
    /// The available types are:
    /// "LOCAL" - use internal FBO and clear it before every render loop.
    /// "FBO_COPY" - blit the depth buffer of external FBO to internal FBO.
    /// "FBO_SHARE" - bind the depth buffer of external FBo to internal FBO.
    /// "DEFAULT_COPY" - blit the depth buffer of default frame buffer to internal FBO.
    ///
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> "LOCAL"
    /// <b>Valid values:</b> "LOCAL", "FBO_COPY", "FBO_SHARE", "DEFAULT_COPY"
    auto_ptr< SFString > depthBufferStorage;

    /// Specifies the external FrameBufferTextureGenerator to be used for FBO_COPY 
    /// and FBO_SHARE option of DepthBufferStorage
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< SFFrameBufferTextureGeneratorNode > externalFBODepthBuffer;

    /// Defines what should be used as the color buffers for current FBO before rendering
    /// the sub-scene children
    /// The available types are:
    /// "LOCAL" - use internal FBO and initialize all needed color buffers specified
    /// "DEFAULT_COPY" - blit the depth buffer of default frame buffer to internal FBO.
    /// "FBO_COPY_x" - blit the color buffer of index x of external FBO to internal FBO.
    /// "FBO_SHARE_x" - bind the color buffer of index x of external FBO to inernal FBO.
    /// 
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> "LOCAL"
    /// <b>Valid values:</b> "LOCAL", "DEFAULT_COPY", "FBO_COPY_x", "FBO_SHARE_x", x depends
    /// on GPU supported max color attachment points.
    auto_ptr< MFString > colorBufferStorages;

    /// Specifies the external FrameBufferTextureGenerator to be used for FBO_COPY_x 
    /// and FBO_SHARE_x option of ColorBufferStorages
    /// <b>Access type:</b> inputOutput
    auto_ptr< MFFrameBufferTextureGeneratorNode > externalFBOColorBuffers;

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

    /// The texture node generated from first color buffer specified in generateColorTextures.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFGeneratedTextureNode > colorTexture;

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
    /// <b>Access type:</b> intputOutput
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
    /// "SPECIFIED_FRAMES_ONLY" will do similar thing as "NEXT_FRAME_ONLY", and
    /// let the user define how many frame it should generate texture until it change
    /// to NONE. The purpose of SPECIFIED_FRAMES_ONLY option is to provide a way
    /// to give a longer delay before the generator stop. Setting the value to "NOW"
    /// will result in immediate rendering of the texture generator without waiting
    /// for the next traversal of the node. The value is then set to "NONE" upon the 
    /// next traversal of render.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "ALWAYS"
    /// <b>Valid values:</b> "NONE", "ALWAYS", "NEXT_FRAME_ONLY", "SPECIFIED_FRAMES_ONLY", "NOW"
    auto_ptr< UpdateMode > update;

    /// The frameBeforeStop is check when SPECIFIED_FRAME_ONLY is set and to provide
    /// the info about how many frame before the update field change to NONE to stop
    /// the generator. By default it is -1
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> -1
    auto_ptr< SFInt32 > framesBeforeStop;

    /// The X3DViewpointNode to use when rendering the scene. If NULL,
    /// the current active viewpoint is used.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> NULL
    auto_ptr< SFViewpointNode > viewpoint;

    /// The NavigationInfo to be used when rendering the scene. Currently, it is
    /// just used for providing far and near cliping distance. If NULL, the
    /// current active viewpoint is used, if no current navigationinfo exists,
    /// near clip distance will be 0.01, far clip distance is set to be -1 which
    /// means infinite
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> NULL
    auto_ptr<SFNavigationInfo> navigationInfo;

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

    /// It contains the actual width value used at realtime
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFInt32 > widthInUse;

    /// It contains the actual height value for this node used at realtime
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFInt32 > heightInUse;

    /// An option to enable the use of same stereo setting as current window
    /// by default,it will be set to false and MONO will be used for rendering
    /// sub-scene.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> false
    auto_ptr< SFBool > useStereo;

    /// An option to enable the use of user navigation which is used by the current
    /// window. If this is enabled, the local viewpoint set by the user will be ignored
    /// as to use a separate viewpoint for navigation will cause the navigation trigger
    /// transition. When no local viewpoint is specified, useNavigation will always have
    /// effect as global active viewpoint will be used then. So basically only specify
    /// useNavigation when you want to use local viewpoint 
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> false
    auto_ptr< SFBool > useNavigation;

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

    /// Returns the OpenGL depth id attached to fbo_id
    /// only valid if isFBOInitialized is true/
    inline GLuint getDepthId() {
      return depth_id;
    }

    /// Return the depth texture of current FrameBufferTextureGenerator
    inline H3DSingleTextureNode* getDepthTexture() {
      return depthTexture->getValue();
    }

    /// Returns the OpenGL color ids attached to fbo_id
    /// only valid if isFBOInitialized is true/
    inline vector<GLuint> getColorIds() {
      return color_ids;
    }

    /// Returns the colorTextures of current FrameBufferTextureGenerator
    /// only valid if isFBOInitialized is true/
    inline NodeVector getColorTextures() {
      return colorTextures->getValue();
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

    /// preProcess the FBO before rendering according to the depthBufferStorage
    /// and colorBufferStorages
    /// \param srcX The x component of low left corner of the area to be copied
    /// \param srcY The y component of low left corner of the area to be copied
    /// \param w    The width of area to be copied
    /// \param h    The height of area to be copied
    /// \param depth The depth of area to be copied
    void preProcessFBO(int srcX, int srcY, int w, int h, int depth);

    /// Blit the depth buffer from src fbo to dst fbo.
    /// \param src  The source fbo used for copy
    /// \param dst  The target fbo used for copy
    /// \param srcX The x component of low left corner of the area to be copied
    /// \param srcY The y component of low left corner of the area to be copied
    /// \param w    The width of area to be copied
    /// \param h    The height of area to be copied
    void blitDepthBuffer(GLenum src, GLenum dst, int srcX, int srcY, int w, int h);

    /// Blit the color buffer from src fbo to dst fbo.
    /// \param src  The source fbo used for copy
    /// \param dst  The target fbo used for copy
    /// \param srcX The x component of low left corner of the area to be copied
    /// \param srcY The y component of low left corner of the area to be copied
    /// \param w    The width of area to be copied
    /// \param h    The height of area to be copied
    /// \param src_index  The color buffer index to be copied
    /// \param dst_index  The color buffer index to be used as target
    void blitColorBuffer(GLenum src, GLenum dst, 
      int srcX, int srcY, int w, int h, int src_index, int dst_index);


    /// Help function that is called when the main frame buffer size has changed.
    /// it will resize all output textures to match the size of the frame buffer.
    /// \param w The new width of the textures(in pixels).
    /// \param h The new height of the textures(in pixels).
    /// \param d The new depth of the textures(in pixels). Only relevant
    /// when output texture type is "2D_ARRAY" or "3D".
    /// \return true on success, false on error.
    bool resizeBuffers( H3DInt32 w, H3DInt32 h, H3DInt32 d );

    /// Function which is used to parse the string in colorBufferStorages
    /// to retrieve what is the color buffer handling style, and index
    /// \param color_buffer_storage The string which specify what should be 
    /// used as the color buffer storage
    /// \param style Options extracted from color_buffer_storage about how the base will be used to 
    /// initialize the color buffer storage. 
    /// \param index The extracted index number of the color buffer attachment in the fbo
    bool parseColorBufferStorage( std::string color_buffer_storage, std::string& style, int& index );

    /// Function which only clear the specified color buffer
    /// \param src  The source FBO of which the clear will do
    /// \param x  The x component of low left corner of the area to be cleared
    /// \param y  The y component of low left corner of the area to be cleared
    /// \param width  The width of the area to be cleared
    /// \param height The height of the area to be cleared
    /// \param value  A four component value to be used as clear color.
    /// \param index  The color buffer index inside current active fbo
    void clearColorBuffer(GLenum src, int x, int y, int width, int height, 
                           GLfloat* value, GLint index );

    /// Clear buffers of src fbo
    /// \param src  The src fbo to be cleared
    /// \param x  The x component of low left corner of the area to be cleared
    /// \param y  The y component of low left corner of the area to be cleared
    /// \param width  The width of the area to be cleared
    /// \param height The height of the area to be cleared
    /// \param mask   The mask indicate what buffers in current fbo will be cleared
    void clearBuffers(GLenum src, int x, int y, int width, int height, GLbitfield mask);


    /// Flag used to determine if initializeFBO has been called or not.
    bool fbo_initialized;

    /// previous sample value
    int last_samples;

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

    /// The number of multi samples currently used for rendering.
    int nr_samples;

    /// The render callback function, if any.
    RenderCallbackFunc render_func;

    /// The specified callback function user data, if any.
    void *render_func_data;

    /// If set to true the currently set up viewport when the render() function 
    /// is called will be used for the rendering. If false, the viewport will be
    /// set to fill the entire frame buffer.
    bool always_use_existing_viewport;

    /// Reference to shadow caster used to cast shadows for shapes
    /// in scene graph in this FrameBufferTextureGenerator.
    AutoRef< ShadowCaster > shadow_caster;

    /// A group node container to gather all child node in MFChild field, used to
    /// gathering bound information.
    AutoRef< X3DGroupingNode > child_to_render;

    private:
      /// A field used to reset the waning printed flag
      class resetPrintedFlag : public TypedField< SFBool, Types<Any<SFString,MFString> > >
      {
      public:
        virtual void update (){
          this->value = false;
        }
      };
      /// A field to reset multiple warning printed flags
      class resetPrintedFlags: public TypedField< MFBool, 
        MFString > {
      public:
        virtual void update(){
          for( vector<bool>::iterator it = value.begin(); it!= value.end(); ++it) {
            
            (*it) = false;
          }
        }
      };
      /// flag control the depth warning output
      auto_ptr<resetPrintedFlag> depthWarningPrinted ;

      /// flag control the warning printing of size mismatch
      auto_ptr<resetPrintedFlag> colorMismatchWarningPrinted;

      /// flag control the warning printing of storage init
      auto_ptr<resetPrintedFlags> colorInitWarningPrinted;
      
  };
}

#endif
