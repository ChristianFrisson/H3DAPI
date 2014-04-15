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
/// \file X3DNurbsSurfaceGeometryNode.h
/// \brief Header file for X3DNurbsSurfaceGeometryNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DNURBSSURFACEGEOMETRYNODE_H__
#define __X3DNURBSSURFACEGEOMETRYNODE_H__

#include <H3D/X3DParametricGeometryNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/X3DTextureCoordinateNode.h>
#include <H3D/NurbsTextureCoordinate.h>
#include <H3D/CoordBoundField.h>
#include <H3D/SFInt32.h>
#include <H3D/MFDouble.h>
#include <H3D/TextureCoordinateGenerator.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DNurbsSurfaceGeometryNode
  /// \brief The X3DNurbsSurfaceGeometryNode represents the abstract
  /// geometry type for all types of NURBS surfaces.
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
  /// \par Internal routes:
  /// \dotfile X3DNurbsSurfaceGeometryNode.dot
  class H3DAPI_API X3DNurbsSurfaceGeometryNode : 
    public X3DParametricGeometryNode {
  public:
    
    /// The SFCoordinateNode is dependent on the propertyChanged field of the 
    /// contained X3DCoordinateNode.
    typedef DependentSFNode< X3DCoordinateNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DCoordinateNode::propertyChanged > > 
    SFCoordinateNode;

    /// The SFTextureCoordinteNode is dependent on the propertyChanged 
    /// field of the contained X3DTextureCoordinateNode or 
    /// NurbsTextureCoordinate which both inherits from 
    /// X3DGeometricPropertyNode.
    typedef DependentSFNode< 
                X3DGeometricPropertyNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DGeometricPropertyNode::propertyChanged > > 
    TexCoordOrNurbsTexDependent;

    /// The bound field for X3DNurbsSurfaceGeometryNode is a CoordBoundField.
    typedef CoordBoundField SFBound;

    /// This field class takes as input SFNode field and checks whether
    /// this is one of the allowed textureNodes or not.
    class H3DAPI_API TexCoordOrNurbsTex: public TexCoordOrNurbsTexDependent {
    public:

      /// Destructor. Sets the value to NULL in order to get the correct
      /// onAdd and onRemove functions to be called and initialize the 
      /// different texture coord fields in the X3DNurbsSurfaceGeometryNode
      /// node when changing the textureNode.
      ~TexCoordOrNurbsTex() {
        value = NULL;
      }

      virtual void onAdd( Node *n ) {
        SFNode::onAdd( n );
        X3DTextureCoordinateNode *tex_coord_node = 
          dynamic_cast< X3DTextureCoordinateNode * >( n );

        NurbsTextureCoordinate *nurbs_tex_coord = 
          dynamic_cast< NurbsTextureCoordinate * >( n );

        if( !tex_coord_node && !nurbs_tex_coord ) {
          stringstream s;
          s << "Expecting X3DTextureCoordinateNode or NurbsTextureCoordinate";
          throw InvalidNodeType( n->getTypeName(),
            s.str(),
            H3D_FULL_LOCATION );
        }
      }
    };
#ifdef __BORLANDC__
    friend class TexCoordOrNurbsTex;
#endif

    /// Destructor
    ~X3DNurbsSurfaceGeometryNode() { 
      if( nurbs_object ) gluDeleteNurbsRenderer( nurbs_object );
    }

    /// Constructor.
    X3DNurbsSurfaceGeometryNode( Inst< SFNode      >  _metadata = 0,
                                 Inst< SFBound     > _bound = 0,
                                 Inst< DisplayList > _displayList = 0,
                                 Inst< MFBool      > _isTouched = 0,
                                 Inst< MFVec3f     > _force = 0,
                                 Inst< MFVec3f     > _contactPoint = 0,
                                 Inst< MFVec3f     > _contactNormal = 0,
                                 Inst< SFCoordinateNode > _controlPoint = 0,
                                 Inst< TexCoordOrNurbsTex > _texCoord = 0,
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

    /// Render the X3DNurbsSurfaceGeometryNode with OpenGL.
    virtual void render();

    /// Traverse the scenegraph. 
    virtual void traverseSG( TraverseInfo &ti );  

    /// The number of triangles renderered in this geometry, we don't know
    /// so return -1
    virtual int nrTriangles() {
      return -1;
    }

    /// Function called by render to render the small part that differs
    /// between NurbsPatchSurface and NurbsTrimmedSurface. The arguments
    /// are things that are needed to render that difference. TODO:
    /// make protected instead of public?
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
                                H3DInt32 &map2Vertex3Or4 ) {};
    
    /// controlPoint defines a set of control points of dimension 
    /// uDimension × vDimension. This set of points defines a mesh where the
    /// points do not have a uniform spacing. uDimension points define a 
    /// polyline in u-direction followed by further u-polylines with the 
    /// v-parameter in ascending order. The number of control points shall be
    /// equal or greater than the order. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_controlPoint.dot 
    auto_ptr< SFCoordinateNode >  controlPoint;

    /// texCoord provides additional information on how to generate texture
    /// coordinates. By default, texture coordinates in the unit square (or
    /// cube for 3D coordinates) are generated automatically from the 
    /// parametric subdivision. A NurbsTextureCoordinate node or simply a 
    /// TextureCoordinate node can then be used to compute a texture 
    /// coordinate given a u/v parameter of the NurbsSurface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_texCoord.dot 
    auto_ptr< TexCoordOrNurbsTex > texCoord;

    /// uTesselation provides a hint to the tessellation system of the
    /// nurbs surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_uTessellation.dot
    auto_ptr< SFInt32 > uTessellation;

    /// vTesselation provides a hint to the tessellation system of the
    /// nurbs surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_vTessellation.dot
    auto_ptr< SFInt32 > vTessellation;

    /// A weight value that shall be greater than zero is assigned to each
    /// controlPoint. The ordering of the values is equivalent to the 
    /// ordering of the control point values. The number of values shall
    /// be identical to the number of control points. If the length of the
    /// weight vector is 0, the default weight 1.0 is assumed for each control
    /// point, thus defining a non-Rational curve. If the number of weight 
    /// values is less than the number of control points, all weight values
    /// shall be ignored and a value of 1.0 shall be used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_weight.dot
    auto_ptr< MFDouble > weight;

    /// Specifies if both sides of the Sphere should be rendered or only the
    /// outside. Both are rendered if false, outside only it true.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_solid.dot
    auto_ptr< SFBool  >  solid;

    /// The uClosed defines whether the curve should be rendered as
    /// a closed object in the u direction.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_uClosed.dot
    auto_ptr< SFBool  >  uClosed;

    /// The vClosed defines whether the curve should be rendered as
    /// a closed object in the v direction.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_vClosed.dot
    auto_ptr< SFBool  >  vClosed;

    /// Defines the number of control points in the u dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_uDimension.dot
    auto_ptr< SFInt32  >  uDimension;

    /// Defines the number of control points in the v dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_vDimension.dot
    auto_ptr< SFInt32  >  vDimension;

    /// Defines the knot values of the surface in the u dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_uKnot.dot
    auto_ptr< MFDouble >  uKnot;

    /// Defines the knot values of the surface in the v dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_vKnot.dot
    auto_ptr< MFDouble >  vKnot;

    /// Defines the order of the surface in the u dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 3 \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_uOrder.dot
    auto_ptr< SFInt32  > uOrder;

    /// Defines the order of the surface in the v dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 3 \n
    /// 
    /// \dotfile X3DNurbsSurfaceGeometryNode_vOrder.dot
    auto_ptr< SFInt32  > vOrder;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  protected:
    GLUnurbsObj *nurbs_object;
    auto_ptr< Field > printWarning;
  };
}

#endif
