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
/// \file ContourPolyline2D.h
/// \brief Header file for ContourPolyline2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CONTOURPOLYLINE2D_H__
#define __CONTOURPOLYLINE2D_H__

#include <H3D/X3DNurbsControlCurveNode.h>
#include <H3D/MFVec2d.h>

namespace H3D {

  /// \ingroup X3DNode
  /// \class ContourPolyline2D
  /// \brief The ContourPolyline2D node defines a piecewise linear curve 
  /// segment as a part of a trimming contour in the u,v domain of a surface.
  ///
  /// The controlPoint field specifies the end points of each segment of 
  /// the piecewise linear curve.
  ///
  /// ContourPolyline2D nodes are used as children of the Contour2D group.
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsTrimmedSurface.x3d">NurbsTrimmedSurface.x3d</a>
  ///     ( <a href="examples/NurbsTrimmedSurface.x3d.html">Source</a> )
  ///

  class H3DAPI_API ContourPolyline2D : 
    public X3DNurbsControlCurveNode {
  public:

    /// Constructor.
    ContourPolyline2D( Inst< SFNode   > _metadata = 0,
                      Inst< MFVec2d  > _controlPoint = 0 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
