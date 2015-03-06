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
/// \file GeneratedTexture3D.h
/// \brief Header file for GeneratedTexture3D.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GENERATEDTEXTURE3D_H__
#define __GENERATEDTEXTURE3D_H__

#include <H3D/X3DTexture3DNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class GeneratedTexture3D
  /// \brief The GeneratedTexture node is a 3d-texture node that are automatically
  /// generated from nodes like FrameBufferTextureGenerator.
  ///
  /// Basically it is just a wrapper around a OpenGL texture id and the generator
  /// will set up all parameters for it.
  ///
  /// The GeneratedTexture node will never set the image field of it's base class
  /// since all the texture operations are performed directly in OpenGL without
  /// storing the data in a separate container such as the image field.
  /// 
  /// This node cannot be instantiated from X3D or Python.
  class H3DAPI_API GeneratedTexture3D : public X3DTexture3DNode {
  public:
        
    /// Constructor.
    GeneratedTexture3D( Inst< DisplayList > _displayList = 0,
                        Inst< SFNode>  _metadata = 0 );
        
    /// Destructor.
    virtual ~GeneratedTexture3D() {}

    /// Ensure that the OpenGL texture id for this texture is initialized. 
    /// \param texture_target The OpenGL texture target the texture id is 
    /// to be used for. E.g. GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY_EXT.
    /// \return  true on success, false on failure to initialize.
    bool ensureInitialized( GLenum _texture_target = GL_TEXTURE_3D );
    
    /// Returns true if the OpenGL texture id for this texture is initialized.
    inline bool textureIdIsInitialized() {
      return texture_id_initialized;
    }

    /// Create a new texture ID for the texture, destroying the old one
    void reinitialize ();

    /// Render this texture.
    virtual void render();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// True of the texture_id parameter has been initialized.
    bool texture_id_initialized;
  };
}

#endif
