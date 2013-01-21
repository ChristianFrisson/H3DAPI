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
/// \file X3DParametricGeometryNode.h
/// \brief Header file for X3DParametricGeometryNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPARAMETRICGEOMETRYNODE_H__
#define __X3DPARAMETRICGEOMETRYNODE_H__

#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DParametricGeometryNode
  /// \brief This abstract node type is the base type for all geometry node
  /// types that are created parametrically and use control points to describe 
  /// the final shape of the surface. 
  /// How the control points are described and interpreted shall be a property
  /// of the individual node type.
  class H3DAPI_API X3DParametricGeometryNode : 
    public X3DGeometryNode {
  public:
    /// Constructor.
    X3DParametricGeometryNode( Inst< SFNode      >  _metadata = 0,
                               Inst< SFBound     > _bound = 0,
                               Inst< DisplayList > _displayList = 0,
                               Inst< MFBool      > _isTouched = 0,
                               Inst< MFVec3f     > _force = 0,
                               Inst< MFVec3f     > _contactPoint = 0,
                               Inst< MFVec3f     > _contactNormal = 0);
  };
}

#endif
