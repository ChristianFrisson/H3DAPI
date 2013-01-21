
%{
//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
//
//  Flex instructions:
//  Generate lex.yy.cc by running   "flex++ -Cfa vrml.l"
//  The -Cfa flag specifies that the parser should generate an
//  uncompressed lookup table and opt for speed over size.
//

#include <H3D/VrmlDriver.h>
#include "vrml.hpp"
#define YYLMAX 65535
%}

%option noyywrap nounput batch debug


ID       [^\-\x30-\x39\x0-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7f{}\[\] ][^\x0-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7f{}\[\] ]*

OLDNUMBER   (([\-\+0-9][0-9\+\-.eE]*)|([\-\+0-9\.][0-9\+\-.eE]+)|(0[xX][0-9a-fA-F]+))

NUMBER    ([+/-]?((([0-9]+(\.)?)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?)|([+\-]?(([0-9]+)|(0[xX][0-9a-fA-F]+))))

COMMENT  \#[^\n]*\n

WHITE    [ ,\t\n\r] 

STRING    \"[^\"]*\"


%%


AS             driver.addLine( yytext ); return yy::VrmlParser::token::AS;
TRUE           driver.addLine( yytext ); return yy::VrmlParser::token::VRMLTRUE;
FALSE          driver.addLine( yytext ); return yy::VrmlParser::token::VRMLFALSE;
PROFILE        driver.addLine( yytext ); return yy::VrmlParser::token::PROFILE;
COMPONENT      driver.addLine( yytext ); return yy::VrmlParser::token::COMPONENT;
EXPORT         driver.addLine( yytext ); return yy::VrmlParser::token::EXPORT;
IMPORT         driver.addLine( yytext ); return yy::VrmlParser::token::IMPORT;
META           driver.addLine( yytext ); return yy::VrmlParser::token::META;
DEF            driver.addLine( yytext ); return yy::VrmlParser::token::DEF;
USE            driver.addLine( yytext ); return yy::VrmlParser::token::USE;
PROTO          driver.addLine( yytext ); return yy::VrmlParser::token::PROTO;
NULL           driver.addLine( yytext ); return yy::VrmlParser::token::VRMLNULL;
inputOnly      driver.addLine( yytext ); return yy::VrmlParser::token::inputOnly;
eventIn        driver.addLine( yytext ); return yy::VrmlParser::token::inputOnly;
outputOnly     driver.addLine( yytext ); return yy::VrmlParser::token::outputOnly;
eventOut       driver.addLine( yytext ); return yy::VrmlParser::token::outputOnly;
initializeOnly driver.addLine( yytext ); return yy::VrmlParser::token::initializeOnly;
field          driver.addLine( yytext ); return yy::VrmlParser::token::initializeOnly;
inputOutput    driver.addLine( yytext ); return yy::VrmlParser::token::inputOutput;
exposedField   driver.addLine( yytext ); return yy::VrmlParser::token::inputOutput;
EXTERNPROTO    driver.addLine( yytext ); return yy::VrmlParser::token::EXTERNPROTO;
ROUTE          driver.addLine( yytext ); return yy::VrmlParser::token::ROUTE;
TO             driver.addLine( yytext ); return yy::VrmlParser::token::TO;
IS             driver.addLine( yytext ); return yy::VrmlParser::token::IS;


{ID}        *yylval=std::string(yytext); driver.addLine( yytext );  return yy::VrmlParser::token::VRMLID;

({COMMENT}({WHITE})*)+  driver.addLine( yytext );


(({NUMBER}|{STRING}|{COMMENT})({WHITE})*)+       driver.addLine( yytext ); *yylval=std::string(yytext); return yy::VrmlParser::token::STRING;


{WHITE}+     driver.addLine( yytext );

.            driver.addLine( yytext ); return yytext[0];



%%

