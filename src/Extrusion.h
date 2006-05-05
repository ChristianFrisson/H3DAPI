//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Extrusion.h
/// \brief Header file for Extrusion, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __EXTRUSION_H__
#define __EXTRUSION_H__

#include "X3DGeometryNode.h"
#include "SFInt32.h"
#include "SFFloat.h"
#include "MFRotation.h"
#include "MFVec2f.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Extrusion
  /// \brief The Extrusion node specifies geometric shapes based on a two
	/// dimensional cross-section extruded along a three dimensional spine 
	/// in the local coordinate system. The cross-section can be scaled and 
	/// rotated at each spine point to produce a wide variety of shapes.
  /// \par Internal routes:
  /// \dotfile Extrusion.dot 
  class H3DAPI_API Extrusion : public X3DGeometryNode {
	protected:
		
		/// returns true if the two points are coincident
		template <typename T>
		inline bool coinc(T a , T b) 
					{ return H3DAbs( ( a - b ).lengthSqr()) < Constants::f_epsilon; }
		
		/// calculate the normal to a face given three vertices in the face.
		inline Vec3f calculateNormal( const vector < Vec3f > &vertex_vector,
																	H3DInt32 right,
																	H3DInt32 middle,
																	H3DInt32 left) {
			return ( vertex_vector[ right ] - vertex_vector[ middle ] ).crossProduct(
							 vertex_vector[ left ] - vertex_vector[ middle ] ); }

		/// find the surrounding faces to the face defined by i and j.
		vector< H3DInt32 > findSurroundingFaces( 
												H3DInt32 i,
												H3DInt32 j,
												bool closedSpine,
												H3DInt32 nrOfSpinePoints,
												bool closedCrossSection,
												H3DInt32 nrOfCrossSectionPoints);

		/// Create a vector from the arguments given
		/// with one normal for each vertex in each face, i.e.
		/// the number of normals will be the number of facess *4
		void generateNormalsPerVertex( 
                      vector < Vec3f > &normalVector,
											const vector < Vec3f > &vertex_vector,
											const vector < Vec2f > &cross_section,
											vector < Vec3f > &yAxis,
											bool ccwcheck,
											H3DInt32 nrOfCrossSectionPoints,
											H3DInt32 nrOfSpinePoints,
											bool closedSpine,
											H3DFloat crease_angle,
											H3DInt32 &ifCapsAdd );
		
		/// Create a vector from the arguments given
		/// with one normal for each vertex. The normal for each
		/// vertex will be the average of the normal of all faces using
		/// that vertex.
    void generateNormalsPerVertex( 
                      vector < Vec3f > &normalVector,
											const vector < Vec3f > &vertex_vector,
											const vector < Vec2f > &cross_section,
											vector < Vec3f > &yAxis,
											bool ccwcheck,
											H3DInt32 nrOfCrossSectionPoints,
											H3DInt32 nrOfSpinePoints,
											bool closedSpine,
											H3DInt32 &ifCapsAdd );

		/// Create a vector from the arguments given
		/// with one normal for each face specified.
		vector< Vec3f > generateNormalsPerFace(  
											const vector < Vec3f > &vertex_vector,
											const vector < Vec2f > &cross_section,
											vector < Vec3f > &yAxis,
											bool ccwcheck,
											H3DInt32 nrOfCrossSectionPoints,
											H3DInt32 nrOfSpinePoints,
											bool closedCrossSection,
											H3DInt32 &ifCapsAdd );

  public:

		/// SFBound is specialized update itself from the fields of the
    /// Extrusion
    ///
    /// routes_in[0] is the vertexVector field of the Extrusion Node
    ///
    class H3DAPI_API SFBound: public TypedField< X3DGeometryNode::SFBound,
                                      Types< MFVec3f > >{
      virtual void update();
    };

		/// Specialized field vertex coordinates from the fields affecting this,
    /// the resulting vertexVector will be used both in render and in bound.
		///
    /// routes_in[0] is the crossSection field.
    /// routes_in[1] is the orientation field.
    /// routes_in[2] is the scale field.
    /// routes_in[3] is the spine field.
    class H3DAPI_API VertexVectors: 
      public TypedField< MFVec3f,
                         Types< MFVec2f, 
                                MFRotation, 
                                MFVec2f, 
                                MFVec3f > > {
      virtual void update();
		};

    /// Render the Extrusion with OpenGL.
    virtual void render();

		/// Traverse the scenegraph. A HLFeedbackShape is added for haptic
    /// rendering if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );  

    /// Constructor.
    Extrusion( Inst< SFNode           > _metadata        = 0,
               Inst< SFBound          > _bound           = 0,
               Inst< DisplayList      > _displayList     = 0,
               Inst< SFBool           > _beginCap				 = 0,
							 Inst< SFBool           > _ccw             = 0,
               Inst< SFBool           > _convex					 = 0,
               Inst< SFFloat          > _creaseAngle     = 0,
               Inst< MFVec2f          > _crossSection    = 0,
               Inst< SFBool						> _endCap	         = 0,
               Inst< MFRotation       > _orientation     = 0,
               Inst< MFVec2f          > _scale	         = 0,
               Inst< SFBool           > _solid           = 0,
							 Inst< MFVec3f          > _spine           = 0,
							 Inst< VertexVectors    > _vertexVector    = 0 );

    /// When the beginCap or endCap fields are specified as TRUE, planar cap
		/// surfaces will be generated regardless of whether the crossSection is
		/// a closed curve. If crossSection is not a closed curve, the caps are 
		/// generated by adding a final point to crossSection that is equal to the
		/// initial point. An open surface can still have a cap, resulting 
		/// (for a simple case) in a shape analogous to a soda can sliced in half 
		/// vertically. These surfaces are generated even if spine is also 
		/// a closed curve. If a field value is FALSE, the corresponding cap is 
		/// not generated.
		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_beginCap.dot 
    auto_ptr< SFBool >  beginCap;

	
		/// The ccw field defines the ordering of the vertex coordinates of 
    /// the geometry with respect to user-given or automatically generated
    /// normal vectors used in the lighting model equations. If ccw is TRUE,
    /// the normals shall follow the right hand rule; the orientation of
    /// each normal with respect to the vertices (taken in order) shall be
    /// such that the vertices appear to be oriented in a counterclockwise
    /// order when the vertices are viewed (in the local coordinate system
    /// of the Shape) from the opposite direction as the normal. If ccw is
    /// FALSE, the normals shall be oriented in the opposite direction. If
    /// normals are not generated but are supplied using a Normal node, and
    /// the orientation of the normals does not match the setting of the 
    /// ccw field, results are undefined.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_ccw.dot 
    auto_ptr< SFBool >  ccw;
    
		/// The convex field indicates whether all polygons in the shape are 
		/// convex (TRUE). A polygon is convex if it is planar, does not intersect
		/// itself, and all of the interior angles at its vertices are less than 
		/// 180 degrees. Non planar and self intersecting polygons may produce 
		/// undefined results even if the convex field is FALSE.
		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_convex.dot 
    auto_ptr< SFBool >  convex;

		/// The creaseAngle field affects how default normals are generated. 
    /// If the angle between the geometric normals of two adjacent faces is
    /// less than the crease angle, normals shall be calculated so that the
    /// faces are shaded smoothly across the edge; otherwise, normals shall
    /// be calculated so that a lighting discontinuity across the edge is 
    /// produced.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-inf]
    ///
    /// \dotfile Extrusion_creaseAngle.dot 
    auto_ptr< SFFloat > creaseAngle;

		/// A 2D crossSection piecewise linear curve 
		/// (described as a series of connected vertices).
		/// The cross-section can be scaled and rotated at each spine point
		/// to produce a wide variety of shapes.
		/// 
    /// <b>Access type:</b> inputOutput \n
		/// <b>Default value:</b> [1 1 1 -1 -1 -1 -1 1 1 1] \n
    /// 
    /// \dotfile Extrusion_crossSection.dot 
    auto_ptr< MFVec2f >  crossSection;

		/// When the beginCap or endCap fields are specified as TRUE, planar cap
		/// surfaces will be generated regardless of whether the crossSection is
		/// a closed curve. If crossSection is not a closed curve, the caps are 
		/// generated by adding a final point to crossSection that is equal to the
		/// initial point. An open surface can still have a cap, resulting 
		/// (for a simple case) in a shape analogous to a soda can sliced in half 
		/// vertically. These surfaces are generated even if spine is also 
		/// a closed curve. If a field value is FALSE, the corresponding cap is 
		/// not generated.
		/// 
		/// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_endCap.dot 
    auto_ptr< SFBool >  endCap;

		/// A list of 3D orientation parameters used to orient the crossSection.
		/// If the number of orientation values is greater than the number
		/// of spine points, the excess values are ignored. If it contains one 
		/// value, it is applied at all spine points. The results are undefined if
		/// the number of orientation values is greater than one but less 
		/// than the number of spine points. 
		/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 1 0 \n
    /// 
    /// \dotfile Extrusion_orientation.dot 
    auto_ptr< MFRotation >  orientation;

		/// A list of 2D scale parameters used to scale the crossSection.
		/// If the number of scale values is greater than the number
		/// of spine points, the excess values are ignored. If it contains one 
		/// value, it is applied at all spine points. The results are undefined if
		/// the number of scale values is greater than one but less 
		/// than the number of spine points. 
		///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 1 \n
    /// 
    /// \dotfile Extrusion_scale.dot 
    auto_ptr< MFVec2f >  scale;

    /// The solid field determines whether one or both sides of each polygon
    /// shall be displayed. If solid is FALSE, each polygon shall be visible
    /// regardless of the viewing direction (i.e., no backface culling shall
    /// be done, and two sided lighting shall be performed to illuminate both
    /// sides of lit surfaces). If solid is TRUE, back face culling will be 
    /// performed.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile Extrusion_solid.dot 
    auto_ptr< SFBool >  solid;

		/// A 3D spine piecewise linear curve 
		/// (also described as a series of connected vertices).
		/// 
    /// <b>Access type:</b> inputOutput \n
		/// <b>Default value:</b> [0 0 0 0 1 0] \n
    /// 
    /// \dotfile Extrusion_spine.dot 
    auto_ptr< MFVec3f >  spine;

		/// The vector of vertices constructing the extrusion-shape
    /// Only accessable in C++.
    ///
    /// \dotfile Extrusion_vertexVector.dot 
    auto_ptr< VertexVectors  >  vertexVector;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
