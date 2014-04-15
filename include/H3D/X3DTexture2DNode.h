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
/// \file X3DTexture2DNode.h
/// \brief Header file for X3DTexture2DNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURE2DNODE_H__
#define __X3DTEXTURE2DNODE_H__

#include <H3D/H3DSingleTextureNode.h>
#include <H3D/H3DImageObject.h>
#include <H3D/TextureProperties.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {
  /// \ingroup AbstractNodes
  /// \brief This abstract node type is the base type for all node types which
  /// specify 2D sources for texture images. 
  ///
  /// \par Internal routes:
  /// \dotfile X3DTexture2DNode.dot  
  class H3DAPI_API X3DTexture2DNode : 
    public H3DSingleTextureNode,
    public H3DImageObject {
  public:
    /// A SFNode encapsulating an Image class
    class H3DAPI_API SFImage: public H3DImageObject::SFImage {
    public:
      virtual void setValueFromString( const string &s );

      virtual string getValueAsString(const string& separator = " ");

      virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFIMAGE; }
     };
        
    /// The SFTextureProperties is dependent on the propertyChanged field of
    /// the contained TextureProperties.
    typedef  DependentSFNode< FieldRef<TextureProperties,
                                       Field,
                                       &TextureProperties::propertyChanged > > 
    SFTextureProperties;
    
    /// Constructor.
    X3DTexture2DNode( Inst< DisplayList > _displayList = 0,
                      Inst< SFNode  > _metadata  = 0,
                      Inst< SFBool  > _repeatS   = 0,
                      Inst< SFBool  > _repeatT   = 0,
                      Inst< SFBool  > _scaleToP2 = 0,
                      Inst< SFImage > _image     = 0,
                      Inst< SFTextureProperties > _textureProperties = 0 );

    /// Destructor.
    ~X3DTexture2DNode() {
      if( texture_id ) glDeleteTextures( 1, &texture_id );
    }

    /// Performs the OpenGL rendering required to install the image
    /// as a texture.
    virtual void render();

    /// Render all OpenGL texture properties.
    virtual void renderTextureProperties();

    /// Get the OpenGL texture id that is used for this texture.
     virtual GLuint getTextureId() {
      return texture_id;
    }

    /// Get the OpenGL texture unit that is used for this texture.
    virtual GLuint getTextureUnit() {
      return texture_unit;
    }

    /// Get the OpenGL texture target that is used for this texture.
    virtual GLenum getTextureTarget();

    /// Virtual function for making all OpenGL calls that are needed to
    /// enable texturing for the texture.
    ///
    virtual void enableTexturing();
      
    /// Virtual function for making all OpenGL calls that are needed to
    /// disable texturing for the texture.
    ///
    virtual void disableTexturing();

    /// Returns the internal OpenGL format to use given an Image
    virtual GLint glInternalFormat( Image *image );

    /// Installs the given image as a OpenGL texture with a call to 
    /// the glTexImage2D function. This function is used by renderImage ()
    /// and uses the glInternalFormat (), glPixelFormat () and
    /// glPixelComponentType () functions to get the arguments to the
    /// glTexImage2D call.
    ///
    virtual void glTexImage( Image *image, GLenum texture_target, 
                             bool scale_to_power_of_two );
      
    /// Replaces part of the current texture from an image. 
    virtual void renderSubImage( Image *image, GLenum texture_target, 
                                 int xoffset, int yoffset,
                                 int width, int height );

    /// If true the texture will repeat itself when the s texture coordinate
    /// is outside the range [0,1]. If false the texture will be clamped if
    /// outside the same range.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile X3DTexture2DNode_repeatS.dot 
    auto_ptr< SFBool >  repeatS;

    /// If true the texture will repeat itself when the t texture coordinate
    /// is outside the range [0,1]. If false the texture will be clamped if
    /// outside the same range.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile X3DTexture2DNode_repeatT.dot 
    auto_ptr< SFBool >  repeatT;

    /// If true the image used will be scaled so that the dimensions are a 
    /// power of two if they are not. This will however take up more memory
    /// and might cause some unwanted strething effects on the texture. The
    /// new texture values will be linearly interpolated from the original 
    /// ones.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile X3DTexture2DNode_image.dot 
    auto_ptr< SFBool > scaleToPowerOfTwo;
    
    /// The textureProperties field contains a TextureProperties node
    /// which allows fine control over a texture's application.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DTexture2DNode_textureProperties.dot 
    auto_ptr< SFTextureProperties >  textureProperties;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:

    /// Returns the default dimensions to use when this texture is saved to file.
    ///
    /// Returns the dimensions of the Image object
    ///
    virtual std::pair<H3DInt32,H3DInt32> getDefaultSaveDimensions ();

    /// The OpenGL texture id of the installed texture, 0 if not installed.
    GLuint texture_id;
    /// The OpenGL texture unit that is used to render this texture.
    GLint texture_unit;
    /// The OpenGL texture target that is used to render this texture.
    GLenum texture_target;  

    /// Field to indicate image will change
    auto_ptr< Field > imageNeedsUpdate;
  };
}

#endif
