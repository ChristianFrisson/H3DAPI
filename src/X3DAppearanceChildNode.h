//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file X3DAppearanceChildNode.h
/// \brief Header file for X3DAppearanceChildNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DAPPEARANCECHILDNODE_H__
#define __X3DAPPEARANCECHILDNODE_H__

#include <X3DNode.h>
#include <H3DDisplayListObject.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DAppearanceChildNode
  /// \brief This is the base node type for the child nodes of the
  ///  X3DAppearanceNode type.
  ///
  /// 
  class H3DAPI_API X3DAppearanceChildNode : 
    public X3DNode, public H3DDisplayListObject {
  public:
  
    /// Constructor.
    X3DAppearanceChildNode( Inst< DisplayList > _displayList = 0,
                            Inst< SFNode>  _metadata = 0 );
    
    /// This function will be called by the X3DShapeNode before any rendering 
    /// of geometry and before the call to the render function. So this is the
    /// place to save the states that are going to be changed in render() in
    /// order to restore it in postRender().
    virtual void preRender() {}

    /// This function will be called by the X3DShapeNode after the geometry
    /// has been rendered to restore the states to what it was before 
    /// the call to preRender().
    virtual void postRender() {}
  };
}

#endif
