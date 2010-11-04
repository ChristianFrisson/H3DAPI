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

namespace H3D {

  /// \ingroup H3DNodes
  /// \class FrameBufferTextureGenerator
  ///
  /// The FrameBufferTextureGenerator provides a mean to render a sub-scene 
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
  /// "RGBA" - normal color texture.
  /// "RGBA32F" - floating point number texture with 32 bit float for each component.
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
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator.x3d">FrameBufferTextureGenerator.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator_shader.x3d">FrameBufferTextureGenerator_shader.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator_shader.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/FrameBufferTextureGenerator.x3d">FrameBufferTextureGenerator_texturearray.x3d</a>
  ///     ( <a href="examples/FrameBufferTextureGenerator_texturearray.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FrameBufferTextureGenerator.dot
  class H3DAPI_API FrameBufferTextureGenerator : public X3DGroupingNode {
  public:
        
    typedef TypedMFNode< X3DTextureNode > MFGeneratedTextureNode;
    typedef TypedSFNode< X3DTextureNode > SFGeneratedTextureNode;
    
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
				 Inst< MFGeneratedTextureNode > _colorTextures = 0, 
				 Inst< SFGeneratedTextureNode > _depthTexture  = 0,
				 Inst< SFString         > _outputTextureType = 0 );
        
    /// Destructor.
    virtual ~FrameBufferTextureGenerator();

    /// Performes the OpenGL calls needed for generating the textures.
    virtual void render();

    /// Defines the color buffer textures to generate and their type. 
    /// For each texture to generate the type of the texture needs to be specified. 
    /// The available types are:
    /// "RGBA" - normal color texture.
    /// "RGBA32F" - floating point number texture with 32 bit float for each component.
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
    /// <b>Valid values:</b> "RGBA", "RGBA32F"
    auto_ptr< MFString > generateColorTextures;

    /// Determines if a depth texture should be generated or not. If true the depthTexture
    /// field will contain a texture containing the depth buffer.
    /// 
    /// <b>Access type:</b> initializeOnly
    /// <b>Default value:</b> false
    auto_ptr< SFBool > generateDepthTexture;

    /// The texture nodes generated from color buffers specified in generateColorTextures.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFGeneratedTextureNode > colorTextures;

    /// The texture node generated from depth buffers if specified in generateDepthTexture.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< SFGeneratedTextureNode > depthTexture;

    //"2D", "3D"

    auto_ptr< SFString > outputTextureType;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    
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
  };
}

#endif
