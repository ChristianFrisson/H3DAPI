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
/// \file IndexedLineSet.cpp
/// \brief CPP file for IndexedLineSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/IndexedLineSet.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase IndexedLineSet::database( "IndexedLineSet", 
                                          &(newInstance<IndexedLineSet>), 
                                          typeid( IndexedLineSet ),
                                          &X3DGeometryNode::database );

namespace IndexedLineSetInternals {
  FIELDDB_ELEMENT( IndexedLineSet, set_colorIndex, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedLineSet, set_coordIndex, INPUT_ONLY );
  FIELDDB_ELEMENT( IndexedLineSet, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedLineSet, coord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedLineSet, colorIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedLineSet, colorPerVertex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedLineSet, coordIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedLineSet, fogCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( IndexedLineSet, attrib, INPUT_OUTPUT );
}

IndexedLineSet::IndexedLineSet( Inst< SFNode           > _metadata,
                                Inst< CoordBoundField  > _bound,
                                Inst< DisplayList      > _displayList, 
                                Inst< MFInt32 >  _set_colorIndex,
                                Inst< MFInt32 >  _set_coordIndex,
                                Inst< SFColorNode  >  _color,
                                Inst< SFCoordinateNode  >  _coord,
                                Inst< MFInt32 >  _colorIndex,
                                Inst< SFBool  >  _colorPerVertex,
                                Inst< MFInt32 >  _coordIndex, 
                                Inst< SFFogCoordinate > _fogCoord,
                                Inst< MFVertexAttributeNode > _attrib ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  set_colorIndex ( _set_colorIndex ),
  set_coordIndex ( _set_coordIndex ),
  color          ( _color          ),
  coord          ( _coord          ),
  colorIndex     ( _colorIndex     ),
  colorPerVertex ( _colorPerVertex ),
  coordIndex     ( _coordIndex     ),
  fogCoord       ( _fogCoord       ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ),
  attrib( _attrib ) {

  type_name = "IndexedLineSet";
  database.initFields( this );

  colorPerVertex->setValue( true );

  color->route( displayList );
  coord->route( displayList );
  colorIndex->route( displayList );
  colorPerVertex->route( displayList );
  coordIndex->route( displayList );
  fogCoord->route( displayList );
  attrib->route( displayList );

  set_colorIndex->route( colorIndex, id );
  set_coordIndex->route( coordIndex, id );

  coord->route( bound );
  coordIndex->route( vboFieldsUpToDate );
}

IndexedLineSet::~IndexedLineSet() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

void IndexedLineSet::DisplayList::callList( bool build_list ) {
  IndexedLineSet *line_set = 
   static_cast< IndexedLineSet * >( owner );

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

void IndexedLineSet::render() {
  X3DColorNode *color_node = color->getValue();
  X3DCoordinateNode *coordinate_node = coord->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();

  const vector< int > &color_index     = colorIndex->getValue();
  const vector< int > &coord_index     = coordIndex->getValue();

  if( coordinate_node ) {

    bool color_per_vertex = colorPerVertex->getValue();
    // Check that we have enough color indices if colorPerVertex.
    if( color_node && color_per_vertex &&
        color_index.size() > 0 && color_index.size() < coord_index.size() ) {
      stringstream s;
      s << "Must contain at least as many elements as coordIndex (" 
        << coord_index.size() << ") in \"" 
        << getName() << "\" node. ";
      throw InvalidColorIndexSize( (int) color_index.size(), s.str(), H3D_FULL_LOCATION );

    }

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

    if( prefer_vertex_buffer_object && ( !color_node ||
      ( color_per_vertex && color_index.empty() ) ) ) {
      // Use vertex buffer objects to create IndexedLineSet.
      // Note that we need to create the index field properly.
      if( !vboFieldsUpToDate->isUpToDate() ) {
        vboFieldsUpToDate->upToDate();
        vector< GLuint > tmp_coord_index;
        tmp_coord_index.reserve( coord_index.size() );
        nr_index.clear();

        for( unsigned int i = 0; i < coord_index.size(); ++i ) {
          // render all vertices for this polyline.
          GLsizei tmp_i = i;
          GLsizei lowest;
          GLsizei highest;
          for(; i < coord_index.size() && coord_index[i] != -1; ++i ) {
            H3DInt32 index = coord_index[i];
            if( i == tmp_i ) {
              lowest = index;
              highest = index;
            } else {
              if( index < lowest )
                lowest = index;
              else if( index > highest )
                highest = index;
            }

            tmp_coord_index.push_back( index );
          }
          if( i - tmp_i > 0 ) {
            pair< GLsizei, pair< GLsizei, GLsizei > > tmp_info;
            tmp_info.first = i - tmp_i;
            tmp_info.second.first = lowest;
            tmp_info.second.second = highest;
            nr_index.push_back( tmp_info );
          }
        }

        if( !vbo_id ) {
          vbo_id = new GLuint;
          glGenBuffersARB( 1, vbo_id );
        }
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *vbo_id );
        glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                         tmp_coord_index.size() * sizeof(GLuint),
                         &(*(tmp_coord_index.begin())), GL_STATIC_DRAW_ARB );
      } else {
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, *vbo_id );
      }

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

      GLsizei offset = 0;
      for( unsigned int i = 0; i < nr_index.size(); ++i ) {
        const pair< GLsizei, pair< GLsizei, GLsizei > > &item = nr_index[i];
        // Draw the triangles
        glDrawRangeElements( GL_LINE_STRIP,
                             item.second.first,
                             item.second.second,
                             item.first,
                             GL_UNSIGNED_INT,
                             (GLvoid*)( offset * sizeof( GLuint ) ) );
        offset += item.first;
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

      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
    } else {
      // index of the current polyline being rendered. It will be incremented
      // for each polyline that is rendered.
      unsigned int line_count = 0;

      // render all polylines. Each loop will render one polyline.
      for( unsigned int i = 0; i < coord_index.size();
           ++i ) {
        // start the polyline rendering.
        glBegin( GL_LINE_STRIP );
      
        // set up colors if the colors are specified per polyline
        if( color_node && !color_per_vertex ) {
          int ci;
          if ( color_index.size() == 0 ) {
            ci = line_count;
          } else {
            if( line_count < color_index.size() )
              ci = color_index[ line_count ];
            else {
              stringstream s;
              s << "Must contain at least as many elements as polylines in \"" 
                << getName() << "\" node. ";
              throw InvalidColorIndexSize( (int) color_index.size(), s.str(), H3D_FULL_LOCATION );
            }
          }
          color_node->render( ci );
        }

        // render all vertices for this polyline.
        for(; i < coord_index.size() && coord_index[i] != -1; ++i ) {
          // Set up colors if colors are specified per vertex.
          if( color_node && color_per_vertex ) {
            int ci;
            if ( color_index.size() == 0 ) {
              ci = coord_index[ i ];
            } else {
              ci = color_index[ i ];
            }
          
            if( ci == -1 ) {
              stringstream s;
              s << "-1 mismatch between coord_index and color_index in \"" 
                << getName() << "\" node. Must be of equal length and have -1 in "
                << "the same places. ";
              throw InvalidColorIndex( ci, s.str(), H3D_FULL_LOCATION );
            } else {
              color_node->render( ci );
            }
          }
          
          // Render vertex attribute
          for( unsigned int attrib_index = 0;
               attrib_index < attrib->size(); ++attrib_index ) {
            X3DVertexAttributeNode *attr = 
                attrib->getValueByIndex( attrib_index );
              if( attr ) {
                attr->render( coord_index[ i ] );
              }
          }
          // Render the vertices.
          coordinate_node->render( coord_index[ i ] );
          if( fog_coord_node ){
            fog_coord_node->render(coord_index[ i ]);
          }

        }
        // end GL_LINE_STRIP
        glEnd();

        ++line_count;
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

