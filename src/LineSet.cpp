//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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

#include <LineSet.h>
#include <X3DTextureNode.h>
#include <HLFeedbackShape.h>

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

}


LineSet::LineSet( Inst< SFNode           > _metadata,
                  Inst< SFBound          > _bound,
                  Inst< DisplayList      > _displayList,
                  Inst< SFColorNode      > _color,
                  Inst< SFCoordinateNode > _coord,
                  Inst< MFInt32          > _vertexCount, 
                  Inst< SFFogCoordinate  > _fogCoord  ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  color          ( _color       ),
  coord          ( _coord       ),
  vertexCount    ( _vertexCount ),
  fogCoord       ( _fogCoord    ){

  type_name = "LineSet";
  database.initFields( this );

  color->route( displayList );
  coord->route( displayList );
  vertexCount->route( displayList );
  fogCoord->route( displayList );

  coord->route( bound );
}

void LineSet::render() {
  X3DColorNode *color_node = color->getValue();
  X3DCoordinateNode *coordinate_node = coord->getValue();
  const vector< int > &vertex_count     = vertexCount->getValue();

  if( coordinate_node ) {
    // Save the old state of GL_LIGHTING 
    GLboolean lighting_enabled;
    glGetBooleanv( GL_LIGHTING, &lighting_enabled );
    glDisable( GL_LIGHTING );

    // disable texturing
    X3DTextureNode *texture = X3DTextureNode::getActiveTexture();
    if( texture ) texture->disableTexturing();

    // If color field is NULL, we use the emissive Color from the current material
    // as color.
    if( !color_node ) {
      float v[4];
      glGetMaterialfv( GL_FRONT, GL_EMISSION, v );
      glColor3f( v[0], v[1], v[2] );
    }

    // index of the current vertex being rendered. It will be incremented
    // for each vertex that is rendered.
    unsigned int vertex_counter = 0;

    // render all polylines. Each loop will render one polyline.
    for( vector< int >::const_iterator  i = vertex_count.begin();
         i != vertex_count.end(); 
         i++ ) {
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
      for( ; vertex_counter < stop_index; vertex_counter++ ) {
        // Set up colors if colors are specified per vertex.
        if( color_node ) {
          color_node->render( vertex_counter );
        }
          
        // Render the vertices.
        coordinate_node->render( vertex_counter );
        if( fogCoord->getValue()){
          fogCoord->getValue()->render(vertex_counter);
        }
      }
      // end GL_POLY_LINE
      glEnd();
    }
  
    // reenable lighting if it was enabled before
    if( lighting_enabled )
      glEnable( GL_LIGHTING );

    if( texture ) texture->enableTexturing();
  }
}

