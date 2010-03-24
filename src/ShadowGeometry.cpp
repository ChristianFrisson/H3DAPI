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
  FIELDDB_ELEMENT( ShadowGeometry, transform, INPUT_OUTPUT );
}

ShadowGeometry::ShadowGeometry( Inst< SFNode>  _metadata,
                                Inst< SFTransformNode > _transform,
                                Inst< SFGeometryNode > _geometry ) :
  H3DShadowObjectNode( _metadata, _transform ),
  geometry( _geometry ) {

  type_name = "ShadowGeometry";
  database.initFields( this );

}


void ShadowGeometry::renderDirectionalLightQuad( const Vec3d &v1, 
                                                 const Vec3d &v2, 
                                                 const Vec3d& dir ) {
  glVertex3d( v1.x, v1.y, v1.z );
  glVertex4d( dir.x, dir.y, dir.z, 0 );
  glVertex4d( dir.x, dir.y, dir.z, 0 );
  glVertex3d( v2.x, v2.y, v2.z );
}

void ShadowGeometry::renderPointLightQuad( const Vec3d &v1, const Vec3d &v2, 
                                           const Vec3d& light_pos ) {
  Vec3d dir1 = v1 - light_pos;
  dir1.normalizeSafe();
  
  Vec3d dir2 = v2 - light_pos;
  dir2.normalizeSafe();
  
  glVertex3d( v1.x, v1.y, v1.z );
  glVertex4d( dir1.x, dir1.y, dir1.z, 0 );
  glVertex4d( dir2.x, dir2.y, dir2.z, 0 );
  glVertex3d( v2.x, v2.y, v2.z );
 
}

void ShadowGeometry::renderShadow( X3DLightNode *light, bool draw_caps ) {
  
  MatrixTransform *t = transform->getValue();
  X3DGeometryNode *g = geometry->getValue();
  
  if( !g ) return;

  Matrix4f m, m_inv;
  if(t) {
    m = t->matrix->getValue();
    m_inv = m.inverse();
  }

  // get all triangles from the geometry
  vector< HAPI::Collision::Triangle > triangles;
  const vector<int> &neighbours = g->boundTree->getValue()->getNeighbours();
  triangles.reserve( neighbours.size() / 3 );
  g->boundTree->getValue()->getAllTriangles( triangles );


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
    GLfloat scale = 0.999f;
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
	  for( size_t i = 0; i < triangles.size(); i++ ) {
      if( triangle_facing_light[i] ) {
        Vec3d v1 = triangles[i].a;
        Vec3d v2 = triangles[i].b;
        Vec3d v3 = triangles[i].c;       
        glVertex3d(	v1.x, v1.y, v1.z);
        glVertex3d(	v2.x, v2.y, v2.z );
        glVertex3d(	v3.x, v3.y, v3.z );
      } else {
        // directional lights do not need a far cap since the shadow volume
        // converge to the same point at infinity.

        if( point_light ) {
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


void ShadowGeometry::SFGeometryNode::onAdd( Node *n ) {
  TypedSFNode< X3DGeometryNode >::onAdd( n );
  X3DGeometryNode*geom = dynamic_cast< X3DGeometryNode * >( n );
  ShadowGeometry*shadow_geom = dynamic_cast< ShadowGeometry * >( getOwner() );
  if( geom ) {
    //    geom->boundTree->route( shadow_geom->triangles_changed );
  }
}

void ShadowGeometry::SFGeometryNode::onRemove( Node *n ) {
  X3DGeometryNode*geom = dynamic_cast< X3DGeometryNode * >( n );
  ShadowGeometry*shadow_geom = dynamic_cast< ShadowGeometry * >( getOwner() );
  if( geom ) {
    //    geom->boundTree->unroute( shadow_geom->triangles_changed );
  }
  TypedSFNode< X3DGeometryNode >::onRemove( n );
}
