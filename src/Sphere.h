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
/// \file Sphere.h
/// \brief Header file for Sphere, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <X3DGeometryNode.h>
#include <SFFloat.h>

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

    /// The number of triangles renderered in this geometry.
    virtual int nrTriangles() {
      return 50 * 50 * 2;
    }

    /// Renders the Sphere with OpenGL.
    virtual void render();

    /// Traverse the scenegraph. Adds a HapticSphere if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );

    /// Detect intersection between a line segment and the sphere.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line
    /// \param theNodes A vector of pairs of pointer and index to
    /// differ between different places in the scene graph for the same Node.
    /// This can happen due to the DEF/USE feature of X3D.
    /// \param current_matrix The current matrix that transforms from the local
    /// coordinate space where this Node resides in the scenegraph to 
    /// global space.
    /// \param geometry_transforms A vector of matrices from the local
    /// coordinate space to global space for each node that the
    /// line intersects.
    /// \param pt_device_affect Flag telling a node if it is affected by a
    /// X3DPointingDeviceSensorNode. Needed to allow for correct behaviour
    /// when using the DEF/USE feature of X3D.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from,
      const Vec3f &to,    
      vector< HAPI::Bounds::IntersectionInfo > &result,
      vector< pair< Node *, H3DInt32 > > &theNodes,
      const Matrix4f &current_matrix,
      vector< Matrix4f > &geometry_transforms,
      bool pt_device_affect = false );

    /// Find closest point on the sphere to point p.
    /// \param p The point to find the closest point to.
    /// \param closest_point Return parameter for each closest point
    /// \param normal Return parameter for normal at each closest point.
    /// \param tex_coord Return paramater for each texture coordinate at
    /// closest point
    virtual void closestPoint( const Vec3f &p,
                               vector< Vec3f > &closest_point,
                               vector< Vec3f > &normal,
                               vector< Vec3f > &tex_coord );

    /// Detect collision between a moving sphere and this sphere.
    /// \param The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to );

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

  };
}

#endif
