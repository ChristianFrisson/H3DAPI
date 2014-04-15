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
/// \file TriangleSet.cpp
/// \brief CPP file for TriangleSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TriangleSet.h>
#include <H3D/Normal.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TriangleSet::database( 
                                      "TriangleSet", 
                                      &(newInstance<TriangleSet>), 
                                      typeid( TriangleSet ),
                                      &X3DComposedGeometryNode::database );

TriangleSet::TriangleSet( 
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
                         Inst< SFFogCoordinate         > _fogCoord ):
  X3DComposedGeometryNode( _metadata, _bound, _displayList,
                           _color, _coord, _normal, _texCoord, 
                           _ccw, _colorPerVertex, _normalPerVertex,
                           _solid, _attrib, _fogCoord ),
  autoNormal( _autoNormal ) {

  type_name = "TriangleSet";
  database.initFields( this );

  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );

  normalPerVertex->route( autoNormal );
  coord->route( autoNormal );
  ccw->route( autoNormal );

  coord->route( bound );
}

void TriangleSet::render() {
  X3DCoordinateNode *coordinate_node = coord->getValue();
  X3DTextureCoordinateNode *tex_coord_node = texCoord->getValue();
  X3DColorNode *color_node = color->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();
  X3DNormalNode *normal_node = normal->getValue();

  bool tex_coord_gen = 
    !tex_coord_node || (tex_coord_node && tex_coord_node->supportsTexGen());
  bool tex_coords_per_vertex = 
    tex_coord_node && tex_coord_node->supportsExplicitTexCoords();

  if( !normal_node ) {
    normal_node = autoNormal->getValue();
  }

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

    unsigned int nr_triangles = coordinate_node->nrAvailableCoords() / 3;

    if( prefer_vertex_buffer_object ) {
      // use vertex buffer objects to render the geometry
      coordinate_node->renderVertexBufferObject();
      normal_node->renderVertexBufferObject();
      if( fog_coord_node) fog_coord_node->renderVertexBufferObject();
      if( color_node ) color_node->renderVertexBufferObject();
      if( tex_coords_per_vertex )
        renderTexCoordVertexBufferObject( tex_coord_node );
      // Set up shader vertex attributes.
      for( unsigned int attrib_index = 0;
           attrib_index < attrib->size(); ++attrib_index ) {
        X3DVertexAttributeNode *attr = 
          attrib->getValueByIndex( attrib_index );
        if( attr ) attr->renderVertexBufferObject();
      }
      glDrawArrays( GL_TRIANGLES, 
                    0,
                    3*nr_triangles );
      
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
    } else {
      // use arrays to render the geometry
      coordinate_node->renderArray();
      normal_node->renderArray();
      if( fog_coord_node) fog_coord_node->renderArray();
      if( color_node ) color_node->renderArray();
      if( tex_coords_per_vertex ) renderTexCoordArray( tex_coord_node );
      // Set up shader vertex attributes.
      for( unsigned int attrib_index = 0;
           attrib_index < attrib->size(); ++attrib_index ) {
        X3DVertexAttributeNode *attr = 
          attrib->getValueByIndex( attrib_index );
        if( attr ) attr->renderArray();
      }
      glDrawArrays( GL_TRIANGLES, 
                    0,
                    3*nr_triangles );
      
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

void TriangleSet::traverseSG( TraverseInfo &ti ) {
  X3DComposedGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}

void TriangleSet::AutoNormal::update() {
  X3DCoordinateNode *coord = 
    static_cast< X3DCoordinateNode * >( static_cast< SFCoordinateNode * >
                                        ( routes_in[1] )->getValue() );
  bool ccw = static_cast< SFBool * >( routes_in[2] )->getValue();

  value = generateNormalsPerVertex( coord, ccw );
 }

X3DNormalNode *TriangleSet::AutoNormal::generateNormalsPerVertex( 
                                   X3DCoordinateNode *coord,
                                   bool ccw ) {
  Normal *normal = new Normal;
  if( coord ) {
    unsigned int nr_coords = coord->nrAvailableCoords();
    vector< Vec3f > normals( nr_coords, 
                             Vec3f( 0, 0, 0 ) );
    for( unsigned int j = 0; j + 2 < nr_coords; j+=3 ) {
      Vec3f norm, A, B, C, AB, BC;
      // make sure we have a valid face. If not use a dummy normal. 
      if( j+2 >= nr_coords ) {
        norm =  Vec3f( 1, 0, 0 );
      } else {  
        // calculate a normal 
        A = coord->getCoord( j );
        B = coord->getCoord( j+1 );
        C = coord->getCoord( j+2 );
        
        AB = B - A;
        BC = C - B;
        
        norm = AB % BC;
      }
      norm.normalizeSafe();
      if( !ccw ) 
        norm = -norm;
      normals[j] += norm;
      normals[j+1] += norm;
      normals[j+2] += norm;
    }
    
    for( vector<Vec3f>::iterator i = normals.begin(); 
         i != normals.end(); 
         ++i ) 
      (*i).normalizeSafe();
    normal->vector->setValue( normals );
  }
  return normal;
}

