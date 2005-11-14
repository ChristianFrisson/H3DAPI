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
/// \file MultiTextureCoordinate.cpp
/// \brief CPP file for MultiTextureCoordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "MultiTextureCoordinate.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MultiTextureCoordinate::database( 
                                       "MultiTextureCoordinate", 
                                       &(newInstance<MultiTextureCoordinate>), 
                                       typeid( MultiTextureCoordinate ),
                                       &X3DTextureCoordinateNode::database );

namespace MultiTextureCoordinateInternals {
  FIELDDB_ELEMENT( MultiTextureCoordinate, texCoord, INPUT_OUTPUT );
}

MultiTextureCoordinate::MultiTextureCoordinate( 
                 Inst< SFNode>  _metadata,
                 Inst< MFTextureCoordinateNode > _texCoord ) :
  X3DTextureCoordinateNode( _metadata ),
  texCoord( _texCoord ) {
  
  type_name = "MultiTextureCoordinate";
  database.initFields( this );
}

void MultiTextureCoordinate::renderForTextureUnits( int index,
                                                    unsigned int start_unit,
                                                    unsigned int end_unit ) {
  
  // the maximum index that can be used in texCoord for rendering
  unsigned int max_index = texCoord->size() - 1;

  if( end_unit > max_index ) {
    unsigned int start_index =  start_unit + end_unit - max_index;
    X3DTextureCoordinateNode *tt = 
      static_cast< X3DTextureCoordinateNode * >( texCoord->back() );
    if( tt ) tt->renderForTextureUnits( index,
                                        start_index,
                                        end_unit );
    end_unit = start_index - 1; 
  }
  
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    X3DTextureCoordinateNode *tt = texCoord->getValueByIndex( i );
    if( tt ) tt->renderForTextureUnit( index, i );
  }	  
}

void MultiTextureCoordinate::renderForTextureUnit( int index,
                                                   unsigned int texture_unit ) {
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( texture_unit );
  if( tc ) tc->renderForTextureUnit( index, texture_unit );
}

void MultiTextureCoordinate::renderArrayForTextureUnit( unsigned int texture_unit ){
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( texture_unit );
  if( tc ) tc->renderArrayForTextureUnit( texture_unit );
}

void MultiTextureCoordinate::renderArrayForTextureUnits( unsigned int start_unit,
                                                         unsigned int end_unit ) {
  
  // the maximum index that can be used in texCoord for rendering
  unsigned int max_index = texCoord->size() - 1;

  if( end_unit > max_index ) {
    unsigned int start_index =  start_unit + end_unit - max_index;
    X3DTextureCoordinateNode *tc = 
      static_cast< X3DTextureCoordinateNode * >( texCoord->back() );
    if( tc ) tc->renderArrayForTextureUnits( start_unit,
                                             end_unit );
    end_unit = start_index - 1; 
  }
  
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    X3DTextureCoordinateNode *tc = texCoord->getValueByIndex( i );
    if( tc ) tc->renderArrayForTextureUnit( i );
  }	  
}

void MultiTextureCoordinate::disableArrayForTextureUnit( unsigned int texture_unit ){
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( texture_unit );
  if( tc ) tc->disableArrayForTextureUnit( texture_unit );
}

void MultiTextureCoordinate::disableArrayForTextureUnits( unsigned int start_unit,
                                                          unsigned int end_unit ) {
  
  // the maximum index that can be used in texCoord for disableing
  unsigned int max_index = texCoord->size() - 1;

  if( end_unit > max_index ) {
    unsigned int start_index =  start_unit + end_unit - max_index;
    X3DTextureCoordinateNode *tt = 
      static_cast< X3DTextureCoordinateNode * >( texCoord->back() );
    if( tt ) tt->disableArrayForTextureUnits( start_unit,
                                              end_unit );
    end_unit = start_index - 1; 
  }
  
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    X3DTextureCoordinateNode *tt = texCoord->getValueByIndex( i );
    if( tt ) tt->disableArrayForTextureUnit( i );
  }	  
}

void MultiTextureCoordinate::render( int index ) {
  stringstream s;
  s << "Cannot use render() function in MultiTextureCoordinate. Use "
    << "renderForTextureUnit() instead to specify which texture "
    << "unit to render for. " << ends;
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

unsigned int MultiTextureCoordinate::nrAvailableTexCoords() {
  MFTextureCoordinateNode::const_iterator i = texCoord->begin();
  if( i == texCoord->end() ) return 0;
  unsigned int available_coords = 
    static_cast< X3DTextureCoordinateNode * >(*i)->nrAvailableTexCoords();
  i++;
  for( ;
       i != texCoord->end();
       i++ ) {
    available_coords = 
      H3DMin( available_coords, static_cast< X3DTextureCoordinateNode * >(*i)->nrAvailableTexCoords() );
  }
  return available_coords;
}

void MultiTextureCoordinate::renderArray() {
  stringstream s;
  s << "Cannot use renderArray() function in MultiTextureCoordinate. Use "
    << "renderArrayForTextureUnit() instead to specify which texture "
    << "unit to render for. " << ends;
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

