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
/// \file H3DScriptNode.cpp
/// \brief cpp file for H3DScriptNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DScriptNode.h>

using namespace H3D;

H3DNodeDatabase H3DScriptNode::database( 
        "H3DScriptNode", 
        NULL,
        typeid( H3DScriptNode ),
        &X3DChildNode::database 
        );

namespace H3DScriptNodeInternals {
  FIELDDB_ELEMENT( H3DScriptNode, url, INITIALIZE_ONLY );
}

H3DScriptNode::H3DScriptNode( Inst< MFString > _url ) : 
  X3DUrlObject( _url ) {
  type_name = "H3DScriptNode";
  database.initFields( this );
}

H3DScriptNode::~H3DScriptNode() {
  clearNamedNodes();
}

void H3DScriptNode::addNamedNodes( X3D::DEFNodes *dn ) {
  if( !dn ) return;

  for( X3D::DEFNodes::const_iterator i = dn->begin();
       i != dn->end(); i++ ) {
    addNamedNode( (*i).first, (*i).second );
  }
}

void H3DScriptNode::addNamedNode( const string &name, Node *n ) {
  NamedNodes::iterator i = named_nodes.find( name );
  if( i != named_nodes.end() ) {
    (*i).second->removeDestructCallback( removeNamedNodeCB, this );
  }
  named_nodes[ name ] = n;
  n->addDestructCallback( removeNamedNodeCB, this );
}

int H3DScriptNode::removeNamedNode( const string &name ) {
  NamedNodes::iterator i = named_nodes.find( name );
  if( i == named_nodes.end() ) {
    return -1;
  } else {
    named_nodes.erase( i );
    (*i).second->removeDestructCallback( removeNamedNodeCB, this );
    return 0;
  }
}
  
void H3DScriptNode::clearNamedNodes() {
  for( NamedNodes::iterator i = named_nodes.begin();
       i != named_nodes.end(); i++ ) {
    (*i).second->removeDestructCallback( removeNamedNodeCB, this );
  }
  named_nodes.clear();
}

Node *H3DScriptNode::getNamedNode( const string &name ) {
  NamedNodes::iterator i = named_nodes.find( name );
  if( i == named_nodes.end() ) {
    return NULL;
  } else {
    return (*i).second;
  }
}

void H3DScriptNode::removeNamedNodeCB( Node *n, void *data ) {
  H3DScriptNode *script_node = static_cast< H3DScriptNode * >( data );
  for( NamedNodes::iterator i = script_node->named_nodes.begin();
       i != script_node->named_nodes.end(); i++ ) {
    if( (*i).second == n ) {
      script_node->named_nodes.erase( i );
      break;
    }
  }
}
