x3dScene ::= 
       headerStatement
       profileStatement
       componentStatements
       metaStatements
statements ;
headerStatement ::=
#X3D 3.0 utf8 string ;
profileStatement ::=
PROFILE profileNameId ;
profileNameId ::=
Id ;
componentStatements ::=
componentStatement |
componentStatement componentStatements |
empty ;
componentStatement ::=
COMPONENT componentNameId:componentSupportLevel ;
componentNameId ::=
Id ;
componentSupportLevel ::=
int32 ;
metaStatements ::=
metaStatement |
metaStatement metaStatements |
empty ;
metaStatement ::=
META metakey metavalue ;
metakey ::=
string ;
metavalue ::=
string ;
statements ::=
statement |
statement statements |
empty ;
statement ::=
nodeStatement |
protoStatement |
routeStatement ;
nodeStatement ::=
node |
DEF nodeNameId node |
USE nodeNameId ;
rootNodeStatement ::=
node | DEF nodeNameId node ;
protoStatement ::=
proto |
externproto ;
protoStatements ::=
protoStatement |
protoStatement protoStatements |
empty ;
proto ::=
PROTO nodeTypeId [ interfaceDeclarations ] { protoBody } ;
protoBody ::=
protoStatements rootNodeStatement statements ;
interfaceDeclarations ::=
interfaceDeclaration |
interfaceDeclaration interfaceDeclarations |
empty ;
restrictedInterfaceDeclaration ::=
eventIn fieldType eventInId |
eventOut fieldType eventOutId |
field fieldType fieldId fieldValue ;
interfaceDeclaration ::=
restrictedInterfaceDeclaration |
exposedField fieldType fieldId fieldValue ;
externproto ::=
EXTERNPROTO nodeTypeId [ externInterfaceDeclarations ] URLList ;
externInterfaceDeclarations ::=
externInterfaceDeclaration |
externInterfaceDeclaration externInterfaceDeclarations |
empty ;
externInterfaceDeclaration ::=
eventIn fieldType eventInId |
eventOut fieldType eventOutId |
field fieldType fieldId |
exposedField fieldType fieldId ;
routeStatement ::=
ROUTE nodeNameId . eventOutId TO nodeNameId . eventInId ;
URLList ::=
mfstringValue ;
empty ::=
;



 A.3 Nodes


 node ::=
 nodeTypeId { nodeBody } |
 Script { scriptBody } ;
nodeBody ::=
nodeBodyElement |
nodeBodyElement nodeBody |
empty ;
scriptBody ::=
scriptBodyElement |
scriptBodyElement scriptBody |
empty ;
scriptBodyElement ::=
nodeBodyElement |
restrictedInterfaceDeclaration |
eventIn fieldType eventInId IS eventInId |
eventOut fieldType eventOutId IS eventOutId |
field fieldType fieldId IS fieldId ;
nodeBodyElement ::=
fieldId fieldValue |
fieldId IS fieldId |
eventInId IS eventInId |
eventOutId IS eventOutId |
routeStatement |
protoStatement ;
nodeNameId ::=
Id ;
nodeTypeId ::=
Id ;
fieldId ::=
Id ;
eventInId ::=
Id ;
eventOutId ::=
Id ;
Id ::=
IdFirstChar |
IdFirstChar IdRestChars ;
IdFirstChar ::=
Any ISO-10646 character encoded using UTF-8 except: 0x30-0x39, 0x0-0x20, 0x22, 0x23, 0x27, 0x2b, 0x2c, 0x2d, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ;
IdRestChars ::=
Any number of ISO-10646 characters except: 0x0-0x20, 0x22, 0x23, 0x27, 0x2c, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ;



 A.4 Fields

 fieldType ::=
MFBool |
MFColor |
MFColorRGBA |
MFDouble |
MFFloat |
MFImage |
MFInt32 |
MFNode |
MFRotation |
MFString |
MFTime |
MFVec2d |
MFVec2f |
MFVec3d |
MFVec3f |
SFBool |
SFColor |
SFColorRGBA |
SFDouble |
SFFloat |
SFImage |
SFInt32 |
SFNode |
SFRotation |
SFString |
SFTime |
SFVec2d |
SFVec2f |
SFVec3d |
 SFVec3f ;
fieldValue ::=
sfboolValue |
sfcolorValue |
sfcolorRGBAValue |
sfdoubleValue |
sffloatValue |
sfimageValue |
sfint32Value |
sfnodeValue |
sfrotationValue |
sfstringValue |
sftimeValue |
sfvec2dValue |
sfvec2fValue |
sfvec3dValue |
sfvec3fValue |
mfboolValue |
mfcolorValue |
mfcolorRGBAValue |
mfdoubleValue |
mffloatValue |
mfimageValue |
mfint32Value |
mfnodeValue |
mfrotationValue |
mfstringValue |
mftimeValue |
mfvec2dValue |
mfvec2fValue |
sfvec3dValue |
mfvec3fValue ;
sfboolValue ::=
TRUE |
FALSE ;
sfcolorValue ::=
float float float ;
sfcolorRGBAValue ::=
float float float float ;
sfdoubleValue ::=
double ;
double ::=
([+/-]?((([0-9]+(\.)?)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?))
     sffloatValue ::=
float ;
float ::=
([+/-]?((([0-9]+(\.)?)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?)).
sfimageValue ::=
int32 int32 int32 ...
sfint32Value ::=
int32 ;
int32 ::=
([+\-]?(([0-9]+)|(0[xX][0-9a-fA-F]+)))
     sfnodeValue ::=
nodeStatement |
NULL ;
sfrotationValue ::=
float float float float ;
sfstringValue ::=
string ;
string ::=
".*" ... double-quotes must be \", backslashes must be \\...
  sftimeValue ::=
  double ;
sfvec2dValue ::=
double double ;
sfvec3dValue ::=
double double double ;
sfvec2fValue ::=
float float ;
sfvec3fValue ::=
float float float ;
mfboolValue ::=
sfboolValue |
[ ] |
[ sfboolValues ] ;
sfboolValues ::=
sfboolValue |
sfboolValue sfboolValues ;
mfcolorValue ::=
sfcolorValue |
[ ] |
[ sfcolorValues ] ;
sfcolorValues ::=
sfcolorValue |
sfcolorValue sfcolorValues ;
mfcolorRGBAValue ::=
sfcolorRGBAValue |
[ ] |
[ sfcolorRGBAValues ] ;
sfcolorRGBAValues ::=
sfcolorRGBAValue |
sfcolorRGBAValue sfcolorRGBAValues ;
mfdoubleValue ::=
sfdoubleValue |
[ ] |
[ sfdoubleValues ] ;
sfdoubleValues ::=
sfdoubleValue |
sfdoubleValue sfdoubleValues ;
mffloatValue ::=
sffloatValue |
[ ] |
[ sffloatValues ] ;
sffloatValues ::=
sffloatValue |
sffloatValue sffloatValues ;
mfimageValue ::=
sfimageValue |
[ ] |
[ sfimageValues ] ;
sfimageValues ::=
sfimageValue |
sfimageValue sfimageValues ;
mfint32Value ::=
sfint32Value |
[ ] |
[ sfint32Values ] ;
sfint32Values ::=
sfint32Value |
sfint32Value sfint32Values ;
mfnodeValue ::=
nodeStatement |
[ ] |
[ nodeStatements ] ;
nodeStatements ::=
nodeStatement |
nodeStatement nodeStatements ;
mfrotationValue ::=
sfrotationValue |
[ ] |
[ sfrotationValues ] ;
sfrotationValues ::=
sfrotationValue |
sfrotationValue sfrotationValues ;
mfstringValue ::=
sfstringValue |
[ ] |
[ sfstringValues ] ;
sfstringValues ::=
sfstringValue |
sfstringValue sfstringValues ;
mftimeValue ::=
sftimeValue |
[ ] |
[ sftimeValues ] ;
sftimeValues ::=
sftimeValue |
sftimeValue sftimeValues ;
mfvec2dValue ::=
sfvec2dValue |
[ ] |
[ sfvec2dValues] ;
sfvec2dValues ::=
sfvec2dValue |
sfvec2dValue sfvec2dValues ;
mfvec3dValue ::=
sfvec3dValue |
[ ] |
[ sfvec3dValues ] ;
sfvec3dValues ::=
sfvec3dValue |
sfvec3dValue sfvec3dValues ;
mfvec2fValue ::=
sfvec2fValue |
[ ] |
[ sfvec2fValues] ;
sfvec2fValues ::=
sfvec2fValue |
sfvec2fValue sfvec2fValues ;
mfvec3fValue ::=
sfvec3fValue |
[ ] |
[ sfvec3fValues ] ;
sfvec3fValues ::=
sfvec3fValue |
sfvec3fValue sfvec3fValues ;

