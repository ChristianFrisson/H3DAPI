#ifndef __VRMLDRIVER__H__
#define __VRMLDRIVER__H__

#include <string>
#include <H3D/Group.h>
#include <H3D/DEFNodes.h>
#include <H3D/FlexLexer.h>
#include <H3D/ProtoDeclaration.h>
#include <H3D/PrototypeVector.h>

using namespace std;

//namespace H3D {
//  class Group;
//}
using namespace H3D;
using namespace X3D;

// Forward declarations.
union YYSTYPE;

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
  void setProtoField( const char* name, const char* type, const
                      Field::AccessType &access_type, 
                      const char* value = 0 );

  // VRML specific functions:

  Group *getRoot() { return root; }
  void setFieldValue( const char*);
  void setNodeStatement( int );
  string getLocationString();
  string getOldLocationString();

  void addLine( const char* );

  VRMLFlexLexer* lexer;
  Group *root;
  DEFNodes *DEF_map;
  DEFNodes *DEF_export;
  
  vector< Node* > node_stack;
  vector< const char* > field_stack;
  
  int vrml_line_no;
  int old_line_no;
  int old_char_no;
  string vrml_line;
  const char *file_name;
  
  vector< ProtoDeclaration *> proto_declarations;
  PrototypeVector *proto_vector;
  static PrototypeVector *global_proto_vector;
  string proto_body;
  
  /// If this is set to non-NULL then the parser is used to create an 
  /// X3DPrototypeInstance. This allows the IS and connect elements to be used.
  X3DPrototypeInstance *proto_instance;  
};

#endif

