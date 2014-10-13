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
/// \file H3DSingleTextureNode.h
/// \brief Header file for H3DSingleTextureNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSINGLETEXTURENODE_H__
#define __H3DSINGLETEXTURENODE_H__

#include <H3D/X3DTextureNode.h>
#include <H3D/H3DImageObject.h>
#include <H3D/TextureProperties.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {
  /// \ingroup AbstractNodes
  /// \brief This abstract node type is the base type for all texture 
  /// nodes that is defined by a single texture.
  ///
  /// More specifically the texture can be defined by a single OpenGL 
  /// texture id.
  class H3DAPI_API H3DSingleTextureNode : 
    public X3DTextureNode {
  public:
    
    /// Constructor.
    H3DSingleTextureNode( Inst< DisplayList > _displayList = 0,
                          Inst< SFNode  > _metadata  = 0 );

    /// Destructor
    virtual ~H3DSingleTextureNode ();

    /// Get the OpenGL texture id that is used for this texture.
    virtual GLuint getTextureId() = 0;

    /// Get the OpenGL texture unit that is used for this texture.
    virtual GLuint getTextureUnit() = 0;

    /// Get the OpenGL texture target that is used for this texture.
    virtual GLenum getTextureTarget() = 0;

    ///{
    /// Bindless texturing functions

    /// Make the texture resident as a bindless texture.
    ///
    /// The first time a texture is made resident it becomes immutable
    virtual bool makeResident ();

    /// Make the texture non-resident
    virtual void makeNonResident ();

    /// Returns true if the texture is resident in GPU memory or false otherwise
    virtual bool isResident () { return is_resident; }

    /// Get the bindless texture handle, or 0 the texture has
    /// never been resident.
    ///
    /// If the texture handle is non-zero then the texture is
    /// immutable and the size or other properties cannot be changed
    virtual GLuint64 getTextureHandle() { return texture_handle; }

    /// This should be called if the texture object is to be replaced
    ///
    /// This will cause the texture to become non-resident and for the texture
    /// handle to be regenerated the next time the texture is made resident
    virtual void invalidateTextureHandle ();

    /// Add a shader field which contains this texture node
    ///
    /// The field will be touched when the texture handle is made
    /// resident, in order to allow the shader uniforms that reference
    /// it to be updated.
    ///
    /// X3DProgrammableShader node takes care of calling these functions
    /// and only does so if bindless textures are enabled.
    void addShaderField ( Field& _field );

    /// Remove a shader field which contains this texture node
    ///
    /// See addShaderField()
    void removeShaderField ( Field& _field );
    
    /// Call to signify that this texture is currently in use
    ///
    /// Textures that are not in use are periodically detected and
    /// made non-resident
    virtual void inUse ();

    /// Look at all resident textures and make those that have not been 
    /// used recently non-resident
    ///
    /// \param _unusedTime Length of time for which textures may be unused
    ///                    before they can be made non-resident.
    static void makeUnusedTexturesNonResident ( H3DTime _unusedTime );

    ///}

  protected:
    typedef std::vector < H3DSingleTextureNode* > TextureVector;

    /// The texture handle (GPU)
    GLuint64 texture_handle;

    /// Shader fields which contain this texture
    Field::FieldVector shader_fields;
    
    /// Time-stamp at which this texture was last used
    H3DTime last_used;

    /// True if this texture is currently resident
    bool is_resident;
    
    /// List of all currently resident textures
    static TextureVector resident_textures;
  };
}

#endif
