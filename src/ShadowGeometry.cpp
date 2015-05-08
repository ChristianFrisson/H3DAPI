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
/// \file ShadowGeometry.cpp
/// \brief CPP file for ShadowGeometry
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShadowGeometry.h>
#include <H3D/GraphicsOptions.h>
#include <H3D/GlobalSettings.h>
#include <H3D/ShadowCasterShaders.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShadowGeometry::database( "ShadowGeometry", 
                                     &(newInstance<ShadowGeometry>), 
                                     typeid( ShadowGeometry ),
                                     &H3DShadowObjectNode::database );

namespace ShadowGeometryInternals {
  FIELDDB_ELEMENT( ShadowGeometry, geometry, INPUT_OUTPUT );
}

ShadowGeometry::ShadowGeometry( Inst< SFNode>  _metadata,
                                Inst< SFTransformNode > _transform,
                                Inst< SFBool > _enabled,
                                Inst< SFGeometryNode > _geometry ) :
H3DShadowObjectNode( _metadata, _transform, _enabled ),
  geometry( _geometry ),
  triangles_changed( new Field ),
  use_geometry_shader_last_loop( false ) {

  type_name = "ShadowGeometry";
  database.initFields( this );

}


void ShadowGeometry::addDirectionalLightQuadPoints( vector< Vec4d > &triangle_points,
                                                    const Vec3d &v1, 
                                                    const Vec3d &v2, 
                                                    const Vec3d& dir ) {
  Vec4d ev1(v1, 1);
  Vec4d ev2(v2, 1);
  Vec4d inf( dir, 0 );

  triangle_points.push_back( ev1 );
  triangle_points.push_back( inf );
  triangle_points.push_back( inf );

  triangle_points.push_back( ev1 );
  triangle_points.push_back( inf );
  triangle_points.push_back( ev2 );
}

void ShadowGeometry::addPointLightQuadPoints( vector< Vec4d > &triangle_points,
                                             const Vec3d &v1, const Vec3d &v2, 
                                             const Vec3d& light_pos ) {
  Vec3d dir1 = v1 - light_pos;
  dir1.normalizeSafe();
  
  Vec3d dir2 = v2 - light_pos;
  dir2.normalizeSafe();
  
  Vec4d ev1(v1, 1 );
  Vec4d ev2(v2, 1 );
  Vec4d inf1( dir1, 0 );
  Vec4d inf2( dir2, 0 );

  triangle_points.push_back( ev1 );
  triangle_points.push_back( inf1 );
  triangle_points.push_back( inf2 );

  triangle_points.push_back( ev1 );
  triangle_points.push_back( inf2 );
  triangle_points.push_back( ev2 );
}

void ShadowGeometry::renderShadow( X3DLightNode *light, 
                                   bool draw_caps,
                                   const Matrix4f &local_to_global ) {
  
  if( !enabled->getValue() ) return;
  
  MatrixTransform *t = transform->getValue();
  X3DGeometryNode *g = geometry->getValue();

  if( !g ) return;

  Matrix4f m, m_inv;
  if(t) {
    m = local_to_global * t->matrix->getValue();
  } else {
    m = local_to_global;
  }
  m_inv = m.inverse();

  GraphicsOptions *graphics_options = NULL;
  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings ) {
    default_settings->getOptionNode( graphics_options );
  }
  
  bool use_geometry_shader = 
      GLEW_EXT_geometry_shader4 && 
      ( !graphics_options || 
        (graphics_options && 
         graphics_options->defaultShadowGeometryAlgorithm->getValue() == "GEOMETRY_SHADER" ) );


  bool rebuild_triangle_info = !triangles_changed->isUpToDate() || 
       use_geometry_shader_last_loop != use_geometry_shader;

  triangles_changed->upToDate();

  ShadowCasterShaders::setTransformMatrix( m );

  if( GLEW_EXT_geometry_shader4 && 
      ( !graphics_options || 
        (graphics_options && 
         graphics_options->defaultShadowGeometryAlgorithm->getValue() == "GEOMETRY_SHADER" ) ) ) {
    renderShadowGeometryShader( g, light, draw_caps, m, m_inv, rebuild_triangle_info );
  } else {
    renderShadowFallback( g, light, draw_caps, m, m_inv, rebuild_triangle_info );
  }

  use_geometry_shader_last_loop = use_geometry_shader;
}

void ShadowGeometry::renderShadowFallback( X3DGeometryNode *g,
                                           X3DLightNode *light, 
                                           bool draw_caps,
                                           const Matrix4f &m,
                                           const Matrix4f &m_inv,
                                           bool rebuild_triangle_info ) {


  if( rebuild_triangle_info ) {
    triangles.clear();
    g->boundTree->getValue()->getAllTriangles( triangles );
    updateNeighbours( triangles );
  }
  
  // only update triangles once per scene loop
  pair< H3DTime, vector< Vec4d > > & point_pair = triangle_points_fallback[light];
  H3DTime &last_update_time = point_pair.first;
  vector< Vec4d > &triangle_points = point_pair.second;
  
  H3DTime frame_time = Scene::time->getValue();
  if( frame_time != last_update_time ) {
    last_update_time = frame_time;
    triangle_points.clear();
    triangle_points.reserve( triangles.size() * 3 *2 );

    // draw quads for each silhouette edge and its projection at infinity.

    // directional light
    DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );     
    if( dir_light ) {
      Vec3f dir = dir_light->direction->getValue();
      dir = m_inv.getRotationPart() * dir;
      updateSilhouetteEdgesDirectionalLight( triangles, neighbours, dir );
      for( size_t i = 0; i < triangles.size(); ++i ) {
        // no silhouette edges are on triangles not facing the light
        if( !triangle_facing_light[i] ) continue;
        if( is_silhouette_edge[ i*3 ] ) 
          addDirectionalLightQuadPoints( triangle_points, triangles[i].a, triangles[i].b, dir );
        if( is_silhouette_edge[ i*3+1 ] ) 
          addDirectionalLightQuadPoints( triangle_points, triangles[i].b, triangles[i].c, dir );
        if( is_silhouette_edge[ i*3 +2] ) 
          addDirectionalLightQuadPoints( triangle_points, triangles[i].c, triangles[i].a, dir );
      }
    }

    // point light
    PointLight *point_light = dynamic_cast< PointLight * >( light );
    Vec3f light_pos;

    if( point_light ) {
      light_pos = point_light->location->getValue();
      light_pos = m_inv * light_pos;
      updateSilhouetteEdgesPointLight( triangles, neighbours, light_pos );
      for( size_t i = 0; i < triangles.size(); ++i ) {
        // no silhouette edges are on triangles not facing the light
        if( !triangle_facing_light[i] ) continue;
        if( is_silhouette_edge[ i*3 ] ) addPointLightQuadPoints( triangle_points, triangles[i].a, triangles[i].b, light_pos );
        if( is_silhouette_edge[ i*3+1 ] ) addPointLightQuadPoints( triangle_points, triangles[i].b, triangles[i].c, light_pos );
        if( is_silhouette_edge[ i*3 +2] ) addPointLightQuadPoints( triangle_points, triangles[i].c, triangles[i].a, light_pos );
      }
    }

    if( draw_caps ) {
      // draw all triangles facing the light as a near cap and all others
      // at infinity.
    
      if( dir_light ) {
        Vec3d dir = dir_light->direction->getValue();
        dir = m_inv.getRotationPart() * dir;
      
        for( size_t i = 0; i < triangles.size(); ++i ) {
          if( triangle_facing_light[i] ) {
            triangle_points.push_back( Vec4d( triangles[i].a, 1 ) );
            triangle_points.push_back( Vec4d( triangles[i].b, 1 ) );
            triangle_points.push_back( Vec4d( triangles[i].c, 1 ) );
          }
          // directional lights do not need a far cap since the shadow volume
          // converge to the same point at infinity.
        }
      } else if( point_light ) {
        for( size_t i = 0; i < triangles.size(); ++i ) {
          if( triangle_facing_light[i] ) {
            triangle_points.push_back( Vec4d( triangles[i].a, 1 ) );
            triangle_points.push_back( Vec4d( triangles[i].b, 1 ) );
            triangle_points.push_back( Vec4d( triangles[i].c, 1 ) );
          } else {
            const Vec3f &p = light_pos;
            Vec3d v1 = triangles[i].a - p;
            Vec3d v2 = triangles[i].b - p;
            Vec3d v3 = triangles[i].c - p;
          
            triangle_points.push_back( Vec4d( v1, 0 ) );
            triangle_points.push_back( Vec4d( v2, 0 ) );
            triangle_points.push_back( Vec4d( v3, 0 ) );
          }
        }
      }
    }
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4, GL_DOUBLE, 0,
                  &(*triangle_points.begin()) );
  glDrawArrays( GL_TRIANGLES, 0, triangle_points.size() );
  glDisableClientState(GL_VERTEX_ARRAY);

}


void ShadowGeometry::updateSilhouetteEdgesDirectionalLight( const vector< HAPI::Collision::Triangle > &_triangles,
                                                            const vector<int> &_neighbours,
                                                            const Vec3d &direction ) {

  triangle_facing_light.resize( _triangles.size(), false );
  is_silhouette_edge.resize( _triangles.size()*3, false );
  
  for( size_t i = 0; i < _triangles.size(); ++i ) {
    triangle_facing_light[i] = direction.dotProduct( _triangles[i].normal ) <= 0;
  }
 
  for( size_t i = 0; i < _neighbours.size(); ++i ) {
    if(!triangle_facing_light[i/3]) {
      // silhouette edges are only on triangles facing the light
      is_silhouette_edge[i] = false;
    } else {
      // silhouette if no neighbour or the neighbour is not facing the light
      is_silhouette_edge[i] = _neighbours[i] == -1 || !triangle_facing_light[_neighbours[i]];
    }
  }
}


void ShadowGeometry::updateSilhouetteEdgesPointLight( const vector< HAPI::Collision::Triangle > &_triangles,
                                                      const vector<int> &_neighbours,
                                                      const Vec3d &pos ) {
  triangle_facing_light.resize( _triangles.size(), false );
  is_silhouette_edge.resize( _triangles.size()*3, false );

  for( size_t i = 0; i < _triangles.size(); ++i ) {
    Vec3d direction = _triangles[i].a - pos;
    triangle_facing_light[i] = direction.dotProduct( _triangles[i].normal ) <= 0;
  }

  for( size_t i = 0; i < _neighbours.size(); ++i ) {
    if(!triangle_facing_light[i/3]) {
      // silhouette edges are only on triangles facing the light
      is_silhouette_edge[i] = false;
    } else {
      // silhouette if no neighbour or the neighbour is not facing the light
      is_silhouette_edge[i] = _neighbours[i] == -1 || !triangle_facing_light[_neighbours[i]];
    }
  }

 }


bool operator< (const Vec3d & s1, const Vec3d &s2) {
  return (s1.x < s2.x || 
          (s2.x == s1.x && s1.y < s2.y) ||
          ( s2.x == s1.x && s2.y == s1.y && s1.z < s2.z) );
}

struct lt {
  bool operator()(const pair<Vec3d, Vec3d>& _Left,
                  const pair<Vec3d, Vec3d >& _Right ) const {
    return (_Left.first < _Right.first ||
            (!(_Right.first < _Left.first) && _Left.second < _Right.second));
  }
};

void ShadowGeometry::updateNeighbours( const vector< HAPI::Collision::Triangle > &_triangles ) {
  neighbours.clear();
  neighbours.resize( _triangles.size()*3, -1 );

  // map from triangle edge(as pair of vertex) to pair of 
  // (triangle index, edge index within triangle) 
  typedef map< pair< Vec3d, Vec3d >, pair<int, int>, lt >  EdgeTriangleMap;
  EdgeTriangleMap edges;
 
  for( unsigned int i = 0; i < _triangles.size(); ++i ) {
    const HAPI::Collision::Triangle &tri = _triangles[i];

    // ignore invalid triangles that are lines or points
    if( tri.a == tri.b || tri.b == tri.c || tri.a == tri.c ) {
      continue;
    }

    // edges of the triangles. We keep a strict ordering when defining an edge so that 
    // the edge (a,b) will be the same as (b,a).
    pair< Vec3d, Vec3d > edge0 = tri.a < tri.b ? make_pair(tri.a, tri.b) : make_pair(tri.b, tri.a );
    pair< Vec3d, Vec3d > edge1 = tri.b < tri.c ? make_pair(tri.b, tri.c) : make_pair(tri.c, tri.b );
    pair< Vec3d, Vec3d > edge2 = tri.c < tri.a ? make_pair(tri.c, tri.a) : make_pair(tri.a, tri.c );

    // Check if the edge exists in previously processed triangle.
    EdgeTriangleMap::iterator edge0_i = edges.find( edge0 );
    EdgeTriangleMap::iterator edge1_i = edges.find( edge1 );
    EdgeTriangleMap::iterator edge2_i = edges.find( edge2 );

    if( edge0_i != edges.end() ) {
      // shared edge found, update the neighbour array.
      int triangle = (*edge0_i).second.first;
      int edge =  (*edge0_i).second.second;
      neighbours[i*3] = triangle;
      if( neighbours[triangle*3+edge] == -1 ) {
         neighbours[triangle*3+edge] = i;
      }
    } else {
      edges[edge0] = make_pair(i, 0);
    }

   if( edge1_i != edges.end() ) {
      // shared edge found, update the neighbour array.
      int triangle = (*edge1_i).second.first;
      int edge =  (*edge1_i).second.second;
      neighbours[i*3+1] = triangle;
      if( neighbours[triangle*3+edge] == -1 ) {
         neighbours[triangle*3+edge] = i;
      }
    } else {
      edges[edge1] = make_pair(i, 1);
    }

   if( edge2_i != edges.end() ) {
     // shared edge found, update the neighbour array.
      int triangle = (*edge2_i).second.first;
      int edge =  (*edge2_i).second.second;
      neighbours[i*3+2] = triangle;
      if( neighbours[triangle*3+edge] == -1 ) {
        neighbours[triangle*3+edge] = i;
      }
    } else {
      edges[edge2] = make_pair(i, 2);
    }
  }
}


void ShadowGeometry::SFGeometryNode::onAdd( Node *n ) {
  TypedSFNode< X3DGeometryNode >::onAdd( n );
  X3DGeometryNode*geom = dynamic_cast< X3DGeometryNode * >( n );
  ShadowGeometry*shadow_geom = dynamic_cast< ShadowGeometry * >( getOwner() );
  if( geom ) {
    geom->boundTree->route( shadow_geom->triangles_changed );
  }
}

void ShadowGeometry::SFGeometryNode::onRemove( Node *n ) {
  X3DGeometryNode*geom = dynamic_cast< X3DGeometryNode * >( n );
  ShadowGeometry*shadow_geom = dynamic_cast< ShadowGeometry * >( getOwner() );
  if( geom ) {
    geom->boundTree->unroute( shadow_geom->triangles_changed );
  }
  TypedSFNode< X3DGeometryNode >::onRemove( n );
}


void ShadowGeometry::renderShadowGeometryShader( X3DGeometryNode *g,
                                                 X3DLightNode *light, 
                                                 bool draw_caps,
                                                 const Matrix4f &m,
                                                 const Matrix4f &m_inv,
                                                 bool rebuild_triangle_info) {



  // update the vertex array and indices for the geometry
  if( rebuild_triangle_info) {
    ShadowCasterShaders::shaderToggle( false );
    triangles.clear();
    g->boundTree->getValue()->getAllTriangles( triangles );
    updateAdjacenctVertexArray( triangles, triangle_points_geom_shader, index_geom_shader );
    ShadowCasterShaders::shaderToggle( true );
  }

  // draw shadow geometry using vertex arrays
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0,
                  &(*triangle_points_geom_shader.begin()) );
  glDrawElements( GL_TRIANGLES_ADJACENCY_EXT,
                  (unsigned int)index_geom_shader.size(),
                  GL_UNSIGNED_INT,
                  &(*(index_geom_shader.begin()) ) );
  
  glDisableClientState(GL_VERTEX_ARRAY);
}


void ShadowGeometry::updateAdjacenctVertexArray( const vector< HAPI::Collision::Triangle > &_triangles,
                                                 vector< Vec3d > &_triangle_points, 
                                                 vector< unsigned int > &_adjacency_index ) {
  updateNeighbours( _triangles );
  
  _triangle_points.clear();
  _triangle_points.reserve( _triangles.size() * 3 );
  _adjacency_index.clear();
  _adjacency_index.reserve( _triangles.size() * 6 );

  for( unsigned int i = 0; i < _triangles.size(); ++i ) {

    _triangle_points.push_back( _triangles[i].a );
    _triangle_points.push_back( _triangles[i].b );
    _triangle_points.push_back( _triangles[i].c );

        // if( triangles[i].a == triangles[i].b ||
        //      triangles[i].b == triangles[i].c ||
        //      triangles[i].c == triangles[i].a ) { Console(4) << "Degenerate" << endl; continue; }
    //    cerr << triangles[i].a << endl;

    //    _adjacency_index.push_back( i*3 );
    //    _adjacency_index.push_back( i*3 +1);
    //    _adjacency_index.push_back( i*3 +2);
    //    continue;
    // Triangle adjacency specification order
    // 1 - - - 2 - - - 3    
    //          ^\            
    //    \     | \     |      
    //          |  \           
    //      \   |   \   |       
    //          |    \        
    //        \ |     \ |      
    //          |      v          
    //          0<------4           
    //
    //            \     |          
    //
    //              \   |              
    //
    //                \ |             
    //
    //                 5    

    _adjacency_index.push_back( i*3 );

    // find neighbour triangle vertex index
    int n0_index = neighbours[ i*3 ];
    if( n0_index == -1 ) {
      // no neighbour so just repeat the vertex
      //Console(4) << "No neighbour" << endl;
      _adjacency_index.push_back( i*3 );
    } else {
      const HAPI::Collision::Triangle &n0 = _triangles[n0_index];
      _adjacency_index.push_back( n0_index*3 + 
                                 getMissingPointIndex( n0,
                                                       _triangles[i].a,
                                                       _triangles[i].b ) );
    }

    _adjacency_index.push_back( i*3+1 );

    // find neighbour triangle vertex index
    int n1_index = neighbours[ i*3 + 1];
    if( n1_index == -1 ) {
      // no neighbour so just repeat the vertex
      // Console(4) << "No neighbour" << endl;
      _adjacency_index.push_back( i*3 + 1 );
    } else {
      const HAPI::Collision::Triangle &n1 = _triangles[n1_index];
      _adjacency_index.push_back( n1_index*3 + 
                                 getMissingPointIndex( n1,
                                                       _triangles[i].b,
                                                       _triangles[i].c ) );
    }

    _adjacency_index.push_back( i*3+2 );

    // find neighbour triangle vertex index
    int n2_index = neighbours[ i*3 + 2];
    if( n2_index == -1 ) {
      // no neighbour so just repeat the vertex
      // Console(4) << "No neighbour" << endl;
      _adjacency_index.push_back( i*3 + 2 );
    } else {
      const HAPI::Collision::Triangle &n2 = _triangles[n2_index];
      _adjacency_index.push_back( n2_index*3 + 
                                 getMissingPointIndex( n2,
                                                       _triangles[i].c,
                                                       _triangles[i].a ) );
    }
  }
}


int ShadowGeometry::getMissingPointIndex( const HAPI::Collision::Triangle  &t,
                                          const Vec3d &p0, 
                                          const Vec3d &p1 ) {
  if( t.a != p0 && t.a != p1 ) return 0;
  if( t.b != p0 && t.b != p1 ) return 1;
  if( t.c != p0 && t.c != p1 ) return 2;
  return -1;
}


