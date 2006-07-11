
from H3D import *

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

  def route( self, dest ):
    return routeField( self, dest )

  def routeNoEvent( self, dest ):
    return routeFieldNoEvent( self, dest )

  def unroute( self, dest ):
    return unrouteField( self, dest )

#  def replaceRoute( self, dest, i ):
#    return unrouteField( self, dest )

#  def replaceRouteNoEvent( self, dest, i ):
#    return unrouteField( self, dest, i )

  def touch( self ):
    return touchField( self )

  def getRoutesIn( self ):
    return getRoutesIn( self )

  def getRoutesOut( self ):
    return getRoutesOut( self )

  def __cmp__( self, o ):
    return getCPtr(self) - getCPtr(o)

class SField( Field ):
  type = UNKNOWN_X3D_TYPE
  def setValue( self, value ):
    setFieldValue( self, value )

  def getValue( self ):
    return getFieldValue( self )

class MField( Field ):
  type = UNKNOWN_X3D_TYPE
  def setValue( self, value ):
    setFieldValue( self, value )

  def getValue( self ):
    return getFieldValue( self )

  def push_back( self, v ):
    pushBackElementInMField( self, v ) 

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
    eraseElementFromMField( self, v ) 


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




