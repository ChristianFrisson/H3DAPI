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
/// \file ElevationGrid.h
/// \brief Header file for ElevationGrid, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ELEVATIONGRID_H__
#define __ELEVATIONGRID_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/TextureCoordinateGenerator.h>
#include <H3D/X3DColorNode.h>
#include <H3D/X3DNormalNode.h>
#include <H3D/X3DVertexAttributeNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/SFInt32.h>
#include <H3D/SFFloat.h>
#include <H3D/FogCoordinate.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ElevationGrid
  /// \brief The ElevationGrid node specifies a uniform rectangular grid of 
  /// varying height in the Y=0 plane of the local coordinate system. 
  ///
  /// The geometry is described by a scalar array of height values that
  /// specify the height of a surface above each point of the grid. 
  /// 
  /// The xDimension and zDimension fields indicate the number of elements
  /// of the grid height array in the X and Z directions. Both xDimension 
  /// and zDimension shall be greater than or equal to zero. If either the
  /// xDimension or the zDimension is less than two, the ElevationGrid
  /// contains no quadrilaterals. The vertex locations for the rectangles
  /// are defined by the height field and the xSpacing and zSpacing fields:
  /// - The height field is an xDimension by zDimension array of scalar values
  /// representing the height above the grid for each vertex.
  /// - The xSpacing and zSpacing fields indicate the distance between 
  /// vertices in the X and Z directions respectively, and shall be greater 
  /// than zero.
  ///
  /// The color field specifies per-vertex or per-quadrilateral colours for 
  /// the ElevationGrid node depending on the value of colorPerVertex. If the
  /// color field is NULL, the ElevationGrid node is rendered with the overall
  /// attributes of the Shape node enclosing the ElevationGrid node).
  ///
  /// The colorPerVertex field determines whether colours specified in the 
  /// color field are applied to each vertex or each quadrilateral of the
  /// ElevationGrid node. If colorPerVertex is FALSE and the color field is
  /// not NULL, the color field shall specify a Color node containing at least
  /// (xDimension-1)×(zDimension-1) colours; one for each quadrilateral.
  ///
  /// If colorPerVertex is TRUE and the color field is not NULL, the color 
  /// field shall specify a Color node containing at least 
  /// xDimension × zDimension colours, one for each vertex.
  ///
  /// The normal field specifies per-vertex or per-quadrilateral normals for
  /// the ElevationGrid node. If the normal field is NULL, normals are 
  /// automatically generated, using the creaseAngle field to determine if
  /// and how normals are smoothed across the surface(see Section 11.2.3 in 
  /// the X3D  specification).
  ///
  /// The normalPerVertex field determines whether normals are applied to
  /// each vertex or each quadrilateral of the ElevationGrid node depending
  /// on the value of normalPerVertex. If normalPerVertex is FALSE and the
  /// normal node is not NULL, the normal field shall specify a Normal node
  /// containing at least (xDimension-1)×(zDimension-1) normals; one for each
  /// quadrilateral
  ///
  /// If normalPerVertex is TRUE and the normal field is not NULL, the normal
  /// field shall specify a Normal node containing at least 
  /// xDimension × zDimension normals; one for each vertex.
  ///
  /// The texCoord field specifies per-vertex texture coordinates for the 
  /// ElevationGrid node. If texCoord is NULL, default texture coordinates
  /// are applied to the geometry. The default texture coordinates range
  /// from (0,0) at the first vertex to (1,1) at the last vertex. The S
  /// texture coordinate is aligned with the positive X-axis, and the T
  /// texture coordinate with positive Z-axis. If texCoord is not NULL,
  /// it shall specify a TextureCoordinate node containing at least 
  /// (xDimension)×(zDimension) texture coordinates; one for each vertex.
  ///
  /// By default, the quadrilaterals are defined with a counterclockwise 
  /// ordering. Hence, the Y-component of the normal is positive. Setting
  /// the ccw field to FALSE reverses the normal direction. Backface culling
  /// is enabled when the solid field is TRUE.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ElevationGrid.x3d">ElevationGrid.x3d</a>
  ///     ( <a href="examples/ElevationGrid.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/ElevationGrid1.x3d">ElevationGrid1.x3d</a>
  ///     ( <a href="examples/ElevationGrid1.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ElevationGrid.dot 
  class H3DAPI_API ElevationGrid : public X3DGeometryNode {
  public:
    /// The SFColorNode is dependent on the propertyChanged field of the 
    /// contained X3DColorNode.
    typedef  DependentSFNode< X3DColorNode,
                              FieldRef< X3DGeometricPropertyNode,
                                        Field,
                                        &X3DColorNode::propertyChanged > >  
    SFColorNode;
    
    /// The SFNormalNode is dependent on the propertyChanged field of the 
    /// contained X3DNormalNode.
    typedef DependentSFNode< X3DNormalNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DNormalNode::propertyChanged > > 
    SFNormalNode;

    /// The SFTextureCoordinteNode is dependent on the propertyChanged 
    /// field of the contained X3DTextureCoordinateNode.
    typedef DependentSFNode< 
                X3DTextureCoordinateNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DTextureCoordinateNode::propertyChanged > > 
    SFTextureCoordinateNode;   

    /// The MFVertexAttributeNode is dependent on the propertyChanged
    /// field of the contained X3DVertexAttributeNode.
    typedef DependentMFNode< 
                X3DVertexAttributeNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DVertexAttributeNode::propertyChanged > > 
    MFVertexAttributeNode;
    
    /// The SFFogCoordinate is dependent on the propertyChanged
    /// field of the contained FogCoordinate.
    typedef DependentSFNode< 
                FogCoordinate,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &FogCoordinate::propertyChanged > > 
    SFFogCoordinate;

    /// Specialized field for automatically generating normals from
    /// coordinates.
    /// The resulting Normal node will contain normals per face
    /// if normalPerVertex is false or creaseAngle <=0.
    /// if normalPerVertex is true, then the normals returned will be
    /// a normal per vertex in coord if creaseAngle >= pi and
    /// a normal per vertex in each face if 0 < creaseAngle < pi
    ///
    /// routes_in[0] is the normalPerVertex field.
    /// routes_in[1] is the xDimension field.
    /// routes_in[2] is the zDimenstion field.
    /// routes_in[3] is the xSpacing field.
    /// routes_in[4] is the zSpacing field.
    /// routes_in[5] is the height field.
    /// routes_in[6] is the ccw field.
    /// routes_in[7] is the creaseAngle field.
    class H3DAPI_API AutoNormal: 
      public TypedField< SFNormalNode,
                         Types< SFBool, 
                                SFInt32, 
                                SFInt32, 
                                SFFloat,
                                SFFloat,
                                MFFloat,
                                SFBool,
                                SFFloat > > {
      virtual void update();

      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each vertex in each face, i.e.
      /// the number of normals will be the number of faces *4
      virtual X3DNormalNode *generateNormalsPerVertex( 
                               H3DInt32 x_dim,
                               H3DInt32 z_dim,
                               H3DFloat x_spacing,
                               H3DFloat z_spacing,
                               const vector< H3DFloat > &height,
                               bool ccw,
                               H3DFloat crease_angle );
    
      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each vertex. The normal for each
      /// vertex will be the average of the normal of all faces using
      /// that vertex.
       virtual X3DNormalNode *generateNormalsPerVertex( 
                               H3DInt32 x_dim,
                               H3DInt32 z_dim,
                               H3DFloat x_spacing,
                               H3DFloat z_spacing,
                               const vector< H3DFloat > &height,
                               bool ccw );

      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each face specified.
      virtual X3DNormalNode *generateNormalsPerFace(  
                               H3DInt32 x_dim,
                               H3DInt32 z_dim,
                               H3DFloat x_spacing,
                               H3DFloat z_spacing,
                               const vector< H3DFloat > &height,
                               bool ccw );

    };

    /// SFBound is specialized update itself from the fields of the
    /// ElevationGrid
    ///
    /// routes_in[0] is the xDimension field of the ElevationGrid node.  
    /// routes_in[1] is the zDimension field of the ElevationGrid node.  
    /// routes_in[2] is the xSpacing field of the ElevationGrid node.  
    /// routes_in[3] is the zSpacing field of the ElevationGrid node.  
    /// routes_in[4] is the height field of the ElevationGrid node.  
    ///
    class H3DAPI_API SFBound: public TypedField< X3DGeometryNode::SFBound,
                                      Types< SFInt32, SFInt32, 
                                             SFFloat, SFFloat, MFFloat > >{
      virtual void update();
    };

    /// Display list is extended in order to set front sidedness of 
    /// triangles outside the display list. This is due to that the 
    /// front face value depends the previous value of front face. If we did 
    /// not do this and the display list was used in several places where
    /// the front face differs there would be normal errors. The drawback with
    /// having it outside the display list is that you cannot call the render()
    /// function to render the geometry correctly, but MUST use the callList()
    /// function in DisplayList.
    class H3DAPI_API DisplayList: public X3DGeometryNode::DisplayList {
    protected:
      /// Perform front face code outside the display list.
      virtual void callList( bool build_list );
    };

    /// Start texture coordinate generation. Texture coordinates will be
    /// generated for all texture units used by the currently active texture.
    /// If tex_coord_gen is NULL then texture coordinates will be generated 
    /// from the bounding box of the geometry as defined in the X3D 
    /// specification. If tex_coord_gen is not NULL then we use the 
    /// TextureCoordinateGenerator to define the texture coordinate generation.
    virtual void startTexGen( TextureCoordinateGenerator *tex_coord_gen );

    /// Stop texture coordinate generation.
    virtual void stopTexGen( TextureCoordinateGenerator *tex_coord_gen );

    /// Render the ElevationGrid with OpenGL.
    virtual void render();
    
    /// The number of triangles renderered in this geometry.
    virtual int nrTriangles() {
      H3DInt32 x_dim = xDimension->getValue();
      H3DInt32 z_dim = zDimension->getValue();
      if( x_dim < 2 || z_dim < 2 ) return 0;
      else return (x_dim - 1) * (z_dim - 1 ) * 2;
    }

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    /// Constructor.
    ElevationGrid( Inst< SFNode           > _metadata        = 0,
                   Inst< SFBound          > _bound           = 0,
                   Inst< DisplayList      > _displayList     = 0,
                   Inst< SFColorNode      > _color           = 0,
                   Inst< SFNormalNode     > _normal          = 0,
                   Inst< SFTextureCoordinateNode > _texCoord = 0,
                   Inst< SFBool           > _ccw             = 0,
                   Inst< SFBool           > _colorPerVertex  = 0,
                   Inst< SFBool           > _normalPerVertex = 0,
                   Inst< SFBool           > _solid           = 0,
                   Inst< MFVertexAttributeNode > _attrib     = 0,
                   Inst< AutoNormal       > _autoNormal      = 0,
                   Inst< SFFloat          > _creaseAngle     = 0,
                   Inst< SFInt32          > _xDimension      = 0,
                   Inst< SFInt32          > _zDimension      = 0,
                   Inst< SFFloat          > _xSpacing        = 0,
                   Inst< SFFloat          > _zSpacing        = 0,
                   Inst< MFFloat          > _height          = 0,
                   Inst< SFFogCoordinate  > _fogCoord        = 0 );

    ~ElevationGrid();

    /// Contains an X3DColorNode whose colors are applied to the
    /// ElevationGrid. If the color field is NULL, the
    /// geometry shall be rendered normally using the Material
    /// and texture defined in the Appearance node
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile ElevationGrid_color.dot 
    auto_ptr< SFColorNode >  color;

    /// Contains an X3DNormalNode which colors are applied to the
    /// ElevationGrid. If the normal field is NULL, the
    /// normals are automatically generated in accordance with the
    /// node's definition. If the node does not define a behaviour,
    /// the default is to generate an averaged normal for all faces
    /// that share that vertex.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile ElevationGrid_normal.dot 
    auto_ptr< SFNormalNode >  normal;
    
    /// Contains an X3DTextureCoordinateNode which colors are applied
    /// to the ElevationGrid.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile ElevationGrid_texCoord.dot 
    auto_ptr< SFTextureCoordinateNode >  texCoord;
    
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
    /// \dotfile ElevationGrid_ccw.dot 
    auto_ptr< SFBool >  ccw;
    
    /// If TRUE the colors in the color field is applied per vertex,
    /// otherwise it is applied per face.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile ElevationGrid_colorPerVertex.dot 
    auto_ptr< SFBool >  colorPerVertex;

    /// If TRUE the normals in the normal field is applied per vertex,
    /// otherwise it is applied per face.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile ElevationGrid_normalPerVertex.dot 
    auto_ptr< SFBool >  normalPerVertex;

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
    /// \dotfile ElevationGrid_solid.dot 
    auto_ptr< SFBool >  solid;
    
    /// If the attrib field is not empty it shall contain a list of
    /// X3DVertexAttributeNode instances with per-vertex attribute
    /// information for programmable shaders.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile ElevationGrid_attrib.dot 
    auto_ptr< MFVertexAttributeNode > attrib;

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
    /// \dotfile ElevationGrid_creaseAngle.dot 
    auto_ptr< SFFloat > creaseAngle;

    /// The number of elements in the grid height array in the x direction.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-inf]
    ///
    /// \dotfile ElevationGrid_xDimension.dot 
    auto_ptr< SFInt32 > xDimension;

    /// The number of elements in the grid height array in the z direction.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-inf]
    ///
    /// \dotfile ElevationGrid_zDimension.dot 
    auto_ptr< SFInt32 > zDimension;
  
    /// The distance between vertices in the x direction.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    /// <b>Valid range:</b> (0-inf]
    ///
    /// \dotfile ElevationGrid_xSpacing.dot 
    auto_ptr< SFFloat > xSpacing;

    /// The distance between vertices in the z direction.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    /// <b>Valid range:</b> (0-inf]
    ///
    /// \dotfile ElevationGrid_zSpacing.dot 
    auto_ptr< SFFloat > zSpacing;

    /// The height field is an xDimension by zDimension array of scalar
    /// values representing the height above the grid for each vertex.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    ///
    /// \dotfile ElevationGrid_height.dot 
    auto_ptr< MFFloat > height;

    /// Auto-generated normals that are used if the normal field is NULL.
    /// Only accessable in C++.
    auto_ptr< AutoNormal  >  autoNormal;

    /// If the fogCoord field is not empty, it shall contain a list 
    /// of per-vertex depth values for calculating fog depth.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile ElevationGrid_fogCoord.dot 
    auto_ptr< SFFogCoordinate > fogCoord;


    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    // Internal field used to know if vertex buffer object can be created.
    auto_ptr< Field > vboFieldsUpToDate;
    // The index for the vertex buffer object
    GLuint *vbo_id;
  };
}

#endif
