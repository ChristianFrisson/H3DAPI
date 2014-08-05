//////////////////////////////////////////////////////////////////////////////
//    Copyright 2010, SenseGraphics AB and Moog
//
//    This file is part of DentalSim. You may not distribute this file without
//    explicit permission from the copyright holders.
//
//
/// \file Capsule.cpp
/// \brief CPP file for Capsule
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/Capsule.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Capsule::database( "Capsule", 
                                   &(newInstance<Capsule>), 
                                   typeid( Capsule ),
                                   &X3DGeometryNode::database );

namespace CapsuleInternals {
  FIELDDB_ELEMENT( Capsule, bound, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Capsule, bottom, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Capsule, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Capsule, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Capsule, side, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Capsule, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Capsule, top, INPUT_OUTPUT );
}

Capsule::Capsule( 
                 Inst< SFNode  >  _metadata,
                 Inst< SFBound > _bound,
                 Inst< SFBool  >  _bottom,
                 Inst< SFFloat >  _height,
                 Inst< SFFloat >  _radius,
                 Inst< SFBool  >  _side,
                 Inst< SFBool  >  _solid,
                 Inst< SFBool  >  _top ) :
  X3DGeometryNode( _metadata, _bound ),
  bottom  ( _bottom   ),
  height  ( _height   ),
  radius  ( _radius   ),
  side    ( _side     ),
  solid   ( _solid    ),
  top     ( _top      ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "Capsule";
  database.initFields( this );

  bottom->setValue( true );
  height->setValue(2);
  radius->setValue(1);
  side->setValue ( true );
  solid->setValue( true );
  top->setValue  ( true );

  bottom->route( displayList );
  height->route( displayList );
  radius->route( displayList );
  side->route( displayList );
  solid->route( displayList );
  top->route( displayList );

  height->route( bound );
  radius->route( bound );

  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  height->route( vboFieldsUpToDate );
  radius->route( vboFieldsUpToDate );
}

Capsule::~Capsule() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 2, vbo_id );
    delete [] vbo_id;
    vbo_id = NULL;
  }
}


void Capsule::render() { 
  
  X3DGeometryNode::render();     

  bool prefer_vertex_buffer_object = false;
  if( GLEW_ARB_vertex_buffer_object ) {
    GraphicsOptions * go = NULL;
    getOptionNode( go );
    if( !go ) {
      GlobalSettings * gs = GlobalSettings::getActive();
      if( gs ) {
        gs->getOptionNode( go );
      }
    }
    if( go ) {
      prefer_vertex_buffer_object = go->preferVertexBufferObject->getValue();
    }
  }

  const float l_radius = radius->getValue();
  const float l_height = height->getValue();
  const float half_height = l_height / 2;

  H3DFloat inc_theta = (H3DFloat) Constants::pi*2 / theta_parts;
  H3DFloat inc_phi =   (H3DFloat) (Constants::pi/2) /phi_parts;
  H3DFloat double_pi = (H3DFloat) Constants::pi * 2;
  unsigned int components_per_triangle = 3; // 3 vertices/triangle.
  
  if( prefer_vertex_buffer_object ) {
    if( !vboFieldsUpToDate->isUpToDate() ) {
      // Only transfer data when it has been modified.
      vboFieldsUpToDate->upToDate();
      unsigned int components_per_vertex = 9; // 9 floats/vertex.
      // 2 * theta_parts for side, then theta_parts for each cap
      // times components_per_vertex.
      GLsizei data_size =
        (GLsizei)( ( 2*theta_parts +  2*(phi_parts+1) *(theta_parts ) ) * components_per_vertex);
      GLfloat * capsule_data = new GLfloat[data_size];
    
      // 2 * theta_parts for side, then theta_parts - 2 for each cap
      // times components_per_triangle.
      GLsizei index_data_size =
        (GLsizei)( ( 2 * theta_parts + 2*2*phi_parts*theta_parts) * components_per_triangle );
      GLuint * capsule_index_data = new GLuint[index_data_size];  

      for( unsigned int i = 0; i < theta_parts; ++i ) {
        float ratio = (float) i / theta_parts;
        float angle = (float)(ratio * (Constants::pi*2));
        
        float sina = sin( angle );
        float cosa = cos( angle );

        /// Create side vertices.
        unsigned int base_data_index =
          (unsigned int)( i * 2 * components_per_vertex );
        // Vertex
        capsule_data[ base_data_index ] = -l_radius * sina;
        capsule_data[ base_data_index + 1 ] = half_height;
        capsule_data[ base_data_index + 2 ] = -l_radius * cosa;
        // Normal
        capsule_data[ base_data_index + 3 ] = -sina;
        capsule_data[ base_data_index + 4 ] = 0;
        capsule_data[ base_data_index + 5 ] = -cosa;
        // Texture coordinate
        capsule_data[ base_data_index + 6 ] = ratio;
        capsule_data[ base_data_index + 7 ] = 1;
        capsule_data[ base_data_index + 8 ] = 0;

        // Vertex
        capsule_data[ base_data_index + 9 ] =
          capsule_data[ base_data_index ];
        capsule_data[ base_data_index + 10 ] = -half_height;
        capsule_data[ base_data_index + 11 ] =
          capsule_data[ base_data_index + 2];
        // Normal
        capsule_data[ base_data_index + 12 ] = -sina;
        capsule_data[ base_data_index + 13 ] = 0;
        capsule_data[ base_data_index + 14 ] = -cosa;
        // Texture coordinate
        capsule_data[ base_data_index + 15 ] = ratio;
        capsule_data[ base_data_index + 16 ] = 0;
        capsule_data[ base_data_index + 17 ] = 0;

        // Create index for side vertex triangles. The triangles constructed
        // are those to the "right" of the vertices defined above if the
        // side is unfolded as a flat map.
        // First triangle.
        unsigned int base_index = i * 2 * components_per_triangle;
        unsigned int vertex_index = i * 2;
        capsule_index_data[ base_index ] = vertex_index;
        capsule_index_data[ base_index + 1 ] = vertex_index + 1;
        if( i < theta_parts - 1 )
          capsule_index_data[ base_index + 2 ] = vertex_index + 2;
        else
          capsule_index_data[ base_index + 2 ] = 0;

        // Second triangle.
        capsule_index_data[ base_index + 3 ] =
          capsule_index_data[ base_index + 2 ];
        capsule_index_data[ base_index + 4 ] =
          capsule_index_data[ base_index + 1 ];
        if( i < theta_parts - 1 )
          capsule_index_data[ base_index + 5 ] = vertex_index + 3;
        else
          capsule_index_data[ base_index + 5 ] = 1;
 
      }

      // the index of the last vertex added 
      unsigned int vertex_index = theta_parts * 2;      
      
      // the index into capsule_data where to add next data
      unsigned int base_data_index =
        (unsigned int)( vertex_index * components_per_vertex );
      
      // the index into capsule_index_data where to add the next triangle definition
      unsigned int base_index =
        (unsigned int)( theta_parts * 2 * components_per_triangle );

      unsigned int phi_index_step = theta_parts;

      // add vertex data for top cap
      for (unsigned int p = 0; p <= phi_parts; p++ ) {
        for (unsigned int t = 0; t < theta_parts; t++ ) { 
          H3DFloat phi = p * inc_phi;
          H3DFloat theta = t * inc_theta;
          H3DFloat next_phi = phi + inc_phi;
          bool at_seam = t == theta_parts - 1;
          H3DFloat next_theta = ( at_seam ? 0 :theta + inc_theta );

          H3DFloat x, y, z;
        
          x = - H3DSin( phi ) * H3DSin( theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( theta );

          // vertex
          capsule_data[ base_data_index ] = x * l_radius;
          capsule_data[ base_data_index + 1] = y * l_radius + half_height;
          capsule_data[ base_data_index + 2] = z * l_radius ;
          // normal
          capsule_data[ base_data_index + 3 ] = x;
          capsule_data[ base_data_index + 4 ] = y;
          capsule_data[ base_data_index + 5 ] = z;
          // tex coord
          capsule_data[ base_data_index + 6 ] = at_seam ? 1 : (H3DFloat) (theta / double_pi);
          capsule_data[ base_data_index + 7 ] = (H3DFloat) (1 - phi/ Constants::pi);
          capsule_data[ base_data_index + 8 ] = 0;

          if( p < phi_parts ) {
            // First triangle.
            capsule_index_data[ base_index ] = vertex_index;
            capsule_index_data[ base_index + 1 ] = vertex_index  + phi_index_step;
            if( t < theta_parts - 1 )
              capsule_index_data[ base_index + 2 ] = vertex_index + 1;
            else
              capsule_index_data[ base_index + 2 ] = vertex_index +  1 - phi_index_step;
          
            // Second triangle.
            capsule_index_data[ base_index + 3 ] = capsule_index_data[ base_index + 2 ];
            capsule_index_data[ base_index + 4 ] = capsule_index_data[ base_index + 1 ];
            if( t < theta_parts - 1 )
              capsule_index_data[ base_index + 5 ] = vertex_index + 1 + phi_index_step;
            else
              capsule_index_data[ base_index + 5 ] = vertex_index + 1;
            base_index += 2 * components_per_triangle;  
          }
          vertex_index++;
          base_data_index += components_per_vertex;

        }
      }


      // add vertex data for bottom cap
      for (unsigned int p = 0; p <= phi_parts; p++ ) {
        for (unsigned int t = 0; t < theta_parts; t++ ) { 
          H3DFloat phi = p * inc_phi + H3DFloat(Constants::pi)/2.0f;
          H3DFloat theta = t * inc_theta;
          bool at_seam = t == theta_parts - 1;
          H3DFloat next_theta = ( at_seam ? 0 :theta + inc_theta );

          H3DFloat x, y, z;
        
          x = - H3DSin( phi ) * H3DSin( theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( theta );

          // vertex
          capsule_data[ base_data_index ] = x * l_radius;
          capsule_data[ base_data_index + 1] = y * l_radius - half_height;
          capsule_data[ base_data_index + 2] = z * l_radius ;
          // normal
          capsule_data[ base_data_index + 3 ] = x;
          capsule_data[ base_data_index + 4 ] = y;
          capsule_data[ base_data_index + 5 ] = z;
          // tex coord
          capsule_data[ base_data_index + 6 ] = at_seam ? 1 : (H3DFloat) (theta / double_pi);
          capsule_data[ base_data_index + 7 ] = (H3DFloat) (1 - phi/ Constants::pi);
          capsule_data[ base_data_index + 8 ] = 0;

          if( p < phi_parts ) {
            // First triangle.
            capsule_index_data[ base_index ] = vertex_index;
            capsule_index_data[ base_index + 1 ] = vertex_index  + phi_index_step;
            if( t < theta_parts - 1 )
              capsule_index_data[ base_index + 2 ] = vertex_index + 1;
            else
              capsule_index_data[ base_index + 2 ] = vertex_index +  1 - phi_index_step;
          
            // Second triangle.
            capsule_index_data[ base_index + 3 ] = capsule_index_data[ base_index + 2 ];
            capsule_index_data[ base_index + 4 ] = capsule_index_data[ base_index + 1 ];
            if( t < theta_parts - 1 )
              capsule_index_data[ base_index + 5 ] = vertex_index + 1 + phi_index_step;
            else
              capsule_index_data[ base_index + 5 ] = vertex_index + 1;
            base_index += 2 * components_per_triangle;  
          }

          vertex_index++;
          base_data_index += components_per_vertex;
        }
      }



      // Create buffer and transfer data.
      if( !vbo_id ) {
        vbo_id = new GLuint[2];
        glGenBuffersARB( 2, vbo_id );
      }
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                       data_size * sizeof(GLfloat),
                       capsule_data, GL_STATIC_DRAW_ARB );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                       index_data_size * sizeof(GLuint),
                       capsule_index_data, GL_STATIC_DRAW_ARB );
      delete [] capsule_data;
      delete [] capsule_index_data;
    } else {
      // Nothing has changed, no data transfer needed.
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
    }

    /////////////////////////////////////////////////////////////

    // Enable vertex buffer objects.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), NULL );
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer( GL_FLOAT, 
                     9 * sizeof(GLfloat),
                     (GLvoid*)(3*sizeof(GLfloat)) );
    X3DTextureCoordinateNode::renderVertexBufferObjectForActiveTexture(
                                                                       3, GL_FLOAT, 9 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)) );

    unsigned int min_side_coord_index = 0;
    unsigned int max_side_coord_index = theta_parts * 2 - 1;
    unsigned int nr_side_triangle_vertices = theta_parts * 2 * components_per_triangle;

    unsigned int min_top_coord_index = max_side_coord_index + 1;
    unsigned int max_top_coord_index = max_side_coord_index + (phi_parts+1) * theta_parts;
    unsigned int nr_top_triangle_vertices = 2 * phi_parts * theta_parts * components_per_triangle;

    unsigned int min_bottom_coord_index = max_top_coord_index + 1;
    unsigned int max_bottom_coord_index = max_top_coord_index + (phi_parts + 1) * theta_parts;
    unsigned int nr_bottom_triangle_vertices = nr_top_triangle_vertices;


    if( side->getValue() && top->getValue() && bottom->getValue() ) {
      // Draw side.
      glDrawRangeElements( GL_TRIANGLES,
                           min_side_coord_index,
                           max_bottom_coord_index,
                           nr_side_triangle_vertices + nr_top_triangle_vertices + nr_bottom_triangle_vertices,
                           GL_UNSIGNED_INT,
                           NULL );
    } else {
      if( side->getValue() ) {
        glDrawRangeElements( GL_TRIANGLES,
                             min_side_coord_index,
                             max_side_coord_index,
                             nr_side_triangle_vertices,
                             GL_UNSIGNED_INT,
                             NULL );
      }
      if( top->getValue() ) {
        glDrawRangeElements( GL_TRIANGLES,
                             min_top_coord_index,
                             max_top_coord_index,
                             nr_top_triangle_vertices,
                             GL_UNSIGNED_INT,
                             (GLvoid*)( nr_side_triangle_vertices * sizeof(GLuint) ) );
      }
      
      if( bottom->getValue() ) {
        glDrawRangeElements( GL_TRIANGLES,
                             min_bottom_coord_index,
                             max_bottom_coord_index,
                             nr_bottom_triangle_vertices,
                             GL_UNSIGNED_INT,
                             (GLvoid*)( (nr_side_triangle_vertices + nr_top_triangle_vertices )* sizeof(GLuint) ) );
      }

    }

  

  } else {

    // render side
    if ( side->getValue() ) {
      glBegin( GL_QUAD_STRIP );
      for( unsigned int i=0; i<=theta_parts; ++i ) {
        float ratio = (float) i / theta_parts;
        float angle =  (float)(ratio * (Constants::pi*2));
      
        float sina = sin( angle );
        float cosa = cos( angle );

        glNormal3f( -sina, 0, -cosa );
        renderTexCoordForActiveTexture( Vec3f( ratio, 1, 0 ) );
        glVertex3f( -l_radius * sina, l_height / 2, -l_radius * cosa );
        renderTexCoordForActiveTexture( Vec3f( ratio, 0, 0 ) );
        glVertex3f( -l_radius * sina, -l_height / 2, -l_radius * cosa );
      }
      glEnd();
    }


    H3DFloat inc_theta = (H3DFloat) Constants::pi*2 / theta_parts;
    H3DFloat inc_phi =   (H3DFloat) (Constants::pi/2) /phi_parts;
    H3DFloat r = radius->getValue();
    H3DFloat h = height->getValue()/2;
    H3DFloat double_pi = (H3DFloat) Constants::pi * 2;

    // render top
    if ( top->getValue() ) {
      glBegin( GL_QUADS );

      for (unsigned int p = 0; p < phi_parts; p++ ) {
        for (unsigned int t = 0; t < theta_parts; t++ ) { 
          H3DFloat phi = p * inc_phi;
          H3DFloat theta = t * inc_theta;
          H3DFloat next_phi = phi + inc_phi;
          bool at_seam = t == theta_parts - 1;
          H3DFloat next_theta = ( at_seam ? 0 :theta + inc_theta );
        
          H3DFloat x, y, z;
        
          x = - H3DSin( phi ) * H3DSin( theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( (H3DFloat) (theta / double_pi), 
                                                (H3DFloat) (1 - phi/ Constants::pi),
                                                0 ) );
          glVertex3f( x * r, y * r + h, z * r );

          x = - H3DSin( next_phi ) * H3DSin( theta );
          y = H3DCos( next_phi );
          z = - H3DSin( next_phi ) * H3DCos( theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( (H3DFloat) (theta / double_pi), 
                                                (H3DFloat) (1 - next_phi/ Constants::pi ),
                                                0 ) );
          glVertex3f( x * r, y * r + h, z * r );

          x = - H3DSin( next_phi ) * H3DSin( next_theta );
          y = H3DCos( next_phi );
          z = - H3DSin( next_phi ) * H3DCos( next_theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( at_seam ? 1 : (H3DFloat) (next_theta / double_pi ), 
                                                (H3DFloat)(1 - next_phi/ Constants::pi),
                                                0 ) );
          glVertex3f( x * r, y * r + h, z * r );

          x = - H3DSin( phi ) * H3DSin( next_theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( next_theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( at_seam ? 1 : (H3DFloat)(next_theta / double_pi), 
                                                (H3DFloat)(1 - phi/ Constants::pi),
                                                0 ) );
          glVertex3f( x * r, y * r + h, z * r );
        }
      }
      glEnd();
    }

    // render bottom
    if ( bottom->getValue() ) {
      glBegin( GL_QUADS );
      for (unsigned int p = 0; p < phi_parts; p++ ) {
        for (unsigned int t = 0; t < theta_parts; t++ ) { 
          H3DFloat phi = p * inc_phi + H3DFloat(Constants::pi)/2.0f;
          H3DFloat theta = t * inc_theta;
          H3DFloat next_phi = phi + inc_phi;
          bool at_seam = t == theta_parts - 1;
          H3DFloat next_theta = ( at_seam ? 0 :theta + inc_theta );
        
          H3DFloat x, y, z;
        
          x = - H3DSin( phi ) * H3DSin( theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( (H3DFloat) (theta / double_pi), 
                                                (H3DFloat) (1 - phi/ Constants::pi),
                                                0 ) );
          glVertex3f( x * r, y * r - h, z * r );

          x = - H3DSin( next_phi ) * H3DSin( theta );
          y = H3DCos( next_phi );
          z = - H3DSin( next_phi ) * H3DCos( theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( (H3DFloat) (theta / double_pi), 
                                                (H3DFloat) (1 - next_phi/ Constants::pi ),
                                                0 ) );
          glVertex3f( x * r, y * r - h, z * r );

          x = - H3DSin( next_phi ) * H3DSin( next_theta );
          y = H3DCos( next_phi );
          z = - H3DSin( next_phi ) * H3DCos( next_theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( at_seam ? 1 : (H3DFloat) (next_theta / double_pi ), 
                                                (H3DFloat)(1 - next_phi/ Constants::pi),
                                                0 ) );
          glVertex3f( x * r, y * r - h, z * r );

          x = - H3DSin( phi ) * H3DSin( next_theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( next_theta );

          glNormal3f( x, y, z );
          renderTexCoordForActiveTexture( 
                                         Vec3f( at_seam ? 1 : (H3DFloat)(next_theta / double_pi), 
                                                (H3DFloat)(1 - phi/ Constants::pi),
                                                0 ) );
          glVertex3f( x * r, y * r - h, z * r );
        }
      }
      glEnd();
    }
  }
};


void Capsule::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  if( solid->getValue() ) {
    useBackFaceCulling( true );
  } else {
    useBackFaceCulling( false );
  }
}


int Capsule::nrTriangles() {
  unsigned int nr_triangles = 0;
  if( side->getValue() ) {
    nr_triangles += 2 * theta_parts;
  }
  if( top->getValue() ) {
    nr_triangles += 2 * theta_parts * phi_parts;
  }
  if( bottom->getValue() ) {
    nr_triangles += 2 * theta_parts * phi_parts;
    }
  
  return nr_triangles;
}
