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
/// \file MFNode.h 
/// \brief Contains the MFNode class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFNODE_H__
#define __MFNODE_H__

#include "RefCountMField.h"
#include "MFNodeAutoRefVector.h"
#include "FieldTemplates.h"
#include "Node.h"

namespace H3D {

  /// \ingroup FieldTypes
  /// \class MFNode
  /// \brief MFNode is almost like any MField but it encapsulates a vector of Node 
  /// pointers. Reference counting on the encapsulated Nodes is upheld.  
  /// 
  class H3DAPI_API MFNode: public RefCountMField< Node > { 
    //    Field > {
  public:   
    
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
    
    /// Returns a string name for this field type i.e. SFNode
    virtual string getTypeName() {
      return classTypeName();
    }
    
    /// Returns a string name for this field type i.e. SFNode
    static string classTypeName() {
      return "MFNode";
    }

    /// Returns the X3DType of the field.
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFNODE; }
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
        stringstream s;
        s << "Expecting " << typeid( NodeType ).name() << ends;
        throw InvalidNodeType( n->getTypeName(),
                               s.str(),
                               H3D_FULL_LOCATION ); 
      } else {
        MFNode::onAdd( n );
      }
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

  /// Template to make sure that the Nodes that is set in a SFNode is
  /// of a specified type. This is the same as TypedMFNode but with 
  /// the difference that the type does not have to be a node type
  /// but can be an abstract base class such as X3DMetadataObject.
  /// This makes it impossible however to override the getValueByIndex()
  /// function to return the correct type. Hence the getCastedValueByIndex()
  /// function must be used. You only have to use this template if
  /// you want to use one of the interface base classes such as 
  /// X3DMetadataObject and H3DDisplayListObject as the Type parameter.
  /// In all other cases the TypedSFNode template is recommended.
  /// \param Type The expected type. 
  /// \ingroup FieldTemplateModifiers
  template< class Type >
  class TypedMFNodeObject: public MFNode {
  protected:
    /// We check that the type of the Node is of the correct type.
    void onAdd( Node *n) {
      if( !dynamic_cast< Type * >( n ) ) {
        stringstream s;
        s << "Expecting " << typeid( Type ).name() << ends;
        throw InvalidNodeType( n->getTypeName(),
                               s.str(),
                               H3D_FULL_LOCATION ); 
      } else {
        MFNode::onAdd( n );
      }
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
