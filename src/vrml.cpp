/* A Bison parser, made by GNU Bison 2.1.  */

/* C++ Skeleton parser for LALR(1) parsing with Bison,
   Copyright (C) 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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


#include "vrml.hpp"

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

/* A pseudo ostream that takes yydebug_ into account. */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << (Title) << ' ';			\
      yysymprint_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (0)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yyreduce_print_ (Rule);		\
} while (0)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (0)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

#if YYERROR_VERBOSE

/* Return YYSTR after stripping away unnecessary quotes and
   backslashes, so that it's suitable for yyerror.  The heuristic is
   that double-quoting is unnecessary unless the string contains an
   apostrophe, a comma, or backslash (other than backslash-backslash).
   YYSTR is taken from yytname.  */
std::string
yy::VrmlParser::yytnamerr_ (const char *yystr)
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

#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

void
yy::VrmlParser::yysymprint_ (int yytype,
                         const semantic_type* yyvaluep, const location_type* yylocationp)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;
  /* Backward compatibility, but should be removed eventually. */
  std::ostream& cdebug_ = *yycdebug_;
  (void) cdebug_;

  *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	     << ' ' << yytname_[yytype] << " ("
             << *yylocationp << ": ";
  switch (yytype)
    {
      default:
        break;
    }
  *yycdebug_ << ')';
}
#endif /* ! YYDEBUG */

void
yy::VrmlParser::yydestruct_ (const char* yymsg,
                         int yytype, semantic_type* yyvaluep, location_type* yylocationp)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yymsg;
  (void) yyvaluep;
  (void) yylocationp;

  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}

void
yy::VrmlParser::yypop_ (unsigned int n)
{
  yystate_stack_.pop (n);
  yysemantic_stack_.pop (n);
  yylocation_stack_.pop (n);
}

std::ostream&
yy::VrmlParser::debug_stream () const
{
  return *yycdebug_;
}

void
yy::VrmlParser::set_debug_stream (std::ostream& o)
{
  yycdebug_ = &o;
}


yy::VrmlParser::debug_level_type
yy::VrmlParser::debug_level () const
{
  return yydebug_;
}

void
yy::VrmlParser::set_debug_level (debug_level_type l)
{
  yydebug_ = l;
}


int
yy::VrmlParser::parse ()
{
  /* Look-ahead and look-ahead in internal form.  */
  int yylooka;
  int yyilooka;

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

  int yyresult_;

  YYCDEBUG << "Starting parse" << std::endl;

  yynerrs_ = 0;
  yyerrstatus_ = 0;

  /* Start.  */
  yystate_ = 0;
  yylooka = yyempty_;


  /* User initialization code. */
  #line 75 "vrml.bison"
{
  // Initialize the initial location.
  yylloc.begin.filename = yylloc.end.filename = &driver.file;
}
/* Line 548 of yacc.c.  */
#line 239 "vrml.cpp"
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
  yystate_stack_.push (yystate_);
  YYCDEBUG << "Entering state " << yystate_ << std::endl;
  goto yybackup;

  /* Backup.  */
yybackup:

  /* Try to take a decision without look-ahead.  */
  yyn_ = yypact_[yystate_];
  if (yyn_ == yypact_ninf_)
    goto yydefault;

  /* Read a look-ahead token.  */
  if (yylooka == yyempty_)
    {
      YYCDEBUG << "Reading a token: ";
      yylooka = yylex (&yylval, &yylloc, driver);
    }


  /* Convert token to internal form.  */
  if (yylooka <= yyeof_)
    {
      yylooka = yyilooka = yyeof_;
      YYCDEBUG << "Now at end of input." << std::endl;
    }
  else
    {
      yyilooka = yytranslate_ (yylooka);
      YY_SYMBOL_PRINT ("Next token is", yyilooka, &yylval, &yylloc);
    }

  /* If the proper action on seeing token ILOOKA_ is to reduce or to
     detect an error, take that action.  */
  yyn_ += yyilooka;
  if (yyn_ < 0 || yylast_ < yyn_ || yycheck_[yyn_] != yyilooka)
    goto yydefault;

  /* Reduce or error.  */
  yyn_ = yytable_[yyn_];
  if (yyn_ < 0)
    {
      if (yyn_ == yytable_ninf_)
	goto yyerrlab;
      else
	{
	  yyn_ = -yyn_;
	  goto yyreduce;
	}
    }
  else if (yyn_ == 0)
    goto yyerrlab;

  /* Accept?  */
  if (yyn_ == yyfinal_)
    goto yyacceptlab;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yyilooka, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yylooka != yyeof_)
    yylooka = yyempty_;

  yysemantic_stack_.push (yylval);
  yylocation_stack_.push (yylloc);

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus_)
    --yyerrstatus_;

  yystate_ = yyn_;
  goto yynewstate;

/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn_ = yydefact_[yystate_];
  if (yyn_ == 0)
    goto yyerrlab;
  goto yyreduce;

/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  yylen_ = yyr2_[yyn_];
  /* If LEN_ is nonzero, implement the default value of the action:
     `$$ = $1'.  Otherwise, use the top of the stack.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  */
  if (yylen_)
    yyval = yysemantic_stack_[yylen_ - 1];
  else
    yyval = yysemantic_stack_[0];

  {
    slice<location_type, location_stack_type> slice (yylocation_stack_, yylen_);
    YYLLOC_DEFAULT (yyloc, slice, yylen_);
  }
  YY_REDUCE_PRINT (yyn_);
  switch (yyn_)
    {
        case 2:
#line 110 "vrml.bison"
    { driver.root = new Group(); ;}
    break;

  case 13:
#line 130 "vrml.bison"
    {
  Node *node= driver.DEF_map->getNode( (yysemantic_stack_[2].val) );
  if ( node ) {
    if ( driver.DEF_export )
      driver.DEF_export->addNode( (yysemantic_stack_[0].val), node );
    else
      Console(3) << "WARNING: EXPORT error. No export DEF map provided!"
                 << driver.getLocationString() << endl;
  } else {
    Console(3) << "WARNING: EXPORT error. Node named \"" 
               << (yysemantic_stack_[2].val) << "\" does not exist."
               << driver.getLocationString() << endl;
  }
;}
    break;

  case 14:
#line 146 "vrml.bison"
    {
if( driver.proto_declarations.size()==0 ) {
  if( strcmp( (yysemantic_stack_[4].val), "H3D_EXPORTS" ) == 0 ) {
    Node *import_node = 
      H3DExports::getH3DExportNode( (yysemantic_stack_[2].val) );
    if( import_node ) {
      driver.DEF_map->addNode( (yysemantic_stack_[0].val), import_node ); 
    } else {
      Console(3) << "WARNING: IMPORT error. H3D_EXPORTS " 
                 << "does not include \""
                 << (yysemantic_stack_[2].val) << "\"" 
                 << driver.getLocationString() << endl;
    }
  } else {
    Node *n = driver.DEF_map->getNode( (yysemantic_stack_[4].val) );
    Inline *inline_node = dynamic_cast< Inline * >( n );
    if( inline_node ) {
      Node *import_node = 
        inline_node->exported_nodes.getNode( (yysemantic_stack_[2].val) );
      if( import_node ) {
        driver.DEF_map->addNode( (yysemantic_stack_[0].val), 
                          import_node ); 
      } else {
        Console(3) << "WARNING: IMPORT error. Inline node \"" 
                   << (yysemantic_stack_[4].val) << "\" does not EXPORT \""
                   << (yysemantic_stack_[2].val) << "\"" 
                   << driver.getLocationString() << endl;
      }
    } else {
      if( n ) {
        Console(3) << "WARNING: IMPORT error. Node \"" 
                   << (yysemantic_stack_[4].val) << "\" is not an Inline node "
                   << driver.getLocationString() << endl;
      } else {
        Console(3) << "WARNING: IMPORT error. Node named \"" 
                   << (yysemantic_stack_[4].val) << "\" does not exist."
                   << driver.getLocationString() << endl;
      }
    }
  }
}
;}
    break;

  case 24:
#line 203 "vrml.bison"
    {
                           if ( driver.proto_declarations.size()==0 ) {
                              Node *node = driver.node_stack.back();
                              driver.node_stack.pop_back();
                              driver.root->children->push_back( node );
                           }
                         ;}
    break;

  case 30:
#line 216 "vrml.bison"
    {
  if ( driver.proto_declarations.size()==0 )
    driver.DEF_map->addNode( (yysemantic_stack_[1].val), driver.node_stack.back() );
  ;}
    break;

  case 31:
#line 220 "vrml.bison"
    {
  if ( driver.proto_declarations.size()==0 )
    driver.node_stack.push_back( driver.DEF_map->getNode( (yysemantic_stack_[0].val) ) );
  ;}
    break;

  case 39:
#line 235 "vrml.bison"
    {
   if ( driver.proto_vector ) {
     driver.proto_declarations.push_back( new ProtoDeclaration( (yysemantic_stack_[0].val) ) );
     if ( driver.proto_declarations.size()==1 ) {
       driver.proto_body = "";
       driver.proto_vector->push_back( driver.proto_declarations.back() );
     }
   }
;}
    break;

  case 40:
#line 244 "vrml.bison"
    { if ( driver.proto_declarations.size()==1 ) driver.proto_body = ""; ;}
    break;

  case 41:
#line 245 "vrml.bison"
    {
   if ( driver.proto_declarations.size()==1 ) {
     // remove trailing '}'
     int pos = driver.proto_body.find_last_of( "}" );
     driver.proto_body[pos]=' ';
     driver.proto_declarations.back()->setProtoBody( driver.proto_body );
   }
   driver.proto_declarations.pop_back();
;}
    break;

  case 47:
#line 263 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[0].val), (yysemantic_stack_[1].val), Field::INPUT_ONLY, NULL );
;}
    break;

  case 48:
#line 266 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[0].val), (yysemantic_stack_[1].val), Field::OUTPUT_ONLY, NULL );
;}
    break;

  case 49:
#line 269 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[1].val), (yysemantic_stack_[2].val), Field::INITIALIZE_ONLY, (yysemantic_stack_[0].val) );
;}
    break;

  case 51:
#line 274 "vrml.bison"
    {
  driver.setProtoField( (yysemantic_stack_[1].val), (yysemantic_stack_[2].val), Field::INPUT_OUTPUT, (yysemantic_stack_[0].val) );
;}
    break;

  case 60:
#line 294 "vrml.bison"
    {
if ( driver.proto_declarations.size()==0 ) {
  Node *fr = driver.DEF_map->getNode( (yysemantic_stack_[6].val) );
  if ( fr ) {
    Field *frf = fr->getField( (yysemantic_stack_[4].val) );
    if ( frf ) {
      Node *to = driver.DEF_map->getNode( (yysemantic_stack_[2].val) );
      if ( to ) {
        Field *tof = to->getField( (yysemantic_stack_[0].val) );
        if ( tof )
          frf->route( tof );
        else {
          Console(3) << "WARNING: Route error. Could not find field named \"" 
                     <<  (yysemantic_stack_[0].val)
                     << "\" in \"" << (yysemantic_stack_[2].val) << "\" Node " 
                     << driver.getLocationString() << endl;
        }
      } else {
        Console(3) << "WARNING: Route error. Could not find destination Node named \"" 
                   << (yysemantic_stack_[2].val)
                   << "\" " 
                   << driver.getLocationString() << endl;
      }
    } else {
      Console(3) << "WARNING: Route error. Could not find field named \"" 
                 << (yysemantic_stack_[4].val)
                 << "\" in \"" << (yysemantic_stack_[6].val) << "\" Node " 
                 << driver.getLocationString() << endl;
    }  
  } else {
    Console(3) << "WARNING: Route error. Could not find source Node named \"" 
               << (yysemantic_stack_[6].val)
               << "\" " 
               << driver.getLocationString() << endl;
  }
}
;}
    break;

  case 63:
#line 337 "vrml.bison"
    { 
if ( driver.proto_declarations.size()==0 ) {
  Node *new_node =  H3DNodeDatabase::createNode( yylval.val );
  if ( !new_node ) {
    // try as a proto:
    ProtoDeclaration *proto = driver.proto_vector->getProtoDeclaration(
    yylval.val );
    if ( proto ) {
       new_node = proto->newProtoInstance();
    }
    if ( !new_node )
      Console(3) << "WARNING: Could not create node \"" << yylval.val << 
        "\" - name not found in the node database." << endl;
  }
  if ( new_node )
    driver.node_stack.push_back( new_node );
}
;}
    break;

  case 78:
#line 374 "vrml.bison"
    { 
                            driver.field_stack.push_back( (yysemantic_stack_[0].val) ); ;}
    break;

  case 79:
#line 376 "vrml.bison"
    {
                           if ( driver.proto_declarations.size()==0 )
                                driver.field_stack.pop_back();
                            ;}
    break;

  case 80:
#line 380 "vrml.bison"
    {
  if ( driver.proto_instance != NULL ) {
    Node *node = driver.node_stack.back();
    Field *node_field = node->getField( (yysemantic_stack_[2].val) );
    Field *proto_field =  
        driver.proto_instance->getField( (yysemantic_stack_[0].val) );
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
;}
    break;

  case 93:
#line 424 "vrml.bison"
    { 
if ( driver.proto_declarations.size()==0 )
  driver.setFieldValue( (yysemantic_stack_[0].val) );
;}
    break;

  case 94:
#line 428 "vrml.bison"
    { 
if ( driver.proto_declarations.size()==0 )
  driver.setFieldValue( (yysemantic_stack_[0].val) );
                        ;}
    break;

  case 95:
#line 432 "vrml.bison"
    {;}
    break;

  case 96:
#line 433 "vrml.bison"
    {;}
    break;

  case 98:
#line 436 "vrml.bison"
    { (yyval.val) = "TRUE"; ;}
    break;

  case 99:
#line 437 "vrml.bison"
    { (yyval.val) = "FALSE";;}
    break;

  case 101:
#line 440 "vrml.bison"
    { (yyval.val) = ""; ;}
    break;

  case 102:
#line 441 "vrml.bison"
    { (yyval.val) = (yysemantic_stack_[1].val); ;}
    break;

  case 103:
#line 444 "vrml.bison"
    {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        ;}
    break;

  case 104:
#line 448 "vrml.bison"
    {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 1 );
                        ;}
    break;

  case 105:
#line 453 "vrml.bison"
    {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        ;}
    break;

  case 106:
#line 457 "vrml.bison"
    { 
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 1 );

                        ;}
    break;

  case 108:
#line 464 "vrml.bison"
    {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        ;}
    break;

  case 109:
#line 468 "vrml.bison"
    {
if ( driver.proto_declarations.size()==0 )
  driver.setNodeStatement( 0 );
                        ;}
    break;


      default: break;
    }

/* Line 676 of lalr1.cc.  */
#line 716 "vrml.cpp"

  yypop_ (yylen_);

  YY_STACK_PRINT ();

  yysemantic_stack_.push (yyval);
  yylocation_stack_.push (yyloc);

  /* Shift the result of the reduction.  */
  yyn_ = yyr1_[yyn_];
  yystate_ = yypgoto_[yyn_ - yyntokens_] + yystate_stack_[0];
  if (0 <= yystate_ && yystate_ <= yylast_
      && yycheck_[yystate_] == yystate_stack_[0])
    yystate_ = yytable_[yystate_];
  else
    yystate_ = yydefgoto_[yyn_ - yyntokens_];
  goto yynewstate;

/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus_)
    {
      ++yynerrs_;
      error (yylloc, yysyntax_error_ (YYERROR_VERBOSE_IF (yyilooka)));
    }

  yyerror_range[0] = yylloc;
  if (yyerrstatus_ == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yylooka <= yyeof_)
        {
	  /* Return failure if at end of input.  */
	  if (yylooka == yyeof_)
	    YYABORT;
        }
      else
        {
          yydestruct_ ("Error: discarding", yyilooka, &yylval, &yylloc);
          yylooka = yyempty_;
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

  yyerror_range[0] = yylocation_stack_[yylen_ - 1];
  yypop_ (yylen_);
  yystate_ = yystate_stack_[0];
  goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn_ = yypact_[yystate_];
      if (yyn_ != yypact_ninf_)
	{
	  yyn_ += yyterror_;
	  if (0 <= yyn_ && yyn_ <= yylast_ && yycheck_[yyn_] == yyterror_)
	    {
	      yyn_ = yytable_[yyn_];
	      if (0 < yyn_)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yystate_stack_.height () == 1)
	YYABORT;

      yyerror_range[0] = yylocation_stack_[0];
      yydestruct_ ("Error: popping",
                   yystos_[yystate_],
                   &yysemantic_stack_[0], &yylocation_stack_[0]);
      yypop_ ();
      yystate_ = yystate_stack_[0];
      YY_STACK_PRINT ();
    }

  if (yyn_ == yyfinal_)
    goto yyacceptlab;

  yyerror_range[1] = yylloc;
  // Using YYLLOC is tempting, but would change the location of
  // the look-ahead.  YYLOC is available though.
  YYLLOC_DEFAULT (yyloc, yyerror_range - 1, 2);
  yysemantic_stack_.push (yylval);
  yylocation_stack_.push (yyloc);

  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos_[yyn_],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

  yystate_ = yyn_;
  goto yynewstate;

  /* Accept.  */
yyacceptlab:
  yyresult_ = 0;
  goto yyreturn;

  /* Abort.  */
yyabortlab:
  yyresult_ = 1;
  goto yyreturn;

yyreturn:
  if (yylooka != yyeof_ && yylooka != yyempty_)
    yydestruct_ ("Cleanup: discarding lookahead", yyilooka, &yylval, &yylloc);

  while (yystate_stack_.height () != 1)
    {
      yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
      yypop_ ();
    }

  return yyresult_;
}

// Generate an error message.
std::string
yy::VrmlParser::yysyntax_error_ (YYERROR_VERBOSE_IF (int tok))
{
  std::string res;
#if YYERROR_VERBOSE
  yyn_ = yypact_[yystate_];
  if (yypact_ninf_ < yyn_ && yyn_ < yylast_)
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  */
      int yyxbegin = yyn_ < 0 ? -yyn_ : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = yylast_ - yyn_;
      int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
      int count = 0;
      for (int x = yyxbegin; x < yyxend; ++x)
        if (yycheck_[x + yyn_] == x && x != yyterror_)
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
            if (yycheck_[x + yyn_] == x && x != yyterror_)
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
const signed char yy::VrmlParser::yypact_ninf_ = -118;
const short int
yy::VrmlParser::yypact_[] =
{
    -118,    23,    20,  -118,    43,    37,  -118,  -118,    48,    38,
      37,  -118,  -118,    29,    49,   178,    38,  -118,  -118,    58,
    -118,    77,  -118,    54,    65,    82,    65,    65,    89,    89,
      65,  -118,  -118,  -118,   178,  -118,  -118,  -118,  -118,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,   185,  -118,
      91,  -118,    70,    26,  -118,  -118,    72,    71,  -118,    74,
    -118,    98,    98,    98,    98,  -118,  -118,  -118,  -118,    78,
     185,  -118,    83,   114,   114,  -118,    93,    57,   119,    15,
    -118,   120,   119,   121,   123,  -118,  -118,   121,   159,  -118,
    -118,   122,    95,    98,    98,    98,    98,    99,    57,  -118,
    -118,   105,  -118,   106,    15,  -118,   113,   116,   127,  -118,
     124,  -118,  -118,  -118,  -118,  -118,   103,  -118,  -118,  -118,
    -118,  -118,  -118,    65,    98,    98,    98,    98,   110,  -118,
      95,  -118,   120,   119,   121,   141,    41,  -118,    65,  -118,
    -118,   120,   119,   121,  -118,   123,  -118,   117,   118,   125,
    -118,   120,   119,   121,   142,   128,  -118,  -118,  -118,  -118,
    -118,    27,  -118,  -118,  -118,   126,  -118,  -118,  -118,  -118,
      79,  -118,  -118,  -118,  -118,   159,  -118,   159,  -118,  -118,
     120,  -118,  -118,    25,  -118,    25,    64,  -118,  -118,  -118,
      65,   178,  -118,   129,    26,  -118,  -118,  -118
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
const unsigned char
yy::VrmlParser::yydefact_[] =
{
       2,     0,     5,     1,     0,    62,     6,     4,     0,    62,
       7,     9,    11,     0,     0,    62,    15,    17,     8,     0,
      19,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     3,    21,    24,    27,    34,    35,    28,
      23,    29,    63,    16,    12,    10,    20,    18,    62,    83,
       0,    85,     0,     0,    31,    39,     0,     0,    22,     0,
      88,     0,     0,     0,     0,    82,    73,    81,    71,     0,
      62,    72,    78,     0,     0,    30,     0,    62,     0,    62,
      92,     0,     0,     0,     0,    65,    70,     0,     0,    86,
      13,     0,    62,     0,     0,     0,     0,     0,    53,    55,
      91,     0,    68,     0,    62,    89,    47,    48,     0,    90,
       0,    80,    97,   104,    98,    99,     0,   103,    79,    93,
      94,    95,    96,     0,     0,     0,     0,     0,     0,    50,
      44,    46,     0,     0,     0,     0,     0,    54,     0,    64,
      67,     0,     0,     0,    49,     0,   101,   109,     0,     0,
      14,     0,     0,     0,     0,     0,    45,    56,    57,    58,
      59,     0,    52,   100,    61,     0,    74,    75,    76,    77,
       0,   102,   107,    47,    48,     0,   111,     0,    40,   101,
       0,   110,    51,    62,    60,    36,     0,    41,    38,    37,
       0,    62,    32,     0,     0,    43,    42,    33
};

/* YYPGOTO[NTERM-NUM].  */
const short int
yy::VrmlParser::yypgoto_[] =
{
    -118,  -118,  -118,  -118,  -118,   140,  -118,  -118,  -118,  -118,
    -118,   135,  -118,  -118,  -118,   -32,  -118,   -14,  -118,   -12,
     -28,  -118,  -118,  -118,  -118,  -118,    31,   -76,  -118,  -118,
      66,  -118,    -9,  -118,    -5,   -46,  -118,    80,   100,  -118,
     -60,  -118,   -18,    11,  -118,   102,   -70,  -117,    24,   -72,
     -36,   -74,   -95,    47,  -118,  -118,    16,  -118,  -118
};

/* YYDEFGOTO[NTERM-NUM].  */
const short int
yy::VrmlParser::yydefgoto_[] =
{
      -1,     1,     2,     5,     7,     9,    10,    13,    45,    31,
      32,    15,    16,    21,    47,    33,    34,   117,   191,    65,
     186,    37,    76,   183,   193,   187,   128,    66,   130,    38,
      97,    98,    67,   162,    40,    41,    59,   103,    69,    70,
      71,    88,    50,    42,    52,    90,    72,   106,   110,   101,
      81,   144,   119,   120,   121,   122,   149,   170,   177
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
const signed char yy::VrmlParser::yytable_ninf_ = -109;
const short int
yy::VrmlParser::yytable_[] =
{
      11,    35,    58,    36,    17,    11,    39,    75,    53,    54,
     107,    17,    57,   108,   118,   157,   129,   111,    60,   104,
      35,   148,    36,     3,   166,    39,    82,    83,    84,    22,
       4,   112,    28,    23,   173,   114,   115,    29,    30,    55,
      56,   163,    28,    68,   104,   112,     6,    29,     8,   114,
     115,    12,    14,    20,   129,    19,   179,   132,   133,   134,
     135,   158,    44,   184,   159,    68,   148,    22,    49,   161,
     167,    23,    99,   168,   102,    93,    94,    95,    96,   190,
     174,    46,    22,   175,    48,    51,    23,   131,   151,   152,
     153,   154,    22,    99,    26,    27,    73,    74,    78,   102,
      77,    80,   147,   182,    79,   150,    22,   112,    87,    85,
      23,   114,   115,   124,   125,   126,   127,    89,    26,    27,
     165,    92,   100,   105,    60,   131,   109,   123,   136,   138,
      22,   112,   146,   113,    23,   114,   115,   139,   141,   155,
     192,   142,    26,    27,   160,   176,  -108,   171,   197,   145,
      18,    43,   143,   180,   172,   116,   147,   189,   178,   195,
     196,   156,    22,   112,   137,   113,    23,   114,   115,   169,
      86,   185,   194,   185,    26,    27,    91,    35,   188,    36,
     188,    22,    39,   164,   140,    23,   181,   116,    60,     0,
      24,    25,     0,    26,    27,    28,     0,     0,     0,     0,
      29,    30,    28,    61,    62,    63,    64,    29,    30
};

/* YYCHECK.  */
const short int
yy::VrmlParser::yycheck_[] =
{
       5,    15,    34,    15,     9,    10,    15,    53,    26,    27,
      82,    16,    30,    83,    88,   132,    92,    87,     3,    79,
      34,   116,    34,     0,   141,    34,    62,    63,    64,     3,
      10,     4,    17,     7,   151,     8,     9,    22,    23,    28,
      29,   136,    17,    48,   104,     4,     3,    22,    11,     8,
       9,     3,    14,     4,   130,    26,    29,    93,    94,    95,
      96,   133,     4,   180,   134,    70,   161,     3,     3,    28,
     142,     7,    77,   143,    79,    18,    19,    20,    21,    15,
     152,     4,     3,   153,    30,     3,     7,    92,   124,   125,
     126,   127,     3,    98,    15,    16,     5,    27,    27,   104,
      28,     3,   116,   177,    30,   123,     3,     4,    25,    31,
       7,     8,     9,    18,    19,    20,    21,     3,    15,    16,
     138,    28,     3,     3,     3,   130,     3,     5,    29,    24,
       3,     4,    29,     6,     7,     8,     9,    31,    25,    29,
     186,    25,    15,    16,     3,     3,    29,    29,   194,    25,
      10,    16,    25,    27,    29,    28,   170,   185,    30,   191,
      31,   130,     3,     4,    98,     6,     7,     8,     9,   145,
      70,   183,   190,   185,    15,    16,    74,   191,   183,   191,
     185,     3,   191,   136,   104,     7,   170,    28,     3,    -1,
      12,    13,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,
      22,    23,    17,    18,    19,    20,    21,    22,    23
};

/* STOS_[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
const unsigned char
yy::VrmlParser::yystos_[] =
{
       0,    33,    34,     0,    10,    35,     3,    36,    11,    37,
      38,    66,     3,    39,    14,    43,    44,    66,    37,    26,
       4,    45,     3,     7,    12,    13,    15,    16,    17,    22,
      23,    41,    42,    47,    48,    49,    51,    53,    61,    64,
      66,    67,    75,    43,     4,    40,     4,    46,    30,     3,
      74,     3,    76,    74,    74,    75,    75,    74,    47,    68,
       3,    18,    19,    20,    21,    51,    59,    64,    66,    70,
      71,    72,    78,     5,    27,    67,    54,    28,    27,    30,
       3,    82,    82,    82,    82,    31,    70,    25,    73,     3,
      77,    77,    28,    18,    19,    20,    21,    62,    63,    66,
       3,    81,    66,    69,    72,     3,    79,    81,    78,     3,
      80,    78,     4,     6,     8,     9,    28,    49,    83,    84,
      85,    86,    87,     5,    18,    19,    20,    21,    58,    59,
      60,    66,    82,    82,    82,    82,    29,    62,    24,    31,
      69,    25,    25,    25,    83,    25,    29,    49,    84,    88,
      74,    82,    82,    82,    82,    29,    58,    79,    81,    78,
       3,    28,    65,    84,    85,    74,    79,    81,    78,    80,
      89,    29,    29,    79,    81,    78,     3,    90,    30,    29,
      27,    88,    83,    55,    79,    51,    52,    57,    66,    52,
      15,    50,    67,    56,    74,    47,    31,    67
};

#if YYDEBUG
/* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
   to YYLEX-NUM.  */
const unsigned short int
yy::VrmlParser::yytoken_number_[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    58,    46,    91,    93,
     123,   125
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
const unsigned char
yy::VrmlParser::yyr1_[] =
{
       0,    32,    34,    33,    35,    35,    36,    37,    37,    37,
      38,    39,    40,    41,    42,    43,    43,    43,    44,    45,
      46,    47,    47,    47,    48,    48,    48,    48,    48,    49,
      49,    49,    50,    50,    51,    51,    52,    52,    52,    54,
      55,    56,    53,    57,    58,    58,    58,    59,    59,    59,
      60,    60,    61,    62,    62,    62,    63,    63,    63,    63,
      64,    65,    66,    68,    67,    67,    69,    69,    69,    70,
      70,    70,    71,    71,    71,    71,    71,    71,    73,    72,
      72,    72,    72,    74,    75,    76,    77,    75,    78,    79,
      80,    81,    82,    83,    83,    83,    83,    84,    84,    84,
      85,    85,    85,    86,    86,    87,    87,    87,    88,    89,
      88,    90
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
const unsigned char
yy::VrmlParser::yyr2_[] =
{
       0,     2,     0,     5,     2,     0,     1,     1,     2,     1,
       4,     1,     1,     4,     6,     1,     2,     1,     3,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     1,     3,     1,     1,     1,     2,     1,     0,
       0,     0,    11,     3,     1,     2,     1,     3,     3,     4,
       1,     4,     6,     1,     2,     1,     3,     3,     3,     3,
       8,     1,     0,     0,     5,     4,     1,     2,     1,     1,
       2,     1,     1,     1,     5,     5,     5,     5,     0,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     1,     1,     1,     2,     3,     1,     0,
       3,     1
};

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at \a yyntokens_, nonterminals. */
const char*
const yy::VrmlParser::yytname_[] =
{
  "$end", "error", "$undefined", "\"identifier\"", "\"field value\"",
  "AS", "VRMLNULL", "SCRIPT", "TRUE", "FALSE", "PROFILE", "COMPONENT",
  "EXPORT", "IMPORT", "META", "DEF", "USE", "PROTO", "inputOnly",
  "outputOnly", "initializeOnly", "inputOutput", "EXTERNPROTO", "ROUTE",
  "TO", "IS", "':'", "'.'", "'['", "']'", "'{'", "'}'", "$accept",
  "x3dScene", "@1", "profileStatement", "profileNameId",
  "componentStatements", "componentStatement", "componentNameId",
  "componentSupportLevel", "exportStatement", "importStatement",
  "metaStatements", "metaStatement", "metakey", "metavalue", "statements",
  "statement", "nodeStatement", "rootNodeStatement", "protoStatement",
  "protoStatements", "proto", "@2", "@3", "@4", "protoBody",
  "interfaceDeclarations", "restrictedInterfaceDeclaration",
  "interfaceDeclaration", "externproto", "externInterfaceDeclarations",
  "externInterfaceDeclaration", "routeStatement", "URLList", "empty",
  "node", "@5", "nodeBody", "scriptBody", "scriptBodyElement",
  "nodeBodyElement", "@6", "nodeNameId", "nodeTypeId", "inlineNodeNameId",
  "exportedNodeNameId", "initializeOnlyId", "inputOnlyId", "inputOutputId",
  "outputOnlyId", "fieldType", "fieldValue", "sfValue", "mfValue",
  "sfnodeValue", "mfnodeValue", "nodeStatements", "@7", "fieldId", 0
};
#endif

#if YYDEBUG
/* YYRHS -- A `-1'-separated list of the rules' RHS. */
const yy::VrmlParser::rhs_number_type
yy::VrmlParser::yyrhs_[] =
{
      33,     0,    -1,    -1,    34,    35,    37,    43,    47,    -1,
      10,    36,    -1,    -1,     3,    -1,    38,    -1,    38,    37,
      -1,    66,    -1,    11,    39,    26,    40,    -1,     3,    -1,
       4,    -1,    12,    74,     5,    77,    -1,    13,    76,    27,
      77,     5,    74,    -1,    44,    -1,    44,    43,    -1,    66,
      -1,    14,    45,    46,    -1,     4,    -1,     4,    -1,    48,
      -1,    48,    47,    -1,    66,    -1,    49,    -1,    41,    -1,
      42,    -1,    51,    -1,    64,    -1,    67,    -1,    15,    74,
      67,    -1,    16,    74,    -1,    67,    -1,    15,    74,    67,
      -1,    53,    -1,    61,    -1,    51,    -1,    51,    52,    -1,
      66,    -1,    -1,    -1,    -1,    17,    75,    54,    28,    58,
      29,    30,    55,    57,    56,    31,    -1,    52,    50,    47,
      -1,    60,    -1,    60,    58,    -1,    66,    -1,    18,    82,
      79,    -1,    19,    82,    81,    -1,    20,    82,    78,    83,
      -1,    59,    -1,    21,    82,    90,    83,    -1,    22,    75,
      28,    62,    29,    65,    -1,    63,    -1,    63,    62,    -1,
      66,    -1,    18,    82,    79,    -1,    19,    82,    81,    -1,
      20,    82,    78,    -1,    21,    82,     3,    -1,    23,    74,
      27,    81,    24,    74,    27,    79,    -1,    85,    -1,    -1,
      -1,    75,    68,    30,    69,    31,    -1,     7,    30,    70,
      31,    -1,    72,    -1,    72,    69,    -1,    66,    -1,    71,
      -1,    71,    70,    -1,    66,    -1,    72,    -1,    59,    -1,
      18,    82,    79,    25,    79,    -1,    19,    82,    81,    25,
      81,    -1,    20,    82,    78,    25,    78,    -1,    21,    82,
      80,    25,    80,    -1,    -1,    78,    73,    83,    -1,    78,
      25,    78,    -1,    64,    -1,    51,    -1,     3,    -1,     3,
      -1,     3,    -1,     3,    -1,     3,    -1,     3,    -1,     3,
      -1,     3,    -1,     3,    -1,     3,    -1,    84,    -1,    85,
      -1,    86,    -1,    87,    -1,     4,    -1,     8,    -1,     9,
      -1,    84,    -1,    28,    29,    -1,    28,    84,    29,    -1,
      49,    -1,     6,    -1,    49,    -1,    28,    29,    -1,    28,
      88,    29,    -1,    49,    -1,    -1,    49,    89,    88,    -1,
       3,    -1
};

/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
const unsigned short int
yy::VrmlParser::yyprhs_[] =
{
       0,     0,     3,     4,    10,    13,    14,    16,    18,    21,
      23,    28,    30,    32,    37,    44,    46,    49,    51,    55,
      57,    59,    61,    64,    66,    68,    70,    72,    74,    76,
      78,    82,    85,    87,    91,    93,    95,    97,   100,   102,
     103,   104,   105,   117,   121,   123,   126,   128,   132,   136,
     141,   143,   148,   155,   157,   160,   162,   166,   170,   174,
     178,   187,   189,   190,   191,   197,   202,   204,   207,   209,
     211,   214,   216,   218,   220,   226,   232,   238,   244,   245,
     249,   253,   255,   257,   259,   261,   263,   265,   267,   269,
     271,   273,   275,   277,   279,   281,   283,   285,   287,   289,
     291,   293,   296,   300,   302,   304,   306,   309,   313,   315,
     316,   320
};

/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
const unsigned short int
yy::VrmlParser::yyrline_[] =
{
       0,   110,   110,   110,   116,   116,   118,   120,   121,   122,
     124,   126,   128,   130,   145,   189,   190,   191,   193,   195,
     197,   199,   200,   201,   203,   210,   211,   212,   213,   215,
     216,   220,   225,   226,   228,   229,   231,   232,   233,   235,
     244,   245,   235,   256,   258,   259,   260,   263,   266,   269,
     273,   274,   279,   283,   284,   286,   288,   289,   290,   291,
     293,   332,   334,   337,   337,   356,   358,   359,   360,   362,
     363,   364,   366,   367,   368,   369,   370,   372,   374,   374,
     380,   401,   402,   404,   406,   408,   410,   412,   414,   416,
     418,   420,   422,   424,   428,   432,   433,   435,   436,   437,
     439,   440,   441,   444,   448,   453,   457,   462,   464,   468,
     468,   473
};

// Print the state stack on the debug stream.
void
yy::VrmlParser::yystack_print_ ()
{
  *yycdebug_ << "Stack now";
  for (state_stack_type::const_iterator i = yystate_stack_.begin ();
       i != yystate_stack_.end (); ++i)
    *yycdebug_ << ' ' << *i;
  *yycdebug_ << std::endl;
}

// Report on the debug stream that the rule \a yyrule is going to be reduced.
void
yy::VrmlParser::yyreduce_print_ (int yyrule)
{
  unsigned int yylno = yyrline_[yyrule];
  /* Print the symbols being reduced, and their result.  */
  *yycdebug_ << "Reducing stack by rule " << yyn_ - 1
             << " (line " << yylno << "), ";
  for (unsigned short int i = yyprhs_[yyn_];
       0 <= yyrhs_[i]; ++i)
    *yycdebug_ << yytname_[yyrhs_[i]] << ' ';
  *yycdebug_ << "-> " << yytname_[yyr1_[yyn_]] << std::endl;
}
#endif // YYDEBUG

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
yy::VrmlParser::token_number_type
yy::VrmlParser::yytranslate_ (int token)
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
  if ((unsigned int) token <= yyuser_token_number_max_)
    return translate_table[token];
  else
    return yyundef_token_;
}

const int yy::VrmlParser::yyeof_ = 0;
const int yy::VrmlParser::yylast_ = 208;
const int yy::VrmlParser::yynnts_ = 59;
const int yy::VrmlParser::yyempty_ = -2;
const int yy::VrmlParser::yyfinal_ = 3;
const int yy::VrmlParser::yyterror_ = 1;
const int yy::VrmlParser::yyerrcode_ = 256;
const int yy::VrmlParser::yyntokens_ = 32;

const unsigned int yy::VrmlParser::yyuser_token_number_max_ = 280;
const yy::VrmlParser::token_number_type yy::VrmlParser::yyundef_token_ = 2;

#line 475 "vrml.bison"



void yy::VrmlParser::error( const yy::VrmlParser::location_type& l,
                            const std::string& m ) {
  driver.error( l, m );
}

int yylex (YYSTYPE* yylval, yy::location* yylloc, VrmlDriver& driver) {
   return driver.lexer->yylex(yylval, yylloc, driver);
}

