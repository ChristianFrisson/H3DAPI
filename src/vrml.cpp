/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     STRING = 259,
     AS = 260,
     VRMLNULL = 261,
     SCRIPT = 262,
     TRUE = 263,
     FALSE = 264,
     PROFILE = 265,
     COMPONENT = 266,
     EXPORT = 267,
     IMPORT = 268,
     META = 269,
     DEF = 270,
     USE = 271,
     PROTO = 272,
     inputOnly = 273,
     outputOnly = 274,
     initializeOnly = 275,
     inputOutput = 276,
     EXTERNPROTO = 277,
     ROUTE = 278,
     TO = 279,
     IS = 280,
     TMFBool = 281,
     TMFColor = 282,
     TMFColorRGBA = 283,
     TMFDouble = 284,
     TMFFloat = 285,
     TMFImage = 286,
     TMFInt32 = 287,
     TMFNode = 288,
     TMFRotation = 289,
     TMFString = 290,
     TMFTime = 291,
     TMFVec2d = 292,
     TMFVec2f = 293,
     TMFVec3d = 294,
     TMFVec3f = 295,
     TSFBool = 296,
     TSFColor = 297,
     TSFColorRGBA = 298,
     TSFDouble = 299,
     TSFFloat = 300,
     TSFImage = 301,
     TSFInt32 = 302,
     TSFNode = 303,
     TSFRotation = 304,
     TSFString = 305,
     TSFTime = 306,
     TSFVec2d = 307,
     TSFVec2f = 308,
     TSFVec3d = 309,
     TSFVec3f = 310
   };
#endif
/* Tokens.  */
#define ID 258
#define STRING 259
#define AS 260
#define VRMLNULL 261
#define SCRIPT 262
#define TRUE 263
#define FALSE 264
#define PROFILE 265
#define COMPONENT 266
#define EXPORT 267
#define IMPORT 268
#define META 269
#define DEF 270
#define USE 271
#define PROTO 272
#define inputOnly 273
#define outputOnly 274
#define initializeOnly 275
#define inputOutput 276
#define EXTERNPROTO 277
#define ROUTE 278
#define TO 279
#define IS 280
#define TMFBool 281
#define TMFColor 282
#define TMFColorRGBA 283
#define TMFDouble 284
#define TMFFloat 285
#define TMFImage 286
#define TMFInt32 287
#define TMFNode 288
#define TMFRotation 289
#define TMFString 290
#define TMFTime 291
#define TMFVec2d 292
#define TMFVec2f 293
#define TMFVec3d 294
#define TMFVec3f 295
#define TSFBool 296
#define TSFColor 297
#define TSFColorRGBA 298
#define TSFDouble 299
#define TSFFloat 300
#define TSFImage 301
#define TSFInt32 302
#define TSFNode 303
#define TSFRotation 304
#define TSFString 305
#define TSFTime 306
#define TSFVec2d 307
#define TSFVec2f 308
#define TSFVec3d 309
#define TSFVec3f 310




/* Copy the first part of user declarations.  */
#line 4 "vrml.bison"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 51 "vrml.bison"
typedef union YYSTYPE {
  char* val;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 245 "vrml.cpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 257 "vrml.cpp"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   242

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  54
/* YYNRULES -- Number of rules. */
#define YYNRULES  138
/* YYNRULES -- Number of states. */
#define YYNSTATES  229

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    56,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    57,     2,    58,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    59,     2,    60,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,    10,    13,    14,    16,    18,    21,
      23,    25,    30,    37,    39,    42,    44,    48,    50,    52,
      54,    57,    59,    61,    63,    65,    67,    69,    71,    75,
      78,    80,    84,    86,    88,    90,    93,    95,   104,   108,
     110,   113,   115,   119,   123,   128,   130,   135,   142,   144,
     147,   149,   153,   157,   161,   165,   174,   176,   177,   178,
     184,   189,   191,   194,   196,   198,   201,   203,   205,   207,
     213,   219,   225,   231,   232,   236,   240,   244,   248,   252,
     254,   256,   258,   260,   262,   264,   266,   268,   270,   272,
     274,   276,   278,   280,   282,   284,   286,   288,   290,   292,
     294,   296,   298,   300,   302,   304,   306,   308,   310,   312,
     314,   316,   318,   320,   322,   324,   326,   328,   330,   332,
     334,   336,   338,   340,   342,   344,   346,   348,   350,   353,
     357,   359,   361,   363,   366,   370,   372,   373,   377
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      62,     0,    -1,    -1,    63,    64,    66,    70,    74,    -1,
      10,    65,    -1,    -1,     3,    -1,    67,    -1,    67,    66,
      -1,    90,    -1,    11,    -1,    12,    98,     5,   101,    -1,
      13,   100,    56,   101,     5,    98,    -1,    71,    -1,    71,
      70,    -1,    90,    -1,    14,    72,    73,    -1,     4,    -1,
       4,    -1,    75,    -1,    75,    74,    -1,    90,    -1,    76,
      -1,    68,    -1,    69,    -1,    78,    -1,    88,    -1,    91,
      -1,    15,    98,    91,    -1,    16,    98,    -1,    91,    -1,
      15,    98,    91,    -1,    80,    -1,    85,    -1,    78,    -1,
      78,    79,    -1,    90,    -1,    17,    99,    57,    82,    58,
      59,    81,    60,    -1,    79,    77,    74,    -1,    84,    -1,
      84,    82,    -1,    90,    -1,    18,   106,   103,    -1,    19,
     106,   105,    -1,    20,   106,   102,   107,    -1,    83,    -1,
      21,   106,   114,   107,    -1,    22,    99,    57,    86,    58,
      89,    -1,    87,    -1,    87,    86,    -1,    90,    -1,    18,
     106,   103,    -1,    19,   106,   105,    -1,    20,   106,   102,
      -1,    21,   106,     3,    -1,    23,    98,    56,   105,    24,
      98,    56,   103,    -1,   109,    -1,    -1,    -1,    99,    92,
      59,    93,    60,    -1,     7,    59,    94,    60,    -1,    96,
      -1,    96,    93,    -1,    90,    -1,    95,    -1,    95,    94,
      -1,    90,    -1,    96,    -1,    83,    -1,    18,   106,   103,
      25,   103,    -1,    19,   106,   105,    25,   105,    -1,    20,
     106,   102,    25,   102,    -1,    21,   106,   104,    25,   104,
      -1,    -1,   102,    97,   107,    -1,   102,    25,   102,    -1,
     103,    25,   103,    -1,   105,    25,   105,    -1,   104,    25,
     104,    -1,    88,    -1,    78,    -1,     3,    -1,     3,    -1,
       3,    -1,     3,    -1,     3,    -1,     3,    -1,     3,    -1,
       3,    -1,     3,    -1,    26,    -1,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    31,    -1,    32,    -1,    33,    -1,
      34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,    -1,
      39,    -1,    40,    -1,    41,    -1,    42,    -1,    43,    -1,
      44,    -1,    45,    -1,    46,    -1,    47,    -1,    48,    -1,
      49,    -1,    50,    -1,    51,    -1,    52,    -1,    53,    -1,
      54,    -1,    55,    -1,   108,    -1,   109,    -1,   110,    -1,
     111,    -1,     4,    -1,     8,    -1,     9,    -1,   108,    -1,
      57,    58,    -1,    57,   108,    58,    -1,    76,    -1,     6,
      -1,    76,    -1,    57,    58,    -1,    57,   112,    58,    -1,
      76,    -1,    -1,    76,   113,   112,    -1,     4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    79,    79,    79,    85,    85,    87,    89,    90,    91,
      93,    99,   114,   156,   157,   158,   160,   162,   164,   166,
     167,   168,   170,   175,   176,   177,   178,   180,   181,   184,
     188,   189,   191,   192,   194,   195,   196,   198,   201,   203,
     204,   205,   208,   209,   210,   212,   213,   215,   219,   220,
     222,   224,   225,   226,   227,   229,   266,   268,   271,   271,
     279,   281,   282,   283,   285,   286,   287,   289,   290,   291,
     292,   293,   295,   297,   297,   302,   303,   304,   305,   306,
     307,   309,   311,   313,   315,   317,   319,   321,   323,   325,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     358,   361,   364,   365,   367,   368,   369,   371,   372,   373,
     376,   379,   383,   386,   390,   392,   395,   395,   399
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRING", "AS", "VRMLNULL",
  "SCRIPT", "TRUE", "FALSE", "PROFILE", "COMPONENT", "EXPORT", "IMPORT",
  "META", "DEF", "USE", "PROTO", "inputOnly", "outputOnly",
  "initializeOnly", "inputOutput", "EXTERNPROTO", "ROUTE", "TO", "IS",
  "TMFBool", "TMFColor", "TMFColorRGBA", "TMFDouble", "TMFFloat",
  "TMFImage", "TMFInt32", "TMFNode", "TMFRotation", "TMFString", "TMFTime",
  "TMFVec2d", "TMFVec2f", "TMFVec3d", "TMFVec3f", "TSFBool", "TSFColor",
  "TSFColorRGBA", "TSFDouble", "TSFFloat", "TSFImage", "TSFInt32",
  "TSFNode", "TSFRotation", "TSFString", "TSFTime", "TSFVec2d", "TSFVec2f",
  "TSFVec3d", "TSFVec3f", "'.'", "'['", "']'", "'{'", "'}'", "$accept",
  "x3dScene", "@1", "profileStatement", "profileNameId",
  "componentStatements", "componentStatement", "exportStatement",
  "importStatement", "metaStatements", "metaStatement", "metakey",
  "metavalue", "statements", "statement", "nodeStatement",
  "rootNodeStatement", "protoStatement", "protoStatements", "proto",
  "protoBody", "interfaceDeclarations", "restrictedInterfaceDeclaration",
  "interfaceDeclaration", "externproto", "externInterfaceDeclarations",
  "externInterfaceDeclaration", "routeStatement", "URLList", "empty",
  "node", "@2", "nodeBody", "scriptBody", "scriptBodyElement",
  "nodeBodyElement", "@3", "nodeNameId", "nodeTypeId", "inlineNodeNameId",
  "exportedNodeNameId", "initializeOnlyId", "inputOnlyId", "inputOutputId",
  "outputOnlyId", "fieldType", "fieldValue", "sfValue", "mfValue",
  "sfnodeValue", "mfnodeValue", "nodeStatements", "@4", "fieldId", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,    46,    91,    93,   123,
     125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    61,    63,    62,    64,    64,    65,    66,    66,    66,
      67,    68,    69,    70,    70,    70,    71,    72,    73,    74,
      74,    74,    75,    75,    75,    75,    75,    76,    76,    76,
      77,    77,    78,    78,    79,    79,    79,    80,    81,    82,
      82,    82,    83,    83,    83,    84,    84,    85,    86,    86,
      86,    87,    87,    87,    87,    88,    89,    90,    92,    91,
      91,    93,    93,    93,    94,    94,    94,    95,    95,    95,
      95,    95,    95,    97,    96,    96,    96,    96,    96,    96,
      96,    98,    99,   100,   101,    99,   102,   103,   104,   105,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     107,   107,   107,   107,   108,   108,   108,   109,   109,   109,
     110,   110,   111,   111,   111,   112,   113,   112,   114
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     5,     2,     0,     1,     1,     2,     1,
       1,     4,     6,     1,     2,     1,     3,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       1,     3,     1,     1,     1,     2,     1,     8,     3,     1,
       2,     1,     3,     3,     4,     1,     4,     6,     1,     2,
       1,     3,     3,     3,     3,     8,     1,     0,     0,     5,
       4,     1,     2,     1,     1,     2,     1,     1,     1,     5,
       5,     5,     5,     0,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       1,     1,     1,     2,     3,     1,     0,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,     0,     5,     1,     0,    57,     6,     4,    10,    57,
       7,     9,     0,    57,    13,    15,     8,    17,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    24,
       3,    19,    22,    25,    32,    33,    26,    21,    27,    58,
      14,    18,    16,    57,    81,     0,    83,     0,     0,    29,
       0,     0,     0,    20,     0,    86,     0,     0,     0,     0,
      80,    68,    79,    66,     0,    57,    67,    73,     0,     0,
       0,     0,     0,    28,    57,    57,     0,    57,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,     0,     0,
       0,     0,    60,    65,     0,     0,     0,     0,     0,    84,
      11,     0,     0,     0,     0,     0,     0,    45,    39,    41,
       0,     0,     0,     0,     0,    48,    50,    89,     0,    63,
       0,    57,    87,    42,    43,    86,     0,    88,     0,    75,
     124,   131,   125,   126,     0,   130,    74,   120,   121,   122,
     123,    76,    78,    77,     0,     0,     0,     0,     0,     0,
      40,     0,     0,     0,     0,     0,    49,     0,    59,    62,
       0,     0,     0,    44,     0,   128,   136,     0,     0,    12,
      42,    43,     0,   138,     0,    57,    51,    52,    53,    54,
       0,    47,   127,    56,     0,    69,    70,    71,    72,     0,
     129,   134,    46,    34,     0,     0,    36,   128,     0,   137,
      35,     0,    57,    30,    37,    55,     0,    38,    31
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,     2,     5,     7,     9,    10,    28,    29,    13,
      14,    18,    42,    30,    31,   155,   222,    60,   214,    34,
     215,   126,    61,   128,    35,   134,   135,    62,   201,    37,
      38,    54,   140,    64,    65,    66,   115,    45,    39,    47,
     120,    67,    68,    69,    70,   108,   183,   157,   158,   159,
     160,   188,   209,   194
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -104
static const short int yypact[] =
{
    -104,    17,    16,  -104,    36,    26,  -104,  -104,  -104,    47,
      26,  -104,    46,   113,    47,  -104,  -104,  -104,    71,  -104,
      22,    79,    80,    79,    79,    89,    89,    79,  -104,  -104,
    -104,   113,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,   134,  -104,    95,  -104,    59,    65,  -104,
      64,    74,    62,  -104,    73,  -104,   187,   187,   187,   187,
    -104,  -104,  -104,  -104,    88,   134,  -104,   108,   114,   137,
     138,   146,   146,  -104,    93,   124,   161,   100,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,  -104,  -104,   163,   161,
     164,   165,  -104,  -104,   164,    51,   163,   165,   161,  -104,
    -104,   166,   187,   187,   187,   187,   112,  -104,    93,  -104,
     187,   187,   187,   187,   116,   124,  -104,  -104,   148,  -104,
     115,   100,  -104,   151,   152,  -104,    37,  -104,   153,  -104,
    -104,  -104,  -104,  -104,    20,  -104,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,    79,   163,   161,   164,   175,   121,
    -104,   163,   161,   164,   179,    81,  -104,    79,  -104,  -104,
     163,   161,   164,  -104,   165,  -104,   126,   127,   128,  -104,
    -104,  -104,    51,  -104,    51,    34,  -104,  -104,  -104,  -104,
      61,  -104,  -104,  -104,   131,  -104,  -104,  -104,  -104,   143,
    -104,  -104,  -104,    34,   158,   129,  -104,  -104,   163,  -104,
    -104,    79,   113,  -104,  -104,  -104,    65,  -104,  -104
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -104,  -104,  -104,  -104,  -104,   178,  -104,  -104,  -104,   176,
    -104,  -104,  -104,   -20,  -104,   -13,  -104,   -12,   -22,  -104,
    -104,    67,   -49,  -104,  -104,    57,  -104,   -11,  -104,    -1,
     -45,  -104,    52,   132,  -104,   -61,  -104,   -17,    23,  -104,
     133,   -80,   -94,   -96,   -71,   -26,  -103,   -91,    24,  -104,
    -104,    -9,  -104,  -104
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -136
static const short int yytable[] =
{
      32,    33,    36,    73,    11,   138,    48,    49,    15,    11,
      52,    53,   156,    15,   143,   148,   141,     3,    32,    33,
      36,   162,   161,    19,   150,   127,     4,    20,   152,   153,
     146,   109,   110,   111,   149,    23,    24,     8,   144,     6,
      19,   150,    63,   151,    20,   152,   153,   163,    50,    51,
      17,    25,    23,    24,    19,   150,    26,   151,    20,   152,
     153,    12,   182,   187,    63,   150,    23,    24,    19,   152,
     153,   190,    20,   129,   136,    41,   139,   196,   185,   127,
     141,    43,    44,    46,   202,   150,   205,   192,   208,   152,
     153,   212,    19,   198,   154,   191,   165,   166,   167,   168,
      71,   197,   207,    55,   171,   172,   173,   174,   154,   187,
     206,   122,   123,   124,   125,    72,    19,    25,    76,   217,
      20,    74,    26,    27,   225,    21,    22,   129,    23,    24,
      25,    75,    77,   114,   136,    26,    27,    55,   200,   116,
     139,   186,   130,   131,   132,   133,    19,   189,   112,   119,
      20,    25,    56,    57,    58,    59,    26,    27,    23,    24,
     204,    19,   117,   118,   137,    20,   142,   145,   147,   223,
     169,   164,   177,   221,   175,   178,   180,   181,   184,   193,
     195,   228,   199,   213,  -135,   210,   211,   218,    16,   224,
      40,   220,   176,   179,   216,   170,   186,   113,     0,   203,
     219,   213,   227,     0,   226,   121,     0,     0,     0,    32,
      33,    36,   216,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107
};

static const short int yycheck[] =
{
      13,    13,    13,    48,     5,    76,    23,    24,     9,    10,
      27,    31,   115,    14,   108,   111,    77,     0,    31,    31,
      31,   117,   116,     3,     4,    74,    10,     7,     8,     9,
     110,    57,    58,    59,   114,    15,    16,    11,   109,     3,
       3,     4,    43,     6,     7,     8,     9,   118,    25,    26,
       4,    17,    15,    16,     3,     4,    22,     6,     7,     8,
       9,    14,    25,   154,    65,     4,    15,    16,     3,     8,
       9,   165,     7,    74,    75,     4,    77,   171,    58,   128,
     141,    59,     3,     3,   175,     4,   180,   167,   184,     8,
       9,   194,     3,   173,    57,   166,   122,   123,   124,   125,
       5,   172,   182,     3,   130,   131,   132,   133,    57,   200,
     181,    18,    19,    20,    21,    56,     3,    17,    56,    58,
       7,    57,    22,    23,   218,    12,    13,   128,    15,    16,
      17,    57,    59,    25,   135,    22,    23,     3,    57,    25,
     141,   154,    18,    19,    20,    21,     3,   164,    60,     3,
       7,    17,    18,    19,    20,    21,    22,    23,    15,    16,
     177,     3,    25,    25,     3,     7,     3,     3,     3,   214,
      58,     5,    24,    15,    58,    60,    25,    25,    25,     4,
      59,   226,     3,   195,    58,    58,    58,    56,    10,    60,
      14,   213,   135,   141,   195,   128,   209,    65,    -1,   175,
     209,   213,   222,    -1,   221,    72,    -1,    -1,    -1,   222,
     222,   222,   213,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    62,    63,     0,    10,    64,     3,    65,    11,    66,
      67,    90,    14,    70,    71,    90,    66,     4,    72,     3,
       7,    12,    13,    15,    16,    17,    22,    23,    68,    69,
      74,    75,    76,    78,    80,    85,    88,    90,    91,    99,
      70,     4,    73,    59,     3,    98,     3,   100,    98,    98,
      99,    99,    98,    74,    92,     3,    18,    19,    20,    21,
      78,    83,    88,    90,    94,    95,    96,   102,   103,   104,
     105,     5,    56,    91,    57,    57,    56,    59,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   106,   106,
     106,   106,    60,    94,    25,    97,    25,    25,    25,     3,
     101,   101,    18,    19,    20,    21,    82,    83,    84,    90,
      18,    19,    20,    21,    86,    87,    90,     3,   105,    90,
      93,    96,     3,   103,   105,     3,   102,     3,   104,   102,
       4,     6,     8,     9,    57,    76,   107,   108,   109,   110,
     111,   103,   104,   105,     5,   106,   106,   106,   106,    58,
      82,   106,   106,   106,   106,    58,    86,    24,    60,    93,
      25,    25,    25,   107,    25,    58,    76,   108,   112,    98,
     103,   105,   102,     4,   114,    59,   103,   105,   102,     3,
      57,    89,   108,   109,    98,   103,   105,   102,   104,   113,
      58,    58,   107,    78,    79,    81,    90,    58,    56,   112,
      79,    15,    77,    91,    60,   103,    98,    74,    91
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
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
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 79 "vrml.bison"
    { root = new Group(); ;}
    break;

  case 11:
#line 99 "vrml.bison"
    {
  Node *node= DEF_map->getNode( (yyvsp[-2].val) );
  if ( node ) {
    if ( DEF_export )
      DEF_export->addNode( (yyvsp[0].val), node );
    else
      Console(3) << "WARNING: EXPORT error. No export DEF map provided!"
                 << getLocationString() << endl;
  } else {
    Console(3) << "WARNING: EXPORT error. Node named \"" 
               << (yyvsp[-2].val) << "\" does not exist."
               << getLocationString() << endl;
  }
;}
    break;

  case 12:
#line 115 "vrml.bison"
    {
  if( strcmp( (yyvsp[-4].val), "H3D_EXPORTS" ) == 0 ) {
    Node *import_node = 
      H3DExports::getH3DExportNode( (yyvsp[-2].val) );
    if( import_node ) {
      DEF_map->addNode( (yyvsp[0].val), import_node ); 
    } else {
      Console(3) << "WARNING: IMPORT error. H3D_EXPORTS " 
                 << "does not include \""
                 << (yyvsp[-2].val) << "\"" 
                 << getLocationString() << endl;
    }
  } else {
    Node *n = DEF_map->getNode( (yyvsp[-4].val) );
    Inline *inline_node = dynamic_cast< Inline * >( n );
    if( inline_node ) {
      Node *import_node = 
        inline_node->exported_nodes.getNode( (yyvsp[-2].val) );
      if( import_node ) {
        DEF_map->addNode( (yyvsp[0].val), 
                          import_node ); 
      } else {
        Console(3) << "WARNING: IMPORT error. Inline node \"" 
                   << (yyvsp[-4].val) << "\" does not EXPORT \""
                   << (yyvsp[-2].val) << "\"" 
                   << getLocationString() << endl;
      }
    } else {
      if( n ) {
        Console(3) << "WARNING: IMPORT error. Node \"" 
                   << (yyvsp[-4].val) << "\" is not an Inline node "
                   << getLocationString() << endl;
      } else {
        Console(3) << "WARNING: IMPORT error. Node named \"" 
                   << (yyvsp[-4].val) << "\" does not exist."
                   << getLocationString() << endl;
      }
    }
  }
;}
    break;

  case 22:
#line 170 "vrml.bison"
    {
                            Node *node = node_stack.back();
                            node_stack.pop_back();
                            root->children->push_back( node );
                         ;}
    break;

  case 28:
#line 181 "vrml.bison"
    {
                           DEF_map->addNode( (yyvsp[-1].val), node_stack.back() );
                         ;}
    break;

  case 29:
#line 184 "vrml.bison"
    {
                           node_stack.push_back( DEF_map->getNode( (yyvsp[0].val) ) );
                         ;}
    break;

  case 55:
#line 230 "vrml.bison"
    {
  Node *fr = DEF_map->getNode( (yyvsp[-6].val) );
  if ( fr ) {
    Field *frf = fr->getField( (yyvsp[-4].val) );
    if ( frf ) {
      Node *to = DEF_map->getNode( (yyvsp[-2].val) );
      if ( to ) {
        Field *tof = to->getField( (yyvsp[0].val) );
        if ( tof )
          frf->route( tof );
        else {
          Console(3) << "WARNING: Route error. Could not find field named \"" 
                     <<  (yyvsp[0].val)
                     << "\" in \"" << (yyvsp[-2].val) << "\" Node " 
                     << getLocationString() << endl;
        }
      } else {
        Console(3) << "WARNING: Route error. Could not find destination Node named \"" 
                   << (yyvsp[-2].val)
                   << "\" " 
                   << getLocationString() << endl;
      }
    } else {
      Console(3) << "WARNING: Route error. Could not find field named \"" 
                 << (yyvsp[-4].val)
                 << "\" in \"" << (yyvsp[-6].val) << "\" Node " 
                 << getLocationString() << endl;
    }  
  } else {
    Console(3) << "WARNING: Route error. Could not find source Node named \"" 
               << (yyvsp[-6].val)
               << "\" " 
               << getLocationString() << endl;
  }
;}
    break;

  case 58:
#line 271 "vrml.bison"
    { 
  Node *new_node =  H3DNodeDatabase::createNode( yylval.val );
  if ( !new_node )
    Console(3) << "WARNING: Could not create node \"" << yylval.val << 
      "\" - name not found in the node database." << endl;
  node_stack.push_back( new_node );
;}
    break;

  case 73:
#line 297 "vrml.bison"
    { 
                            field_stack.push_back( (yyvsp[0].val) ); ;}
    break;

  case 74:
#line 299 "vrml.bison"
    {
                                field_stack.pop_back();
                            ;}
    break;

  case 120:
#line 358 "vrml.bison"
    { 
  setFieldValue( (yyvsp[0].val) );
;}
    break;

  case 121:
#line 361 "vrml.bison"
    { 
  setFieldValue( (yyvsp[0].val) );
                        ;}
    break;

  case 125:
#line 368 "vrml.bison"
    { (yyval.val) = "TRUE"; ;}
    break;

  case 126:
#line 369 "vrml.bison"
    { (yyval.val) = "FALSE";;}
    break;

  case 128:
#line 372 "vrml.bison"
    { (yyval.val) = ""; ;}
    break;

  case 129:
#line 373 "vrml.bison"
    { (yyval.val) = (yyvsp[-1].val); ;}
    break;

  case 130:
#line 376 "vrml.bison"
    {
  setNodeStatement( 0 );
                        ;}
    break;

  case 131:
#line 379 "vrml.bison"
    {
  setNodeStatement( 1 );
                        ;}
    break;

  case 132:
#line 383 "vrml.bison"
    {
  setNodeStatement( 0 );
                        ;}
    break;

  case 133:
#line 386 "vrml.bison"
    { 
  setNodeStatement( 1 );

                        ;}
    break;

  case 135:
#line 392 "vrml.bison"
    {
  setNodeStatement( 0 );
                        ;}
    break;

  case 136:
#line 395 "vrml.bison"
    {
  setNodeStatement( 0 );
                        ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1706 "vrml.cpp"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
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
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 401 "vrml.bison"




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

