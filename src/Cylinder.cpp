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
/// \file Cylinder.cpp
/// \brief CPP file for Cylinder, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Cylinder.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Cylinder::database( "Cylinder", 
                                    &(newInstance<Cylinder>), 
                                    typeid( Cylinder ),
                                    &X3DGeometryNode::database );

namespace CylinderInternals {
  FIELDDB_ELEMENT( Cylinder, bottom, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, side, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, top, INPUT_OUTPUT );
}

Cylinder::Cylinder( 
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

  type_name = "Cylinder";
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

Cylinder::~Cylinder() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 2, vbo_id );
    delete [] vbo_id;
    vbo_id = NULL;
  }
}

void Cylinder::render() { 
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
  int nr_faces = 120;

  if( prefer_vertex_buffer_object ) {
    if( !vboFieldsUpToDate->isUpToDate() ) {
      // Only transfer data when it has been modified.
      vboFieldsUpToDate->upToDate();
      unsigned int nr_data_vertices = 9; // 9 floats/vertex.
      // 2 * nr_faces for side, then nr_faces for each cap
      // times nr_data_vertices.
      GLsizei data_size =
        (GLsizei)( 4 * nr_faces * nr_data_vertices );
      GLfloat * cylinder_data = new GLfloat[data_size];
      unsigned int nr_index_data = 3; // 3 vertices/triangle.
      // 2 * nr_faces for side, then nr_faces - 2 for each cap
      // times nr_index_data.
      GLsizei index_data_size =
        (GLsizei)( ( 4 * nr_faces - 4 ) * nr_index_data );
      GLuint * cylinder_index_data = new GLuint[index_data_size];
      for( int i = 0; i < nr_faces; ++i ) {
        float ratio = (float) i / nr_faces;
        float angle = (float)(ratio * (Constants::pi*2));
        
        float sina = sin( angle );
        float cosa = cos( angle );

        /// Create side vertices.
        unsigned int base_data_index =
          (unsigned int)( i * 2 * nr_data_vertices );
        // Vertex
        cylinder_data[ base_data_index ] = -l_radius * sina;
        cylinder_data[ base_data_index + 1 ] = half_height;
        cylinder_data[ base_data_index + 2 ] = -l_radius * cosa;
        // Normal
        cylinder_data[ base_data_index + 3 ] = -sina;
        cylinder_data[ base_data_index + 4 ] = 0;
        cylinder_data[ base_data_index + 5 ] = -cosa;
        // Texture coordinate
        cylinder_data[ base_data_index + 6 ] = ratio;
        cylinder_data[ base_data_index + 7 ] = 1;
        cylinder_data[ base_data_index + 8 ] = 0;

        // Vertex
        cylinder_data[ base_data_index + 9 ] =
          cylinder_data[ base_data_index ];
        cylinder_data[ base_data_index + 10 ] = -half_height;
        cylinder_data[ base_data_index + 11 ] =
          cylinder_data[ base_data_index + 2];
        // Normal
        cylinder_data[ base_data_index + 12 ] = -sina;
        cylinder_data[ base_data_index + 13 ] = 0;
        cylinder_data[ base_data_index + 14 ] = -cosa;
        // Texture coordinate
        cylinder_data[ base_data_index + 15 ] = ratio;
        cylinder_data[ base_data_index + 16 ] = 0;
        cylinder_data[ base_data_index + 17 ] = 0;

        // Create index for side vertex triangles. The triangles constructed
        // are those to the "right" of the vertices defined above if the
        // side is unfolded as a flat map.
        // First triangle.
        unsigned int base_index = i * 2 * nr_index_data;
        unsigned int vertex_index = i * 2;
        cylinder_index_data[ base_index ] = vertex_index;
        cylinder_index_data[ base_index + 1 ] = vertex_index + 1;
        if( i < nr_faces - 1 )
          cylinder_index_data[ base_index + 2 ] = vertex_index + 2;
        else
          cylinder_index_data[ base_index + 2 ] = 0;

        // Second triangle.
        cylinder_index_data[ base_index + 3 ] =
          cylinder_index_data[ base_index + 2 ];
        cylinder_index_data[ base_index + 4 ] =
          cylinder_index_data[ base_index + 1 ];
        if( i < nr_faces - 1 )
          cylinder_index_data[ base_index + 5 ] = vertex_index + 3;
        else
          cylinder_index_data[ base_index + 5 ] = 1;

        // Create top vertex, it differs from side vertices when it
        // comes to the normal and texture coordinates.
        unsigned int top_base_data_index =
          (unsigned int)( ( 2 * nr_faces + i ) * nr_data_vertices );

        // Vertex
        cylinder_data[ top_base_data_index ] =
          cylinder_data[ base_data_index ];
        cylinder_data[ top_base_data_index + 1 ] =
          cylinder_data[ base_data_index + 1];
        cylinder_data[ top_base_data_index + 2 ] =
          cylinder_data[ base_data_index + 2];
        // Normal
        cylinder_data[ top_base_data_index + 3 ] = 0;
        cylinder_data[ top_base_data_index + 4 ] = 1;
        cylinder_data[ top_base_data_index + 5 ] = 0;
        // Texture coordinate
        cylinder_data[ top_base_data_index + 6 ] = 0.5f - 0.5f * sina;
        cylinder_data[ top_base_data_index + 7 ] = 0.5f + 0.5f * cosa;
        cylinder_data[ top_base_data_index + 8 ] = 0;

        // Create bottom vertex, it differs from side vertices when it
        // comes to the normal and texture coordinates.
        unsigned int bottom_base_data_index = (unsigned int)(
          ( 3 * nr_faces + i ) * nr_data_vertices );
        // Vertex
        cylinder_data[ bottom_base_data_index ] =
          cylinder_data[ base_data_index + 9 ];
        cylinder_data[ bottom_base_data_index + 1 ] =
          cylinder_data[ base_data_index + 10 ];
        cylinder_data[ bottom_base_data_index + 2 ] =
          cylinder_data[ base_data_index + 11 ];
        // Normal
        cylinder_data[ bottom_base_data_index + 3 ] = 0;
        cylinder_data[ bottom_base_data_index + 4 ] = -1;
        cylinder_data[ bottom_base_data_index + 5 ] = 0;
        // Texture coordinate
        cylinder_data[ bottom_base_data_index + 6 ] =
          cylinder_data[ top_base_data_index + 6 ];
        cylinder_data[ bottom_base_data_index + 7 ] =
          cylinder_data[ top_base_data_index + 7 ];
        cylinder_data[ bottom_base_data_index + 8 ] =
          cylinder_data[ top_base_data_index + 8 ];

        if( i > 1 ) {
          // Create indices for top triangles.
          base_index = ( 2 * nr_faces + i - 2 ) * nr_index_data;
          unsigned int base_vertex_index = 2 * nr_faces;
          cylinder_index_data[ base_index ] = base_vertex_index;
          cylinder_index_data[ base_index + 1 ] = base_vertex_index + i - 1;
          cylinder_index_data[ base_index + 2 ] = base_vertex_index + i;
           // Create indices for bottom triangles.
          base_index = ( 3 * nr_faces + i - 4 ) * nr_index_data;
          base_vertex_index = 3 * nr_faces;
          cylinder_index_data[ base_index ] = base_vertex_index;
          cylinder_index_data[ base_index + 1 ] = base_vertex_index + i;
          cylinder_index_data[ base_index + 2 ] = base_vertex_index + i - 1;
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
                       cylinder_data, GL_STATIC_DRAW_ARB );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                       index_data_size * sizeof(GLuint),
                       cylinder_index_data, GL_STATIC_DRAW_ARB );
      delete [] cylinder_data;
      delete [] cylinder_index_data;
    } else {
      // Nothing has changed, no data transfer needed.
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
    }

    // Enable vertex buffer objects.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), NULL );
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer( GL_FLOAT, 9 * sizeof(GLfloat),
                     (GLvoid*)(3*sizeof(GLfloat)) );
    X3DTextureCoordinateNode::renderVertexBufferObjectForActiveTexture(
      3, GL_FLOAT, 9 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)) );

    unsigned int min_index = 0;
    unsigned int max_index = nr_faces * 2 - 1;
    unsigned int nr_side_index = (unsigned int)( nr_faces * 6 );
    unsigned int nr_cap_index = ( nr_faces - 2 ) * 3;
    if( side->getValue() ) {
      // Draw side.
      glDrawRangeElements( GL_TRIANGLES,
                           min_index,
                           max_index,
                           nr_side_index,
                           GL_UNSIGNED_INT,
                           NULL );
    }

    min_index = max_index + 1;
    max_index = min_index + nr_faces - 1;
    if( top->getValue() ) {
      // Draw top.
      glDrawRangeElements( GL_TRIANGLES,
                           min_index,
                           max_index,
                           nr_cap_index,
                           GL_UNSIGNED_INT,
                           (GLvoid*)( nr_side_index * sizeof(GLuint) ) );
    }

    min_index = max_index + 1;
    max_index = min_index + nr_faces - 1;
    if( bottom->getValue() ) {
      // Draw bottom.
      glDrawRangeElements( GL_TRIANGLES,
                           min_index,
                           max_index,
                           nr_cap_index,
                           GL_UNSIGNED_INT,
                           (GLvoid*)( ( nr_side_index + nr_cap_index ) *
                                      sizeof(GLuint) ) );
    }

    // Disable vertex buffer object.
    X3DTextureCoordinateNode::disableVBOForActiveTexture();
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
  } else {
    // Render cylinder using glBegin calls.
    // render side
    if ( side->getValue() ) {
      glBegin( GL_QUAD_STRIP );
      for( int i=0; i<=nr_faces; ++i ) {
        float ratio = (float) i / nr_faces;
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

    // render top
    if ( top->getValue() ) {
      glBegin( GL_POLYGON );
      glNormal3f( 0, 1, 0 );
      for( int i = 0; i < nr_faces; ++i ) {
        float angle = (float)( i * (Constants::pi*2) / (float) nr_faces);
        float sina = sin( angle );
        float cosa = cos( angle );
        renderTexCoordForActiveTexture( Vec3f( 0.5f - 0.5f * sina, 
                                               0.5f + 0.5f * cosa, 0 ) );
        glVertex3f( -l_radius * sina, l_height / 2, -l_radius * cosa );
      }
      glEnd();
    }

    // render bottom
    if ( bottom->getValue() ) {
      glBegin( GL_POLYGON );
      glNormal3f( 0, -1, 0 );
      for( int i = nr_faces; i >= 0; --i ) {
        float angle = (float)( i * (Constants::pi*2) / (float) nr_faces);
        float sina = sin( angle );
        float cosa = cos( angle );
        renderTexCoordForActiveTexture( Vec3f( 0.5f - 0.5f * sina, 
                                               0.5f + 0.5f * cosa, 0 ) );
        glVertex3f( -l_radius * sina, -l_height / 2, -l_radius * cosa );
      }
      glEnd();
    }
  }
}


void Cylinder::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  if( solid->getValue() ) {
    useBackFaceCulling( true );
  } else {
    useBackFaceCulling( false );
  }
}

