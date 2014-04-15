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
/// \file Cylinder.h
/// \brief Header file for Cylinder, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Cylinder
  /// \brief The Cylinder node specifies a capped cylinder centred at (0,0,0) 
  /// in the local coordinate system and with a central axis oriented along the
  /// local Y-axis.
  ///
  /// By default, the cylinder is sized at "-1" to "+1" in all three
  /// dimensions. The radius field specifies the radius of the cylinder and
  /// the height field specifies the height of the cylinder along the
  /// central axis. Both radius and height shall be greater than
  /// zero. 
  ///
  /// The cylinder has three parts: the side, the top (Y = +height/2) and
  /// the bottom (Y = -height/2). Each part has an associated SFBool field
  /// that indicates whether the part exists (TRUE) or does not exist
  /// (FALSE). Parts which do not exist are not rendered and not eligible
  /// for intersection tests (EXAMPLE collision detection or sensor
  /// activation).
  ///
  /// When a texture is applied to a cylinder, it is applied differently to
  /// the sides, top, and bottom. On the sides, the texture wraps
  /// counterclockwise (from above) starting at the back of the
  /// cylinder. The texture has a vertical seam at the back, intersecting
  /// the X=0 plane. For the top and bottom caps, a circle is cut out of the
  /// unit texture squares centred at (0, +-height/2, 0) with dimensions 2 X
  /// radius by 2 X radius. The top texture appears right side up when the
  /// top of the cylinder is tilted toward the +Z-axis, and the bottom
  /// texture appears right side up when the top of the cylinder is tilted
  /// toward the Z-axis. TextureTransform affects the texture coordinates of
  /// the Cylinder node.
  ///
  /// The solid field determines whether the cylinder is visible when viewed
  /// from the inside. If solid is false both sides of the cylinder will be 
  /// rendered.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Cylinder.x3d">Cylinder.x3d</a>
  ///     ( <a href="examples/Cylinder.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Cylinder.dot
  class H3DAPI_API Cylinder : public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the radius and
    /// heigh field of the Cylinder node.
    ///
    /// routes_in[0] is the height field of the Cylinder node.  
    /// routes_in[1] is the radius field of the Cylinder node.  
    ///
    class SFBound: public X3DGeometryNode::SFBound {
      /// Update the bound from the radius field. 
      virtual void update() {
        H3DFloat h = static_cast< SFFloat * >( routes_in[0] )->getValue();
        H3DFloat r = static_cast< SFFloat * >( routes_in[1] )->getValue();
        BoxBound *bb = new BoxBound;
        H3DFloat d = 2 * r;
        bb->size->setValue( Vec3f( d, h, d ) );
        value = bb;
      }
    };

    /// Constructor.
    Cylinder( Inst<    SFNode >  _metadata = 0,
              Inst< SFBound >                 _bound = 0,
              Inst< SFBool >  _bottom   = 0,
              Inst< SFFloat>  _height   = 0,
              Inst< SFFloat>  _radius   = 0,
              Inst< SFBool >  _side     = 0,
              Inst< SFBool >  _solid    = 0,
              Inst< SFBool >  _top      = 0 );

    /// Destructor
    ~Cylinder();

    /// Renders the Cylinder with OpenGL.
    virtual void render();

    /// Traverse the scenegraph. Adds a HapticCylinder if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );    

    /// The number of triangles rendered by this geometry.
    virtual int nrTriangles() {
      return 720;
    }
    /// Specifies if the bottom of the Cylinder should be rendered or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Cylinder_bottom.dot
    auto_ptr< SFBool  >  bottom;
    
    /// The height of the Cylinder in meters.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 2 \n
    /// <b>Valid range:</b> >0
    /// 
    /// \dotfile Cylinder_height.dot
    auto_ptr< SFFloat >  height;

    /// The radius of the Cylinder.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// <b>Valid range:</b> >0
    /// 
    /// \dotfile Cylinder_radius.dot
    auto_ptr< SFFloat >  radius;

    /// Specifies if the sides of the Cylinder should be rendered or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Cylinder_side.dot
    auto_ptr< SFBool  >  side;

    /// The solid field determines whether the cylinder is visible when viewed
    /// from the inside. If solid is false both sides of the sphere will be 
    /// rendered.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Cylinder_top.dot
    auto_ptr< SFBool  >  solid;

    /// Specifies if the top of the cylinder should be rendered or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Cylinder_top.dot
    auto_ptr< SFBool  >  top;

    static H3DNodeDatabase database;
  protected:
    // Internal field used to know if vertex buffer object can be created.
    auto_ptr< Field > vboFieldsUpToDate;
    // The index for the vertex buffer object
    GLuint *vbo_id;
  };
}

#endif
