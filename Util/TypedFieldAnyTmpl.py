
file_template = """
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004-2008, SenseGraphics AB
//    All Rights Reserved
//
/// \\file TypedFieldAnyTmpl.h
/// \\brief This file has been automatically generated from the 
/// Util/TypedFieldAnyTmpl.py. It contains the definition of the Any 
/// template class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TYPESFIELDANYTEMPL_H__
#define __TYPESFIELDANYTEMPL_H__

#define H3D_ANY_TEMPLATE_LIST %s
#define H3D_ANY_CLASS %s
#define H3D_ANY_TEMPLATE_SPEC %s

namespace H3D {
  namespace TypedFieldTypeCheck {
%s
}
}

#endif
"""

def typesListMacro( total_nr_classes ):
  s = ""
  for i in range( total_nr_classes ):
    if( i != 0 ): s += ", "
    s += "class Type" + `i`
  s += "\n"
  return s
  
def typesClassMacro( total_nr_classes ):
  s = "Any< "
  for i in range( total_nr_classes ):
    if( i != 0 ): s += ", "
    s += "Type" + `i`
  s += " >\n"
  return s

def classDocumentation():
  return """
        /// The Any template can be used in the RequiredArgTypes or 
        /// OptionalArgTypes arguments of a TypedField template instantiation.
        /// It defines that a routed Field can be any of the types specified 
        /// in the template.
        /// An Any template can contain Field types,
        /// No AnyNumber <> template and Types <> template instantiations are
        /// allowed.
        ///
        /// \\par Examples:
        /// Any< SFInt32, MFInt32 > means that the route should be either a 
        /// SFInt32 or a MFInt32
        /// \\par
        /// Any< SFInt32, MFInt32, SFFloat, MFFloat > means that the route
        /// should have any of the Field types listed
        ///   
"""

def fullTemplateDecl( total_nr_classes ):
  s = ""
  for i in range( total_nr_classes ):
    if( i != 0 ): s += ", "
    s += "class Type" + `i` + " = void"
  return s

def templateDef( nr_classes_defined, total_nr_classes ):
  s = "    template< "
  for i in range( nr_classes_defined ):
    if( i != 0 ): s += ", "
    s += "class Type" + `i`
  s += " >\n"
  return s

def structDef( nr_classes_defined, total_nr_classes ):
  s = "    struct Any< "
  for i in range( nr_classes_defined ):
    if( i != 0 ): s += ", "
    s += "Type" + `i`

  for i in range( nr_classes_defined, total_nr_classes ):
    if( nr_classes_defined > 0 or i != nr_classes_defined ): s += ", "
    s += "void"
  s += " > {\n"
  return s
  
def checkTemplateArgumentsFunc( nr_classes_defined ):
  s =  "/// Checks that the template arguments is of valid form. \n"
  s += "/// \\throws Exception::InvalidTemplateArguments\n"
  s += "///\n"
  s += "    static void checkTemplateArguments() {\n"
  for i in range( nr_classes_defined ):
    s += "      AnyType< Type" + `i` + " >::checkTemplateArguments();\n"
  s += "    }\n"
  return s

def checkTypeFunc( nr_classes_defined ):
  s =  "/// Check that a field is of the correct type. \n"
  s += "/// \\param f The field to check the type of\n"
  s += "/// \\param index The index of the field in the template\n"
  s += "/// types specified in Type.\n"
  s += "/// \\throws Exception::InvalidType\n"
  s += "///\n"
  s += "    static void checkType( Field *f ) { \n      if( \n"
  for i in range( nr_classes_defined ):
    if( i != 0 ): s += " &&\n"
    s += "         !AnyType< Type" + `i` + " >::checkType( f )"
  s += "        ) { \n         throw Exception::InvalidType( typeid( f ).name(),\n                                       typeid( Any< " 
  for i in range( nr_classes_defined ):
    if( i != 0 ): s += ", "
    s += "Type" + `i`
  s += "> ).name() ); \n      }\n    }\n"
  return s

def generateTemplClasses( total_nr_classes ):
  s = classDocumentation()
  s += "  template< H3D_ANY_TEMPLATE_SPEC > \n"
  s += "    struct Any {\n"
  s +=checkTemplateArgumentsFunc( total_nr_classes );
  s += "\n"
  s +=checkTypeFunc( total_nr_classes );
  s +="     };\n"
  for i in range( 1, total_nr_classes ):
    s += templateDef( i, total_nr_classes )
    s += structDef( i, total_nr_classes )
    s +=checkTemplateArgumentsFunc( i );
    s += "\n"
    s +=checkTypeFunc( i );
    s +="     };\n"
  return s 


def generateFile( total_nr_classes ):
  return file_template % (   typesListMacro( total_nr_classes ),
                            typesClassMacro( total_nr_classes ),
                            fullTemplateDecl( total_nr_classes ),
                            generateTemplClasses( total_nr_classes ) )

print generateFile( 40 )

