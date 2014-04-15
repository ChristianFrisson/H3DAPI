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
/// \file LineSet.cpp
/// \brief CPP file for LineSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/LineSet.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase LineSet::database( 
                                  "LineSet", 
                                  &(newInstance<LineSet>), 
                                  typeid( LineSet ),
                                  &X3DGeometryNode::database );

namespace LineSetInternals {
  FIELDDB_ELEMENT( LineSet, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LineSet, coord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LineSet, vertexCount, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LineSet, fogCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LineSet, attrib, INPUT_OUTPUT );

}


LineSet::LineSet( Inst< SFNode           > _metadata,
                  Inst< SFBound          > _bound,
                  Inst< DisplayList      > _displayList,
                  Inst< SFColorNode      > _color,
                  Inst< SFCoordinateNode > _coord,
                  Inst< MFInt32          > _vertexCount, 
                  Inst< SFFogCoordinate  > _fogCoord,
                  Inst< MFVertexAttributeNode > _attrib ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  color          ( _color       ),
  coord          ( _coord       ),
  vertexCount    ( _vertexCount ),
  fogCoord       ( _fogCoord    ),
  attrib( _attrib ) {

  type_name = "LineSet";
  database.initFields( this );

  color->route( displayList );
  coord->route( displayList );
  vertexCount->route( displayList );
  fogCoord->route( displayList );
  attrib->route( displayList );

  coord->route( bound );
}

void LineSet::DisplayList::callList( bool build_list ) {
  LineSet *line_set = 
   static_cast< LineSet * >( owner );
  glPushAttrib( GL_CURRENT_BIT );

  // If color field is NULL, we use the emissive Color from the current material
  // as color.
  if( !line_set->color->getValue() ) {
    float v[4];
    glGetMaterialfv( GL_FRONT, GL_EMISSION, v );
    glColor3f( v[0], v[1], v[2] );
  }

  X3DGeometryNode::DisplayList::callList( build_list );
  glPopAttrib();
}

void LineSet::render() {
  X3DColorNode *color_node = color->getValue();
  X3DCoordinateNode *coordinate_node = coord->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();
  const vector< int > &vertex_count     = vertexCount->getValue();

  if( coordinate_node ) {
    // Save the old state of GL_LIGHTING 
    GLboolean lighting_enabled;
    glGetBooleanv( GL_LIGHTING, &lighting_enabled );
    glDisable( GL_LIGHTING );

    // disable texturing
    X3DTextureNode *texture = X3DTextureNode::getActiveTexture();
    if( texture ) texture->disableTexturing();

    // set fog to get fog depth from fog coordinates if available
    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPushAttrib( GL_FOG_BIT );
      glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
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

    if( prefer_vertex_buffer_object ) {
      if( fog_coord_node )
        fog_coord_node->renderVertexBufferObject();
      if( color_node )
        color_node->renderVertexBufferObject();
      if( coordinate_node )
        coordinate_node->renderVertexBufferObject();

      for( unsigned int attrib_index = 0;
            attrib_index < attrib->size(); ++attrib_index ) {
        X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->renderVertexBufferObject();
      }

      // index of the current vertex whose index is added. It will be
      // incremented for each vertex whose index is added.
      unsigned int vertex_counter = 0;
      // render all polylines. Each loop will render one polyline.
      for( vector< int >::const_iterator  i = vertex_count.begin();
           i != vertex_count.end(); ++i ) {
        // Check that the vertex count value is > 2
        if( (*i) < 2 ) {
          stringstream s;
          s << "Must be >= 2 (in \"" 
            << getName() << "\" node) ";
          throw InvalidVertexCountValue( *i, s.str(), H3D_FULL_LOCATION );
        }

        glDrawArrays( GL_LINE_STRIP, vertex_counter, (*i) );
        vertex_counter = vertex_counter + (*i);
      }

      if( fog_coord_node )
        fog_coord_node->disableVertexBufferObject();
      if( color_node )
        color_node->disableVertexBufferObject();
      if( coordinate_node )
        coordinate_node->disableVertexBufferObject();

      for( unsigned int attrib_index = 0;
        attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
            attrib->getValueByIndex( attrib_index );
          if( attr ) attr->disableVertexBufferObject();
      }
    } else {

      // index of the current vertex being rendered. It will be incremented
      // for each vertex that is rendered.
      unsigned int vertex_counter = 0;

      // render all polylines. Each loop will render one polyline.
      for( vector< int >::const_iterator  i = vertex_count.begin();
           i != vertex_count.end(); 
           ++i ) {
        // start the polyline rendering.
        glBegin( GL_LINE_STRIP );

        // Check that the vertex count value is > 2
        if( (*i) < 2 ) {
          stringstream s;
          s << "Must be >= 2 (in \"" 
            << getName() << "\" node) ";
          throw InvalidVertexCountValue( *i, s.str(), H3D_FULL_LOCATION );
        }

        unsigned int stop_index = vertex_counter + (*i);

        // render all vertices for this polyline.
        for( ; vertex_counter < stop_index; ++vertex_counter ) {
          // Set up colors if colors are specified per vertex.
          if( color_node ) {
            color_node->render( vertex_counter );
          }
          // Render vertex attribute
          for( unsigned int attrib_index = 0;
               attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
                attrib->getValueByIndex( attrib_index );
              if( attr ) {
                attr->render( vertex_counter );
              }
          }
            
          // Render the vertices.
          coordinate_node->render( vertex_counter );
          if( fog_coord_node){
            fog_coord_node->render(vertex_counter);
          }
        }
        // end GL_POLY_LINE
        glEnd();
      }
    }
  
    // restore previous fog attributes
    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPopAttrib();
    }  

    // reenable lighting if it was enabled before
    if( lighting_enabled )
      glEnable( GL_LIGHTING );

    if( texture ) texture->enableTexturing();
  }
}

