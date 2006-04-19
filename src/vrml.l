%option noyywrap


ID       [a-zA-Z][a-zA-Z0-9]*

NUMBER   [\-\+0-9][0-9.e]*

WHITE    [ ,\t\n\r] 

STRING   \"[^\"]+\"

%%

%{
#include "vrml.bison.tab.h"
%}

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
MFBool      return MFBool;
MFColor     return MFColor;
MFColorRGBA return MFColorRGBA;
MFDouble    return MFDouble;
MFFloat     return MFFloat;
MFImage     return MFImage;
MFInt32     return MFInt32;
MFNode      return MFNode;
MFRotation  return MFRotation;
MFString    return MFString;
MFTime      return MFTime;
MFVec2d     return MFVec2d;
MFVec2f     return MFVec2f;
MFVec3d     return MFVec3d;
MFVec3f     return MFVec3f;
SFBool      return SFBool;
SFColor     return SFColor;
SFColorRGBA return SFColorRGBA;
SFDouble    return SFDouble;
SFFloat     return SFFloat;
SFImage     return SFImage;
SFInt32     return SFInt32;
SFNode      return SFNode;
SFRotation  return SFRotation;
SFString    return SFString;
SFTime      return SFTime;
SFVec2d     return SFVec2d;
SFVec2f     return SFVec2f;
SFVec3d     return SFVec2d;
SFVec3f     return SFVec3f;


{ID}        yylval.val=yytext; return ID;

"#"[^\n]+   /* eat up comments */


(({NUMBER}|{STRING})+({WHITE})*)+     yylval.val=yytext; return STRING;

{WHITE}+       /* eat up whitespace */

.           return yytext[0];



%%


