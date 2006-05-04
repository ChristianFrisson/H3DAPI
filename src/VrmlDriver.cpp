
#include "VrmlDriver.h"
#include "vrml.hpp"

PrototypeVector *vrml_driver::global_proto_vector=NULL;

vrml_driver::vrml_driver ()
  : trace_scanning (false), trace_parsing (false) {

  proto_instance = 0;
}

vrml_driver::~vrml_driver () {
}


int vrml_driver::parse( istream *inp, const char *fn, DEFNodes *dn, DEFNodes
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
    lexer = new yyFlexLexer( inp, &Console );
    scan_begin ();
      yy::vrml_parser parser (*this);
    if ( !global_proto_vector ) {
      global_proto_vector=proto_vector;
      parser.parse ();
      global_proto_vector=NULL;
    } else
      parser.parse ();

    //parser.set_debug_level (trace_parsing);
    scan_end ();
  }
  
}

void vrml_driver::error (const yy::location& l, const std::string& m) {
  cerr << "VrmlParser error: " << getLocationString() << " - " << m << endl;
}

void vrml_driver::error (const std::string& m) {
  cerr << "VrmlParser error: " << getLocationString() << " - " << m << endl;
}


void vrml_driver::setFieldValue( const char* v ) {
  Node *node = node_stack.back();
  Field *field = node->getField( field_stack.back() );
  ParsableField *pfield =  
    dynamic_cast< ParsableField * >( field );
  if ( pfield ) {
    try {
      string s;
      s.reserve( strlen(v) );
      // first remove all VRML comments from the string:
      const char *p=v;
      while( p && *p!='\0' ) {
        if ( *p != '#' ) {
          s += *p;
          p++;
        } else {
          while( p && *p != '\0' && *p != '\n' ) p++;
        }       
      }
      // then pass the string on to the field value parser:
      pfield->setValueFromString( s );
    } catch( const Convert::X3DFieldConversionError &e ) {
      Console(3) << "WARNING: Could not convert field " 
                 << field->getName() << " argument in node "
                 << node->getName() << " ( " << getOldLocationString()
                 << " ) " << endl;
    }
  } else
    Console(3) << "WARNING: Could not find field named " << field_stack.back() << " in " << node->getName() << " ( " << getOldLocationString() << " )" << endl;
  
}



void vrml_driver::setNodeStatement( int nullnode ) {
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
  Field *field = node->getField( field_stack.back() );
  SFNode *sf = dynamic_cast< SFNode *>( field );    
  if ( sf ) {
    sf->setValue( node_value );
  } else {
    MFNode *mf = dynamic_cast< MFNode *>( field );    
    if ( mf ) {
      if ( node_value )
        mf->push_back( node_value );
      else
        mf->clear();
    } else
      Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << endl;
  }
}



void vrml_driver::setProtoField( const char* name, const char* type, const
                                 Field::AccessType &access_type, 
                                 const char* value = 0 ) {
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


void vrml_driver::addLine( const char *c ) {
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

string vrml_driver::getLocationString() {
  stringstream ss;
  ss << file_name << ":" << vrml_line_no << "." << vrml_line.length();
  return ss.str();
}

string vrml_driver::getOldLocationString() {
  stringstream ss;
  ss << file_name << ":" << old_line_no << "." << old_char_no;
  return ss.str();
}


int yyFlexLexer::yylex() {
  Console(3) << "BAD yylex called" << endl;
}
