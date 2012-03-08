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
/// \file Contour2D.h
/// \brief Header file for Contour2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CONTOUR2D_H__
#define __CONTOUR2D_H__

#include <H3D/X3DNode.h>
#include <H3D/MFNode.h>
#include <H3D/NurbsCurve2D.h>
#include <H3D/ContourPolyline2D.h>
#include "GL/glew.h"

namespace H3D {

  /// \ingroup X3DNode
  /// \class Contour2D
  /// \brief The Contour2D node groups a set of curve segments to a composite 
	/// contour. The children shall form a closed loop with the first point of 
	/// the first child repeated as the last point of the last child and the 
	/// last point of a segment repeated as the first point of the consecutive 
	/// one. The segments shall be defined by concrete nodes that implement the
	/// X3DNurbsControlCurveNode abstract type nodes and shall be enumerated in
	/// the child field in consecutive order according to the topology of the
	/// contour.
	///
	/// The 2D coordinates used by the node shall be interpreted to lie in 
	/// the (u, v) coordinate space defined by the NURBS surface.
	/// 
	/// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsTrimmedSurface.x3d">NurbsTrimmedSurface.x3d</a>
  ///     ( <a href="examples/NurbsTrimmedSurface.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Contour2D.dot

  class H3DAPI_API Contour2D : 
    public X3DNode {
  public:


		/// This field class checks whether the given node
		/// is of the right type (ContourPolyline2D or NurbsCurve2D). 
		/// If this is the case the node is added to the children field.
		class H3DAPI_API AddTheChildren: public MFNode {
		public:

			virtual void onAdd( Node *n ) {
				MFNode::onAdd( n );
				NurbsCurve2D *nurbs_curve_2D = 
					dynamic_cast< NurbsCurve2D * >( n );

				ContourPolyline2D *contour_poly_line_2D = 
					dynamic_cast< ContourPolyline2D * >( n );

				if( !nurbs_curve_2D && !contour_poly_line_2D ) {
					stringstream s;
					s << "Expecting NurbsCurve2D or ContourPolyline2D";
					throw InvalidNodeType( n->getTypeName(),
						s.str(),
						H3D_FULL_LOCATION );
				}
				else{
					Contour2D * contourNode2D = 
          static_cast< Contour2D * >( getOwner() );
						contourNode2D->children->push_back( n );
				}
			}
		};
#ifdef __BORLANDC__
    friend class AddTheChildren;
#endif

		/// This field class checks whether the given node
		/// is of the right type (ContourPolyline2D or NurbsCurve2D). 
		/// If this is the case the node is added to the children field.
		class H3DAPI_API RemoveTheChildren: public MFNode {
		public:

			
			virtual void onRemove( Node *n ) {
				NurbsCurve2D *nurbs_curve_2D = 
					dynamic_cast< NurbsCurve2D * >( n );

				ContourPolyline2D *contour_poly_line_2D = 
					dynamic_cast< ContourPolyline2D * >( n );

				if( !nurbs_curve_2D && !contour_poly_line_2D ) {
					stringstream s;
					s << "Expecting NurbsCurve2D or ContourPolyline2D";
					throw InvalidNodeType( n->getTypeName(),
						s.str(),
						H3D_FULL_LOCATION );
				}
				else {
					Contour2D * contourNode2D = 
          static_cast< Contour2D * >( getOwner() );
					contourNode2D->children->erase( n );
				}

				MFNode::onRemove( n );
			}
		};
#ifdef __BORLANDC__
    friend class RemoveTheChildren;
#endif

		/// This field class checks whether the given Node
		/// is of the right type (ContourPolyline2D or NurbsCurve2D)
		/// for adding or removing.
		class H3DAPI_API CheckTheChildren: public MFNode {
		public:

			virtual void onAdd( Node *n ) {
				MFNode::onAdd( n );
				NurbsCurve2D *nurbs_curve_2D = 
					dynamic_cast< NurbsCurve2D * >( n );

				ContourPolyline2D *contour_poly_line_2D = 
					dynamic_cast< ContourPolyline2D * >( n );

				if( !nurbs_curve_2D && !contour_poly_line_2D ) {
					stringstream s;
					s << "Expecting NurbsCurve2D or ContourPolyline2D";
					throw InvalidNodeType( n->getTypeName(),
						s.str(),
						H3D_FULL_LOCATION );
				}
			}

			virtual void onRemove( Node *n ) {
				NurbsCurve2D *nurbs_curve_2D = 
					dynamic_cast< NurbsCurve2D * >( n );

				ContourPolyline2D *contour_poly_line_2D = 
					dynamic_cast< ContourPolyline2D * >( n );

				if( !nurbs_curve_2D && !contour_poly_line_2D ) {
					stringstream s;
					s << "Expecting NurbsCurve2D or ContourPolyline2D";
					throw InvalidNodeType( n->getTypeName(),
						s.str(),
						H3D_FULL_LOCATION );
				}

				MFNode::onRemove( n );
			}
		};
#ifdef __BORLANDC__
    friend class CheckTheChildren;
#endif

		/// Make the check of the curve and call OpenGL routines to render it.
		void renderTrimmedCurve( GLUnurbsObj *nurbs_object );

    /// Constructor.
    Contour2D( Inst< SFNode >						 _metadata			 = 0,
               Inst< AddTheChildren		 > _addChildren		 = 0,
               Inst< RemoveTheChildren > _removeChildren = 0,
               Inst< CheckTheChildren	 > _children			 = 0 );

    /// Adds a node to the children field if it is the correct type of Node.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile Contour2D_addChildren.dot 
    auto_ptr< AddTheChildren >  addChildren;

		/// Removes a node from the children field if it is the correct type 
		/// of Node.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile Contour2D_removeChildren.dot
    auto_ptr< RemoveTheChildren > removeChildren;

    /// Contains the curves making up the contour.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Contour2D_children.dot
    auto_ptr< CheckTheChildren > children;

		/// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
