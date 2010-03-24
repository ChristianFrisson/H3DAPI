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
/// \file ShadowBox.h
/// \brief Header file for ShadowBox.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADOWGEOMETRY_H__
#define __SHADOWGEOMETRY_H__

#include <H3D/H3DShadowObjectNode.h>
#include <H3D/DirectionalLight.h>
#include <H3D/PointLight.h>
#include <H3D/SFRotation.h>
#include <H3D/MatrixTransform.h>
#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ShadowGeometry
  /// The ShadowGeometry object specifies a X3DGeometryNode that should be
  /// used for casting a shadow when used in the ShadowCaster node.
  ///
  /// The geometry field specifies the X3DGeometryNode that should cast a 
  /// shadow.
  ///
  /// The transform field specifies a possible transformation of the 
  /// geometry.
  ///
  /// \par Internal routes:
  /// \dotfile ShadowGeometry.dot  
  class ShadowGeometry : public H3DShadowObjectNode {
  public:

    /// SFGeometryNode is specialized to route the boundTree field
    /// from the X3DGeometryNodes that are put into the field to 
    /// the triangles_changed field of the ShadowGeometry that
    /// contains it.
    class SFGeometryNode: public TypedSFNode< X3DGeometryNode > {
    protected:
      virtual void onAdd( Node *n );
      virtual void onRemove( Node *n );
    };

    /// Constructor.
    ShadowGeometry( Inst< SFNode          > _metadata  = 0,
                    Inst< SFTransformNode > _transform = 0,
                    Inst< SFGeometryNode  > _geometry  = 0);

    /// Render the shadow volume for this shadow object.
    virtual void renderShadow( X3DLightNode *light, bool render_caps );      

  public:
    auto_ptr< SFGeometryNode > geometry;

    /// The H3DNodeDatabase object for this node.
    static H3DNodeDatabase database;
    
  protected:
   

    /// Render a quad using the given points and its projections at 
    /// infinity assuming a DirectionalLight with direction dir.
    void renderDirectionalLightQuad( const Vec3d &v1, const Vec3d &v2, 
                                     const Vec3d& dir );

    /// Render a quad using the given points and its projections at 
    /// infinity assuming a PointLight with position light_pos.
    void renderPointLightQuad( const Vec3d &v1, const Vec3d &v2, 
                               const Vec3d& light_pos );

    /// Updates the neighbours array by analysing the triangles from
    /// the node in the geometry field.
    void updateNeighbours( const vector< HAPI::Collision::Triangle > &triangles);

    /// Updates the is_silhouette_edge and triangle_facing_light 
    /// arrays given a directional light source shining in the
    /// direction direction.
    void updateSilhouetteEdgesDirectionalLight( const vector< HAPI::Collision::Triangle > &,
                                                const vector<int> &neighbours,
                                                const Vec3d &direction );

    /// Updates the is_silhouette_edge and triangle_facing_light 
    /// arrays given a point light source shining at position pos.
    void updateSilhouetteEdgesPointLight( const vector< HAPI::Collision::Triangle > &,
                                          const vector<int> &neighbours,
                                          const Vec3d &pos );

    /// Array of 3*nr_triangles values, one for each triangle edge. The value
    /// specifies if the edge is a silhouette edge or not, i.e. if the edge
    /// is shared between two triangles and one of the triangles are facing
    /// the light while the other one is not, or the edge is only used
    /// by one triangle.
    vector< bool > is_silhouette_edge;

    /// Array of nr_triangle values indicating if the triangle is facing
    /// the light source or not.
    vector< bool > triangle_facing_light;

  };
}

#endif
