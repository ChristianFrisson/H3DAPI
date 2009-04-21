//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
//
//    This file is part of H3D API.
//
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
/// \file IndexedFaceSet.cpp
/// \brief CPP file for IndexedFaceSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/IndexedFaceSet.h>
#include <H3D/Normal.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase IndexedFaceSet::database( "IndexedFaceSet", 
                                          &(newInstance<IndexedFaceSet>), 
                                          typeid( IndexedFaceSet ),
                                          &X3DComposedGeometryNode::database);

namespace IndexedFaceSetInternals {
  FIELDDB_ELEMENT( IndexedFaceSet, set_colorIndex, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedFaceSet, set_coordIndex, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedFaceSet, set_normalIndex, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedFaceSet, set_texCoordIndex, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedFaceSet, colorIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedFaceSet, convex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedFaceSet, coordIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedFaceSet, creaseAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedFaceSet, normalIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedFaceSet, texCoordIndex, INPUT_OUTPUT );
}


IndexedFaceSet::IndexedFaceSet( 
                    Inst< SFNode                  > _metadata,
                    Inst< SFBound                 > _bound,
                    Inst< DisplayList             > _displayList,
                    Inst< SFColorNode             > _color,
                    Inst< SFCoordinateNode        >  _coord,
                    Inst< SFNormalNode            > _normal,
                    Inst< SFTextureCoordinateNode >  _texCoord,
                    Inst< SFBool       > _ccw,
                    Inst< SFBool       > _colorPerVertex,
                    Inst< SFBool       >  _normalPerVertex,
                    Inst< SFBool       > _solid,
                    Inst< MFVertexAttributeNode > _attrib,
                    Inst< AutoNormal   > _autoNormal,
                    Inst< SFBool       > _convex,
                    Inst< SFFloat      > _creaseAngle,
                    Inst< MFInt32      > _set_colorIndex,
                    Inst< MFInt32      > _set_coordIndex,
                    Inst< MFInt32      > _set_normalIndex,
                    Inst< MFInt32      > _set_texCoordIndex,
                    Inst< MFInt32      > _colorIndex,
                    Inst< MFInt32      > _coordIndex,
                    Inst< MFInt32      > _normalIndex,
                    Inst< MFInt32      > _texCoordIndex,
                    Inst< SFFogCoordinate         > _fogCoord ): 
  X3DComposedGeometryNode( _metadata, _bound, _displayList, _color, _coord,  _normal, 
                           _texCoord, _ccw, _colorPerVertex, 
                           _normalPerVertex, _solid, _attrib, _fogCoord ),
  set_colorIndex   ( _set_colorIndex    ),
  set_coordIndex   ( _set_coordIndex    ),
  set_normalIndex  ( _set_normalIndex   ),
  set_texCoordIndex( _set_texCoordIndex ),
  colorIndex       ( _colorIndex        ),
  convex           ( _convex            ),
  coordIndex       ( _coordIndex        ),
  creaseAngle      ( _creaseAngle       ),
  normalIndex      ( _normalIndex       ),
  texCoordIndex    ( _texCoordIndex     ),
  autoNormal      ( _autoNormal       ) {
  
  type_name = "IndexedFaceSet";
  database.initFields( this );

  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );
  
  normalPerVertex->route( autoNormal );
  coord->route( autoNormal );
  coordIndex->route( autoNormal );
  ccw->route( autoNormal );
  creaseAngle->route( autoNormal );

  coord->route( bound );

  set_coordIndex->route( coordIndex, id );
  set_normalIndex->route( normalIndex, id );
  set_texCoordIndex->route( texCoordIndex, id );
  set_colorIndex->route( colorIndex, id );

  coordIndex->route( displayList );
  normalIndex->route( displayList );
  texCoordIndex->route( displayList );
  colorIndex->route( displayList );
  autoNormal->route( displayList );
  convex->route( displayList );

  ccw->setValue( true );
  colorPerVertex->setValue( true );
  convex->setValue( true );
  creaseAngle->setValue( 0 );
  normalPerVertex->setValue( true );
  solid->setValue( true );
}


void IndexedFaceSet::render() {
  //  X3DCoordinateNode *coords = static_cast< X3DCoordinateNode * >( coord->getValue() );
  X3DCoordinateNode *coords = getCoord();
  X3DTextureCoordinateNode *tex_coords = texCoord->getValue();

  X3DColorNode *colors = color->getValue();
  X3DNormalNode *normals = normal->getValue();
  bool using_auto_normals = normals == NULL;
  if( !normals ) {
    normals = autoNormal->getValue();
  }

  bool tex_coord_gen = !tex_coords || (tex_coords && tex_coords->supportsTexGen());
  bool tex_coords_per_vertex = tex_coords && tex_coords->supportsExplicitTexCoords();

  const vector< int > &color_index     = colorIndex->getValue();
  const vector< int > &coord_index     = coordIndex->getValue();
  const vector< int > &normal_index    = normalIndex->getValue();
  const vector< int > &tex_coord_index = texCoordIndex->getValue();

  //  glEnable( GL_AUTO_NORMAL );

  // we need coordinates to render 
  if( coords ) {
    // if no X3DTextureCoordinateNode, we generate texture coordinates
    // based on the bounding box according to the X3D specification.
    // also if X3DTextureCoordinateNode is generated based the texture
    // coordinate generation will start
    if( tex_coord_gen ) startTexGen( tex_coords );

    // if we have a color node we use the color from that instead
    // of the previously installed Material node.
    if ( colors ) {
      colors->preRender();
    } 

    GLhandleARB shader_program = 0;
    // Set the attribute index to use for all vertex attributes
    if( GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader ) {
      shader_program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );
      if( shader_program ) {
        for( unsigned int i = 0; i < attrib->size(); i++ ) {
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

    // index of the current face being rendered. It will be incremented
    // for each face that is rendered.
    unsigned int face_count = 0;
    
    // index of the current vertex. It will be incremented for each vertex
    // that is rendered. 
    unsigned int vertex_count = 0;

    //    if texcoord and tex_coord_index check size is equal with coord_index;
    
    for( unsigned int i = 0; i < coord_index.size(); i++ ) {
      glBegin( GL_POLYGON );
      // set up normals if the normals are specified per face
      if( normals ) {
        if ( !normalPerVertex->getValue() || (using_auto_normals && 
                                              creaseAngle->getValue() <= 0 ) ) {
          int ni;
          if ( normal_index.size() == 0 || using_auto_normals ) {
            ni = face_count;
          } else {
            ni = normal_index[ face_count ];
          }
          normals->render( ni );
        }
      }

      // set up colors if the colors are specified per face
      if( colors ) {
        if ( !colorPerVertex->getValue() ) {
          int ci;
          if ( color_index.size() == 0 ) {
            ci = face_count;
          } else {
            ci = color_index[ face_count ];
          }
          colors->render( ci );
        }
      }
      
      // render all vertices for this face.
      for(;  i < coord_index.size() && coord_index[i] != -1; i++ ) {
        // Set up texture coordinates.
        if( tex_coords_per_vertex ) {
          int tci;
          if( tex_coord_index.size() == 0 ) {
            tci = coord_index[ i ];
          } else {
            tci = tex_coord_index[i];
          }
    
          if( tci == -1 ) {
            stringstream s;
            s << "-1 mismatch between coord_index and tex_coord_index in \"" 
              << getName()<< "\" node. Must be of equal length and have -1 in "
              << "the same places. ";
            throw InvalidTexCoordIndex( tci, s.str() );
          } else {
            renderTexCoord( tci, tex_coords );
          } 
        } 
  
        // Set up normals if the normals are specified per vertex.
        if( normals ) {
          if ( normalPerVertex->getValue() && 
               !( using_auto_normals &&
                  creaseAngle->getValue() <= 0 ) ) {
            if( !using_auto_normals ) {
              int ni;
              if ( normal_index.size() == 0 ) {
                ni = coord_index[ i ];
              } else {
                ni = normal_index[ i ];
              }
              
              if( ni == -1 ) {
                stringstream s;
                s << "-1 mismatch between coord_index and normal_index in \"" 
                  << getName()
                  << "\" node. Must be of equal length and have -1 in "
                  << "the same places. ";
                throw InvalidNormalIndex( ni, s.str() );
              } else {
                normals->render( ni );
              }
            } else {
              // normals have been automatically generated.
              if( creaseAngle->getValue() < Constants::pi )
                normals->render( vertex_count );
              else 
                normals->render( coord_index[i] );
            }
          }
        } 
  
        // Set up colors.
        if( colors ) {
          if ( colorPerVertex->getValue() ) {
            int ci;
            if ( color_index.size() == 0 ) {
              ci = coord_index[ i ];
            } else {
              ci = color_index[ i ];
            }
      
            if( ci == -1 ) {
              stringstream s;
              s << "-1 mismatch between coord_index and color_index in \"" 
                << getName() 
                << "\" node. Must be of equal length and have -1 in "
                << "the same places. ";
              throw InvalidColorIndex( ci, s.str() );
            } else {
              colors->render( ci );
            }
          }
        } 
  
        // Set up shader vertex attributes.
        if( shader_program ) {
          for( unsigned int j = 0; j < attrib->size(); j++ ) {
            X3DVertexAttributeNode *attr =  attrib->getValueByIndex( j );
            if( attr ) {
              attr->render( coord_index[i] );
            }
          }
        }
        // Set up fogCoordinates
        if(fogCoord->getValue()){
           fogCoord->getValue()->render(coord_index[i]);
        }

        // Render the vertices.
        coords->render( coord_index[ i ] );
        vertex_count++;
      }
      glEnd();
      face_count++;
    }

    // disable texture coordinate generation.
    if( tex_coord_gen ) stopTexGen( tex_coords );

    if ( colors ) {
      colors->postRender();
    } 
  }
}

void IndexedFaceSet::AutoNormal::update() {
  bool normals_per_vertex = 
    static_cast< SFBool * >( routes_in[0] )->getValue();
  X3DCoordinateNode *coord = 
    static_cast< X3DCoordinateNode * >( static_cast< SFCoordinateNode * >
                                        ( routes_in[1] )->getValue() );
  const vector<int> &index = 
    static_cast< MFInt32 * >( routes_in[2] )->getValue();
  bool ccw = static_cast< SFBool * >( routes_in[3] )->getValue();
  H3DFloat crease_angle = static_cast< SFFloat * >( routes_in[4] )->getValue();
  if( normals_per_vertex && crease_angle > 0 ) {
    if( crease_angle < Constants::pi )
      value = generateNormalsPerVertex( coord, index, ccw, crease_angle );
    else 
      value = generateNormalsPerVertex( coord, index, ccw );
  } else {
    value = generateNormalsPerFace( coord, index, ccw );
  }
}


X3DNormalNode *IndexedFaceSet::AutoNormal::generateNormalsPerVertex( 
                                   X3DCoordinateNode *coord,
                                   const vector< int > &coord_index,
                                   bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals( coord->nrAvailableCoords(), 
                             Vec3f( 0, 0, 0 ) );
    AutoRef< X3DNormalNode > normals_per_face;
    normals_per_face.reset( generateNormalsPerFace( coord,
                                                    coord_index,
                                                    ccw ) );
    unsigned int i = 0;
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); face++ ) {
      Vec3f norm = normals_per_face->getNormal( face );
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        normals[coord_index[i++]]   += norm;
      }
      i++;
    }
    
    for( vector<Vec3f>::iterator i = normals.begin(); 
         i != normals.end(); 
         i++ ) {
      (*i).normalizeSafe();
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

X3DNormalNode *IndexedFaceSet::AutoNormal::generateNormalsPerVertex( 
                                   X3DCoordinateNode *coord,
                                   const vector< int > &coord_index,
                                   bool ccw,
                                   H3DFloat crease_angle ) {
  Normal *normal = new Normal;
  if( coord ) {
    AutoRef< X3DNormalNode > normals_per_face;
    normals_per_face.reset( generateNormalsPerFace( coord,
                                                    coord_index,
                                                    ccw ) );
    unsigned int i = 0;
    vector< Vec3f > normals( coord_index.size(),
                             Vec3f( 0, 0, 0 ) );
    normals.clear();

    std::vector< vector< int > > point_to_face_normal_map;
    point_to_face_normal_map.resize( coord->nrAvailableCoords() );

    H3DFloat cos_crease_angle = H3DCos( crease_angle );

    // build a map from each vertex to a vector of all the normals 
    // of the faces the vertex is a part of.
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); face++ ) {
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        int index = coord_index[i++];
        vector< int > &v =point_to_face_normal_map[ index ];
        v.push_back( face );
      }
      i++;
    }

    i = 0;
    
    // the normal for the vertex is the average of all normals of the faces
    // which normal angle from the current face is less than the crease angle
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); face++ ) {
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        int index = coord_index[i++];
        Vec3f face_normal = normals_per_face->getNormal( face );
        Vec3f point_normal( 0, 0, 0 );

        const vector< int > &face_normals = 
          point_to_face_normal_map[ index ];
        
        for( vector< int >::const_iterator n = face_normals.begin();
             n != face_normals.end(); n++ ) {
          // a < b <=> cos(a) > cos(b)
          Vec3f nn = normals_per_face->getNormal( *n );
          if( face_normal * nn > cos_crease_angle ) {
            point_normal += nn;
          }
        }
        point_normal.normalizeSafe();
        normals.push_back( point_normal );
      }
      i++;
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

X3DNormalNode *IndexedFaceSet::AutoNormal::generateNormalsPerFace( 
                                             X3DCoordinateNode *coord,
                                             const vector< int > &coord_index,
                                             bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals;
    for( size_t j = 0; j < coord_index.size(); ) {
      Vec3f norm, A, B, C, AB, BC;
      // make sure we have a valid face. If not use a dummy normal. 
      if( j+2 >= coord_index.size() ||
          coord_index[j]   == -1 ||
          coord_index[j+1] == -1 ||
          coord_index[j+2] == -1 ) {
        norm =  Vec3f( 1, 0, 0 );
      } else {  
        // we try to calculate a normal using the first three vertices
        // in the face.
        A = coord->getCoord( coord_index[ j++ ] );
        B = coord->getCoord( coord_index[ j++ ] );
        C = coord->getCoord( coord_index[ j++ ] );
      
        AB = B - A;
        BC = C - B;

        norm = AB % BC;
      }
    
      try {
        norm.normalize();
      } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
        // normalization failed so we have a zero vector. Since most normals
        // are ok we don't do any checks until one fails.
      
        // make sure AB is not a zero vector.
        while( AB*AB < Constants::f_epsilon &&
               j < coord_index.size() &&
               coord_index[j] != -1 ) {
          A = coord->getCoord( coord_index[ j++ ] ); 
          AB = B - A;
        }

        norm = AB % BC;
        H3DFloat l = norm.length();
        // check if zero vector
        if( l > 0 ) {
          // if not normalize and add to vector.
          norm = norm / l;
        } else {
          // try to find an edge that together with AB can generate
          // a normal
          while( j < coord_index.size() && coord_index[j] != -1 ) {
            C = coord->getCoord( coord_index[ j++ ] );
            BC = B - C;
            norm = AB % BC;
            l = norm.length();
            if( l > 0 ) {
              norm = norm / l;
              break;
            }
          }
          // we did not find any edges that could be used for 
          // generate a normal so just add a dummy normal.
          if( j >= coord_index.size() ||
              coord_index[j] == -1 ) {
            norm = Vec3f( 1, 0, 0 );
          }
        }
      }
      // skip the rest of the vertices in the face.
      while( j < coord_index.size() && coord_index[j++] != -1 );

      if( !ccw ) 
        norm = -norm;

      normals.push_back( norm );
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

void IndexedFaceSet::traverseSG( TraverseInfo &ti ) {
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );

  X3DComposedGeometryNode::traverseSG( ti );
}


