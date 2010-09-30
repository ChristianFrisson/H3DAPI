/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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


#include "vrml.hpp"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc.  */
#line 43 "vrml.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
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

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace yy
{
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
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
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
#endif /* ! YYDEBUG */

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


  int
  VrmlParser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    #line 76 "vrml.bison"
{
  // Initialize the initial location.
  yylloc.begin.filename = yylloc.end.filename = &driver.file;
}
  /* Line 555 of yacc.c.  */
#line 280 "vrml.cpp"
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
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
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

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
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
#line 100 "vrml.bison"
    { driver.root.reset( new Group() ); ;}
    break;

  case 13:
#line 120 "vrml.bison"
    {
  Node *node= driver.DEF_map->getNode( (yysemantic_stack_[(4) - (2)]) );
  if ( node ) {
    if ( driver.DEF_export )
      driver.DEF_export->addNode( (yysemantic_stack_[(4) - (4)]), node );
    else
      Console(3) << "WARNING: EXPORT error. No export DEF map provided!"
                 << driver.getLocationString() << endl;
  } else {
    Console(3) << "WARNING: EXPORT error. Node named \"" 
               << (yysemantic_stack_[(4) - (2)]) << "\" does not exist."
               << driver.getLocationString() << endl;
  }
;}
    break;

  case 14:
#line 136 "vrml.bison"
    {
if( !driver.insideProtoDeclaration() ) {
  if(  (yysemantic_stack_[(6) - (2)]) == "H3D_EXPORTS" ) {
    Node *import_node = 
      H3DExports::getH3DExportNode( (yysemantic_stack_[(6) - (4)]) );
    if( import_node ) {
      driver.DEF_map->addNode( (yysemantic_stack_[(6) - (6)]), import_node ); 
    } else {
      Console(3) << "WARNING: IMPORT error. H3D_EXPORTS " 
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
        Console(3) << "WARNING: IMPORT error. Inline node \"" 
                   << (yysemantic_stack_[(6) - (2)]) << "\" does not EXPORT \""
                   << (yysemantic_stack_[(6) - (4)]) << "\"" 
                   << driver.getLocationString() << endl;
      }
    } else {
      if( n ) {
        Console(3) << "WARNING: IMPORT error. Node \"" 
                   << (yysemantic_stack_[(6) - (2)]) << "\" is not an Inline node "
                   << driver.getLocationString() << endl;
      } else {
        Console(3) << "WARNING: IMPORT error. Node named \"" 
                   << (yysemantic_stack_[(6) - (2)]) << "\" does not exist."
                   << driver.getLocationString() << endl;
      }
    }
  }
}
;}
    break;

  case 24:
#line 193 "vrml.bison"
    {
                           if ( !driver.insideProtoDeclaration() ) {
                              Node *node = driver.node_stack.back();
                              driver.node_stack.pop_back();
                              if ( node ) 
                                driver.root->children->push_back( node );
                           }
                         ;}
    break;

  case 30:
#line 207 "vrml.bison"
    {
  if ( !driver.insideProtoDeclaration() &&
       driver.node_stack.back() ) {
    driver.DEF_map->addNode( (yysemantic_stack_[(3) - (2)]), driver.node_stack.back() );
    driver.node_stack.back()->setName( (yysemantic_stack_[(3) - (2)]) );
  } 
;}
    break;

  case 31:
#line 214 "vrml.bison"
    {
  if ( !driver.insideProtoDeclaration() )
    driver.node_stack.push_back( driver.DEF_map->getNode( (yysemantic_stack_[(2) - (2)]) ) );
  ;}
    break;

  case 39:
#line 229 "vrml.bison"
    {
   if ( driver.proto_vector ) {
     driver.proto_declarations.push_back( new ProtoDeclaration( (yysemantic_stack_[(2) - (2)]) ) );

     // only process proto declaration if not nested in another proto declaration.
     if ( driver.insideTopMostProtoDeclaration() ) {
       driver.proto_body = "";
       driver.proto_vector->push_back( driver.proto_declarations.back() );
     }
   }
;}
    break;

  case 40:
#line 240 "vrml.bison"
    {  
    // if in top-most proto declaration, reset proto_body to start
    // collecting the string to use
    if ( driver.insideTopMostProtoDeclaration() ) driver.proto_body = ""; 
   ;}
    break;

  case 41:
#line 245 "vrml.bison"
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
;}
    break;

  case 47:
#line 266 "vrml.bison"
    {
   driver.setProtoField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INPUT_ONLY );
;}
    break;

  case 48:
#line 269 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::OUTPUT_ONLY );
;}
    break;

  case 49:
#line 272 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[(4) - (3)]), (yysemantic_stack_[(4) - (2)]), Field::INITIALIZE_ONLY, (yysemantic_stack_[(4) - (4)]) );
;}
    break;

  case 51:
#line 277 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[(4) - (3)]), (yysemantic_stack_[(4) - (2)]), Field::INPUT_OUTPUT, (yysemantic_stack_[(4) - (4)]) );
;}
    break;

  case 60:
#line 297 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() ) {
  Node *fr = driver.DEF_map->getNode( (yysemantic_stack_[(8) - (2)]) );
  if ( fr ) {
    Field *frf = fr->getField( (yysemantic_stack_[(8) - (4)]) );
    if ( frf ) {
      Node *to = driver.DEF_map->getNode( (yysemantic_stack_[(8) - (6)]) );
      if ( to ) {
        Field *tof = to->getField( (yysemantic_stack_[(8) - (8)]) );
        if ( tof )
          frf->route( tof );
        else {
          Console(3) << "WARNING: Route error. Could not find field named \"" 
                     <<  (yysemantic_stack_[(8) - (8)])
                     << "\" in \"" << (yysemantic_stack_[(8) - (6)]) << "\" Node " 
                     << driver.getLocationString() << endl;
        }
      } else {
        Console(3) << "WARNING: Route error. Could not find destination Node named \"" 
                   << (yysemantic_stack_[(8) - (6)])
                   << "\" " 
                   << driver.getLocationString() << endl;
      }
    } else {
      Console(3) << "WARNING: Route error. Could not find field named \"" 
                 << (yysemantic_stack_[(8) - (4)])
                 << "\" in \"" << (yysemantic_stack_[(8) - (2)]) << "\" Node " 
                 << driver.getLocationString() << endl;
    }  
  } else {
    Console(3) << "WARNING: Route error. Could not find source Node named \"" 
               << (yysemantic_stack_[(8) - (2)])
               << "\" " 
               << driver.getLocationString() << endl;
  }
}
;}
    break;

  case 63:
#line 340 "vrml.bison"
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
        Console(3) << "WARNING: Could not create node \"" << yylval << 
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
        script_node->addNamedNodes( driver.DEF_map );
        driver.script_nodes.push_back( script_node );
      } 
    }
  }
  driver.node_stack.push_back( new_node ); 
  }
;}
    break;

  case 78:
#line 381 "vrml.bison"
    { 
			   if( !driver.insideProtoDeclaration() )	
                             driver.field_stack.push_back( (yysemantic_stack_[(1) - (1)]) );		
                         ;}
    break;

  case 79:
#line 385 "vrml.bison"
    {
                           if ( !driver.insideProtoDeclaration() )
                              driver.field_stack.pop_back();
                          ;}
    break;

  case 80:
#line 389 "vrml.bison"
    {
    driver.connectProtoField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (1)]) );
  /*if ( driver.proto_instance != NULL ) {
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
  }*/
;}
    break;

  case 83:
#line 413 "vrml.bison"
    {
                            driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INPUT_ONLY );		 
			 ;}
    break;

  case 84:
#line 416 "vrml.bison"
    {
                           driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::OUTPUT_ONLY );		 
                         ;}
    break;

  case 85:
#line 419 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() )	
                             driver.field_stack.push_back( (yysemantic_stack_[(3) - (3)]) );	
			   driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INITIALIZE_ONLY );
                         ;}
    break;

  case 86:
#line 424 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() ) {
			     driver.field_stack.pop_back();
                           }
                         ;}
    break;

  case 87:
#line 429 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() )	
                             driver.field_stack.push_back( (yysemantic_stack_[(3) - (3)]) );	
			   driver.addDynamicField( (yysemantic_stack_[(3) - (3)]), (yysemantic_stack_[(3) - (2)]), Field::INPUT_OUTPUT );
                         ;}
    break;

  case 88:
#line 434 "vrml.bison"
    {
                           if( !driver.insideProtoDeclaration() ) {
			     driver.field_stack.pop_back();
                           }
                         ;}
    break;

  case 89:
#line 439 "vrml.bison"
    {
                            driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::INPUT_ONLY );
                            driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
			 ;}
    break;

  case 90:
#line 443 "vrml.bison"
    {
                           driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::OUTPUT_ONLY );
                           driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
                         ;}
    break;

  case 91:
#line 447 "vrml.bison"
    {
			   driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::INITIALIZE_ONLY );
                           driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
                         ;}
    break;

  case 92:
#line 451 "vrml.bison"
    {
			   driver.addDynamicField( (yysemantic_stack_[(5) - (3)]), (yysemantic_stack_[(5) - (2)]), Field::INPUT_OUTPUT );
                           driver.connectProtoField( (yysemantic_stack_[(5) - (5)]), (yysemantic_stack_[(5) - (3)]) );
                         ;}
    break;

  case 102:
#line 474 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() )
  driver.setFieldValue( ((yysemantic_stack_[(1) - (1)])).c_str() );
;}
    break;

  case 103:
#line 478 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() )
  driver.setFieldValue( ((yysemantic_stack_[(1) - (1)])).c_str() );
                        ;}
    break;

  case 104:
#line 482 "vrml.bison"
    {;}
    break;

  case 105:
#line 483 "vrml.bison"
    {;}
    break;

  case 107:
#line 486 "vrml.bison"
    { (yyval) = "TRUE"; ;}
    break;

  case 108:
#line 487 "vrml.bison"
    { (yyval) = "FALSE";;}
    break;

  case 110:
#line 490 "vrml.bison"
    { (yyval) = ""; ;}
    break;

  case 111:
#line 491 "vrml.bison"
    { (yyval) = (yysemantic_stack_[(3) - (2)]); ;}
    break;

  case 112:
#line 494 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        ;}
    break;

  case 113:
#line 498 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 1 );
                        ;}
    break;

  case 114:
#line 503 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        ;}
    break;

  case 115:
#line 507 "vrml.bison"
    { 
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 1 );

                        ;}
    break;

  case 117:
#line 514 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        ;}
    break;

  case 118:
#line 518 "vrml.bison"
    {
if ( !driver.insideProtoDeclaration() )
  driver.setNodeStatement( 0 );
                        ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 851 "vrml.cpp"
	default: break;
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
	/* If just tried and failed to reuse look-ahead token after an
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

    /* Else will try to reuse look-ahead token after shifting the error
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
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

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

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
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
    if (yychar != yyeof_ && yychar != yyempty_)
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
  const short int VrmlParser::yypact_ninf_ = -132;
  const short int
  VrmlParser::yypact_[] =
  {
      -132,    19,    18,  -132,    32,    31,  -132,  -132,    38,    35,
      31,  -132,  -132,    40,    56,   204,    35,  -132,  -132,    60,
    -132,    63,  -132,    43,    65,    85,    65,    65,    86,    86,
      65,  -132,  -132,  -132,   204,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,   211,  -132,
      87,  -132,    64,    22,  -132,  -132,    69,    73,  -132,    71,
    -132,    99,    99,    99,    99,  -132,  -132,  -132,  -132,    74,
     211,  -132,    79,   103,   103,  -132,    89,    95,   119,   219,
    -132,   125,   119,   126,   130,  -132,  -132,   126,   174,  -132,
    -132,   129,   106,    99,    99,    99,    99,   107,    95,  -132,
    -132,   111,    99,    99,    99,    99,  -132,   108,   219,  -132,
     112,   115,   147,   116,   117,  -132,  -132,  -132,  -132,  -132,
    -132,    30,  -132,  -132,  -132,  -132,  -132,  -132,    65,    99,
      99,    99,    99,   114,  -132,   106,  -132,   125,   119,   126,
     141,    53,  -132,    65,   125,   119,   126,   130,  -132,  -132,
     125,   119,   126,   174,  -132,   146,   174,  -132,   123,   128,
     131,  -132,   125,   119,   126,   155,   134,  -132,  -132,  -132,
    -132,  -132,    46,  -132,  -132,  -132,   132,   136,   140,   142,
     144,  -132,  -132,  -132,  -132,  -132,  -132,  -132,    37,  -132,
    -132,  -132,  -132,   174,  -132,   174,  -132,  -132,   125,   125,
     119,   126,   146,  -132,  -132,    34,  -132,  -132,  -132,  -132,
    -132,    34,    83,  -132,  -132,  -132,    65,   204,  -132,   135,
      22,  -132,  -132,  -132
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  VrmlParser::yydefact_[] =
  {
         2,     0,     5,     1,     0,    62,     6,     4,     0,    62,
       7,     9,    11,     0,     0,    62,    15,    17,     8,     0,
      19,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     3,    21,    24,    27,    34,    35,    28,
      23,    29,    63,    16,    12,    10,    20,    18,    62,    93,
       0,    95,     0,     0,    31,    39,     0,     0,    22,     0,
      97,     0,     0,     0,     0,    82,    73,    81,    71,     0,
      62,    72,    78,     0,     0,    30,     0,    62,     0,    62,
     101,     0,     0,     0,     0,    65,    70,     0,     0,    96,
      13,     0,    62,     0,     0,     0,     0,     0,    53,    55,
     100,     0,     0,     0,     0,     0,    68,     0,    62,    98,
      47,    48,     0,   120,     0,    87,    80,   106,   113,   107,
     108,     0,   112,    79,   102,   103,   104,   105,     0,     0,
       0,     0,     0,     0,    50,    44,    46,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,    64,    67,
       0,     0,     0,     0,    49,     0,     0,   110,   118,     0,
       0,    14,     0,     0,     0,     0,     0,    45,    56,    57,
      58,    59,     0,    52,   109,    61,     0,    83,    84,    85,
       0,    74,    75,    76,    86,    99,    77,    88,     0,   111,
     116,    47,    48,     0,   120,     0,    40,   110,     0,     0,
       0,     0,     0,   119,    51,    62,    60,    89,    90,    91,
      92,    36,     0,    41,    38,    37,     0,    62,    32,     0,
       0,    43,    42,    33
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  VrmlParser::yypgoto_[] =
  {
      -132,  -132,  -132,  -132,  -132,   160,  -132,  -132,  -132,  -132,
    -132,   157,  -132,  -132,  -132,   -26,  -132,   -14,  -132,   -13,
     -40,  -132,  -132,  -132,  -132,  -132,    44,   -77,  -132,  -132,
      88,  -132,   -12,  -132,    -5,   -44,  -132,    76,   118,  -132,
     -61,  -132,  -132,  -132,   -20,    -2,  -132,   113,   -70,  -114,
    -131,   -68,    16,   -57,  -109,    52,  -132,  -132,    -3,  -132,
      29
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  VrmlParser::yydefgoto_[] =
  {
        -1,     1,     2,     5,     7,     9,    10,    13,    45,    31,
      32,    15,    16,    21,    47,    33,    34,   122,   217,    65,
     212,    37,    76,   205,   219,   213,   133,    66,   135,    38,
      97,    98,    67,   173,    40,    41,    59,   107,    69,    70,
      71,    88,   153,   156,    50,    42,    52,    90,    72,   110,
     114,   101,    81,   154,   124,   125,   126,   127,   160,   188,
     115
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char VrmlParser::yytable_ninf_ = -118;
  const short int
  VrmlParser::yytable_[] =
  {
        11,    35,    36,    39,    17,    11,    53,    54,    58,    75,
      57,    17,   159,   112,   111,   134,   180,   116,   108,     3,
      35,    36,    39,   168,   186,    22,    55,    56,     4,    23,
     177,   123,   174,    22,   117,     6,   181,    23,   119,   120,
      22,    12,     8,    68,    23,    26,    27,   108,   191,    14,
     117,    28,    26,    27,   119,   120,    29,   117,   134,   157,
      20,   119,   120,   159,    44,    68,    19,    46,    49,   170,
     169,   210,    99,    48,   106,   197,   179,   178,    82,    83,
      84,   172,   183,   182,   206,   207,    22,   136,    51,    22,
      23,    74,    73,    99,   193,   192,   184,    77,   216,   187,
      78,    79,    80,   106,    87,    85,    89,   158,   161,   137,
     138,   139,   140,    93,    94,    95,    96,    92,   144,   145,
     146,   147,   100,   176,   129,   130,   131,   132,   109,    60,
     136,   209,   208,   113,   128,   143,   141,   150,   204,   148,
     151,   -99,   155,   166,   171,   162,   163,   164,   165,   185,
      22,   117,  -117,   118,    23,   119,   120,   189,   194,   198,
     190,   199,    26,    27,   196,   200,   222,   201,   218,   202,
      18,   215,   152,    43,   158,   121,   223,    22,   117,   167,
     118,    23,   119,   120,   149,   203,   142,    91,    86,    26,
      27,   221,   211,   175,   195,     0,   220,     0,   211,     0,
     214,     0,   121,    35,    36,    39,   214,    22,     0,     0,
       0,    23,     0,     0,    60,     0,    24,    25,     0,    26,
      27,    28,    60,     0,     0,     0,    29,    30,    28,    61,
      62,    63,    64,    29,    30,     0,    28,   102,   103,   104,
     105,    29,    30
  };

  /* YYCHECK.  */
  const short int
  VrmlParser::yycheck_[] =
  {
         5,    15,    15,    15,     9,    10,    26,    27,    34,    53,
      30,    16,   121,    83,    82,    92,   147,    87,    79,     0,
      34,    34,    34,   137,   155,     3,    28,    29,    10,     7,
     144,    88,   141,     3,     4,     3,   150,     7,     8,     9,
       3,     3,    11,    48,     7,    15,    16,   108,   162,    14,
       4,    17,    15,    16,     8,     9,    22,     4,   135,    29,
       4,     8,     9,   172,     4,    70,    26,     4,     3,   139,
     138,   202,    77,    30,    79,    29,   146,   145,    62,    63,
      64,    28,   152,   151,   198,   199,     3,    92,     3,     3,
       7,    27,     5,    98,   164,   163,   153,    28,    15,   156,
      27,    30,     3,   108,    25,    31,     3,   121,   128,    93,
      94,    95,    96,    18,    19,    20,    21,    28,   102,   103,
     104,   105,     3,   143,    18,    19,    20,    21,     3,     3,
     135,   201,   200,     3,     5,    24,    29,    25,   195,    31,
      25,    25,    25,    29,     3,   129,   130,   131,   132,     3,
       3,     4,    29,     6,     7,     8,     9,    29,     3,    27,
      29,    25,    15,    16,    30,    25,    31,    25,   212,    25,
      10,   211,    25,    16,   188,    28,   220,     3,     4,   135,
       6,     7,     8,     9,   108,   188,    98,    74,    70,    15,
      16,   217,   205,   141,   165,    -1,   216,    -1,   211,    -1,
     205,    -1,    28,   217,   217,   217,   211,     3,    -1,    -1,
      -1,     7,    -1,    -1,     3,    -1,    12,    13,    -1,    15,
      16,    17,     3,    -1,    -1,    -1,    22,    23,    17,    18,
      19,    20,    21,    22,    23,    -1,    17,    18,    19,    20,
      21,    22,    23
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  VrmlParser::yystos_[] =
  {
         0,    33,    34,     0,    10,    35,     3,    36,    11,    37,
      38,    66,     3,    39,    14,    43,    44,    66,    37,    26,
       4,    45,     3,     7,    12,    13,    15,    16,    17,    22,
      23,    41,    42,    47,    48,    49,    51,    53,    61,    64,
      66,    67,    77,    43,     4,    40,     4,    46,    30,     3,
      76,     3,    78,    76,    76,    77,    77,    76,    47,    68,
       3,    18,    19,    20,    21,    51,    59,    64,    66,    70,
      71,    72,    80,     5,    27,    67,    54,    28,    27,    30,
       3,    84,    84,    84,    84,    31,    70,    25,    73,     3,
      79,    79,    28,    18,    19,    20,    21,    62,    63,    66,
       3,    83,    18,    19,    20,    21,    66,    69,    72,     3,
      81,    83,    80,     3,    82,    92,    80,     4,     6,     8,
       9,    28,    49,    85,    86,    87,    88,    89,     5,    18,
      19,    20,    21,    58,    59,    60,    66,    84,    84,    84,
      84,    29,    62,    24,    84,    84,    84,    84,    31,    69,
      25,    25,    25,    74,    85,    25,    75,    29,    49,    86,
      90,    76,    84,    84,    84,    84,    29,    58,    81,    83,
      80,     3,    28,    65,    86,    87,    76,    81,    83,    80,
      82,    81,    83,    80,    85,     3,    82,    85,    91,    29,
      29,    81,    83,    80,     3,    92,    30,    29,    27,    25,
      25,    25,    25,    90,    85,    55,    81,    81,    83,    80,
      82,    51,    52,    57,    66,    52,    15,    50,    67,    56,
      76,    47,    31,    67
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  VrmlParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    58,    46,    91,    93,
     123,   125
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  VrmlParser::yyr1_[] =
  {
         0,    32,    34,    33,    35,    35,    36,    37,    37,    37,
      38,    39,    40,    41,    42,    43,    43,    43,    44,    45,
      46,    47,    47,    47,    48,    48,    48,    48,    48,    49,
      49,    49,    50,    50,    51,    51,    52,    52,    52,    54,
      55,    56,    53,    57,    58,    58,    58,    59,    59,    59,
      60,    60,    61,    62,    62,    62,    63,    63,    63,    63,
      64,    65,    66,    68,    67,    67,    69,    69,    69,    70,
      70,    70,    71,    71,    71,    71,    71,    71,    73,    72,
      72,    72,    72,    72,    72,    74,    72,    75,    72,    72,
      72,    72,    72,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    85,    85,    85,    86,    86,    86,    87,
      87,    87,    88,    88,    89,    89,    89,    90,    91,    90,
      92
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  VrmlParser::yyr2_[] =
  {
         0,     2,     0,     5,     2,     0,     1,     1,     2,     1,
       4,     1,     1,     4,     6,     1,     2,     1,     3,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     1,     3,     1,     1,     1,     2,     1,     0,
       0,     0,    11,     3,     1,     2,     1,     3,     3,     4,
       1,     4,     6,     1,     2,     1,     3,     3,     3,     3,
       8,     1,     0,     0,     5,     4,     1,     2,     1,     1,
       2,     1,     1,     1,     5,     5,     5,     5,     0,     3,
       3,     1,     1,     3,     3,     0,     5,     0,     5,     5,
       5,     5,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     1,     1,     1,     2,     3,     1,     0,     3,
       1
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
  "EXTERNPROTO", "ROUTE", "TO", "IS", "':'", "'.'", "'['", "']'", "'{'",
  "'}'", "$accept", "x3dScene", "@1", "profileStatement", "profileNameId",
  "componentStatements", "componentStatement", "componentNameId",
  "componentSupportLevel", "exportStatement", "importStatement",
  "metaStatements", "metaStatement", "metakey", "metavalue", "statements",
  "statement", "nodeStatement", "rootNodeStatement", "protoStatement",
  "protoStatements", "proto", "@2", "@3", "@4", "protoBody",
  "interfaceDeclarations", "restrictedInterfaceDeclaration",
  "interfaceDeclaration", "externproto", "externInterfaceDeclarations",
  "externInterfaceDeclaration", "routeStatement", "URLList", "empty",
  "node", "@5", "nodeBody", "scriptBody", "scriptBodyElement",
  "nodeBodyElement", "@6", "@7", "@8", "nodeNameId", "nodeTypeId",
  "inlineNodeNameId", "exportedNodeNameId", "initializeOnlyId",
  "inputOnlyId", "inputOutputId", "outputOnlyId", "fieldType",
  "fieldValue", "sfValue", "mfValue", "sfnodeValue", "mfnodeValue",
  "nodeStatements", "@9", "fieldId", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const VrmlParser::rhs_number_type
  VrmlParser::yyrhs_[] =
  {
        33,     0,    -1,    -1,    34,    35,    37,    43,    47,    -1,
      10,    36,    -1,    -1,     3,    -1,    38,    -1,    38,    37,
      -1,    66,    -1,    11,    39,    26,    40,    -1,     3,    -1,
       4,    -1,    12,    76,     5,    79,    -1,    13,    78,    27,
      79,     5,    76,    -1,    44,    -1,    44,    43,    -1,    66,
      -1,    14,    45,    46,    -1,     4,    -1,     4,    -1,    48,
      -1,    48,    47,    -1,    66,    -1,    49,    -1,    41,    -1,
      42,    -1,    51,    -1,    64,    -1,    67,    -1,    15,    76,
      67,    -1,    16,    76,    -1,    67,    -1,    15,    76,    67,
      -1,    53,    -1,    61,    -1,    51,    -1,    51,    52,    -1,
      66,    -1,    -1,    -1,    -1,    17,    77,    54,    28,    58,
      29,    30,    55,    57,    56,    31,    -1,    52,    50,    47,
      -1,    60,    -1,    60,    58,    -1,    66,    -1,    18,    84,
      81,    -1,    19,    84,    83,    -1,    20,    84,    80,    85,
      -1,    59,    -1,    21,    84,    92,    85,    -1,    22,    77,
      28,    62,    29,    65,    -1,    63,    -1,    63,    62,    -1,
      66,    -1,    18,    84,    81,    -1,    19,    84,    83,    -1,
      20,    84,    80,    -1,    21,    84,     3,    -1,    23,    76,
      27,    83,    24,    76,    27,    81,    -1,    87,    -1,    -1,
      -1,    77,    68,    30,    69,    31,    -1,     7,    30,    70,
      31,    -1,    72,    -1,    72,    69,    -1,    66,    -1,    71,
      -1,    71,    70,    -1,    66,    -1,    72,    -1,    59,    -1,
      18,    84,    81,    25,    81,    -1,    19,    84,    83,    25,
      83,    -1,    20,    84,    80,    25,    80,    -1,    21,    84,
      82,    25,    82,    -1,    -1,    80,    73,    85,    -1,    80,
      25,    80,    -1,    64,    -1,    51,    -1,    18,    84,    81,
      -1,    19,    84,    83,    -1,    -1,    20,    84,    80,    74,
      85,    -1,    -1,    21,    84,    92,    75,    85,    -1,    18,
      84,    81,    25,    81,    -1,    19,    84,    83,    25,    83,
      -1,    20,    84,    80,    25,    80,    -1,    21,    84,    82,
      25,    82,    -1,     3,    -1,     3,    -1,     3,    -1,     3,
      -1,     3,    -1,     3,    -1,     3,    -1,     3,    -1,     3,
      -1,    86,    -1,    87,    -1,    88,    -1,    89,    -1,     4,
      -1,     8,    -1,     9,    -1,    86,    -1,    28,    29,    -1,
      28,    86,    29,    -1,    49,    -1,     6,    -1,    49,    -1,
      28,    29,    -1,    28,    90,    29,    -1,    49,    -1,    -1,
      49,    91,    90,    -1,     3,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  VrmlParser::yyprhs_[] =
  {
         0,     0,     3,     4,    10,    13,    14,    16,    18,    21,
      23,    28,    30,    32,    37,    44,    46,    49,    51,    55,
      57,    59,    61,    64,    66,    68,    70,    72,    74,    76,
      78,    82,    85,    87,    91,    93,    95,    97,   100,   102,
     103,   104,   105,   117,   121,   123,   126,   128,   132,   136,
     141,   143,   148,   155,   157,   160,   162,   166,   170,   174,
     178,   187,   189,   190,   191,   197,   202,   204,   207,   209,
     211,   214,   216,   218,   220,   226,   232,   238,   244,   245,
     249,   253,   255,   257,   261,   265,   266,   272,   273,   279,
     285,   291,   297,   303,   305,   307,   309,   311,   313,   315,
     317,   319,   321,   323,   325,   327,   329,   331,   333,   335,
     337,   340,   344,   346,   348,   350,   353,   357,   359,   360,
     364
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  VrmlParser::yyrline_[] =
  {
         0,   100,   100,   100,   106,   106,   108,   110,   111,   112,
     114,   116,   118,   120,   135,   179,   180,   181,   183,   185,
     187,   189,   190,   191,   193,   201,   202,   203,   204,   206,
     207,   214,   219,   220,   222,   223,   225,   226,   227,   229,
     240,   245,   229,   259,   261,   262,   263,   266,   269,   272,
     276,   277,   282,   286,   287,   289,   291,   292,   293,   294,
     296,   335,   337,   340,   340,   363,   365,   366,   367,   369,
     370,   371,   373,   374,   375,   376,   377,   379,   381,   381,
     389,   411,   412,   413,   416,   419,   419,   429,   429,   439,
     443,   447,   451,   456,   458,   460,   462,   464,   466,   468,
     470,   472,   474,   478,   482,   483,   485,   486,   487,   489,
     490,   491,   494,   498,   503,   507,   512,   514,   518,   518,
     523
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
	       << " (line " << yylno << "), ";
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
       2,     2,     2,     2,     2,     2,    27,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    26,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    28,     2,    29,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
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
  const int VrmlParser::yylast_ = 242;
  const int VrmlParser::yynnts_ = 61;
  const int VrmlParser::yyempty_ = -2;
  const int VrmlParser::yyfinal_ = 3;
  const int VrmlParser::yyterror_ = 1;
  const int VrmlParser::yyerrcode_ = 256;
  const int VrmlParser::yyntokens_ = 32;

  const unsigned int VrmlParser::yyuser_token_number_max_ = 280;
  const VrmlParser::token_number_type VrmlParser::yyundef_token_ = 2;

} // namespace yy

#line 525 "vrml.bison"



void yy::VrmlParser::error( const yy::VrmlParser::location_type& l,
                            const std::string& m ) {
  driver.error( l, m );
}

int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver) {
   return driver.lexer->yylex(yylval, yylloc, driver);
}

