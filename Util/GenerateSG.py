########################################################################
#
# Script to generate the .h and .cpp files for each node in
# the X3D scene-graph
#

########################################################################
#
# Input
#
throw_exceptions = 1

########################################################################
#
# Imports
#
import string
import re

########################################################################
#
# String templates
#

header_templ="""
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \\file %s
/// \\brief UNIMPLEMENTED: Header file for %s, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __%s_H__
#define __%s_H__

#include <iostream>
#include "%s.h"
#include "FieldTemplates.h"
#include "Exception.h"

/// Standard Template Library namespace
using namespace std;

/// H3D namespace
using namespace H3D;

/// Exception handling namespace
using namespace Exception;

/// H3D API namespace
namespace H3D {

/// \\class %s
/// \\brief UNIMPLEMENTED: %s
///
%s
  class H3DAPI_API %s : public %s {
  public:
    %s

  /// newInstance() will generate an instance of this node, used
  /// when creating nodes via the H3DNodeDatabase service.
    static Node *newInstance() { return new %s; };

  /// getField() returns the field pointer to the field with the
  /// specified string field name, used when creating nodes via
  /// the H3DNodeDatabase service.
    virtual Field *getField( const string &name );

%s    

    // Fields
%s

  private:
    static H3DNodeDatabase database;
  };
}

#endif
"""

cpp_templ="""
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \\file %s
/// \\brief CPP file for %s, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "%s.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase %s::database( 
        "%s", 
        &(%s::newInstance) 
);

Field *%s::getField( const string &name ) {
%s
    return NULL;
}

%s

"""

constructor_def="""
    %s( %s );
"""
constructor_impl="""
%s::%s( 
%s ) :
%s {

%s 

%s

}
"""

def_cont_field_func = """    
  /// Returns the default xml containerField attribute value.
  /// For this node it is "%s".
  ///
  virtual string defaultXMLContainerField() {
     return "%s";
  }
"""


#######################################################################
#
# Utility functions
#

def Pad( str, length ):
  while len( str ) < length:
  str = str + " "
  return str

def PrePad( str, length ):
  while len( str ) < length:
  str = " " + str
  return str

def MaxFieldLen( fields ):
  maxacclen  = 0
  maxtypelen = 0
  maxnamelen = 0
  for f in fields:
  ( access, type, name, default ) = f
  if len(access) > maxacclen:
    maxacclen = len( access )
  if len(type) > maxtypelen:
    maxtypelen = len( type )
  if len(name) > maxnamelen:
    maxnamelen = len( name )

  return ( maxacclen, maxtypelen, maxnamelen )


#######################################################################
#
# Output generating functions
#

def GenerateConstructorDef( node, fields ):
  fieldtext = ""
  #fieldinit = ""
  first = 1
  ( maxacclen, maxtypelen, maxnamelen ) = MaxFieldLen( fields )
  for f in fields:
  ( access, type, name, default ) = f
  if first:
    first=0
    fieldtext = fieldtext + "Inst< %s<%s> > _%s = 0" % \
          ( PrePad( access, maxacclen  ), 
          Pad( type,   maxtypelen ), 
          Pad( name,   maxnamelen ) )
  else:
    fieldtext = fieldtext + ",\n           Inst< %s<%s> > _%s = 0"% \
          ( PrePad( access, maxacclen  ), 
          Pad( type,   maxtypelen ), 
          Pad( name,   maxnamelen ) )

  return constructor_def % ( node, fieldtext )


def GenerateConstructorImpl( node, base, fields ):
  global Inheritance

  fieldtext = ""
  fieldinit = ""
  fielddefaults = ""
  first = 1
  firstinit = 1
  ( maxacclen, maxtypelen, maxnamelen ) = MaxFieldLen( fields )
  for f in fields:
  ( access, type, name, default ) = f
  if default != "":
    fielddefaults = fielddefaults + "    //" + name + " = " + default + ";\n"

  if first:
    fieldtext = "    Inst< %s<%s> > _%s" % \
          ( PrePad( access, maxacclen  ), 
          Pad( type,   maxtypelen ), name )
    first=0
  else:
    fieldtext = fieldtext + ",\n    Inst< %s<%s> > _%s"% \
          ( PrePad( access, maxacclen  ), 
          Pad( type,   maxtypelen ), name )

  if firstinit:
    if base != "Node":
    if name not in Inheritance[base]:
      firstinit=0
      fieldinit = "      %s( _%s )" %\
            ( Pad( name, maxnamelen ), Pad( name, maxnamelen ) ) 
  else:
    if base != "Node":
    if name not in Inheritance[base]:
      fieldinit = fieldinit + ",\n      %s( _%s )" %\
            ( Pad( name, maxnamelen ), Pad( name, maxnamelen ) ) 
      
  # Call base class with fields from base class
  callbase=""
  if base != "Node":
  callbase="    "+base+"( "
  first=0
  for f in Inheritance[base]:
    if first==0:
    first=1
    callbase=callbase + "_" + f
    else:
    callbase=callbase + ", " + "_" + f 

  callbase=callbase + " )"
  if fieldinit != "":
    fieldinit = fieldinit + ",\n" + callbase
  else:
    fieldinit = callbase

  exception = ""
  if( throw_exceptions ):
    exception = "   cerr << \"Warning: " + node + " unimplemented!\" << endl;"

  str = constructor_impl % ( node, node, fieldtext, \
                fieldinit, exception, fielddefaults )

  #print str
  return str 

def FieldDefs( name, base, fields ):
  global Inheritance

  fieldtext = ""
  ( maxacclen, maxtypelen, maxnamelen ) = MaxFieldLen( fields )
  for f in fields:
  ( access, type, name, default ) = f
  # get base class fields
  if base != "Node":
    bf = Inheritance[base]
  else:
    bf = []

  if name not in bf:
    fieldtext = fieldtext + "    auto_ptr< %s< %s > > %s;\n" %\
          ( PrePad( access, maxacclen ),
          Pad( type, maxtypelen ), name )  
    
  return fieldtext

def GetFieldFunc( fields ):
  fieldtext = ""
  ( maxacclen, maxtypelen, maxnamelen ) = MaxFieldLen( fields )
  for f in fields:
  ( access, type, name, default ) = f
  fieldtext = fieldtext + "    if ( name == %s ) return %s.get();\n" %\
        ( Pad( "\""+name+"\"", maxnamelen+2) , name )
  return fieldtext
  
def_cont_field_dict = { "X3DAppearanceNode": "appearance",
                        "FillProperties": "fillProperties",
                        "LineProperties": "lineProperties",
                        "X3DMaterialNode": "material",
                        "X3DTextureNode": "texture",
                        "X3DTextureTransformNode" : "textureTransform",
                        "X3DGeometryNode" : "geometry",
                        "X3DColorNode": "color",
                        "X3DCoordinateNode": "coord",
                        "X3DNormalNode": "normal",
                        "X3DTextureCoordinateNode": "texCoord",
                        "X3DFontStyleNode": "fontStyle" }
  
def GetDefContField( name ):
  try:
    cont_field = def_cont_field_dict[ name ]
    return def_cont_field_func % ( cont_field, cont_field )
  except:
    return ""

#######################################################################
#
#nodes
#
def ExtractFields( fieldtext ):
  fields = []

  field_re = re.compile( r"""(?msx)
[\s\n]*([^\s]*)\s*([^\s]*)\s*([^\s]*)([^\n]*)\n
""" )
  m = field_re.search( fieldtext )
  while m:
  fieldtype   = fieldtext[m.start(1):m.end(1)]
  fieldaccess = fieldtext[m.start(2):m.end(2)]
  fieldname   = fieldtext[m.start(3):m.end(3)]
  fielddef    = fieldtext[m.start(4):m.end(4)]

  if fieldaccess == "[]":
    fieldaccessm = "initializeOnly"
  elif fieldaccess == "[in]":
    fieldaccessm = "inputOnly"
  elif fieldaccess == "[out]":
    fieldaccessm = "outputOnly"
  elif fieldaccess == "[in,out]":
    fieldaccessm = "inputOutput"
  else:
    fieldaccessm = "UNKNOWN"

  fields.append( ( fieldaccessm, fieldtype, fieldname, fielddef ) )
  m = field_re.search( fieldtext, m.end(0) )


  return fields




def ExtractNode( nodetext ):
  node = ()
  node_re = re.compile( r"""(?msx)
\s*([^\n\s]*)\s*:\s*([^\n\s]*)\s*{([^}]*)}\s*{([^}]*)}\s*{([^}]*)}
""" )
  m = node_re.search( nodetext )
  if m:
  nodename = nodetext[m.start(1):m.end(1)]
  print nodename
  (f,t) = m.span(2)
  nodebase = nodetext[f:t]
  (f,t) = m.span(3)
  fields = ExtractFields( nodetext[f:t] )
  (f,t) = m.span(4)
  briefdesc = nodetext[f:t]
  (f,t) = m.span(5)
  desc = nodetext[f:t]

  else:
  print "Something wrong!"
  return ( nodename, nodebase, fields, briefdesc, desc )




def LoadNodes():
  nodes = []
  f = open( "Nodes.dat", "r" )
  text = f.read()
  f.close()

  node_re = re.compile( r"\s*[^\n\s]*\s*:\s*[^\n]*\s*{[^}]*}\s*{[^}]*}\s*{[^}]*}" )
  m = node_re.search( text )
  while m:
  (fr, to) = m.span(0)
  nodes.append( ExtractNode( text[fr:to] ) )
  m = node_re.search( text, m.end(0) )
  
  return nodes

#######################################################################
#
# Main function:
#
nodes = LoadNodes()
Inheritance={}

# first generate Inheritance structures
for n in nodes:
  ( name, base, fields, briefdesc, desc ) = n
  print "Preprocessing " + name + ".h/.cpp...."
  fieldnames = []
  for f in fields:
  ( a, b, fname, d ) = f
  fieldnames.append( fname )

  Inheritance[name] = ( fieldnames )

for n in nodes:
  ( name, base, fields, briefdesc, desc ) = n
  print "Generating " + name + ".h/.cpp...."

  cdef = GenerateConstructorDef( name, fields )
  cimpl = GenerateConstructorImpl( name, base, fields )
  fielddefs = FieldDefs( name, base, fields )
  getfieldfunc = GetFieldFunc( fields )
  default_container_field_func = GetDefContField( name )

  header = header_templ %\
       ( name + ".h", name,
       string.upper( name ),
       string.upper( name ),
       base,
       name, briefdesc, desc, name,
       base,
       cdef,
       name,
       default_container_field_func,
       fielddefs )

  cpp = cpp_templ %\
    ( name + ".cpp", name, 
      name, name, name, name, name,
      getfieldfunc,
      cimpl )

  f = open( name + ".h", "w" )
  f.write( header )
  f.close

  f = open( name + ".cpp", "w" )
  f.write( cpp )
  f.close


