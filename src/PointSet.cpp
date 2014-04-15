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
/// \file PointSet.cpp
/// \brief CPP file for PointSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PointSet.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PointSet::database( 
                                   "PointSet", 
                                   &(newInstance<PointSet>), 
                                   typeid( PointSet ),
                                   &X3DGeometryNode::database );

namespace PointSetInternals {
  FIELDDB_ELEMENT( PointSet, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointSet, coord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointSet, fogCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointSet, attrib, INPUT_OUTPUT );
}

PointSet::PointSet(  Inst< SFNode                > _metadata,
                     Inst< SFBound               > _bound,
                     Inst< DisplayList           > _displayList,
                     Inst< SFColorNode           > _color,
                     Inst< SFCoordinateNode      > _coord,
                     Inst< SFFogCoordinate       > _fogCoord,
                     Inst< MFVertexAttributeNode > _attrib ):
  X3DGeometryNode( _metadata, _bound, _displayList ),
  color   ( _color    ),
  coord   ( _coord    ),
  fogCoord( _fogCoord ),
  attrib ( _attrib ) {

  type_name = "PointSet";
  database.initFields( this );

  color->route( displayList );
  coord->route( displayList );
  fogCoord->route( displayList );
  attrib->route( displayList );

  coord->route( bound );

}

void PointSet::DisplayList::callList( bool build_list ) {
  PointSet *point_set = 
   static_cast< PointSet * >( owner );

  glPushAttrib( GL_CURRENT_BIT );

  // If color field is NULL, we use the emissive Color from the current material
  // as color.
  if( !point_set->color->getValue() ) {
    float v[4];
    glGetMaterialfv( GL_FRONT, GL_EMISSION, v );
    glColor3f( v[0], v[1], v[2] );
  }

  X3DGeometryNode::DisplayList::callList( build_list );

  glPopAttrib();
}   



void PointSet::render() {
  X3DColorNode *color_node = color->getValue();
  X3DCoordinateNode *coordinate_node = coord->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();

  if( coordinate_node ) {
    // Save the old state of GL_LIGHTING 
    GLboolean lighting_enabled;
    glGetBooleanv( GL_LIGHTING, &lighting_enabled );
    glDisable( GL_LIGHTING );
    
    // disable texturing
    X3DTextureNode *texture = X3DTextureNode::getActiveTexture();
    if( texture ) texture->disableTexturing();

    if( color_node ) {
      // make sure we have enough colors
      if( coordinate_node->nrAvailableCoords() > color_node->nrAvailableColors() ) {
        stringstream s;
        s << "Must be at least as many as coordinates in the coord field ("
          << coordinate_node->nrAvailableCoords() << ") in "
          << getName() << "\" node) ";
        throw NotEnoughColors( color_node->nrAvailableColors(), s.str(), 
                               H3D_FULL_LOCATION );
      }
    }
    
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

      glDrawArrays( GL_POINTS, 0, coordinate_node->nrAvailableCoords() );

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
      // render the points
      glBegin( GL_POINTS );
      for( unsigned int i = 0; i < coordinate_node->nrAvailableCoords(); ++i ) {
        // Set up colors if colors are specified per vertex.
        if( color_node ) {
          color_node->render( i );
        }

        // Render vertex attribute
        for( unsigned int attrib_index = 0;
             attrib_index < attrib->size(); ++attrib_index ) {
          X3DVertexAttributeNode *attr = 
              attrib->getValueByIndex( attrib_index );
            if( attr ) {
              attr->render( i );
            }
        }

        // Render the vertices.
        coordinate_node->render( i );
         if( fog_coord_node) fog_coord_node->render(i);
      }
      // end GL_POLY_LINE
      glEnd();
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

