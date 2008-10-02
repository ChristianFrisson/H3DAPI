
%skeleton "lalr1.cc"
%define "parser_class_name" "VrmlParser"
%defines

%{
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
//
/// \file vrml.hpp
/// \brief Bison VRML grammar header file.
//
//  To generate vrml.cpp and vrml.hpp, simply run:
//     bison -o vrml.cpp vrml.bison
//
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/H3DApi.h>
#include <H3D/Node.h>
#include <H3D/Group.h>
#include <H3D/DEFNodes.h>
#include <H3D/Inline.h>
#include <H3D/H3DExports.h>
#include <H3D/X3D.h>
#include <H3D/X3DTypeFunctions.h>

using namespace H3D;
using namespace X3D;

// For H3D API purposes, we include a copy of FlexLexer.h so that
// users don't need a copy of Flex installed just to compile the
// API.
#include <H3D/FlexLexer.h>
#include <H3D/VrmlDriver.h>

#include <iostream>
#include <sstream>
using namespace std;


#define YYERROR_VERBOSE  1

int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver);

%}


%parse-param { VrmlDriver& driver }
%lex-param   { VrmlDriver& driver }

%locations

%initial-action {
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%debug
%error-verbose

%union {
  char* val;
}

%type<val> fieldValue sfValue
%type<val> mfValue STRING VRMLTRUE VRMLFALSE
%type<val> nodeNameId inputOnlyId outputOnlyId initializeOnlyId
%type<val> inlineNodeNameId exportedNodeNameId
%type<val> nodeTypeId 
%type<val> VRMLID fieldType fieldId


%token VRMLID   "identifier"
%token STRING   "field value"
%token AS
%token VRMLNULL
%token SCRIPT

%token VRMLTRUE VRMLFALSE PROFILE COMPONENT EXPORT IMPORT META DEF USE
%token PROTO inputOnly outputOnly initializeOnly inputOutput
%token EXTERNPROTO ROUTE TO IS


%%



x3dScene : { driver.root = new Group(); }
       profileStatement 
       componentStatements
       metaStatements
       statements

profileStatement:        PROFILE profileNameId | ;

profileNameId:           VRMLID ;

componentStatements:     componentStatement |
                         componentStatement componentStatements |
                         empty ;

componentStatement:      COMPONENT componentNameId ':' componentSupportLevel ;

componentNameId:         VRMLID ;

componentSupportLevel:   STRING ;

exportStatement:         EXPORT nodeNameId AS exportedNodeNameId {
  Node *node= driver.DEF_map->getNode( $2 );
  if ( node ) {
    if ( driver.DEF_export )
      driver.DEF_export->addNode( $4, node );
    else
      Console(3) << "WARNING: EXPORT error. No export DEF map provided!"
                 << driver.getLocationString() << endl;
  } else {
    Console(3) << "WARNING: EXPORT error. Node named \"" 
               << $2 << "\" does not exist."
               << driver.getLocationString() << endl;
  }
};

importStatement:         IMPORT inlineNodeNameId '.' exportedNodeNameId 
                         AS nodeNameId {
if( driver.proto_declarations.size()==0 ) {
  if( strcmp( $2, "H3D_EXPORTS" ) == 0 ) {
    Node *import_node = 
      H3DExports::getH3DExportNode( $4 );
    if( import_node ) {
      driver.DEF_map->addNode( $6, import_node ); 
    } else {
      Console(3) << "WARNING: IMPORT error. H3D_EXPORTS " 
                 << "does not include \""
                 << $4 << "\"" 
                 << driver.getLocationString() << endl;
    }
  } else {
    Node *n = driver.DEF_map->getNode( $2 );
    Inline *inline_node = dynamic_cast< Inline * >( n );
    if( inline_node ) {
      Node *import_node = 
        inline_node->exported_nodes.getNode( $4 );
      if( import_node ) {
        driver.DEF_map->addNode( $6, 
                          import_node ); 
      } else {
        Console(3) << "WARNING: IMPORT error. Inline node \"" 
                   << $2 << "\" does not EXPORT \""
                   << $4 << "\"" 
                   << driver.getLocationString() << endl;
      }
    } else {
      if( n ) {
        Console(3) << "WARNING: IMPORT error. Node \"" 
                   << $2 << "\" is not an Inline node "
                   << driver.getLocationString() << endl;
      } else {
        Console(3) << "WARNING: IMPORT error. Node named \"" 
                   << $2 << "\" does not exist."
                   << driver.getLocationString() << endl;
      }
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
                           if ( driver.proto_declarations.size()==0 ) {
                              Node *node = driver.node_stack.back();
                              driver.node_stack.pop_back();
                              if ( node ) 
                                driver.root->children->push_back( node );
                           }
                         } | 
                         exportStatement |
                         importStatement |
                         protoStatement |
                         routeStatement ;

nodeStatement:           node  |
                         DEF nodeNameId node {
  if ( driver.proto_declarations.size()==0 &&
       driver.node_stack.back() ) {
    driver.DEF_map->addNode( $2, driver.node_stack.back() );
    driver.node_stack.back()->setName( $2 );
  } 
} |
                         USE nodeNameId {
  if ( driver.proto_declarations.size()==0 )
    driver.node_stack.push_back( driver.DEF_map->getNode( $2 ) );
  };

rootNodeStatement:       node | 
                         DEF nodeNameId node ;

protoStatement:          proto |
                         externproto ;

protoStatements:         protoStatement |
                         protoStatement protoStatements |
                         empty ;

proto:                   PROTO nodeTypeId {
   if ( driver.proto_vector ) {
     driver.proto_declarations.push_back( new ProtoDeclaration( $2 ) );
     if ( driver.proto_declarations.size()==1 ) {
       driver.proto_body = "";
       driver.proto_vector->push_back( driver.proto_declarations.back() );
     }
   }
} '[' interfaceDeclarations ']'  
  '{' { if ( driver.proto_declarations.size()==1 ) driver.proto_body = ""; } 
   protoBody  {
   if ( driver.proto_declarations.size()==1 ) {
     // remove trailing '}'
     int pos = driver.proto_body.find_last_of( "}" );
     driver.proto_body[pos]=' ';
     driver.proto_declarations.back()->setProtoBody( driver.proto_body );
   }
   driver.proto_declarations.pop_back();
}
  '}' ;

protoBody:               protoStatements rootNodeStatement statements ;

interfaceDeclarations:   interfaceDeclaration |
                         interfaceDeclaration interfaceDeclarations |
                         empty ;

restrictedInterfaceDeclaration: 
                         inputOnly fieldType inputOnlyId {
  driver.setProtoField( $3, $2, Field::INPUT_ONLY, NULL );
}|
                         outputOnly fieldType outputOnlyId {
  driver.setProtoField( $3, $2, Field::OUTPUT_ONLY, NULL );
} |
                         initializeOnly fieldType initializeOnlyId fieldValue {
  driver.setProtoField( $3, $2, Field::INITIALIZE_ONLY, $4 );
};

interfaceDeclaration:    restrictedInterfaceDeclaration |
                         inputOutput fieldType fieldId fieldValue {
  driver.setProtoField( $3, $2, Field::INPUT_OUTPUT, $4 );
};


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
                         inputOutput fieldType VRMLID ;

routeStatement:          ROUTE nodeNameId '.' outputOnlyId 
                         TO nodeNameId '.' inputOnlyId {
if ( driver.proto_declarations.size()==0 ) {
  Node *fr = driver.DEF_map->getNode( $2 );
  if ( fr ) {
    Field *frf = fr->getField( $4 );
    if ( frf ) {
      Node *to = driver.DEF_map->getNode( $6 );
      if ( to ) {
        Field *tof = to->getField( $8 );
        if ( tof )
          frf->route( tof );
        else {
          Console(3) << "WARNING: Route error. Could not find field named \"" 
                     <<  $8
                     << "\" in \"" << $6 << "\" Node " 
                     << driver.getLocationString() << endl;
        }
      } else {
        Console(3) << "WARNING: Route error. Could not find destination Node named \"" 
                   << $6
                   << "\" " 
                   << driver.getLocationString() << endl;
      }
    } else {
      Console(3) << "WARNING: Route error. Could not find field named \"" 
                 << $4
                 << "\" in \"" << $2 << "\" Node " 
                 << driver.getLocationString() << endl;
    }  
  } else {
    Console(3) << "WARNING: Route error. Could not find source Node named \"" 
               << $2
               << "\" " 
               << driver.getLocationString() << endl;
  }
}
};

URLList:                 mfValue ;

empty:                   ;


node:                    nodeTypeId { 
if ( driver.proto_declarations.size()==0 ) {
  Node *new_node = NULL;     
  if ( driver.node_stack.size() == 0 || 
       driver.node_stack.back() != NULL ) {
    new_node =  H3DNodeDatabase::createNode( yylval.val );
    if ( !new_node ) {
      // try as a proto:
      ProtoDeclaration *proto = driver.proto_vector->getProtoDeclaration(
      yylval.val );
      if ( proto ) {
         new_node = proto->newProtoInstance();
      }
      if ( !new_node )
        Console(3) << "WARNING: Could not create node \"" << yylval.val << 
          "\" - name not found in the node database ( " <<
          driver.getLocationString() << " )." << endl;
    }
  }
  driver.node_stack.push_back( new_node ); 
  }
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
                            driver.field_stack.push_back( $1 ); } fieldValue
                            {
                           if ( driver.proto_declarations.size()==0 )
                                driver.field_stack.pop_back();
                            } |
                         initializeOnlyId IS initializeOnlyId {
  if ( driver.proto_instance != NULL ) {
    Node *node = driver.node_stack.back();
    Field *node_field = node->getField( $1 );
    Field *proto_field =  
        driver.proto_instance->getField( $3 );
    if ( node && node_field && proto_field ) {
      Field::AccessType access_type = node_field->getAccessType();
        
      if( access_type == Field::OUTPUT_ONLY ) {
        node_field->route( proto_field, driver.proto_instance->id );
      } else if ( access_type == Field::INPUT_OUTPUT ) {
        node_field->routeNoEvent( proto_field, driver.proto_instance->id );
      }
      
      if( access_type != Field::OUTPUT_ONLY ) {
        proto_field->route( node_field, driver.proto_instance->id );
      }
    }
  }
}|
                         routeStatement |
                         protoStatement ;

nodeNameId:              VRMLID ;

nodeTypeId:              VRMLID ;

inlineNodeNameId:        VRMLID ;

exportedNodeNameId:      VRMLID ;

initializeOnlyId:        VRMLID ;

inputOnlyId:             VRMLID ;

inputOutputId:           VRMLID ;

outputOnlyId:            VRMLID ;

fieldType:              VRMLID ;

fieldValue:             sfValue  { 
if ( driver.proto_declarations.size()==0 )
  driver.setFieldValue( $1 );
} |
                        mfValue  { 
if ( driver.proto_declarations.size()==0 )
  driver.setFieldValue( $1 );
                        } |
                        sfnodeValue {} |
                        mfnodeValue {};

sfValue:                STRING |
                        VRMLTRUE { $$ = "TRUE"; }|
                        VRMLFALSE { $$ = "FALSE";} ;

mfValue:                sfValue |
                        '[' ']' { $$ = ""; }  |
                        '[' sfValue ']' { $$ = $2; } ;


sfnodeValue:            nodeStatement {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        } |
                        VRMLNULL {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 1 );
                        };

mfnodeValue:            nodeStatement  {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        } |
                        '[' ']' { 
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 1 );

                        } |
                        '[' nodeStatements ']' ;

nodeStatements:         nodeStatement  {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        } |
                        nodeStatement  {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        }  nodeStatements ;

fieldId:                VRMLID;

%%


void yy::VrmlParser::error( const yy::VrmlParser::location_type& l,
                            const std::string& m ) {
  driver.error( l, m );
}

int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver) {
   return driver.lexer->yylex(yylval, yylloc, driver);
}
