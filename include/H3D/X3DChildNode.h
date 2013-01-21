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
/// \file X3DChildNode.h
/// \brief Header file for X3DChildNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DCHILDNODE_H__
#define __X3DCHILDNODE_H__

#include <H3D/X3DNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DChildNode
  /// \brief This abstract node type indicates that the concrete nodes which 
  /// are instantiated based on it may be used in children, addChildren, and
  /// removeChildren fields. 
  ///
  class H3DAPI_API X3DChildNode : public X3DNode {
  public:
    
    /// Constructor.
    X3DChildNode( Inst< SFNode>  _metadata = 0 );

  };
}

#endif
