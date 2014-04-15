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
/// \file Cone.h
/// \brief Header file for Cone, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CONE_H__
#define __CONE_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Cone
  /// \brief The Cone node specifies a cone which is
  /// centred in the local coordinate system and whose central axis is
  /// aligned with the local Y-axis.
  ///
  /// The bottomRadius field specifies the radius of the cone's base, and
  /// the height field specifies the height of the cone from the centre of
  /// the base to the apex. By default, the cone has a radius of 1.0 at the
  /// bottom and a height of 2.0, with its apex at y = height/2 and its
  /// bottom at y = -height/2. Both bottomRadius and height shall be greater
  /// than zero. 
  ///
  /// The side field specifies whether sides of the cone are created and the
  /// bottom field specifies whether the bottom cap of the cone is
  /// created. A value of TRUE specifies that this part of the cone exists,
  /// while a value of FALSE specifies that this part does not exist (not
  /// rendered or eligible for collision or sensor intersection tests).
  ///
  /// When a texture is applied to the sides of the cone, the texture wraps
  /// counterclockwise (from above) starting at the back of the cone. The
  /// texture has a vertical seam at the back in the X=0 plane, from the
  /// apex (0, height/2, 0) to the point (0, -height/2, -bottomRadius). For
  /// the bottom cap, a circle is cut out of the texture square centred at
  /// (0, -height/2, 0) with dimensions (2 X bottomRadius) by (2 X
  /// bottomRadius). The bottom cap texture appears right side up when the
  /// top of the cone is rotated towards the -Z-axis. TextureTransform
  /// affects the texture coordinates of the Cone.
  ///
  /// The solid field determines whether the cone is visible when viewed
  /// from the inside.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Cone.x3d">Cone.x3d</a>
  ///     ( <a href="examples/Cone.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Cone.dot
  class H3DAPI_API Cone : public X3DGeometryNode {
  public:
    
    /// SFBound is specialized update itself from the radius and
    /// heigh field of the Cone node.
    ///
    /// routes_in[0] is the height field of the Cylinder node.  
    /// routes_in[1] is the bottomRadius field of the Cylinder node.  
    ///
    class SFBound: public X3DGeometryNode::SFBound {
      /// Update the bound from the radius field. 
      virtual void update() {
        H3DFloat h = static_cast< SFFloat * >( routes_in[0] )->getValue();
        H3DFloat br = static_cast< SFFloat * >( routes_in[1] )->getValue();
        BoxBound *bb = new BoxBound;
        H3DFloat d = 2 * br;
        bb->size->setValue( Vec3f( d, h, d ) );
        value = bb;
      }
    };
    
    /// Constructor.
    Cone( Inst< SFNode  > _metadata     = 0,
          Inst< SFBound >  _bound       = 0,
          Inst< SFBool  > _bottom       = 0,
          Inst< SFFloat > _bottomRadius = 0,
          Inst< SFFloat > _height       = 0,
          Inst< SFBool  > _side         = 0,
          Inst< SFBool  > _solid        = 0 );

    /// Renders the Box using OpenGL.
    virtual void render();

    /// The number of triangles rendered by this geometry.
    virtual int nrTriangles() {
      return 560;
    }

    /// Traverse the scenegraph. 
    virtual void traverseSG( TraverseInfo &ti ); 

    /// The side field specifies whether the bottom cap of the cone is
    /// rendered.
    /// <b>Access type:</b>  inputOutput
    ///
    /// \dotfile Cone_bottom.dot 
    auto_ptr< SFBool  >  bottom;

    /// The bottomRadius field specifies the radius of the cone's base.
    /// <b>Access type:</b>  inputOutput
    ///
    /// \dotfile Cone_bottomRadius.dot 
    auto_ptr< SFFloat >  bottomRadius;

    /// The height field specifies the height of the cone from the centre of
    /// the base to the apex.
    /// <b>Access type:</b>  inputOutput
    ///
    /// \dotfile Cone_height.dot 
    auto_ptr< SFFloat >  height;

    /// The side field specifies whether the sides of the cone are rendered.
    /// <b>Access type:</b>  inputOutput
    ///
    /// \dotfile Cone_side.dot 
    auto_ptr< SFBool  >  side;

    /// The solid field determines whether the box is visible when viewed from
    /// the inside. 
    /// <b>Access type:</b>  inputOutput
    ///
    /// \dotfile Cone_solid.dot 
    auto_ptr< SFBool  >  solid;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
