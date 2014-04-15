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
/// \file RenderTargetSelectGroup.h
/// \brief Header file for RenderTargetSelectGroup.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RENDERTARGETSELECTGROUP_H__
#define __RENDERTARGETSELECTGROUP_H__

#include <H3D/X3DGroupingNode.h>
#include <H3D/MFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class RenderTargetSelectGroup
  /// \brief The RenderTargetSelectGroup is used in combination with the 
  /// FrameBufferTextureGenerator to select which color textures to render into.
  ///
  /// The RenderTargetSelectGroup must always be a child of a FrameBufferTextureGenerator.
  /// If used outside a FrameBufferTextureGenerator the result is undefined.
  ///
  /// The renderTargets field contains the indices of the color textures of the parent
  /// FrameBufferTextureGenerator you want to render the children into. If an index provided
  /// is outside the range of the parent FrameBufferTextureGenerator color textures the 
  /// result is undefined.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RenderTargetSelectGroup.x3d">RenderTargetSelectGroup.x3d</a>
  ///     ( <a href="examples/RenderTargetSelectGroup.x3d.html">Source</a> )
  class H3DAPI_API RenderTargetSelectGroup : public X3DGroupingNode {
  public:
    /// Constructor.
    RenderTargetSelectGroup ( Inst< AddChildren    > _addChildren    = 0,
            Inst< RemoveChildren > _removeChildren = 0,
            Inst< MFChild        > _children       = 0,
            Inst< SFNode         > _metadata       = 0,
            Inst< SFBound        > _bound          = 0,
            Inst< SFVec3f        > _bboxCenter     = 0,
            Inst< SFVec3f        > _bboxSize       = 0,
            Inst< MFInt32        > _renderTargets  = 0);

    virtual void render();

    /// The renderTargets field contains the indices of the color textures of the parent
    /// FrameBufferTextureGenerator you want to render the children into. If an index provided
    /// is outside the range of the parent FrameBufferTextureGenerator color textures the 
    /// result is undefined.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> []
    auto_ptr< MFInt32 > renderTargets;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Internal buffer for storing OpenGL enumerators for use with
    /// the glDrawBuffers call.
    auto_ptr< GLenum > draw_buffers;

    /// The maximum number of draw buffers supported by the graphics card. 
    int max_nr_draw_buffers;

    /// Flag used to avoid printing more than one error message when the graphics
    /// card does not support the extensions needed for this node to work.
    bool support_message_displayed;
  };
}

#endif
