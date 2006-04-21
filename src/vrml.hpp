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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 50 "vrml.bison"
typedef union YYSTYPE {
  char* val;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 152 "vrml.hpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



