//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file Node.cpp
/// \brief cpp file for Node base class
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Node.h>

using namespace H3D;

H3DNodeDatabase Node::database( typeid( Node ) );
int Node::nr_nodes_created = 0;

Node::Node() : 
  RefCountedClass( ),
  id( nr_nodes_created++ ),
  proto_parent( NULL ) {}

Field *Node::getField( const string &name ) {
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *this ) );
  if ( db )
    return db->getField( this, name ); 
  else
    return NULL;  // Should probably throw an error in this case
}

/// Add a callback function to be run on destruction of node.
/// Returns 0 on success.
int Node::addDestructCallback( void (*func)( Node *, void * ), void *args ) {
  destruct_callbacks.push_back( make_pair( func, args ) );
  return 0;
}

/// Add a callback function to be run on destruction of node.
/// Returns 0 on success, -1 if the callback does not exist.
int Node::removeDestructCallback( void (*func)( Node *, void * ), void *args ) {
  DestructCallbacks::iterator i = 
    std::find( destruct_callbacks.begin(), destruct_callbacks.end(), 
               make_pair( func, args ) );
  if( i == destruct_callbacks.end() ) {
    return -1;
  } else {
    destruct_callbacks.erase( i );
    return 0;
  }
}

Node::~Node() {
  for( DestructCallbacks::iterator i = destruct_callbacks.begin();
       i != destruct_callbacks.end(); i++ ) {
    (*i).first( this, (*i).second );
  }
}
