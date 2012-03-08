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
/// \file NurbsCurve2D.h
/// \brief Header file for NurbsCurve2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NURBSCURVE2D_H__
#define __NURBSCURVE2D_H__

#include <H3D/X3DNurbsControlCurveNode.h>
#include <H3D/SFInt32.h>
#include <H3D/MFDouble.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNode
  /// \class NurbsCurve2D
  /// \brief The NurbsCurve2D node defines a trimming segment that is part of a
	/// trimming contour in the u,v domain of the surface.
	/// NurbsCurve2D nodes are used as children of the Contour2D group.
	/// 
	/// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsTrimmedSurface.x3d">NurbsTrimmedSurface.x3d</a>
  ///     ( <a href="examples/NurbsTrimmedSurface.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NurbsCurve2D.dot

  class H3DAPI_API NurbsCurve2D : 
    public X3DNurbsControlCurveNode {
  public:

    /// Constructor.
    NurbsCurve2D( Inst< SFNode	 > _metadata		 = 0,
									Inst< MFVec2d  > _controlPoint = 0,
									Inst< SFInt32	 > _tessellation = 0,
									Inst< MFDouble > _weight			 = 0,
									Inst< MFDouble > _knot				 = 0,
									Inst< SFInt32	 > _order				 = 0,
									Inst< SFBool	 > _closed			 = 0 );

		/// The tessellation field gives a hint to the curve tessellator by setting
		/// an absolute number of subdivision steps. These values shall be greater 
		/// than or equal to the Order field. A value of 0 indicates that the 
		/// browser choose a suitable tessellation. Interpretation of values 
		/// below 0 is implementation dependent.
    ///
    /// <b>Access type:</b> inputOutput \n
		/// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile NurbsCurve2D_tessellation.dot
    auto_ptr< SFInt32 > tessellation;

    /// A weight value that shall be greater than zero is assigned to each 
		/// controlPoint. The ordering of the values is equivalent to the ordering
		/// of the control point values. The number of values shall be identical 
		/// to the number of control points. If the length of the weight vector 
		/// is 0, the default weight 1.0 is assumed for each control point, thus
		/// defining a non-Rational curve. If the number of weight values is less
		/// than the number of control points, all weight values shall be ignored 
		/// and a value of 1.0 shall be used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsCurve2D_weight.dot
    auto_ptr< MFDouble >  weight;

		/// A weight value that shall be greater than zero is assigned to each
		/// controlPoint. The ordering of the values is equivalent to the ordering
		/// of the control point values. The number of values shall be identical to
		/// the number of control points. If the length of the weight vector is 0, 
		/// the default weight 1.0 is assumed for each control point, thus defining 
		/// a non-Rational curve. If the number of weight values is less than the 
		/// number of control points, all weight values shall be ignored and a 
		/// value of 1.0 shall be used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsCurve2D_knot.dot
    auto_ptr< MFDouble >  knot;

		/// Defines the order of curve. From a mathematical point of view, the 
		/// curve is defined by a polynomial of the degree order?1. The value
		/// of order shall be greater than or equal to 2. An implementation may
		/// limit order to a certain number. If it does so, then a warning shall
		/// be generated and the surface not displayed. An implementation shall at
		/// least support orders 2,3 and 4. The number of control points shall be 
		/// at least equal to the order of the curve. The order defines the number
		/// of adjacent control points that influence a given control point.
    ///
    /// <b>Access type:</b> inputOutput \n
		/// <b>Default value:</b> 3 \n
    /// 
    /// \dotfile NurbsCurve2D_order.dot
    auto_ptr< SFInt32 >  order;

		/// closed define whether or not to evaluate the curve as closed.
    ///
    /// <b>Access type:</b> inputOutput \n
		/// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile NurbsCurve2D_closed.dot
    auto_ptr< SFBool >  closed;

		/// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
