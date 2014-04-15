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
/// \file X3DNurbsControlCurveNode.h
/// \brief Header file for X3DNurbsControlCurveNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DNURBSCONTROLCURVENODE_H__
#define __X3DNURBSCONTROLCURVENODE_H__

#include <H3D/X3DNode.h>
#include <H3D/MFVec2d.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DNurbsControlCurveNode
  /// \brief This abstract node type is the base type for all node types that
  /// provide control curve information in 2D space. 
  class H3DAPI_API X3DNurbsControlCurveNode : 
    public X3DNode {
  public:
    /// Constructor.
    X3DNurbsControlCurveNode( Inst< SFNode      > _metadata     = 0,
                              Inst< MFVec2d     > _controlPoint = 0);

    /// The control points are defined in 2D coordinate space and interpreted 
    /// according to the descendent node type as well as the user of this 
    /// node instance.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFVec2d >  controlPoint;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
