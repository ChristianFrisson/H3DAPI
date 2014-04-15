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
/// \file IndexedTriangleSet.cpp
/// \brief CPP file for IndexedTriangleSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/IndexedTriangleSet.h>
#include <H3D/Normal.h>
#include <H3D/H3DHapticsDevice.h>
#include <H3D/DeviceInfo.h>
#include <H3D/H3DSurfaceNode.h>
#include <H3D/TextureCoordinate.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;



// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase IndexedTriangleSet::database( 
  "IndexedTriangleSet", 
  &(newInstance<IndexedTriangleSet>),
  typeid( IndexedTriangleSet ),
  &X3DComposedGeometryNode::database );

namespace IndexedTriangleSetInternals {
  FIELDDB_ELEMENT( IndexedTriangleSet, set_index, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedTriangleSet, index, INPUT_OUTPUT );
}


IndexedTriangleSet::IndexedTriangleSet( 
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
  autoTangent( new AutoTangent ),
  render_tangents( false ),
  render_patches( false ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {
  
  type_name = "IndexedTriangleSet";
  database.initFields( this );
  
  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );
  autoTangent->setName( "autoTangent" );
  autoTangent->setOwner( this );
  
  index->route( displayList );
  set_index->route( index, id );
  
  normalPerVertex->route( autoNormal );
  coord->route( autoNormal );
  index->route( autoNormal );
  ccw->route( autoNormal );
  
  normalPerVertex->route( autoTangent );
  coord->route( autoTangent );
  index->route( autoTangent );
  texCoord->route( autoTangent );

  coord->route( bound );

  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  index->route( vboFieldsUpToDate );
}

IndexedTriangleSet::~IndexedTriangleSet() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

void IndexedTriangleSet::render() {
  X3DCoordinateNode *coordinate_node = coord->getValue();
  X3DTextureCoordinateNode *tex_coord_node = texCoord->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();
  X3DColorNode *color_node = color->getValue();
  X3DNormalNode *normal_node = normal->getValue();

  if( !normal_node ) {
    normal_node = autoNormal->getValue();
  }

  bool tex_coord_gen = 
    !tex_coord_node || (tex_coord_node && tex_coord_node->supportsTexGen());
  bool tex_coords_per_vertex = 
    tex_coord_node && tex_coord_node->supportsExplicitTexCoords();

  const vector< int > &indices  = index->getValue();

  if( coordinate_node && !indices.empty() ) {

    // Check that the number of available coords are not 0 since we use
    // "coordinate_node->nrAvailableCoords() - 1" as argument to
    // glDrawRangeElements and we do not want some strange error. Besides
    // if the number of coordinates is 0 then nothing will be rendered so
    // this is a slight optimization.
    if( coordinate_node->nrAvailableCoords() <= 0 )
      return;

    // no X3DTextureCoordinateNode, so we generate texture coordinates
    // based on the bounding box according to the X3D specification.
    if( tex_coord_gen ) {
      startTexGen( tex_coord_node );
    } 

    if( tex_coords_per_vertex &&
      coordinate_node->nrAvailableCoords() > 
      tex_coord_node->nrAvailableTexCoords() ) {
      // check if texture coordinate generator
      if( tex_coord_node->nrAvailableTexCoords() != -1 ) {
        stringstream s;
        s << "Must contain at least as many elements as coord (" 
          << coordinate_node->nrAvailableCoords() << ") in \"" 
          << getName() << "\" node. ";
        throw NotEnoughTextureCoordinates( tex_coord_node->nrAvailableTexCoords(),
          s.str(), H3D_FULL_LOCATION );
      }
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

        // render tangents as an attribute if needed.
        if( render_tangents ) {
          for( unsigned int i = 0; i < autoTangent->size(); ++i ) {
            X3DVertexAttributeNode *attr = autoTangent->getValueByIndex( i );
            if( attr ) {
              GLint loc = 
                glGetAttribLocationARB( shader_program, 
                                        attr->name->getValue().c_str()); 
              attr->setAttribIndex( loc );
            }
          }
        }
      }
    }

    unsigned int nr_triangles = (unsigned int) indices.size() / 3;
    if( normalPerVertex->getValue() ) {
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
      // if normal per vertex we can use arrays or vertex buffer objects
      // to render the geometry, they all use the same indices.
      if( prefer_vertex_buffer_object ) {
        // Create and bind vertex buffer objects for all the different
        // features.
        coordinate_node->renderVertexBufferObject();
        normal_node->renderVertexBufferObject();
        if( color_node ) color_node->renderVertexBufferObject();
        if( tex_coords_per_vertex )
          renderTexCoordVertexBufferObject( tex_coord_node );
        if( fog_coord_node ) fog_coord_node->renderVertexBufferObject();
        if( render_tangents ) {
          for( unsigned int attrib_index = 0;
               attrib_index < autoTangent->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              autoTangent->getValueByIndex( attrib_index );
            if( attr ) attr->renderVertexBufferObject();
          }
        }

        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->renderVertexBufferObject();
        }

        if( !vboFieldsUpToDate->isUpToDate() ) {
          // Only transfer data when it has been modified.
          vboFieldsUpToDate->upToDate();
          if( !vbo_id ) {
            vbo_id = new GLuint;
            glGenBuffersARB( 1, vbo_id );
          }
          glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *vbo_id );
          glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                           indices.size() * sizeof(GLuint),
                           &(*(indices.begin()) ), GL_STATIC_DRAW_ARB );
        } else {
          glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *vbo_id );
        }

        // Draw the triangles, the last parameter is NULL since vertex buffer
        // objects are used.
        if ( !render_patches || !GLEW_ARB_tessellation_shader ) {
          glDrawRangeElements( GL_TRIANGLES,
                               0,
                               coordinate_node->nrAvailableCoords() - 1,
                               3*nr_triangles,
                               GL_UNSIGNED_INT,
                               NULL );
        } else {
          glPatchParameteri(GL_PATCH_VERTICES, 3);
          glDrawRangeElements( GL_PATCHES,
                               0,
                               coordinate_node->nrAvailableCoords() - 1,
                               3*nr_triangles,
                               GL_UNSIGNED_INT,
                               NULL );
        }

        // Disable all vertex buffer objects.
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
        coordinate_node->disableVertexBufferObject();
        normal_node->disableVertexBufferObject();
        if( color_node ) color_node->disableVertexBufferObject();
        if( tex_coords_per_vertex )
          disableTexCoordVertexBufferObject( tex_coord_node );
        if( fog_coord_node) fog_coord_node->disableVertexBufferObject();
          for( unsigned int attrib_index = 0;
          attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->disableVertexBufferObject();
        } 

        if( render_tangents ) {
          for( unsigned int attrib_index = 0;
               attrib_index < autoTangent->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              autoTangent->getValueByIndex( attrib_index );
            if( attr ) attr->disableVertexBufferObject();
          }
        }
      } else {
        // Use vertex arrays.
        coordinate_node->renderArray();
        normal_node->renderArray();
        if( color_node ) color_node->renderArray();
        if( tex_coords_per_vertex ) renderTexCoordArray( tex_coord_node );
        if( fog_coord_node ) fog_coord_node->renderArray();
        if( render_tangents ) {
          for( unsigned int attrib_index = 0;
               attrib_index < autoTangent->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              autoTangent->getValueByIndex( attrib_index );
            if( attr ) attr->renderArray();
          }
        }

        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->renderArray();
        }

        if ( !render_patches || !GLEW_ARB_tessellation_shader ) {
          glDrawRangeElements( GL_TRIANGLES,
                               0,
                               coordinate_node->nrAvailableCoords() - 1,
                               3*nr_triangles, 
                               GL_UNSIGNED_INT,
                               &(*(indices.begin()) ) );
        } else {
          glPatchParameteri(GL_PATCH_VERTICES, 3);
          glDrawRangeElements( GL_PATCHES,
                               0,
                               coordinate_node->nrAvailableCoords() - 1,
                               3*nr_triangles, 
                               GL_UNSIGNED_INT,
                               &(*(indices.begin()) ) );
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

        if( render_tangents ) {
          for( unsigned int attrib_index = 0;
               attrib_index < autoTangent->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              autoTangent->getValueByIndex( attrib_index );
            if( attr ) attr->disableArray();
          }
        }
      }
    } else {
      // we cannot use arrays to render when normal_per_vertex is false
      // since the indexing into the normals are different from the rest.

      // Render either as triangles or patches
      if ( !render_patches || !GLEW_ARB_tessellation_shader ) {
        glBegin( GL_TRIANGLES );
      } else {
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glBegin( GL_PATCHES );
      }

      for( unsigned int i = 0; i < nr_triangles; ++i ) {
        normal_node->render( i );
        if( render_tangents ) {
          for( unsigned int attrib_index = 0;
               attrib_index < autoTangent->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              autoTangent->getValueByIndex( attrib_index );
            if( attr ) attr->render( i );
          }
        }

        unsigned int v = i * 3;
        // vertex 1
        if( color_node ) color_node->render( indices[v] );
        if( tex_coords_per_vertex ) renderTexCoord( indices[v],
          tex_coord_node );
        if( fog_coord_node) fog_coord_node->render(indices[v]);
        for( unsigned int attrib_index = 0;
          attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->render( indices[v] );
        }
        coordinate_node->render( indices[v] );

        // vertex 2
        if( color_node ) color_node->render( indices[v+1] );
        if( tex_coords_per_vertex ) renderTexCoord( indices[v+1],
          tex_coord_node );
        if( fog_coord_node) fog_coord_node->render(indices[v+1]);
        for( unsigned int attrib_index = 0;
          attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->render( indices[v+1] );
        }
        coordinate_node->render( indices[v+1] );

        // vertex 3
        if( color_node ) color_node->render( indices[v+2] );
        if( tex_coords_per_vertex ) renderTexCoord( indices[v+2],
          tex_coord_node );
        if( fog_coord_node) fog_coord_node->render(indices[v+2]);
        for( unsigned int attrib_index = 0;
          attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->render( indices[v+2] );
        }
        coordinate_node->render( indices[v+2] );
      }
      glEnd();
    }

    // restore previous fog attributes
    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPopAttrib();
    }

    // disable texture coordinate generation.
    if( tex_coord_gen ) {
      stopTexGen( tex_coord_node);
    }

    if ( color_node ) {
      color_node->postRender();
    } 
  } 
}

void IndexedTriangleSet::traverseSG( TraverseInfo &ti ) {
  X3DComposedGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );

  // In order to avoid problems with caching when the IndexedTriangleSet
  // is reused in several places in the scene graph where some places
  // require normals and some not, we always render tangents after 
  // one usage requires it. Otherwise it can  be cached as 
  // off if unlucky.
  // 
  bool * shader_requires_tangents = NULL;
  if( !render_tangents && 
      !ti.getUserData( "shaderRequiresTangents", 
                       (void **)&shader_requires_tangents) &&
      *shader_requires_tangents ) {  
    render_tangents = true;
    displayList->breakCache();
  }

  // If multiple uses of the IndexedTriangleSet then whether patches
  // are rendered or not is decided by the last usage to be traversed
  bool * shader_requires_patches = NULL;
  bool render_patches_new= ti.getUserData( "shaderRequiresPatches", 
                                           (void **)&shader_requires_patches)==0 && *shader_requires_patches;
  if ( render_patches_new != render_patches ) {
    render_patches= render_patches_new;
    if ( !GLEW_ARB_tessellation_shader ) {
      if ( render_patches_new ) {
        Console(4) << "Warning: Tessellation shaders are not supported by your graphics hardware! "
          "IndexedTriangleSet " << getName() << " will not be rendered as GL_PATCHES." << endl;
      }
    } else {
      displayList->breakCache();
    }
  }
}

void IndexedTriangleSet::AutoNormal::update() {
 // Console(4) << "Calculating normals" << endl;
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

void IndexedTriangleSet::AutoTangent::update() {
  bool normals_per_vertex = 
    static_cast< SFBool * >( routes_in[0] )->getValue();
  X3DCoordinateNode *coord = 
    static_cast< X3DCoordinateNode * >( static_cast< SFCoordinateNode * >
    ( routes_in[1] )->getValue() );
  const vector<int> &index = 
    static_cast< MFInt32 * >( routes_in[2] )->getValue();
  X3DTextureCoordinateNode *tex_coord = 
    static_cast< X3DTextureCoordinateNode * >( static_cast< SFTextureCoordinateNode * >
                                               ( routes_in[3] )->getValue() );

  IndexedTriangleSet *its = static_cast< IndexedTriangleSet * >( getOwner() );

  bool have_tangents_in_attrib = false; 
  bool have_binormals_in_attrib = false; 
  
  for( unsigned int i = 0; i < its->attrib->size(); ++i ) {
    X3DVertexAttributeNode *attr = its->attrib->getValueByIndex( i );
    if( attr ) {
      const string &name = attr->name->getValue();
      if( name == "tangent" ) have_tangents_in_attrib = true; 
      if( name == "binormal" ) have_binormals_in_attrib = true; 
    }
  }

  if( have_tangents_in_attrib ) {
    value.clear();
    return;
  }

  FloatVertexAttribute *tangent = NULL;
  FloatVertexAttribute *binormal = NULL;

  unsigned int nr_attribs_used = 0;

  if( !have_tangents_in_attrib ) {
    if( value.size() > 0 ) {
      tangent = static_cast< FloatVertexAttribute * >(value[0]);
    } else {
      tangent = new FloatVertexAttribute;
      value.push_back( tangent );
    }
    ++nr_attribs_used;
    //Console(4) << "Calculating tangents" << endl;
  }

  if( !have_binormals_in_attrib ) {
    unsigned int i = nr_attribs_used;
    
    if( value.size() > i ) {
      binormal = static_cast< FloatVertexAttribute * >(value[i]);
    } else {
      binormal = new FloatVertexAttribute;
      value.push_back( binormal );
    }
    
    //Console(4) << "Calculating binormal" << endl;
  }

  if( normals_per_vertex ) 
    generateTangentsPerVertex( coord, tex_coord, index, tangent, binormal );
  else
    generateTangentsPerFace( coord, tex_coord, index, tangent, binormal );
}


X3DNormalNode *IndexedTriangleSet::AutoNormal::generateNormalsPerVertex( 
  X3DCoordinateNode *coord,
  const vector< int > &index,
  bool ccw ) {
    Normal *normal = new Normal;
    if( coord ) {
      vector< Vec3f > normals( coord->nrAvailableCoords(), 
        Vec3f( 0, 0, 0 ) );
      for( unsigned int j = 0; j < index.size(); j+=3 ) {
        Vec3f norm, A, B, C, AB, BC;
        // make sure we have a valid face. If not use a dummy normal. 
        if( j+2 >= index.size() ) {
          norm =  Vec3f( 1, 0, 0 );
        } else {  
          // calculate a normal 
          A = coord->getCoord( index[ j ] );
          B = coord->getCoord( index[ j+1 ] );
          C = coord->getCoord( index[ j+2 ] );

          AB = B - A;
          BC = C - B;

          norm = AB % BC;
        }
        norm.normalizeSafe();
        if( !ccw ) 
          norm = -norm;
        normals[index[ j ]] += norm;
        normals[index[ j+1 ]] += norm;
        normals[index[ j+2 ]] += norm;
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

X3DNormalNode *IndexedTriangleSet::AutoNormal::generateNormalsPerFace( 
                                      X3DCoordinateNode *coord,
                                      const vector< int > &index,
                                      bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals;
    normals.reserve( index.size() / 3 );
    for( size_t j = 0; j < index.size(); j+=3 ) {
      Vec3f norm, A, B, C, AB, BC;
      // make sure we have a valid face. If not use a dummy normal. 
      if( j+2 >= index.size() ) {
        norm =  Vec3f( 1, 0, 0 );
      } else {  
        // calculate a normal for the triangle
        A = coord->getCoord( index[ j ] );
        B = coord->getCoord( index[ j + 1 ] );
        C = coord->getCoord( index[ j + 2 ] );
        
        AB = B - A;
        BC = C - B;
        
        norm = AB % BC;
      }
      
      try {
        norm.normalize();
      } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
        norm = Vec3f( 1, 0, 0 );
      }
      
      if( !ccw ) 
        norm = -norm;
      
      normals.push_back( norm );
    }
    normal->vector->setValue( normals );
  }
  return normal;
}



void IndexedTriangleSet::AutoTangent::generateTangentsPerVertex( 
                                                           X3DCoordinateNode *coord,
                                                           X3DTextureCoordinateNode *tex_coord,
                                                           const vector< int > &index,
                                                           FloatVertexAttribute *tangent_node,
                                                           FloatVertexAttribute *binormal_node) {
  if( tangent_node ) {
    tangent_node->name->setValue( "tangent" );
    tangent_node->numComponents->setValue( 3 );
    tangent_node->value->clear();
  }

  if( binormal_node ) {
    binormal_node->name->setValue( "binormal" );
    binormal_node->numComponents->setValue( 3 );
    binormal_node->value->clear();
  }

  if( coord ) {
    // the tangent and binormal for a vertex is the average of the tangent
    // of all triangles sharing that vertex.
    vector< float > tangents( coord->nrAvailableCoords() * 3, 0 );
    vector< float > binormals( coord->nrAvailableCoords() * 3, 0 );
    for( unsigned int j = 0; j < index.size(); j+=3 ) {
      Vec3f tangent, binormal;
      // make sure we have a valid face. If not use a dummy tangent. 
      if( j+2 >= index.size() ) {
        tangent =  Vec3f( 0, 1, 0 );
        binormal =  Vec3f( 0, 0, 1 );
      } else {  
        // calculate a tangent
        Vec3f a = coord->getCoord( index[ j ] );
        Vec3f b = coord->getCoord( index[ j + 1 ] );
        Vec3f c = coord->getCoord( index[ j + 2 ] );

        Vec3f ta = getTexCoord( coord, tex_coord, index[j] );
        Vec3f tb = getTexCoord( coord, tex_coord, index[j+1] );
        Vec3f tc = getTexCoord( coord, tex_coord, index[j+2] );
        
        calculateTangent( a, b, c,
                          ta, tb, tc,
                          tangent, binormal );

      }

      tangents[index[ j     ]*3   ] += tangent.x;
      tangents[index[ j     ]*3+1 ] += tangent.y;
      tangents[index[ j     ]*3+2 ] += tangent.z;
      tangents[index[ j + 1 ]*3   ] += tangent.x;
      tangents[index[ j + 1 ]*3+1 ] += tangent.y;
      tangents[index[ j + 1 ]*3+2 ] += tangent.z;
      tangents[index[ j + 2 ]*3   ] += tangent.x;
      tangents[index[ j + 2 ]*3+1 ] += tangent.y;
      tangents[index[ j + 2 ]*3+2 ] += tangent.z;

      binormals[index[ j     ]*3   ] += binormal.x;
      binormals[index[ j     ]*3+1 ] += binormal.y;
      binormals[index[ j     ]*3+2 ] += binormal.z;
      binormals[index[ j + 1 ]*3   ] += binormal.x;
      binormals[index[ j + 1 ]*3+1 ] += binormal.y;
      binormals[index[ j + 1 ]*3+2 ] += binormal.z;
      binormals[index[ j + 2 ]*3   ] += binormal.x;
      binormals[index[ j + 2 ]*3+1 ] += binormal.y;
      binormals[index[ j + 2 ]*3+2 ] += binormal.z;
 
     }
    
    for( unsigned int i = 0; i < tangents.size(); i+=3 ) {
      Vec3f t( tangents[i], tangents[i+1], tangents[i+2] );
      Vec3f b( binormals[i], binormals[i+1], binormals[i+2] );
      t.normalizeSafe();
      b.normalizeSafe();
      tangents[i] = t.x;
      tangents[i+1] = t.y;
      tangents[i+2] = t.z;
      binormals[i] = b.x;
      binormals[i+1] = b.y;
      binormals[i+2] = b.z;
    }

    /*
    cerr << "Tangents: " ;
    for( unsigned int i = 0; i < tangents.size(); ++i ) {
      cerr << tangents[i] << " ";
    }
    cerr << endl;
    cerr << "Binormal: ";
    for( unsigned int i = 0; i < binormals.size(); ++i ) {
      cerr << binormals[i] << " ";
    }
    cerr << endl;
    */

    if( tangent_node ) {
      tangent_node->value->setValue( tangents );
    }

    if( binormal_node ) {
      binormal_node->value->setValue( binormals );
    }
  }
}

void IndexedTriangleSet::AutoTangent::generateTangentsPerFace( 
                                                              X3DCoordinateNode *coord,
                                                              X3DTextureCoordinateNode *tex_coord,
                                                              const vector< int > &index,
                                                              FloatVertexAttribute *tangent_node,
                                                              FloatVertexAttribute *binormal_node ) {

  if( tangent_node ) {
    tangent_node->name->setValue( "tangent" );
    tangent_node->numComponents->setValue( 3 );
    tangent_node->value->clear();
  }
  
  if( binormal_node ) {
    binormal_node->name->setValue( "binormal" );
    binormal_node->numComponents->setValue( 3 );
    binormal_node->value->clear();
  }

  if( coord ) {
    vector< float > tangents;
    vector< float > binormals;

    tangents.reserve( index.size() / 3 );
    binormals.reserve( index.size() / 3 );

    for( size_t j = 0; j < index.size(); j+=3 ) {
      Vec3f tangent, binormal;
      // make sure we have a valid face. If not use a dummy normal. 
      if( j+2 >= index.size() ) {
        tangent =  Vec3f( 0, 1, 0 );
        binormal = Vec3f( 0, 0, 1 );
      } else {  
        // calculate a tangent
        Vec3f a = coord->getCoord( index[ j ] );
        Vec3f b = coord->getCoord( index[ j + 1 ] );
        Vec3f c = coord->getCoord( index[ j + 2 ] );

        Vec3f ta = getTexCoord( coord, tex_coord, index[j] );
        Vec3f tb = getTexCoord( coord, tex_coord, index[j+1] );
        Vec3f tc = getTexCoord( coord, tex_coord, index[j+2] );
        
        calculateTangent( a, b, c,
                          ta, tb, tc,
                          tangent, binormal );
      }
      
      try {
        binormal.normalize();
      } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
        binormal = Vec3f( 0, 1, 0 );
      }
      binormals.push_back( binormal.x );
      binormals.push_back( binormal.y );
      binormals.push_back( binormal.z );

      try {
        tangent.normalize();
      } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
        tangent = Vec3f( 0, 0, 1 );
      }
      tangents.push_back( tangent.x );
      tangents.push_back( tangent.y );
      tangents.push_back( tangent.z );

    }


    if( tangent_node ) {
      tangent_node->value->setValue( tangents );
    }

    if( binormal_node ) {
      binormal_node->value->setValue( binormals );
    }
  }
}



Vec3f IndexedTriangleSet::AutoTangent::getTexCoord( X3DCoordinateNode *coord,
                                                    X3DTextureCoordinateNode *tex_coord,
                                                    int index ) {
  if( tex_coord ) {
    if( tex_coord->supportsGetTexCoord( 0 ) ) {
      Vec4f tc = tex_coord->getTexCoord( index, 0 );
      return Vec3f( tc.x, tc.y, tc.z ) / tc.w;
    } else {
      Console(4) << "Warning: X3DTextureCoordinateNode does not support getTexCoord() function. Tangents and binormals cannot be calculated for IndexedTriangleSet." << endl;
    }
  } else {
    IndexedTriangleSet *its = static_cast< IndexedTriangleSet * >( getOwner() );
    Matrix4f to_str = its->getDefaultTexGenMatrix();
    return to_str * coord->getCoord( index );
  }

  return Vec3f(0,0,0);
}

void IndexedTriangleSet::AutoTangent::calculateTangent( const Vec3f &a, const Vec3f &b, const Vec3f &c,
                                                        const Vec3f &ta, const Vec3f &tb, const Vec3f &tc,
                                                        Vec3f &tangent, Vec3f &binormal ) {
  const Vec3f& v1 = a;
  const Vec3f& v2 = b;
  const Vec3f& v3 = c;
  
  Vec2f w1(ta.x, ta.y );
  Vec2f w2(tb.x, tb.y );
  Vec2f w3(tc.x, tc.y) ;
    
  H3DFloat x1 = v2.x - v1.x;
  H3DFloat x2 = v3.x - v1.x;
  H3DFloat y1 = v2.y - v1.y;
  H3DFloat y2 = v3.y - v1.y;
  H3DFloat z1 = v2.z - v1.z;
  H3DFloat z2 = v3.z - v1.z;
  
  H3DFloat s1 = w2.x - w1.x;
  H3DFloat s2 = w3.x - w1.x;
  H3DFloat t1 = w2.y - w1.y;
  H3DFloat t2 = w3.y - w1.y;
  
  H3DFloat denom = (s1 * t2 - s2 * t1);

  if( denom == 0 ) {
    tangent = Vec3f( 0, 0, 0 );
    binormal = Vec3f( 0, 0, 0 );
    return;
  }

  H3DFloat r = 1.0F / denom;
  Vec3f sdir((t2 * x1 - t1 * x2) * r, 
             (t2 * y1 - t1 * y2) * r,
             (t2 * z1 - t1 * z2) * r);
  Vec3f tdir((s1 * x2 - s2 * x1) * r, 
             (s1 * y2 - s2 * y1) * r,
             (s1 * z2 - s2 * z1) * r);

  tangent = sdir;
  binormal = tdir;
}
