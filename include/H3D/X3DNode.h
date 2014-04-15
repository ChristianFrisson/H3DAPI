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
/// \file X3DNode.h
/// \brief Header file for X3DNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DNODE_H__
#define __X3DNODE_H__

#include <H3D/SFNode.h>
#include <H3D/Node.h>
#include <H3D/FieldTemplates.h>

namespace H3D {

  /// \ingroup Nodes
  /// \defgroup X3DNodes X3D node classes.
  /// These are the X3D Node classes available in H3DAPI.

  /// \ingroup Nodes
  /// \defgroup H3DNodes H3D specific Node classes.
  /// These are the Node classes available in H3DAPI that are not part of the
  /// X3D-specification.

  /// \ingroup Nodes
  /// \defgroup AbstractNodes Abstract nodes.
  /// An abstract node is a node that cannot be instantiated itself via
  /// the H3DNodeDatabase but is a common base class for other nodes.

  /// \defgroup AbstractInterface Abstract interface classes.
  /// An abstract interface is a class that defines functions and variables
  /// that are to be implemented in the class that inherits from them. A node 
  /// implements an abstract interface by both inheriting from its base node
  /// class and the abstract interface and implements the required interface.
  
  /// \ingroup AbstractNodes
  /// \class X3DNode
  /// \brief This abstract node type is the base type for all nodes in the X3D
  ///  system. 
  ///
  /// 
  class H3DAPI_API X3DNode : public Node {
  public:
    
    /// Constructor.
    X3DNode( Inst< SFNode>  _metadata = 0 );

    /// Meta data about the node.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFNode >  metadata;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
