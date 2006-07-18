
#include "VrmlDriver.h"
#include "vrml.hpp"

PrototypeVector *VrmlDriver::global_proto_vector=NULL;

VrmlDriver::VrmlDriver ()
  : trace_scanning (false), trace_parsing (false) {

  proto_instance = 0;
}

VrmlDriver::~VrmlDriver () {
}


int VrmlDriver::parse( istream *inp, const char *fn, DEFNodes *dn, DEFNodes
                        *exported_nodes, PrototypeVector *prototypes ) {
  lexer = NULL;
  root = NULL;
  DEF_map = dn;
  DEF_export = exported_nodes;
  proto_vector = prototypes;
  file_name = fn;
  vrml_line_no = 1;
  old_line_no = 1;
  old_char_no = 0;
  vrml_line = "";

  if (!proto_vector) {
    if (global_proto_vector)
      proto_vector = global_proto_vector;
    else
      proto_vector = new PrototypeVector;
  }

  if (!dn) DEF_map = new DEFNodes();

  if (!inp->fail()) {
    lexer = new VRMLFlexLexer( inp, &Console );
    yy::VrmlParser parser (*this);
    if ( !global_proto_vector ) {
      global_proto_vector=proto_vector;
      parser.parse ();
      global_proto_vector=NULL;
    } else
      parser.parse ();
  }
  return 1;
}

void VrmlDriver::error (const yy::location& l, const std::string& m) {
  Console(3) << "VrmlParser error: " << getLocationString() << " - " << m << endl;
}

void VrmlDriver::error (const std::string& m) {
  Console(3) << "VrmlParser error: " << getLocationString() << " - " << m << endl;
}


void VrmlDriver::setFieldValue( const char* v ) {
  Node *node = node_stack.back();
  if ( !node ) return;
  Field *field = node->getField( field_stack.back() );

  // trim field value... remove any outer quotes and trailing whitespace
  // that comes after those quotes:
  const char *p=v;
  const char *l=0;
  const char *r=0;
  if ( dynamic_cast<SFString*>(field) ) {
    while ( *p=='\n' || *p==' ' || *p=='\t' ) p++;
    if ( *p=='"' ) {
      l=p+1;
      // go to end
      while ( *(p+1) ) p++;
      // now go backwards
      while ( *p=='\n' || *p==' ' || *p=='\t' ) p--;
      if ( *p=='"' )
        r=p;
    } 
  }

  ParsableField *pfield =  
    dynamic_cast< ParsableField * >( field );
  if ( pfield ) {
    string s;
    s.reserve( strlen(v) );
    // first remove all VRML comments from the string:
    const char *p;
    if ( l != NULL ) p=l; else p=v;
    while( p && *p!='\0' && p!=r ) {
      if ( *p != '#' ) {
        s += *p;
        p++;
      } else {
        while( p && *p != '\0' && *p != '\n' ) p++;
      }       
    }
    // then pass the string on to the field value parser:
    try {
      pfield->setValueFromString( s );
    } catch( const Convert::X3DFieldConversionError & ) {
      Console(3) << "WARNING: Could not convert field " 
                 << field->getName() << " argument in node "
                 << node->getName() << " ( " << getOldLocationString()
                 << " ) " << endl;
    }
  } else {
    MFNode *nfield = dynamic_cast< MFNode * >( field );
    if ( nfield ) {
      // it is an MFNode type field, not sure how to handle..
      // most likely trying to set an MFNode to "[ ]".
      if ( strlen(v)==0 )
        setNodeStatement(1);
      else
        Console(3) << "WARNING: Attempt to set an MFNode to a string value. Field named " << field_stack.back() << " in " << node->getName() << " ( " << getOldLocationString() << " )" << endl;
    } else 
      Console(3) << "WARNING: Could not find field named " << field_stack.back() << " in " << node->getName() << " ( " << getOldLocationString() << " )" << endl;
  }  
}



void VrmlDriver::setNodeStatement( int nullnode ) {
  Node *node_value = NULL;
  if ( !nullnode ) {
    node_value = node_stack.back();
    node_stack.pop_back();
  }
  if ( node_stack.size() == 0 || field_stack.size() == 0 ) {
    Console(3) << "WARNING: Could not set field at " << getOldLocationString() << endl;
    return;
  }
  Node *node = node_stack.back();
  if ( ! node )
    return;   // error message??

  Field *field = node->getField( field_stack.back() );
  if ( ! field ) {
    Console(3) << "WARNING: Cannot set value for field named \"" << field_stack.back() << "\"  - it does not exist in node " << node->getName() << " at " << getOldLocationString() << endl ;
    return;
  }
  SFNode *sf = dynamic_cast< SFNode *>( field );    
  if ( sf ) {
    sf->setValue( node_value );
  } else {
    MFNode *mf = dynamic_cast< MFNode *>( field );    
    if ( mf ) {
      if ( nullnode ) 
        mf->clear();
      else if ( node_value )
        mf->push_back( node_value );
    } else
      Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << " at " << getOldLocationString() << endl;
  }
}



void VrmlDriver::setProtoField( const char* name, const char* type, const
                                 Field::AccessType &access_type, 
                                 const char* value ) {
  if ( proto_declarations.size()==1 ) {
    X3DTypes::X3DType x3d_type = 
      X3DTypes::stringToType(  type );
    if( x3d_type == X3DTypes::UNKNOWN_X3D_TYPE ) {
      Console(3) << "WARNING: Invalid type value field \"" 
                 << name << "\", " << type << " " 
                 << getLocationString() << endl;
    } else {
      if ( value ) 
        proto_declarations.back()->addFieldDeclaration( name, x3d_type,
                                                        access_type, value );
      else
        proto_declarations.back()->addFieldDeclaration( name, x3d_type,
                                                        access_type );
      
    }
  }
}


void VrmlDriver::addLine( const char *c ) {
   if ( proto_declarations.size() != 0 )
     proto_body += c;

   old_line_no=vrml_line_no;
   old_char_no=vrml_line.length();

   const char *x=c;
   const char *last_line=c;
   while (*x!='\0') {
     if (*x=='\n') {
       vrml_line_no++;
       last_line=x+1;
       vrml_line="";
     }
     x++;
  }
   vrml_line += last_line;
}

string VrmlDriver::getLocationString() {
  stringstream ss;
  ss << file_name << ":" << vrml_line_no << "." << vrml_line.length();
  return ss.str();
}

string VrmlDriver::getOldLocationString() {
  stringstream ss;
  ss << file_name << ":" << old_line_no << "." << old_char_no;
  return ss.str();
}


int yyFlexLexer::yylex() {
  Console(3) << "BAD yylex called" << endl;
  return 0;
}
