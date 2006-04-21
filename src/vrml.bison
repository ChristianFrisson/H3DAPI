


%{
#include "H3DApi.h"
#include "Node.h"
#include "Group.h"
#include "DEFNodes.h"
#include "Inline.h"
#include "H3DExports.h"


using namespace H3D;
using namespace X3D;

// For H3D API purposes, we include a copy of FlexLexer.h so that
// users don't need a copy of Flex installed just to compile the
// API.
#include "FlexLexer.h"

#include <iostream>
#include <fstream>
using namespace std;

#define YYERROR_VERBOSE  1
extern FILE *yyin;

void setyylval( char *);
int yyerror( char const *e );
int yylex();
int parse( istream *, DEFNodes*, DEFNodes* );
char *getLocationString();
Group *getRoot();
void setFieldValue( const char*);
void setNodeStatement( int );

FlexLexer* lexer;
Group *root;
DEFNodes *DEF_map;
DEFNodes *DEF_export;

vector< Node* > node_stack;
vector< const char* > field_stack;

// %type<node_t> node
//   struct Node* node_t;

%}


%union {
  char* val;
}

%type<val> fieldValue sfValue
%type<val> mfValue STRING TRUE FALSE
%type<val> nodeNameId inputOnlyId outputOnlyId initializeOnlyId
%type<val> inlineNodeNameId exportedNodeNameId

%token ID
%token STRING
%token AS
%token VRMLNULL
%token SCRIPT

%token TRUE FALSE PROFILE COMPONENT EXPORT IMPORT META DEF USE
%token PROTO inputOnly outputOnly initializeOnly inputOutput
%token EXTERNPROTO ROUTE TO IS

%token TMFBool TMFColor TMFColorRGBA TMFDouble TMFFloat TMFImage
%token TMFInt32 TMFNode TMFRotation TMFString TMFTime TMFVec2d
%token TMFVec2f TMFVec3d TMFVec3f TSFBool TSFColor TSFColorRGBA
%token TSFDouble TSFFloat TSFImage TSFInt32 TSFNode TSFRotation
%token TSFString TSFTime TSFVec2d TSFVec2f TSFVec3d TSFVec3f


%%

x3dScene : { root = new Group(); }
       profileStatement 
       componentStatements
       metaStatements
       statements

profileStatement:        PROFILE profileNameId | ;

profileNameId:           ID ;

componentStatements:     componentStatement |
                         componentStatement componentStatements |
                         empty ;

componentStatement:      COMPONENT componentNameId:componentSupportLevel ;

componentNameId:         ID ;

componentSupportLevel:   STRING ;

exportStatement:         EXPORT nodeNameId AS exportedNodeNameId {
  Node *node= DEF_map->getNode( $2 );
  if ( node ) {
    if ( DEF_export )
      DEF_export->addNode( $4, node );
    else
      Console(3) << "WARNING: EXPORT error. No export DEF map provided!"
                 << getLocationString() << endl;
  } else {
    Console(3) << "WARNING: EXPORT error. Node named \"" 
               << $2 << "\" does not exist."
               << getLocationString() << endl;
  }
};

importStatement:         IMPORT inlineNodeNameId '.' exportedNodeNameId 
                         AS nodeNameId {
  if( strcmp( $2, "H3D_EXPORTS" ) == 0 ) {
    Node *import_node = 
      H3DExports::getH3DExportNode( $4 );
    if( import_node ) {
      DEF_map->addNode( $6, import_node ); 
    } else {
      Console(3) << "WARNING: IMPORT error. H3D_EXPORTS " 
                 << "does not include \""
                 << $4 << "\"" 
                 << getLocationString() << endl;
    }
  } else {
    Node *n = DEF_map->getNode( $2 );
    Inline *inline_node = dynamic_cast< Inline * >( n );
    if( inline_node ) {
      Node *import_node = 
        inline_node->exported_nodes.getNode( $4 );
      if( import_node ) {
        DEF_map->addNode( $6, 
                          import_node ); 
      } else {
        Console(3) << "WARNING: IMPORT error. Inline node \"" 
                   << $2 << "\" does not EXPORT \""
                   << $4 << "\"" 
                   << getLocationString() << endl;
      }
    } else {
      if( n ) {
        Console(3) << "WARNING: IMPORT error. Node \"" 
                   << $2 << "\" is not an Inline node "
                   << getLocationString() << endl;
      } else {
        Console(3) << "WARNING: IMPORT error. Node named \"" 
                   << $2 << "\" does not exist."
                   << getLocationString() << endl;
      }
    }
  }
};

metaStatements:          metaStatement |
                         metaStatement metaStatements |
                         empty ;

metaStatement:           META metakey metavalue ;

metakey:                 STRING ;

metavalue:               STRING ;

statements:              statement |
                         statement statements |
                         empty ;

statement:               nodeStatement {
                            Node *node = node_stack.back();
                            node_stack.pop_back();
                            root->children->push_back( node );
                         } | 
                         exportStatement |
                         importStatement |
                         protoStatement |
                         routeStatement ;

nodeStatement:           node  |
                         DEF nodeNameId node {
                           DEF_map->addNode( $2, node_stack.back() );
                         } |
                         USE nodeNameId {
                           node_stack.push_back( DEF_map->getNode( $2 ) );
                         };

rootNodeStatement:       node | 
                         DEF nodeNameId node ;

protoStatement:          proto |
                         externproto ;

protoStatements:         protoStatement |
                         protoStatement protoStatements |
                         empty ;

proto:                   PROTO nodeTypeId '[' interfaceDeclarations ']' 
                         '{' protoBody '}' ;

protoBody:               protoStatements rootNodeStatement statements ;

interfaceDeclarations:   interfaceDeclaration |
                         interfaceDeclaration interfaceDeclarations |
                         empty ;

restrictedInterfaceDeclaration: 
                         inputOnly fieldType inputOnlyId |
                         outputOnly fieldType outputOnlyId |
                         initializeOnly fieldType initializeOnlyId fieldValue;

interfaceDeclaration:    restrictedInterfaceDeclaration |
                         inputOutput fieldType fieldId fieldValue ;

externproto:             EXTERNPROTO nodeTypeId 
                         '[' externInterfaceDeclarations ']' URLList ;

externInterfaceDeclarations:
                         externInterfaceDeclaration |
                         externInterfaceDeclaration 
                         externInterfaceDeclarations |
                         empty ;
externInterfaceDeclaration:
                         inputOnly fieldType inputOnlyId |
                         outputOnly fieldType outputOnlyId |
                         initializeOnly fieldType initializeOnlyId |
                         inputOutput fieldType ID ;

routeStatement:          ROUTE nodeNameId '.' outputOnlyId 
                         TO nodeNameId '.' inputOnlyId {
  Node *fr = DEF_map->getNode( $2 );
  if ( fr ) {
    Field *frf = fr->getField( $4 );
    if ( frf ) {
      Node *to = DEF_map->getNode( $6 );
      if ( to ) {
        Field *tof = to->getField( $8 );
        if ( tof )
          frf->route( tof );
        else {
          Console(3) << "WARNING: Route error. Could not find field named \"" 
                     <<  $8
                     << "\" in \"" << $6 << "\" Node " 
                     << getLocationString() << endl;
        }
      } else {
        Console(3) << "WARNING: Route error. Could not find destination Node named \"" 
                   << $6
                   << "\" " 
                   << getLocationString() << endl;
      }
    } else {
      Console(3) << "WARNING: Route error. Could not find field named \"" 
                 << $4
                 << "\" in \"" << $2 << "\" Node " 
                 << getLocationString() << endl;
    }  
  } else {
    Console(3) << "WARNING: Route error. Could not find source Node named \"" 
               << $2
               << "\" " 
               << getLocationString() << endl;
  }
};

URLList:                 mfValue ;

empty:                   ;


node:                    nodeTypeId { 
  Node *new_node =  H3DNodeDatabase::createNode( yylval.val );
  if ( !new_node )
    Console(3) << "WARNING: Could not create node \"" << yylval.val << 
      "\" - name not found in the node database." << endl;
  node_stack.push_back( new_node );
}
                        '{' nodeBody '}' |
                         SCRIPT '{' scriptBody '}' ;

nodeBody:                nodeBodyElement |
                         nodeBodyElement nodeBody |
                         empty ;

scriptBody:              scriptBodyElement |
                         scriptBodyElement scriptBody |
                         empty ;

scriptBodyElement:       nodeBodyElement |
                         restrictedInterfaceDeclaration |
                         inputOnly fieldType inputOnlyId IS inputOnlyId |
                         outputOnly fieldType outputOnlyId IS outputOnlyId |
                         initializeOnly fieldType initializeOnlyId IS
                         initializeOnlyId |
                         inputOutput fieldType inputOutputId IS inputOutputId;

nodeBodyElement:         initializeOnlyId { 
                            field_stack.push_back( $1 ); } fieldValue
                            {
                                field_stack.pop_back();
                            } |
                         initializeOnlyId IS initializeOnlyId |
                         inputOnlyId IS inputOnlyId |
                         outputOnlyId IS outputOnlyId |
                         inputOutputId IS inputOutputId |
                         routeStatement |
                         protoStatement ;

nodeNameId:              ID ;

nodeTypeId:              ID ;

inlineNodeNameId:        ID ;

exportedNodeNameId:      ID ;

nodeTypeId:              ID ;

initializeOnlyId:        ID ;

inputOnlyId:             ID ;

inputOutputId:           ID ;

outputOnlyId:            ID ;

fieldType:              TMFBool |
                        TMFColor |
                        TMFColorRGBA |
                        TMFDouble |
                        TMFFloat |
                        TMFImage |
                        TMFInt32 |
                        TMFNode |
                        TMFRotation |
                        TMFString |
                        TMFTime |
                        TMFVec2d |
                        TMFVec2f |
                        TMFVec3d |
                        TMFVec3f |
                        TSFBool |
                        TSFColor |
                        TSFColorRGBA |
                        TSFDouble |
                        TSFFloat |
                        TSFImage |
                        TSFInt32 |
                        TSFNode |
                        TSFRotation |
                        TSFString |
                        TSFTime |
                        TSFVec2d |
                        TSFVec2f |
                        TSFVec3d |
                        TSFVec3f ;

fieldValue:             sfValue  { 
  setFieldValue( $1 );
} |
                        mfValue  { 
  setFieldValue( $1 );
                        } |
                        sfnodeValue |
                        mfnodeValue;

sfValue:                STRING |
                        TRUE { $$ = "TRUE"; }|
                        FALSE { $$ = "FALSE";} ;

mfValue:                sfValue |
                        '[' ']' { $$ = ""; }  |
                        '[' sfValue ']' { $$ = $2; } ;


sfnodeValue:            nodeStatement {
  setNodeStatement( 0 );
                        } |
                        VRMLNULL {
  setNodeStatement( 1 );
                        };

mfnodeValue:            nodeStatement  {
  setNodeStatement( 0 );
                        } |
                        '[' ']' { 
  setNodeStatement( 1 );

                        } |
                        '[' nodeStatements ']' ;

nodeStatements:         nodeStatement  {
  setNodeStatement( 0 );
                        } |
                        nodeStatement  {
  setNodeStatement( 0 );
                        }  nodeStatements ;

fieldId:                STRING;

%%



int yyerror( char const *e ) {
   cerr << "YYERROR: " << e << endl;
   return 0;
}


void setFieldValue( const char* v ) {
  Node *node = node_stack.back();
  Field *field = node->getField( field_stack.back() );
  ParsableField *pfield =  
    dynamic_cast< ParsableField * >( field );
  if ( pfield ) {
    pfield->setValueFromString( v );
  } else
    Console(3) << "WARNING: Could not find field named " << field_stack.back() << " in " << node->getName() << " ( " << getLocationString() << " )" << endl;
}

void setNodeStatement( int nullnode ) {
  Node *node_value = NULL;
  if ( !nullnode ) {
    node_value = node_stack.back();
    node_stack.pop_back();
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

int yylex() {
  return lexer->yylex();
}

int parse( istream *inp, DEFNodes *dn, DEFNodes *exported_nodes ) {
  if ( !dn )
    DEF_map = new DEFNodes();
  else DEF_map = dn;
  DEF_export = exported_nodes;

  //yyin = fopen( f, "r" );
  if (!inp->fail()) {
    lexer = new yyFlexLexer( inp, &Console );
    yyparse();
    if ( !dn )
      delete DEF_map;
    delete lexer;
    return 1;
  } else 
    return 0;
}

char *getLocationString() {
  return "Nowhere";
}

Group *getRoot() {
  return root;
}

void setyylval( char* l ) {
  yylval.val = strdup(l);
}
