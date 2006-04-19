
%{
#define YYERROR_VERBOSE  1
%}


%union {
  char* val;
}

%token ID
%token STRING
%token AS
%token VRMLNULL
%token SCRIPT

%token PROFILE
%token COMPONENT
%token EXPORT
%token IMPORT
%token META
%token DEF
%token USE
%token PROTO
%token inputOnly
%token outputOnly
%token initializeOnly
%token inputOutput
%token EXTERNPROTO
%token ROUTE
%token TO
%token IS

%token MFBool
%token MFColor
%token MFColorRGBA
%token MFDouble
%token MFFloat
%token MFImage
%token MFInt32
%token MFNode
%token MFRotation
%token MFString
%token MFTime
%token MFVec2d
%token MFVec2f
%token MFVec3d
%token MFVec3f
%token SFBool
%token SFColor
%token SFColorRGBA
%token SFDouble
%token SFFloat
%token SFImage
%token SFInt32
%token SFNode
%token SFRotation
%token SFString
%token SFTime
%token SFVec2d
%token SFVec2f
%token SFVec3d
%token SFVec3f


%%

x3dScene :
       profileStatement 
       componentStatements
       metaStatements
       statements

profileStatement:      PROFILE profileNameId;

profileNameId:         ID ;

componentStatements:     componentStatement |
                         componentStatement componentStatements |
                         empty ;

componentStatement:      COMPONENT componentNameId:componentSupportLevel ;

componentNameId:         ID ;

componentSupportLevel:   STRING ;

exportStatement:         EXPORT nodeNameId AS exportedNodeNameId ;

importStatement:         IMPORT inlineNodeNameId '.' exportedNodeNameId 
                         AS nodeNameId ;

metaStatements:          metaStatement |
                         metaStatement metaStatements |
                         empty ;

metaStatement:           META metakey metavalue ;

metakey:                 STRING ;

metavalue:               STRING ;

statements:              statement |
                         statement statements |
                         empty ;

statement:               nodeStatement | 
                         importStatement |
                         protoStatement |
                         routeStatement ;

nodeStatement:           node  |
                         DEF nodeNameId node |
                         USE nodeNameId ;

rootNodeStatement:       node | 
                         DEF nodeNameId node ;

protoStatement:          proto |
                         externproto ;

protoStatements:         protoStatement |
                         protoStatement protoStatements |
                         empty ;

proto:                   PROTO nodeTypeId '[' interfaceDeclarations ']' 
                         '{' protoBody '}' ;

protoBody:               protoStatements rootNodeStatement statements ;

interfaceDeclarations:   interfaceDeclaration |
                         interfaceDeclaration interfaceDeclarations |
                         empty ;

restrictedInterfaceDeclaration: 
                         inputOnly fieldType inputOnlyId |
                         outputOnly fieldType outputOnlyId |
                         initializeOnly fieldType initializeOnlyId fieldValue;

interfaceDeclaration:    restrictedInterfaceDeclaration |
                         inputOutput fieldType fieldId fieldValue ;

externproto:             EXTERNPROTO nodeTypeId 
                         '[' externInterfaceDeclarations ']' URLList ;

externInterfaceDeclarations:
                         externInterfaceDeclaration |
                         externInterfaceDeclaration 
                         externInterfaceDeclarations |
                         empty ;
externInterfaceDeclaration:
                         inputOnly fieldType inputOnlyId |
                         outputOnly fieldType outputOnlyId |
                         initializeOnly fieldType initializeOnlyId |
                         inputOutput fieldType ID ;

routeStatement:          ROUTE nodeNameId '.' outputOnlyId 
                         TO nodeNameId '.' inputOnlyId ;

URLList:                 mfValue ;

empty:                   ;


node:                    nodeTypeId { printf("%s\n", yylval.val ); } '{' nodeBody '}' |
                         SCRIPT '{' scriptBody '}' ;

nodeBody:                nodeBodyElement |
                         nodeBodyElement nodeBody |
                         empty ;

scriptBody:              scriptBodyElement |
                         scriptBodyElement scriptBody |
                         empty ;

scriptBodyElement:       nodeBodyElement |
                         restrictedInterfaceDeclaration |
                         inputOnly fieldType inputOnlyId IS inputOnlyId |
                         outputOnly fieldType outputOnlyId IS outputOnlyId |
                         initializeOnly fieldType initializeOnlyId IS
                         initializeOnlyId |
                         inputOutput fieldType inputOutputId IS inputOutputId;

nodeBodyElement:         initializeOnlyId fieldValue |
                         initializeOnlyId IS initializeOnlyId |
                         inputOnlyId IS inputOnlyId |
                         outputOnlyId IS outputOnlyId |
                         inputOutputId IS inputOutputId |
                         routeStatement |
                         protoStatement ;

nodeNameId:              ID ;

nodeTypeId:              ID ;

inlineNodeNameId:        ID ;

exportedNodeNameId:      ID ;

nodeTypeId:              ID ;

initializeOnlyId:        ID ;

inputOnlyId:             ID ;

inputOutputId:           ID ;

outputOnlyId:            ID ;

fieldType:              MFBool |
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

fieldValue:             sfValue |
                        mfValue |
                        sfnodeValue |
                        mfnodeValue;

sfValue:                STRING;

mfValue:                sfValue |
                        '[' ']' |
                        '[' sfValue ']' ;


sfnodeValue:            nodeStatement |
                        VRMLNULL ;

mfnodeValue:            nodeStatement |
                        '[' ']' |
                        '[' nodeStatements ']' ;

nodeStatements:         nodeStatement |
                        nodeStatement nodeStatements ;

fieldId:                STRING;

%%

int yyerror( char const *e ) {
     printf( "%s\n", e );
}


int main (void) {
  return yyparse ();
}
