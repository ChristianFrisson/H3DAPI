


%{
#include "H3DApi.h"
#include "Node.h"
#include "Group.h"
#include "DEFNodes.h"

using namespace H3D;
using namespace X3D;

#include <iostream>
using namespace std;

#define YYERROR_VERBOSE  1
extern FILE *yyin;

extern "C" {
 int yylex();
}
int yyerror( char const *e );
int parse( char *s );
char *getLocationString();
Group *getRoot();

Group *root;
DEFNodes *DEF_map;

vector< Node* > node_stack;
vector< const char* > field_stack;

// %type<node_t> node
//   struct Node* node_t;

%}


%union {
  char* val;
}

%type<val> initializeOnlyId
%type<val> fieldValue 
%type<val> sfValue
%type<val> mfValue
%type<val> STRING
%type<val> TRUE
%type<val> FALSE
%type<val> nodeNameId
%type<val> inputOnlyId
%type<val> outputOnlyId


%token ID
%token STRING
%token AS
%token VRMLNULL
%token SCRIPT

%token TRUE
%token FALSE
%token PROFILE
%token COMPONENT
%token EXPORT
%token IMPORT
%token META
%token DEF
%token USE
%token PROTO
%token inputOnly
%token outputOnly
%token initializeOnly
%token inputOutput
%token EXTERNPROTO
%token ROUTE
%token TO
%token IS

%token TMFBool
%token TMFColor
%token TMFColorRGBA
%token TMFDouble
%token TMFFloat
%token TMFImage
%token TMFInt32
%token TMFNode
%token TMFRotation
%token TMFString
%token TMFTime
%token TMFVec2d
%token TMFVec2f
%token TMFVec3d
%token TMFVec3f
%token TSFBool
%token TSFColor
%token TSFColorRGBA
%token TSFDouble
%token TSFFloat
%token TSFImage
%token TSFInt32
%token TSFNode
%token TSFRotation
%token TSFString
%token TSFTime
%token TSFVec2d
%token TSFVec2f
%token TSFVec3d
%token TSFVec3f


%%

x3dScene : { root = new Group(); }
       profileStatement 
       componentStatements
       metaStatements
       statements

profileStatement:      PROFILE profileNameId | ;

profileNameId:         ID ;

componentStatements:     componentStatement |
                         componentStatement componentStatements |
                         empty ;

componentStatement:      COMPONENT componentNameId:componentSupportLevel ;

componentNameId:         ID ;

componentSupportLevel:   STRING ;

exportStatement:         EXPORT nodeNameId AS exportedNodeNameId ;

importStatement:         IMPORT inlineNodeNameId '.' exportedNodeNameId 
                         AS nodeNameId ;

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
  Node *node = node_stack.back();
  Field *field = node->getField( field_stack.back() );
  ParsableField *pfield =  
    dynamic_cast< ParsableField * >( field );
  if ( pfield ) {
    pfield->setValueFromString( $1 );
  } else
    Console(3) << "WARNING: Could not find field named " << field_stack.back() << " in " << node->getName() << " ( " << getLocationString() << " )" << endl;
} |
                        mfValue  { 
  Node *node = node_stack.back();
  Field *field = node->getField( field_stack.back() );
  ParsableField *pfield =  
    dynamic_cast< ParsableField * >( field );
  if ( pfield ) {
    pfield->setValueFromString( $1 );
  } else
    Console(3) << "WARNING: Could not find field named " << field_stack.back() << " in " << node->getName() << " ( " << getLocationString() << " )" << endl;
                        } |
                        sfnodeValue |
                        mfnodeValue;

sfValue:                STRING |
                        TRUE { $$ = "TRUE"; }|
                        FALSE { $$ = "TRUE";} ;

mfValue:                sfValue |
                        '[' ']' { $$ = ""; }  |
                        '[' sfValue ']' { $$ = $2; } ;


sfnodeValue:            nodeStatement {
                           Node *node_value = node_stack.back();
                           node_stack.pop_back();
                           Node *node = node_stack.back();
                           Field *field = node->getField( field_stack.back() );
                           SFNode *sf = dynamic_cast< SFNode *>( field );    
                           if ( sf ) {
                             sf->setValue( node_value );
                           } else {
                             MFNode *mf = dynamic_cast< MFNode *>( field );    
                             if ( mf ) {
                               mf->push_back( node_value );
                             } else
                               Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << endl;
                           }
                        } |
                        VRMLNULL {
                           Node *node = node_stack.back();
                           Field *field = node->getField( field_stack.back() );
                           SFNode *sf = dynamic_cast< SFNode *>( field );    
                           if ( sf ) {
                             sf->setValue( NULL );
                           } else {
                             MFNode *mf = dynamic_cast< MFNode *>( field );    
                             if ( mf ) {
                               mf->clear();
                             } else
                               Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" to NULL in node " << node->getName() << endl; 
                           }
                        };

mfnodeValue:            nodeStatement  {
                           Node *node_value = node_stack.back();
                           node_stack.pop_back();
                           Node *node = node_stack.back();
                           Field *field = node->getField( field_stack.back() );
                           SFNode *sf = dynamic_cast< SFNode *>( field );    
                           if ( sf ) {
                             sf->setValue( node_value );
                           } else {
                             MFNode *mf = dynamic_cast< MFNode *>( field );    
                             if ( mf ) {
                               mf->push_back( node_value );
                             } else
                                Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << endl; 
                           }
                        }|
                        '[' ']' |
                        '[' nodeStatements ']' ;

nodeStatements:         nodeStatement  {
                           Node *node_value = node_stack.back();
                           node_stack.pop_back();
                           Node *node = node_stack.back();
                           Field *field = node->getField( field_stack.back() );
                           SFNode *sf = dynamic_cast< SFNode *>( field );    
                           if ( sf ) {
                             sf->setValue( node_value );
                           } else {
                             MFNode *mf = dynamic_cast< MFNode *>( field );    
                             if ( mf ) {
                               mf->push_back( node_value );
                             } else
                                Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << endl; 
                           }
                        } |
                        nodeStatement  {
                           Node *node_value = node_stack.back();
                           node_stack.pop_back();
                           Node *node = node_stack.back();
                           Field *field = node->getField( field_stack.back() );
                           SFNode *sf = dynamic_cast< SFNode *>( field );    
                           if ( sf ) {
                             sf->setValue( node_value );
                           } else {
                             MFNode *mf = dynamic_cast< MFNode *>( field );    
                             if ( mf ) {
                               mf->push_back( node_value );
                             } else
                               Console(3) << "WARNING: Could not set field \"" << field_stack.back() << "\" in node " << node->getName() << endl; 
                           }
                        }  nodeStatements ;

fieldId:                STRING;

%%



int yyerror( char const *e ) {
   cerr << "YYERROR: " << e << endl;
}


int parse( char *f ) {
  DEF_map = new DEFNodes();
  yyin = fopen( f, "r" );
  if (yyin)
    return yyparse();
  else
    return -1;
}

char *getLocationString() {
  return "Nowhere";
}

Group *getRoot() {
  return root;
}

