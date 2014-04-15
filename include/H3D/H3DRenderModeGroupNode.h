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
/// \file H3DRenderModeGroupNode.h
/// \brief Header file for H3DRenderModeGroupNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DRENDERMODEGROUPNODE_H__
#define __H3DRENDERMODEGROUPNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DBoundedObject.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/X3DGroupingNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DRenderModeGroupNode
  /// \brief The H3DRenderModeGroupNode is a base class for all nodes
  /// that wants in some way change the OpenGL state during the rendering
  /// of all X3DGeometryNode instances in the children field. This will 
  /// override the settings in Appearance nodes such as shaders, texturing,
  /// culling etc.
  class H3DAPI_API H3DRenderModeGroupNode : 
  public X3DGroupingNode {
  public:
    
    /// Constructor.
    H3DRenderModeGroupNode( Inst< AddChildren    > _addChildren    = 0,
          Inst< RemoveChildren > _removeChildren = 0,
          Inst< MFChild        > _children       = 0,
          Inst< SFNode         > _metadata       = 0,
          Inst< SFBound        > _bound          = 0,
          Inst< SFVec3f        > _bboxCenter     = 0,
          Inst< SFVec3f        > _bboxSize       = 0 );

    /// Virtual function that should be implemented by all sub-classes
    /// to save the current OpenGL state of the parameters that is
    /// going to be changed in changeGeometryState. 
    virtual void saveCurrentState() = 0;

    /// Add the changes to the OpenGL state that is needed.
    virtual void changeGeometryState() = 0;

    /// Virtual function that should restore the OpenGL state to what it
    /// was before the call to saveCurrentState.
    virtual void restoreState() = 0;

    /// Render the children fields.
    virtual void render();

    /// Traverse the scene graph.
    virtual void traverseSG( TraverseInfo &ti );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    protected:
    friend class X3DGeometryNode;
    static H3DRenderModeGroupNode *current_render_mode_group;
  };
}

#endif
