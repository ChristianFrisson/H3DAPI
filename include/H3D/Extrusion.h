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
/// \file Extrusion.h
/// \brief Header file for Extrusion, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __EXTRUSION_H__
#define __EXTRUSION_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFFloat.h>
#include <H3D/MFRotation.h>
#include <H3D/MFVec2f.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/Normal.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Extrusion
  /// \brief The Extrusion node specifies geometric shapes based on a two
  /// dimensional cross-section extruded along a three dimensional spine 
  /// in the local coordinate system. The cross-section can be scaled and 
  /// rotated at each spine point to produce a wide variety of shapes.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Extrusion.x3d">Extrusion.x3d</a>
  ///     ( <a href="examples/Extrusion.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Extrusion.dot 
  /// \todo Implement vertex arrays in no-normal-per-face cases.
  class H3DAPI_API Extrusion : public X3DGeometryNode {
  public:

    /// The SFNormalNode is dependent on the propertyChanged field of the 
    /// contained Normal.
    typedef DependentSFNode< Normal,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DNormalNode::propertyChanged > > 
    SFNormalNode;

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
    class H3DAPI_API VertexVector: 
      public TypedField< MFVec3f,
                         Types< MFVec2f, 
                                MFRotation, 
                                MFVec2f, 
                                MFVec3f > > {
      virtual void update();
    };
#ifdef __BORLANDC__
    friend class VertexVector;
#endif

    /// Specialized field for automatically generating normals from
    /// coordinates.
    /// The resulting Normal node will contain normals per face if
    /// creaseAngle <=0. If creaseAngle >= pi, then the normals returned will
    /// be a normal per vertex in vertexVector and finally the normals returned
    /// will contain a normal per vertex in each face if 0 < creaseAngle < pi.
    ///
    /// routes_in[0] is the vertexVector field.
    /// routes_in[1] is the creaseAngle field.
    class H3DAPI_API AutoNormal: 
      public TypedField< SFNormalNode,
                         Types< VertexVector,
                                SFFloat,
                                SFBool,
                                SFBool > > {
      virtual void update();

      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each vertex in each face, i.e.
      /// the number of normals will be the number of faces *4
      virtual Normal *generateNormalsPerVertex(
                      const vector < Vec3f > &vertex_vector,
                      const vector < Vec2f > &cross_section,
                      vector < Vec3f > &yAxis,
                      bool ccw_value,
                      H3DInt32 nr_of_cross_section_points,
                      H3DInt32 spine_size,
                      bool closedSpine,
                      H3DFloat crease_angle,
                      bool begin_cap,
                      bool end_cap );
    
      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each vertex. The normal for each
      /// vertex will be the average of the normal of all faces using
      /// that vertex.
      virtual Normal *generateNormalsPerVertex(
                      const vector < Vec3f > &vertex_vector,
                      const vector < Vec2f > &cross_section,
                      vector < Vec3f > &yAxis,
                      bool ccw_value,
                      H3DInt32 nr_of_cross_section_points,
                      H3DInt32 spine_size,
                      bool closedSpine,
                      bool begin_cap,
                      bool end_cap );

      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each face specified.
      virtual Normal *generateNormalsPerFace(
                      const vector < Vec3f > &vertex_vector,
                      const vector < Vec2f > &cross_section,
                      vector < Vec3f > &yAxis,
                      bool ccw_value,
                      H3DInt32 nr_of_cross_section_points,
                      H3DInt32 spine_size,
                      bool closedCrossSection,
                      bool begin_cap,
                      bool end_cap );
    };


    /// Render the Extrusion with OpenGL.
    virtual void render();

    /// An upper bound on the number of triangles.
    virtual int nrTriangles() {
      return( crossSection->size() * 2 + 
              crossSection->size() * spine->size() * 2 );
    }

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    /// Constructor.
    Extrusion( Inst< SFNode           > _metadata        = 0,
               Inst< SFBound          > _bound           = 0,
               Inst< DisplayList      > _displayList     = 0,
               Inst< SFBool           > _beginCap         = 0,
               Inst< SFBool           > _ccw             = 0,
               Inst< SFBool           > _convex           = 0,
               Inst< SFFloat          > _creaseAngle     = 0,
               Inst< MFVec2f          > _crossSection    = 0,
               Inst< SFBool           > _endCap           = 0,
               Inst< MFRotation       > _orientation     = 0,
               Inst< MFVec2f          > _scale           = 0,
               Inst< SFBool           > _solid           = 0,
               Inst< MFVec3f          > _spine           = 0 );

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
    auto_ptr< SFBool > beginCap;

  
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
    auto_ptr< SFBool > ccw;
    
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
    auto_ptr< SFBool > convex;

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
    auto_ptr< MFVec2f > crossSection;

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
    auto_ptr< SFBool > endCap;

    /// A list of 3D orientation parameters used to orient the crossSection.
    /// If the number of orientation values is greater than the number
    /// of spine points, the excess values are ignored. If it contains one 
    /// value, it is applied at all spine points. The results are undefined if
    /// the number of orientation values is greater than one but less
    /// than the number of spine points.
    /// The final orientation of each cross-section is computed by first
    /// orienting it relative to the spine segments on either side of point at
    /// which the cross-section is placed. This is known as the spine-aligned
    /// cross-section plane (SCP), and is designed to provide a smooth
    /// transition from one spine segment to the next. The SCP
    /// is then rotated by the corresponding orientation value. This rotation is
    /// performed relative to the SCP. For example, to impart twist in the
    /// cross-section, a rotation about the Y-axis (0 1 0) would be used. Other
    /// orientations are valid and rotate the cross-section out of the SCP.
    /// The unmodified SCP for point is has its y-axis in the direction of
    /// the spine point i-1 to i+1. The x and z axis are perpendicular to
    /// this direction.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 1 0 \n
    /// 
    /// \dotfile Extrusion_orientation.dot 
    auto_ptr< MFRotation > orientation;

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
    auto_ptr< MFVec2f > scale;

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
    auto_ptr< SFBool > solid;

    /// A 3D spine piecewise linear curve 
    /// (also described as a series of connected vertices).
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [0 0 0 0 1 0] \n
    /// 
    /// \dotfile Extrusion_spine.dot 
    auto_ptr< MFVec3f > spine;

    /// The vector of vertices constructing the extrusion-shape
    /// Only accessable in C++.
    auto_ptr< VertexVector > vertexVector;

    /// Auto-generated normals.
    /// Only accessable in C++.
    auto_ptr< AutoNormal > autoNormal;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    // Texture coordinates for u, v direction of the body.
    vector< H3DFloat > u_tex_coord;
    vector< H3DFloat > v_tex_coord;
    // Texture coordinates for the caps.
    vector< Vec3f > caps_tex_coord;
  };
}

#endif
