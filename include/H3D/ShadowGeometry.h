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
/// \file ShadowGeometry.h
/// \brief Header file for ShadowGeometry.
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
#include <H3D/ComposedShader.h>

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
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ShadowCaster.x3d">ShadowCaster.x3d</a>
  ///     ( <a href="examples/ShadowCaster.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ShadowGeometry.dot  
  class H3DAPI_API ShadowGeometry : public H3DShadowObjectNode {
  public:

    /// SFGeometryNode is specialized to route the boundTree field
    /// from the X3DGeometryNodes that are put into the field to 
    /// the triangles_changed field of the ShadowGeometry that
    /// contains it.
    class H3DAPI_API SFGeometryNode: public TypedSFNode< X3DGeometryNode > {
    protected:
      virtual void onAdd( Node *n );
      virtual void onRemove( Node *n );
    };

    /// Constructor.
    ShadowGeometry( Inst< SFNode          > _metadata  = 0,
                    Inst< SFTransformNode > _transform = 0,
                    Inst< SFGeometryNode  > _geometry  = 0);

    /// Render the shadow volume for this shadow object.
    virtual void renderShadow( X3DLightNode *light, 
                               bool render_caps,
                               const Matrix4f &local_to_global = Matrix4f() );

  protected:
    /// This field will be sent an event when the triangles in the geometry
    /// field have changed.
    //  This field has to be defined before geometry in the node since when
    //  geometry is destructed this field is used and have to exist.
    auto_ptr<Field> triangles_changed; 
  public:

    /// The geometry field specifies the X3DGeometryNode that should cast a 
    /// shadow.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// \dotfile ShadowGeometry_geometry.dot
    auto_ptr< SFGeometryNode > geometry;

    /// The H3DNodeDatabase object for this node.
    static H3DNodeDatabase database;
    
  protected:
    /// Render the shadow volume for this shadow object using geometry
    /// shader. Requires graphics card to support OpenGL 
    /// extension EXT_geometry_shader4.
    virtual void renderShadowGeometryShader( X3DGeometryNode *geometry, 
                                             X3DLightNode *light, 
                                             bool render_caps,
                                             const Matrix4f &local_to_global,
                                             const Matrix4f &global_to_local,
                                             bool rebuild_triangle_info );
   
    /// Render the shadow volume for this shadow object without geometry
    /// shader. This is the fallback option if graphics card does not
    /// support geometry shaders
    virtual void renderShadowFallback( X3DGeometryNode *geometry, 
                                       X3DLightNode *light, 
                                       bool render_caps,
                                       const Matrix4f &local_to_global,
                                       const Matrix4f &global_to_local,
                                       bool rebuild_triangle_info );

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

    /// Geven a list of triangles the vectors triangle_points and adjacency_index
    /// is filled with coordinates and indices for use for rendering all the triangles
    /// as vertex array with adjacency information.
    void updateAdjacenctVertexArray( const vector< HAPI::Collision::Triangle > &triangles, 
                                     vector< Vec3d > &triangle_points, 
                                     vector< unsigned int > &adjacency_index );

    /// Given a triangle and two of its vertices(p0 and p1) the index of the
    /// third point of the triangle is returned. If point a 0 is returned,
    /// if b then 1 and if c 2.
    /// If -1 is returned then the functions has been used incorrectly.
    int getMissingPointIndex( const HAPI::Collision::Triangle &t,
                              const Vec3d &p0, const Vec3d &p1 );

    /// Returns the string to use as geometry shader for a PointLight
    /// source.
    virtual string getPointLightGeometryShaderString();

    /// Returns the string to use as geometry shader for a DirectionalLight
    /// source.
    virtual string getDirLightGeometryShaderString();

    /// Array of 3*nr_triangles triangle indices specifying for each triangle
    /// edge which triangle is its neighbour. 
    vector< int > neighbours;

    /// The triangles of the geometry. We cache them here instead of 
    /// getting them from the boundTree since that is quite slow.
    vector< HAPI::Collision::Triangle > triangles;

    /// Points to be used for vertex array rendering with adjacency info.
    vector< Vec3d > triangle_points;

    /// Index to be used for vertex array rendering with adjacency info.
    vector< unsigned int > adjacency_index;

    /// Shader to use for point lights when using a geometry shader to
    /// draw shader volumes,
    AutoRef< ComposedShader > point_light_shader;
    
    /// Fields used for uniform variables in point_light_shader. The 
    /// point_light_shader node owns the pointer so it should not be
    /// removed by the ShadowGeometry node.
    auto_ptr< SFMatrix4f > modelMatrix;
    auto_ptr< SFVec3f > lightParam;
    auto_ptr< SFBool > drawCaps;
    
    /// Shader to use for directional lights when using a geometry shader to
    /// draw shader volumes,
    AutoRef< ComposedShader > dir_light_shader;

    /// Array of 3*nr_triangles values, one for each triangle edge. The value
    /// specifies if the edge is a silhouette edge or not, i.e. if the edge
    /// is shared between two triangles and one of the triangles are facing
    /// the light while the other one is not, or the edge is only used
    /// by one triangle.
    vector< bool > is_silhouette_edge;

    /// Array of nr_triangle values indicating if the triangle is facing
    /// the light source or not.
    vector< bool > triangle_facing_light;

    /// True if geometry shader was used last time this ShadowGeometry was rendered.
    bool use_geometry_shader_last_loop;
  };
}

#endif
