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
/// \file DependentNodeFields.h Contains the DependentSFNode and 
/// DependentMFNode template classes.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEPENDENTNODEFIELDS_H__
#define __DEPENDENTNODEFIELDS_H__

#include <H3D/SFNode.h>
#include <H3D/MFNode.h>

namespace H3D {

  /// Class used for specifying a field member of a node in a
  /// DependentSFNode or DependentMFNode specification.
  /// \param NodeType The type of the Node the field is defined in.
  /// \param FieldType The type of the Field.
  /// \param member_ptr A member pointer to the field in the node.
  ///
  /// \par Example:
  /// FieldRef< Coordinate, MFVec3f, &Coordinate::point >
  ///
  template< class NodeType, 
            class FieldType, 
            auto_ptr< FieldType > NodeType:: *member_ptr >
  class FieldRef {
  public:
    typedef NodeType node_type;
    typedef FieldType field_type;
    typedef auto_ptr< FieldType > NodeType:: * member_ptr_type;
    static Field *getField( NodeType *n ) {
      return (n->*member_ptr).get();
    }
    static member_ptr_type getMemberPtr() {
      return member_ptr;
    }
  };

  namespace FieldInternals {
    
    /// \internal 
    /// Help class for setting up routes from dependent fields
    /// in a DependentSFNode or DependentMFNode.
    ///
    template< class FieldRefType >
    class DoRoutes {
    public:
      /// The Node type in the FieldRef we are to route from. 
      typedef typename FieldRefType::node_type node_type;
            
      /// Set up a route from the field referenced by the 
      /// FieldRefType template argument in the given Node n
      /// to the given field f.
      ///
      static void route( node_type *n, Field *f ) {
        FieldRefType::getField( n )->route( f ); 
      }

      /// Unroute the route from the field referenced by the 
      /// FieldRefType template argument in the given Node n
      /// to the given field f.
      ///
      static void unroute( node_type *n, Field *f ) {
        FieldRefType::getField( n )->unroute( f ); 
      }
    };
        
    /// \internal Template specialization for when we have 
    /// several FieldRefTypes
    ///
    template< class FieldRefType, class FieldRefs >
    class DoRoutes< pair< FieldRefType, FieldRefs > > {
    public:
      /// The Node type in the FieldRef we are to route from. 
      typedef typename FieldRefType::node_type node_type;       
            
      /// Set up a route from the field referenced by the 
      /// FieldRefType template argument in the given Node n
      /// to the given field f.
      ///
      static void route( node_type *n, Field *f ) {
        FieldRefType::getField( n )->route( f ); 
        DoRoutes< FieldRefs >::route( n , f );
      }

      /// Unroute the route from the field referenced by the 
      /// FieldRefType template argument in the given Node n
      /// to the given field f.
      ///
      static void unroute( node_type *n, Field *f ) {
        DoRoutes< FieldRefs >::unroute( n , f );
        FieldRefType::getField( n )->unroute( f ); 
      }
    };

    /// \internal
    /// This Field class is used to propagate all events from the 
    /// fields we are dependent on to the main field. We route all
    /// the dependent fields to an instance of this field and on any
    /// event from this field an sendEvent () call is made on the main
    /// field.
    ///
    class H3DAPI_API EventField: public AutoUpdate< Field > {
    public:
      /// Constructor.
      /// \param f The containing DependentSFNode.
      EventField( Field *f ):
        field( f ) {
        name = "EventField";
      }

      /// Send an event on the containing field.
      virtual void update() {
        field->touch();
      }
      /// The DependentSFNode containing this instance of the class.
      Field *field;
    };

    /// \internal
    /// Common base template class for DependentSFNode and
    /// DependentMFNode. We override the onAdd and onRemove
    /// to maintain the routes from the dependent fields in the Node
    /// contained in the field.
    ///
    template< class BaseField, class FieldRefs >
    class DependentNodeFieldBase: public BaseField {
    public:
      typedef typename FieldRefs::node_type fr_node_type; 
            
      /// Constructor. Default value of value is NULL.
      DependentNodeFieldBase() {
        event_field.reset( new EventField( this ) );
      }
            
      // Constructor.
      //            DependentNodeField( Node *_value ) : 
      //                BaseFieldType( _value ) {
      //                event_field.reset( new EventField( this ) );
      //                }
    protected:
      /// The instance of EventField used to propagate the events
      /// from the fields we are dependent on to the main field.
      auto_ptr< EventField > event_field;
    };

    /// \internal
    /// Common base template class for DependentSFNode and
    /// DependentMFNode. We override the onAdd and onRemove
    /// to maintain the routes from the dependent fields in the Node
    /// contained in the field.
    ///
    template< class BaseField, class FieldRefs, bool DynamicCast = false >
    class DependentNodeField: public DependentNodeFieldBase< BaseField, FieldRefs > {
    protected:
      typedef typename FieldRefs::node_type fr_node_type;

      /// When a Node is added we route all the fields of the Node that
      /// we are dependent on to the event_field.
      /// 
      virtual void onAdd( Node *n ) {
        BaseField::onAdd( n );
        fr_node_type *node = static_cast< fr_node_type * >( n );
        if( node )
        DoRoutes< FieldRefs >::route( node,
                                      this->event_field.get() );
      }
            
            
      /// When a Node is added we unroute all the fields of the Node that
      /// we are dependent on to the event_field.
      /// 
      virtual void onRemove( Node *n ) {
        fr_node_type *node = static_cast< fr_node_type * >( n );
        if( node )
        DoRoutes< FieldRefs >::unroute( node, 
                                        this->event_field.get() );
        BaseField::onRemove( n );
      }
    };

    template< class BaseField, class FieldRefs >
    class DependentNodeField< BaseField, FieldRefs, true >: public DependentNodeFieldBase< BaseField, FieldRefs > {
    protected:
      typedef typename FieldRefs::node_type fr_node_type;

      /// When a Node is added we route all the fields of the Node that
      /// we are dependent on to the event_field.
      /// 
      virtual void onAdd( Node *n ) {
        BaseField::onAdd( n );
        fr_node_type *node = dynamic_cast< fr_node_type * >( n );
        if( node )
        DoRoutes< FieldRefs >::route( node,
                                      this->event_field.get() );
      }
            
            
      /// When a Node is added we unroute all the fields of the Node that
      /// we are dependent on to the event_field.
      /// 
      virtual void onRemove( Node *n ) {
        fr_node_type *node = dynamic_cast< fr_node_type * >( n );
        if( node )
        DoRoutes< FieldRefs >::unroute( node, 
                                        this->event_field.get() );
        BaseField::onRemove( n );
      }
    };
  }
  /// DependentSFNode are modified TypedSFNode fields where the field 
  /// dirty status is dependent on fields in the node it contains. 
  /// E.g. if we have a Coordinate node, a TypedSFNode< Coordinate > would 
  /// only generate an event to fields it is routed to if the Coordinate 
  /// node itself is changed. If the points in the Coordinate nodes point 
  /// field are changed however no event is generated. With a DependentSFNode
  /// you can specify that the field is dependent on e.g. the 'point' field
  /// of the Node and then the field will generate an event when the point
  /// field in the Coordinate node generates an event. 
  ///
  /// \par Usage:
  /// If the member variable is declared in the class we want to use 
  /// as NodeType we can specify it as 
  /// \code 
  /// DependentSFNode< FieldRef< Coordinate, SFVec3f, &Coordinate::point > >
  /// \endcode
  /// \par 
  /// If we want to make it dependent on several fields in the node we can
  /// do that by creating a list of FieldRef with the pair template e.g.
  /// \code
  /// DependentSFNode< pair< FieldRef< Coordinate, SFVec3f, &Coordinate::point >, 
  ///                  pair< FieldRef< Coordinate, SFVec3f, &Coordinate::field2 >, 
  ///                        FieldRef< Coordinate, SFVec3f, &Coordinate::field3 > > > 
  /// \endcode
  /// \par
  /// If the member variable is declared in a super class of the node we
  /// want to use as NodeType we have to use another syntax. The NodeType
  /// is the specified separately from the FieldRef templates and the 
  /// NodeType in the FieldRef must be the super class the field is declared
  /// in. E.g.
  /// \code
  /// DependentSFNode< Shape, FieldRef< X3DShapeNode, SFVec3f, &Shape::bboxSize > >
  /// \endcode
  /// \par
  /// As above, if we want to be dependent on several fields we can use the
  /// pair template,
  /// \code
  ///  DependentSFNode< Shape, pair< FieldRef< X3DShapeNode, SFVec3f, &Shape::bboxSize >, 
  ///                                FieldRef< X3DShapeNode, SFVec3f, &Shape::bboxCenter > > >, 
  /// \endcode
  /// 
  /// In the example above the Node that is set in the field must always be of the type that
  /// is specified in the FieldRef. But if you e.g. have a MFChild field and want to be 
  /// dependent on the displayList field of the children nodes that have one(i.e. inherits
  /// from H3DDisplayListObject then you can specify that as
  /// \code
  /// struct H3DAPI_API MFChild :
  /// public DependentMFNode< X3DChildNode,
  ///                         FieldRef< H3DDisplayListObject,
  ///                         H3DDisplayListObject::DisplayList,
  ///                         &H3DDisplayListObject::displayList >,
  ///                         true >
  /// \endcode
  /// It is the last parameter set to true that will ignore all Nodes that are not of 
  /// H3DDisplayListObject.
  ///
  /// \ingroup FieldTemplateModifiers
  template< class NodeType, class FieldRefs = void, bool DynamicCast = false > 
  class DependentSFNode: 
    public FieldInternals::DependentNodeField< 
    TypedSFNode< NodeType >,
    FieldRefs, DynamicCast > {
  public:
        
    /// Constructor. Default value of value is NULL.
    DependentSFNode(){}

    /// Destructor.
    ~DependentSFNode(){
      this->value = NULL;
    }

    /// Constructor.
    //        DependentSFNode( Node *_value ) : 
    //            FieldInternals::DependentNodeField< TypedSFNode< NodeType >,
    //                           FieldRefs >( _value ) {}
        
  };

  /// \internal
  /// Template specialization to handle the case when we only use FieldRefs.
  ///
  template< class FieldRefs, bool DynamicCast > 
  class DependentSFNode< FieldRefs, void, DynamicCast >: 
    public FieldInternals::DependentNodeField< 
    TypedSFNode< typename FieldInternals::DoRoutes< FieldRefs >::node_type >,
    FieldRefs, DynamicCast >  {
  public:
        
    /// Constructor. Default value of value is NULL.
    DependentSFNode(){}
    ~DependentSFNode(){ 
      this->value = NULL;
    }

    /// Constructor.
    //        DependentSFNode( Node *_value ) : 
    //            FieldInternals::DependentNodeField< 
    //            TypedSFNode< typename DoRoutes< FieldRefs >::node_type >,
    //            FieldRefs >( _value ) {}
        
  };

  /// DependentMFNode are modified TypedMFNode fields where the field 
  /// dirty status is dependent on fields in the nodes it contains.
  /// See the documentation for DependentSFNode for info on how it is
  /// used.
  ///
  /// \ingroup FieldTemplateModifiers
  template< class NodeType, class FieldRefs = void, bool DynamicCast = false > 
  class DependentMFNode: 
    public FieldInternals::DependentNodeField< 
    TypedMFNode< NodeType >,
    FieldRefs,
    DynamicCast > {
  public:
        
    /// Constructor. Default value of value is NULL.
    DependentMFNode(){}
      
    /// Destructor.
    ~DependentMFNode(){
      this->clear( this->ownerId() );
    }

    /// Creates an Dependent with space reserved for size nodes.
    //        DependentMFNode( MFNode::size_type size ) : 
    //            FieldInternals::DependentNodeField< 
    //            TypedMFNode< typename DoRoutes< FieldRefs >::node_type >,
    //            FieldRefs >( size ){}
  };
    
  /// \internal
  /// Template specialization to handle the case when we only use FieldRefs.
  ///
  template< class FieldRefs, bool DynamicCast > 
  class DependentMFNode< FieldRefs, void, DynamicCast >: 
    public FieldInternals::DependentNodeField< 
    TypedMFNode< typename FieldInternals::DoRoutes< FieldRefs >::node_type >,
    FieldRefs,
    DynamicCast >  {
  public:
        
    /// Constructor. Default value of value is NULL.
    DependentMFNode(){}

    /// Destructor.
    ~DependentMFNode(){
      this->clear( this->ownerId() );
    }
        
    /// Creates an Dependent with space reserved for size nodes.
    //        DependentMFNode( MFNode::size_type size ) : 
    //            FieldInternals::DependentNodeField< 
    //              TypedMFNode< typename FieldInternals::DoRoutes< FieldRefs >::node_type >,
    //            FieldRefs >( size ){}
        
  };
}

#endif
