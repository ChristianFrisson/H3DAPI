//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//
#include <stdlib.h>
#include <H3D/ProfileSAX2Handlers.h>
#include <iostream>
#include <sstream>
#include <H3D/H3DNodeDatabase.h>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

using namespace std;


  
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
      s[i] = xmls[i];
    return s;
  }

namespace ProfileSAX2HandlersInternals {
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
using namespace ProfileSAX2HandlersInternals;

string ProfileSAX2Handlers::getLocationString() {
  ostringstream s;
  s << "(" << locator->getSystemId() << " line " 
    << locator->getLineNumber() << ")";  
  return s.str();
}

// ProfileSAX2Handlers: Implementation of the SAX DocumentHandler interface
void ProfileSAX2Handlers::startElement(const XMLCh* const uri,
                                   const XMLCh* const localname, 
                                   const XMLCh* const qname,
                                   const Attributes& attrs) {
  
  string localname_string = toString( localname );

  if( localname_string == "Component" ) {
    int nr_attrs = attrs.getLength();
    if( in_profile ) {
      string comp_name;
      int level = 0;
      for( int i = 0; i < nr_attrs; i++ ) {
        string name = toString( attrs.getQName( i ) );
        if( name == "name" ) {
          comp_name = toString( attrs.getValue( i ) );
        }
        else if( name == "level" ) {
          level = atoi( (char * )attrs.getValue( i ) );
        }
      }
      profile_vector.back().component_names[ comp_name ] = level;
    }
    else {
      for( int i = 0; i < nr_attrs; i++ ) {
        string name = toString( attrs.getQName( i ) );
        if( name == "name" ) {
          myX3DComponent temp_component;
          temp_component.name = toString( attrs.getValue(i) );
          component_vector.push_back( temp_component );
        }
      }
    }
  }
  else if( localname_string == "Level_1" ||
           localname_string == "Level_2" ||
           localname_string == "Level_3" ||
           localname_string == "Level_4" ||
           localname_string == "Level_5" ||
           localname_string == "Level_6" ||
           localname_string == "Level_7" ||
           localname_string == "Level_8" ||
           localname_string == "Level_9" ||
           localname_string == "Level_10" ) {
    current_level++;
  }
  else if( localname_string == "Node" ) {
    in_node = true;
    int nr_attrs = attrs.getLength();
    for( int i = 0; i < nr_attrs; i++ ) {
      string name = toString( attrs.getQName( i ) );
      if( name == "name" ) {
        myX3DNode temp_node;
        temp_node.name = toString( attrs.getValue( i ) );
        component_vector.back().nodes_per_level[ current_level ].
          push_back( temp_node );
      }
    }
  }
  else if( localname_string == "Prerequisites" ) {
    in_prerequisites = true;
  }
  else if( localname_string == "Profile" ) {
    in_profile = true;
    int nr_attrs = attrs.getLength();
    for( int i = 0; i < nr_attrs; i++ ) {
      string name = toString( attrs.getQName( i ) );
      if( name == "name" ) {
        myX3DProfile temp_profile;
        temp_profile.name = toString( attrs.getValue(i) );
        profile_vector.push_back( temp_profile );
      }
    }
  }
  else if( in_node ) {
    component_vector.back().nodes_per_level[ current_level ].back().
      fields_per_level[ current_level ].push_back( localname_string );
  }
  else if( in_prerequisites ) {
    int nr_attrs = attrs.getLength();
    int the_level = 0;
    for( int i = 0; i < nr_attrs; i++ ) {
      string name = toString( attrs.getQName( i ) );
      if( name == "level" ) {
        the_level = atoi( (char * )attrs.getValue( i ) );
        break;
      }
    }
    component_vector.back().prerequisites[ current_level ].
      push_back( make_pair( localname_string, the_level ) );
  }
}



void ProfileSAX2Handlers::endElement (const XMLCh *const uri,
                                  const XMLCh *const localname, 
                                  const XMLCh *const qname) {
  string localname_string = toString( localname );
  if( localname_string == "Component" ) {
    if( !in_profile ) {
      component_vector.back().max_level = current_level;
    }
    current_level = 0;
  }
  else if( localname_string == "Node" )
    in_node = false;
  else if( localname_string == "Prerequisites" )
    in_prerequisites = false;
  else if( localname_string == "Profile" )
    in_profile = false;
}
 
//  ProfileSAX2Handlers: Overrides of the SAX ErrorHandler interface
void ProfileSAX2Handlers::error(const SAXParseException& e) {
  stringstream s;
  s << e.getMessage() << " (" << e.getSystemId()
    << ", line " << e.getLineNumber()
    << ", char " << e.getColumnNumber() << ")";
}

void ProfileSAX2Handlers::fatalError(const SAXParseException& e) {
  stringstream s;
  s << e.getMessage();
  if( e.getSystemId() && e.getSystemId()[0] != '\0' ) {
    s << " (" << e.getSystemId()
      << ", line " << e.getLineNumber()
      << ", char " << e.getColumnNumber() << ")";
  } 
}

void ProfileSAX2Handlers::warning(const SAXParseException& e) {
  cerr << "Warning: " << e.getMessage() << " (" << e.getSystemId()
             << ", line " << e.getLineNumber()
             << ", char " << e.getColumnNumber() << ")" << endl;
}


