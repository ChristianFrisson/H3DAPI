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
/// \file SFNode.h
/// \brief Contains the MFNode class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SFNODE_H__
#define __SFNODE_H__

#include "FieldTemplates.h"
#include "RefCountSField.h"
#include "Node.h"


namespace H3D {
  

  /// SFNode is almost like any SField but it encapsulates a pointer to 
  /// a Node. Reference counting on the encapsulated Node is also upheld.  
  /// \ingroup FieldTypes
  class H3DAPI_API SFNode: 
    public RefCountSField< Node,
                           TypedField< Field,
                                       void, 
                                       AnyNumber< SFNode > > > {
  public:   
    /// Constructor.
    SFNode() {}
    
    /// Constructor.
    SFNode( Node *_value ):
      RefCountSField< Node,
                      TypedField< Field,
                                  void, 
                                  AnyNumber< SFNode > > > ( _value ) {}
    
    /// Returns a string name for this field type i.e. SFNode
    virtual string getTypeName() {
      return classTypeName();
    }
    
    /// Returns a string name for this field type i.e. SFNode
    static string classTypeName() {
      return "SFNode";
    }
    /// Returns the X3DType of the field.
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFNODE; }
  };

  /// Template to make sure that the Node that is set in a SFNode is
  /// of a specified Node type.
  /// \param NodeType The expected Node type. Must be a subclass of Node.
  /// \ingroup FieldTemplateModifiers
  template< class NodeType >
  class TypedSFNode: public SFNode {
  protected:
    /// The subclass of value_type of that the TypedSFNode has made
    /// sure that the node is of.
    typedef NodeType typed_class_type;
    /// A pointer type to the subclass value_type of that the TypedSFNode
    /// has made sure that the node is of.
    typedef NodeType * typed_value_type;

    /// We check that the type of the Node is of the correct type.
    void onAdd( Node *n) {
      if( !dynamic_cast< NodeType * >( n ) ) {
        stringstream s;
        s << "Expecting " << typeid( NodeType ).name() << ends;
        throw InvalidNodeType( n->getTypeName(),
                               s.str(),
                               H3D_FULL_LOCATION );
  
      }
      else { 
        SFNode::onAdd( n );
      }
    }
  public:
    /// Get the value casted to the NodeType.
    virtual NodeType *getValue( int id = 0 ) {
      return static_cast< NodeType * >( SFNode::getValue( id ) );
    }

    /// Get the value casted to the NodeType.  
    virtual NodeType *getCastedValue( int id = 0 ) {
      return this->getValue();
    }

  };

  /// Template to make sure that the Node that is set in a SFNode is
  /// of a specified type. This is the same as TypedSFNode but with 
  /// the difference that the type does not have to be a node type
  /// but can be an abstract base class such as X3DMetadataObject.
  /// This makes it impossible however to override the getValue()
  /// function to return the correct type. Hence the getCastedValue()
  /// function must be used. You only have to use this template if
  /// you want to use one of the interface base classes such as 
  /// X3DMetadataObject and H3DDisplayListObject as the Type parameter.
  /// In all other cases the TypedSFNode template is recommended.
  /// \param Type The expected type. 
  /// \ingroup FieldTemplateModifiers
  template< class Type >
  class TypedSFNodeObject: public SFNode {
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
        SFNode::onAdd( n );
      }
    }
  public:
    /// Get the value casted to the Type.
    virtual Type *getCastedValue( int id = 0 ) {
      return reinterpret_cast< Type * >( this->getValue( id ) );
    }
  };
}
#endif
