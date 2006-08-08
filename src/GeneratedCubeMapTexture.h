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
/// \file GeneratedCubeMapTexture.h
/// \brief Header file for GeneratedCubeMapTexture, X3D abstract scene-graph 
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COMPOSEDCUBEMAPTEXTURE_H__
#define __COMPOSEDCUBEMAPTEXTURE_H__

#include "X3DEnvironmentTextureNode.h"
#include "Viewpoint.h"
#include "X3DTexture2DNode.h"
#include "DependentNodeFields.h"
#include <list>
#include "SFInt32.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class GeneratedCubeMapTexture
  /// The ComposedCubeMapTexture node defines a cubic environment map 
  /// that sources its data from internally generated images. 
  ///
  /// The viewpoint location of a texture is the location of the geometry
  /// in world space.
  ///  
  /// The field of view shall be 90 degrees with an aspect ratio of 1:1.
  /// 
  /// The size field indicates the resolution of the generated images in 
  /// number of pixels per side.
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
  /// \par Internal routes:
  /// \dotfile GeneratedCubeMapTexture.dot
  class H3DAPI_API GeneratedCubeMapTexture : public X3DEnvironmentTextureNode {
  public:
    /// Constructor.
    GeneratedCubeMapTexture( Inst< DisplayList > _displayList = 0,
                             Inst< SFNode      > _metadata   = 0,
                             Inst< SFString    > _update     = 0,
                             Inst< SFInt32     > _size       = 0 );

    /// Destructor.
    ~GeneratedCubeMapTexture();

    /// Enables cube map texturing.
    virtual void enableTexturing();

    /// Disables cube map texturing.
    virtual void disableTexturing();

    /// Creates the cube map from the texture fields.
    virtual void render();

    /// preRender disables rendering if it is called when rendering
    /// the cube maps for this node. This will cause the geometry the
    /// GeneratedCubeMapTexture is applied to, not to be rendered as
    /// part of the cube map.
    virtual void preRender() {
      X3DEnvironmentTextureNode::preRender();
      if( generating_textures ) {
        glPushAttrib( GL_COLOR_BUFFER_BIT );
        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_NEVER, 0 );
      }
    }

    /// postRender enables rendering again if it was disabled by preRender.
    virtual void postRender() {
      X3DEnvironmentTextureNode::postRender();
      if( generating_textures ) {
        glPopAttrib();
      }
    }
    
    /// Initialize OpenGL textures by allocation memory and setting up
    /// texture parameters.
    virtual void initializeTextures();

    /// Updates the cube map textures by rendering the given node
    /// from all directionss and saving the result as textures.
    /// \param n The scenegraph to render. 
    /// \param vp The viewpoint the scene is currently rendered with
    /// in the normal rendering.
    virtual void updateCubeMapTextures( X3DChildNode *n,
                                        Viewpoint *vp );

#ifdef USE_HAPTICS
    /// Saves the accumulatedForwardMatrix in the local_to_global
    /// member variable.
    virtual void traverseSG( TraverseInfo &ti );
#endif

    /// Updates the cube map textures of all instances of 
    /// GeneratedCubeMapTexture. The update field will be checked to 
    /// see if an update is required. 
    /// \param n The scenegraph to render. 
    /// \param vp The viewpoint the scene is currently rendered with
    /// in the normal rendering.
    static void updateAllCubeMapTextures( X3DChildNode *n,
                                          Viewpoint *vp );

    /// The texture to use on the back face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile GeneratedCubeMapTexture_back.dot
    auto_ptr< SFString > update;

    /// The texture to use on the front face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile GeneratedCubeMapTexture_front.dot
    auto_ptr< SFInt32 > size;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// True when the cube map textures has been initialized.
    bool textures_initialized;
    
    /// True if this not is currrently rendering the scene in order
    /// to create a cube map.
    bool generating_textures;

    /// Renders a scene and puts it into a texture. 
    /// \param texture_target The texture target of the texture to put
    /// the rendered scene in.
    /// \param camera_position The position of the camera used to render the
    /// scene (in the coordinate system of vp)
    /// \param camera_dir The look at vector of the camera.
    /// \param camera_up The up vector of the camera.
    /// \param n The node to render.
    /// \param vp The Viewpoint that is used currently in the normal
    /// rendering of the scene.
    virtual void updateCubeMap( GLuint texture_target,
                                const Vec3f & camera_position,
                                const Vec3f & camera_dir,
                                const Vec3f & camera_up,
                                X3DChildNode *n,
                                Viewpoint *vp );

    /// Transformation matrix from the local coordinate system of the node
    /// to the global coordinate system.
    Matrix4f local_to_global;

    /// All instances of GeneratedCubeMapTexture that has been created.
    static list< GeneratedCubeMapTexture * > instances;
  };
}

#endif
