//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include "X3DSAX2Handlers.h"
#include "FieldTemplates.h"
#include <iostream>
#include <sstream>
#include <H3DNodeDatabase.h>
#include "MFNode.h"
#include "Group.h"
#include "H3DDynamicFieldsObject.h"
#include "X3DTypeFunctions.h"


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
    stringstream t;
    t << xmls;
    t << ends;
    // Possible bug in stringstream's str() function, at least on Apple;
    // returned string fails compare() calls that should succeeed.
    // Temporary solution: convert all the way to a char* and back to a string
    return t.str().c_str();
  }

  
}

namespace X3DSAX2HandlersInternals {
  static const XMLCh gEmptyString[] = { chNull }; 
  static const XMLCh gUSE[] =  { chLatin_U, chLatin_S, chLatin_E, chNull };
  static const XMLCh gX3D[] =  { chLatin_X, chDigit_3, chLatin_D, chNull };
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
  stringstream s;
  s << "(" << locator->getSystemId() << " line " 
    << locator->getLineNumber() << ")" << ends;  
  return s.str();
}

// X3DSAX2Handlers: Implementation of the SAX DocumentHandler interface
void X3DSAX2Handlers::startElement(const XMLCh* const uri,
                                   const XMLCh* const localname, 
                                   const XMLCh* const qname,
                                   const Attributes& attrs) {
  if( error_on_elements ) {
    // TODO: correct error handling 
    cerr << "Element not allowed in ROUTE element" << endl;
  }
  Node *new_node = NULL;
  Node *parent = NULL;
  // skip special element X3D
  if( XMLString::equals( localname, gX3D ) ) {
    if( node_stack.size() > 0 ) 
      throw X3D::XMLParseError( "X3D element only allowed at toplevel.", "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    else
      return;
  }
  // special element Scene works as a Group node so we use a Group node.
  if( XMLString::equals( localname, gScene ) ) {
    if( node_stack.size() > 0 ) 
      throw X3D::XMLParseError( "Scene element only allowed at toplevel or in X3D element.", "", 
                                toString( locator->getSystemId() ),
                                locator->getLineNumber()  );
    else {
      node_stack.push( NodeElement( new Group ) );
      return;
    }
  }
  
  if( node_stack.size() > 0 ) parent = node_stack.top().getNode();  
  if( !parent && node_stack.size() > 0 ) {
    // the previous node element could not be created so we skip all
    // children as well. 
    if( toString( localname ).compare( "ROUTE" ) !=0 ||
        toString( localname ).compare( "ROUTE_NO_EVENT" ) !=0 ) {
      node_stack.push( NodeElement( NULL ) );
    }
  } else {
    // NOT PROPERLY IMPLEMENTED
    // if the element is a fieldValue element put the field it refers to
    // on the node stack
    if( toString( localname ).compare( "fieldValue" ) == 0 ) {
      // make sure that the parent element is a node != fieldValue
      if( parent && dynamic_cast< FieldValue * >( parent ) ) {
        // TODO: filename and linenumber
        string s( "fieldValue element not allowed in fieldValue element." );
        throw X3D::XMLParseError( "fieldValue element not allowed \
in fieldValue element.", "", 
                                  toString( locator->getSystemId() ),
                                  locator->getLineNumber()  );
      }
      const XMLCh *name = attrs.getValue( gname );
      const XMLCh *value = attrs.getValue( gvalue );
      //cerr << "fieldValue ";
      // if a 'value' field exists set the field to that value
      if( name && value ) {
        //TODO: lookup field and set value
        cerr << name << " " << value << endl;
      }
      // TODO: interface lookup of field 
      FieldValue *fv = new FieldValue( toString( localname ), NULL );
      node_stack.push( NodeElement( fv ) ); 
    } // end localname == "fieldValue"

    else if( toString( localname ).compare( "ROUTE" ) == 0 || 
             toString( localname ).compare( "ROUTE_NO_EVENT" ) == 0) {
      // The element is a ROUTE specification 
      int nr_attrs = attrs.getLength();
      const XMLCh *from_node_name  = NULL;
      const XMLCh *from_field_name = NULL;
      const XMLCh *to_node_name    = NULL;
      const XMLCh *to_field_name   = NULL;
            
      // get all the route specific attributes
      for( int i = 0; i < nr_attrs; i++ ) {
        const XMLCh *name = attrs.getQName( i );
        if( !from_node_name && toString( name ).compare( "fromNode" ) == 0 ) {
          from_node_name = attrs.getValue( i );
        } else if( !to_node_name && 
                   toString( name ).compare( "toNode" ) == 0 ) {
          to_node_name = attrs.getValue( i );
        } else if( !from_field_name && 
                   toString( name ).compare( "fromField" ) == 0 ) {
          from_field_name = attrs.getValue( i );
        } else if( !to_field_name && 
                   toString( name ).compare( "toField" ) == 0) {
          to_field_name = attrs.getValue( i );
        } else {
          cerr << "WARNING: Unknown attribute \"" << name  
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
      }
            
      // Lookup the nodes and fields and set up the route.
      Node *from_node = DEF_map->getNode( toString( from_node_name ) );
      if( from_node ) {
        Field *from_field = from_node->getField( toString( from_field_name ).c_str() );
        if( from_field ) {
          Node *to_node = DEF_map->getNode( toString( to_node_name ) );
          if( to_node ) {
            Field *to_field = to_node->getField( toString( to_field_name ).c_str() );
            if( to_field ) {
              if( toString( localname ).compare( "ROUTE" ) == 0 )
                from_field->route( to_field );
              else
                from_field->routeNoEvent( to_field );
            } else {
              cerr << "WARNING: Route error. Could not find field named \"" 
                   << to_field_name
                   << "\" in \"" << to_node_name << "\" Node " 
                   << getLocationString() << endl;
            }
          } else {
            cerr << "WARNING: Route error. Could not find Node named \"" 
                 << to_node_name
                 << "\" specified in \"toNode\" attribute " 
                 << getLocationString() << endl;
          }
        } else {
          cerr << "WARNING: Route error. Could not find field named \"" 
               << from_field_name
               << "\" in \"" << from_node_name << "\" Node " 
               << getLocationString() << endl;
        }
      } else {
        cerr << "WARNING: Route error. Could not find Node named \"" 
             << from_node_name
             << "\" specified in \"fromNode\" attribute " 
             << getLocationString() << endl;
      }
      // make sure no more elements before the end of these route element
      error_on_elements = true;
    } // end localname == "ROUTE"
    else if( toString( localname ).compare( "field" ) == 0 ) {
      H3DDynamicFieldsObject *dyn_object = 
        dynamic_cast< H3DDynamicFieldsObject * >( parent );
      if( dyn_object ) {
        int nr_attrs = attrs.getLength();
        const XMLCh *field_name  = NULL;
        const XMLCh *field_type = NULL;
        const XMLCh *field_access_type    = NULL;
        const XMLCh *field_value    = NULL;
            
        for( int i = 0; i < nr_attrs; i++ ) {
          const XMLCh *name = attrs.getQName( i );
          if( !field_name && toString( name ).compare( "name" ) == 0 ) {
            field_name = attrs.getValue( i );
          } else if( !field_type && 
                     toString( name ).compare( "type" ) == 0 ) {
            field_type = attrs.getValue( i );
          } else if( !field_access_type && 
                     toString( name ).compare( "accessType" ) == 0 ) {
            field_access_type = attrs.getValue( i );
          } else if( !field_value && 
                     toString( name ).compare( "value" ) == 0) {
            field_value = attrs.getValue( i );
          } else {
            cerr << "WARNING: Unknown attribute \"" << name  
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
            s << "Cannot parse field \"" << f->getName() 
              << "\". Field type must be a subclass of ParsableField "
              << "in order to be parsable. " << ends;
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
                     toString( field_value ): "value" ) 
                << "\" to " << e.value << " for field \"" 
                << f->getName() << "\"." << ends;
              throw X3D::XMLParseError( s.str(), "", 
                                        toString( locator->getSystemId() ),
                                        locator->getLineNumber() );
            }
            catch( const Convert::UnimplementedConversionType &e ) {
              stringstream s;
              s << "Field conversion error when converting value for field \"" 
                << f->getName() << "\". Conversion for " << e.value 
                << " not implemented" << ends;
              throw X3D::XMLParseError( s.str(), "", 
                                        toString( locator->getSystemId() ),
                                        locator->getLineNumber() ); 
            }
          } else {
            cerr << "Warning: 'value' attribute ignored. Only used if "
                 << "accesstype is initializeOnly or inputOutput " 
                 << getLocationString() << endl;
          }
        }

        dyn_object->addField( toString( field_name ),
                              access_type,
                              f );
        FieldValue *fv = new FieldValue( toString( localname ), f );
        node_stack.push( NodeElement( fv ) ); 
      }  else {
        cerr << "Warning: 'field' declaration in Node that is does not support"
             << " it " << getLocationString() << endl;
        node_stack.push( NULL ); 
      }
    } else {
      // if we have a USE attribute use that Node.

      /*
      // Note: this could really could be replaced with 
      //   const XMLCh *use_name = attrs.getValue( (XMLCh*)L"USE" );
      // but there is a problem / configuration error on OS X that 
      // causes getValue( onst XMLCh* ) to fail.
      const XMLCh *use_name='\0';
      int nr_attrs = attrs.getLength();
      for( int i = 0; i < nr_attrs; i++ ) {
        const XMLCh *name = attrs.getQName( i );
        if( toString( name ).compare("USE") == 0 )
          use_name =  attrs.getValue( i );
      }
      */  
      // = attrs.getValue( (const char*)"USE" );

      const XMLCh *use_name = attrs.getValue( gUSE );

      if( use_name ) {
        new_node = DEF_map->getNode( toString( use_name ) );
        if( !new_node ) {
          stringstream s;
          s << "Invalid USE attribute. " 
            << "No node with DEF name \"" << use_name 
            << "\" defined." << ends;
          throw X3D::XMLParseError( s.str(), "", 
                                    toString( locator->getSystemId() ),
                                    locator->getLineNumber() );
        }
                
        //cerr << "USE node: " << use_name << "(" 
        //     << new_node << ")" << endl;
        error_on_elements = true;
      } else {
        // we do not have a USE attribute so create a new Node.
        try {
          new_node = 
            H3DNodeDatabase::createNode( toString( localname ).c_str() );
          if( !new_node ) {
            cerr << "WARNING: Could not create \"" << localname 
                 << "\" node. It does not exist in the H3DNodeDatabase " 
                 << getLocationString() << endl;
          } else {
            // remove the initialization that occurs on first reference
            // since we don't want it to occur until all child nodes
            // have been created and set.
            new_node->setManualInitialize( true );
          }
        } catch (  const H3D::Exception::H3DException &e ) {
          cerr << "WARNING: Could not create \"" << localname 
               << "\" node. Exception in constructor: "
               << e << " " << getLocationString() << endl;
        }
      }
      string container_field = new_node ?
        new_node->defaultXMLContainerField(): "" ;  

      if( new_node ) {
        int nr_attrs = attrs.getLength();
        for( int i = 0; i < nr_attrs; i++ ) {
          const XMLCh *name = attrs.getQName( i );
          // if USE name only containerField and class attributes
          // allowed
          if( use_name && 
              toString( name ).compare( "USE" )!=0 &&
              toString( name ).compare( "containerField" )!=0 &&
              toString( name ).compare( "class" )!=0 ) {
            // cannot throw an exception here since if we use DTD in the x3d 
            // file all attributes will be set even if USE is used, so we must
            // allow other attributes to be specified. However we ignore them.
            continue;                 
          }
          if( toString( name ).compare("USE") == 0 ) {
            // this case has already been specially handled above, 
            // so we just ignore it
          } else if( toString( name ).compare("DEF") == 0 ) {
            const XMLCh *def_name = attrs.getValue( i );
            if( new_node && DEF_map ) {
              DEF_map->addNode( toString( def_name ), 
                                new_node ); 
              new_node->setName( toString( def_name ) );
            }
          } else if( toString( name ).compare( "containerField" ) == 0 ) {
            string s = toString( attrs.getValue( i ) );
            container_field = s; 
          } else {
            Field *field = new_node->getField( toString( name ).c_str() );
            if( !field ) {
              cerr << "WARNING: Couldn't find field named \"" << name 
                   << "\" in " << qname << " node " 
                   << getLocationString() << endl;
            } else {
              ParsableField *pfield = 
                dynamic_cast< ParsableField * >( field );
              if( !pfield ) {
                stringstream s;
                s << "Cannot parse field \"" << name 
                  << "\". Field type must be a subclass of ParsableField "
                  << "in order to be parsable. " << ends;
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
                         toString( attr_value ): "value" ) 
                    << "\" to " << e.value << " for field \"" 
                    << name << "\"." << ends;
                  throw X3D::XMLParseError( s.str(), "", 
                                            toString( locator->getSystemId() ),
                                            locator->getLineNumber() );
                }
                catch( const Convert::UnimplementedConversionType &e ) {
                  stringstream s;
                  s << "Field conversion error when converting value for field \"" 
                    << name << "\". Conversion for " << e.value 
                    << " not implemented" << ends;
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
              << "\". The field does not exist in parent node (" << parent->getName() << ")"  
              << ends;
            throw X3D::XMLParseError( s.str(), "", 
                                      toString( locator->getSystemId() ),
                                      locator->getLineNumber() ); 
          } else {
            if( !dynamic_cast< SFNode *>( f ) && 
                !dynamic_cast< MFNode *>( f ) ) {
              stringstream s;
              s << "Invalid containerField attribute \"" 
                <<  container_field
                << "\". Field is not of type SFNode or MFNode." 
                << ends; 
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



void X3DSAX2Handlers::endElement (const XMLCh *const uri,
                                  const XMLCh *const localname, 
                                  const XMLCh *const qname) {
  if( error_on_elements ) {
    error_on_elements = false;
  }
  // skip special element X3D.
  if( toString( localname ).compare( "X3D" ) == 0 ||
      toString( localname ).compare( "ROUTE" ) == 0 || 
      toString( localname ).compare( "ROUTE_NO_EVENT" ) == 0) {
    return;
  }
  
  NodeElement new_node_element = node_stack.top();
  Node *new_node = new_node_element.getNode();
  node_stack.pop();
  Node *parent = NULL;
  if( node_stack.size() > 0 ) 
    parent = node_stack.top().getNode();

  if( toString( localname ).compare( "field" ) == 0 ) {
    return;
  } else {
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
			  if( mf ) mf->push_back( new_node );
			  else cerr << "Warning: 'value' element ignored. Only used if "
                 << "field type is SFNode or MFNode" 
                 << getLocationString() << endl;
			}
          } else {
            cerr << "Warning: 'value' element ignored. Only used if "
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
}

 
//  X3DSAX2Handlers: Overrides of the SAX ErrorHandler interface
void X3DSAX2Handlers::error(const SAXParseException& e) {
  stringstream s;
  s << e.getMessage() << " (" << e.getSystemId()
    << ", line " << e.getLineNumber()
    << ", char " << e.getColumnNumber() << ")" << ends;
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
  s << ends;
  throw X3D::XMLParseError( s.str() );
}

void X3DSAX2Handlers::warning(const SAXParseException& e) {
  cerr << "Warning: " << e.getMessage() << " (" << e.getSystemId()
       << ", line " << e.getLineNumber()
       << ", char " << e.getColumnNumber() << ")" << endl;
}


