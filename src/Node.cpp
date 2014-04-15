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
/// \file Node.cpp
/// \brief cpp file for Node base class
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Node.h>
#include <H3D/MFNode.h>
#include <H3D/MFString.h>
#include <H3D/SFString.h>
#include <H3D/H3DDynamicFieldsObject.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/X3DTypeFunctions.h>

using namespace H3D;

H3DNodeDatabase Node::database( typeid( Node ) );
int Node::nr_nodes_created = 0;

Node::Node() : 
  RefCountedClass( ),
  id( nr_nodes_created++ ),
  proto_parent( NULL ) {}

Node* Node::clone ( bool deepCopy, DeepCopyMap *deepCopyMap ) {
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *this ) );
  if ( db ) {
    bool delete_deep_copy_map = false;
    if( deepCopyMap == NULL ) {
      deepCopyMap = new DeepCopyMap;
      delete_deep_copy_map = true;
    }
    // Create a new instance of this node type
    Node* n= db->createNode ();
    
    // Also clone base URL so that the same resources can be loaded
    X3DUrlObject* url_obj= dynamic_cast<X3DUrlObject*> ( this );
    if ( url_obj ) {
      X3DUrlObject* n_url_obj= dynamic_cast<X3DUrlObject*> ( n );
      n_url_obj->setURLBase ( url_obj->getURLBase () );
    }

    // For all dynamic fields
    H3DDynamicFieldsObject* dfo= dynamic_cast<H3DDynamicFieldsObject*> ( this );
    if ( dfo ) {
      H3DDynamicFieldsObject* n_dfo= dynamic_cast<H3DDynamicFieldsObject*> ( n );
      for ( H3DDynamicFieldsObject::field_iterator i= dfo->firstField(); i != dfo->endField(); ++i ) {
        Field* f_from= *i;
        Field* f_to= X3DTypes::newFieldInstance ( f_from->getX3DType() );

        if ( f_to ) {
          // Add this field in the clone
          if ( n_dfo->addField( f_from->getName(), f_from->getAccessType(), f_to ) ) {
            // Copy the field value
            cloneFieldValue ( *f_from, *f_to, deepCopy, *deepCopyMap );
          } else {
            delete f_to;
            Console(4) << "Warning: Failed to add cloned dynamic field " << f_from->getFullName() << "!" << endl;
          }
        } else {
          Console(4) << "Warning: Failed to clone dynamic field " << f_from->getFullName() << "!" << endl;
        }
      }
    }

    // For each (static) field
    for ( H3DNodeDatabase::FieldDBConstIterator i= db->fieldDBBegin(); i != db->fieldDBEnd(); ++i ) {
      Field* f_from= getField ( *i );
      Field* f_to= n->getField ( *i );

      // Copy the field value
      if ( f_from && f_to ) {
        cloneFieldValue ( *f_from, *f_to, deepCopy, *deepCopyMap );
      }
    }

    if( delete_deep_copy_map ) {
      delete deepCopyMap;
      deepCopyMap = NULL;
    }

    return n; 
  } else {
    return NULL;
  }
}

Node* Node::getClonedInstance ( Node* original, bool deepCopy, DeepCopyMap& deepCopyMap ) {
  if ( original ) {
    if ( !deepCopy ) {
      // For shallow copy return the original
      return original;
    } else {
      // Deep copy
      // First check if the node has already been cloned
      DeepCopyMap::iterator i= deepCopyMap.find ( original );
      if ( i != deepCopyMap.end() ) {
        // The node has already been cloned, return that instance
        return (*i).second;
      } else {
        // The node has not already been cloned, clone it and add to the map
        Node* n= original->clone( deepCopy, &deepCopyMap );
        deepCopyMap[original]= n;
        return n;
      }
    }
  } else {
    return NULL;
  }
}

void Node::cloneFieldValue ( Field& _from, Field& _to, bool deepCopy, DeepCopyMap& deepCopyMap ) {
  // Only copy INPUT_OUTPUT and INITIALIZE_ONLY
  if ( _from.getAccessType() == Field::INPUT_OUTPUT || _from.getAccessType() == Field::INITIALIZE_ONLY ) {

    // MField types
    if( MFieldClass *mfield_from = dynamic_cast< MFieldClass * >( &_from ) ) {
            
      // MFNode
      if( MFNode *mfnode_from = dynamic_cast< MFNode * >( &_from ) ) {
        if ( MFNode *mfnode_to = dynamic_cast< MFNode * >( &_to ) ) {
          NodeVector src= mfnode_from->getValue();
          for ( size_t j= 0; j < src.size(); ++j ) {
            src.set ( j, getClonedInstance ( src[j], deepCopy, deepCopyMap ) );
          }
          mfnode_to->setValue ( src );
        }

        // MFString
      } else if ( MFString *mfstring_from = dynamic_cast< MFString * > ( &_from ) ) {
        // MFString does not work with getValueAsVoidPtr. Do special case or fix getValueAsVoidPtr.
        if ( MFString *mfstring_to = dynamic_cast< MFString * >( &_to ) ) {
          if ( !mfstring_from->getValueAsString().empty() ){
            // avoid setting value when not actaully needed
            mfstring_to->setValue ( mfstring_from->getValue ( ) );
          }
        }

        // Generic MField
      } else {
        if ( MFieldClass *mfield_to = dynamic_cast< MFieldClass * >( &_to ) ) {
          unsigned int data_size = mfield_from->valueTypeSize() * mfield_from->size();
          unsigned int nr_elements;
          unsigned char* data = new unsigned char[ data_size ];
          int result = mfield_from->getValueAsVoidPtr( data, nr_elements, data_size );
          if ( result != -1 ) {
            mfield_to->setValueFromVoidPtr ( data, nr_elements, data_size );
          }
          delete [] data;
        }
      }

      // SField types
    } else if( SFieldClass *sfield_from = dynamic_cast< SFieldClass * >( &_from ) ) {
      
      // SFString
      if( SFString *sfstring_from = dynamic_cast< SFString * > ( &_from ) ) {
        // the getValueAsVoidPtr function does not work well with
        // SFString fields so do a special case for that type.
        if ( SFString *sfstring_to = dynamic_cast< SFString * >( &_to ) ) {
          if ( !sfstring_from->getValue().empty() ){
            // avoid setValue for sfstring_to when not really needed
            sfstring_to->setValue ( sfstring_from->getValue ( ) );
          }
        }
        // Generic SField
      } else {
        if ( SFieldClass *sfield_to = dynamic_cast< SFieldClass * >( &_to ) ) {
          unsigned int data_size = sfield_from->valueTypeSize();
          unsigned char* data = new unsigned char[ data_size ];
          data_size = sfield_from->getValueAsVoidPtr( data, data_size );
          if ( data_size != -1 ) {
            sfield_to->setValueFromVoidPtr ( data, data_size );
          }
          delete [] data;
        }
      }

      // SFNode
    } else if( SFNode *sfnode_from = dynamic_cast< SFNode * >( &_from ) ) {
      if ( SFNode *sfnode_to = dynamic_cast< SFNode * >( &_to ) ) {
        Node* n= getClonedInstance ( sfnode_from->getValue(), deepCopy, deepCopyMap );
        sfnode_to->setValue ( n );
      }
    }
  }
}

Field *Node::getField( const string &name ) const {
  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( this );
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
       i != destruct_callbacks.end(); ++i ) {
    (*i).first( this, (*i).second );
  }
}
