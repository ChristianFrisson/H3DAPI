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
/// \file IndexedLineSet.cpp
/// \brief CPP file for IndexedLineSet, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "IndexedLineSet.h"
#ifdef USE_HAPTICS
#include "HLFeedbackShape.h"
#endif
#include "X3DTextureNode.h"

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
                                Inst< MFInt32 >  _coordIndex ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  set_colorIndex ( _set_colorIndex ),
  set_coordIndex ( _set_coordIndex ),
  color          ( _color          ),
  coord          ( _coord          ),
  colorIndex     ( _colorIndex     ),
  colorPerVertex ( _colorPerVertex ),
  coordIndex     ( _coordIndex     ) {

  type_name = "IndexedLineSet";
  database.initFields( this );

  colorPerVertex->setValue( true );

  color->route( displayList );
  coord->route( displayList );
  colorIndex->route( displayList );
  colorPerVertex->route( displayList );
  coordIndex->route( displayList );

  set_colorIndex->route( colorIndex, id );
  set_coordIndex->route( coordIndex, id );

  coord->route( bound );
}

void IndexedLineSet::render() {
  X3DColorNode *color_node = color->getValue();
  X3DCoordinateNode *coordinate_node = coord->getValue();

  const vector< int > &color_index     = colorIndex->getValue();
  const vector< int > &coord_index     = coordIndex->getValue();

  if( coordinate_node ) {

    // Check that we have enough color indices if colorPerVertex.
    if( color_node && colorPerVertex->getValue() &&
        color_index.size() > 0 && color_index.size() < coord_index.size() ) {
      stringstream s;
      s << "Must contain at least as many elements as coordIndex (" 
        << coord_index.size() << ") in \"" 
        << getName() << "\" node. ";
      throw InvalidColorIndexSize( color_index.size(), s.str(), H3D_FULL_LOCATION );

    }

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

    // index of the current polyline being rendered. It will be incremented
    // for each polyline that is rendered.
    unsigned int line_count = 0;

    // render all polylines. Each loop will render one polyline.
    for( unsigned int i = 0; i < coord_index.size();
         i++ ) {
      // start the polyline rendering.
      glBegin( GL_LINE_STRIP );
    
      // set up colors if the colors are specified per polyline
      if( color_node && !colorPerVertex->getValue() ) {
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
            throw InvalidColorIndexSize( color_index.size(), s.str(), H3D_FULL_LOCATION );
          }
        }
        color_node->render( ci );
      }
    
      // render all vertices for this polyline.
      for(; i < coord_index.size() && coord_index[i] != -1;  i ++ ) {
        // Set up colors if colors are specified per vertex.
        if( color_node && colorPerVertex->getValue() ) {
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
      
        // Render the vertices.
        coordinate_node->render( coord_index[ i ] );
      }
      // end GL_POLY_LINE
      glEnd();

      line_count++;
    }

    // reenable lighting if it was enabled before
    if( lighting_enabled )
      glEnable( GL_LIGHTING );

    if( texture ) texture->enableTexturing();
  }
}

#ifdef USE_HAPTICS
void IndexedLineSet::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
    ti.addHapticShapeToAll( new HLFeedbackShape( this,
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix(),
                                                 coordIndex->size() ) );
  }
}
#endif
