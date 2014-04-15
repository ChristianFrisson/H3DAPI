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
/// \file Box.h
/// \brief Header file for Box, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOX_H__
#define __BOX_H__

#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Box
  /// \brief The Box node specifies a rectangular
  /// parallelepiped box centred at (0, 0, 0) in the local coordinate
  /// system and aligned with the local coordinate axes.
  ///
  /// By default, the box measures 2 units in each dimension, from -1 to
  /// +1. The size field specifies the extents of the box along the X-, Y-,
  /// and Z-axes respectively and each component value shall be greater than
  /// zero. 
  ///
  /// Textures are applied individually to each face of the box. On the
  /// front (+Z), back (-Z), right (+X), and left (-X) faces of the box,
  /// when viewed from the outside with the +Y-axis up, the texture is
  /// mapped onto each face with the same orientation as if the image were
  /// displayed normally in 2D. On the top face of the box (+Y), when viewed
  /// from above and looking down the Y-axis toward the origin with the
  /// -Z-axis as the view up direction, the texture is mapped onto the face
  /// with the same orientation as if the image were displayed normally in
  /// 2D. On the bottom face of the box (-Y), when viewed from below looking
  /// up the Y-axis toward the origin with the +Z-axis as the view up
  /// direction, the texture is mapped onto the face with the same
  /// orientation as if the image were displayed normally in
  /// 2D. TextureTransform affects the texture coordinates of the Box.
  ///
  /// The solid field determines whether the box is visible when viewed from
  /// the inside. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Box.x3d">Box.x3d</a>
  ///     ( <a href="examples/Box.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Box.dot
  class H3DAPI_API Box : public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the size field 
    /// of the Box node.
    ///
    /// routes_in[0] is the size field of the Box node.  
    ///
    class SFBound: public X3DGeometryNode::SFBound {
      /// Update the bound from the size field. 
      virtual void update() {
        Vec3f b = static_cast< SFVec3f * >( routes_in[0] )->getValue();
        BoxBound *bb = new BoxBound;
        bb->size->setValue( b );
        value = bb;
      }
    };
    
    /// Constructor.
    Box( Inst< SFNode  >  _metadata = 0,
         Inst< SFBound >  _bound    = 0,
         Inst< SFVec3f >  _size     = 0,
         Inst< SFBool  >  _solid    = 0 );

    /// Renders the Box using OpenGL.
    virtual void render();

    /// The number of triangles rendered by this geometry.
    virtual int nrTriangles() {
      return 12;
    }

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    ///  The size field specifies the extents of the box along the X-, Y-,
    /// and Z-axes respectively.
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 2 2 2 \n
    /// <b>Valid range:</b> >0
    ///
    /// \dotfile Box_size.dot 
    auto_ptr< SFVec3f >  size;

    /// The solid field determines whether the box is visible when viewed from
    /// the inside. 
    /// <b>Access type:</b>  inputOutput
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile Box_solid.dot 
    auto_ptr< SFBool  >  solid;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
