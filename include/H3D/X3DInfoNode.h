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
/// \file X3DInfoNode.h
/// \brief Header file for X3DInfoNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DINFONODE_H__
#define __X3DINFONODE_H__

#include <H3D/X3DChildNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DInfoNode
  /// \brief This is the base node type for all nodes that contain only 
  /// information without visual semantics.
  class H3DAPI_API X3DInfoNode : public X3DChildNode {
  public:
    /// Constructor.
    X3DInfoNode( Inst< SFNode>  _metadata = 0 ) :
      X3DChildNode( _metadata ){
      type_name = "X3DInfoNode";
    }
  };
}

#endif
