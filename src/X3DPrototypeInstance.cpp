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
/// \file X3DPrototypeInstance.cpp
/// \brief CPP file for X3DPrototypeInstance, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DPrototypeInstance.h>
#include <H3D/H3DDisplayListObject.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DPrototypeInstance::database( 
                                               "X3DPrototypeInstance", 
                                               NULL,
                                               typeid( X3DPrototypeInstance ),
                                               &X3DNode::database );

X3DPrototypeInstance::X3DPrototypeInstance( Inst< SFNode>  _metadata ):
  X3DNode( _metadata ),
  H3DDynamicFieldsObject( &database ),
  H3DBoundedObject(),
  prototyped_node( NULL ) {
  type_name = "X3DPrototypeInstance";

  bound->setOwner( this );
  bound->setValue( new EmptyBound );
}

void X3DPrototypeInstance::render() {
  if( prototyped_node.get() ) {
    H3DDisplayListObject *dlo = 
      dynamic_cast< H3DDisplayListObject * >( prototyped_node.get() );
    if( dlo ) {
      dlo->displayList->callList();
    } else {
      prototyped_node.get()->render();
    }
  }

  for( unsigned int i = 0; i < prototyped_node_extras.size(); i++ ) {
    H3DDisplayListObject *dlo = 
      dynamic_cast< H3DDisplayListObject * >( prototyped_node_extras[i] );
    if( dlo ) {
      dlo->displayList->callList();
    } else {
      prototyped_node_extras[i]->render();
    }
  }
}
 
void X3DPrototypeInstance::traverseSG( TraverseInfo &ti ) {
  if( prototyped_node.get() )
    prototyped_node.get()->traverseSG( ti );

  for( unsigned int i = 0; i < prototyped_node_extras.size(); i++ ) {
    prototyped_node_extras[i]->traverseSG( ti );
  }
  
}

bool X3DPrototypeInstance::connectField( const string &proto_field_name,
                                         Field *node_field ) {
  Field *proto_field = 
    this->getField( proto_field_name );

  if( !proto_field ) return false;

  Field::AccessType access_type = node_field->getAccessType();
  
  // set up routes from node_field to proto field
  if( access_type == Field::OUTPUT_ONLY ) {
    node_field->route( proto_field, this->id );
  } else if ( access_type == Field::INPUT_OUTPUT ) {
    node_field->routeNoEvent( proto_field, this->id );
  } else if( access_type == Field::INITIALIZE_ONLY ) {
		bool access_check_node_field = node_field->isAccessCheckOn();
		node_field->setAccessCheck( false );
		bool access_check_proto_field = proto_field->isAccessCheckOn();
		proto_field->setAccessCheck( false );
		node_field->route( proto_field, this->id );
		node_field->setAccessCheck( access_check_node_field );
		proto_field->setAccessCheck( access_check_proto_field );
	}
  
  // and the other way around proto_field -> node_field
  if( access_type != Field::OUTPUT_ONLY &&
			access_type != Field::INITIALIZE_ONLY ) {
    proto_field->route( node_field, this->id );
  }

  return true;
}

void X3DPrototypeInstance::setPrototypedNode( Node *n ) {
  if( n ) 
    n->setProtoInstanceParent( this );
  
  // remove old bound route
  if( prototyped_node.get() ) {
    H3DBoundedObject *b = 
      dynamic_cast< H3DBoundedObject * >( prototyped_node.get() );
    if( b ) {
      b->bound->unroute( bound );
    }
  }

  // set up new bound route
  if( n ) {
    H3DBoundedObject *b = dynamic_cast< H3DBoundedObject * >( n );
    if( b ) {
      b->bound->route( bound );
    }
  }

  prototyped_node.reset( n );
}

