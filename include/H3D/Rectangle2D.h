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
/// \file Rectangle2D.h
/// \brief Header file for Rectangle2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RECTANGLE2D_H__
#define __RECTANGLE2D_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFVec2f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Rectangle2D
  /// \brief The Rectangle2D node specifies a rectangle centred at (0, 0) in
  /// the current local 2D coordinate system and aligned with the local
  /// coordinate axes. 
  ///
  /// By default, the box measures 2 units in each dimension, from -1 to +1. 
  /// The size field specifies the extents of the box along the X-, and Y-axes
  /// respectively and each component value shall be greater than zero. 
  /// 
  /// Textures are applied individually to each face of the Rectangle2D. 
  /// On the front (+Z) and back (-Z) faces of the Rectangle2D, when viewed 
  /// from the +Z-axis, the texture is mapped onto each face with the same
  /// orientation as if the image were displayed normally in 2D. 
  /// TextureTransform affects the texture coordinates of the Rectangle2D
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Rectangle2D.x3d">Rectangle2D.x3d</a>
  ///     ( <a href="examples/Rectangle2D.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Rectangle2D.dot
  
  class H3DAPI_API Rectangle2D : 
    public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the size field 
    /// of the Rectangle2D node.
    ///
    /// routes_in[0] is the size field of the Rectangle2D node.  
    ///
    class SFBound: public X3DGeometryNode::SFBound {
      /// Update the bound from the box field. 
      virtual void update() {
        Vec2f b = static_cast< SFVec2f * >( routes_in[0] )->getValue();
        BoxBound *bb = new BoxBound;
        bb->size->setValue( Vec3f( b.x, b.y, 0 ) );
        value = bb;
      }
    };

    /// Constructor.
    Rectangle2D( Inst< SFNode      > _metadata = 0,
                 Inst< SFBound     > _bound = 0,
                 Inst< DisplayList > _displayList = 0,
                 Inst< MFBool      > _isTouched = 0,
                 Inst< MFVec3f     > _force = 0,
                 Inst< MFVec3f     > _contactPoint = 0,
                 Inst< MFVec3f     > _contactNormal = 0,
                 Inst< SFVec2f     > _size = 0,
                 Inst< SFBool      > _solid = 0 );
   
    /// Renders the rectangle2D using OpenGL.
    virtual void render();

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    /// The number of triangles rendered by this geometry
    virtual int nrTriangles() {
      return 2;
    }

    /// The size field specifies the extents of the box along the X-, and
    /// Y-axes respectively and each component value shall be greater than 
    /// zero.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 2 2
    /// 
    /// \dotfile Rectangle2D_size.dot
    auto_ptr< SFVec2f > size;

    /// The solid field specifies if both sides of the rectangle should be 
    /// rendered. If solid is FALSE then both sides will be rendered.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE
    /// 
    /// \dotfile Rectangle2D_solid.dot
    auto_ptr< SFBool > solid;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
