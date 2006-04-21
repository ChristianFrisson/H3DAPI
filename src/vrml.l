%option noyywrap


ID       [^\x30-\x39\x0-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7f{}\[\] ][^\x0-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7f{}\[\] ]*

OLDID       [a-zA-Z][a-zA-Z0-9_]*

NUMBER   (([\-\+0-9][0-9\+\-.eE]*)|(0[xX][0-9a-fA-F]+))

WHITE    [ ,\t\n\r] 

STRING   \"(([^\\\"])|[\"]|\\.)+\"

%%

%{
#include "vrml.hpp"
%}

AS          return AS;
TRUE        return TRUE;
FALSE       return FALSE;
PROFILE     return PROFILE;
COMPONENT   return COMPONENT;
EXPORT      return EXPORT;
IMPORT      return IMPORT;
META        return META;
DEF         return DEF;
USE         return USE;
PROTO       return PROTO;
NULL        return VRMLNULL;
inputOnly   return inputOnly;
outputOnly  return outputOnly;
initializeOnly return initializeOnly;
inputOutput return inputOutput;
EXTERNPROTO return EXTERNPROTO;
ROUTE       return ROUTE;
TO          return TO;
IS          return IS;
MFBool      return TMFBool;
MFColor     return TMFColor;
MFColorRGBA return TMFColorRGBA;
MFDouble    return TMFDouble;
MFFloat     return TMFFloat;
MFImage     return TMFImage;
MFInt32     return TMFInt32;
MFNode      return TMFNode;
MFRotation  return TMFRotation;
MFString    return TMFString;
MFTime      return TMFTime;
MFVec2d     return TMFVec2d;
MFVec2f     return TMFVec2f;
MFVec3d     return TMFVec3d;
MFVec3f     return TMFVec3f;
SFBool      return TSFBool;
SFColor     return TSFColor;
SFColorRGBA return TSFColorRGBA;
SFDouble    return TSFDouble;
SFFloat     return TSFFloat;
SFImage     return TSFImage;
SFInt32     return TSFInt32;
SFNode      return TSFNode;
SFRotation  return TSFRotation;
SFString    return TSFString;
SFTime      return TSFTime;
SFVec2d     return TSFVec2d;
SFVec2f     return TSFVec2f;
SFVec3d     return TSFVec2d;
SFVec3f     return TSFVec3f;


{ID}        /*printf("ID=%s\n",yytext);*/ yylval.val=strdup(yytext); return ID;

"#"[^\n]+   /* eat up comments */


(({NUMBER}|{STRING})+({WHITE})*)+     /*printf("VAL=%s\n",yytext);*/ yylval.val=strdup(yytext); return STRING;

{WHITE}+       /* eat up whitespace */

.           return yytext[0];



%%


