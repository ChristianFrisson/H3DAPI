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
/// \file X3DFontStyleNode.cpp
/// \brief CPP file for X3DFontStyleNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DFontStyleNode.h"

using namespace H3D;

X3DFontStyleNode::X3DFontStyleNode( 
                                   Inst< SFNode>  _metadata ) :
  X3DNode( _metadata ),
  fonts_built( false ) {
  
  type_name = "X3DFontStyleNode";
}

Vec3f X3DFontStyleNode::stringDimensions( const string &text,
                                          Alignment alignment ) {
  Vec3f dimensions = Vec3f( 0, 0, 0 ); 
  
  if( alignment == HORIZONTAL ) {
    for( size_t i = 0; i < text.length(); i++ ) {
      Vec3f c_dimensions = charDimensions( text[i] ); 
      dimensions.x += c_dimensions.x;
      if( dimensions.y < c_dimensions.y )
        dimensions.y = c_dimensions.y;
      if( dimensions.z < c_dimensions.z )
        dimensions.z = c_dimensions.z;
    }
  } else {
    for( size_t i = 0; i < text.length(); i++ ) {
      Vec3f c_dimensions = charDimensions( text[i] ); 
      if( dimensions.x < c_dimensions.x )
        dimensions.x = c_dimensions.x;
      dimensions.y += c_dimensions.y;
      if( dimensions.z < c_dimensions.z )
        dimensions.z = c_dimensions.z;
    }
  }
  return dimensions;
}

Vec3f X3DFontStyleNode::stringDimensions( const vector< string > &text,
                                          Alignment alignment,
                                          const vector< H3DFloat > &length ) {
  Vec3f dims( 0, 0, 0 );
  
  H3DFloat spacing = getSpacing();

  vector< H3DFloat >::const_iterator l = length.begin();

  for( vector< string >::const_iterator i = text.begin();
       i != text.end();
       i++ ) {
    Vec3f line_dims = stringDimensions( *i, alignment );
    if( alignment == X3DFontStyleNode::HORIZONTAL ) {
      if( l != length.end() ) {
        line_dims.x = *l;
        l++;
      }
      if( i != text.begin() ) {
        if( dims.x < line_dims.x ) dims.x = line_dims.x;
        dims.y += line_dims.y * spacing;
      } else {
        dims = line_dims;
      } 
    } else if( alignment == X3DFontStyleNode::VERTICAL ) {
      if( l != length.end() ) {
        line_dims.y = *l;
        l++;
      }
      if( i != text.begin() ) {
        dims.x += line_dims.x * spacing;
        if( dims.y < line_dims.y ) dims.y = line_dims.y;
      } else {
        dims = line_dims;
      }
    }
  } 
  return dims;
}
