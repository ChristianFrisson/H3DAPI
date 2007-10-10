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
/// \file X3DMaterialNode.h
/// \brief Header file for X3DMaterialNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DMATERIALNODE_H__
#define __X3DMATERIALNODE_H__

#include <H3D/X3DAppearanceChildNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DMaterialNode
  /// \brief This is the base node type for all Material nodes.
  ///
  /// 
  class H3DAPI_API X3DMaterialNode : public X3DAppearanceChildNode {
  public:
    
    /// Constructor.
    X3DMaterialNode( Inst< DisplayList > _displayList = 0,
                     Inst< SFNode>  _metadata = 0 );

    /// This function checks the transparency field to determine if the
    /// material requires that the geometry is rendered with transparency
    virtual bool isTransparent() {
      return false;
    }
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "material".
    ///
    virtual string defaultXMLContainerField() {
      return "material";
    }
  };
}

#endif
