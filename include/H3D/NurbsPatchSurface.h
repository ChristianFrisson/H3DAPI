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
/// \file NurbsPatchSurface.h
/// \brief Header file for NurbsPatchSurface, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NURBSPATCHSURFACE_H__
#define __NURBSPATCHSURFACE_H__

#include <H3D/X3DNurbsSurfaceGeometryNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class NurbsPatchSurface
  /// \brief The NurbsPatchSurface node is a contiguous NURBS surface patch.
  ///
  /// uDimension and vDimension define the number of control points in 
  /// the u and v dimensions. 
  ///
  /// uOrder and vOrder define the order of the surface in the u and v 
  /// dimensions.
  ///
  /// uKnot and vKnot define the knot values of the surface in the u and
  /// v dimensions.
  ///
  /// uClosed and vClosed define whether or not the specific dimension 
  /// is to be evaluated as a closed surface or not in the u and v dimensions.
  ///
  /// controlPoint defines a set of control points of dimension 
  /// uDimension × vDimension. This set of points defines a mesh where the
  /// points do not have a uniform spacing. uDimension points define a 
  /// polyline in u-direction followed by further u-polylines with the 
  /// v-parameter in ascending order. The number of control points shall be
  /// equal or greater than the order. A closed surface shall be specified
  /// by repeating the limiting control points and setting the closed field 
  /// to TRUE. If the closed field is set to FALSE, the implementation shall
  /// not be required to smoothly blend the edges of the surface in that
  /// dimension into a continuous surface. If the last set of control points
  /// are not identical, the field is ignored.
  ///
  /// The control vertex corresponding to the control point P[i,j] on the 
  /// control grid is: 
  ///
  /// P[i,j].x = controlPoints[i + ( j × uDimension)].x
  /// P[i,j].y = controlPoints[i + ( j × uDimension)].y
  /// P[i,j].z = controlPoints[i + ( j × uDimension)].z
  /// P[i,j].w = weight[ i + (j × uDimension)]
  ///
  /// where 0 = i < uDimension and 
  ///       0 = j < vDimension.
  ///
  /// For an implementation subdividing the surface in a equal number of
  /// subdivision steps, tessellation values could be interpreted in the 
  /// following way:
  ///
  /// if a tessellation value is greater than 0, the number of tessellation
  /// points is:
  ///
  ///  tessellation+1;
  ///
  /// if a tessellation value is smaller than 0, the number of tessellation
  /// points is:
  ///
  ///   -tessellation × (u/v)dimension)+1;
  ///
  /// if a tessellation value is 0, the number of tessellation points is:
  ///
  ///    (2 × (u/v)dimension)+1. 
  ///
  /// For implementations doing tessellations based on chord length, 
  /// tessellation values less than zero are interpreted as the maximum chord
  /// length deviation in pixels. Implementations doing fully automatic 
  /// tessellation may ignore the tessellation hint parameters.
  ///
  /// texCoord provides additional information on how to generate texture
  /// coordinates. By default, texture coordinates in the unit square (or
  /// cube for 3D coordinates) are generated automatically from the parametric
  /// subdivision. A NurbsTextureCoordinate node or simply a TextureCoordinate
  /// node can then be used to compute a texture coordinate given a u/v 
  /// parameter of the NurbsSurface. The NurbsTextureCoordinate also supports 
  /// non-animated surfaces to specify a "chord length"-based texture 
  /// coordinate parametrization. 
  /// 
  /// The solid field determines whether the surface is visible when viewed
  /// from the inside
  ///
  /// closed defines whether the curve should be rendered as a closed object
  /// in the given parametric direction allowing the object to be closed in
  /// one direction, but not the other (EXAMPLE  cylinder).
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsPatchSurface.x3d">NurbsPatchSurface.x3d</a>
  ///     ( <a href="examples/NurbsPatchSurface.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NurbsPatchSurface.dot
  class NurbsPatchSurface : 
    public X3DNurbsSurfaceGeometryNode {
  public:
    
    /// Constructor.
    NurbsPatchSurface( Inst< SFNode      >  _metadata = 0,
                       Inst< SFBound     > _bound = 0,
                       Inst< DisplayList > _displayList = 0,
                       Inst< MFBool      > _isTouched = 0,
                       Inst< MFVec3f     > _force = 0,
                       Inst< MFVec3f     > _contactPoint = 0,
                       Inst< MFVec3f     > _contactNormal = 0,
                       Inst< SFCoordinateNode > _controlPoint = 0,
                       Inst< TexCoordOrNurbsTex >  _texCoord = 0,
                       Inst< SFInt32     > _uTessellation = 0,
                       Inst< SFInt32     > _vTessellation = 0,
                       Inst< MFDouble    > _weight = 0,
                       Inst< SFBool      > _solid = 0,
                       Inst< SFBool      > _uClosed = 0,
                       Inst< SFBool      > _vClosed = 0,
                       Inst< SFInt32     > _uDimension = 0,
                       Inst< SFInt32     > _vDimension = 0,
                       Inst< MFDouble    > _uKnot = 0,
                       Inst< MFDouble    > _vKnot = 0,
                       Inst< SFInt32     > _uOrder = 0,
                       Inst< SFInt32     > _vOrder = 0 );

    // Function called by render to render the small part that differs
    // between NurbsPatchSurface and NurbsTrimmedSurface. The arguments
    // are things that are needed to render that difference.
    virtual void renderBetweenBeginEnd(
                                GLUnurbsObj *nurbs_object,
                                X3DTextureCoordinateNode *tex_coord_node,
                                TextureCoordinateGenerator *tex_coord_gen,
                                NurbsTextureCoordinate *nurbs_tex_coord, 
                                GLfloat *texKnotU,
                                GLfloat *texKnotV,
                                GLfloat *texCoordinates, 
                                GLfloat *u_knots, 
                                GLfloat *v_knots, 
                                H3DInt32 &sizeOfVertex,
                                GLfloat *withWeights, 
                                H3DInt32 &uSizeToUse, 
                                H3DInt32 &vSizeToUse, 
                                H3DInt32 &map2Vertex3Or4 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
