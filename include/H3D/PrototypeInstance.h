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
/// \file PrototypeInstance.h
/// \brief Header file for PrototypeInstance, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PROTOTYPEINSTANCE_H__
#define __PROTOTYPEINSTANCE_H__

#include <H3D/X3DPrototypeInstance.h>

namespace H3D {

  /// \ingroup Nodes
  /// \class PrototypeInstance
  /// \brief The PrototypeInstance is an instance of a prototyped node.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Prototype.x3d">Prototype.x3d</a>
  ///     ( <a href="examples/Prototype.x3d.html">Source</a> )
  class H3DAPI_API PrototypeInstance : public X3DPrototypeInstance {
  public:
    /// Constructor.
    PrototypeInstance( Node *_prototyped_node,
                       Inst< SFNode>  _metadata = 0 ):
      X3DPrototypeInstance( _metadata ) {
      type_name = "PrototypeInstance";
     
      setPrototypedNode( _prototyped_node );
    }

    /// Returns a new instance of this node type with the same state as this one
    ///
    /// The default implementation copies the node's registered field values
    /// of access type INPUT_OUTPUT and INITIALIZE_ONLY.
    ///
    /// \param deepCopy If true then references to other nodes will also be cloned. 
    ///                 Otherwise just the pointer is copied.
    ///
    /// \param deepCopyMap A map from original nodes to their cloned versions. 
    ///                    This parameter is normally not required by the root caller, 
    ///                    but is passed down the call graph to ensure that nodes that
    ///                    appear multiple times will be assigned the same clone.
    ///
    virtual Node* clone ( bool deepCopy= true, DeepCopyMap *deepCopyMap= NULL );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
