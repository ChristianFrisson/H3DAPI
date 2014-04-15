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
/// \file IndexedLineSet.h
/// \brief Header file for IndexedLineSet, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __INDEXEDLINESET_H__
#define __INDEXEDLINESET_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/X3DColorNode.h>
#include <H3D/CoordBoundField.h>
#include <H3D/MFInt32.h>
#include <H3D/FogCoordinate.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class IndexedLineSet
  /// \brief The IndexedLineSet node represents a 3D geometry 
  /// formed by constructing polylines from 3D vertices specified in the coord
  /// field. 
  ///
  /// \anchor IndexedLineSet_details
  /// IndexedLineSet uses the indices in its coordIndex field to
  /// specify the polylines by connecting vertices from the coord field. An
  /// index of "-1" indicates that the current polyline has ended and the
  /// next one begins. The last polyline may be (but does not have to be)
  /// followed by a "-1". IndexedLineSet is specified in the local
  /// coordinate system and is affected by the transformations of its
  /// ancestors.
  ///
  /// The coord field specifies the 3D vertices of the line set and contains
  /// a X3DCoordinateNode.
  /// 
  /// Lines are not lit, are not texture-mapped, and do not participate in
  /// collision detection. The width of lines is implementation dependent
  /// and each line segment is solid (i.e., not dashed).
  ///
  /// \anchor IndexedLineSet_color_details
  /// If the color field is not NULL, it shall contain a Color node. The
  /// colours are applied to the line(s) as follows:
  ///
  /// <ul>
  /// <li>
  /// If colorPerVertex is FALSE: 
  /// <ol><li>
  ///   If the colorIndex field is not empty, one colour is used for each
  ///   polyline of the IndexedLineSet. There shall be at least as many
  ///   indices in the colorIndex field as there are polylines in the
  ///   IndexedLineSet. If the greatest index in the colorIndex field is N,
  ///   there shall be N+1 colours in the Color node. The colorIndex field
  ///   shall not contain any negative entries. 
  /// <li>
  ///   If the colorIndex field is empty, the colours from the Color node are
  ///   applied to each polyline of the IndexedLineSet in order. There shall
  ///   be at least as many colours in the Color node as there are polylines. 
  /// </ol>
  ///
  /// <li>
  /// If colorPerVertex is TRUE: 
  /// <ol> <li>
  ///   If the colorIndex field is not empty, colours are applied to each
  ///   vertex of the IndexedLineSet in exactly the same manner that the
  ///   coordIndex field is used to supply coordinates for each vertex from
  ///   the Coordinate node. The colorIndex field shall contain at least as
  ///   many indices as the coordIndex field and shall contain end-of-polyline
  ///   markers (-1) in exactly the same places as the coordIndex field. If
  ///   the greatest index in the colorIndex field is N, there shall be N+1
  ///   colours in the Color node. 
  /// <li>
  ///   If the colorIndex field is empty, the coordIndex field is used to
  ///   choose colours from the Color node. If the greatest index in the
  ///   coordIndex field is N, there shall be N+1 colours in the Color node. 
  /// </ol></ul>
  /// If the color field is NULL and there is a Material defined for the
  /// Appearance affecting this IndexedLineSet, the emissiveColor of the
  /// Material shall be used to draw the lines. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/IndexedLineSet.x3d">IndexedLineSet.x3d</a>
  ///     ( <a href="examples/IndexedLineSet.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile IndexedLineSet.dot
  class H3DAPI_API IndexedLineSet : public X3DGeometryNode {
  public:
    /// Thrown when the colorIndex field does not match the coordIndex 
    /// field properly.
    H3D_VALUE_EXCEPTION( int, InvalidColorIndex );

    /// Thrown when the colorIndex field's size does not match the coordIndex 
    /// field properly.
    H3D_VALUE_EXCEPTION( int, InvalidColorIndexSize );

    /// SFCoordinateNode is dependent on coordChanged field in 
    /// X3DCoordinateNode.
    typedef DependentSFNode< X3DCoordinateNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DCoordinateNode::propertyChanged > > 
    SFCoordinateNode;

    /// SFColorNode is dependent on colorChanged field in 
    /// X3DColorNode.
    typedef DependentSFNode< X3DColorNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DColorNode::propertyChanged > > 
    SFColorNode;

    /// The SFFogCoordinate is dependent on the propertyChanged
    /// field of the contained FogCoordinate.
    typedef DependentSFNode< 
                FogCoordinate,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &FogCoordinate::propertyChanged > > 
    SFFogCoordinate; 

    /// The MFVertexAttributeNode is dependent on the propertyChanged
    /// field of the contained X3DVertexAttributeNode.
    class H3DAPI_API MFVertexAttributeNode : public DependentMFNode< 
                X3DVertexAttributeNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DVertexAttributeNode::propertyChanged > > {
    };

    /// Display list is extended in order to set color to emissive
    /// color from material outside of display list, since we have to
    /// do a glGet to get the value. If we have it inside, the display
    /// list will not be rebuilt and the color not change.
    class H3DAPI_API DisplayList: public X3DGeometryNode::DisplayList {
    public:
      /// Set the color of the lineset outside display list.
      virtual void callList( bool build_list = true );
    };
    
    /// Constructor.
    IndexedLineSet( Inst< SFNode           > _metadata       = 0,
                    Inst< CoordBoundField  > _bound          = 0,
                    Inst< DisplayList      > _displayList    = 0,
                    Inst< MFInt32          > _set_colorIndex = 0,
                    Inst< MFInt32          > _set_coordIndex = 0,
                    Inst< SFColorNode      > _color          = 0,
                    Inst< SFCoordinateNode > _coord          = 0,
                    Inst< MFInt32          > _colorIndex     = 0,
                    Inst< SFBool           > _colorPerVertex = 0,
                    Inst< MFInt32          > _coordIndex     = 0, 
                    Inst< SFFogCoordinate  > _fogCoord       = 0,
                    Inst< MFVertexAttributeNode > _attrib    = 0 );

    ~IndexedLineSet();

    /// The number of lines rendered by this geometry.
    virtual int nrLines() {
      unsigned int size = coordIndex->size();
      if( size > 1 )return size - 1;
      else return 0;
    }

    /// Render the IndexedLineSet with OpenGL
    virtual void render();

    /// Detect collision between a moving sphere and the geometry.
    /// \param radius The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \param result A struct containing various results of intersections
    /// such as which geometries intersected the moving sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to,
                                        NodeIntersectResult &result ) {
      return false;
    }

    /// Field for setting the value of the colorIndex field.
    /// <b>Access type:</b> inputOnly 
    ///
    /// \dotfile IndexedLineSet_set_colorIndex.dot 
    auto_ptr< MFInt32 >  set_colorIndex;

    /// Field for setting the value of the coordIndex field.
    /// <b>Access type:</b> inputOnly 
    ///
    /// \dotfile IndexedLineSet_set_coordIndex.dot 
    auto_ptr< MFInt32 >  set_coordIndex;

    /// Contains an X3DColorNode which colors are applied to the
    /// X3DComposedGeometryNode. If the color field is NULL and there is a
    /// Material defined for the Appearance affecting this IndexedLineSet,
    /// the emissiveColor of the Material shall be used to draw the lines.
    /// If a color is not NULL colors are applied as described in the
    /// \ref IndexedLineSet_color_details "detailed description" 
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile IndexedLineSet_color.dot 
    auto_ptr< SFColorNode >  color;

    /// The coord field specifies the 3D vertices of the line set and contains
    /// a X3DCoordinateNode.
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile IndexedLineSet_coord.dot 
    auto_ptr< SFCoordinateNode >  coord;

    /// Indices into the color field. See 
    /// \ref IndexedLineSet_details "Detailed description" 
    /// for details on how it is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> [0,inf) or -1
    ///
    /// \dotfile IndexedLineSet_colorIndex.dot 
    auto_ptr< MFInt32 >  colorIndex;

    /// If TRUE the colors in the color field are applied per vertex,
    /// otherwise it is applied per polyline.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile IndexedLineSet_colorPerVertex.dot 
    auto_ptr< SFBool  >  colorPerVertex;

    /// Indices into the coord field. See 
    /// \ref IndexedLineSet_details "Detailed description" 
    /// for details on how it is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> [0,inf) or -1
    ///
    /// \dotfile IndexedLineSet_coordIndex.dot 
    auto_ptr< MFInt32 >  coordIndex;

    /// If the fogCoord field is not empty, it shall contain a list 
    /// of per-vertex depth values for calculating fog depth.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile IndexedLineSet_fogCoord.dot 
    auto_ptr< SFFogCoordinate > fogCoord;

    /// If the attrib field is not empty it shall contain a list of
    /// X3DVertexAttributeNode instances with per-vertex attribute
    /// information for programmable shaders.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile IndexedLineSet_attrib.dot 
    auto_ptr< MFVertexAttributeNode > attrib;

    /// The H3DNodeDatabase instance for this node.
    static H3DNodeDatabase database;
  protected:
    // Internal field used to know if vertex buffer object can be created.
    auto_ptr< Field > vboFieldsUpToDate;
    // The index for the vertex buffer object
    GLuint *vbo_id;
    vector< pair< GLsizei, pair< GLsizei, GLsizei > > > nr_index;
  };
}

#endif
