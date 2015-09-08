//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file RenderTargetTexture.h
/// \brief Header file for RenderTargetTexture.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RENDERTARGETTEXTURE_H__
#define __RENDERTARGETTEXTURE_H__

#include <H3D/X3DTextureNode.h>
#include <H3D/FrameBufferTextureGenerator.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class RenderTargetTexture
  /// The RenderTargetTexture is a wrapper node to use one of the render targets(color textures) 
  /// from a FrameBufferTextureGenerator directly as a texture.
  ///
  /// The generator field contains the FrameBufferTextureGenerator node from which to use a 
  /// render target (color texture ).
  ///
  /// The index field indicates which render target (color texture) to use from the 
  /// FrameBufferTextureGenerator.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RenderTargetTexture.x3d">RenderTargetTexture.x3d</a>
  ///     ( <a href="examples/RenderTargetTexture.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile RenderTargetTexture.dot 
  class H3DAPI_API RenderTargetTexture : public H3DSingleTextureNode {
  public:

    typedef TypedSFNode< FrameBufferTextureGenerator > SFFrameBufferTextureGenerator;

    /// Constructor.
    RenderTargetTexture( Inst< DisplayList          > _displayList = 0,
                         Inst< SFNode               > _metadata = 0,
                         Inst< SFFrameBufferTextureGenerator > _generator = 0,
                         Inst< SFInt32              > _index = 0);
        
    /// Destructor.
    virtual ~RenderTargetTexture() {}
    
    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void render();

    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void postRender();

   /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void preRender();

   /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void enableTexturing();

   /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void disableTexturing();

    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual GLuint getTextureId();

    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual GLuint getTextureUnit();

    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual GLenum getTextureTarget();

    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void setTextureId( GLuint id );
    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void setTextureUnit( GLint unit );
    /// Overridden virtual function to perform the same function on the correct 
    /// texture from the generator.
    virtual void setTextureTarget( GLenum target );

    /// Make the texture resident as a bindless texture
    virtual bool makeResident ();

    /// Make the texture non-resident
    virtual void makeNonResident ();

    /// Returns true if the texture is resident in GPU memory or false otherwise
    virtual bool isResident ();

    /// Get the bindless texture handle, or 0 if not resident
    virtual GLuint64 getTextureHandle();

    /// This should be called if the texture object is to be replaced
    ///
    /// This will cause the texture to become non-resident and for the texture
    /// handle to be regenerated the next time the texture is made resident
    virtual void invalidateTextureHandle ();

    /// Call to signify that this texture is currently in use
    ///
    /// Textures that are not in use are periodically detected and
    /// made non-resident
    virtual void inUse ();

    /// The generator field contains the FrameBufferTextureGenerator node from which to use a 
    /// render target (color texture ).
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< SFFrameBufferTextureGenerator > generator;

    /// The index field indicates which render target (color texture) to use from the 
    /// FrameBufferTextureGenerator.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0
    auto_ptr< SFInt32 > index;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Override getDefaultSaveDimensions function to get current texture dimension
    /// for this RenderTargetTexture
    virtual std::pair<H3DInt32,H3DInt32> getDefaultSaveDimensions ();

  };
}

#endif
