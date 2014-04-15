//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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
  autoNormal       ( _autoNormal       ),
  autoTangent      ( new AutoTangent ),
  render_tangents( false )  {

  type_name = "IndexedFaceSet";
  database.initFields( this );

  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );
  autoTangent->setName( "autoTangent" );
  autoTangent->setOwner( this );
  
 
  normalPerVertex->route( autoNormal );
  coord->route( autoNormal );
  coordIndex->route( autoNormal );
  ccw->route( autoNormal );
  creaseAngle->route( autoNormal );

  normalPerVertex->route( autoTangent );
  coord->route( autoTangent );
  coordIndex->route( autoTangent );
  texCoord->route( autoTangent );
  texCoordIndex->route( autoTangent );
  creaseAngle->route( autoTangent );
  ccw->route( autoTangent );

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
  FogCoordinate *fog_coords = fogCoord->getValue();

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

    // set fog to get fog depth from fog coordinates if available
    if( GLEW_EXT_fog_coord && fog_coords ) {
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

    // index of the current face being rendered. It will be incremented
    // for each face that is rendered.
    unsigned int face_count = 0;
    
    // index of the current vertex. It will be incremented for each vertex
    // that is rendered. 
    unsigned int vertex_count = 0;

    //    if texcoord and tex_coord_index check size is equal with coord_index;
    
    for( unsigned int i = 0; i < coord_index.size(); ++i ) {
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

          // render tangents(they have the same layout as normals)
          if( render_tangents ) {
            for( unsigned int attrib_index = 0;
                 attrib_index < autoTangent->size(); ++attrib_index ) {
              X3DVertexAttributeNode *attr = 
                autoTangent->getValueByIndex( attrib_index );
              if( attr ) attr->render( ni );
            }
          }
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
      for(;  i < coord_index.size() && coord_index[i] != -1; ++i ) {
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
                // render tangents(they have the same layout as normals)
                if( render_tangents ) {
                  for( unsigned int attrib_index = 0;
                       attrib_index < autoTangent->size(); ++attrib_index ) {
                    X3DVertexAttributeNode *attr = 
                      autoTangent->getValueByIndex( attrib_index );
                    if( attr ) attr->render( ni );
                  }
                }
              }
            } else {
              // normals have been automatically generated.
              int ni;

              if( creaseAngle->getValue() < Constants::pi ) {
                ni = vertex_count;
              } else  {
                ni = coord_index[i];
              }

              normals->render( ni );

              // render tangents(they have the same layout as normals)
              if( render_tangents ) {
                for( unsigned int attrib_index = 0;
                     attrib_index < autoTangent->size(); ++attrib_index ) {
                  X3DVertexAttributeNode *attr = 
                    autoTangent->getValueByIndex( attrib_index );
                  if( attr ) attr->render( ni );
                }
              }
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
          for( unsigned int j = 0; j < attrib->size(); ++j ) {
            X3DVertexAttributeNode *attr =  attrib->getValueByIndex( j );
            if( attr ) {
              attr->render( coord_index[i] );
            }
          }
        }
        // Set up fogCoordinates
        if(fog_coords ){
           fog_coords->render(coord_index[i]);
        }

        // Render the vertices.
        coords->render( coord_index[ i ] );
        ++vertex_count;
      }
      glEnd();
      ++face_count;
    }

    // restore previous fog attributes
    if( GLEW_EXT_fog_coord && fog_coords ) {
      glPopAttrib();
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
         face < normals_per_face->nrAvailableNormals(); ++face ) {
      Vec3f norm = normals_per_face->getNormal( face );
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        normals[coord_index[i++]]   += norm;
      }
      ++i;
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
         face < normals_per_face->nrAvailableNormals(); ++face ) {
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        int index = coord_index[i++];
        vector< int > &v =point_to_face_normal_map[ index ];
        v.push_back( face );
      }
      ++i;
    }

    i = 0;
    
    // the normal for the vertex is the average of all normals of the faces
    // which normal angle from the current face is less than the crease angle
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); ++face ) {
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        int index = coord_index[i++];
        Vec3f face_normal = normals_per_face->getNormal( face );
        Vec3f point_normal( 0, 0, 0 );

        const vector< int > &face_normals = 
          point_to_face_normal_map[ index ];
        
        for( vector< int >::const_iterator n = face_normals.begin();
             n != face_normals.end(); ++n ) {
          // a < b <=> cos(a) > cos(b)
          Vec3f nn = normals_per_face->getNormal( *n );
          if( face_normal * nn > cos_crease_angle ) {
            point_normal += nn;
          }
        }
        point_normal.normalizeSafe();
        normals.push_back( point_normal );
      }
      ++i;
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
}


Vec3f IndexedFaceSet::AutoTangent::getTexCoord( X3DCoordinateNode *coord,
                                                    X3DTextureCoordinateNode *tex_coord,
                                                    int index ) {
  if( tex_coord ) {
    if( tex_coord->supportsGetTexCoord( 0 ) ) {
      Vec4f tc = tex_coord->getTexCoord( index, 0 );
      return Vec3f( tc.x, tc.y, tc.z ) / tc.w;
    } else {
      Console(4) << "Warning: X3DTextureCoordinateNode does not support getTexCoord() function. Tangents and binormals cannot be calculated for IndexedFaceSet." << endl;
    }
  } else {
    IndexedFaceSet *its = static_cast< IndexedFaceSet * >( getOwner() );
    Matrix4f to_str = its->getDefaultTexGenMatrix();
    return to_str * coord->getCoord( index );
  }

  return Vec3f(0,0,0);
}

void IndexedFaceSet::AutoTangent::calculateTangent( const Vec3f &a, const Vec3f &b, const Vec3f &c,
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


void IndexedFaceSet::AutoTangent::update() {
  bool normals_per_vertex = 
    static_cast< SFBool * >( routes_in[0] )->getValue();
  X3DCoordinateNode *coord = 
    static_cast< X3DCoordinateNode * >( static_cast< SFCoordinateNode * >
                                        ( routes_in[1] )->getValue() );
  const vector<int> &coord_index = 
    static_cast< MFInt32 * >( routes_in[2] )->getValue();

  X3DTextureCoordinateNode *tex_coord = 
    static_cast< X3DTextureCoordinateNode * >( static_cast< SFTextureCoordinateNode * >
                                               ( routes_in[3] )->getValue() );
  
  const vector<int> &tex_coord_index = 
    static_cast< MFInt32 * >( routes_in[4] )->getValue();

  H3DFloat crease_angle = static_cast< SFFloat * >( routes_in[5] )->getValue();
  bool ccw = static_cast< SFBool * >( routes_in[6] )->getValue();
  
  if( value.empty() ) {
    value.push_back( new FloatVertexAttribute );
    value.push_back( new FloatVertexAttribute );
  }

  FloatVertexAttribute *tangent = static_cast< FloatVertexAttribute * >(value[0]);
  FloatVertexAttribute *binormal = static_cast< FloatVertexAttribute * >(value[1]);
  if( normals_per_vertex && crease_angle > 0 ) {
    if( crease_angle < Constants::pi ) {
      generateTangentsPerVertex( coord, tex_coord, 
                                 coord_index, 
                                 tex_coord_index.empty() ? 
                                 coord_index : tex_coord_index, 
                                 crease_angle, ccw,
                                 tangent, binormal );
    } else { 
      generateTangentsPerVertex( coord, tex_coord, 
                                 coord_index, 
                                 tex_coord_index.empty() ? 
                                 coord_index : tex_coord_index, 
                                 tangent, binormal );
    }
  } else {
    generateTangentsPerFace(  coord, tex_coord, 
                              coord_index, 
                              tex_coord_index.empty() ? 
                              coord_index : tex_coord_index, 
                              true,
                              tangent, binormal );
  }
}


void IndexedFaceSet::AutoTangent::generateTangentsPerFace( 
                                             X3DCoordinateNode *coord,
                                             X3DTextureCoordinateNode *tex_coord,
                                             const vector< int > &coord_index,
                                             const vector< int > &tex_coord_index,
                                             bool normalize_values,
                                             FloatVertexAttribute *tangent_node,
                                             FloatVertexAttribute *binormal_node ) {
  tangent_node->name->setValue( "tangent" );
  tangent_node->numComponents->setValue( 3 );
  tangent_node->value->clear();
  binormal_node->name->setValue( "binormal" );
  binormal_node->numComponents->setValue( 3 );
  binormal_node->value->clear();

  if( coord ) {
    vector< float > tangents;
    vector< float > binormals;

    tangents.reserve( coord_index.size() / 3 );
    binormals.reserve( tex_coord_index.size() / 3 );

    for( size_t j = 0; j < coord_index.size(); ) {
      Vec3f tangent, binormal;
      // make sure we have a valid face. If not use a dummy normal. 
      if( j+2 >= coord_index.size() ||
          coord_index[j]   == -1 ||
          coord_index[j+1] == -1 ||
          coord_index[j+2] == -1 ) {
        tangent =  Vec3f( 0, 1, 0 );
        binormal = Vec3f( 0, 0, 1 );
      } else {  
        Vec3f a = coord->getCoord( coord_index[ j ] );
        Vec3f b = coord->getCoord( coord_index[ j + 1 ] );
        Vec3f c = coord->getCoord( coord_index[ j + 2 ] );
        
        Vec3f ta = getTexCoord( coord, tex_coord, tex_coord_index[j] );
        Vec3f tb = getTexCoord( coord, tex_coord, tex_coord_index[j+1] );
        Vec3f tc = getTexCoord( coord, tex_coord, tex_coord_index[j+2] );

        calculateTangent( a, b, c,
                          ta, tb, tc,
                          tangent, binormal );
        
        j += 3;

      }
    
      // Calculation only works for triangle meshes so print a warning
      // if it is not a triangle mesh.
      if( j < coord_index.size() && coord_index[j] != -1 ) {
        Console(4) << "Warning: Automatic tangent vertex attribute generation only"
                   << " supported for triangle meshes. IndexedFaceSet node "
                   << "includes non-triangle faces so results are undefined" << endl;
      }

      // skip the rest of the vertices in the face.
      while( j < coord_index.size() && coord_index[j++] != -1 );


      if( normalize_values ) {
        try {
          binormal.normalize();
        } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
          binormal = Vec3f( 0, 1, 0 );
        }

        try {
          tangent.normalize();
        } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
          tangent = Vec3f( 0, 0, 1 );
        }
      }
      binormals.push_back( binormal.x );
      binormals.push_back( binormal.y );
      binormals.push_back( binormal.z );
      tangents.push_back( tangent.x );
      tangents.push_back( tangent.y );
      tangents.push_back( tangent.z );
    }
    tangent_node->value->setValue( tangents );
    binormal_node->value->setValue( binormals );
  }
}

void IndexedFaceSet::AutoTangent::generateTangentsPerVertex( 
                                X3DCoordinateNode *coord,
                                X3DTextureCoordinateNode *tex_coord,
                                const vector< int > &coord_index,
                                const vector< int > &tex_coord_index,
                                FloatVertexAttribute *tangent_node,
                                FloatVertexAttribute *binormal_node ) {
  tangent_node->name->setValue( "tangent" );
  tangent_node->numComponents->setValue( 3 );
  tangent_node->value->clear();
  binormal_node->name->setValue( "binormal" );
  binormal_node->numComponents->setValue( 3 );
  binormal_node->value->clear();

  if( coord ) {
    // the tangent and binormal for a vertex is the average of the tangent
    // of all triangles sharing that vertex.
    vector< float > tangents( coord->nrAvailableCoords() * 3, 0 );
    vector< float > binormals( coord->nrAvailableCoords() * 3, 0 );

    AutoRef< FloatVertexAttribute > tangents_per_face_node( new FloatVertexAttribute );
    AutoRef< FloatVertexAttribute > binormals_per_face_node( new FloatVertexAttribute );
    generateTangentsPerFace( coord,
                             tex_coord,
                             coord_index,
                             tex_coord_index,
                             false,
                             tangents_per_face_node.get(),
                             binormals_per_face_node.get() );

    const vector< H3DFloat > &tangents_per_face = 
      tangents_per_face_node->value->getValue();
    const vector< H3DFloat > &binormals_per_face = 
      binormals_per_face_node->value->getValue();

    unsigned int i = 0;
    for( unsigned int face = 0; face < tangents_per_face.size()/3; ++face ) {
       // add tangent and binormal value from each face to all vertices
       // the face has.
       while( i < coord_index.size() && coord_index[i] != -1 ) {    
         tangents[coord_index[ i ]*3   ] += tangents_per_face[face*3  ];
         tangents[coord_index[ i ]*3+1 ] += tangents_per_face[face*3+1];
         tangents[coord_index[ i ]*3+2 ] += tangents_per_face[face*3+2];
         binormals[coord_index[ i ]*3   ] += binormals_per_face[face*3  ];
         binormals[coord_index[ i ]*3+1 ] += binormals_per_face[face*3+1];
         binormals[coord_index[ i ]*3+2 ] += binormals_per_face[face*3+2];
         ++i;
       }
       ++i;
    }
    
    // normalize tangents and binormals
           
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

    tangent_node->value->setValue( tangents );
    binormal_node->value->setValue( binormals );
  }
}


void IndexedFaceSet::AutoTangent::generateTangentsPerVertex( 
                                X3DCoordinateNode *coord,
                                X3DTextureCoordinateNode *tex_coord,
                                const vector< int > &coord_index,
                                const vector< int > &tex_coord_index,
                                H3DFloat crease_angle,
                                bool ccw,
                                FloatVertexAttribute *tangent_node,
                                FloatVertexAttribute *binormal_node ) {
  tangent_node->name->setValue( "tangent" );
  tangent_node->numComponents->setValue( 3 );
  tangent_node->value->clear();
  binormal_node->name->setValue( "binormal" );
  binormal_node->numComponents->setValue( 3 );
  binormal_node->value->clear();

  IndexedFaceSet *ifs = static_cast< IndexedFaceSet * >( getOwner() );

  if( coord ) {
    AutoRef< X3DNormalNode > normals_per_face;
    normals_per_face.reset( ifs->autoNormal->generateNormalsPerFace( coord,
                                                                     coord_index,
                                                                     ccw ) );

    AutoRef< FloatVertexAttribute > tangents_per_face_node( new FloatVertexAttribute );
    AutoRef< FloatVertexAttribute > binormals_per_face_node( new FloatVertexAttribute );
    generateTangentsPerFace( coord,
                             tex_coord,
                             coord_index,
                             tex_coord_index,
                             false,
                             tangents_per_face_node.get(),
                             binormals_per_face_node.get() );

    const vector< H3DFloat > &tangents_per_face = 
      tangents_per_face_node->value->getValue();
    const vector< H3DFloat > &binormals_per_face = 
      binormals_per_face_node->value->getValue();
    
    unsigned int i = 0;
     // the tangent and binormal for a vertex is the average of the tangent
    // of all triangles sharing that vertex.
    vector< float > tangents;
    vector< float > binormals;

    tangents.reserve( coord_index.size() * 3 );
    binormals.reserve( coord_index.size() * 3 );

    std::vector< vector< int > > point_to_face_normal_map;
    point_to_face_normal_map.resize( coord->nrAvailableCoords() );

    H3DFloat cos_crease_angle = H3DCos( crease_angle );

    // build a map from each vertex to a vector of all the normals 
    // of the faces the vertex is a part of.
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); ++face ) {
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        int index = coord_index[i++];
        vector< int > &v = point_to_face_normal_map[ index ];
        v.push_back( face );
      }
      ++i;
    }

    i = 0;
    
    // the tangent and binormal for the vertex is the average of all tangents
    // and binormals of the faces which normal angle from the current face
    // is less than the crease angle
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); ++face ) {
      while( i < coord_index.size() && coord_index[i] != -1 ) {    
        int index = coord_index[i++];
        Vec3f face_normal = normals_per_face->getNormal(face);
        Vec3f point_tangent( 0, 0, 0 );
        Vec3f point_binormal( 0, 0, 0 );

        const vector< int > &face_normals = 
          point_to_face_normal_map[ index ];
        
        for( vector< int >::const_iterator n = face_normals.begin();
             n != face_normals.end(); ++n ) {
          // a < b <=> cos(a) > cos(b)
          Vec3f nn = normals_per_face->getNormal( *n );
          if( face_normal * nn > cos_crease_angle ) {
            point_tangent.x += tangents_per_face[(*n) * 3];
            point_tangent.y += tangents_per_face[(*n) * 3 + 1];
            point_tangent.z += tangents_per_face[(*n) * 3 + 2];
            point_binormal.x += binormals_per_face[(*n) * 3];
            point_binormal.y += binormals_per_face[(*n) * 3 + 1];
            point_binormal.z += binormals_per_face[(*n) * 3 + 2];
          }
        }

        point_tangent.normalizeSafe();
        point_binormal.normalizeSafe();
        tangents.push_back( point_tangent.x );
        tangents.push_back( point_tangent.y );
        tangents.push_back( point_tangent.z );
        binormals.push_back( point_binormal.x );
        binormals.push_back( point_binormal.y );
        binormals.push_back( point_binormal.z );

      }
      ++i;
    }

    tangent_node->value->setValue( tangents );
    binormal_node->value->setValue( binormals );
  }
}
