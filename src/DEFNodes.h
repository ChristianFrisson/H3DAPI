//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Danel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file DEFNodes.h Contains the DEFNodes class
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __DEFNODES_H__
#define __DEFNODES_H__

#include "Exception.h"
#include "Node.h"
#include <map>
#include <string>

using namespace std;

namespace H3D {
  namespace X3D {
    /// An exception for errors when the Node type is not what was
    /// expected.  
    /// 
    H3D_VALUE_EXCEPTION( const string, InvalidNodeType );

    /// An exception for errors when the DEF name does not exist in the
    /// DEF nodes structure.  
    /// 
    H3D_VALUE_EXCEPTION( const string, NoSuchDEFName );

    /// Provides a mapping between defined DEF names in X3D and 
    /// the nodes they refer to.
    /// 
    class H3DAPI_API DEFNodes : 
      private map< const string, Node * > { //, Util::LtStr > {
    public: 
      
      /// Destructor. unrefs all the nodes in the map.
      ~DEFNodes() {
		  for( iterator i = map< const string, Node * >::begin(); 
			   i != map< const string, Node * >::end(); 
			   i++ ) {
          (*i).second->unref();
        }
      }
      
      typedef  map< const string, Node * >::const_iterator const_iterator;

      /// Returns a const_iterator to the first element
      /// in the DEFNodes struct.
      const_iterator begin() {
        return map< const string, Node * >::begin();
      }

      /// Returns a const_iterator to the end element
      /// in the DEFNodes struct.
      const_iterator end() {
        return map< const string, Node * >::end();
      }

      /// Adds a DEF name/Node * pair to the structure.
      /// \param def_name The DEF name of the node.
      /// \param def_node The Node assiciated with the given DEF name. 
      ///
      void addNode( const string &def_name, Node *def_node ) {
        iterator i = find( def_name );
        // increase the reference counter for the new node
        def_node->ref();
        if( i == end() ) {
          insert( make_pair( def_name, def_node ) );
        } else {
          // the def name already exists in the map. Since the node
          // that is there will be overwritten we unref it.
          (*i).second->unref();
          (*i).second = def_node;
        }
      }

      /// Removes a DEF name/Node * pair from the structure.
      /// \param def_name The DEF name to remove.
      ///
      void removeNode( const string& def_name ) {
        iterator i = find( def_name );
        if( i == end() ) {
          throw X3D::NoSuchDEFName( def_name );
        } else {
          // unref the node that is removed
          (*i).second->unref();
          erase( i );
        }
      }

      /// Add the entries from another DEFNodes structure do this one.
      /// If an entry already exists it will be replaced.
      void merge( DEFNodes *dn ) {
	if( dn ) {
	  for( const_iterator i = dn->begin(); i != dn->end(); i++ )
	    addNode( (*i).first, (*i).second );
	}
      }

      /// Removes all the entries in the map.
      void clear() {
	for( const_iterator i = begin(); i != end(); i++ )
          // unref the node that is removed
          (*i).second->unref();
	map< const string, Node * >::clear();
      }

      /// Get the node by its DEF name.
      /// \param def_name The DEF name of the node as specified in the 
      /// X3D input.
      /// \return The Node with the given DEF name. NULL if no such node 
      /// exists.
      ///
      Node *getNode( const string &def_name ) {
        iterator i = find( def_name );
        if( i == end() ) {
          return NULL; //throw Exception::NoSuchDEFNameInX3D( def_name, "getNode()" );
        } else {
          return (*i).second;
        }
      }

      /// Get the node by its DEF name.
      /// \param def_name The DEF name of the node as specified in the 
      /// X3D input.
      /// \param return_node This pointer ref will be set to the the Node
      /// with the given DEF name. NULL if no such node exists.  
      /// \throws InvalidNodeType Thrown when the DEF Node type
      /// is not a subclass of the type of return_node.
      ///      
      template< class NodeType >
      void getNode( const string &def_name, 
                    NodeType *&return_node ) {
        Node *node = getNode( def_name ) ;
        NodeType *n = dynamic_cast< NodeType * >( node );
        if( !n ) {
          stringstream msg;
          msg << "DEFNodes::getNode (expecting " 
              << typeid( NodeType ).name() 
              << ")" << ends;
          throw InvalidNodeType( typeid( *node ).name(), 
                                 msg.str() );
        }
        return_node = n;
      }; 
      
    };
  }
}

#endif
