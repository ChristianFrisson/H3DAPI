//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Sphere.h
/// \brief Header file for Sphere, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "X3DGeometryNode.h"
#include "SFFloat.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Sphere
  /// \brief The Sphere node specifies a sphere centred at (0, 0, 0) in the
  /// local coordinate system. 
  ///
  /// The radius field specifies the radius of the sphere and shall be
  /// greater than zero. Figure 13.8 depicts the fields of the Sphere node.
  ///
  /// When a texture is applied to a sphere, the texture covers the entire
  /// surface, wrapping counterclockwise from the back of the sphere (i.e.,
  /// longitudinal arc intersecting the -Z-axis) when viewed from the top of
  /// the sphere. The texture has a seam at the back where the X=0 plane
  /// intersects the sphere and Z values are negative. 
  ///
  /// The solid field determines whether the sphere is visible when viewed
  /// from the inside. If solid is false both sides of the sphere will be 
  /// rendered.
  ///
  /// \par Internal routes:
  /// \dotfile Sphere.dot
  class H3DAPI_API Sphere : public X3DGeometryNode {
  public:
    
    /// SFBound is specialized update itself from the radius field 
    /// of the Sphere node.
    ///
    /// routes_in[0] is the radius field of the Sphere node.  
    ///
    class SFBound: public TypedField< X3DGeometryNode::SFBound,
                                      SFFloat >{
      /// Update the bound from the radius field. 
      virtual void update() {
        H3DFloat d = 2 * static_cast< SFFloat * >( routes_in[0] )->getValue();
        BoxBound *bb = new BoxBound;
        bb->size->setValue( Vec3f( d, d, d ) );
        value = bb;
      }
    };

    /// Constructor.
    Sphere( Inst<    SFNode >  _metadata = 0,
            Inst< SFBound >                 _bound = 0,
            Inst< SFFloat>  _radius   = 0,
            Inst< SFBool >  _solid    = 0 );

    /// Renders the Sphere with OpenGL.
    virtual void render();

#ifdef USE_HAPTICS
    /// Traverse the scenegraph. Adds a HapticSphere if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );
#endif

    /// The radius of the Sphere in metres.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// <b>Valid range:</b> >0
    /// 
    /// \dotfile Sphere_radius.dot
    auto_ptr< SFFloat >  radius;

    /// Specifies if both sides of the Sphere should be rendered or only the
    /// outside. Both are rendered if false, outside only it true.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Sphere_solid.dot
    auto_ptr< SFBool  >  solid;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    GLUquadricObj* gl_quadric;
  };
}

#endif
