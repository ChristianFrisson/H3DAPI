
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */

/* Line 311 of lalr1.cc  */
#line 6 "vrml.bison"

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
#include <H3D/GlobalSettings.h>

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

#define YYSTYPE std::string
#define YYERROR_VERBOSE  1

int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver);



/* Line 311 of lalr1.cc  */
#line 106 "vrml.cpp"


#include "vrml.hpp"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc  */
#line 115 "vrml.cpp"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)  \
do {              \
  if (yydebug_)            \
    {              \
      *yycdebug_ << Title << ' ';      \
      yy_symbol_print_ ((Type), (Value), (Location));  \
      *yycdebug_ << std::endl;        \
    }              \
} while (false)

# define YY_REDUCE_PRINT(Rule)    \
do {          \
  if (yydebug_)        \
    yy_reduce_print_ (Rule);    \
} while (false)

# define YY_STACK_PRINT()    \
do {          \
  if (yydebug_)        \
    yystack_print_ ();      \
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok    (yyerrstatus_ = 0)
#define yyclearin  (yychar = yyempty_)

#define YYACCEPT  goto yyacceptlab
#define YYABORT    goto yyabortlab
#define YYERROR    goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 380 of lalr1.cc  */
#line 184 "vrml.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  VrmlParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  VrmlParser::VrmlParser (VrmlDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  VrmlParser::~VrmlParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  VrmlParser::yy_symbol_value_print_ (int yytype,
         const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
    break;
      }
  }


  void
  VrmlParser::yy_symbol_print_ (int yytype,
         const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
         << ' ' << yytname_[yytype] << " ("
         << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  VrmlParser::yydestruct_ (const char* yymsg,
         int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
  default:
    break;
      }
  }

  void
  VrmlParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  VrmlParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  VrmlParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  VrmlParser::debug_level_type
  VrmlParser::debug_level () const
  {
    return yydebug_;
  }

  void
  VrmlParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  VrmlParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 553 of lalr1.cc  */
#line 76 "vrml.bison"
{
  // Initialize the initial location.
  yylloc.begin.filename = yylloc.end.filename = &driver.file;
}

/* Line 553 of lalr1.cc  */
#line 368 "vrml.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
  YYCDEBUG << "Reading a token: ";
  yychar = yylex (&yylval, &yylloc, driver);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
  yychar = yytoken = yyeof_;
  YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
  yytoken = yytranslate_ (yychar);
  YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
  if (yyn == 0 || yyn == yytable_ninf_)
  goto yyerrlab;
  yyn = -yyn;
  goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
    case 2:

/* Line 678 of lalr1.cc  */
#line 100 "vrml.bison"
    { driver.root.reset( new Group() ); }
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 120 "vrml.bison"
    {
  Node *node= driver.DEF_map->getNode( (yysemantic_stack_[(4) - (2)]) );
  if ( node ) {
    if ( driver.DEF_export )
      driver.DEF_export->addNode( (yysemantic_stack_[(4) - (4)]), node );
    else
      Console(3) << "Warning: EXPORT error. No export DEF map provided!"
                 << driver.getLocationString() << endl;
  } else {
    Console(3) << "Warning: EXPORT error. Node named \"" 
               << (yysemantic_stack_[(4) - (2)]) << "\" does not exist."
               << driver.getLocationString() << endl;
  }
}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 136 "vrml.bison"
    {
if( !driver.insideProtoDeclaration() ) {
  if(  (yysemantic_stack_[(6) - (2)]) == "H3D_EXPORTS" ) {
    Node *import_node = 
      H3DExports::getH3DExportNode( (yysemantic_stack_[(6) - (4)]) );
    if( import_node ) {
      driver.DEF_map->addNode( (yysemantic_stack_[(6) - (6)]), import_node ); 
    } else {
      Console(3) << "Warning: IMPORT error. H3D_EXPORTS " 
                 << "does not include \""
                 << (yysemantic_stack_[(6) - (4)]) << "\"" 
                 << driver.getLocationString() << endl;
    }
  } else {
    Node *n = driver.DEF_map->getNode( (yysemantic_stack_[(6) - (2)]) );
    Inline *inline_node = dynamic_cast< Inline * >( n );
    if( inline_node ) {
      Node *import_node = 
        inline_node->exported_nodes.getNode( (yysemantic_stack_[(6) - (4)]) );
      if( import_node ) {
        driver.DEF_map->addNode( (yysemantic_stack_[(6) - (6)]), 
                          import_node ); 
      } else {
        Console(3) << "Warning: IMPORT error. Inline node \"" 
                   << (yysemantic_stack_[(6) - (2)]) << "\" does not EXPORT \""
                   << (yysemantic_stack_[(6) - (4)]) << "\"" 
                   << driver.getLocationString() << endl;
      }
    } else {
      if( n ) {
        Console(3) << "Warning: IMPORT error. Node \"" 
                   << (yysemantic_stack_[(6) - (2)]) << "\" is not an Inline node "
                   << driver.getLocationString() << endl;
      } else {
        Console(3) << "Warning: IMPORT error. Node named \"" 
                   << (yysemantic_stack_[(6) - (2)]) << "\" does not exist."
                   << driver.getLocationString() << endl;
      }
    }
  }
}
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 193 "vrml.bison"
    {
                           if ( !driver.insideProtoDeclaration() ) {
                              Node *node = driver.node_stack.back();
                              driver.node_stack.pop_back();
                              if ( node ) 
                                driver.root->children->push_back( node );
                           }
                         }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 207 "vrml.bison"
    {
  if ( !driver.insideProtoDeclaration() &&
       driver.node_stack.back() ) {
    Node *new_node = driver.node_stack.back();
    if( !new_node->isInitialized() && new_node->getManualInitialize() ) 
      new_node->initialize();   

    driver.DEF_map->addNode( (yysemantic_stack_[(3) - (2)]), new_node );
    new_node->setName( (yysemantic_stack_[(3) - (2)]) );
  } 
}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 218 "vrml.bison"
    {
  if ( !driver.insideProtoDeclaration() )
    driver.node_stack.push_back( driver.DEF_map->getNode( (yysemantic_stack_[(2) - (2)]) ) );
  }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 233 "vrml.bison"
    {
   if ( driver.proto_vector ) {
     driver.proto_declarations.push_back( new ProtoDeclaration( (yysemantic_stack_[(2) - (2)]) ) );

     // only process proto declaration if not nested in another proto declaration.
     if ( driver.insideTopMostProtoDeclaration() ) {
       driver.proto_body = "";
       driver.proto_vector->push_back( driver.proto_declarations.back() );
     }
   }
}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 244 "vrml.bison"
    {  
    // if in top-most proto declaration, reset proto_body to start
    // collecting the string to use
    if ( driver.insideTopMostProtoDeclaration() ) driver.proto_body = ""; 
   }
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 249 "vrml.bison"
    {

   // only process proto declaration if not nested in another proto declaration.
   if ( driver.insideTopMostProtoDeclaration() ) {
     // remove trailing '}'
     int pos = driver.proto_body.find_last_of( "}" );
     driver.proto_body[pos]=' ';
     driver.proto_declarations.back()->setProtoBody( driver.proto_body );
   }
   // TODO: delete proto declaration if nested?
   driver.proto_declarations.pop_back();
}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 270 "vrml.bison"
    {
   driver.setProtoField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INPUT_ONLY );
}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 273 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::OUTPUT_ONLY );
}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 276 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[(4) - (3)]), (yysemantic_stack_[(4) - (2)]), Field::INITIALIZE_ONLY, (yysemantic_stack_[(4) - (4)]) );
}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 281 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[(4) - (3)]), (yysemantic_stack_[(4) - (2)]), Field::INPUT_OUTPUT, (yysemantic_stack_[(4) - (4)]) );
}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 301 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() ) {
  Node *fr = driver.DEF_map->getNode( (yysemantic_stack_[(8) - (2)]) );
  if ( fr ) {
    Field *frf = fr->getField( (yysemantic_stack_[(8) - (4)]) );
    if ( frf ) {
      Node *to = driver.DEF_map->getNode( (yysemantic_stack_[(8) - (6)]) );
      if ( to ) {
        Field *tof = to->getField( (yysemantic_stack_[(8) - (8)]) );
        if ( tof ) {
           bool event = GlobalSettings::default_x3d_route_sends_event;
           GlobalSettings *gs = GlobalSettings::getActive();
           if( gs ) event = gs->x3dROUTESendsEvent->getValue();
           if( event ) frf->route( tof );
           else frf->routeNoEvent( tof );
  }
        else {
          Console(3) << "Warning: Route error. Could not find field named \"" 
                     <<  (yysemantic_stack_[(8) - (8)])
                     << "\" in \"" << (yysemantic_stack_[(8) - (6)]) << "\" Node " 
                     << driver.getLocationString() << endl;
        }
      } else {
        Console(3) << "Warning: Route error. Could not find destination Node named \"" 
                   << (yysemantic_stack_[(8) - (6)])
                   << "\" " 
                   << driver.getLocationString() << endl;
      }
    } else {
      Console(3) << "Warning: Route error. Could not find field named \"" 
                 << (yysemantic_stack_[(8) - (4)])
                 << "\" in \"" << (yysemantic_stack_[(8) - (2)]) << "\" Node " 
                 << driver.getLocationString() << endl;
    }  
  } else {
    Console(3) << "Warning: Route error. Could not find source Node named \"" 
               << (yysemantic_stack_[(8) - (2)])
               << "\" " 
               << driver.getLocationString() << endl;
  }
}
}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 344 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() ) {
  Node *new_node = NULL;     
  if ( driver.node_stack.size() == 0 || 
       driver.node_stack.back() != NULL ) {
    new_node =  H3DNodeDatabase::createNode( yylval );
    if ( !new_node ) {
      // try as a proto:
      ProtoDeclaration *proto = driver.proto_vector->getProtoDeclaration(
      yylval );
      if ( proto ) {
         new_node = proto->newProtoInstance();
      }
      if ( !new_node )
        Console(3) << "Warning: Could not create node \"" << yylval << 
          "\" - name not found in the node database and is not a proto name ( " <<
          driver.getLocationString() << " )." << endl;
    } else {
      // if node is a script node add the current named nodes
      // from the current DEF_map and store the node in the 
      // script_nodes vector. When parsing is done the named
      // nodes will be updated in all scripts to contain
      // all named nodes from the parsed file.
      if( H3DScriptNode *script_node = 
             dynamic_cast< H3DScriptNode * >( new_node ) ) {
         script_node->setManualInitialize( true );
         script_node->addNamedNodes( driver.DEF_map );
         driver.script_nodes.push_back( script_node );
      } 
    }   
  }
  driver.node_stack.push_back( new_node ); 
  }
}
    break;

  case 74:

/* Line 678 of lalr1.cc  */
#line 397 "vrml.bison"
    { 
         if( !driver.insideProtoDeclaration() )  
                             driver.field_stack.push_back( (yysemantic_stack_[(1) - (1)]) );    
                         }
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 401 "vrml.bison"
    {
                           if ( !driver.insideProtoDeclaration() )
                              driver.field_stack.pop_back();
                          }
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 405 "vrml.bison"
    {
    driver.connectProtoField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (1)]) ); 
}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 410 "vrml.bison"
    {
                            driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INPUT_ONLY );     
       }
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 413 "vrml.bison"
    {
                           driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::OUTPUT_ONLY );     
                         }
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 416 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() )  
                             driver.field_stack.push_back( (yysemantic_stack_[(3) - (3)]) );  
         driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INITIALIZE_ONLY );
                         }
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 421 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() ) {
           driver.field_stack.pop_back();
                           }
                         }
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 426 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() )  
                             driver.field_stack.push_back( (yysemantic_stack_[(3) - (3)]) );  
         driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INPUT_OUTPUT );
                         }
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 431 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() ) {
           driver.field_stack.pop_back();
                           }
                         }
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 436 "vrml.bison"
    {
                            driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::INPUT_ONLY );
                            driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
       }
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 440 "vrml.bison"
    {
                           driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::OUTPUT_ONLY );
                           driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
                         }
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 444 "vrml.bison"
    {
         driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::INITIALIZE_ONLY );
                           driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
                         }
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 448 "vrml.bison"
    {
         driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::INPUT_OUTPUT );
                           driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
                         }
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 471 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() )
  driver.setFieldValue( ((yysemantic_stack_[(1) - (1)])).c_str() );
}
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 475 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() )
  driver.setFieldValue( ((yysemantic_stack_[(1) - (1)])).c_str() );
                        }
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 479 "vrml.bison"
    {}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 480 "vrml.bison"
    {}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 483 "vrml.bison"
    { (yyval) = "TRUE"; }
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 484 "vrml.bison"
    { (yyval) = "FALSE";}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 487 "vrml.bison"
    { (yyval) = ""; }
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 488 "vrml.bison"
    { (yyval) = (yysemantic_stack_[(3) - (2)]); }
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 491 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        }
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 495 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 1 );
                        }
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 500 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        }
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 504 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 1 );

                        }
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 511 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        }
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 515 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        }
    break;



/* Line 678 of lalr1.cc  */
#line 1022 "vrml.cpp"
  default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
  && yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
  ++yynerrs_;
  error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
  /* If just tried and failed to reuse lookahead token after an
   error, discard it.  */

  if (yychar <= yyeof_)
    {
    /* Return failure if at end of input.  */
    if (yychar == yyeof_)
      YYABORT;
    }
  else
    {
      yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
      yychar = yyempty_;
    }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;  /* Each real token shifted decrements this.  */

    for (;;)
      {
  yyn = yypact_[yystate];
  if (yyn != yypact_ninf_)
  {
    yyn += yyterror_;
    if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
      {
        yyn = yytable_[yyn];
        if (0 < yyn)
    break;
      }
  }

  /* Pop the current state because it cannot handle the error token.  */
  if (yystate_stack_.height () == 1)
  YYABORT;

  yyerror_range[0] = yylocation_stack_[0];
  yydestruct_ ("Error: popping",
         yystos_[yystate],
         &yysemantic_stack_[0], &yylocation_stack_[0]);
  yypop_ ();
  yystate = yystate_stack_[0];
  YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
         &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
  yydestruct_ ("Cleanup: popping",
       yystos_[yystate_stack_[0]],
       &yysemantic_stack_[0],
       &yylocation_stack_[0]);
  yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  VrmlParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
  /* Start YYX at -YYN if negative to avoid negative indexes in
     YYCHECK.  */
  int yyxbegin = yyn < 0 ? -yyn : 0;

  /* Stay within bounds of both yycheck and yytname.  */
  int yychecklim = yylast_ - yyn + 1;
  int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
  int count = 0;
  for (int x = yyxbegin; x < yyxend; ++x)
    if (yycheck_[x + yyn] == x && x != yyterror_)
      ++count;

  // FIXME: This method of building the message is not compatible
  // with internationalization.  It should work like yacc.c does it.
  // That is, first build a string that looks like this:
  // "syntax error, unexpected %s or %s or %s"
  // Then, invoke YY_ on this string.
  // Finally, use the string as a format to output
  // yytname_[tok], etc.
  // Until this gets fixed, this message appears in English only.
  res = "syntax error, unexpected ";
  res += yytnamerr_ (yytname_[tok]);
  if (count < 5)
    {
      count = 0;
      for (int x = yyxbegin; x < yyxend; ++x)
        if (yycheck_[x + yyn] == x && x != yyterror_)
    {
      res += (!count++) ? ", expecting " : " or ";
      res += yytnamerr_ (yytname_[x]);
    }
    }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char VrmlParser::yypact_ninf_ = -121;
  const short int
  VrmlParser::yypact_[] =
  {
      -121,    21,    14,  -121,    39,    36,  -121,  -121,    45,    43,
      36,  -121,  -121,    55,   188,    43,  -121,  -121,  -121,  -121,
      49,    65,    76,    65,    65,    81,    81,    65,  -121,  -121,
    -121,   188,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,   203,  -121,    86,  -121,    66,    57,  -121,  -121,    88,
      90,  -121,    91,  -121,   114,   114,   114,   114,  -121,  -121,
    -121,  -121,    92,   203,  -121,    93,   116,   116,  -121,    97,
      79,   122,   210,  -121,   123,   122,   124,   125,  -121,  -121,
     124,   154,  -121,  -121,   128,    89,   114,   114,   114,   114,
     102,    79,  -121,  -121,   110,   114,   114,   114,   114,  -121,
     105,   210,  -121,   111,   112,   140,   117,   120,  -121,  -121,
    -121,  -121,  -121,  -121,    22,  -121,  -121,  -121,  -121,  -121,
    -121,    65,   114,   114,   114,   114,   126,  -121,    89,  -121,
     123,   122,   124,   147,    47,  -121,    65,   123,   122,   124,
     125,  -121,  -121,   123,   122,   124,   154,  -121,   148,   154,
    -121,   131,   136,   143,  -121,   123,   122,   124,   149,   144,
    -121,  -121,  -121,  -121,  -121,    35,  -121,  -121,  -121,   127,
     150,   151,   152,   153,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,    46,  -121,  -121,  -121,  -121,   154,  -121,   154,  -121,
    -121,   123,   123,   122,   124,   148,  -121,  -121,    24,  -121,
    -121,  -121,  -121,  -121,    24,    78,  -121,  -121,  -121,    65,
     188,  -121,   142,    57,  -121,  -121,  -121
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  VrmlParser::yydefact_[] =
  {
         2,     0,     5,     1,     0,    58,     6,     4,     0,    58,
       7,     9,    10,     0,    58,    13,    15,     8,    16,    90,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    22,
       3,    17,    20,    23,    30,    31,    24,    19,    25,    59,
      14,    58,    89,     0,    91,     0,     0,    27,    35,     0,
       0,    18,     0,    93,     0,     0,     0,     0,    78,    69,
      77,    67,     0,    58,    68,    74,     0,     0,    26,     0,
      58,     0,    58,    97,     0,     0,     0,     0,    61,    66,
       0,     0,    92,    11,     0,    58,     0,     0,     0,     0,
       0,    49,    51,    96,     0,     0,     0,     0,     0,    64,
       0,    58,    94,    43,    44,     0,   116,     0,    83,    76,
     102,   109,   103,   104,     0,   108,    75,    98,    99,   100,
     101,     0,     0,     0,     0,     0,     0,    46,    40,    42,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,    60,    63,     0,     0,     0,     0,    45,     0,     0,
     106,   114,     0,     0,    12,     0,     0,     0,     0,     0,
      41,    52,    53,    54,    55,     0,    48,   105,    57,     0,
      79,    80,    81,     0,    70,    71,    72,    82,    95,    73,
      84,     0,   107,   112,    43,    44,     0,   116,     0,    36,
     106,     0,     0,     0,     0,     0,   115,    47,    58,    56,
      85,    86,    87,    88,    32,     0,    37,    34,    33,     0,
      58,    28,     0,     0,    39,    38,    29
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  VrmlParser::yypgoto_[] =
  {
      -121,  -121,  -121,  -121,  -121,   169,  -121,  -121,  -121,   165,
    -121,   -28,  -121,   -13,  -121,   -12,   -21,  -121,  -121,  -121,
    -121,  -121,    56,   -74,  -121,  -121,    94,  -121,    -8,  -121,
      -5,   -39,  -121,    87,   133,  -121,   -56,  -121,  -121,  -121,
     -15,     7,  -121,   141,   -63,  -103,  -120,   -61,    16,   -59,
     -99,    53,  -121,  -121,     8,  -121,    32
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  VrmlParser::yydefgoto_[] =
  {
        -1,     1,     2,     5,     7,     9,    10,    28,    29,    14,
      15,    30,    31,   115,   210,    58,   205,    34,    69,   198,
     212,   206,   126,    59,   128,    35,    90,    91,    60,   166,
      37,    38,    52,   100,    62,    63,    64,    81,   146,   149,
      43,    39,    45,    83,    65,   103,   107,    94,    74,   147,
     117,   118,   119,   120,   153,   181,   108
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char VrmlParser::yytable_ninf_ = -114;
  const short int
  VrmlParser::yytable_[] =
  {
        11,    32,    33,    51,    16,    11,    36,    68,    46,    47,
      16,   127,    50,   105,   104,   152,   101,   109,    32,    33,
     173,     3,   116,    36,     4,    19,   110,   161,   179,    20,
     112,   113,    48,    49,   170,   167,    61,    23,    24,   110,
     174,    25,     6,   112,   113,   101,    26,     8,    12,    19,
     150,   110,   184,    20,   127,   112,   113,    13,    61,    18,
      19,    23,    24,   190,    20,    92,   152,    99,    42,   163,
     162,    75,    76,    77,   165,   203,   172,   171,    41,    44,
     129,    19,   176,   175,    19,    20,    92,   177,   199,   200,
     180,    66,    67,   209,   186,   185,    99,    86,    87,    88,
      89,   151,   130,   131,   132,   133,   154,   122,   123,   124,
     125,   137,   138,   139,   140,    70,    71,    73,    80,    82,
      72,   169,    78,   129,    85,    93,   102,    53,   106,   197,
     134,   202,   201,   121,   136,   141,   143,   144,   155,   156,
     157,   158,   -95,    19,   110,   148,   111,    20,   112,   113,
     164,   178,   187,   191,   159,    23,    24,    19,   110,  -113,
     111,    20,   112,   113,   182,   145,   211,   114,   151,    23,
      24,   183,   215,   189,   216,   192,   193,   194,   195,    17,
      40,   114,   214,   208,   160,   135,   204,   168,   142,   196,
     188,    19,   204,   207,   213,    20,    79,    32,    33,   207,
      21,    22,    36,    23,    24,    25,    53,     0,    84,     0,
      26,    27,     0,    53,     0,     0,     0,     0,     0,     0,
      25,    54,    55,    56,    57,    26,    27,    25,    95,    96,
      97,    98,    26,    27
  };

  /* YYCHECK.  */
  const short int
  VrmlParser::yycheck_[] =
  {
         5,    14,    14,    31,     9,    10,    14,    46,    23,    24,
      15,    85,    27,    76,    75,   114,    72,    80,    31,    31,
     140,     0,    81,    31,    10,     3,     4,   130,   148,     7,
       8,     9,    25,    26,   137,   134,    41,    15,    16,     4,
     143,    17,     3,     8,     9,   101,    22,    11,     3,     3,
      28,     4,   155,     7,   128,     8,     9,    14,    63,     4,
       3,    15,    16,    28,     7,    70,   165,    72,     3,   132,
     131,    55,    56,    57,    27,   195,   139,   138,    29,     3,
      85,     3,   145,   144,     3,     7,    91,   146,   191,   192,
     149,     5,    26,    15,   157,   156,   101,    18,    19,    20,
      21,   114,    86,    87,    88,    89,   121,    18,    19,    20,
      21,    95,    96,    97,    98,    27,    26,     3,    25,     3,
      29,   136,    30,   128,    27,     3,     3,     3,     3,   188,
      28,   194,   193,     5,    24,    30,    25,    25,   122,   123,
     124,   125,    25,     3,     4,    25,     6,     7,     8,     9,
       3,     3,     3,    26,    28,    15,    16,     3,     4,    28,
       6,     7,     8,     9,    28,    25,   205,    27,   181,    15,
      16,    28,    30,    29,   213,    25,    25,    25,    25,    10,
      15,    27,   210,   204,   128,    91,   198,   134,   101,   181,
     158,     3,   204,   198,   209,     7,    63,   210,   210,   204,
      12,    13,   210,    15,    16,    17,     3,    -1,    67,    -1,
      22,    23,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    17,    18,    19,
      20,    21,    22,    23
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  VrmlParser::yystos_[] =
  {
         0,    32,    33,     0,    10,    34,     3,    35,    11,    36,
      37,    61,     3,    14,    40,    41,    61,    36,     4,     3,
       7,    12,    13,    15,    16,    17,    22,    23,    38,    39,
      42,    43,    44,    46,    48,    56,    59,    61,    62,    72,
      40,    29,     3,    71,     3,    73,    71,    71,    72,    72,
      71,    42,    63,     3,    18,    19,    20,    21,    46,    54,
      59,    61,    65,    66,    67,    75,     5,    26,    62,    49,
      27,    26,    29,     3,    79,    79,    79,    79,    30,    65,
      25,    68,     3,    74,    74,    27,    18,    19,    20,    21,
      57,    58,    61,     3,    78,    18,    19,    20,    21,    61,
      64,    67,     3,    76,    78,    75,     3,    77,    87,    75,
       4,     6,     8,     9,    27,    44,    80,    81,    82,    83,
      84,     5,    18,    19,    20,    21,    53,    54,    55,    61,
      79,    79,    79,    79,    28,    57,    24,    79,    79,    79,
      79,    30,    64,    25,    25,    25,    69,    80,    25,    70,
      28,    44,    81,    85,    71,    79,    79,    79,    79,    28,
      53,    76,    78,    75,     3,    27,    60,    81,    82,    71,
      76,    78,    75,    77,    76,    78,    75,    80,     3,    77,
      80,    86,    28,    28,    76,    78,    75,     3,    87,    29,
      28,    26,    25,    25,    25,    25,    85,    80,    50,    76,
      76,    78,    75,    77,    46,    47,    52,    61,    47,    15,
      45,    62,    51,    71,    42,    30,    62
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  VrmlParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    46,    91,    93,   123,
     125
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  VrmlParser::yyr1_[] =
  {
         0,    31,    33,    32,    34,    34,    35,    36,    36,    36,
      37,    38,    39,    40,    40,    40,    41,    42,    42,    42,
      43,    43,    43,    43,    43,    44,    44,    44,    45,    45,
      46,    46,    47,    47,    47,    49,    50,    51,    48,    52,
      53,    53,    53,    54,    54,    54,    55,    55,    56,    57,
      57,    57,    58,    58,    58,    58,    59,    60,    61,    63,
      62,    62,    64,    64,    64,    65,    65,    65,    66,    66,
      66,    66,    66,    66,    68,    67,    67,    67,    67,    67,
      67,    69,    67,    70,    67,    67,    67,    67,    67,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    80,
      80,    80,    81,    81,    81,    82,    82,    82,    83,    83,
      84,    84,    84,    85,    86,    85,    87
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  VrmlParser::yyr2_[] =
  {
         0,     2,     0,     5,     2,     0,     1,     1,     2,     1,
       2,     4,     6,     1,     2,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     1,     3,
       1,     1,     1,     2,     1,     0,     0,     0,    11,     3,
       1,     2,     1,     3,     3,     4,     1,     4,     6,     1,
       2,     1,     3,     3,     3,     3,     8,     1,     0,     0,
       5,     4,     1,     2,     1,     1,     2,     1,     1,     1,
       5,     5,     5,     5,     0,     3,     3,     1,     1,     3,
       3,     0,     5,     0,     5,     5,     5,     5,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     1,     1,
       1,     2,     3,     1,     0,     3,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const VrmlParser::yytname_[] =
  {
    "$end", "error", "$undefined", "\"identifier\"", "\"field value\"",
  "AS", "VRMLNULL", "SCRIPT", "VRMLTRUE", "VRMLFALSE", "PROFILE",
  "COMPONENT", "EXPORT", "IMPORT", "META", "DEF", "USE", "PROTO",
  "inputOnly", "outputOnly", "initializeOnly", "inputOutput",
  "EXTERNPROTO", "ROUTE", "TO", "IS", "'.'", "'['", "']'", "'{'", "'}'",
  "$accept", "x3dScene", "$@1", "profileStatement", "profileNameId",
  "componentStatements", "componentStatement", "exportStatement",
  "importStatement", "metaStatements", "metaStatement", "statements",
  "statement", "nodeStatement", "rootNodeStatement", "protoStatement",
  "protoStatements", "proto", "$@2", "$@3", "$@4", "protoBody",
  "interfaceDeclarations", "restrictedInterfaceDeclaration",
  "interfaceDeclaration", "externproto", "externInterfaceDeclarations",
  "externInterfaceDeclaration", "routeStatement", "URLList", "empty",
  "node", "$@5", "nodeBody", "scriptBody", "scriptBodyElement",
  "nodeBodyElement", "$@6", "$@7", "$@8", "nodeNameId", "nodeTypeId",
  "inlineNodeNameId", "exportedNodeNameId", "initializeOnlyId",
  "inputOnlyId", "inputOutputId", "outputOnlyId", "fieldType",
  "fieldValue", "sfValue", "mfValue", "sfnodeValue", "mfnodeValue",
  "nodeStatements", "$@9", "fieldId", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const VrmlParser::rhs_number_type
  VrmlParser::yyrhs_[] =
  {
        32,     0,    -1,    -1,    33,    34,    36,    40,    42,    -1,
      10,    35,    -1,    -1,     3,    -1,    37,    -1,    37,    36,
      -1,    61,    -1,    11,     3,    -1,    12,    71,     5,    74,
      -1,    13,    73,    26,    74,     5,    71,    -1,    41,    -1,
      41,    40,    -1,    61,    -1,    14,     4,    -1,    43,    -1,
      43,    42,    -1,    61,    -1,    44,    -1,    38,    -1,    39,
      -1,    46,    -1,    59,    -1,    62,    -1,    15,    71,    62,
      -1,    16,    71,    -1,    62,    -1,    15,    71,    62,    -1,
      48,    -1,    56,    -1,    46,    -1,    46,    47,    -1,    61,
      -1,    -1,    -1,    -1,    17,    72,    49,    27,    53,    28,
      29,    50,    52,    51,    30,    -1,    47,    45,    42,    -1,
      55,    -1,    55,    53,    -1,    61,    -1,    18,    79,    76,
      -1,    19,    79,    78,    -1,    20,    79,    75,    80,    -1,
      54,    -1,    21,    79,    87,    80,    -1,    22,    72,    27,
      57,    28,    60,    -1,    58,    -1,    58,    57,    -1,    61,
      -1,    18,    79,    76,    -1,    19,    79,    78,    -1,    20,
      79,    75,    -1,    21,    79,     3,    -1,    23,    71,    26,
      78,    24,    71,    26,    76,    -1,    82,    -1,    -1,    -1,
      72,    63,    29,    64,    30,    -1,     7,    29,    65,    30,
      -1,    67,    -1,    67,    64,    -1,    61,    -1,    66,    -1,
      66,    65,    -1,    61,    -1,    67,    -1,    54,    -1,    18,
      79,    76,    25,    76,    -1,    19,    79,    78,    25,    78,
      -1,    20,    79,    75,    25,    75,    -1,    21,    79,    77,
      25,    77,    -1,    -1,    75,    68,    80,    -1,    75,    25,
      75,    -1,    59,    -1,    46,    -1,    18,    79,    76,    -1,
      19,    79,    78,    -1,    -1,    20,    79,    75,    69,    80,
      -1,    -1,    21,    79,    87,    70,    80,    -1,    18,    79,
      76,    25,    76,    -1,    19,    79,    78,    25,    78,    -1,
      20,    79,    75,    25,    75,    -1,    21,    79,    77,    25,
      77,    -1,     3,    -1,     3,    -1,     3,    -1,     3,    -1,
       3,    -1,     3,    -1,     3,    -1,     3,    -1,     3,    -1,
      81,    -1,    82,    -1,    83,    -1,    84,    -1,     4,    -1,
       8,    -1,     9,    -1,    81,    -1,    27,    28,    -1,    27,
      81,    28,    -1,    44,    -1,     6,    -1,    44,    -1,    27,
      28,    -1,    27,    85,    28,    -1,    44,    -1,    -1,    44,
      86,    85,    -1,     3,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  VrmlParser::yyprhs_[] =
  {
         0,     0,     3,     4,    10,    13,    14,    16,    18,    21,
      23,    26,    31,    38,    40,    43,    45,    48,    50,    53,
      55,    57,    59,    61,    63,    65,    67,    71,    74,    76,
      80,    82,    84,    86,    89,    91,    92,    93,    94,   106,
     110,   112,   115,   117,   121,   125,   130,   132,   137,   144,
     146,   149,   151,   155,   159,   163,   167,   176,   178,   179,
     180,   186,   191,   193,   196,   198,   200,   203,   205,   207,
     209,   215,   221,   227,   233,   234,   238,   242,   244,   246,
     250,   254,   255,   261,   262,   268,   274,   280,   286,   292,
     294,   296,   298,   300,   302,   304,   306,   308,   310,   312,
     314,   316,   318,   320,   322,   324,   326,   329,   333,   335,
     337,   339,   342,   346,   348,   349,   353
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  VrmlParser::yyrline_[] =
  {
         0,   100,   100,   100,   106,   106,   108,   110,   111,   112,
     114,   120,   135,   179,   180,   181,   183,   189,   190,   191,
     193,   201,   202,   203,   204,   206,   207,   218,   223,   224,
     226,   227,   229,   230,   231,   233,   244,   249,   233,   263,
     265,   266,   267,   270,   273,   276,   280,   281,   286,   290,
     291,   293,   295,   296,   297,   298,   300,   339,   341,   344,
     344,   379,   381,   382,   383,   385,   386,   387,   389,   390,
     391,   392,   393,   395,   397,   397,   405,   408,   409,   410,
     413,   416,   416,   426,   426,   436,   440,   444,   448,   453,
     455,   457,   459,   461,   463,   465,   467,   469,   471,   475,
     479,   480,   482,   483,   484,   486,   487,   488,   491,   495,
     500,   504,   509,   511,   515,   515,   520
  };

  // Print the state stack on the debug stream.
  void
  VrmlParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
   i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  VrmlParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
         << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
           yyrhs_[yyprhs_[yyrule] + yyi],
           &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
           &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  VrmlParser::token_number_type
  VrmlParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    26,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    27,     2,    28,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,    30,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int VrmlParser::yyeof_ = 0;
  const int VrmlParser::yylast_ = 233;
  const int VrmlParser::yynnts_ = 57;
  const int VrmlParser::yyempty_ = -2;
  const int VrmlParser::yyfinal_ = 3;
  const int VrmlParser::yyterror_ = 1;
  const int VrmlParser::yyerrcode_ = 256;
  const int VrmlParser::yyntokens_ = 31;

  const unsigned int VrmlParser::yyuser_token_number_max_ = 280;
  const VrmlParser::token_number_type VrmlParser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 1054 of lalr1.cc  */
#line 1650 "vrml.cpp"


/* Line 1056 of lalr1.cc  */
#line 522 "vrml.bison"



void yy::VrmlParser::error( const yy::VrmlParser::location_type& l,
                            const std::string& m ) {
  driver.error( l, m );
}

int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver) {
   return driver.lexer->yylex(yylval, yylloc, driver);
}

