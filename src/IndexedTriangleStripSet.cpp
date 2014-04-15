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
/// \file IndexedTriangleStripSet.cpp
/// \brief CPP file for IndexedTriangleStripSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/IndexedTriangleStripSet.h>
#include <H3D/Normal.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase IndexedTriangleStripSet::database( 
                                     "IndexedTriangleStripSet", 
                                     &(newInstance<IndexedTriangleStripSet>), 
                                     typeid( IndexedTriangleStripSet ),
                                     &X3DComposedGeometryNode::database );

namespace IndexedTriangleStripSetInternals {
  FIELDDB_ELEMENT( IndexedTriangleStripSet, set_index, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedTriangleStripSet, index, INPUT_OUTPUT );
}


IndexedTriangleStripSet::IndexedTriangleStripSet( 
                              Inst< SFNode                  > _metadata,
                              Inst< SFBound                 > _bound,
                              Inst< DisplayList             > _displayList,
                              Inst< SFColorNode             > _color,
                              Inst< SFCoordinateNode        > _coord,
                              Inst< SFNormalNode            > _normal,
                              Inst< SFTextureCoordinateNode > _texCoord,
                              Inst< SFBool                  > _ccw,
                              Inst< SFBool                  > _colorPerVertex,
                              Inst< SFBool                  > _normalPerVertex,
                              Inst< SFBool                  > _solid,
                              Inst< MFVertexAttributeNode   > _attrib,
                              Inst< AutoNormal              > _autoNormal,
                              Inst< MFInt32                 > _set_index,
                              Inst< MFInt32                 > _index, 
                              Inst< SFFogCoordinate         > _fogCoord ) :
  X3DComposedGeometryNode( _metadata, _bound, _displayList, 
                           _color, _coord, _normal, _texCoord, 
                           _ccw, _colorPerVertex, _normalPerVertex, 
                           _solid, _attrib, _fogCoord ),
  autoNormal( _autoNormal ),
  set_index( _set_index ),
  index( _index ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "IndexedTriangleStripSet";
  database.initFields( this );
  
  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );

  index->route( displayList );
  set_index->route( index, id );

  normalPerVertex->route( autoNormal );
  coord->route( autoNormal );
  index->route( autoNormal );
  ccw->route( autoNormal );

  coord->route( bound );
  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  index->route( vboFieldsUpToDate );
}

IndexedTriangleStripSet::~IndexedTriangleStripSet() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}


void IndexedTriangleStripSet::render() {
  X3DCoordinateNode *coordinate_node = coord->getValue();
  X3DTextureCoordinateNode *tex_coord_node = texCoord->getValue();
  X3DColorNode *color_node = color->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();
  X3DNormalNode *normal_node = normal->getValue();

  if( !normal_node ) {
    normal_node = autoNormal->getValue();
  }

  bool tex_coord_gen = 
    !tex_coord_node || (tex_coord_node && tex_coord_node->supportsTexGen());
  bool tex_coords_per_vertex = 
    tex_coord_node && tex_coord_node->supportsExplicitTexCoords();

  const vector< int > &indices  = index->getValue();
 
  if( coordinate_node ) {
    // no X3DTextureCoordinateNode, so we generate texture coordinates
    // based on the bounding box according to the X3D specification.
    if( tex_coord_gen ) startTexGen( tex_coord_node );
    
    if(  tex_coords_per_vertex &&
         coordinate_node->nrAvailableCoords() > 
         tex_coord_node->nrAvailableTexCoords() ) {
      stringstream s;
      s << "Must contain at least as many elements as coord (" 
        << coordinate_node->nrAvailableCoords() << ") in \"" 
        << getName() << "\" node. ";
      throw NotEnoughTextureCoordinates( tex_coord_node->nrAvailableTexCoords(),
                                         s.str(), H3D_FULL_LOCATION );
    }

    // if we have a color node we use the color from that instead
    // of the previously installed Material node.
    if( color_node ) {
      // Make sure we have enough colors      
      if( coordinate_node->nrAvailableCoords() > 
          color_node->nrAvailableColors() ) {
        stringstream s;
        s << "Must contain at least as many elements as coord (" 
          << coordinate_node->nrAvailableCoords() << ") in \"" 
          << getName() << "\" node. ";
        throw NotEnoughColors( color_node->nrAvailableColors(),
                               s.str(), H3D_FULL_LOCATION );
      }
      color_node->preRender();
    }

    // set fog to get fog depth from fog coordinates if available
    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPushAttrib( GL_FOG_BIT );
      glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
    }

    GLhandleARB shader_program = 0;
    // Set the attribute index to use for all vertex attributes
    if( GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader ) {
      shader_program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );
      if( shader_program ) {
        for( unsigned int i = 0; i < attrib->size(); ++i ) {
          X3DVertexAttributeNode *attr = attrib->getValueByIndex( i );
          if( attr ) {
            GLint loc = 
              glGetAttribLocationARB( shader_program, 
                                      attr->name->getValue().c_str()); 
            attr->setAttribIndex( loc );
          }
        }
      }
    }
    
    if( normalPerVertex->getValue() ) {
      // if normal per vertex we can use arrays or vertex buffer objects
      // to render the geometry, they all use the same indices.

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
          prefer_vertex_buffer_object =
            go->preferVertexBufferObject->getValue();
        }
      }

      if( prefer_vertex_buffer_object ) {

        coordinate_node->renderVertexBufferObject();
        normal_node->renderVertexBufferObject();
        if( color_node ) color_node->renderVertexBufferObject();
        if( tex_coords_per_vertex )
          renderTexCoordVertexBufferObject( tex_coord_node );
        if( fog_coord_node ) fog_coord_node->renderVertexBufferObject();
        // Set up shader vertex attributes.
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->renderVertexBufferObject();
        }

        if( !vboFieldsUpToDate->isUpToDate() ) {
          // Only transfer data when it has been modified.
          vboFieldsUpToDate->upToDate();
          vector< GLuint > tmp_indices;
          tmp_indices.reserve( indices.size() );
          nr_index_per_strip.clear();
          // increase i to the end of the triangle fan
          for( unsigned int i = 0; i < indices.size(); ++i ) {
            unsigned int old_i = i;
            for( ; i < indices.size() && indices[i] != -1; ++i )
              tmp_indices.push_back( indices[i] );
            if( i - old_i > 0 ) {
              nr_index_per_strip.push_back( i - old_i );
            }
          }
          if( !vbo_id ) {
            vbo_id = new GLuint;
            glGenBuffersARB( 1, vbo_id );
          }
          glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *vbo_id );
          glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                           tmp_indices.size() * sizeof(GLuint),
                           &(*(tmp_indices.begin()) ), GL_STATIC_DRAW_ARB );
        } else {
          glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *vbo_id );
        }

        // the index in indices for the start of the current triangle strip.
        GLsizei start_pos = 0;
        // draw each triangle strip from the arrays 
        for( unsigned int i = 0; i < nr_index_per_strip.size(); ++i ) {
          // render the triangle fan
          glDrawElements( GL_TRIANGLE_STRIP, 
                          nr_index_per_strip[i],
                          GL_UNSIGNED_INT,
                          (GLvoid*)( start_pos * sizeof( GLuint ) ) );
          start_pos += nr_index_per_strip[i];
        }

        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
        coordinate_node->disableVertexBufferObject();
        normal_node->disableVertexBufferObject();
        if( color_node ) color_node->disableVertexBufferObject();
        if( tex_coords_per_vertex )
          disableTexCoordVertexBufferObject( tex_coord_node );
        if( fog_coord_node ) fog_coord_node->disableVertexBufferObject();
        // Set up shader vertex attributes.
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->disableVertexBufferObject();
        } 
      } else {
        coordinate_node->renderArray();
        normal_node->renderArray();
        if( color_node ) color_node->renderArray();
        if( tex_coords_per_vertex ) renderTexCoordArray( tex_coord_node );
        if( fog_coord_node) fog_coord_node->renderArray();
        // Set up shader vertex attributes.
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->renderArray();
        }
        // the index in indices for the start of the current triangle strip.
        unsigned int start_pos = 0;
   
        // draw each triangle strip from the arrays 
        for( unsigned int i = 0; i < indices.size(); ++i ) {
          // increase i to the end of the triangle strip
          for( ; i < indices.size() && indices[i] != -1; ++i );

          // render the triangle strip
          glDrawElements( GL_TRIANGLE_STRIP, 
                          i - start_pos, 
                          GL_UNSIGNED_INT,
                          &(*(indices.begin())) + start_pos );
          start_pos = i + 1;
        }
        
        coordinate_node->disableArray();
        normal_node->disableArray();
        if( color_node ) color_node->disableArray();
        if( tex_coords_per_vertex ) disableTexCoordArray( tex_coord_node );
        if( fog_coord_node) fog_coord_node->disableArray();
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->disableArray();
        }
      }
    } else {
      // the number of triangles rendered so far in the entire 
      // IndexedTriangleStripSet
      unsigned int triangle_counter = 0;

      // we cannot use arrays to render when normal_per_vertex is false
      // since the indexing into the normals are different from the rest.

      // render as triangles, we cannot use GL_TRIANGLE_STRIP since the normals
      // have to be per face. Also since the indices specifies a triangle strip
      // we have to alternate the direction of vertices every second triangle
      // in order to get the front face correct.
      for( unsigned int i = 0; i < indices.size() - 2; ++i ) {
        glBegin( GL_TRIANGLES );
        // the number of triangles rendered so far in this triangle strip
        unsigned int triangles_in_strip = 0;
        for( ; i < indices.size() - 2 && indices[i] != -1; ++i ) {
          if( indices[i+1] != -1 && 
              indices[i+2] != -1 ) {
            normal_node->render( triangle_counter );
            if( triangles_in_strip % 2 == 0 ) {
              // vertex 1
              if( color_node ) color_node->render( indices[i] );
              if( tex_coords_per_vertex ) renderTexCoord( indices[i], 
                                                          tex_coord_node );
              if( fog_coord_node) fog_coord_node->render(indices[i]);
              for( unsigned int attrib_index = 0;
                   attrib_index < attrib->size(); ++attrib_index ) {
                X3DVertexAttributeNode *attr = 
                  attrib->getValueByIndex( attrib_index );
                if( attr ) attr->render( indices[i] );
              } 
              coordinate_node->render( indices[i] );

              // vertex 2
              if( color_node ) color_node->render( indices[i+1] );
              if( tex_coords_per_vertex ) renderTexCoord( indices[i+1],
                                                          tex_coord_node );
              if( fog_coord_node) fog_coord_node->render(indices[i+1]);
              for( unsigned int attrib_index = 0;
                   attrib_index < attrib->size(); ++attrib_index ) {
                X3DVertexAttributeNode *attr = 
                  attrib->getValueByIndex( attrib_index );
                if( attr ) attr->render( indices[i+1] );
              }
              coordinate_node->render( indices[i+1] );

              // vertex 3
              if( color_node ) color_node->render( indices[i+2] );
              if( tex_coords_per_vertex ) renderTexCoord( indices[i+2],
                                                          tex_coord_node );
              if( fog_coord_node) fog_coord_node->render(indices[i+2]);
              for( unsigned int attrib_index = 0;
                   attrib_index < attrib->size(); ++attrib_index ) {
                X3DVertexAttributeNode *attr = 
                  attrib->getValueByIndex( attrib_index );
                if( attr ) attr->render( indices[i+2] );
              }
              coordinate_node->render( indices[i+2] );
            } else {
              // vertex 1
              if( color_node ) color_node->render( indices[i] );
              if( tex_coords_per_vertex ) renderTexCoord( indices[i],
                                                          tex_coord_node );
              for( unsigned int attrib_index = 0;
                   attrib_index < attrib->size(); ++attrib_index ) {
                X3DVertexAttributeNode *attr = 
                  attrib->getValueByIndex( attrib_index );
                if( attr ) attr->render( indices[i] );
              }
              coordinate_node->render( indices[i] );

              // vertex 2
              if( color_node ) color_node->render( indices[i+2] );
              if( tex_coords_per_vertex ) renderTexCoord( indices[i+2],
                                                          tex_coord_node );
              for( unsigned int attrib_index = 0;
                   attrib_index < attrib->size(); ++attrib_index ) {
                X3DVertexAttributeNode *attr = 
                  attrib->getValueByIndex( attrib_index );
                if( attr ) attr->render( indices[i+2] );
              }
              coordinate_node->render( indices[i+2] );

              // vertex 3
              if( color_node ) color_node->render( indices[i+1] );
              if( tex_coords_per_vertex ) renderTexCoord( indices[i+1],
                                                          tex_coord_node );
              for( unsigned int attrib_index = 0;
                   attrib_index < attrib->size(); ++attrib_index ) {
                X3DVertexAttributeNode *attr = 
                  attrib->getValueByIndex( attrib_index );
                if( attr ) attr->render( indices[i+1] );
              }
              coordinate_node->render( indices[i+1] );
            }
            ++triangle_counter;
            ++triangles_in_strip;
          }
        }
        glEnd();
      }
    }

    // restore previous fog attributes
    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPopAttrib();
    }      

    // disable texture coordinate generation.
    if( tex_coord_gen ) stopTexGen( tex_coord_node );

    if ( color_node ) {
      color_node->postRender();
    } 
  } 
}

void IndexedTriangleStripSet::traverseSG( TraverseInfo &ti ) {
  X3DComposedGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}



void IndexedTriangleStripSet::AutoNormal::update() {
  bool normals_per_vertex = 
    static_cast< SFBool * >( routes_in[0] )->getValue();
  X3DCoordinateNode *coord = 
    static_cast< X3DCoordinateNode * >( static_cast< SFCoordinateNode * >
                                        ( routes_in[1] )->getValue() );
  const vector<int> &index = 
    static_cast< MFInt32 * >( routes_in[2] )->getValue();
  bool ccw = static_cast< SFBool * >( routes_in[3] )->getValue();

  if( normals_per_vertex ) 
    value = generateNormalsPerVertex( coord, index, ccw );
  else
    value = generateNormalsPerFace( coord, index, ccw );
}

X3DNormalNode *IndexedTriangleStripSet::AutoNormal::generateNormalsPerVertex( 
                                   X3DCoordinateNode *coord,
                                   const vector< int > &index,
                                   bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals( coord->nrAvailableCoords(), 
                             Vec3f( 0, 0, 0 ) );
    unsigned int triangles_in_strip = 0;
    for( unsigned int j = 0; j < index.size() - 2; ++j ) {
      if( j+2 < index.size() && 
          index[j] != -1 && 
          index[j+1] != -1 && 
          index[j+2] != -1 ) {
        Vec3f norm, A, B, C, AB, BC;
        // calculate a normal 
        A = coord->getCoord( index[ j ] );
        B = coord->getCoord( index[ j+1 ] );
        C = coord->getCoord( index[ j+2 ] );
        
        AB = B - A;
        BC = C - B;
        
        norm = AB % BC;

        norm.normalizeSafe();
        if( !ccw ) 
          norm = -norm;
        // since indices are specified as triangle strip we have to flip the 
        // normal every second triangle
        if( triangles_in_strip % 2 == 1 )
          norm = -norm;
        normals[index[ j ]] += norm;
        normals[index[ j+1 ]] += norm;
        normals[index[ j+2 ]] += norm;
        ++triangles_in_strip;
      } else {
        triangles_in_strip = 0;
      }
    }

    for( vector<Vec3f>::iterator i = normals.begin(); 
         i != normals.end(); 
         ++i ) {
        (*i).normalizeSafe();
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

X3DNormalNode *IndexedTriangleStripSet::AutoNormal::generateNormalsPerFace( 
                                             X3DCoordinateNode *coord,
                                             const vector< int > &index,
                                             bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals;
    unsigned int triangles_in_strip = 0;
    for( unsigned int j = 0; j < index.size() - 2; ++j ) {
      
      if( j+2 < index.size() && 
          index[j] != -1 && 
          index[j+1] != -1 && 
          index[j+2] != -1 ) {
        
        Vec3f norm, A, B, C, AB, BC;
        // calculate a normal for the triangle
        A = coord->getCoord( index[ j ] );
        B = coord->getCoord( index[ j + 1 ] );
        C = coord->getCoord( index[ j + 2 ] );
        
        AB = B - A;
        BC = C - B;
        
        norm = AB % BC;
        
        try {
          norm.normalize();
        } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
          norm = Vec3f( 1, 0, 0 );
        }
        
        if( !ccw ) 
          norm = -norm;
        
        // since indices are specified as triangle strip we have to flip the 
        // normal every second triangle
        if( triangles_in_strip % 2 == 1 )
          norm = -norm;

        normals.push_back( norm );
        ++triangles_in_strip;
      } else {
        triangles_in_strip = 0;
      }
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

