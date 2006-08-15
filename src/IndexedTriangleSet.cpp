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
/// \file IndexedTriangleSet.cpp
/// \brief CPP file for IndexedTriangleSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "IndexedTriangleSet.h"
#include "Normal.h"

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
                              Inst< MFInt32                 > _index ) :
  X3DComposedGeometryNode( _metadata, _bound, _displayList, 
                           _color, _coord, _normal, _texCoord, 
                           _ccw, _colorPerVertex, _normalPerVertex,
                           _solid, _attrib ),
  autoNormal( _autoNormal ),   
  set_index( _set_index ),
  index( _index ) {

  type_name = "IndexedTriangleSet";
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
}



void IndexedTriangleSet::render() {
  X3DCoordinateNode *coordinate_node = coord->getValue();
  X3DTextureCoordinateNode *tex_coord_node = texCoord->getValue();
  TextureCoordinateGenerator *tex_coord_gen = 
    dynamic_cast< TextureCoordinateGenerator * >( tex_coord_node );
  X3DColorNode *color_node = color->getValue();
  X3DNormalNode *normal_node = normal->getValue();

  if( !normal_node ) {
    normal_node = autoNormal->getValue();
  }
  bool tex_coords_per_vertex = tex_coord_node && !tex_coord_gen;
  const vector< int > &indices  = index->getValue();
 
  if( coordinate_node ) {
    // enable backface culling if solid is true
    if( solid->getValue() ) {
      glEnable( GL_CULL_FACE );
      glCullFace( GL_BACK );
    } else {
      glDisable( GL_CULL_FACE );
    }
    
    // no X3DTextureCoordinateNode, so we generate texture coordinates
    // based on the bounding box according to the X3D specification.
    if( !tex_coords_per_vertex ) {
      startTexGen( tex_coord_gen );
    } else if( coordinate_node->nrAvailableCoords() > 
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
      glEnable( GL_COLOR_MATERIAL );
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

    unsigned int nr_triangles = indices.size() / 3;

    if( normalPerVertex->getValue() ) {
      // if normal per vertex we can use arrays to render the geometry
      // they all use the same indices.
      coordinate_node->renderArray();
      normal_node->renderArray();
      if( color_node ) color_node->renderArray();
      if( tex_coords_per_vertex ) renderTexCoordArray( tex_coord_node );
      for( unsigned int attrib_index = 0;
           attrib_index < attrib->size(); attrib_index++ ) {
        X3DVertexAttributeNode *attr = 
          attrib->getValueByIndex( attrib_index );
        if( attr ) attr->renderArray();
      }
      
      glDrawElements( GL_TRIANGLES, 
                      3*nr_triangles, 
                      GL_UNSIGNED_INT,
                      &(*(indices.begin()) ) );
      
      coordinate_node->disableArray();
      normal_node->disableArray();
      if( color_node ) color_node->disableArray();
      if( tex_coords_per_vertex ) disableTexCoordArray( tex_coord_node );
       for( unsigned int attrib_index = 0;
           attrib_index < attrib->size(); attrib_index++ ) {
        X3DVertexAttributeNode *attr = 
          attrib->getValueByIndex( attrib_index );
        if( attr ) attr->disableArray();
      }      
    } else {
      // we cannot use arrays to render when normal_per_vertex is false
      // since the indexing into the normals are different from the rest.
      glBegin( GL_TRIANGLES );
      for( unsigned int i = 0; i < nr_triangles; i++ ) {
        normal_node->render( i );

        unsigned int v = i * 3;
        // vertex 1
        if( color_node ) color_node->render( indices[v] );
        if( tex_coords_per_vertex ) renderTexCoord( indices[v],
                                                    tex_coord_node );
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); attrib_index++ ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->render( indices[v] );
        }
        coordinate_node->render( indices[v] );

        // vertex 2
        if( color_node ) color_node->render( indices[v+1] );
        if( tex_coords_per_vertex ) renderTexCoord( indices[v+1],
                                                    tex_coord_node );
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); attrib_index++ ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->render( indices[v+1] );
        }
        coordinate_node->render( indices[v+1] );

        // vertex 3
        if( color_node ) color_node->render( indices[v+2] );
        if( tex_coords_per_vertex ) renderTexCoord( indices[v+2],
                                                    tex_coord_node );
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); attrib_index++ ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->render( indices[v+2] );
        }
        coordinate_node->render( indices[v+2] );
      }
      glEnd();
    }
    // disable texture coordinate generation.
    if( !tex_coords_per_vertex ) {
      stopTexGen( tex_coord_gen );
    }

    if ( color_node ) {
      glDisable( GL_COLOR_MATERIAL );
    } 
  } 
}

#ifdef USE_HAPTICS
void IndexedTriangleSet::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
#ifdef HAVE_OPENHAPTICS
    ti.addHapticShapeToAll( getOpenGLHapticShape(
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix() ) );
#endif
  }
}
#endif


void IndexedTriangleSet::AutoNormal::update() {
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
         i++ ) {
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
