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
/// \file MultiTextureCoordinate.cpp
/// \brief CPP file for MultiTextureCoordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MultiTextureCoordinate.h>

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
  texCoord->route( propertyChanged, id );
}

void MultiTextureCoordinate::renderForTextureUnit( int index,
                                                   unsigned int texture_unit ) {
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int i = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( i );
  if( tc ) tc->renderForTextureUnit( index, texture_unit );
}

void MultiTextureCoordinate::renderArrayForTextureUnit( unsigned int texture_unit ){
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) tc->renderArrayForTextureUnit( texture_unit );
}


void MultiTextureCoordinate::disableArrayForTextureUnit( unsigned int texture_unit ){
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) tc->disableArrayForTextureUnit( texture_unit );
}

void MultiTextureCoordinate::render( int index ) {
  stringstream s;
  s << "Cannot use render() function in MultiTextureCoordinate. Use "
    << "renderForTextureUnit() instead to specify which texture "
    << "unit to render for. ";
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

unsigned int MultiTextureCoordinate::nrAvailableTexCoords() {
  unsigned int available_coords = -1; 
  bool first_value = true;

  for( MFTextureCoordinateNode::const_iterator i = texCoord->begin();
       i != texCoord->end();
       ++i ) {
    X3DTextureCoordinateNode *tc = static_cast< X3DTextureCoordinateNode * >(*i);
    
    if( tc->supportsExplicitTexCoords() ) { 
      unsigned int nr_coords = tc->nrAvailableTexCoords();
      if( first_value ) {
        available_coords = nr_coords;
        first_value = false;
      } else {
        available_coords = 
          H3DMin( available_coords, nr_coords );
      }
    }
  }
  return available_coords;
}

void MultiTextureCoordinate::renderArray() {
  stringstream s;
  s << "Cannot use renderArray() function in MultiTextureCoordinate. Use "
    << "renderArrayForTextureUnit() instead to specify which texture "
    << "unit to render for. ";
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

bool MultiTextureCoordinate::supportsExplicitTexCoords() {

  for( MFTextureCoordinateNode::const_iterator i = texCoord->begin();
       i != texCoord->end();
       ++i ) {
    X3DTextureCoordinateNode *tc = static_cast< X3DTextureCoordinateNode * >(*i);
    if( tc->supportsExplicitTexCoords() ) return true;
  }
  return false;
}

bool MultiTextureCoordinate::supportsTexGen() {

  for( MFTextureCoordinateNode::const_iterator i = texCoord->begin();
       i != texCoord->end();
       ++i ) {
    X3DTextureCoordinateNode *tc = static_cast< X3DTextureCoordinateNode * >(*i);
    if( tc->supportsTexGen() ) return true;
  }
  return false;
}


void MultiTextureCoordinate::startTexGenForTextureUnit( unsigned int texture_unit ){
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) tc->startTexGenForTextureUnit( texture_unit );
}

void MultiTextureCoordinate::stopTexGenForTextureUnit( unsigned int texture_unit ){
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) tc->stopTexGenForTextureUnit( texture_unit );
}

void MultiTextureCoordinate::startTexGen() {
  stringstream s;
  s << "Cannot use startTexGen() function in MultiTextureCoordinate. Use "
    << "startTexGenForTextureUnit() instead to specify which texture "
    << "unit to render for. ";
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

void MultiTextureCoordinate::stopTexGen() {
  stringstream s;
  s << "Cannot use stopTexGen() function in MultiTextureCoordinate. Use "
    << "stopTexGenForTextureUnit() instead to specify which texture "
    << "unit to render for. ";
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}


/// Returns true if the getTexCoord function is available for use.
bool MultiTextureCoordinate::supportsGetTexCoord( unsigned int texture_unit ) {
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return false;
  
  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) return tc->supportsGetTexCoord( texture_unit );
  else return false;
}

/// Gets texture coordinate of the given index and texture unit.
Vec4f MultiTextureCoordinate::getTexCoord( int i, 
             unsigned int texture_unit ) {
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return Vec4f( 0, 0, 0, 1 );
   
  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) return tc->getTexCoord( i, texture_unit );
  else return Vec4f( 0, 0, 0, 1 );

}

void MultiTextureCoordinate::renderVertexBufferObject() {
  stringstream s;
  s << "Cannot use renderVertexBufferObject() function in "
    << "MultiTextureCoordinate. Use renderVertexBufferObjectForTextureUnit() "
    << "instead to specify which texture unit to render for. ";
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

void MultiTextureCoordinate::renderVertexBufferObjectForTextureUnit(
  unsigned int texture_unit ){
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) tc->renderVertexBufferObjectForTextureUnit( texture_unit );
}


void MultiTextureCoordinate::disableVertexBufferObjectForTextureUnit(
  unsigned int texture_unit ){
  unsigned int nr_nodes = texCoord->size();
  if( nr_nodes == 0 ) return;

  unsigned int index = H3DMin( nr_nodes - 1, texture_unit );
  X3DTextureCoordinateNode *tc = 
    texCoord->getValueByIndex( index );
  if( tc ) tc->disableVertexBufferObjectForTextureUnit( texture_unit );
}

