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
/// \file MFNode.h 
/// \brief Contains the MFNode class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFNODE_H__
#define __MFNODE_H__

#include <H3D/RefCountMField.h>
#include <H3D/MFNodeAutoRefVector.h>
#include <H3D/FieldTemplates.h>
#include <H3D/Node.h>
#include <H3D/X3DPrototypeInstance.h>

namespace H3D {

  /// \ingroup FieldTypes
  /// \class MFNode
  /// \brief MFNode is almost like any MField but it encapsulates a vector of Node 
  /// pointers. Reference counting on the encapsulated Nodes is upheld.  
  /// 
  class H3DAPI_API MFNode: public RefCountMField< Node > { 
    //    Field > {
  public:   
    
    class const_iterator: public RefCountMField< Node >::const_iterator {
    public:
      typedef RefCountMField< Node >::const_iterator base_const_iterator;
      const_iterator( base_const_iterator i ):
        RefCountMField< Node >::const_iterator( i ) {
      }
      
      Node * const operator*() {
        Node *n = RefCountMField< Node >::const_iterator::operator*();
        X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( n );
        if( pi ) {
          return pi->getPrototypedNode();
        } else {
          return n;
        }
      }
    };
    
    class const_reverse_iterator: 
      public RefCountMField< Node >::const_reverse_iterator {
    public:
      typedef RefCountMField< Node >::const_reverse_iterator base_const_reverse_iterator;
      /// Constructor.
      const_reverse_iterator( base_const_reverse_iterator i ):
        RefCountMField< Node >::const_reverse_iterator( i ) {

      }
      
      Node * const operator*() {
        Node *n = RefCountMField< Node >::const_reverse_iterator::operator*();
        X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( n );
        if( pi ) {
          return pi->getPrototypedNode();
        } else {
          return n;
        }
      }
    };

    /// Returns a const_iterator pointing to the beginning of the vector.
    inline const_iterator begin( int id = 0 ) { 
      return const_iterator( RefCountMField<Node>::begin( id ) );
    }
    
    /// Returns a const_iterator pointing to the end of the vector.
    inline const_iterator end( int id = 0 ) { 
      return const_iterator( RefCountMField<Node>::end( id ) );
    }

    /// Returns a const_reverse_iterator pointing to the beginning of the vector.
    inline const_reverse_iterator rbegin( int id = 0 ) { 
      return const_reverse_iterator( RefCountMField<Node>::rbegin( id ) );
    }

    /// Returns a const_reverse_iterator pointing to the end of the vector.
    inline const_reverse_iterator rend( int id = 0 ) { 
      return const_reverse_iterator( RefCountMField<Node>::rend( id ) );
    }

    /// Returns the n'th element.
    inline virtual Node * operator[](size_type i ) {
      Node *n = RefCountMField< Node >::operator[](i);
      X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( n );
      if( pi ) {
        return pi->getPrototypedNode();
      } else {
        return n;
      }
    }
    
    /// Returns the first element.
    inline virtual Node * front( int id = 0 ) { 
      Node *n = RefCountMField< Node >::front(id);
      X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( n );
      if( pi ) {
        return pi->getPrototypedNode();
      } else {
        return n;
      }
    }

    /// Returns the last element.
    inline virtual Node * back( int id = 0 ) {
      Node *n = RefCountMField< Node >::back(id);
      X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( n );
      if( pi ) {
        return pi->getPrototypedNode();
      } else {
        return n;
      }
    }

    /// Get the value of the MField.
    inline virtual Node * getValueByIndex( 
                                          size_type i,
                                          int id = 0 ) {
      Node *n = RefCountMField< Node >::getValueByIndex( i, id );
      X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( n );
      if( pi ) {
        return pi->getPrototypedNode();
      } else {
        return n;
      }
    }


    
    virtual Node *preOnAdd( Node *n ) {
      Node *pn = getPrototypeNode( n );
      if( pn ) {
        return pn;
      }
      else
        return n;
    }
    
    virtual Node *preOnRemove( Node *n ) {
      Node *pn = getPrototypeNode( n );
      if( pn ) 
        return pn;
      else
        return n;
    }


    /// Default constructor. Creates an empty MFNode.
    MFNode() {}
    
    /// Creates an MFNode with space reserved for size nodes.
    MFNode( size_type sz ) : 
      RefCountMField< Node >( sz ) {} 
    
    /// Get the value of the MField.
    inline virtual const NodeVector &getValue( int id = 0 ) {
      // check that we have the correct access type
      checkAccessTypeGet( id );
      // check that the field is up-to-date first
      this->upToDate();
      return value;
    }

    /// Returns a string name for this field type i.e. MFNode
    virtual string getTypeName() {
      return classTypeName();
    }
    
    /// Returns a string name for this field type i.e. MFNode
    static string classTypeName() {
      return "MFNode";
    }

    /// Returns the X3DType of the field.
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFNODE; }

  protected:
    Node *getPrototypeNode( Node *n );
  };

  template<>
  inline string MFieldBase<Node *>::classTypeName() {
    return "MFNode";
  }  
  
  
  /// Template to make sure that the Nodes that are added to a MFNode are
  /// of a specified Node type.
  /// \param NodeType The expected Node type. Must be a subclass of Node. 
  /// \ingroup FieldTemplateModifiers
  template< class NodeType >
  class TypedMFNode: public MFNode {
  protected:
    /// We check that the type of the Node is of the correct type.
    virtual void onAdd( Node *n) {
      if( !dynamic_cast< NodeType * >( n ) ) {
        Node *pi = getPrototypeNode( n );
        if( !dynamic_cast< NodeType * >( pi ) ) {
          stringstream s;
          s << "Expecting " << typeid( NodeType ).name();
          throw InvalidNodeType( n->getTypeName(),
                                 s.str(),
                                 H3D_FULL_LOCATION ); 
        } 
      }
      MFNode::onAdd( n );
    }
    
  public:
    /// Get the value casted to the NodeType.
    virtual NodeType *getValueByIndex( typename MFNode::size_type i, 
                                       int id = 0 ) {
      return static_cast< NodeType * >( MFNode::getValueByIndex( i, id ) );
    }

    /// Get the value casted to the NodeType.
    virtual NodeType *getCastedValueByIndex( typename MFNode::size_type i, 
                                             int id = 0 ) {
      return this->getValueByIndex( i, id );
    }
  };

  /// Template to make sure that the Nodes that are set in a MFNode are
  /// of a specified type. This is the same as TypedMFNode but with 
  /// the difference that the type does not have to be a node type
  /// but can be an abstract base class such as X3DMetadataObject.
  /// This makes it impossible however to override the getValueByIndex()
  /// function to return the correct type. Hence the getCastedValueByIndex()
  /// function must be used. You only have to use this template if
  /// you want to use one of the interface base classes such as 
  /// X3DMetadataObject and H3DDisplayListObject as the Type parameter.
  /// In all other cases the TypedMFNode template is recommended.
  /// \param Type The expected type. 
  /// \ingroup FieldTemplateModifiers
  template< class Type >
  class TypedMFNodeObject: public MFNode {
  protected:
    /// We check that the type of the Node is of the correct type.
    void onAdd( Node *n) {
      if( !dynamic_cast< Type * >( n ) ) {
        Node *pi = getPrototypeNode( n );
        if( !dynamic_cast< Type * >( pi ) ) {
          stringstream s;
          s << "Expecting " << typeid( Type ).name();
          throw InvalidNodeType( n->getTypeName(),
                                 s.str(),
                                 H3D_FULL_LOCATION ); 
        } 
      }
      MFNode::onAdd( n );
    }
    
  public:
    /// Get the value casted to the Type.
    virtual Type *getCastedValueByIndex( typename MFNode::size_type i, 
                                         int id = 0 ) {
      return reinterpret_cast< Type * >( this->getValueByIndex( i, id ) );
    }
  };
  
  
}

#endif
