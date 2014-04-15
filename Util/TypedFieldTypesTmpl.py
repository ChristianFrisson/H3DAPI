
file_template = """
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004-2008, SenseGraphics AB
//    All Rights Reserved
//
/// \\file TypedFieldTypesTmpl.h
/// \\brief This file has been automatically generated from
/// the Util/TypedFieldTypesTmpl.py. It contains the definition of the
/// Types template class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TYPESFIELDTYPESTEMPL_H__
#define __TYPESFIELDTYPESTEMPL_H__

#define H3D_TYPES_TEMPLATE_LIST %s
#define H3D_TYPES_CLASS %s
#define H3D_TYPES_TEMPLATE_SPEC %s

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
  s = "Types< "
  for i in range( total_nr_classes ):
    if( i != 0 ): s += ", "
    s += "Type" + `i`
  s += " >\n"
  return s


def classDocumentation():
  return """
        /// The Types template can be used in the RequiredArgTypes or 
        /// OptionalArgTypes arguments of a TypedField template instantiation.
        /// It defines a list of types and the types of the fields routed
        /// must follow that order.
        /// A Types template can contain Field types and Any <> template
        /// instantiations. No AnyNumber <> template instantiations are
        /// allowed.
        ///
        /// \\par Examples:
        /// Types< SFInt32, MFInt32 > means that the first routed field should 
        /// be SFInt32 and the second a MFInt32
        /// \\par
        /// Types< Any< SFInt32, MFInt32 >, SFFloat > means that the first
        /// routed field should be either of type SFInt32 or MFInt32. The 
        /// second routed field should be SFFloat.
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
  s = "    struct Types< "
  for i in range( nr_classes_defined ):
    if( i != 0 ): s += ", "
    s += "Type" + `i`

  for i in range( nr_classes_defined, total_nr_classes ):
    if( nr_classes_defined > 0 or i != nr_classes_defined ): s += ", "
    s += "void"
  s += " > {\n"
  return s
  
def numberTypesFunc( nr_classes_defined ):
  s = "/// Returns the number of types specified as template arguments.\n"
  s += "/// \\returns The number of types specified in the template.\n"
  s += "///\n"
  return s + "      static int numberTypes() { return " + `nr_classes_defined` + "; }\n"

def checkTemplateArgumentsFunc( nr_classes_defined ):
  s =  "/// Checks that the template arguments is of valid form. \n"
  s += "/// \\throws Exception::InvalidTemplateArguments\n"
  s += "///\n"
  s += "    static void checkTemplateArguments() {\n"
  for i in range( nr_classes_defined ):
    s += "      TypesType< Type" + `i` + " >::checkTemplateArguments();\n"
  s += "    }\n"
  return s

def checkTypeFunc( nr_classes_defined ):
  s =  "/// Check that a field is of the correct type. \n"
  s += "/// \\param f The field to check the type of\n"
  s += "/// \\param index The index of the field in the template\n"
  s += "/// types specified in Type.\n"
  s += "/// \\throws Exception::InvalidType\n"
  s += "///\n"
  s += "    static void checkType( Field *f, int index ) { \n"
  for i in range( nr_classes_defined ):
    if( i != 0 ): s += "      else "
    s += "if( index == " + `i` + " ) {\n"
    s += "         TypesType< Type" + `i` + " >::checkType( f );\n"
    s += "      } \n"
  s += "    }\n"    
  return s

def generateTemplClasses( total_nr_classes ):
  s = classDocumentation()
  s += "  template< H3D_TYPES_TEMPLATE_SPEC > \n"
  s += "    struct Types {\n"
  s +=checkTemplateArgumentsFunc( total_nr_classes );
  s += "\n"
  s += numberTypesFunc( total_nr_classes );
  s += "\n"
  s +=checkTypeFunc( total_nr_classes );
  s +="     };\n"
  for i in range( 1, total_nr_classes ):
    s += templateDef( i, total_nr_classes )
    s += structDef( i, total_nr_classes )
    s +=checkTemplateArgumentsFunc( i );
    s += "\n"
    s += numberTypesFunc( i );
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

