
from H3D import *

import sys

Console = H3DConsole()

sys.stdout = Console
sys.stderr = Console

###################################################################
#
# Base types; Vec2f, Vec3f, Vec2d, Vec2d, etc, etc, etc
#
###################################################################

###################################################################
#
# Field base class and SF* MF* class definitions
#
###################################################################
class Field:
  type = UNKNOWN_X3D_TYPE
  def __init__( self, auto_update = 0 ):
    module = self.__class__.__dict__["__module__"]
    createField( self, auto_update, module + "." + self.__class__.__name__ )

  def setName( self, name ):
    return fieldSetName( self, name )

  def getName( self ):
    return fieldGetName( self )
    
  def getFullName( self ):
    return fieldGetFullName( self )
 
  def getTypeName( self ):
    return fieldGetTypeName( self )

  def getOwner( self ):
    return fieldGetOwner( self )

  def setOwner( self, n ):
    return fieldSetOwner( self, n )

  def route( self, dest ):
    return fieldRoute( self, dest )

  def routeNoEvent( self, dest ):
    return fieldRouteNoEvent( self, dest )

  def unroute( self, dest ):
    return fieldUnroute( self, dest )

  def replaceRoute( self, dest, i ):
    return fieldReplaceRoute( self, dest, i )

  def replaceRouteNoEvent( self, dest, i ):
    return fieldReplaceRouteNoEvent( self, dest, i )

  def unrouteAll( self ):
    return fieldUnrouteAll( self )
    
  def touch( self ):
    return fieldTouch( self )

  def routesTo( self, f ):
    return fieldRoutesTo( self, f )

  def hasRouteFrom( self, f ):
    return fieldHasRouteFrom( self, f )

  def getRoutesIn( self ):
    return fieldGetRoutesIn( self )

  def getRoutesOut( self ):
    return fieldGetRoutesOut( self )

  def setAccessType( self, access_type ):
    return fieldSetAccessType( self, access_type )

  def getAccessType( self ):
    return fieldGetAccessType( self )

  def setAccessCheck( self, access_check ):
    return fieldSetAccessCheck( self, access_check )

  def isAccessCheckOn( self ):
    return fieldIsAccessCheckOn( self )

  def setValueFromString( self, value ):
    return fieldSetValueFromString( self, value )

  def getValueAsString( self ):
    return fieldGetValueAsString( self )

  def upToDate( self ):
    return fieldUpToDate( self )

  def isUpToDate( self ):
    return fieldIsUpToDate( self )
    
  def __cmp__( self, o ):
    return getCPtr(self) - getCPtr(o)

class SField( Field ):
  type = UNKNOWN_X3D_TYPE
  def setValue( self, value ):
    fieldSetValue( self, value )

  def getValue( self ):
    return fieldGetValue( self )

class MField( Field ):
  type = UNKNOWN_X3D_TYPE
  def setValue( self, value ):
    fieldSetValue( self, value )

  def getValue( self ):
    return fieldGetValue( self )

  def push_back( self, v ):
    MFieldPushBack( self, v ) 

  def pop_back( self ):
    MFieldPopBack( self )

  def empty( self ):
    return MFieldEmpty( self )

  def front( self ):
    return MFieldFront( self )

  def back( self ):
    return MFieldBack( self )

  def clear( self ):
    MFieldClear( self )

  def erase( self, v ):
    MFieldErase( self, v ) 
    
  def size( self ):
    return MFieldSize( self )


# Install all built-in Field types:
sfield_types = [ 
  ( SFFLOAT,    "SFFloat" ),
  ( SFDOUBLE,   "SFDouble" ),
  ( SFTIME,     "SFTime" ),
  ( SFINT32,    "SFInt32" ),
  ( SFVEC2F,    "SFVec2f" ),
  ( SFVEC2D,    "SFVec2d" ),
  ( SFVEC3F,    "SFVec3f" ),
  ( SFVEC3D,    "SFVec3d" ),
  ( SFVEC4F,    "SFVec4f" ),
  ( SFVEC4D,    "SFVec4d" ),
  ( SFBOOL,     "SFBool"  ),
  ( SFSTRING,   "SFString" ),
  ( SFCOLOR,    "SFColor" ),
  ( SFCOLORRGBA,"SFColorRGBA" ),
  ( SFROTATION, "SFRotation" ),
  ( SFQUATERNION, "SFQuaternion" ),
  ( SFMATRIX3F, "SFMatrix3f" ),
  ( SFMATRIX4F, "SFMatrix4f" ),
  ( SFMATRIX3D, "SFMatrix3d" ),
  ( SFMATRIX4D, "SFMatrix4d" ),
  ( SFNODE    , "SFNode"     ) ]

mfield_types = [
  ( MFFLOAT,    "MFFloat" ),
  ( MFDOUBLE,   "MFDouble" ),
  ( MFTIME,     "MFTime" ),
  ( MFINT32,    "MFInt32" ),
  ( MFVEC2F,    "MFVec2f" ),
  ( MFVEC2D,    "MFVec2d" ),
  ( MFVEC3F,    "MFVec3f" ),
  ( MFVEC3D,    "MFVec3d" ),
  ( MFVEC4F,    "MFVec4f" ),
  ( MFVEC4D,    "MFVec4d" ),
  ( MFBOOL,     "MFBool"  ),
  ( MFSTRING,   "MFString" ),
  ( MFCOLOR,    "MFColor" ),
  ( MFCOLORRGBA,"MFColorRGBA" ),
  ( MFROTATION, "MFRotation" ),
  ( MFQUATERNION, "MFQuaternion" ),
  ( MFMATRIX3F, "MFMatrix3f" ),
  ( MFMATRIX4F, "MFMatrix4f" ),
  ( MFMATRIX3D, "MFMatrix3d" ),
  ( MFMATRIX4D, "MFMatrix4d" ),
  ( MFNODE    , "MFNode"     )
]

for t in sfield_types:
  exec """
class %s( SField ):
  type = %s
""" % (t[1], t[0] )

for t in mfield_types:
  exec """
class %s( MField ):
  type = %s
""" % (t[1], t[0] )


typed_field_classes = {}

def TypedField( base_class, type_info = None, opt_type_info = None ):
  class TypedBase( base_class ):
    pass
  global typed_field_classes
  if( typed_field_classes.has_key( (base_class, type_info, opt_type_info) ) ):
    return typed_field_classes[(base_class, type_info, opt_type_info)]

  if type_info == None:
    TypedBase.__type_info__ = ()
  elif type( type_info ) != type(()):
    TypedBase.__type_info__ = ( type_info, )
  else:
    TypedBase.__type_info__ = type_info

  if opt_type_info == None:
    TypedBase.__opt_type_info__ = ()
  elif type( opt_type_info ) != type(()):
    TypedBase.__opt_type_info__ = ( opt_type_info, )
  else:
    TypedBase.__opt_type_info__ = opt_type_info

  typed_field_classes[(base_class, type_info, opt_type_info)] = TypedBase
  return TypedBase


auto_update_classes = {}


# AutoUpdate "template" as in C++
def AutoUpdate( base_class ):
  class AutoUpdateBase( base_class ):
    def __init__( self ):
      base_class.__init__( self, 1 )

  global auto_update_classes
  if( auto_update_classes.has_key( base_class ) ):
    return auto_update_classes[base_class]
  else:
    auto_update_classes[base_class] = AutoUpdateBase
    return AutoUpdateBase

periodic_update_classes = {}

def PeriodicUpdate( base_class ):
  class PeriodicUpdateBase( base_class ):
    def __init__( self ):
      base_class.__init__( self, 0 )
      self.route( eventSink )
  
  global periodic_update_classes
  if( periodic_update_classes.has_key( base_class ) ):
    return periodic_update_classes[base_class]
  else:
    periodic_update_classes[base_class] = PeriodicUpdateBase
    return PeriodicUpdateBase
