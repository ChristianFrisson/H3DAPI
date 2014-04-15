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
/// \file TriangleFanSet.cpp
/// \brief CPP file for TriangleFanSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TriangleFanSet.h>
#include <H3D/Normal.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TriangleFanSet::database( 
                                         "TriangleFanSet", 
                                         &(newInstance<TriangleFanSet>), 
                                         typeid( TriangleFanSet ),
                                         &X3DComposedGeometryNode::database );

namespace TriangleFanSetInternals {
  FIELDDB_ELEMENT( TriangleFanSet, fanCount, INPUT_OUTPUT );
}

TriangleFanSet::TriangleFanSet( 
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
                              Inst< MFInt32                 > _fanCount,
                              Inst< SFFogCoordinate         > _fogCoord ):
  X3DComposedGeometryNode( _metadata, _bound, _displayList,
                           _color, _coord, _normal, _texCoord, 
                           _ccw, _colorPerVertex, _normalPerVertex, 
                           _solid, _attrib, _fogCoord ),
  autoNormal( _autoNormal ),
  fanCount( _fanCount ) {

  type_name = "TriangleFanSet";
  database.initFields( this );
  
  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );

  fanCount->route( displayList );

  normalPerVertex->route( autoNormal );
  coord->route( autoNormal );
  fanCount->route( autoNormal );
  ccw->route( autoNormal );

  coord->route( bound );
}

void TriangleFanSet::render() {
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

  const vector< int > &fan_count  = fanCount->getValue();
 
  if( coordinate_node ) {
    
    // no X3DTextureCoordinateNode, so we generate texture coordinates
    // based on the bounding box according to the X3D specification.
    if( tex_coord_gen ) startTexGen( tex_coord_node );
    
    if( tex_coords_per_vertex &&
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
      // if normal per vertex we can use arrays to render the geometry
      // they all use the same indices.
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

      // the index in indices for the start of the current triangle fan.
      unsigned int start_pos = 0;
 
      // draw each triangle fan from the arrays 
      for( vector<int>::const_iterator sc = fan_count.begin();
           sc != fan_count.end();
           ++sc ) {

        // check that fan count value >=3
        if( (*sc) < 3 ) {
          stringstream s;
          s << "Must be >=3 in \"" 
            << getName() << "\" node. ";
          throw InvalidFanCount( *sc, s.str(), H3D_FULL_LOCATION );
        }

        // render the triangle fan
        glDrawArrays( GL_TRIANGLE_FAN, 
                      start_pos, 
                      *sc );
        start_pos += *sc;
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
      
    } else {
      // the number of triangles rendered so far in the entire 
      // TriangleFanSet
      unsigned int triangle_counter = 0;
      
      // the current vertex index
      unsigned int vertex_counter = 0;

      // the index of the root vertex of the current fan
      unsigned int fan_root =0;

      // we cannot use arrays to render when normal_per_vertex is false
      // since the indexing into the normals are different from the rest.

      // render as triangles, we cannot use GL_TRIANGLE_FAN since the normals
      // have to be per face. Also since the indices specifies a triangle fan
      // we have to alternate the direction of vertices every second triangle
      // in order to get the front face correct.

      for( vector<int>::const_iterator sc = fan_count.begin();
           sc != fan_count.end();
           ++sc ) {

        // check that fan count value >=3
        if( (*sc) < 3 ) {
          stringstream s;
          s << "Must be >=3 in \"" 
            << getName() << "\" node. ";
          throw InvalidFanCount( *sc, s.str(), H3D_FULL_LOCATION );
        }
        
        glBegin( GL_TRIANGLES );

        for( int i = 0; i < (*sc) - 2; ++i ) {
          normal_node->render( triangle_counter );

          // vertex 1
          if( color_node ) color_node->render( fan_root );
          if( tex_coords_per_vertex ) renderTexCoord( fan_root, 
                                                      tex_coord_node );
          if( fog_coord_node) fog_coord_node->render(fan_root);
          for( unsigned int attrib_index = 0;
               attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->render( fan_root );
          }
          coordinate_node->render( fan_root );

          // vertex 2
          if( color_node ) color_node->render( vertex_counter+1 );
          if( tex_coords_per_vertex ) renderTexCoord( vertex_counter+1, 
                                                      tex_coord_node );
          if( fog_coord_node) fog_coord_node->render(vertex_counter+1);
          for( unsigned int attrib_index = 0;
               attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->render( vertex_counter+1 );
          }
          coordinate_node->render( vertex_counter+1 );

          // vertex 3
          if( color_node ) color_node->render( vertex_counter+2 );
          if( tex_coords_per_vertex ) renderTexCoord( vertex_counter+2,
                                                      tex_coord_node );
          if( fog_coord_node) fog_coord_node->render(vertex_counter+2);
          for( unsigned int attrib_index = 0;
               attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) attr->render( vertex_counter+2 );
          }
          coordinate_node->render( vertex_counter+2 );
          ++triangle_counter;
          ++vertex_counter;
        }
        glEnd();
        
        if( (*sc) >=3 )
          // skip to the next triangle fan
          vertex_counter += 2;

        fan_root += *sc;
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

void TriangleFanSet::traverseSG( TraverseInfo &ti ) {
  X3DComposedGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}


void TriangleFanSet::AutoNormal::update() {
  bool normals_per_vertex = 
    static_cast< SFBool * >( routes_in[0] )->getValue();
  X3DCoordinateNode *coord = 
    static_cast< X3DCoordinateNode * >( static_cast< SFCoordinateNode * >
                                        ( routes_in[1] )->getValue() );
  const vector<int> &fan_count = 
    static_cast< MFInt32 * >( routes_in[2] )->getValue();
  bool ccw = static_cast< SFBool * >( routes_in[3] )->getValue();

  if( normals_per_vertex ) 
    value = generateNormalsPerVertex( coord, fan_count, ccw );
  else
    value = generateNormalsPerFace( coord, fan_count, ccw );
}

X3DNormalNode *TriangleFanSet::AutoNormal::generateNormalsPerVertex( 
                                   X3DCoordinateNode *coord,
                                   const vector< int > &fan_count,
                                   bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals( coord->nrAvailableCoords(), 
                             Vec3f( 0, 0, 0 ) );
    // the current vertex index
    unsigned int vertex_count = 0;
    // the index of the root vertex of the fan
    unsigned int fan_root = 0;
    
    for( vector<int>::const_iterator sc = fan_count.begin();
         sc != fan_count.end();
         ++sc ) {

      // calculate normals for the triangle fan
      for( int j = 0; j < (*sc) - 2; ++j ) {
        Vec3f norm, A, B, C, AB, BC;
        // calculate a normal 
        A = coord->getCoord( fan_root );
        B = coord->getCoord( vertex_count+1 );
        C = coord->getCoord( vertex_count+2 );
        AB = B - A;
        BC = C - B;
        norm = AB % BC;
        norm.normalizeSafe();
        if( !ccw ) 
          norm = -norm;
        
        normals[ fan_root ] += norm;
        normals[ vertex_count + 1 ] += norm;
        normals[ vertex_count + 2 ] += norm;
        ++vertex_count;
      }
      
      if( (*sc) >=3 )
        // skip to the next triangle fan
        vertex_count += 2;

      fan_root += *sc;
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

X3DNormalNode *TriangleFanSet::AutoNormal::generateNormalsPerFace( 
                                             X3DCoordinateNode *coord,
                                             const vector< int > &fan_count,
                                             bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    vector< Vec3f > normals;
    // the current vertex index
    unsigned int vertex_count = 0;
    // the index of the root vertex of the current fan
    unsigned int fan_root = 0;
    
    for( vector<int>::const_iterator sc = fan_count.begin();
         sc != fan_count.end();
         ++sc ) {

      // calculate normals for the triangle fan
      for( int j = 0; j < (*sc) - 2; ++j ) {
      
        Vec3f norm, A, B, C, AB, BC;
        // calculate a normal for the triangle
        A = coord->getCoord( fan_root );
        B = coord->getCoord( vertex_count + 1 );
        C = coord->getCoord( vertex_count + 2 );
        
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
        
        normals.push_back( norm );
        ++vertex_count;
      }
      if( (*sc) >=3 )
        // skip to the next triangle fan
        vertex_count += 2;

      fan_root += *sc;
    }
    normal->vector->setValue( normals );
  }
  return normal;
}


