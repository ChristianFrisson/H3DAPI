
%{

#include "VrmlDriver.h"
#include "vrml.hpp"
//#define YYLMAX 16777216
//#define YYLMAX 16777
%}

%option noyywrap nounput batch debug


ID       [^\x30-\x39\x0-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7f{}\[\] ][^\x0-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7f{}\[\] ]*

NUMBER   (([\-\+0-9\.][0-9\+\-.eE]*)|(0[xX][0-9a-fA-F]+))

COMMENT  \#[^\n]*

WHITE    [ ,\t\n\r] 

STRING    \"[^\"]+\"


%{
//# define YY_USER_ACTION yylloc->columns (yyleng);
%}

%%

%{
//yylloc->step ();
%}

AS             driver.addLine( yytext ); return AS;
TRUE           driver.addLine( yytext ); return TRUE;
FALSE          driver.addLine( yytext ); return FALSE;
PROFILE        driver.addLine( yytext ); return PROFILE;
COMPONENT      driver.addLine( yytext ); return COMPONENT;
EXPORT         driver.addLine( yytext ); return EXPORT;
IMPORT         driver.addLine( yytext ); return IMPORT;
META           driver.addLine( yytext ); return META;
DEF            driver.addLine( yytext ); return DEF;
USE            driver.addLine( yytext ); return USE;
PROTO          driver.addLine( yytext ); return PROTO;
NULL           driver.addLine( yytext ); return VRMLNULL;
inputOnly      driver.addLine( yytext ); return inputOnly;
eventIn        driver.addLine( yytext ); return inputOnly;
outputOnly     driver.addLine( yytext ); return outputOnly;
eventOut       driver.addLine( yytext ); return outputOnly;
initializeOnly driver.addLine( yytext ); return initializeOnly;
field          driver.addLine( yytext ); return initializeOnly;
inputOutput    driver.addLine( yytext ); return inputOutput;
exposedField   driver.addLine( yytext ); return inputOutput;
EXTERNPROTO    driver.addLine( yytext ); return EXTERNPROTO;
ROUTE          driver.addLine( yytext ); return ROUTE;
TO             driver.addLine( yytext ); return TO;
IS             driver.addLine( yytext ); return IS;


{ID}        /*printf("ID=%s\n",yytext);*/ yylval->val=strdup(yytext); driver.addLine( yytext );  return VRMLID;

({COMMENT}({WHITE})*)+    driver.addLine( yytext ); /* eat up comments */


(({NUMBER}|{STRING}|{COMMENT})({WHITE})*)+      driver.addLine( yytext );/* printf("VAL=%s\n",yytext); */ yylval->val=strdup(yytext); return STRING;


{WHITE}+     driver.addLine( yytext );  /* eat up whitespace */

.            driver.addLine( yytext ); return yytext[0];



%%

void vrml_driver::scan_begin () {
  //yy_flex_debug = trace_scanning;
 // if (!(lexer->yyin = fopen (file.c_str (), "r")))
 //   error (std::string ("cannot open ") + file);
}

void vrml_driver::scan_end () {
  //fclose (lexer->yyin);
}
