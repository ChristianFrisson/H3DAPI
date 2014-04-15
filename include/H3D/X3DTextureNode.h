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
/// \file X3DTextureNode.h
/// \brief Header file for X3DTextureNode, X3D abstract texture scene-graph 
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURENODE_H__
#define __X3DTEXTURENODE_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>
#include <H3DUtil/Image.h>
#include <H3D/FieldTemplates.h>
#include "GL/glew.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DTextureNode
  /// This abstract node type is the base type for 
  /// all node types which specify sources for texture images. 
  /// 
  class H3DAPI_API X3DTextureNode : public X3DAppearanceChildNode {
  public:
        
    /// Thrown by renderImage () if the dimensions of an image are
    /// invalid, e.g. not a power of 2.
    H3D_API_EXCEPTION( InvalidTextureDimensions );

    /// Thrown when an OpenGL error occurs while setting up
    /// the texture.
    H3D_API_EXCEPTION( OpenGLTextureError );

    /// Thrown when a subclass to X3DTextureNode calls renderImage
    /// without defining the glTexImage () function.
    H3D_API_EXCEPTION( glTexImageFunctionNotDefined );

    /// A field used to execute the save to URL operation when the URL is set
    class H3DAPI_API UpdateSaveToURL : public OnNewValueSField < AutoUpdate < SFString > > {
      virtual void onNewValue( const std::string &v );
    };

    /// Constructor.
    X3DTextureNode( Inst< DisplayList > _displayList = 0,
                    Inst< SFNode>  _metadata = 0,
                    Inst< UpdateSaveToURL > _saveToUrl = 0,
                    Inst< SFBool > _saveSuccess = 0,
                    Inst< SFInt32 > _saveHeight = 0,
                    Inst< SFInt32 > _saveWidth = 0
                    );
        
    /// Virtual function for making all OpenGL calls that are needed to
    /// enable texturing for the texture.
    ///
    virtual void enableTexturing() = 0;

    /// Virtual function for making all OpenGL calls that are needed to
    /// disable texturing for the texture.
    ///
    virtual void disableTexturing() = 0;
        
    /// Returns the default xml containerField attribute value.
    /// For this node it is "texture".
    ///
    virtual string defaultXMLContainerField() {
      return "texture";
    }

    /// This function will be called by the X3DShapeNode before any rendering 
    /// of geometry and before the call to the render function.
    virtual void preRender() {
      setActiveTexture( this );
    }

    /// This function will be called by the X3DShapeNode after the geometry
    /// has been rendered to restore the states to what it was before 
    /// the call to preRender().
    virtual void postRender() {
      setActiveTexture( NULL );
    }

    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() { 
      return GL_TEXTURE_BIT | GL_LIGHTING_BIT;
    }

    /// Sometimes the texture represents a volume in space, e.g. when
    /// it contains volume data to be rendered. This function returns
    /// the size of the space the volume occupies in metres. 
    virtual Vec3f textureSize() { return Vec3f( 0, 0, 0 ); } 

    /// Gets the currently active texture, i.e. the texture that is currently 
    /// bound in OpenGL. NULL us returned if no X3DTextureNode is bound. 
    static X3DTextureNode *getActiveTexture() { return active_texture; }

    /// Set the active texture. The active texture is the texture that is
    /// currently in bound in OpenGL, so as soon a call to glBindTexture is
    /// made this function should be called also a pointer to the texture
    /// being bound.
    ///
    static void setActiveTexture( X3DTextureNode *t ) { active_texture = t; }

    /// Installs the image as a OpenGL texture. Uses the glTexImage function
    /// for the glTexImage call, so it has to be defined for subclasses
    /// that use this function. 
    /// \param image The image to use as texture.
    /// \param texture_target The OpenGL texture target to use for the GL texture. 
    /// E.g. GL_TEXTURE_2D.
    /// \param scale_to_power_of_two Scale the image data to the closest higher power
    /// of two if dimensions are not a power of two.
    /// \returns The texture id of the OpenGL texture that got installed.
    ///
    virtual GLuint renderImage( Image *image, 
                                GLenum texture_target,
                                bool scale_to_power_of_two );

    // Below are help functions for getting OpenGL constants from an Image.

    /// Returns the internal OpenGL format to use given an Image, e.g.
    /// GL_LUMINANCE, GL_RGB, GL_RGB8, etc
    ///
    virtual GLint glInternalFormat( Image *image );

    /// Returns the OpenGL pixel format to use given an Image, e.g. 
    /// GL_RGB, GL_LUMINANCE, etc
    ///
    virtual GLenum glPixelFormat( Image *image );

    /// Returns the OpenGL pixel compnent type to use given an Image, e.g. 
    /// GL_BYTE, GL_UNSIGNED_INT, etc
    ///
    virtual GLenum glPixelComponentType( Image *image );

    /// Install the given image as a OpenGL texture with a call to 
    /// a glTexImage function. Subclasses that use the renderImage () 
    /// function must define this function. By default an 
    /// glTexImageFunctionNotDefined exception is thrown.
    /// if scale_to_power_of_two is true then the image data will be 
    /// scaled to the closest higher power of two if dimensions are not a power
    /// of two.
    ///
    virtual void glTexImage( Image *image,
                             GLenum texture_target,
                             bool scale_to_power_of_two ){
      throw glTexImageFunctionNotDefined( "", H3D_FULL_LOCATION ); 
    }

    /// Create a new Image object which contains the result of 
    /// rendering this texture.
    ///
    /// The image is obtained by rendering the texture to a FBO
    /// and reading it into the image using glReadPixels.
    ///
    /// \param _width The width of the Image to create
    /// \param _height The height of the Image to create
    /// \return A new Image instance, or NULL on failure.
    ///
    Image* renderToImage ( H3DInt32 _width, H3DInt32 _height );

    /// This is the default value for the GlobalSettings::loadTexturesInThread
    /// field.
    /// If set to true the default behaviour is that all images that are read
    /// from an url will be downloaded in a separate
    /// thread allowing the program to continue execution while waiting
    /// for the textures to download. The textures will then be applied
    /// as soon as they are downloaded.
    /// 
    /// By default it is set to true.
    static bool load_images_in_separate_thread;

    /// When a new value is specified for saveToUrl the current texture is rendered to
    /// a buffer and saved to the specified filename. The URL must be a local filename.
    ///
    /// The texture is written immediately and the success of failure of the save 
    /// operation can be checked using the saveSuccess field.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    ///
    auto_ptr < UpdateSaveToURL > saveToUrl;

    /// Contains the result of the last save operation. True if the last save operation
    /// was successful.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> false \n
    ///
    auto_ptr < SFBool > saveSuccess;

    /// The height of the image to write to file when the saveToUrl field is used
    ///
    /// If -1 then a default is chosen. If this type of texture node has dimensions
    /// then these will be used as the default.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1 \n
    ///
    auto_ptr < SFInt32 > saveHeight;

    /// The width of the image to write to file when the saveToUrl field is used
    ///
    /// If -1 then a default is chosen. If this type of texture node has dimensions
    /// then these will be used as the default.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1 \n
    ///
    auto_ptr < SFInt32 > saveWidth;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Returns the default dimensions to use when this texture is saved to file.
    ///
    /// Texture nodes that have dimensions should use those dimensions as the default.
    /// The default implementation returns (512, 512).
    ///
    virtual std::pair<H3DInt32,H3DInt32> getDefaultSaveDimensions ();
  
  private:
    static X3DTextureNode *active_texture;
  };
}

#endif
