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
/// \file VrmlDriver.h
/// \brief This file contains a class that parses VRML files.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VRMLDRIVER__H__
#define __VRMLDRIVER__H__

#include <string>
#include <stdio.h>
#include <H3D/Group.h>
#include <H3D/DEFNodes.h>
#include <H3D/FlexLexer.h>
#include <H3D/ProtoDeclaration.h>
#include <H3D/PrototypeVector.h>
#include <H3D/H3DScriptNode.h>

using namespace std;

//namespace H3D {
//  class Group;
//}
using namespace H3D;
using namespace X3D;

// Forward declarations.
//union YYSTYPE {
//  char* val;
//};

#define YYSTYPE std::string

class VrmlDriver;

struct VRMLFlexLexer : public yyFlexLexer {
  VRMLFlexLexer( istream* arg_yyin = 0, ostream* arg_yyout = 0 ) :
    yyFlexLexer( arg_yyin, arg_yyout ) {};

  int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver);
};

// Announce to Flex the prototype we want for lexing function, ...
# define YY_DECL                                                      \
  int VRMLFlexLexer::yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver)
// ... and declare it for the parser's sake.
//YY_DECL;

// Conducting the whole scanning and parsing of Calc++.
class VrmlDriver {
public:
  VrmlDriver ();
  virtual ~VrmlDriver ();

  int result;

  // Handling the scanner.
  bool trace_scanning;
  
  // Handling the parser.
  int parse( istream *inp, const char *fn, DEFNodes *dn, DEFNodes
             *exported_nodes, PrototypeVector *prototypes );
  string file;
  bool trace_parsing;
  
  // Error handling.
  void error (const yy::location& l, const string& m);
  void error (const string& m);

  void setProtoInstance ( X3DPrototypeInstance *p ) {
    proto_instance = p;
  }

  /// Adds a field declaration to the top-most proto declaration
  /// if we are not inside a nested proto declaration.
  void setProtoField( const string &name, 
                      const string& type, 
                      const Field::AccessType &access_type, 
                      const string & value = "" );

  /// Adds a new field to the current node if it is a H3DDynamicFieldsObject
  bool addDynamicField( const string &name, 
      const string& type, 
      const Field::AccessType &access_type, 
      const string & value = "" );

  /// Connects the current proto instance field with the node field from the
  /// not at the top of the node_stack.
  bool connectProtoField( const string &proto_field, const string &node_field );

  // VRML specific functions:

  Group *getRoot() { return root.get(); }
  void setFieldValue( const char *);
  void setNodeStatement( int );
  string getLocationString();
  string getOldLocationString();

  void addLine( const char* );

  /// Returns true if we are inside a proto declatarion
  inline bool insideProtoDeclaration() {
    return !proto_declarations.empty();
  }

  /// Returns true if the parser is currently parsing text inside
  /// a top-most proto declaration and not inside a nested proto
  /// declaration.
  inline bool insideTopMostProtoDeclaration() {
    return proto_declarations.size() == 1;
  }

  /// Return true if the current node being parsed is a H3DDynamicFields object,
  /// that is if it is possible to defined new fields for it at run-time.
  inline bool insideDynamicFieldsObject() {
    if( node_stack.empty() ) return false;
    return dynamic_cast< H3DDynamicFieldsObject * >( node_stack.back() ) != NULL;
  }

  auto_ptr< VRMLFlexLexer > lexer;
  AutoRef< Group > root;

  DEFNodes *DEF_map;
  DEFNodes *DEF_export;

  /// All instances of H3DScriptNode that has been instanced so far during
  /// parsing.
  AutoRefVector< H3DScriptNode > script_nodes;

  auto_ptr< DEFNodes > local_DEF_map;
  
  vector< Node* > node_stack;
  vector< string > field_stack;
  
  int vrml_line_no;
  int old_line_no;
  int old_char_no;
  string vrml_line;
  const char *file_name;
  
  /// 
  vector< ProtoDeclaration *> proto_declarations;

  /// Place to put the 
  PrototypeVector *proto_vector;

  /// This PrototypeVector is used to store proto definitions during
  /// a call to parse() if a place is not provided to the parse function.
  auto_ptr< PrototypeVector > local_proto_vector;

  static PrototypeVector *global_proto_vector;

  /// Used to collect the proto body part of a file as a string in 
  /// a prototype declaration.
  string proto_body;
  
  /// If this is set to non-NULL then the parser is used to create an 
  /// X3DPrototypeInstance. This allows the IS and connect elements to be used.
  X3DPrototypeInstance *proto_instance;  
};

#endif

