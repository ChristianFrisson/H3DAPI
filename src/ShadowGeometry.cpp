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
/// \file ShadowGeometry.cpp
/// \brief CPP file for ShadowGeometry
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShadowGeometry.h>

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
                                Inst< SFGeometryNode > _geometry ) :
  H3DShadowObjectNode( _metadata, _transform ),
  geometry( _geometry ),
  triangles_changed( new Field ) {

  type_name = "ShadowGeometry";
  database.initFields( this );

}

// The shadow geometry does not use the exact same rendering function
// as the original geometry, since it needs to render triangles facing the
// light differently from the ones facing away from the light.
// In order to do this the boundTree triangles of the geometry are used.
// These triangles have been collected with a HAPI::FeedbackBufferCollector
// using OpenGL. Somewhere in this process, possibly in the transformation
// from window coordinates to world coordinates(model view matrix seems ok),
// the original coordinate used in rendering differs from the coordinate
// in the boundTree by some epsilon value due to floating point inaccuracies.
// This means that when drawing caps for ZFAIL algorithm, we can get a case
// of z-fighting between the original geometry and the shadow geometry,
// making the geometry flicker.
// 
// In order to avoid this we must try to render the shadow geometry so that
// it is not closer to the light than the original geometry at any time.
// We try to do this by:
// - Scaling the shadow geometry down 
// - Moving the shadow geometry along the light ray
//
// This can probably be done properly by using a geometry shader for 
// graphics cards that support them, rendering the original geometry
// and using the geometry shader to generate the shadow geometry.


// The scale factor to use to scale down the shadow geometry.
const double scale_factor = 0.999;

// The offset to move silhouetteedges in the light ray direction.
const double offset_dir_light =1e-4;

void ShadowGeometry::renderDirectionalLightQuad( const Vec3d &v1, 
                                                 const Vec3d &v2, 
                                                 const Vec3d& dir ) {
  Vec3d epsilon = offset_dir_light * dir;
  Vec3d ev1 = v1 + epsilon;
  Vec3d ev2 = v2 + epsilon;

  glVertex3d( ev1.x, ev1.y, ev1.z );
  glVertex4d( dir.x, dir.y, dir.z, 0 );
  glVertex4d( dir.x, dir.y, dir.z, 0 );
  glVertex3d( ev2.x, ev2.y, ev2.z );
}

void ShadowGeometry::renderPointLightQuad( const Vec3d &v1, const Vec3d &v2, 
                                           const Vec3d& light_pos ) {
  Vec3d dir1 = v1 - light_pos;
  dir1.normalizeSafe();
  
  Vec3d dir2 = v2 - light_pos;
  dir2.normalizeSafe();
  
  Vec3d ev1 = v1 + dir1 * offset_dir_light;
  Vec3d ev2 = v2 + dir2 * offset_dir_light;

  glVertex3d( ev1.x, ev1.y, ev1.z );
  glVertex4d( dir1.x, dir1.y, dir1.z, 0 );
  glVertex4d( dir2.x, dir2.y, dir2.z, 0 );
  glVertex3d( ev2.x, ev2.y, ev2.z );
 
}

void ShadowGeometry::renderShadow( X3DLightNode *light, 
                              	   bool draw_caps,
                                   const Matrix4f &local_to_global ) {
  
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

  if(!triangles_changed->isUpToDate() ) {
    triangles_changed->upToDate();
    triangles.clear();
    g->boundTree->getValue()->getAllTriangles( triangles );
    updateNeighbours( triangles );
  }

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();

  GLfloat mv[] = { 
    m[0][0], m[1][0], m[2][0], 0,
    m[0][1], m[1][1], m[2][1], 0,
    m[0][2], m[1][2], m[2][2], 0,
    m[0][3], m[1][3], m[2][3], 1 };

  glMultMatrixf( mv );

  if( draw_caps ) {
    // if drawing caps make the geometry a little smaller to avoid z-buffer
    // issues since both the shadow volume and geometry itself would be 
    // drawn in the same place, causing flickering.
    GLfloat scale = scale_factor;
    glScalef( scale, scale, scale );
  }

  // draw quads for each silhouette edge and its projection at infinity.
  glBegin( GL_QUADS );

  // directional light
  DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );     
  if( dir_light ) {
    Vec3f dir = dir_light->direction->getValue();
    dir = m_inv.getRotationPart() * dir;
    updateSilhouetteEdgesDirectionalLight( triangles, neighbours, dir );
    for( size_t i = 0; i < triangles.size(); i++ ) {
      // no silhouette edges are on triangles not facing the light
      if( !triangle_facing_light[i] ) continue;
      if( is_silhouette_edge[ i*3 ] ) 
        renderDirectionalLightQuad( triangles[i].a, triangles[i].b, dir );
      if( is_silhouette_edge[ i*3+1 ] ) 
        renderDirectionalLightQuad( triangles[i].b, triangles[i].c, dir );
      if( is_silhouette_edge[ i*3 +2] ) 
        renderDirectionalLightQuad( triangles[i].c, triangles[i].a, dir );
    }
  }

  // point light
  PointLight *point_light = dynamic_cast< PointLight * >( light );
  Vec3f light_pos;

  if( point_light ) {
    light_pos = point_light->location->getValue();
    light_pos = m_inv * light_pos;
    updateSilhouetteEdgesPointLight( triangles, neighbours, light_pos );
    for( size_t i = 0; i < triangles.size(); i++ ) {
      // no silhouette edges are on triangles not facing the light
      if( !triangle_facing_light[i] ) continue;
      if( is_silhouette_edge[ i*3 ] ) renderPointLightQuad( triangles[i].a, triangles[i].b, light_pos );
      if( is_silhouette_edge[ i*3+1 ] ) renderPointLightQuad( triangles[i].b, triangles[i].c, light_pos );
      if( is_silhouette_edge[ i*3 +2] ) renderPointLightQuad( triangles[i].c, triangles[i].a, light_pos );
    }
  }

  glEnd();

  if( draw_caps ) {
    // draw all triangles facing the light as a near cap and all others
    // at infinity.
    glBegin(GL_TRIANGLES);
    
    if( dir_light ) {
      Vec3d dir = dir_light->direction->getValue();
      dir = m_inv.getRotationPart() * dir;
      
      for( size_t i = 0; i < triangles.size(); i++ ) {
        if( triangle_facing_light[i] ) {
          Vec3d v1 = triangles[i].a + offset_dir_light * dir;
          Vec3d v2 = triangles[i].b + offset_dir_light * dir;
          Vec3d v3 = triangles[i].c + offset_dir_light * dir;       
          glVertex3d( v1.x, v1.y, v1.z);
          glVertex3d( v2.x, v2.y, v2.z );
          glVertex3d( v3.x, v3.y, v3.z );
        }
        // directional lights do not need a far cap since the shadow volume
        // converge to the same point at infinity.
      }
    } else if( point_light ) {
      for( size_t i = 0; i < triangles.size(); i++ ) {
        if( triangle_facing_light[i] ) {
          Vec3d dir1 = triangles[i].a - light_pos;
          Vec3d dir2 = triangles[i].b - light_pos;
          Vec3d dir3 = triangles[i].c - light_pos;
          
          dir1.normalizeSafe();
          dir2.normalizeSafe();
          dir3.normalizeSafe();
          
          Vec3d v1 = triangles[i].a + offset_dir_light * dir1;
          Vec3d v2 = triangles[i].b + offset_dir_light * dir2;
          Vec3d v3 = triangles[i].c + offset_dir_light * dir3;
          
          glVertex3d( v1.x, v1.y, v1.z);
          glVertex3d( v2.x, v2.y, v2.z );
          glVertex3d( v3.x, v3.y, v3.z );
        } else {
          const Vec3f &p = light_pos;
          Vec3d v1 = triangles[i].a - p;
          Vec3d v2 = triangles[i].b - p;
          Vec3d v3 = triangles[i].c - p;
          
          glVertex4d(	v1.x, v1.y, v1.z, 0 );
          glVertex4d(	v2.x, v2.y, v2.z, 0 );
          glVertex4d(	v3.x, v3.y, v3.z, 0 );
        }
      }
    }
    glEnd();
  }
  glPopMatrix();
}

void ShadowGeometry::updateSilhouetteEdgesDirectionalLight( const vector< HAPI::Collision::Triangle > &triangles,
                                                            const vector<int> &neighbours,
                                                            const Vec3d &direction ) {

  triangle_facing_light.resize( triangles.size(), false );
  is_silhouette_edge.resize( triangles.size()*3, false );
  
  for( size_t i = 0; i < triangles.size(); i++ ) {
    triangle_facing_light[i] = direction.dotProduct( triangles[i].normal ) <= 0;
  }
 
  for( size_t i = 0; i < neighbours.size(); i++ ) {
    if(!triangle_facing_light[i/3]) {
      // silhouette edges are only on triangles facing the light
      is_silhouette_edge[i] = false;
    } else {
      // silhouette if no neighbour or the neighbour is not facing the light
      is_silhouette_edge[i] = neighbours[i] == -1 || !triangle_facing_light[neighbours[i]];
    }
  }
}


void ShadowGeometry::updateSilhouetteEdgesPointLight( const vector< HAPI::Collision::Triangle > &triangles,
						      const vector<int> &neighbours,
                                                      const Vec3d &pos ) {
  triangle_facing_light.resize( triangles.size(), false );
  is_silhouette_edge.resize( triangles.size()*3, false );

  for( size_t i = 0; i < triangles.size(); i++ ) {
    Vec3d direction = triangles[i].a - pos;
    triangle_facing_light[i] = direction.dotProduct( triangles[i].normal ) <= 0;
  }

  for( size_t i = 0; i < neighbours.size(); i++ ) {
    if(!triangle_facing_light[i/3]) {
      // silhouette edges are only on triangles facing the light
      is_silhouette_edge[i] = false;
    } else {
      // silhouette if no neighbour or the neighbour is not facing the light
      is_silhouette_edge[i] = neighbours[i] == -1 || !triangle_facing_light[neighbours[i]];
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
            !(_Right.first < _Left.first) && _Left.second < _Right.second);
  }
};

void ShadowGeometry::updateNeighbours( const vector< HAPI::Collision::Triangle > &triangles ) {

  neighbours.resize( triangles.size()*3, -1 );

  // map from triangle edge(as pair of vertex) to pair of 
  // (triangle index, edge index within triangle) 
  typedef map< pair< Vec3d, Vec3d >, pair<int, int>, lt >  EdgeTriangleMap;
  EdgeTriangleMap edges;
 
  for( size_t i = 0; i < triangles.size(); i++ ) {
    const HAPI::Collision::Triangle &tri = triangles[i];

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
