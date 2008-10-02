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
/// \file IndexedFaceSet.h
/// \brief Header file for IndexedFaceSet, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __INDEXEDFACESET_H__
#define __INDEXEDFACESET_H__

#include <H3D/X3DComposedGeometryNode.h>
#include <H3D/CoordBoundField.h>
#include <H3D/MFInt32.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class IndexedFaceSet
  /// \brief The IndexedFaceSet node represents a 3D shape formed by constructing
  /// faces (polygons) from vertices listed in the coord field. 
  ///
  /// \anchor IndexedFaceSet_details
  /// The coord field contains a Coordinate node that defines the 3D
  /// vertices referenced by the coordIndex field. IndexedFaceSet uses the
  /// indices in its coordIndex field to specify the polygonal faces by
  /// indexing into the coordinates in the Coordinate node. An index of "-1"
  /// indicates that the current face has ended and the next one begins. The
  /// last face may be (but does not have to be) followed by a "-1" index. If
  /// the greatest index in the coordIndex field is N, the Coordinate node
  /// shall contain N+1 coordinates (indexed as 0 to N). Each face of the
  /// IndexedFaceSet shall have:
  ///
  /// - at least three non-coincident vertices;
  /// - vertices that define a planar polygon;
  /// - vertices that define a non-self-intersecting polygon.
  ///
  /// Otherwise, The results are undefined.
  ///
  /// The IndexedFaceSet node is specified in the local coordinate system
  /// and is affected by the transformations of its ancestors. Descriptions
  /// of the coord, normal, and texCoord fields are provided in the
  /// Coordinate, Normal, and TextureCoordinate nodes, respectively.
  /// Details on lighting equations and the interaction between color field,
  /// normal field, textures, materials, and geometries are provided in 11
  /// Rendering component and 12 Shape component.
  ///
  /// If the color field is not NULL, it shall contain a Color node whose
  /// colours are applied to the vertices or faces of the IndexedFaceSet as
  /// follows:
  ///
  /// <ul>
  /// <li>
  /// If colorPerVertex is FALSE, colours are applied to each face, as
  /// follows:
  /// <ol> <li>
  ///    If the colorIndex field is not empty, one colour is used for each face
  ///    of the IndexedFaceSet. There shall be at least as many indices in
  ///    the colorIndex field as there are faces in the IndexedFaceSet. If
  ///    the greatest index in the colorIndex field is N, then there shall be
  ///    N+1 colours in the Color node. The colorIndex field shall not contain
  ///    any negative entries.
  /// <li>
  ///    If the colorIndex field is empty, the colours in the Color node are
  ///    applied to each face of the IndexedFaceSet in order. There shall be at
  ///    least as many colours in the Color node as there are faces.
  /// </ol>
  ///
  /// <li>
  /// If colorPerVertex is TRUE, colours are applied to each vertex, as
  /// follows:
  /// <ol>
  /// <li>
  ///    If the colorIndex field is not empty, colours are applied to each
  ///    vertex of the IndexedFaceSet in exactly the same manner that the
  ///    coordIndex field is used to choose coordinates for each vertex from
  ///    the Coordinate node. The colorIndex field shall contain at least as
  ///    many indices as the coordIndex field, and shall contain end-of-face
  ///    markers (1) in exactly the same places as the coordIndex field. If the
  ///    greatest index in the colorIndex field is N, then there shall be N+1
  ///    colours in the Color node.
  /// <li>
  ///    If the colorIndex field is empty, the coordIndex field is used to
  ///    choose colours from the Color node. If the greatest index in the
  ///    coordIndex field is N, then there shall be N+1 colours in the Color
  ///    node.
  /// </ol></ul>
  /// If the color field is NULL, the geometry shall be rendered normally
  /// using the Material and texture defined in the Appearance node.
  ///
  /// If the normal field is not NULL, it shall contain a Normal node whose
  /// normals are applied to the vertices or faces of the IndexedFaceSet in
  /// a manner exactly equivalent to that described above for applying
  /// colours to vertices/faces (where normalPerVertex corresponds to
  /// colorPerVertex and normalIndex corresponds to colorIndex). If the
  /// normal field is NULL, the browser shall automatically generate
  /// normals, using creaseAngle to determine if and how normals are
  /// smoothed across shared vertices (see Section 11.2.3 in the X3D 
  /// specification).
  /// 
  /// If the texCoord field is not NULL, it shall contain a
  /// TextureCoordinate node. The texture coordinates in that node are
  /// applied to the vertices of the IndexedFaceSet as follows:
  ///
  /// -  If the texCoordIndex field is not empty, then it is used to choose
  ///    texture coordinates for each vertex of the IndexedFaceSet in exactly
  ///    the same manner that the coordIndex field is used to choose
  ///    coordinates for each vertex from the Coordinate node. The
  ///    texCoordIndex field shall contain at least as many indices as the
  ///    coordIndex field, and shall contain end-of-face markers (1) in exactly
  ///    the same places as the coordIndex field. If the greatest index in the
  ///    texCoordIndex field is N, then there shall be N+1 texture coordinates
  ///    in the TextureCoordinate node.
  /// 
  /// -  If the texCoordIndex field is empty, then the coordIndex array is used
  ///    to choose texture coordinates from the TextureCoordinate node. If the
  ///    greatest index in the coordIndex field is N, then there shall be N+1
  ///    texture coordinates in the TextureCoordinate node.
  /// 
  /// If the texCoord field is NULL, a default texture coordinate mapping is
  /// calculated using the local coordinate system bounding box of the
  /// shape. The longest dimension of the bounding box defines the S
  /// coordinates, and the next longest defines the T coordinates. If two or
  /// all three dimensions of the bounding box are equal, ties shall be
  /// broken by choosing the X, Y, or Z dimension in that order of
  /// preference. The value of the S coordinate ranges from 0 to 1, from one
  /// end of the bounding box to the other. The T coordinate ranges between
  /// 0 and the ratio of the second greatest dimension of the bounding box
  /// to the greatest dimension and the R coordinate ranges between 0 and the
  /// ratio of the smallest dimension of the bounding box to the greatest
  /// dimension.
  ///
  /// \par Internal routes:
  /// \dotfile IndexedFaceSet.dot
  class H3DAPI_API IndexedFaceSet : 
    public X3DComposedGeometryNode {
  public:
    H3D_VALUE_EXCEPTION( int, InvalidTexCoordIndex );
    H3D_VALUE_EXCEPTION( int, InvalidNormalIndex );
    H3D_VALUE_EXCEPTION( int, InvalidColorIndex );

    /// Specialized field for automatically generating normals from
    /// coordinates.
    /// The resulting Normal node will contain normals per face
    /// if normalPerVertex is false or creaseAngle <=0.
    /// if normalPerVertex is true, then the normals returned will be
    /// a normal per vertex in coord if creaseAngle >= pi and
    /// a normal per vertex specified with coordIndex if 0 < creaseAngle < pi
    ///
    /// routes_in[0] is the normalPerVertex field.
    /// routes_in[1] is the coord field.
    /// routes_in[2] is the coordIndex field.
    /// routes_in[3] is the ccw field.
    /// routes_in[4] is the creaseAngle field.
    
    class H3DAPI_API AutoNormal: 
      public TypedField< SFNormalNode,
                         Types< SFBool, 
                                SFCoordinateNode, 
                                MFInt32, 
                                SFBool, 
                                SFFloat > > {
      virtual void update();

      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each vertex specified by coord_index, i.e.
      /// the number of normals will be the number of non-(-1) values
      /// in coord_index.
      ///
      /// \param coord Node with the coordinates.
      /// \param coord_index The indices in coord for the vertices.
      /// \param ccw Defines the ordering of the vertex coordinates of the 
      /// geometry with respect to generated normal vectors used in the 
      /// lighting model equations. If ccw is TRUE, the normals shall 
      /// follow the right hand rule; the orientation of each normal with
      /// respect to the vertices (taken in order) shall be such that the
      /// vertices appear to be oriented in a counterclockwise order when 
      /// the vertices are viewed (in the local coordinate system of the Shape)
      /// from the opposite direction as the normal.
      /// \param crease_angle If the angle between the geometric normals of two 
      /// adjacent faces is less than the crease angle, normals are 
      /// calculated so that the faces are shaded smoothly across the edge;
      /// otherwise, normals are calculated so that a lighting discontinuity
      /// across the edge is produced. 
      /// \returns A new Normal node with a normal for each
      /// vertex.
      ///
      virtual X3DNormalNode *generateNormalsPerVertex( 
                                    X3DCoordinateNode *coord,
                                    const vector< int > &coord_index,
                                    bool ccw,
                                    H3DFloat crease_angle );
    
      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each vertex in coord. The normal for each
      /// vertex will be the average of the normal of all faces using
      /// that vertex.
      ///
      /// \param coord Node with the coordinates.
      /// \param coord_index The indices in coord for the vertices.
      /// \param ccw Defines the ordering of the vertex coordinates of the 
      /// geometry with respect to generated normal vectors used in the 
      /// lighting model equations. If ccw is TRUE, the normals shall 
      /// follow the right hand rule; the orientation of each normal with
      /// respect to the vertices (taken in order) shall be such that the
      /// vertices appear to be oriented in a counterclockwise order when 
      /// the vertices are viewed (in the local coordinate system of the Shape)
      /// from the opposite direction as the normal.
       virtual X3DNormalNode *generateNormalsPerVertex( 
                                    X3DCoordinateNode *coord,
                                    const vector< int > &coord_index,
                                    bool ccw );

      /// Create a new X3DNormalNode from the arguments given
      /// with one normal for each face specified.
      ///
      /// \param coord Node with the coordinates.
      /// \param coord_index The indices in coord for the vertices.
      /// \param ccw Defines the ordering of the vertex coordinates of the 
      /// geometry with respect to generated normal vectors used in the 
      /// lighting model equations. If ccw is TRUE, the normals shall 
      /// follow the right hand rule; the orientation of each normal with
      /// respect to the vertices (taken in order) shall be such that the
      /// vertices appear to be oriented in a counterclockwise order when 
      /// the vertices are viewed (in the local coordinate system of the Shape)
      /// from the opposite direction as the normal.
      /// \returns A new Normal node with a normal for each
      /// vertex.
      ///
      virtual X3DNormalNode *generateNormalsPerFace( 
                                    X3DCoordinateNode *coord,
                                    const vector< int > &coord_index,
                                    bool ccw );

    };

    /// The bound field for IndexedFaceSet is a CoordBoundField.
    typedef CoordBoundField SFBound;

    /// Constructor.       
    IndexedFaceSet( Inst< SFNode           > _metadata        = 0,
                    Inst< SFBound          > _bound           = 0,
                    Inst< DisplayList      > _displayList     = 0,
                    Inst< SFColorNode      > _color           = 0,
                    Inst< SFCoordinateNode >  _coord          = 0,
                    Inst< SFNormalNode     > _normal          = 0,
                    Inst< SFTextureCoordinateNode >  _texCoord = 0,
                    Inst< SFBool       > _ccw                = 0,
                    Inst< SFBool       > _colorPerVertex     = 0,
                    Inst< SFBool       >  _normalPerVertex   = 0,
                    Inst< SFBool       > _solid              = 0,
                    Inst< MFVertexAttributeNode > _attrib     = 0,
                    Inst< AutoNormal   > _autoNormal         = 0,
                    Inst< SFBool       > _convex             = 0,
                    Inst< SFFloat      > _creaseAngle        = 0,
                    Inst< MFInt32      > _set_colorIndex     = 0,
                    Inst< MFInt32      > _set_coordIndex     = 0,
                    Inst< MFInt32      > _set_normalIndex    = 0,
                    Inst< MFInt32      > _set_texCoordIndex  = 0,
                    Inst< MFInt32      > _colorIndex         = 0,
                    Inst< MFInt32      > _coordIndex         = 0,
                    Inst< MFInt32      > _normalIndex        = 0,
                    Inst< MFInt32      > _texCoordIndex      = 0, 
                    Inst< SFFogCoordinate > _fogCoord        = 0);

    virtual X3DCoordinateNode *getCoord() {
      return static_cast< X3DCoordinateNode * >( coord->getValue() );
    }

    /// Renders the IndexedFaceSet using GL_POLYGONs.
    virtual void render();

    /// An upper bound on the number of triangles.
    virtual int nrTriangles() {
      return coordIndex->size();
    }

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    /// Field for setting the value of the colorIndex field.
    /// <b>Access type:</b> inputOnly 
    ///
    /// \dotfile IndexedFaceSet_set_colorIndex.dot 
    auto_ptr< MFInt32 >  set_colorIndex;

    /// Field for setting the value of the coordIndex field.
    /// <b>Access type:</b> inputOnly 
    ///
    /// \dotfile IndexedFaceSet_set_coordIndex.dot 
    auto_ptr< MFInt32 >  set_coordIndex;

    /// Field for setting the value of the normalIndex field. 
    /// <b>Access type:</b> inputOnly 
    ///
    /// \dotfile IndexedFaceSet_set_normalIndex.dot 
    auto_ptr< MFInt32 >  set_normalIndex;

    /// Field for setting the value of the texCoordIndex field. 
    /// <b>Access type:</b> inputOnly 
    ///
    /// \dotfile IndexedFaceSet_set_texCoordIndex.dot 
    auto_ptr< MFInt32 >  set_texCoordIndex;
    
    /// Indices into the color field. See 
    /// \ref IndexedFaceSet_details "Detailed description" 
    /// for details on how it is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> [0,inf) or -1
    ///
    /// \dotfile IndexedFaceSet_colorIndex.dot 
    auto_ptr< MFInt32 >  colorIndex;

    /// The convex field indicates whether all polygons in the shape are 
    /// convex. A polygon is convex if it is planar, does not intersect
    /// itself, and all of the interior angles at its vertices are less 
    /// than 180 degrees. Non planar and self intersecting polygons may 
    /// produce undefined results even if the convex field is FALSE.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile IndexedFaceSet_convex.dot 
    auto_ptr< SFBool  >  convex;

    /// Indices into the coord field. See 
    /// \ref IndexedFaceSet_details "Detailed description" 
    /// for details on how it is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> [0,inf) or -1
    ///
    /// \dotfile IndexedFaceSet_coordIndex.dot 
    auto_ptr< MFInt32 >  coordIndex;

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
    /// \dotfile IndexedFaceSet_creaseAngle.dot 
    auto_ptr< SFFloat >  creaseAngle;

    /// Indices into the normal field. See 
    /// \ref IndexedFaceSet_details "Detailed description" 
    /// for details on how it is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> [0,inf) or -1 \n
    ///
    /// \dotfile IndexedFaceSet_normalIndex.dot 
    auto_ptr< MFInt32 >  normalIndex;

    /// Indices into the texCoord field. See 
    /// \ref IndexedFaceSet_details "Detailed description" 
    /// for details on how it is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> [0,inf) or -1
    ///
    /// \dotfile IndexedFaceSet_texCoordIndex.dot 
    auto_ptr< MFInt32 >  texCoordIndex;

    /// Auto-generated normals that are used if the normal field is NULL.
    /// Only accessable in C++.
    ///
    /// \dotfile IndexedFaceSet_autoNormal.dot 
    auto_ptr< AutoNormal  >  autoNormal;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
