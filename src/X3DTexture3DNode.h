//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file X3DTexture3DNode.h
/// \brief Header file for X3DTexture3DNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURE3DNODE_H__
#define __X3DTEXTURE3DNODE_H__

#include "X3DTextureNode.h"
#include "H3DImageObject.h"

namespace H3D {
  /// \ingroup AbstractNodes
  /// \brief This abstract node type is the base type for all node types which
  /// specify 3D sources for texture images. 
  ///
  /// \par Internal routes:
  /// \dotfile X3DTexture3DNode.dot  
  class H3DAPI_API X3DTexture3DNode : 
    public X3DTextureNode,
    public H3DImageObject {
  public:
       
    /// Constructor.
    X3DTexture3DNode( Inst< DisplayList > _displayList = 0,
                      Inst< SFNode  > _metadata  = 0,
                      Inst< SFBool  > _repeatS   = 0,
                      Inst< SFBool  > _repeatT   = 0,
                      Inst< SFBool  > _repeatR   = 0,
                      Inst< SFBool  > _scaleToP2 = 0,
                      Inst< SFImage > _image     = 0,
                      Inst< SFBool  > _interpolate = 0 );

    /// Performs the OpenGL rendering required to install the image
    /// as a texture.
    virtual void render();

    /// Virtual function for making all OpenGL calls that are needed to
    /// enable texturing for the texture.
    ///
    inline virtual void enableTexturing() {
      glEnable( GL_TEXTURE_3D );
      Image * i = static_cast< Image * >(image->getValue());
      if( i && 
          ( i->pixelType() == Image::LUMINANCE_ALPHA || 
            i->pixelType() == Image::RGBA || 
            i->pixelType() == Image::BGRA ) ) {
        glEnable( GL_BLEND );
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }
    }
      
    /// Virtual function for making all OpenGL calls that are needed to
    /// disable texturing for the texture.
    ///
    inline virtual void disableTexturing() {
      glDisable( GL_TEXTURE_3D );
      Image * i = static_cast< Image * >(image->getValue());
      if( i && 
          ( i->pixelType() == Image::LUMINANCE_ALPHA || 
            i->pixelType() == Image::RGBA || 
            i->pixelType() == Image::BGRA ) ) {
        glDisable( GL_BLEND );
      }
    }

    /// Get the OpenGL texture id that is used for this texture.
    inline GLuint getTextureId() {
      return texture_id;
    }

    /// Get the OpenGL texture unit that is used for this texture.
    inline GLuint getTextureUnit() {
      return texture_unit;
    }

    /// Sometimes the texture represents a volume in space, e.g. when
    /// it contains volume data to be rendered. This function returns
    /// the size of the space the volume occupies in metres. 
    virtual Vec3f textureSize() { 
      Image *tex_image = image->getValue();
      if( tex_image ) {
        Vec3f size = tex_image->pixelSize();
        size.x *= tex_image->width();
        size.y *= tex_image->height();
        size.z *= tex_image->depth();
        return size;
      } else {
        return Vec3f( 0, 0, 0 );
      } 
    }

    /// Installs the given image as a OpenGL texture with a call to 
    /// the glTexImage3D function. This function is used by renderImage ()
    /// and uses the glInternalFormat (), glPixelFormat () and
    /// glPixelComponentType () functions to get the arguments to the
    /// glTexImage3D call.
    ///
    virtual void glTexImage( Image *image, GLenum texture_target, 
                             bool scale_to_power_of_two );
      
    /// If true the texture will repeat itself when the s texture coordinate
    /// is outside the range [0,1]. If false the texture will be clamped if
    /// outside the same range.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    ///
    /// \dotfile X3DTexture3DNode_repeatS.dot 
    auto_ptr< SFBool >  repeatS;

    /// If true the texture will repeat itself when the t texture coordinate
    /// is outside the range [0,1]. If false the texture will be clamped if
    /// outside the same range.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    ///
    /// \dotfile X3DTexture3DNode_repeatT.dot 
    auto_ptr< SFBool >  repeatT;

    /// If true the texture will repeat itself when the r texture coordinate
    /// is outside the range [0,1]. If false the texture will be clamped if
    /// outside the same range.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    ///
    /// \dotfile X3DTexture3DNode_repeatR.dot 
    auto_ptr< SFBool >  repeatR;

    /// If true the image used will be scaled so that the dimensions are a 
    /// power of two if they are not. This will however take up more memory
    /// and might cause some unwanted strething effects on the texture. The
    /// new texture values will be linearly interpolated from the original 
    /// ones.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    ///
    /// \dotfile X3DTexture3DNode_image.dot 
    auto_ptr< SFBool > scaleToPowerOfTwo;
    
    /// If true the texture values will be interpolated when applied to a
    /// geometry.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile X3DTexture3DNode_interpolate.dot 
    auto_ptr< SFBool >  interpolate;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The OpenGL texture id of the installed texture, 0 if not installed.
    GLuint texture_id;
    /// The OpenGL texture unit that is used to render this texture.
    GLint texture_unit;
  };
}

#endif
