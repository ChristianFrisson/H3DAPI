//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file X3DSAX2Handlers.cpp
/// \brief CPP file used to parse X3D files and build nodes with H3DAPI.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DSAX2Handlers.h>

#ifdef HAVE_XERCES
#include <H3D/FieldTemplates.h>
#include <iostream>
#include <sstream>
#include <H3D/H3DNodeDatabase.h>
#include <H3D/MFNode.h>
#include <H3D/Group.h>
#include <H3D/H3DDynamicFieldsObject.h>
#include <H3D/X3DTypeFunctions.h>
#include <H3D/Inline.h>
#include <H3D/H3DExports.h>
#include <H3D/ProfilesAndComponents.h>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

using namespace std;
using namespace H3D;
using namespace X3D;


namespace H3D {
  
  // Temporary solution to be able to print XMLCh *
  ostream& operator<<(ostream& os,const XMLCh* & s) { 
    for( int i = 0; s[i] != 0; i++ ) {
      os << (unsigned char)s[i];
    }
    return os; 
  }
  
  ostream& operator<<(ostream& os,const XMLCh* const & s) { 
    for( int i = 0; s[i] != 0; i++ ) {
      os << (unsigned char)s[i];
    }
    return os; 
  }


  string toString( const XMLCh * const xmls ) {
    unsigned int str_len = XMLString::stringLen( xmls );
    string s( str_len, 'a' );
    for( unsigned int i = 0; i < str_len; i++)
      s[i] = (char)xmls[i];
    return s;
  }
}

namespace X3DSAX2HandlersInternals {
  static const XMLCh gEmptyString[] = { chNull }; 
  static const XMLCh gUSE[] =  { chLatin_U, chLatin_S, chLatin_E, chNull };
  static const XMLCh gX3D[] =  { chLatin_X, chDigit_3, chLatin_D, chNull };
  static const XMLCh gurl[] =  { chLatin_u, chLatin_r, chLatin_l, chNull };
  static const XMLCh gname[] =  
    { chLatin_n, chLatin_a, chLatin_m, chLatin_e, chNull };
  static const XMLCh gvalue[] =  
    { chLatin_v, chLatin_a, chLatin_l, chLatin_u,  chLatin_e, chNull };
  static const XMLCh gScene[] =  
    { chLatin_S, chLatin_c, chLatin_e, chLatin_n, chLatin_e, chNull };
}
using namespace X3DSAX2HandlersInternals;

class FieldValue: public Node {
public:
  FieldValue( const string &n, Field *f ): Node(), field(f ) {
    type_name = "FieldValue";
    name = n;
  }
  Field *field;
};

string X3DSAX2Handlers::getLocationString() {
  ostringstream s;
  s << "(" << locator->getSystemId() << " line " 
    << locator->getLineNumber() << ")";  
  return s.str();
}

void X3DSAX2Handlers::handleProtoInterfaceFieldElement( const Attributes &attrs ) {
  if( proto_declaration ) {
    int nr_attrs = attrs.getLength();
    const XMLCh *field_name  = NULL;
    const XMLCh *field_type = NULL;
    const XMLCh *field_access_type    = NULL;
    const XMLCh *field_value    = NULL;
    
    for( int i = 0; i < nr_attrs; i++ ) {
      string name = toString( attrs.getQName( i ) );
      if( !field_name && name == "name" ) {
        field_name = attrs.getValue( i );
      } else if( !field_type && name == "type" ) {
        field_type = attrs.getValue( i );
      } else if( !field_access_type && name == "accessType" ) {
        field_access_type = attrs.getValue( i );
      } else if( !field_value &&  name == "value" ) {
        field_value = attrs.getValue( i );
      } else {
        Console(3) << "WARNING: Unknown attribute \"" << name  
             << "\" in 'field' element " << getLocationString() << endl;
      }
    }
    
    // All field attributes were not found, so throw exception
    if( !field_name ) {
      string message = 
        "Invalid 'field' specification. Missing \"name\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    } else if ( !field_type ) {
      string message = 
        "Invalid 'field' specification. Missing \"type\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    } else if ( !field_access_type ) {
      string message = 
        "Invalid 'field' specification. Missing \"accessType\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    } 

    string access_type_string = toString( field_access_type );
    Field::AccessType access_type;
    if( access_type_string == "initializeOnly" ) 
      access_type= Field::INITIALIZE_ONLY;
    else if( access_type_string == "outputOnly" )
      access_type = Field::OUTPUT_ONLY;
    else if( access_type_string == "inputOnly" )
      access_type = Field::INPUT_ONLY;
    else if( access_type_string == "inputOutput" )
      access_type = Field::INPUT_OUTPUT;
    else {
      string message = 
        "Invalid value for 'accessType' attribute of 'field' element.";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    }

    X3DTypes::X3DType x3d_type = 
      X3DTypes::stringToType(  toString( field_type ).c_str() );

    if( x3d_type == X3DTypes::UNKNOWN_X3D_TYPE ) {
      Console(3) << "WARNING: Invalid value for \"type\" attribute of \"field\" element "
           << getLocationString() << endl;
    } else {
      string v = "";
      if( field_value && 
          x3d_type != X3DTypes::SFNODE && 
          x3d_type != X3DTypes::MFNODE ) {
        v = toString( field_value );
      }
      proto_declaration->addFieldDeclaration( toString( field_name ),
                                              x3d_type,
                                              access_type,
                                              v );
    }
    FieldValue *fv = new FieldValue( toString( field_name ), NULL );
    node_stack.push( NodeElement( fv ) ); 
  }
}

Field * X3DSAX2Handlers::handleFieldElement( const Attributes &attrs, 
                                             Node* parent ) {
  H3DDynamicFieldsObject *dyn_object = 
    dynamic_cast< H3DDynamicFieldsObject * >( parent );
  if( dyn_object ) {
    int nr_attrs = attrs.getLength();
    const XMLCh *field_name  = NULL;
    const XMLCh *field_type = NULL;
    const XMLCh *field_access_type    = NULL;
    const XMLCh *field_value    = NULL;
    
    for( int i = 0; i < nr_attrs; i++ ) {
      string name = toString( attrs.getQName( i ) );
      if( !field_name && name == "name" ) {
        field_name = attrs.getValue( i );
      } else if( !field_type && name == "type" ) {
        field_type = attrs.getValue( i );
      } else if( !field_access_type &&  name == "accessType" ) {
        field_access_type = attrs.getValue( i );
      } else if( !field_value && name == "value" ) {
        field_value = attrs.getValue( i );
      } else {
        Console(3) << "WARNING: Unknown attribute \"" << name  
             << "\" in 'field' element " << getLocationString() << endl;
      }
    }
    
    // All field attributes were not found, so throw exception
    if( !field_name ) {
      string message = 
        "Invalid 'field' specification. Missing \"name\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    } else if ( !field_type ) {
      string message = 
        "Invalid 'field' specification. Missing \"type\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    } else if ( !field_access_type ) {
      string message = 
        "Invalid 'field' specification. Missing \"accessType\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    } 
    Field *f = X3DTypes::newFieldInstance( toString( field_type ).c_str() );
    
    if( !f ) {
      string message = 
        "Invalid value for 'type' attribute of 'field' specification.";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    }
    
    f->setOwner( parent );
    f->setName( toString( field_name ) );

    string access_type_string = toString( field_access_type );
    Field::AccessType access_type;
    if( access_type_string == "initializeOnly" ) 
      access_type= Field::INITIALIZE_ONLY;
    else if( access_type_string == "outputOnly" )
      access_type = Field::OUTPUT_ONLY;
    else if( access_type_string == "inputOnly" )
      access_type = Field::INPUT_ONLY;
    else if( access_type_string == "inputOutput" )
      access_type = Field::INPUT_OUTPUT;
    else {
      string message = 
        "Invalid value for 'accessType' attribute of 'field' specification.";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    }
    
    if( field_value ) {
      ParsableField *pfield = 
        dynamic_cast< ParsableField * >( f );
      if( !pfield ) {
        stringstream s;
        s << "Cannot parse field \"" << f->getFullName() 
          << "\". Field type must be a subclass of ParsableField "
          << "in order to be parsable. ";
        throw X3D::XMLParseError( s.str(), "", 
                                  toString( locator->getSystemId() ),
                                  locator->getLineNumber() );
      }
      if( access_type == Field::INITIALIZE_ONLY ||
          access_type == Field::INPUT_OUTPUT ) {
        try {
          pfield->setValueFromString( toString( field_value ) ); 
        }
        catch( const Convert::X3DFieldConversionError &e ) {
          stringstream s;
          s << "Could not convert \"" 
            << ( XMLString::stringLen( field_value ) < 100 ? 
                 toString( field_value ): (string)"value" )
            << "\" to " << e.value << " for field \"" 
            << f->getFullName() << "\".";
          throw X3D::XMLParseError( s.str(), "", 
                                    toString( locator->getSystemId() ),
                                    locator->getLineNumber() );
        }
        catch( const Convert::UnimplementedConversionType &e ) {
          stringstream s;
          s << "Field conversion error when converting value for field \"" 
            << f->getFullName() << "\". Conversion for " << e.value 
            << " not implemented";
          throw X3D::XMLParseError( s.str(), "", 
                                    toString( locator->getSystemId() ),
                                    locator->getLineNumber() ); 
        }
      } else {
        Console(3) << "Warning: 'value' attribute ignored. Only used if "
             << "accesstype is initializeOnly or inputOutput " 
             << getLocationString() << endl;
      }
    }
    dyn_object->addField( toString( field_name ),
                          access_type,
                          f );
    FieldValue *fv = new FieldValue( toString( field_name ), f );
    node_stack.push( NodeElement( fv ) ); 
    return f;
  }  else {
    Console(3) << "Warning: 'field' declaration in Node that is does not support"
         << " it " << getLocationString() << endl;
    node_stack.push( NodeElement( NULL ) ); 
  }
  return NULL;
}


void X3DSAX2Handlers::handleRouteElement( const Attributes &attrs, 
                                          bool route_no_event  ) {
  int nr_attrs = attrs.getLength();
  const XMLCh *from_node_name  = NULL;
  const XMLCh *from_field_name = NULL;
  const XMLCh *to_node_name    = NULL;
  const XMLCh *to_field_name   = NULL;
  
  // get all the route specific attributes
  for( int i = 0; i < nr_attrs; i++ ) {
    string name = toString( attrs.getQName( i ) );
    if( !from_node_name && name == "fromNode" ) {
      from_node_name = attrs.getValue( i );
    } else if( !to_node_name && name == "toNode" ) {
      to_node_name = attrs.getValue( i );
    } else if( !from_field_name && name == "fromField" ) {
      from_field_name = attrs.getValue( i );
    } else if( !to_field_name && name == "toField" ) {
      to_field_name = attrs.getValue( i );
    } else {
      Console(3) << "WARNING: Unknown attribute \"" << name  
           << "\" in ROUTE element " << getLocationString() << endl;
    }
  }
            
  // All route attributes were not found, so whrow exception
  if( !from_node_name ) {
    string message = "Invalid ROUTE specification. Missing \"fromNode\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber()  );
  } else if ( !from_field_name ) {
    string message = "Invalid ROUTE specification. Missing \"fromField\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber() );
  } else if ( !to_node_name ) {
    string message = "Invalid ROUTE specification. Missing \"toNode\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber() );
  }
  else if ( !to_field_name ) {
    string message = "Invalid ROUTE specification. Missing \"toField\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber() );
  } else {
    // Lookup the nodes and fields and set up the route.
    Node *from_node = DEF_map->getNode( toString( from_node_name ) );
    if( from_node ) {
      Field *from_field = 
        from_node->getField( toString( from_field_name ).c_str() );
      if( from_field ) {
        Node *to_node = DEF_map->getNode( toString( to_node_name ) );
        if( to_node ) {
          Field *to_field = 
            to_node->getField( toString( to_field_name ).c_str() );
          if( to_field ) {
            if( !route_no_event )
              from_field->route( to_field );
            else
              from_field->routeNoEvent( to_field );
          } else {
            Console(3) << "WARNING: Route error. Could not find field named \"" 
                 << to_field_name
                 << "\" in \"" << to_node_name << "\" Node " 
                       << getLocationString() << endl;
          }
        } else {
          Console(3) << "WARNING: Route error. Could not find Node named \"" 
               << to_node_name
               << "\" specified in \"toNode\" attribute " 
               << getLocationString() << endl;
        }
      } else {
        Console(3) << "WARNING: Route error. Could not find field named \"" 
             << from_field_name
             << "\" in \"" << from_node_name << "\" Node " 
             << getLocationString() << endl;
      }
    } else {
      Console(3) << "WARNING: Route error. Could not find Node named \"" 
           << from_node_name
           << "\" specified in \"fromNode\" attribute " 
           << getLocationString() << endl;
    }
  }
  // push NULL on the node stack to skip elements within ROUTE element.
  node_stack.push( NodeElement( NULL ) );
}


void X3DSAX2Handlers::handleImportElement( const Attributes &attrs  ) {
  int nr_attrs = attrs.getLength();
  const XMLCh *inline_def_name  = NULL;
  const XMLCh *exported_def_name = NULL;
  const XMLCh *as_name    = NULL;
  
  // get all the route specific attributes
  for( int i = 0; i < nr_attrs; i++ ) {
    string name = toString( attrs.getQName( i ) );
    if( !inline_def_name && name == "inlineDEF" ) {
      inline_def_name = attrs.getValue( i );
    } else if( !exported_def_name &&  name == "exportedDEF" ) {
      exported_def_name = attrs.getValue( i );
    } else if( !as_name && name == "AS" ) {
      as_name = attrs.getValue( i );
    } else {
      Console(3) << "WARNING: Unknown attribute \"" << name  
           << "\" in IMPORT element " << getLocationString() << endl;
    }
  }
            
  // All route attributes were not found, so whrow exception
  if( !inline_def_name ) {
    string message = "Invalid IMPORT specification. Missing \"inlineDEF\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber()  );
  } else if ( !exported_def_name ) {
    string message = "Invalid IMPORT specification. Missing \"exportedDEF\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber() );
  } else if ( !as_name ) {
    string message = "Invalid IMPORT specification. Missing \"AS\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber() );
  } else {
    // Lookup the nodes and fields and set up the route.
    const string &inline_def_string = toString( inline_def_name );

    if( inline_def_string == "H3D_EXPORTS" ) {
      Node *import_node = 
        H3DExports::getH3DExportNode( toString( exported_def_name ) );
      if( import_node ) {
        DEF_map->addNode( toString( as_name ), 
                          import_node ); 
      } else {
        Console(3) << "WARNING: IMPORT error. H3D_EXPORTS " 
             << "does not include \""
             << exported_def_name << "\"" 
             << getLocationString() << endl;
      }
    } else {
      Node *n = DEF_map->getNode( inline_def_string );
      Inline *inline_node = dynamic_cast< Inline * >( n );
      if( inline_node ) {
        Node *import_node = 
          inline_node->exported_nodes.getNode( toString( exported_def_name ) );
        if( import_node ) {
          DEF_map->addNode( toString( as_name ), 
                            import_node ); 
        } else {
          Console(3) << "WARNING: IMPORT error. Inline node \"" 
               << inline_def_name << "\" does not EXPORT \""
               << exported_def_name << "\"" 
               << getLocationString() << endl;
        }
      } else {
        if( n ) {
          Console(3) << "WARNING: IMPORT error. Node \"" 
               << inline_def_name << "\" is not an Inline node "
               << getLocationString() << endl;
        } else {
          Console(3) << "WARNING: IMPORT error. Node named \"" 
               << inline_def_name << "\" does not exist."
               << getLocationString() << endl;
        }
      }
    }
  }
  // push NULL on the node stack to skip elements within IMPORT element.
  node_stack.push( NodeElement( NULL ) );
}

void X3DSAX2Handlers::handleExportElement( const Attributes &attrs  ) {
  int nr_attrs = attrs.getLength();
  const XMLCh *local_def_name  = NULL;
  const XMLCh *as_name    = NULL;
  
  // get all the route specific attributes
  for( int i = 0; i < nr_attrs; i++ ) {
    string name = toString( attrs.getQName( i ) );
    if( !local_def_name && name == "localDEF" ) {
      local_def_name = attrs.getValue( i );
    } else if( !as_name &&  name == "AS" ) {
      as_name = attrs.getValue( i );
    } else {
      Console(3) << "WARNING: Unknown attribute \"" << name  
           << "\" in EXPORT element " << getLocationString() << endl;
    }
  }
            
  if( !local_def_name ) {
    string message = "Invalid EXPORT specification. Missing \"localDEF\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber()  );
  } else if ( !as_name ) {
    string message = "Invalid IMPORT specification. Missing \"AS\" attribute";
    throw X3D::XMLParseError( message, "", 
                              toString( locator->getSystemId() ),
                              locator->getLineNumber() );
  } else {
    // Lookup the nodes and fields and set up the route.
    Node *n = DEF_map->getNode( toString( local_def_name ) );
    if( n ) {
      exported_nodes->addNode( toString( as_name ), 
                               n ); 
    } else {
      Console(3) << "WARNING: EXPORT error. Node named \"" 
           << local_def_name << "\" does not exist."
           << getLocationString() << endl;
    }
  }
  // push NULL on the node stack to skip elements within EXPORT element.
  node_stack.push( NodeElement( NULL ) );
}

void X3DSAX2Handlers::handleConnectElement( const Attributes &attrs, 
                                            Node *parent ) {
  // push NULL on the node stack to skip elements within connect element.
  node_stack.push( NodeElement( NULL ) );

  if( !defining_proto_connections ) {
    Console(3) << "Warning: \"connect\" element can only be defined in a IS element "
         << getLocationString() << endl;
  } else if( proto_instance ) {
  
    int nr_attrs = attrs.getLength();
    const XMLCh *node_field_name  = NULL;
    const XMLCh *proto_field_name    = NULL;
    
    // get all the route specific attributes
    for( int i = 0; i < nr_attrs; i++ ) {
      string name = toString( attrs.getQName( i ) );
      if( !node_field_name && name == "nodeField" ) {
        node_field_name = attrs.getValue( i );
      } else if( !proto_field_name &&  name =="protoField" ) {
        proto_field_name = attrs.getValue( i );
      } else {
        Console(3) << "WARNING: Unknown attribute \"" << name  
             << "\" in EXPORT element " << getLocationString() << endl;
      }
    }
    
    if( !node_field_name ) {
      string message = "Invalid \"connect\" specification. Missing \"nodeField\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    } else if ( !proto_field_name ) {
      string message = "Invalid \"connect\" specification. Missing \"protoField\" attribute";
      throw X3D::XMLParseError( message, "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber() );
    } else {
      
      // Lookup the nodes and fields and set up the route.
      Field *node_field = parent->getField( toString( node_field_name ) ); 
      Field *proto_field = 
        proto_instance->getField( toString( proto_field_name ) );
      
      if( !node_field ) {
        Console(3) << "WARNING: Could not find field \"" << node_field_name  
             << "\" in parent node" << getLocationString() << endl;
        return;
      }
      
      if( !proto_field ) {
        Console(3) << "WARNING: Could not find field \"" << proto_field_name  
             << "\" in ProtoInterface of PROTO node" 
             << getLocationString() << endl;
        return;
      }
      
      if( node_field->getAccessType() != proto_field->getAccessType() ) {
        Console(3) << "WARNING: accessType of \"nodeField\" and \"protoField\" does not match"
             << getLocationString() << endl;
        return;
      }
      
      if( node_field->getX3DType() != proto_field->getX3DType() ) {
        Console(3) << "WARNING: type of \"nodeField\" and \"protoField\" does not match"
             << getLocationString() << endl;
        return;
      }
      
      Field::AccessType access_type = node_field->getAccessType();
      
      if( access_type == Field::OUTPUT_ONLY ) {
        node_field->route( proto_field, proto_instance->id );
      } else if ( access_type == Field::INPUT_OUTPUT ) {
        node_field->routeNoEvent( proto_field, proto_instance->id );
      }
      
      if( access_type != Field::OUTPUT_ONLY ) {
        proto_field->route( node_field, proto_instance->id );
      }
    }
  }
}

void X3DSAX2Handlers::protoStartElement( const XMLCh* const uri,
                                         const XMLCh* const localname, 
                                         const XMLCh* const qname,
                                         const Attributes& attrs ) {
  Node *parent = NULL;
  if( node_stack.size() > 0 ) parent = node_stack.top().getNode();  

  if( !parent ) {
    node_stack.push( NodeElement( NULL ) );
    return;
  }

  string localname_string = toString( localname );

  if( localname_string == "ProtoDeclare" ) {
    if( proto_declaration ) {
      Console(3) << "Warning: ProtoDeclare element not allowed inside other ProtoDeclare element"
           << getLocationString() << endl;
      node_stack.push( NodeElement( NULL ) );
    } else {
      const XMLCh *name = attrs.getValue( gname );
      if( !name ) {
        Console(3) << "Warning: ProtoDeclare element is missing required \"name\" attibute "
             << getLocationString() << endl;
        node_stack.push( NodeElement( NULL ) );
      } else {
        proto_declaration = new ProtoDeclaration( toString( name ) );
      }
    }
  } else  if( localname_string == "ExternProtoDeclare" ) {
    handleExternProtoDeclareElement( attrs );
  } else if( localname_string == "ProtoInterface" ) {
    if( !proto_declaration || defining_proto_interface || 
        defining_proto_body || defining_extern_proto ) {
      Console(3) << "Warning: ProtoInterface element can only be a child element of ProtoDeclare element "
           << getLocationString() << endl;
      node_stack.push( NodeElement( NULL ) );
    } else {
      defining_proto_interface = true;
    }
  } else if( localname_string == "ProtoBody" ) {
    if( !proto_declaration || defining_proto_interface || 
        defining_proto_body || defining_extern_proto ||
        proto_declaration->getProtoBody() != "" ) {
      Console(3) << "Warning: ProtoBody element can only be a child element of ProtoDeclare element "
           << getLocationString() << endl;
      node_stack.push( NodeElement( NULL ) );
    } else {
      defining_proto_body = true;
    }
  } else if( proto_declaration && !defining_proto_interface && 
             !defining_proto_body && !defining_extern_proto ) {
    // Element in ProtoDeclare proto declare that is not ProtoInterface 
    // or ProtoBody.
    Console(3) << "Warning: Only ProtoBody and ProtoInterface elements allowed in ProtoDeclare element "
         << getLocationString() << endl;
    node_stack.push( NodeElement( NULL ) );
  } else {
    if( defining_proto_interface || defining_extern_proto ) { 
      if( localname_string == "field" ) {
        handleProtoInterfaceFieldElement( attrs );
      } else if( FieldValue *fv = dynamic_cast< FieldValue * >( parent ) ) {
        const string &name = fv->getName();
        ProtoDeclaration::FieldDeclaration *fd = 
          proto_declaration->getFieldDeclaration( name );
        if( fd ) {stringstream s;
        s << "<" << localname << " ";
        for( unsigned int i = 0; i < attrs.getLength(); i++ ) {
          s << attrs.getQName( i ) << "=\"" << attrs.getValue( i ) << "\" ";
        }
        s << ">" << endl;
        fd->value = fd->value + s.str();
        }
      } else {
        Console(3) << "Warning: Only \"field\" elements allowed in ProtoInstance element "
             << getLocationString() << endl;
        node_stack.push( NodeElement( NULL ) );
      }
    } else if( defining_proto_body ) {
      if( defining_proto_connections ) {
        Console(3) << "Warning: Only \"connect\" elements allowed in IS element "
             << getLocationString() << endl;
        node_stack.push( NodeElement( NULL ) );
      } else {
        stringstream s;
        s << "<" << localname << " ";
        for( unsigned int i = 0; i < attrs.getLength(); i++ ) {
          // make sure that MFString are correct. All attribute values
          // are enclosed in "..", unless the string itself starts with "
          // then they are inclosed in '..'
          const XMLCh *v = attrs.getValue( i );
          char quote = '"';
          if( v ) {
            unsigned int ci = 0;
            while( v[ci] != '\0' && isspace( v[ci] ) ) ci++;
            if( v[ci] == '"' ) quote = '\'';
          }
          s << attrs.getQName( i ) << "=" << quote << v << quote << " ";
        }
        s << ">" << endl;
        proto_declaration->setProtoBody( proto_declaration->getProtoBody() + 
                                         s.str() );
      } 
	
    }
  }
}

void X3DSAX2Handlers::protoEndElement( const XMLCh* const uri,
                                       const XMLCh* const localname, 
                                       const XMLCh* const qname ) {
  AutoRef< Node > top( node_stack.top().getNode() );  
  if( !top.get() ) {
    node_stack.pop();
    return;
  }

  string localname_string = toString( localname );

  if( localname_string == "ProtoDeclare" ) {
    if( proto_declaration ) {
      proto_declarations->push_back( proto_declaration );
      if( !proto_declarations->getFirstProtoDeclaration() ) {
        proto_declarations->setFirstProtoDeclaration( proto_declaration );
      }
      proto_declaration = NULL;
    }
  } else if( localname_string == "ExternProtoDeclare" ) {
    if( proto_declaration ) {
      proto_declarations->push_back( proto_declaration );
      proto_declaration = NULL;
    }
    defining_extern_proto = false;
  } else if( localname_string == "ProtoInterface" ) {
    defining_proto_interface = false;
  } else if( localname_string == "ProtoBody" ) {
    defining_proto_body = false;
  } else {
    if( defining_proto_body ) { 
      stringstream s;
      s << "</" << localname << ">" << endl;
      proto_declaration->setProtoBody( proto_declaration->getProtoBody() + 
                                       s.str() );
    } else if( defining_proto_interface || defining_extern_proto ) { 
      FieldValue *fv = dynamic_cast< FieldValue * >( top.get() );
      if( fv ) {
        if( localname_string == "field" ) {
          node_stack.pop();
        } else {
          const string &name = fv->getName();
          ProtoDeclaration::FieldDeclaration *fd = 
            proto_declaration->getFieldDeclaration( name );
          if( fd ) {
            stringstream s;
            s << "</" << localname << ">" << endl;
            fd->value = fd->value + s.str();
          }
        }
      }
    }
  }
}

X3DPrototypeInstance* X3DSAX2Handlers::handleProtoInstanceElement( const Attributes &attrs ) {
  const XMLCh *name = attrs.getValue( gname );
  if( name ) {
    string s_name = toString( name );
    ProtoDeclaration *proto = 
      proto_declarations->getProtoDeclaration( s_name );
    if( proto ) return proto->newProtoInstance();
    else {
      Console(3) << "Warning: Could not find PROTO declaration for \"" << s_name  
           << "\" instance (" << toString( locator->getSystemId() )
           << ", line " << locator->getLineNumber() << ")" << endl;
    }
  } else {
    Console(3) << "Warning: ProtoInstance element is missing required \"name\" attibute ("
         << toString( locator->getSystemId() )




         << ", line " << locator->getLineNumber() << ")" << endl;
  }
  return NULL;
}

void X3DSAX2Handlers::handleExternProtoDeclareElement( const Attributes &attrs ) {
  if( proto_declaration ) {
    Console(3) << "Warning: ExternProtoDeclare element not allowed inside other ProtoDeclare element"
         << getLocationString() << endl;
    node_stack.push( NodeElement( NULL ) );
  } else {
    const XMLCh *name = attrs.getValue( gname );
    const XMLCh *url = attrs.getValue( gurl );
    if( !name ) {
      Console(3) << "Warning: ExternProtoDeclare element is missing required \"name\" attibute "
           << getLocationString() << endl;
      node_stack.push( NodeElement( NULL ) );
    } else if( !url ) {
      Console(3) << "Warning: ExternProtoDeclare element is missing required \"url\" attibute "
           << getLocationString() << endl;
      node_stack.push( NodeElement( NULL ) );
    } else {
      vector< string > urls;
      try {
        X3D::X3DStringToVector( toString( url ), urls ); 
      } catch( const Convert::X3DFieldConversionError &e ) {
        stringstream s;
        s << "Could not convert \"" 
          << ( XMLString::stringLen( url ) < 100 ?
               toString( url ): (string)"value" )
          << "\" to " << e.value << " for attribute \"" 
          << url << "\".";
        throw X3D::XMLParseError( s.str(), "", 
                                  toString( locator->getSystemId() ),
                                  locator->getLineNumber() );
      }
      PrototypeVector proto_vector;
      string url_used = "";
      string proto_name = "";

      for( vector<string>::iterator i = urls.begin(); i != urls.end(); i++ ) {
        proto_vector.clear();
        string base_url;
        string::size_type pos = (*i).find_last_of( "#" );
        if( pos == string::npos ) {
          base_url = *i;
        } else {
          base_url = (*i).substr( 0, pos );
        }

        try {
          X3D::createX3DNodeFromURL( base_url, NULL, NULL, &proto_vector );
        } catch( const Exception::H3DException & ) {
          continue;
        }
        if( pos != string::npos )
          proto_name = (*i).substr( pos + 1, (*i).size() - pos );
        url_used = *i;
        break;
      }
      if( url_used == "" ) {
        Console(3) << "Warning: None of the urls in ExternProtoDeclare element could "
             << "be sucessfully loaded. "<< getLocationString()<< endl;
        node_stack.push( NodeElement( NULL ) );
        return;
      }

      ProtoDeclaration *pd = NULL;
      string proto_body;

      if( proto_name == "" ) {
        // get the first prototype declaration from the url that was read.
        pd = proto_vector.getFirstProtoDeclaration();
        if( !pd ) {
          Console(3) << "Warning: Could not find any PROTO declaration "
               << getLocationString()<< endl;
        }
      } else {
        pd = proto_vector.getProtoDeclaration( proto_name );
        if( !pd ) {
          Console(3) << "Warning: Could not find PROTO declaration \"" 
               << proto_name
               << "\" " << getLocationString()<< endl;
        }
      }
      
      if( pd ) {
        proto_body = pd->getProtoBody();
      }
      
      proto_declaration = new ProtoDeclaration( toString( name ), proto_body );
      
      defining_extern_proto = true;
    }
  }
}

void X3DSAX2Handlers::handleFieldValueElement( const Attributes &attrs, 
                                               Node *parent ) {
  const XMLCh *name = attrs.getValue( gname );
  const XMLCh *value = attrs.getValue( gvalue );
  node_stack.push( NodeElement( NULL ) ); 
  
  X3DPrototypeInstance *pi = dynamic_cast< X3DPrototypeInstance * >( parent );

  if( !pi ) {
    Console(3) << "Warning: fieldValue element can only appear inside ProtoInstance "
         << "element (" << toString( locator->getSystemId() )
         << ", line " << locator->getLineNumber() << ")" << endl;
    return;
  }

  if( name && value ) {
    Field *field = pi->database.getField( parent, toString( name ) );
    if( !field ) {
      Console(3) << "Warning: Field named \"" << name 
           << "\" does not exist in ProtoInstance node (" 
           << toString( locator->getSystemId() )
           << ", line " << locator->getLineNumber() << ")" << endl;
      return;
    }
    ParsableField *pfield = 
      dynamic_cast< ParsableField * >( field );
    if( !pfield ) {
      Console(3) << "Warning: Cannot parse field \"" << name 
           << "\". Field type must be a subclass of ParsableField "
           << "in order to be parsable (" << toString( locator->getSystemId() )
           << ", line " << locator->getLineNumber() << ")" << endl;
      return;
    } else {
      try {
        pfield->setValueFromString( toString( value ) ); 
        return;
      }
      catch( const Convert::X3DFieldConversionError &e ) {
        stringstream s;
        s << "Could not convert \"" 
          << ( XMLString::stringLen( value ) < 100 ?
               toString( value ): (string)"value" )
          << "\" to " << e.value << " for field \"" 
          << name << "\".";
        throw X3D::XMLParseError( s.str(), "", 
                                  toString( locator->getSystemId() ),
                                  locator->getLineNumber() );
      }
      catch( const Convert::UnimplementedConversionType &e ) {
        stringstream s;
        s << "Field conversion error when converting value for field \"" 
          << name << "\". Conversion for " << e.value 
          << " not implemented";
        throw X3D::XMLParseError( s.str(), "", 
                                  toString( locator->getSystemId() ),
                                  locator->getLineNumber() ); 
      } 
    }
  } else {
    Console(3) << "Warning: Both \"name\" and \"value\" attributes required in fieldValue element" 
         << getLocationString() << endl;
    return;
  }
}

// X3DSAX2Handlers: Implementation of the SAX DocumentHandler interface
void X3DSAX2Handlers::startElement(const XMLCh* const uri,
                                   const XMLCh* const localname, 
                                   const XMLCh* const qname,
                                   const Attributes& attrs) {
  // we are inside a ProtoDeclare element, so use protoStartElement 
  // instead of the normal  element handling.
  if( proto_declaration ) {
    protoStartElement( uri, localname, qname, attrs );
    return;
  }

  string localname_string = toString( localname );

  Node *new_node = NULL;
  Node *parent = NULL;

  // skip special element X3D
  if( XMLString::equals( localname, gX3D ) ) {
    if( node_stack.size() > 0 ) 
      throw X3D::XMLParseError( "X3D element only allowed at toplevel.", "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    else {
      //cerr << "localname " << localname << endl;
      int nr_attrs = attrs.getLength();
      string profile_name;
      string version;
      for( int i = 0; i < nr_attrs; i++ ) {
        string name = toString( attrs.getQName( i ) );
        if( name == "profile" ) {
          profile_name = toString( attrs.getValue(i) );
          //cerr << name << " " << attrs.getValue(i) << endl;

        }
        else if( name == "version" ) {
          version = toString( attrs.getValue( i ) );
        }
      }

      if( !profile_set ) {
        string tmp_err_msg;
        if( ProfilesAndComponents::setProfile( profile_name,
          tmp_err_msg,
          version ) ) {
            profile_set = true;
        }
        else {
          throw X3D::XMLParseError( tmp_err_msg, "", 
            toString( locator->getSystemId() ),
            locator->getLineNumber()  );
        }
      }
      return;
    }
  }

  // special element Scene works as a Group node so we use a Group node.
  if( XMLString::equals( localname, gScene ) ) {
    if( node_stack.size() > 0 ) 
      throw X3D::XMLParseError( "Scene element only allowed at toplevel or in X3D element.", "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    else {
      if( profile_set ) {
        ProfilesAndComponents::setMainProfileDone( profile_set );
      }
      node_stack.push( NodeElement( new Group ) );
      return;
    }
  }
  
  // get the parent node.
  if( node_stack.size() > 0 ) parent = node_stack.top().getNode();  

  if( !parent && node_stack.size() > 0 ) {
    // a NULL node is our parent, which means that we should skip all new 
    // elements. 
    node_stack.push( NodeElement( NULL ) );
  } else {
    // handle special cases for special parent node

    X3DPrototypeInstance *pi = 
      dynamic_cast< X3DPrototypeInstance * >( parent );
    if( pi ) {
      // parent is X3DPrototypeInstance that was created with a ProtoInstance element.
      // Only fieldValue elements are allowed.
      if( localname_string !=  "fieldValue" ) {
        Console(3) << "WARNING: Only fieldValue elements allowed in ProtoInstance element "
             << getLocationString() << endl;
        node_stack.push( NodeElement( NULL ) );
      } else {
        handleFieldValueElement( attrs, parent );
      }
    } else {
      if( localname_string == "fieldValue" ) {
        handleFieldValueElement( attrs, parent );
      } else if( localname_string == "ProtoDeclare" ||
                 localname_string == "ExternProtoDeclare" ) {
        protoStartElement( uri, localname, qname, attrs );
        return;
      } else if( localname_string == "ProtoInterface" ) {
        Console(3) << "WARNING: ProtoInterface elements only allowed inside ProtoDeclare elements"
             << getLocationString() << endl;
        node_stack.push( NodeElement( NULL ) );
      } else if( localname_string == "ProtoBody" ) {
        Console(3) << "WARNING: ProtoBody elements only allowed inside ProtoDeclare elements"
             << getLocationString() << endl;
        node_stack.push( NodeElement( NULL ) );
      } else if( localname_string == "IS" )  {
        if( !proto_instance ) {
          Console(3) << "WARNING: IS elements only allowed inside ProtoBody elements"
               << getLocationString() << endl;
          node_stack.push( NodeElement( NULL ) );
        } else {
          defining_proto_connections = true;
        }
      }	else if( localname_string == "connect"  ) {
        handleConnectElement( attrs, parent );
      } else if( localname_string == "ROUTE" ) {
        handleRouteElement( attrs, false );
      } else if( localname_string == "ROUTE_NO_EVENT" ) {
        handleRouteElement( attrs, true );
      } else if( localname_string == "IMPORT" ) {
        handleImportElement( attrs );
      } else if( localname_string == "EXPORT" ) {
        handleExportElement( attrs );
      } else if( localname_string == "field" ) {
        handleFieldElement( attrs, parent );
      } else if( localname_string == "head" ) {
        inside_head = true;
        //node_stack.push( NodeElement( NULL ) );
      } else if( localname_string == "component" ) {
        if( !inside_head || !profile_set || meta_set ) {
          throw X3D::XMLParseError(
            ( string( "component element only allowed inside head element" ) +
              string( " before any meta element." ) ).c_str(),
            "",
            toString( locator->getSystemId() ),
            locator->getLineNumber()  );
        }
        else {
          int nr_attrs = attrs.getLength();
          string component_name;
          int level = 0;
          for( int i = 0; i < nr_attrs; i++ ) {
            string name = toString( attrs.getQName( i ) );
            if( name == "name" ) {
              component_name = toString( attrs.getValue(i) );
              //cerr << name << " " << attrs.getValue(i) << endl;
            }
            else if( name == "level" ) {
              level = atoi( (char * )attrs.getValue(i) );
            }
          }

          string tmp_err_msg;
          if( !ProfilesAndComponents::addComponent( component_name, level,
            tmp_err_msg ) ) {
              throw X3D::XMLParseError( tmp_err_msg, "", 
                toString( locator->getSystemId() ),
                locator->getLineNumber()  );
          }
        }
      } else if( localname_string == "meta" ) {
        if( !inside_head || !profile_set ) {
          throw X3D::XMLParseError(
            ( string("meta element only allowed in head element immediately " )
              + string( "after any eventual component definitions are set." )
              ).c_str(),
            "",
            toString( locator->getSystemId() ),
            locator->getLineNumber()  );
        }
        else {
          meta_set = true;
          //handleComponentAttributes( attrs );
        }
      } else {
        const XMLCh *use_name = attrs.getValue( gUSE );
        bool proto_instance = false;

        if( localname_string == "ProtoInstance" ) {
          // if a ProtoInstance create a new instance of the prototype.
          new_node = handleProtoInstanceElement( attrs );
          proto_instance = true;
        } else if( use_name ) {
          // if we have a USE attribute, lookup the matching node and use that.
          new_node = DEF_map->getNode( toString( use_name ) );
          if( !new_node ) {
            stringstream s;
            s << "Invalid USE attribute. " 
              << "No node with DEF name \"" << use_name 
              << "\" defined.";
            throw X3D::XMLParseError( s.str(), "", 
                                      toString( locator->getSystemId() ),
                                      locator->getLineNumber() );
          }
        } else {
          // the element is a node declaration so create a new Node.
          try {
            new_node = 
              H3DNodeDatabase::createNode( localname_string );
            if( !new_node ) {
              Console(3) << "WARNING: Could not create \"" << localname 
                   << "\" node. It does not exist in the H3DNodeDatabase " 
                   << getLocationString() << endl;
            } else {
              // remove the initialization that occurs on first reference
              // since we don't want it to occur until all child nodes
              // have been created and set.
              new_node->setManualInitialize( true );
            }
          } catch (  const H3D::Exception::H3DException &e ) {
            Console(3) << "WARNING: Could not create \"" << localname 
                 << "\" node. Exception in constructor: "
                 << e << " " << getLocationString() << endl;
          }
        }
        string container_field = new_node ?
          new_node->defaultXMLContainerField():(string)"" ;

        if( new_node ) {
          int nr_attrs = attrs.getLength();
          for( int i = 0; i < nr_attrs; i++ ) {
            string name = toString( attrs.getQName( i ) );
	    
            if( name == "USE" ) {
              // this case has already been specially handled above, 
              // so we just ignore it
            } else if( name == "class" ) {
              // class is a reserved attribute name 
            } else if( name == "DEF" ) {
              const XMLCh *def_name = attrs.getValue( i );
              if( new_node && DEF_map ) {
                DEF_map->addNode( toString( def_name ), 
                                  new_node ); 
                new_node->setName( toString( def_name ) );
              }
            } else if( name == "containerField" ) {
              string s = toString( attrs.getValue( i ) );
              container_field = s; 
            } else if( proto_instance ) {
            } else if( use_name ) {
              // node is defined by USE, so we ignore all fields that are not
              // USE, class, DEF, or containerField
            } else {
              Field *field = new_node->getField( name );
              if( !field ) {
                Console(3) << "WARNING: Couldn't find field named \"" << name 
                           << "\" in " << qname << " node " 
                           << getLocationString() << endl;
              } else {
                ParsableField *pfield = 
                  dynamic_cast< ParsableField * >( field );
                if( !pfield ) {
                  stringstream s;
                  s << "Cannot parse field \"" << name 
                    << "\". Field type must be a subclass of ParsableField "
                    << "in order to be parsable. ";
                  throw X3D::XMLParseError( s.str(), "", 
                                            toString( locator->getSystemId() ),
                                            locator->getLineNumber() );
                } else {
                  const XMLCh *attr_value = attrs.getValue( i );
                  // the standard DTD spec for X3D uses "" as default fixed
                  // values for some fields. Therefore we skip all attributes
                  // which values are "". This will unfortunately also mean that
                  // we allow users to specify "" as values for the attributes
                  // without throwing an error.
                  if( XMLString::equals( attr_value, gEmptyString ) )
                    continue;
		  
                  try {
                    pfield->setValueFromString( toString( attr_value ) ); 
                  }
                  catch( const Convert::X3DFieldConversionError &e ) {
                    stringstream s;
                    s << "Could not convert \"" 
                      << ( XMLString::stringLen( attr_value ) < 100 ?
                           toString( attr_value ): (string)"value" ) 
                      << "\" to " << e.value << " for field \"" 
                      << name << "\".";
                    throw X3D::XMLParseError( s.str(), "", 
                                              toString( locator->getSystemId() ),
                                              locator->getLineNumber() );
                  }
                  catch( const Convert::UnimplementedConversionType &e ) {
                    stringstream s;
                    s << "Field conversion error when converting value for field \"" 
                      << name << "\". Conversion for " << e.value 
                      << " not implemented";
                    throw X3D::XMLParseError( s.str(), "", 
                                              toString( locator->getSystemId() ),
                                              locator->getLineNumber() ); 
                  } 
                }
              }
            }
          }
          if( parent && !dynamic_cast< FieldValue * >( parent ) ) {
            Field *f = parent->getField( container_field.c_str() );
            if( !f ) {
              stringstream s;
              s << "Invalid containerField attribute \"" 
                <<  container_field 
                << "\". The field does not exist in parent node (" 
                << parent->getName() << ")";
              throw X3D::XMLParseError( s.str(), "", 
                                        toString( locator->getSystemId() ),
                                        locator->getLineNumber() ); 
            } else {
              if( !dynamic_cast< SFNode *>( f ) && 
                  !dynamic_cast< MFNode *>( f ) ) {
                stringstream s;
                s << "Invalid containerField attribute \"" 
                  <<  container_field
                  << "\". Field is not of type SFNode or MFNode.";
                throw X3D::XMLParseError( s.str(), "", 
                                          toString( locator->getSystemId() ),
                                          locator->getLineNumber() ); 
              }
            }
          }
        }
        NodeElement ne= NodeElement( new_node,
                                     container_field );
        node_stack.push( ne );
      }
    }
  }
}



void X3DSAX2Handlers::endElement (const XMLCh *const uri,
                                  const XMLCh *const localname, 
                                  const XMLCh *const qname) {
  if( proto_declaration ) {
    protoEndElement( uri, localname, qname );
    return;
  }

  string localname_string = toString( localname );

  // skip special element X3D.
  if( localname_string == "X3D" ) {
    profile_set = false;
    return;
  }

  if( localname_string == "head" ) {
    inside_head = false;
    return;
  }

  if( localname_string == "component"
      || localname_string == "meta" )
    return;
  

  if( localname_string == "IS" && defining_proto_connections ) {
    defining_proto_connections = false;
    return;
  }
  
  NodeElement new_node_element = node_stack.top();
  Node *new_node = new_node_element.getNode();

  node_stack.pop();

  Node *parent = NULL;
  if( node_stack.size() > 0 ) 
    parent = node_stack.top().getNode();

  if( dynamic_cast< FieldValue * > ( new_node ) ) return;

  // we have a node type
  if( new_node ) {
    // intialize the node if it is not already initialized
    if( !new_node->isInitialized() && new_node->getManualInitialize() ) 
      new_node->initialize();
    if( parent ) {
      FieldValue *fv = dynamic_cast< FieldValue * >( parent );
      if( fv ) {
        Field::AccessType access_type = fv->field->getAccessType();
        if( access_type == Field::INITIALIZE_ONLY ||
            access_type == Field::INPUT_OUTPUT ) {
          SFNode *sf = dynamic_cast< SFNode *>( fv->field );
          if( sf ) { 
            sf->setValue( new_node );
          } else {
            MFNode *mf = dynamic_cast< MFNode *>( fv->field );
            if( mf ) {
              mf->push_back( new_node );
            }
            else Console(3) << "Warning: 'value' element ignored. Only used if "
                      << "field type is SFNode or MFNode" 
                      << getLocationString() << endl;
          }
        } else {
          Console(3) << "Warning: 'value' element ignored. Only used if "
               << "accesstype is initializeOnly or inputOutput " 
               << getLocationString() << endl;
        }
      } else {
        Field *cf = 
          parent->getField( new_node_element.getContainerField().c_str() );
        SFNode *sfnode = dynamic_cast< SFNode * >( cf );
        if( sfnode ) {
          sfnode->setValue( new_node );
        } else {
          // we have already checked that the container field is 
          // of type SFNode or MFNode in startElement, so it
          // must be of type MFNode here
          MFNode *mfnode = static_cast< MFNode * >( cf );
          mfnode->push_back( new_node );
        }
      }
    } else {
      if( node_stack.empty() ) {
        // this is the topmost node, so it is the final node  
        resulting_node.reset( new_node );
      }
    }
  }
}
 
//  X3DSAX2Handlers: Overrides of the SAX ErrorHandler interface
void X3DSAX2Handlers::error(const SAXParseException& e) {
  stringstream s;
  s << e.getMessage() << " (" << e.getSystemId()
    << ", line " << e.getLineNumber()
    << ", char " << e.getColumnNumber() << ")";
  throw X3D::XMLParseError( s.str());

}

void X3DSAX2Handlers::fatalError(const SAXParseException& e) {
  stringstream s;
  s << e.getMessage();
  if( e.getSystemId() && e.getSystemId()[0] != '\0' ) {
    s << " (" << e.getSystemId()
      << ", line " << e.getLineNumber()
      << ", char " << e.getColumnNumber() << ")";
  } 
  throw X3D::XMLParseError( s.str() );
}

void X3DSAX2Handlers::warning(const SAXParseException& e) {
  Console(3) << "Warning: " << e.getMessage() << " (" << e.getSystemId()
             << ", line " << e.getLineNumber()
             << ", char " << e.getColumnNumber() << ")" << endl;
}


#endif
