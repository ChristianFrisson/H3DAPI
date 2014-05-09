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
  triangles_changed( new Field ),
  modelMatrix( new SFMatrix4f ),
  lightParam( new SFVec3f ),
  drawCaps( new SFBool ),
  use_geometry_shader_last_loop( false ) {

  type_name = "ShadowGeometry";
  database.initFields( this );

}



void ShadowGeometry::renderDirectionalLightQuad( const Vec3d &v1, 
                                                 const Vec3d &v2, 
                                                 const Vec3d& dir ) {
  Vec3d ev1 = v1;
  Vec3d ev2 = v2;

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
  
  Vec3d ev1 = v1;
  Vec3d ev2 = v2;

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

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();

  GLfloat mv[] = { 
    m[0][0], m[1][0], m[2][0], 0,
    m[0][1], m[1][1], m[2][1], 0,
    m[0][2], m[1][2], m[2][2], 0,
    m[0][3], m[1][3], m[2][3], 1 };

  glMultMatrixf( mv );

  // draw quads for each silhouette edge and its projection at infinity.
  glBegin( GL_QUADS );

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
    for( size_t i = 0; i < triangles.size(); ++i ) {
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
      
      for( size_t i = 0; i < triangles.size(); ++i ) {
        if( triangle_facing_light[i] ) {
          Vec3d v1 = triangles[i].a;
          Vec3d v2 = triangles[i].b;
          Vec3d v3 = triangles[i].c;
          glVertex3d( v1.x, v1.y, v1.z);
          glVertex3d( v2.x, v2.y, v2.z );
          glVertex3d( v3.x, v3.y, v3.z );
        }
        // directional lights do not need a far cap since the shadow volume
        // converge to the same point at infinity.
      }
    } else if( point_light ) {
      for( size_t i = 0; i < triangles.size(); ++i ) {
        if( triangle_facing_light[i] ) {
          Vec3d v1 = triangles[i].a;
          Vec3d v2 = triangles[i].b;
          Vec3d v3 = triangles[i].c;
          
          glVertex3d( v1.x, v1.y, v1.z);
          glVertex3d( v2.x, v2.y, v2.z );
          glVertex3d( v3.x, v3.y, v3.z );
        } else {
          const Vec3f &p = light_pos;
          Vec3d v1 = triangles[i].a - p;
          Vec3d v2 = triangles[i].b - p;
          Vec3d v3 = triangles[i].c - p;
          
          glVertex4d(   v1.x, v1.y, v1.z, 0 );
          glVertex4d(   v2.x, v2.y, v2.z, 0 );
          glVertex4d(   v3.x, v3.y, v3.z, 0 );
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
  
  for( size_t i = 0; i < triangles.size(); ++i ) {
    triangle_facing_light[i] = direction.dotProduct( triangles[i].normal ) <= 0;
  }
 
  for( size_t i = 0; i < neighbours.size(); ++i ) {
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

  for( size_t i = 0; i < triangles.size(); ++i ) {
    Vec3d direction = triangles[i].a - pos;
    triangle_facing_light[i] = direction.dotProduct( triangles[i].normal ) <= 0;
  }

  for( size_t i = 0; i < neighbours.size(); ++i ) {
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
            (!(_Right.first < _Left.first) && _Left.second < _Right.second));
  }
};

void ShadowGeometry::updateNeighbours( const vector< HAPI::Collision::Triangle > &triangles ) {

  neighbours.resize( triangles.size()*3, -1 );

  // map from triangle edge(as pair of vertex) to pair of 
  // (triangle index, edge index within triangle) 
  typedef map< pair< Vec3d, Vec3d >, pair<int, int>, lt >  EdgeTriangleMap;
  EdgeTriangleMap edges;
 
  for( unsigned int i = 0; i < triangles.size(); ++i ) {
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


const char *vertex_shader_string = 
"glsl:\n"
"uniform mat4 modelMatrix; \n"
"void main() {\n"
"  gl_Position = gl_ModelViewMatrix * modelMatrix * gl_Vertex;\n"
"\n"
"} \n";

const char *fragment_shader_string = 
"glsl:\n"
"void main() {\n"
"  gl_FragColor = vec4( vec3(gl_LightSource[0].position), 1.0 ); \n"
"} \n";


void ShadowGeometry::renderShadowGeometryShader( X3DGeometryNode *g,
                                                 X3DLightNode *light, 
                                                 bool draw_caps,
                                                 const Matrix4f &m,
                                                 const Matrix4f &m_inv,
                                                 bool rebuild_triangle_info) {

  PointLight *point_light = dynamic_cast< PointLight * >( light );
  DirectionalLight *dir_light = dynamic_cast< DirectionalLight * >( light );

  // initialize shader
  if( point_light && !point_light_shader.get() ) {
    point_light_shader.reset( new ComposedShader );

    point_light_shader->geometryInputType->setValue( "TRIANGLES_ADJACENCY" );
    point_light_shader->geometryOutputType->setValue( "TRIANGLE_STRIP" );

    // add field for uniform variable lightPosition.
    Field *lightPos = new SFVec3f;
    lightParam->route( lightPos );
    point_light_shader->addField( "lightPosition", Field::INPUT_OUTPUT, lightPos );

    Field *model_matrix = new SFMatrix4f;
    modelMatrix->route( model_matrix );
    point_light_shader->addField( "modelMatrix", 
                                  Field::INPUT_OUTPUT, model_matrix );

    Field *drawCapsField = new SFBool;
    drawCaps->route( drawCapsField );
    point_light_shader->addField( "drawCaps", Field::INPUT_OUTPUT, drawCapsField );

    // vertex shader
    ShaderPart *vertex_shader = new ShaderPart;
    vertex_shader->type->setValue( "VERTEX" );
    vertex_shader->url->push_back( vertex_shader_string );
    point_light_shader->parts->push_back( vertex_shader );

    // geometry shader
    ShaderPart *geom_shader = new ShaderPart;
    geom_shader->type->setValue( "GEOMETRY" );
    geom_shader->url->push_back( getPointLightGeometryShaderString() );
    point_light_shader->parts->push_back( geom_shader );

    // fragment shader
    ShaderPart *fragment_shader = new ShaderPart;
    fragment_shader->type->setValue( "FRAGMENT" );
    fragment_shader->url->push_back( fragment_shader_string );
    point_light_shader->parts->push_back( fragment_shader );  
  }

  if( dir_light && !dir_light_shader.get() ) {
    dir_light_shader.reset( new ComposedShader );

    dir_light_shader->geometryInputType->setValue( "TRIANGLES_ADJACENCY" );
    dir_light_shader->geometryOutputType->setValue( "TRIANGLE_STRIP" );

    // add field for uniform variable lightDirection.
    Field *lightDir = new SFVec3f;
    lightParam->route( lightDir );
    dir_light_shader->addField( "lightDirection", Field::INPUT_OUTPUT, lightDir );

    Field *model_matrix = new SFMatrix4f;
    modelMatrix->route( model_matrix );
    dir_light_shader->addField( "modelMatrix", 
                                Field::INPUT_OUTPUT, model_matrix );


    Field *drawCapsField = new SFBool;
    drawCaps->route( drawCapsField );
    dir_light_shader->addField( "drawCaps", Field::INPUT_OUTPUT, drawCapsField );

    // vertex shader
    ShaderPart *vertex_shader = new ShaderPart;
    vertex_shader->type->setValue( "VERTEX" );
    vertex_shader->url->push_back( vertex_shader_string );
    dir_light_shader->parts->push_back( vertex_shader );

    // geometry shader
    ShaderPart *geom_shader = new ShaderPart;
    geom_shader->type->setValue( "GEOMETRY" );
    geom_shader->url->push_back( getDirLightGeometryShaderString() );
    dir_light_shader->parts->push_back( geom_shader );

    // fragment shader
    ShaderPart *fragment_shader = new ShaderPart;
    fragment_shader->type->setValue( "FRAGMENT" );
    fragment_shader->url->push_back( fragment_shader_string );
    dir_light_shader->parts->push_back( fragment_shader );  
  }

  // update the vertex array and indices for the geometry
  if( rebuild_triangle_info) {
    triangles.clear();
    g->boundTree->getValue()->getAllTriangles( triangles );
    updateAdjacenctVertexArray( triangles, triangle_points, adjacency_index );
  }

  modelMatrix->setValue( m );
  drawCaps->setValue( draw_caps );

  // enable appropriate shader
  if( point_light ) {
    lightParam->setValue( point_light->location->getValue() );
    point_light_shader->preRender();
    point_light_shader->displayList->callList();
  } else if( dir_light ) {
    lightParam->setValue( dir_light->direction->getValue() );
    dir_light_shader->preRender();
    dir_light_shader->displayList->callList();
  }

  // draw shadow geometry using vertex arrays
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0,
                  &(*triangle_points.begin()) );
  glDrawElements( GL_TRIANGLES_ADJACENCY_EXT,
                  (unsigned int)adjacency_index.size(),
                  GL_UNSIGNED_INT,
                  &(*(adjacency_index.begin()) ) );
  
  glDisableClientState(GL_VERTEX_ARRAY);
  
  // disable shaders
  if( point_light ) {
    point_light_shader->postRender();
  } else if( dir_light ) {
    dir_light_shader->postRender();
  }
}


void ShadowGeometry::updateAdjacenctVertexArray( const vector< HAPI::Collision::Triangle > &triangles, 
                                                 vector< Vec3d > &triangle_points, 
                                                 vector< unsigned int > &adjacency_index ) {
  updateNeighbours( triangles );
  
  triangle_points.clear();
  triangle_points.reserve( triangles.size() * 3 );
  adjacency_index.clear();
  adjacency_index.reserve( triangles.size() * 6 );

  for( unsigned int i = 0; i < triangles.size(); ++i ) {
   
        triangle_points.push_back( triangles[i].a );
    triangle_points.push_back( triangles[i].b );
    triangle_points.push_back( triangles[i].c );

        // if( triangles[i].a == triangles[i].b ||
        //      triangles[i].b == triangles[i].c ||
        //      triangles[i].c == triangles[i].a ) { Console(4) << "Degenerate" << endl; continue; }
    //    cerr << triangles[i].a << endl;

    //    adjacency_index.push_back( i*3 );
    //    adjacency_index.push_back( i*3 +1);
    //    adjacency_index.push_back( i*3 +2);
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

    adjacency_index.push_back( i*3 );

    // find neighbour triangle vertex index
    int n0_index = neighbours[ i*3 ];
    if( n0_index == -1 ) {
      // no neighbour so just repeat the vertex
      //Console(4) << "No neighbour" << endl;
      adjacency_index.push_back( i*3 );
    } else {
      const HAPI::Collision::Triangle &n0 = triangles[n0_index];
      adjacency_index.push_back( n0_index*3 + 
                                 getMissingPointIndex( n0,
                                                       triangles[i].a,
                                                       triangles[i].b ) );
    }

    adjacency_index.push_back( i*3+1 );

    // find neighbour triangle vertex index
    int n1_index = neighbours[ i*3 + 1];
    if( n1_index == -1 ) {
      // no neighbour so just repeat the vertex
      // Console(4) << "No neighbour" << endl;
      adjacency_index.push_back( i*3 + 1 );
    } else {
      const HAPI::Collision::Triangle &n1 = triangles[n1_index];
      adjacency_index.push_back( n1_index*3 + 
                                 getMissingPointIndex( n1,
                                                       triangles[i].b,
                                                       triangles[i].c ) );
    }

    adjacency_index.push_back( i*3+2 );

    // find neighbour triangle vertex index
    int n2_index = neighbours[ i*3 + 2];
    if( n2_index == -1 ) {
      // no neighbour so just repeat the vertex
      // Console(4) << "No neighbour" << endl;
      adjacency_index.push_back( i*3 + 2 );
    } else {
      const HAPI::Collision::Triangle &n2 = triangles[n2_index];
      adjacency_index.push_back( n2_index*3 + 
                                 getMissingPointIndex( n2,
                                                       triangles[i].c,
                                                       triangles[i].a ) );
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


const char *geometry_shader_functions_string = 
  "#version 120 \n"
  "#extension GL_EXT_geometry_shader4 : enable\n"
  "\n"
  "// returns true if the incoming triangle does not have an area \n"
  "bool isTriangleDegenerate() { \n"
  "  return( gl_PositionIn[0] == gl_PositionIn[2] || \n" 
  "          gl_PositionIn[2] == gl_PositionIn[4] || \n"
  "          gl_PositionIn[4] == gl_PositionIn[0] ); \n"
  "} \n"
  "\n"
  "float facing_epsilon = -0.00001f;\n"
  "\n"
  "// returns true if pos is on the same side of the triangle as the normal is pointing\n"
  "bool triangleFacingPos(vec3 v0, vec3 v1, vec3 v2, vec3 pos) {\n"
  "  // Normalizing is required in order to avoid some incorrectly calculated shadow triangles.\n"
  "  vec3 e0 = normalize( v1 - v0 );\n"
  "  vec3 e1 = normalize( v2 - v0 );\n"
  "  vec3 normal = cross(e1, e0);\n"
  "  return dot(normal, normalize( pos-v0 )) <= facing_epsilon;\n"
  "}\n"
  "\n"
  "// returns true if dir points in the opposite direction of the triangle normal\n"
  "bool triangleFacingDir(vec3 v0, vec3 v1, vec3 v2, vec3 dir) {\n"
  "  // Normalizing is required in order to avoid some incorrectly calculated shadow triangles. Yes it slows things down a bit, but otherwise shadows can actually be missing.\n"
  "  vec3 e0 = normalize( v1 - v0 );\n"
  "  vec3 e1 = normalize( v2 - v0 );\n"
  "  vec3 normal = normalize( cross(e1, e0) );\n"
  "  return dot(normal, normalize(-dir)) <= facing_epsilon;\n"
  "}\n"
  "\n"
  "// given a silhouette edge(v0, v1) and a point light position a quad is drawn \n"
  "// from the edge to its projection at infinity\n"
  "// all input in normalized device coordinates.\n"
  "void edgeToInfinityGeometryPointLight( vec3 v0, vec3 v1, vec3 light_pos ) {\n"
  "  vec3 dir_v0 = v0 - light_pos;\n"
  "  vec3 dir_v1 = v1 - light_pos;\n"
  "  \n"
  "  gl_Position = gl_ProjectionMatrix * vec4( v1, 1.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( v0, 1.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( dir_v1, 0.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( dir_v0, 0.0 );\n"
  "  EmitVertex();\n"
  "  EndPrimitive();\n"
  "}\n"
  "\n"
  "// given a silhouette edge(v0, v1) and a directional light direction a quad is drawn \n"
  "// from the edge to its projection at infinity.\n"
  "// All input in view coordinates\n"
  "void edgeToInfinityGeometryDirLight( vec3 v0, vec3 v1, vec3 light_dir ) {\n"
  "  gl_Position = gl_ProjectionMatrix * vec4(v1, 1.0);\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4(v0, 1.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( light_dir, 0.0 );\n"
  "  EmitVertex();\n"
  "  gl_Position = gl_ProjectionMatrix * vec4( light_dir, 0.0 );\n"
  "  EmitVertex();\n"
  "  EndPrimitive();\n"
  "}\n"
  "\n"
  "// light_pos_view is light position in view coordinates\n"
  "void mainPointLight( vec4 light_pos_view, bool draw_caps ) {\n"
  "  vec3 light_pos = light_pos_view.xyz;\n"
  "  \n"
  "  if( triangleFacingPos( gl_PositionIn[0].xyz, \n"
  "                         gl_PositionIn[2].xyz, \n"
  "                         gl_PositionIn[4].xyz,\n"
  "                         light_pos ) ) {\n"
  "    // triangle facing light\n"
  "\n"
  "    if( draw_caps ) {\n"
  "      // render the triangle itself\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[0];\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[2];\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[4];\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "\n"
  "    // if a silhouette edge, render edge to infinity.\n"
  "\n"
  "    // edge 0 silhouette edge\n"
  "    if( gl_PositionIn[0] == gl_PositionIn[1] || \n"
  "        !triangleFacingPos( gl_PositionIn[0].xyz, \n"
  "                            gl_PositionIn[1].xyz, \n"
  "                            gl_PositionIn[2].xyz,\n"
  "                            light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( gl_PositionIn[0].xyz, \n"
  "                                        gl_PositionIn[2].xyz, \n"
  "                                        light_pos );\n"
  "    }\n"
  "\n"
  "    // edge 1 silhouette edge\n"
  "    if( gl_PositionIn[2] == gl_PositionIn[3] || \n"
  "       !triangleFacingPos( gl_PositionIn[2].xyz, \n"
  "                           gl_PositionIn[3].xyz, \n"
  "                           gl_PositionIn[4].xyz,\n"
  "                           light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( gl_PositionIn[2].xyz, \n"
  "                                        gl_PositionIn[4].xyz, \n"
  "                                        light_pos );\n"
  "\n"
  "    }\n"
  "\n"
  "    // edge 2 silhouette edge\n"
  "    if( gl_PositionIn[4] == gl_PositionIn[5] || \n"
  "        !triangleFacingPos( gl_PositionIn[4].xyz, \n"
  "                            gl_PositionIn[5].xyz, \n"
  "                            gl_PositionIn[0].xyz,\n"
  "                            light_pos ) ) {\n"
  "      edgeToInfinityGeometryPointLight( gl_PositionIn[4].xyz, \n"
  "                                        gl_PositionIn[0].xyz, \n"
  "                                        light_pos );\n"
  "\n"
  "    }\n"
  "  } else {\n"
  "    if( draw_caps ) {\n"
  "      // triangle facing away from light.\n"
  "      vec3 dir_v0 = gl_PositionIn[0].xyz - light_pos;\n"
  "      vec3 dir_v1 = gl_PositionIn[2].xyz - light_pos;\n"
  "      vec3 dir_v2 = gl_PositionIn[4].xyz - light_pos;\n"
  "      \n"
  "      gl_Position = gl_ProjectionMatrix * vec4( dir_v0, 0.0 );\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * vec4( dir_v1, 0.0 );\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix * vec4( dir_v2, 0.0 );\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "  }\n"
  "}\n"
  "\n"
  "\n"
  "// light_dir is in view coordinates\n"
  "void mainDirectionalLight( vec3 light_dir, bool draw_caps ) {\n"
  "\n"
  "  // transforming to view space\n"
  "  vec3 p0 = gl_PositionIn[0].xyz;\n"
  "  vec3 p1 = gl_PositionIn[1].xyz;\n"
  "  vec3 p2 = gl_PositionIn[2].xyz;\n"
  "  vec3 p3 = gl_PositionIn[3].xyz;\n"
  "  vec3 p4 = gl_PositionIn[4].xyz;\n"
  "  vec3 p5 = gl_PositionIn[5].xyz;\n"
  "\n"
  "  // if a silhouette edge, render edge to infinity.\n"
  "  if( triangleFacingDir( p0, p2, p4, light_dir ) ) {\n"
  "\n"
  "    if( draw_caps ) {\n"
  "      // render the triangle itself(only when drawing caps)\n"
  "      // no back cap needed for directional lights since all points \n"
  "      // converge to the same point at infinity.\n"
  "      gl_Position = gl_ProjectionMatrix * gl_PositionIn[0];\n"
  "      EmitVertex();\n"
  "      gl_Position = gl_ProjectionMatrix *  gl_PositionIn[2];\n"
  "      EmitVertex();\n"
  "      gl_Position =  gl_ProjectionMatrix * gl_PositionIn[4];\n"
  "      EmitVertex();\n"
  "      EndPrimitive();\n"
  "    }\n"
  "   \n"
  "    // edge 0 silhouette edge\n"
  "    if( p0 == p1 || !triangleFacingDir( p0, p1, p2, light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p0, p2, light_dir );\n"
  "    }\n"
  "\n"
  "    // edge 1 silhouette edge\n"
  "    if( p2 == p3 || !triangleFacingDir( p2, p3, p4,light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p2, p4,light_dir );\n"
  "    }\n"
  "\n"
  "    // edge 2 silhouette edge\n"
  "    if( p4 == p5 || !triangleFacingDir( p4, p5, p0, light_dir ) ) {\n"
  "      edgeToInfinityGeometryDirLight( p4, p0, light_dir );\n"
  "    }\n"
  "  }\n"
  "}\n";



string ShadowGeometry::getPointLightGeometryShaderString() {
  stringstream s;
  s << "glsl:" << endl;
  s << geometry_shader_functions_string << endl;
  s << "uniform vec3 lightPosition; " << endl;
  s << "uniform bool drawCaps; " << endl;
  s << "void main() { " << endl;
  s << "  if( isTriangleDegenerate() ) return; " << endl;
  s << "  mainPointLight( gl_ModelViewMatrix * vec4( lightPosition, 1.0 ), drawCaps ); " << endl;
  s << "}" << endl;
  return s.str();
}

string ShadowGeometry::getDirLightGeometryShaderString() {
  stringstream s;
  s << "glsl:" << endl;
  s << geometry_shader_functions_string << endl;
  s << "uniform vec3 lightDirection; " << endl;
  s << "uniform bool drawCaps; " << endl;
  s << "void main() { " << endl;
  s << "  if( isTriangleDegenerate() ) return; " << endl;
  s << "  mainDirectionalLight( (gl_ModelViewMatrix * vec4( lightDirection, 0.0 )).xyz, drawCaps ); " << endl;
  s << "}" << endl;
  return s.str();
}
