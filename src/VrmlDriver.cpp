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
/// \file VrmlDriver.cpp
/// \brief CPP file for VrmlDriver. A class taking care of parsing of VRMl
/// code.
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTypeFunctions.h>
#include <H3D/VrmlDriver.h>
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
  lexer.reset( NULL );
  root.reset( NULL );
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
    else {
      local_proto_vector.reset( new PrototypeVector );
      proto_vector = local_proto_vector.get();
    }
  }

  if (!dn) {
    DEF_map = new DEFNodes();
    local_DEF_map.reset( DEF_map );
  }

  if (!inp->fail()) {
    lexer.reset( new VRMLFlexLexer( inp, &Console ) );
    yy::VrmlParser parser (*this);
    if ( !global_proto_vector ) {
      global_proto_vector=proto_vector;
      parser.parse ();
      global_proto_vector=NULL;

    } else
      parser.parse ();
  }

  // add named nodes from script to all script nodes.
  for( size_t i = 0; i < script_nodes.size(); ++i ) {
    script_nodes[i]->addNamedNodes( DEF_map );
  }

  return 1;
}

void VrmlDriver::error (const yy::location& l, const std::string& m) {
  Console(3) << "VrmlParser error: " << getLocationString() << " - " << m << endl;
}

void VrmlDriver::error (const std::string& m) {
  Console(3) << "VrmlParser error: " << getLocationString() << " - " << m << endl;
}


void VrmlDriver::setFieldValue( const char *v ) {
  Node *node = node_stack.back();
  if ( !node ) return;
  Field *field = node->getField( field_stack.back() );

  // trim field value... remove any outer quotes and trailing whitespace
  // that comes after those quotes:
  const char *p=v;
  const char *l=0;
  const char *r=0;
  bool sf_or_mf_string = false;
  if ( dynamic_cast<SFString*>(field) ) {
    sf_or_mf_string = true;
    while ( *p=='\n' || *p==' ' || *p=='\t' ) ++p;
    if ( *p=='"' ) {
      l=p+1;
      // go to end
      while ( *(p+1) ) ++p;
      // now go backwards
      while ( *p=='\n' || *p==' ' || *p=='\t' ) --p;
      if ( *p=='"' )
        r=p;
    } 
  }

  ParsableField *pfield =  
    dynamic_cast< ParsableField * >( field );
  if ( pfield ) {
    string s;
    s.reserve( strlen(v) );
    if( !sf_or_mf_string && dynamic_cast< MFString *>(field) ) {
      sf_or_mf_string = true;
    }

    // first remove all VRML comments from the string:
    const char *p;
    if ( l != NULL ) p=l; else p=v;
    while( p && *p!='\0' && p!=r ) {
      if ( *p != '#' ) {
        if( !sf_or_mf_string && (*p) == ',' )
          s += ' ';
        else
          s += *p;
        ++p;
      } else {
        while( p && *p != '\0' && *p != '\n' ) ++p;
      }       
    }
    // then pass the string on to the field value parser:
    try {
      pfield->setValueFromString( s );
    } catch( const Convert::X3DFieldConversionError & ) {
      Console(3) << "Warning: Could not convert field " 
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
        Console(3) << "Warning: Attempt to set an MFNode to a string value. Field named " << field_stack.back() << " in " << node->getName() << " ( " << getOldLocationString() << " )" << endl;
    } else 
      Console(3) << "Warning: Could not find field named " << field_stack.back() << " in " << node->getName() << " ( " << getOldLocationString() << " )" << endl;
  }  
}



void VrmlDriver::setNodeStatement( int nullnode ) {
  Node *node_value = NULL;
  if ( !nullnode ) {
    node_value = node_stack.back();
    if( !node_value->isInitialized() && node_value->getManualInitialize() ) {
      node_value->initialize();
    }
    node_stack.pop_back();
  }
  if ( node_stack.size() == 0 || field_stack.size() == 0 ) {
    Console(3) << "Warning: Could not set field at " << getOldLocationString() << endl;
    return;
  }
  Node *node = node_stack.back();
  if ( ! node )
    return;   // error message??

  Field *field = node->getField( field_stack.back() );
  if ( ! field ) {
    Console(3) << "Warning: Cannot set value for field named \"" << field_stack.back() << "\"  - it does not exist in node " << node->getName() << " at " << getOldLocationString() << endl ;
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
      Console(3) << "Warning: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << " at " << getOldLocationString() << endl;
  }
}



void VrmlDriver::setProtoField( const string& name, 
                                const string& type, 
                                const Field::AccessType &access_type, 
                                const string & value ) {
  if ( insideTopMostProtoDeclaration() ) {
    X3DTypes::X3DType x3d_type = 
      X3DTypes::stringToType(  type.c_str() );
    if( x3d_type == X3DTypes::UNKNOWN_X3D_TYPE ) {
      Console(3) << "Warning: Invalid type value field \"" 
                 << name << "\", " << type << " " 
                 << getLocationString() << endl;
    } else {
        proto_declarations.back()->addFieldDeclaration( name, x3d_type,
                                                        access_type, value );    
    }
  }
}


void VrmlDriver::addLine( const char *c ) {
   if ( proto_declarations.size() != 0 )
     proto_body += c;

   old_line_no=vrml_line_no;
   old_char_no=(int)vrml_line.length();

   const char *x=c;
   const char *last_line=c;
   while (*x!='\0') {
     if (*x=='\n') {
       ++vrml_line_no;
       last_line=x+1;
       vrml_line="";
     }
     ++x;
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


/// Adds a new field to the current node if it is a H3DDynamicFieldsObject
bool VrmlDriver::addDynamicField( const string &name, 
          const string& type, 
          const Field::AccessType &access_type, 
          const string & value ) {
  if( node_stack.empty() ) return false;
  
  H3DDynamicFieldsObject *df = 
    dynamic_cast< H3DDynamicFieldsObject * >( node_stack.back() );
  if( !df ) return false;

  Field *f = X3DTypes::newFieldInstance( type.c_str() );
    
  if( !f ) {
    string message = 
      "Invalid value for 'type' attribute of 'field' specification.";
    return false;
  }
    
  return df->addField( name, access_type, f );
}


/// Connects the current proto instance field with the node field from the
/// not at the top of the node_stack.
bool VrmlDriver::connectProtoField( const string &proto_field_name, 
            const string &node_field_name ) {
  if ( proto_instance != NULL ) {
    Node *node = node_stack.back();
    Field *node_field = node->getField( node_field_name );
    Field *proto_field =  
        proto_instance->getField( proto_field_name );
    if ( node && node_field && proto_field ) {
      return proto_instance->connectField( proto_field_name, node_field );
    }  
  }
  return false;
}
