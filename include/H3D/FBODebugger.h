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
/// \file FBODebugger.h
/// \brief Header file for FBODebugger.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FBODEBUGGER_H__
#define __FBODEBUGGER_H__

#include <H3D/X3DChildNode.h>
#include <H3D/Group.h>
#include <H3D/SFString.h>
#include <H3D/X3D.h>
#include <H3D/RenderTargetTexture.h>

namespace H3D {

  /// \class FBODebugger
  /// \brief The FBODebugger node provides a convenient way to check the result of
  /// all FrameBufferTextureGenerator nodes in the scene.
  ///
  /// The FBODebugger node can be put into the scene-graph and will draw the 
  /// content of a buffer in a FrameBufferTextureGenerator in fullscreen when
  /// rendered.
  /// 
  /// The fbo field contains the name of the FrameBufferTextureGenerator that
  /// you want to see the buffers in. The valid values changes depending on the
  /// scene and it is easiest to use the Tree view to change between the 
  /// different available buffers. If set to "NONE" it will do nothing.
  ///
  /// The buffer field specifies which buffer to display. Valid values are:
  ///
  /// -"DEPTH" - the depth buffer
  /// -"COLOR0" - the color buffer with index 0
  /// -"COLOR1" - the color buffer with index 1
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FBODebugger.x3d">FBODebugger.x3d</a>
  ///     ( <a href="examples/FBODebugger.x3d.html">Source</a> )
  class FBODebugger : public X3DChildNode {
  public:
    
    /// Constructor.
    FBODebugger( Inst< SFNode   > _metadata = 0,
                 Inst< SFString > _fbo = 0,
                 Inst< SFString > _buffer = 0 );
    
    /// Traverse the scenegraph. 
    virtual void traverseSG( TraverseInfo &ti );

    /// Render the node. Render the FBO texture in fullscreen.
    virtual void render();

    /// The fbo field contains the name of the FrameBufferTextureGenerator that
    /// you want to see the buffers in. The valid values changes depending on the
    /// scene and it is easiest to use the Tree view to change between the 
    /// different available buffers. If set to "NONE" it will do nothing.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "NONE"
    /// <b>Valid values: "NONE" or the name of any FrameBufferTextureGenerator in the scene</b> 
    auto_ptr< SFString > fbo;

    /// The buffer field specifies which buffer to display. Valid values are:
    ///
    /// -"DEPTH" - the depth buffer
    /// -"COLOR0" - the color buffer with index 0
    /// -"COLOR1" - the color buffer with index 1
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "NONE"
    /// <b>Valid values: "NONE" or the name of any FrameBufferTextureGenerator in the scene</b> 
    auto_ptr< SFString > buffer;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The scene used to render the selected texture in fullscreen.
    AutoRef< Group > texture_scene;

    /// DEFNodes from the texture_scene.
    X3D::DEFNodes texture_scene_dn;

    /// The type of buffer currently being used.
    string current_buffer;

    /// The name of the FrameBufferTextureGenerator currently being used.
    string current_fbo;

    /// RenderTargetTexture used to show the color buffers of a fbo if
    /// selected.
    AutoRef<RenderTargetTexture> render_target_texture;
  };
}

#endif
