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
/// \file TriangleSet2D.h
/// \brief Header file for TriangleSet2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TRIANGLESET2D_H__
#define __TRIANGLESET2D_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/MFVec2f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TriangleSet2D
  /// \brief The TriangleSet2D node specifies a set of triangles in the local 2D 
  /// coordinate system.  
  /// The vertices field specifies the triangles to be displayed. The number of
  /// vertices provided shall be evenly divisible by three.  Excess vertices
  /// shall be ignored. 
  /// 
  /// Textures are applied individually to each face of the TriangleSet2D. On the
  /// front (+Z) and back (-Z) faces of the TriangleSet2D, when viewed from the 
  /// +Z-axis, the texture is mapped onto each face with the same orientation
  /// as if the image were displayed normally in 2D. 
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TriangleSet2D.x3d">TriangleSet2D.x3d</a>
  ///     ( <a href="examples/TriangleSet2D.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile TriangleSet2D.dot
  
  class H3DAPI_API TriangleSet2D : 
    public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the vertices field 
    /// of the TriangleSet2D node.
    ///
    /// routes_in[0] is the vertices field of the TriangleSet2D node.  
    ///
    class SFBound: public TypedField< X3DGeometryNode::SFBound,
    MFVec2f > {
      /// Update the bound from the vertices field. 
      virtual void update() {
        const vector< Vec2f > &points = 
          static_cast< MFVec2f * >( routes_in[0] )->getValue();
        BoxBound *bound = new BoxBound();
        bound->fitAround2DPoints( points.begin(), points.end() );
        value = bound;
      }
    };
    
    /// Constructor.
    TriangleSet2D( Inst< SFNode      > _metadata = 0,
                   Inst< SFBound     > _bound = 0,
                   Inst< DisplayList > _displayList = 0,
                   Inst< MFBool      > _isTouched = 0,
                   Inst< MFVec3f     > _force = 0,
                   Inst< MFVec3f     > _contactPoint = 0,
                   Inst< MFVec3f     > _contactNormal = 0,
                   Inst< MFVec2f     > _vertices = 0,
                   Inst< SFBool      > _solid = 0 );

    ~TriangleSet2D();

    /// Renders the TriangleSet2D using OpenGL.
    virtual void render();

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    /// The number of triangles renderered in this geometry.
    virtual int nrTriangles() {
      return vertices->size() / 3;
    }

    /// The vertices field specifies the triangles to be displayed. 
    /// The number of vertices provided shall be evenly divisible by three.
    /// Excess vertices shall be ignored.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TriangleSet2D_vertices.dot
    auto_ptr< MFVec2f > vertices;

    /// The solid field specifies if both sides of the closed arc should be 
    /// rendered. If solid is FALSE then both sides will be rendered.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE
    /// 
    /// \dotfile TriangleSet2D_solid.dot
    auto_ptr< SFBool > solid;

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
