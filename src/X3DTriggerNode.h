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
/// \file X3DTriggerNode.h
/// \brief Header file for X3DTriggerNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTRIGGERNODE_H__
#define __X3DTRIGGERNODE_H__

#include "X3DChildNode.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class X3DTriggerNode
  /// \brief This abstract node type is the base node type from which all Triggers are derived.
  ///
  /// \par Internal routes:
  /// \dotfile X3DTriggerNode.dot
  
	class X3DTriggerNode : public X3DChildNode {
  public:

    /// Constructor.
    X3DTriggerNode( Inst< SFNode > _metadata = 0 );

    /// The H3DNodedatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
