
from H3D import *

#import sys

#Console = H3DConsole()

#sys.stdout = Console
#sys.stderr = Console


## The base class for all Field types in Python.
## It provides common basic event handling methods for all fields.
class Field:
  ## The X3D Type of the value this field contains. This will be set
  ## by base classes to the type it handles.
  type = UNKNOWN_X3D_TYPE

  ## Contructor. 
  ## \param auto_update Specifies if the field should be using lazy
  ## evaluation or not. If auto_update is true the field will call
  ## its update function as soon as it receives an event instead of
  ## only when the value is asked for.
  def __init__( self, auto_update = 0 ):
    module = self.__class__.__dict__["__module__"]
    createField( self, auto_update, module + "." + self.__class__.__name__ )

  ## Set up a route from this field to another field. The function will
  ## generate an event to the destination field.
  ## \param dest The field to route to.
  def route( self, dest ):
    return routeField( self, dest )

  ## Set up a route from this field to another field without generating
  ## an event to the destination field. 
  ## \param dest The field to route to.
  def routeNoEvent( self, dest ):
    return routeFieldNoEvent( self, dest )

  ## Remove a route between this field and another field. If no such 
  ## route exists nothing happens.
  ## \param dest The field to remove the route to.
  def unroute( self, dest ):
    return unrouteField( self, dest )

#  def replaceRoute( self, dest, i ):
#    return unrouteField( self, dest )

#  def replaceRouteNoEvent( self, dest, i ):
#    return unrouteField( self, dest, i )

  ## Generate an event from this field.
  def touch( self ):
    return touchField( self )

  ## Returns a tuple containing all the fields that are routed to this field. 
  def getRoutesIn( self ):
    return getRoutesIn( self )

  ## Returns a tuple containing all the fields that this field is routed to.
  def getRoutesOut( self ):
    return getRoutesOut( self )

  ## Returns a int constant describing the field's access type.
  ##
  ## Possible return values: INITIALIZE_ONLY, OUTPUT_INPUT, INPUT_ONLY, INPUT_OUTPUT.
  ##
  def getAccessType( self ):
    return getFieldAccessType( self )
    
  ## Comparison operator. Two fields are equal if it encapsulates the same
  ## underlying C++-field.
  def __cmp__( self, o ):
    return getCPtr(self) - getCPtr(o)

## Base class for all Field types that contain a single value of a 
## specified type.
class SField( Field ):
  ## The X3D Type of the value this field contains.
  type = UNKNOWN_X3D_TYPE

  ## Set the value of the field.
  def setValue( self, value ):
    setFieldValue( self, value )

  ## Get the value of the field.
  def getValue( self ):
    return getFieldValue( self )

## Base class for all Field types that contain a multiple values of a 
## specified type. 
class MField( Field ):
  ## The X3D Type of the value this field contains.
  type = UNKNOWN_X3D_TYPE

  ## Set the value of the field.
  ## \param value The value is a list of values of the type of the MField.
  def setValue( self, value ):
    setFieldValue( self, value )

  ## Returns the value of this field.
  ## The returned value is a list of values.
  def getValue( self ):
    return getFieldValue( self )

  ## Add a value to the end of the list of values.
  ## \param v The value to add.
  def push_back( self, v ):
    pushBackElementInMField( self, v ) 

  ## Removes the last element. 
  def pop_back( self ):
    MFieldPopBack( self )

  ## Returns True if the MField contains no elements, False other wise.
  def empty( self ):
    return MFieldEmpty( self )

  ## Returns the first element in the MField.
  def front( self ):
    return MFieldFront( self )

  ## Returns the last element of the MField.
  def back( self ):
    return MFieldBack( self )

  ## Removes all elements from the MField.
  def clear( self ):
    MFieldClear( self )

  ## Removes an element from the MField. 
  ## \param v The element to remove.
  def erase( self, v ):
    eraseElementFromMField( self, v ) 
    
  ## Returns the size of the MField. 
  ## \return The number of elements in the MField.
  def size( self ):
    return MFieldSize( self )


# Install all built-in Field types:

#sfield_types = [ 
#  ( SFFLOAT,    "SFFloat" ),
#  ( SFDOUBLE,   "SFDouble" ),
#  ( SFTIME,     "SFTime" ),
#  ( SFINT32,    "SFInt32" ),
#  ( SFVEC2F,    "SFVec2f" ),
#  ( SFVEC2D,    "SFVec2d" ),
#  ( SFVEC3F,    "SFVec3f" ),
#  ( SFVEC3D,    "SFVec3d" ),
#  ( SFVEC4F,    "SFVec4f" ),
#  ( SFVEC4D,    "SFVec4d" ),
#  ( SFBOOL,     "SFBool"  ),
#  ( SFSTRING,   "SFString" ),
#  ( SFCOLOR,    "SFColor" ),
#  ( SFCOLORRGBA,"SFColorRGBA" ),
#  ( SFROTATION, "SFRotation" ),
#  ( SFQUATERNION, "SFQuaternion" ),
#  ( SFMATRIX3F, "SFMatrix3f" ),
#  ( SFMATRIX4F, "SFMatrix4f" ),
#  ( SFMATRIX3D, "SFMatrix3d" ),
#  ( SFMATRIX4D, "SFMatrix4d" ),
#  ( SFNODE    , "SFNode"     ) ]

# \internal
#mfield_types = [
#  ( MFFLOAT,    "MFFloat" ),
#  ( MFDOUBLE,   "MFDouble" ),
#  ( MFTIME,     "MFTime" ),
#  ( MFINT32,    "MFInt32" ),
#  ( MFVEC2F,    "MFVec2f" ),
#  ( MFVEC2D,    "MFVec2d" ),
#  ( MFVEC3F,    "MFVec3f" ),
#  ( MFVEC3D,    "MFVec3d" ),
#  ( MFVEC4F,    "MFVec4f" ),
#  ( MFVEC4D,    "MFVec4d" ),
#  ( MFBOOL,     "MFBool"  ),
#  ( MFSTRING,   "MFString" ),
#  ( MFCOLOR,    "MFColor" ),
#  ( MFCOLORRGBA,"MFColorRGBA" ),
#  ( MFROTATION, "MFRotation" ),
#  ( MFQUATERNION, "MFQuaternion" ),
#  ( MFMATRIX3F, "MFMatrix3f" ),
#  ( MFMATRIX4F, "MFMatrix4f" ),
#  ( MFMATRIX3D, "MFMatrix3d" ),
#  ( MFMATRIX4D, "MFMatrix4d" ),
#  ( MFNODE    , "MFNode"     )
#]

#for t in sfield_types:
#  exec """
#class %s( SField ):
#  type = %s
#""" % (t[1], t[0] )

#for t in mfield_types:
#  exec """
#class %s( MField ):
#  type = %s
#""" % (t[1], t[0] )


#typed_field_classes = {}


## \defgroup TemplateFunctions Field template functions.
## Template functions in H3DInterface is similar to templates in C++.
## They return a new class definition based on the paramters and is usually
## used to change the typing rules for a new field.
##
## <b>Example usage: </b>
## <pre>
## class KeyHandler( AutoUpdate( SFString ) ):
##   def update( self, event ):
##      key = event.getValue()
##      print key
##      return key
## </pre>
## 



## \ingroup TemplateFunctions
## Returns a class definition with specified typing rules.
## Works similar to the TypedField template in C++. It sets restrictions
## on what types of fields can be routed to the class.
## \param base_class A Field base class to change typing for.
## \param type_info A tuple of Field classes or a single field class
## that the class requires in order to work. 
## \param opt_type_info A tuple of Field classes or a single field class 
## which can be optionally routed to the class.
##
## <b>Example usage: </b>
## <pre>
## # Field class to convert a SFVec3f to a SFString.
## class Vec3fToStringField( TypedField( SFString, SFVec3f ) ):
##   def update( self, event ):
##      return str( event.getValue() )
##
## # Field class to convert a two SFVec3fs to a SFString.
## class Vec3fsToStringField( TypedField( SFString, (SFVec3f, SFVec3f) ) ):
##   def update( self, event ):
##      routes_in = self.getRoutesIn()
##      return str( routes_in[0].getValue() ) + str( routes_in[1].getValue() )
##
## # Field class to convert a SFVec3f to a SFString not requiring the route
## # to be set up.
## class OptVec3fToStringField( TypedField( SFString, None, SFVec3f ) ):
##   def update( self, event ):
##      routes_in = self.getRoutesIn()
##      if( len( routes_in ) > 0 ) :
##         return str( routes_in[0].getValue()
##      else :
##         return "No input"
## </pre>
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


#auto_update_classes = {}


## \ingroup TemplateFunctions
## Returns a class definition where the event update method has
## changed from lazy evaluation to eager evaluation.
## Works similar to the AutoUpdate template in C++. 
## The update function will be called as soon as the field receives
## an event instead of only when the value is asked for. Avoid this
## as much as possible since it can reduce performance if the update
## function is called a lot when not needed. Consider using 
## PeriodicUpdate as an alternative.
## 
## <b> Example usage: </b>
## <pre>
## # Field for handling keyboard event as soon as they happen.
## class KeyHandler( AutoUpdate( SFString ) ):
##   def update( self, event ):
##      key = event.getValue()
##      print key
##      return key
## </pre>
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

#periodic_update_classes = {}

## \ingroup TemplateFunctions
## Returns a class definition where the field value is checked
## that it is up-to-date each scene graph loop. If it is not it is 
## updated. This is a preferrable to AutoUpdate in many cases where it
## is not crucial that the update function has to be called on every 
## event but it is enough to check and update at a regular interval.
## 
## <b> Example usage: </b>
## <pre>
## # Update function will be called in the next scene-graph loop
## # after an event has been received even if the value is not 
## # requested by the user application.
## class UpdateHandler( PeriodicUpdate( SFString ) ):
##   def update( self, event ):
##      v = event.getValue()
##      print v
##      return key
## </pre>
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


# Documentation of classes generated in for loop above. 

## \namespace H3DInterface
## \class H3DInterface.SFFloat
## \extends SField
## \brief SField type containing a float value.

## \namespace H3DInterface
## \class H3DInterface.SFDouble
## \extends SField
## \brief SField type containing a double value.

## \namespace H3DInterface
## \class H3DInterface.SFTime
## \extends SField
## \brief SField type containing a double value representing time.

## \namespace H3DInterface
## \class H3DInterface.SFInt32
## \extends SField
## \brief SField type containing a 32 bit integer.

## \namespace H3DInterface
## \class H3DInterface.SFVec2f
## \extends SField
## \brief SField type containing a Vec2f value.

## \namespace H3DInterface
## \class H3DInterface.SFVec2d
## \extends SField
## \brief SField type containing a Vec2d value.

## \namespace H3DInterface
## \class H3DInterface.SFVec3f
## \extends SField
## \brief SField type containing a Vec3f value.

## \namespace H3DInterface
## \class H3DInterface.SFVec3d
## \extends SField
## \brief SField type containing a Vec3d value.

## \namespace H3DInterface
## \class H3DInterface.SFVec4f
## \extends SField
## \brief SField type containing a Vec4f value.

## \namespace H3DInterface
## \class H3DInterface.SFVec4d
## \extends SField
## \brief SField type containing a Vec4d value.

## \namespace H3DInterface
## \class H3DInterface.SFBool
## \extends SField
## \brief SField type containing a boolean value.

## \namespace H3DInterface
## \class H3DInterface.SFString
## \extends SField
## \brief SField type containing a string value.

## \namespace H3DInterface
## \class H3DInterface.SFColor
## \extends SField
## \brief SField type containing a RGB value.

## \namespace H3DInterface
## \class H3DInterface.SFRGBA
## \extends SField
## \brief SField type containing a RGBA value.

## \namespace H3DInterface
## \class H3DInterface.SFRotation
## \extends SField
## \brief SField type containing a Rotation value.

## \namespace H3DInterface
## \class H3DInterface.SFQuaternion
## \extends SField
## \brief SField type containing a Quaternion value.

## \namespace H3DInterface
## \class H3DInterface.SFMatrix3f
## \extends SField
## \brief SField type containing a Matrix3f.

## \namespace H3DInterface
## \class H3DInterface.SFMatrix3d
## \extends SField
## \brief SField type containing a Matrix3d.

## \namespace H3DInterface
## \class H3DInterface.SFMatrix4f
## \extends SField
## \brief SField type containing a Matrix4f.

## \namespace H3DInterface
## \class H3DInterface.SFMatrix4d
## \extends SField
## \brief SField type containing a Matrix4d.

## \namespace H3DInterface
## \class H3DInterface.SFNode
## \extends SField
## \brief SField type containing a H3D Node instance.



# MField classes

## \namespace H3DInterface
## \class H3DInterface.MFFloat
## \extends MField
## \brief MField type containing float values.

## \namespace H3DInterface
## \class H3DInterface.MFDouble
## \extends MField
## \brief MField type containing double values.

## \namespace H3DInterface
## \class H3DInterface.MFTime
## \extends MField
## \brief MField type containing double values representing time.

## \namespace H3DInterface
## \class H3DInterface.MFInt32
## \extends MField
## \brief MField type containing 32 bit integers.

## \namespace H3DInterface
## \class H3DInterface.MFVec2f
## \extends MField
## \brief MField type containing Vec2f values.

## \namespace H3DInterface
## \class H3DInterface.MFVec2d
## \extends MField
## \brief MField type containing Vec2d values.

## \namespace H3DInterface
## \class H3DInterface.MFVec3f
## \extends MField
## \brief MField type containing Vec3f values.

## \namespace H3DInterface
## \class H3DInterface.MFVec3d
## \extends MField
## \brief MField type containing Vec3d values.

## \namespace H3DInterface
## \class H3DInterface.MFVec4f
## \extends MField
## \brief MField type containing Vec4f values.

## \namespace H3DInterface
## \class H3DInterface.MFVec4d
## \extends MField
## \brief MField type containing Vec4d values.

## \namespace H3DInterface
## \class H3DInterface.MFBool
## \extends MField
## \brief MField type containing boolean values.

## \namespace H3DInterface
## \class H3DInterface.MFString
## \extends MField
## \brief MField type containing string values.

## \namespace H3DInterface
## \class H3DInterface.MFColor
## \extends MField
## \brief MField type containing RGB values.

## \namespace H3DInterface
## \class H3DInterface.MFRGBA
## \extends MField
## \brief MField type containing RGB values.

## \namespace H3DInterface
## \class H3DInterface.MFRotation
## \extends MField
## \brief MField type containing Rotation values.

## \namespace H3DInterface
## \class H3DInterface.MFQuaternion
## \extends MField
## \brief MField type containing Quaternion values.

## \namespace H3DInterface
## \class H3DInterface.MFMatrix3f
## \extends MField
## \brief MField type containing Matrix3f.

## \namespace H3DInterface
## \class H3DInterface.MFMatrix3d
## \extends MField
## \brief MField type containing Matrix3d.

## \package H3DInterface
## \class H3DInterface.MFMatrix4f
## \extends MField
## \brief MField type containing Matrix4f.

## \namespace H3DInterface
## \class H3DInterface.MFMatrix4d
## \extends MField
## \brief MField type containing Matrix4d.

## \namespace H3DInterface
## \class H3DInterface.MFNode
## \extends MField
## \brief MField type containing H3D Node instances.


# Documentation of functions from PythonMethods.h

## \namespace H3DInterface
## \fn def createNode( node_name )
## \brief Create a new Node instance of the given name.
## \param node_name Type name of the node to create, e.g. "Sphere" or "Group".
## \return A new instance of the node type specified. None if no such node type
## exists.
## 
## <b>Example usage: </b>
## \code
## sphere = createNode( "Sphere" )
## # Set color of sphere to red.
## sphere.diffuseColor.setValue( 1, 0, 0 )
## \endcode
def createNode( node_name ):
  pass

## \namespace H3DInterface
## \fn def createX3DFromURL( url )
## \brief Create a local H3D scene graph from the scene description in the file
## specified. The file can be in X3D/XML or X3D/VRML format.
## \param url The path to the file with the scene description. Can be a local
## path such as /home/ben/test.x3d or remote http://www.h3d.org/test.x3d.
## \return A tuple with a Group containing the top nodes of the file and 
## a dictionary with all the DEF nodes in the file for later access. 
## On error an exception will be thrown.
## 
## <b>Example usage: </b>
## \code
## test.x3d:
##
## <Shape>
##   <Appearance> <Material DEF="MAT"/> </Appearance>
##   <Sphere radius="0.01" />
## </Shape>
## \endcode
## \code
## group, dn = createX3DFromURL( "test.x3d" )
## # Set color of sphere to red.
## dn["MAT"].diffuseColor.setValue( 1, 0, 0 )
## \endcode
def createX3DFromURL( url ):
  pass

## \namespace H3DInterface
## \fn def createX3DFromString( s )
## \brief Create a local H3D scene graph from the scene description specified
## in a string. The string can be in X3D/XML or X3D/VRML format.
## \param s string with the scene description. 
## \return A tuple with a Group containing the top nodes of the file and 
## a dictionary with all the DEF nodes in the file for later access. On 
## error an exception will be thrown.
## 
## <b>Example usage: </b>
## \code
## s = """<Shape>
##   <Appearance> <Material DEF="MAT"/> </Appearance>
##   <Sphere radius="0.01" />
## </Shape>"""
##
## group, dn = createX3DFromString( s )
## # Set color of sphere to red.
## dn["MAT"].diffuseColor.setValue( 1, 0, 0 )
## \endcode
def createX3DFromString( url ):
  pass


## \namespace H3DInterface
## \fn def createX3DNodeFromURL( url )
## \brief Create a local H3D scene graph from the scene description in the file
## specified. The file can be in X3D/XML or X3D/VRML format.
## \param url The path to the file with the scene description. Can be a local
## path such as /home/ben/test.x3d or remote http://www.h3d.org/test.x3d.
## \return A tuple with the top node of the scene and a dictionary with
## all the DEF nodes in the file for later access. On error an exception
## will be thrown.
## 
## <b>Example usage: </b>
## \code
## test.x3d:
##
## <Shape>
##   <Appearance> <Material DEF="MAT"/> </Appearance>
##   <Sphere radius="0.01" />
## </Shape>
## \endcode
## \code
## node, dn = createX3DNodeFromURL( "test.x3d" )
## # Set color of sphere to red.
## dn["MAT"].diffuseColor.setValue( 1, 0, 0 )
## \endcode
def createX3DNodeFromURL( url ):
  pass

## \namespace H3DInterface
## \fn def createX3DNodeFromString( s )
## \brief Create a local H3D scene graph from the scene description specified
## in a string. The string can be in X3D/XML or X3D/VRML format.
## \param s string with the scene description. 
## \return A tuple with the top node of the scene and a dictionary with
## all the DEF nodes in the file for later access. On error an exception
## will be thrown.
## 
## <b>Example usage: </b>
## \code
## s = """<Shape>
##   <Appearance> <Material DEF="MAT"/> </Appearance>
##   <Sphere radius="0.01" />
## </Shape>"""
##
## node, dn = createX3DNodeFromString( s )
## # Set color of sphere to red.
## dn["MAT"].diffuseColor.setValue( 1, 0, 0 )
## \endcode
def createX3DNodeFromString( url ):
  pass

## \namespace H3DInterface
## \fn def createVRMLFromURL( url )
## \deprecated Use createX3DFromURL instead.
## \brief Deprecated. Same as createX3DFromURL.
def createVRMLFromURL( url ):
  pass

## \namespace H3DInterface
## \fn def createVRMLFromString( s )
## \deprecated Use createX3DFromString instead.
## \brief Deprecated. Same as createX3DFromString.
def createVRMLFromString( s ):
  pass

## \namespace H3DInterface
## \fn def createVRMLNodeFromURL( url )
## \deprecated Use createX3DNodeFromURL instead.
## \brief Deprecated. Same as createX3DFNoderomURL.
def createVRMLNodeFromURL( url ):
  pass

## \namespace H3DInterface
## \fn def createVRMLNodeFromString( s )
## \deprecated Use createX3DNodeFromString instead.
## \brief Deprecated. Same as createX3DNodeFromString.
def createVRMLNodeFromString( s ):
  pass

## \namespace H3DInterface 
## \fn def writeNodeAsX3D( node )
## Returns an X3D/XML string representing a node.
## This is a snapshot of all nodes and field values at the time
## and might not always be used to recreate the state of the scene
## depending on what nodes you use and your application.
## \param node The node to get a string representation for.
## \return An X3D/XML string representing the node.
def writeNodeAsX3D( node ):
  pass

## \namespace H3DInterface 
## \fn def getCurrentScenes()
## Returns a list of all Scene node instances that are created. 
def getCurrentScenes():
  pass

## \namespace H3DInterface 
## \fn def getActiveBindableNode( s )
## Returns the currently active bindable node from a bindable node stack.
## \param s The bindable stack to get the active node for.
## \return The currently active bindable node for the specified stack. None 
## if no such node exists or the specified bindable stack does not exist.
def getActiveBindableNode( s ):
  pass

## \namespace H3DInterface 
## \fn def getActiveDeviceInfo()
## Returns the currently active DeviceInfo node.
## \return The currently active DeviceInfo node. None if no DeviceInfo
## node exist.
def getActiveDeviceInfo():
  pass

## \namespace H3DInterface 
## \fn def getActiveViewpoint()
## Returns the currently active Viewpoint node.
## \return The currently active Viewpoint node. None if no Viewpoint
## node exist.
def getActiveViewpoint():
  pass

## \namespace H3DInterface 
## \fn def getActiveNavigationInfo()
## Returns the currently active NavigationInfo node.
## \return The currently active NavigationInfo node. None if no 
## NavigationInfo node exist.
def getActiveNavigationInfo():
  pass

## \namespace H3DInterface 
## \fn def getActiveStereoInfo()
## Returns the currently active StereoInfo node.
## \return The currently active StereoInfo node. None if no StereoInfo
## node exist.
def getActiveStereoInfo():
  pass

## \namespace H3DInterface 
## \fn def getActiveBackground()
## Returns the currently active Background node.
## \return The currently active Background node. None if no Background
## node exist.
def getActiveBackground():
  pass

## \namespace H3DInterface 
## \fn def getActiveFog()
## Returns the currently active Fog node.
## \return The currently active Fog node. None if no Fog
## node exist.
def getActiveFog():
  pass

## \namespace H3DInterface 
## \fn def getActiveGlobalSettings()
## Returns the currently active GlobalSettings node.
## \return The currently active GlobalSettings node. None if no 
## GlobalSettings node exist.
def getActiveGlobalSettings():
  pass

## \namespace H3DInterface 
## \fn def getNrHapticsDevices()
## Returns the number of currently specified haptics devices.
## Same as the number of devices specified in the currently bound
## DeviceInfo node.
## \return The number of currently specified haptics devices.
def getNrHapticsDevices():
  pass

## \namespace H3DInterface 
## \fn def getHapticsDevice( index )
## Returns the haptics device node with the specified index.
## \param index The index of the haptics device. 0 for the first device,
## 1 for second and so on.
## \return The H3DHapticsDevice node instance at the specified index. None
## if no node exists at the given index.
def getHapticsDevice( index ):
  pass

## \namespace H3DInterface 
## \fn def getNamedNode( DEF_name )
## Returns the node with the given DEF name from the parsed x3d file
## the PythonScript node that runs the current script was defined in.
## \param DEF_name The name of the node as specified by DEF keywork.
## \return The node with the specified name if it exists, None otherwise.
def getNamedNode( DEF_name ):
  pass

## \namespace H3DInterface 
## \fn def resolveURLAsFile( url )
## Resolves a url and returns the path a local file with the file content.
## This will include URNResolver lookups if active.
## \param url The url to resolve.
## \return string with path to local file. If the specified url could not
## be resolved "" is returned.
def resolveURLAsFile( url ):
  pass

## \namespace H3DInterface 
## \fn def resolveURLAsFolder( url )
## Resolves a url and returns the path a local folder it represents.
## This will include URNResolver lookups if active.
## \param url The url to resolve.
## \return string with path to local folder. If the specified url could not
## be resolved "" is returned.
def resolveURLAsFolder( url ):
  pass

## \namespace H3DInterface 
## \fn def throwQuitAPIException()
## Shut down the program.
## Throws a QuitAPI C++-exception. This can be used in H3DLoad and H3DViewer
## to exit the program and clean up.
def throwQuitAPIException():
  pass


## \namespace H3DInterface
## \var time 
## \brief Python access to the Scene::time field. 
## The time field value is updated once each scene-graph loop with the 
## current time.
time = SFTime

## \namespace H3DInterface
## \var eventSink 
## \brief Python access to the Scene::eventSink field. 
## The eventSink field will make sure that all fields routed to it is
## up-to-date once per scene-graph loop.
eventSink = Field



## \namespace H3DInterface
## \class Node
## \brief Python encapsulation of an H3D Node instance.
## Any node type is encapsulated by this class. Public fields
## from the encapsulated node can be accessed through attributes
## of the Python Node instance.
##
## <b>Example: </b>
## \code
## n = createX3DNodeFromString( "<Sphere />" )[0]
## # access the radius field of the sphere node by node attribute.
## n.radius.setValue( 0.2 ) 
## \endcode
class Node:

  ## Returns a new instance of this node type with the same state as this one
  ##
  ## The default implementation copies the node's registered field values
  ## of access type INPUT_OUTPUT and INITIALIZE_ONLY.
  ##
  ## \param deepCopy Bool: If true then references to other nodes will also be 
  ##                 cloned. Otherwise just the pointer is copied.
  ##
  def clone ( deepCopy ):
    pass

  ## Returns a list with all the fields of the node.
  ## \return A list with all the fields of the node.
  def getFieldList():
    pass

  ## Add a field to a node.
  ## This only works if the node is a H3DDynamicFieldsObject. If it is 
  ## not an exception will be thrown.
  ## \param name The name of the new field.
  ## \param field_type The type of the new field.
  ##  SFFLOAT,      MFFLOAT,
  ##  SFDOUBLE,     MFDOUBLE,
  ##  SFTIME,       MFTIME,
  ##  SFINT32,      MFINT32,
  ##  SFVEC2F,      MFVEC2F,
  ##  SFVEC2D,      MFVEC2D,
  ##  SFVEC3F,      MFVEC3F,
  ##  SFVEC3D,      MFVEC3D,
  ##  SFVEC4F,      MFVEC4F,
  ##  SFVEC4D,      MFVEC4D,
  ##  SFBOOL,       MFBOOL,
  ##  SFSTRING,     MFSTRING,
  ##  SFNODE,       MFNODE,
  ##  SFCOLOR,      MFCOLOR,
  ##  SFCOLORRGBA,  MFCOLORRGBA,
  ##  SFROTATION,   MFROTATION,
  ##  SFQUATERNION,   MFQUATERNION,
  ##  SFMATRIX3F,   MFMATRIX3F,
  ##  SFMATRIX4F,   MFMATRIX4F,
  ##  SFMATRIX3D,   MFMATRIX3D,
  ##  SFMATRIX4D,   MFMATRIX4D,
  ##  SFIMAGE,      MFIMAGE,
  ##  UNKNOWN_X3D_TYPE 
  ## \param access_type The access type of the new field. Must be one of
  ## "inputOnly", "outputOnly", "initializeOnly" or "inputOutput".
  def addField( name, field_type, access_type ):
    pass

  ## Returns a field from the node.
  ## \param field_name The name of the field in the node to return.
  def getField( field_name ):
    pass

  ## Returns the name of the node.
  ## \return The name of the node. If this node has been specified
  ## with a DEF keyword that will be the name. 
  def getName():
    pass

  ## Set the name of the node.
  ## \param new_name The new name of the node.
  def setName( new_name ):
    pass

  ## Returns a string specifying the encapsulated node's type.
  ## \return A string specifying the encapsulated node's type. E.g.
  ## a python node object encapsulating an IndexedFaceSet node will
  ## return "IndexedFaceSet".
  def getTypeName():
    pass

## The Vec2f class represents a 2d vector type using single precition 
## floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>v1 + v2</td><td>Component-wise addition. </td></tr>
## <tr><td>v1 - v2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>x * v1, v1 * x </td><td>Component-wise multiplication where x is a scalar value. </td></tr>
## <tr><td>v1 * v2 </td><td>Multiplication between to Vec2f types is defined as the dot product between the two vectors. </td></tr>
## <tr><td>v1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Component-wise negation. </td></tr>
## </table>
class Vec2f:
  ## Constructs a new Vec2f with its values set to (0,0)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Vec2f with values copied from another instance 
  ## of Vec2f or Vec2d.
  ## \param v The Vec2f or Vec2d to copy.
  def __init__( self, v ):
    pass

  ## Constructs a new Vec2f with its values set to (x,y)
  ## \param x The x value of the new vector.
  ## \param y The y value of the new vector.
  def __init__( self, x, y ):
    ## The x-component of the vector.
    self.x = x
    ## The y-component of the vector.
    self.y = y
    pass
  
  ## Returns the length of the vector.
  ## \return The length of the vector.
  def length( self ):
    pass

  ## Returns the length squared of the vector.
  ## This is a faster operation than length since it does not
  ## involve a square root. 
  ## \return The length squared of the vector.  
  def lengthSqr( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector an exception will
  ## be thrown.
  def normalize( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector this is a no-op.
  def normalizeSafe( self ):
    pass

  ## Returns the dot product between this vector and v2.
  ## \param v2 A Vec2f instance to calculate dot product with.
  ## \return The dot product between this vector and v2.
  def dotProduct( self, v2 ):
    pass


## The Vec2d class represents a 2d vector type using double precision
## floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>v1 + v2</td><td>Component-wise addition. </td></tr>
## <tr><td>v1 - v2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>x * v1, v1 * x </td><td>Component-wise multiplication where x is a scalar value. </td></tr>
## <tr><td>v1 * v2 </td><td>Multiplication between to Vec2d types is defined as the dot product between the two vectors. </td></tr>
## <tr><td>v1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Component-wise negation. </td></tr>
## </table>
class Vec2d:
  ## Constructs a new Vec2d with its values set to (0,0)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Vec2d with values copied from another instance 
  ## of Vec2f or Vec2d.
  ## \param v The Vec2f or Vec2d to copy.
  def __init__( self, v ):
    pass

  ## Constructs a new Vec2d with its values set to (x,y)
  ## \param x The x value of the new vector.
  ## \param y The y value of the new vector.
  def __init__( self, x, y ):
    ## The x-component of the vector.
    self.x = x
    ## The y-component of the vector.
    self.y = y
    pass
  
  ## Returns the length of the vector.
  ## \return The length of the vector.
  def length( self ):
    pass

  ## Returns the length squared of the vector.
  ## This is a faster operation than length since it does not
  ## involve a square root. 
  ## \return The length squared of the vector.  
  def lengthSqr( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector an exception will
  ## be thrown.
  def normalize( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector this is a no-op.
  def normalizeSafe( self ):
    pass

  ## Returns the dot product between this vector and v2.
  ## \param v2 A Vec2d instance to calculate dot product with.
  ## \return The dot product between this vector and v2.
  def dotProduct( self, v2 ):
    pass



## The Vec3f class represents a 3d vector type using single precision
## floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>v1 + v2</td><td>Component-wise addition. </td></tr>
## <tr><td>v1 - v2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>x * v1, v1 * x </td><td>Component-wise multiplication where x is a scalar value. </td></tr>
## <tr><td>v1 * v2 </td><td>Multiplication between to Vec3f types is defined as the dot product between the two vectors. </td></tr>
## <tr><td>v1 % v2 </td><td>Cross product. </td></tr>
## <tr><td>v1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Component-wise negation. </td></tr>
## </table>
class Vec3f:
  ## Constructs a new Vec3f with its values set to (0,0,0)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Vec3f with values copied from another instance 
  ## of Vec3f or Vec3d.
  ## \param v The Vec3f or Vec3d to copy.
  def __init__( self, v ):
    pass

  ## Constructs a new Vec3f with its values set to (x,y,z)
  ## \param x The x value of the new vector.
  ## \param y The y value of the new vector.
  ## \param z The z value of the new vector.
  def __init__( self, x, y, z ):
    ## The x-component of the vector.
    self.x = x
    ## The y-component of the vector.
    self.y = y
    ## The z-component of the vector.
    self.z = z
    pass
  
  ## Returns the length of the vector.
  ## \return The length of the vector.
  def length( self ):
    pass

  ## Returns the length squared of the vector.
  ## This is a faster operation than length since it does not
  ## involve a square root. 
  ## \return The length squared of the vector.  
  def lengthSqr( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector an exception will
  ## be thrown.
  def normalize( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector this is a no-op.
  def normalizeSafe( self ):
    pass

  ## Returns the dot product between this vector and v2.
  ## \param v2 A Vec3f instance to calculate dot product with.
  ## \return The dot product between this vector and v2.
  def dotProduct( self, v2 ):
    pass

  ## Returns the cross product between this vector and v2.
  ## \param v2 A Vec3f instance to calculate cross product with.
  ## \return The cross product between this vector and v2.
  def crossProduct( self, v2 ):
    pass



## The Vec3d class represents a 3d vector type using double precision
## floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>v1 + v2</td><td>Component-wise addition. </td></tr>
## <tr><td>v1 - v2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>x * v1, v1 * x </td><td>Component-wise multiplication where x is a scalar value. </td></tr>
## <tr><td>v1 * v2 </td><td>Multiplication between to Vec3d types is defined as the dot product between the two vectors. </td></tr>
## <tr><td>v1 % v2 </td><td>Cross product. </td></tr>
## <tr><td>v1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Component-wise negation. </td></tr>
## </table>
class Vec3d:
  ## Constructs a new Vec3d with its values set to (0,0,0)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Vec3d with values copied from another instance 
  ## of Vec3f or Vec3d.
  ## \param v The Vec3f or Vec3d to copy.
  def __init__( self, v ):
    pass

  ## Constructs a new Vec3d with its values set to (x,y,z)
  ## \param x The x value of the new vector.
  ## \param y The y value of the new vector.
  ## \param z The z value of the new vector.
  def __init__( self, x, y, z ):
    ## The x-component of the vector.
    self.x = x
    ## The y-component of the vector.
    self.y = y
    ## The z-component of the vector.
    self.z = z
    pass
  
  ## Returns the length of the vector.
  ## \return The length of the vector.
  def length( self ):
    pass

  ## Returns the length squared of the vector.
  ## This is a faster operation than length since it does not
  ## involve a square root. 
  ## \return The length squared of the vector.  
  def lengthSqr( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector an exception will
  ## be thrown.
  def normalize( self ):
    pass

  ## Normalize the vector values so the length is 1. 
  ## If the vector is a zero vector this is a no-op.
  def normalizeSafe( self ):
    pass

  ## Returns the dot product between this vector and v2.
  ## \param v2 A Vec3d instance to calculate dot product with.
  ## \return The dot product between this vector and v2.
  def dotProduct( self, v2 ):
    pass

  ## Returns the cross product between this vector and v2.
  ## \param v2 A Vec3d instance to calculate cross product with.
  ## \return The cross product between this vector and v2.
  def crossProduct( self, v2 ):
    pass


## The Vec4f class represents a 4d vector type or a homogeneous 
## coordinate using single precision floating point values for
## its components.
## 
## Operators supported:
## <table>
## <tr><td>v1 + v2</td><td>Component-wise addition. </td></tr>
## <tr><td>v1 - v2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>x * v1, v1 * x </td><td>Component-wise multiplication where x is a scalar value. </td></tr>
## <tr><td>v1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Component-wise negation. </td></tr>
## </table>
class Vec4f:

  ## Constructs a new Vec4f with its values set to (0,0,0,1)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Vec4f with values copied from another instance 
  ## of Vec4f or Vec4d.
  ## \param v The Vec4f or Vec4d to copy.
  def __init__( self, v ):
    pass

  ## Constructs a new Vec4f with its values set to (x,y,z,w)
  ## \param x The x value of the new vector.
  ## \param y The y value of the new vector.
  ## \param z The z value of the new vector.
  ## \param w The z value of the new vector.
  def __init__( self, x, y, z, w ):
    ## The x-component of the vector.
    self.x = x
    ## The y-component of the vector.
    self.y = y
    ## The z-component of the vector.
    self.z = z
    ## The w-component of the vector.
    self.w = w
    pass


## The Vec4d class represents a 4d vector type or a homogeneous 
## coordinate using double precision floating point values for
## its components.
## 
## Operators supported:
## <table>
## <tr><td>v1 + v2</td><td>Component-wise addition. </td></tr>
## <tr><td>v1 - v2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>x * v1, v1 * x </td><td>Component-wise multiplication where x is a scalar value. </td></tr>
## <tr><td>v1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Component-wise negation. </td></tr>
## </table>
class Vec4d:
  ## Constructs a new Vec4d with its values set to (0,0,0,1)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Vec4d with values copied from another instance 
  ## of Vec4f or Vec4d.
  ## \param v The Vec4f or Vec4d to copy.
  def __init__( self, v ):
    pass

  ## Constructs a new Vec4d with its values set to (x,y,z,w)
  ## \param x The x value of the new vector.
  ## \param y The y value of the new vector.
  ## \param z The z value of the new vector.
  ## \param w The z value of the new vector.
  def __init__( self, x, y, z, w ):
    ## The x-component of the vector.
    self.x = x
    ## The y-component of the vector.
    self.y = y
    ## The z-component of the vector.
    self.z = z
    ## The w-component of the vector.
    self.w = w
    pass


## The Matrix3f class represents a 3x3 matrix type using single
## precision floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>m1 + m2</td><td>Component-wise addition. </td></tr>
## <tr><td>m1 - m2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>m1 * m2 </td><td>Matrix multiplication. </td></tr>
## <tr><td>m1 * v </td><td>Matrix multiplication with column vector. v can be Vec3f or Vec3d</td></tr>
## <tr><td>m1 * x </td><td>Component-wise multiplication where x is a scalar value.</td></tr>
## <tr><td>m1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## </table>
class Matrix3f:

  ## Constructs a new idenity Matrix3f.
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Matrix3f with values copied from another instance 
  ## of Matrix3f or Matrix3d.
  ## \param m The Matrix3f or Matrix3d to copy.
  def __init__( self, m ):
    pass

  ## Casting constructor.
  ## Constructs a new Matrix3f rotation matrix from another rotation
  ## representation. 
  ## \param r A Rotation or Quaternion instance to create a rotation matrix for.
  def __init__( self, r ):
    pass

  ## Constructs a new Matrix3f with its values set to 
  ## ( m00, m01, m02,
  ##   m10, m11, m12,
  ##   m20, m21, m22 )
  ##
  ## \param m00 The value at position (0,0)
  ## \param m01 The value at position (0,1)
  ## \param m02 The value at position (0,2)
  ## \param m10 The value at position (1,0)
  ## \param m11 The value at position (1,1)
  ## \param m12 The value at position (1,2)
  ## \param m20 The value at position (2,0)
  ## \param m21 The value at position (2,1)
  ## \param m22 The value at position (2,2)
  def __init__( self, m00, m01, m02, m10, m11, m12, m20, m21, m22 ):
    pass

  ## Set this matrix to the identity matrix.
  def setToIdentity():
    pass

  ## Get the inverse of the matrix.
  ## \return The inverse of the matrix.
  def inverse():
    pass

  ## Get a row of the matrix.
  ## \param row The row to get(0-2).
  ## \return A Vec3f with the values from the row.
  def getRow( row ):
    pass

  ## Get a column of the matrix.
  ## \param column The column to get(0-2).
  ## \return A Vec3f with the values from the row.
  def getColumn( column ):
    pass

  ## Get an element of the matrix.
  ## \param row The row of the element to get(0-2)
  ## \param column The column of the element to get(0-2)
  ## \return The element at the given postion.
  def getElement( row, column ):
    pass

  ## Set an element of the matrix.
  ## \param row The row of the element to set(0-2)
  ## \param column The column of the element to set(0-2)
  ## \param value The value to set. 
  def setElement( row, column, value ):
    pass

  ## Get the scale part of each axis assuming the matrix is a
  ## transformation matrix.
  ## \return A Vec3f with the scaling factors for each axis.
  def getScalePart():
    pass

  ## Get the transpose of the matrix.
  ## \return The transpose of the matrix.
  def transpose():
    pass

  ## Get the euler angles( yaw, pitch, roll ) representation of 
  ## the rotation matrix. Assumes that the Matrix3f is a rotation matrix.
  ## \return A Vec3f with each components (yaw, pitch, roll) in radians.
  def toEulerAngles():
    pass



## The Matrix3d class represents a 3x3 matrix type using double
## precision floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>m1 + m2</td><td>Component-wise addition. </td></tr>
## <tr><td>m1 - m2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>m1 * m2 </td><td>Matrix multiplication. </td></tr>
## <tr><td>m1 * v </td><td>Matrix multiplication with column vector. v can be Vec3f or Vec3d</td></tr>
## <tr><td>m1 * x </td><td>Component-wise multiplication where x is a scalar value.</td></tr>
## <tr><td>m1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## </table>
class Matrix3d:

  ## Constructs a new idenity Matrix3d.
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Matrix3d with values copied from another instance 
  ## of Matrix3f or Matrix3d.
  ## \param m The Matrix3f or Matrix3d to copy.
  def __init__( self, m ):
    pass

  ## Casting constructor.
  ## Constructs a new Matrix3d rotation matrix from another rotation
  ## representation. 
  ## \param r A Rotation or Quaternion instance to create a rotation matrix for.
  def __init__( self, r ):
    pass

  ## Constructs a new Matrix3d with its values set to 
  ## ( m00, m01, m02,
  ##   m10, m11, m12,
  ##   m20, m21, m22 )
  ##
  ## \param m00 The value at position (0,0)
  ## \param m01 The value at position (0,1)
  ## \param m02 The value at position (0,2)
  ## \param m10 The value at position (1,0)
  ## \param m11 The value at position (1,1)
  ## \param m12 The value at position (1,2)
  ## \param m20 The value at position (2,0)
  ## \param m21 The value at position (2,1)
  ## \param m22 The value at position (2,2)
  def __init__( self, m00, m01, m02, m10, m11, m12, m20, m21, m22 ):
    pass

  ## Set this matrix to the identity matrix.
  def setToIdentity():
    pass

  ## Get the inverse of the matrix.
  ## \return The inverse of the matrix.
  def inverse():
    pass

  ## Get a row of the matrix.
  ## \param row The row to get(0-2).
  ## \return A Vec3d with the values from the row.
  def getRow( row ):
    pass

  ## Get a column of the matrix.
  ## \param column The column to get(0-2).
  ## \return A Vec3d with the values from the row.
  def getColumn( column ):
    pass

  ## Get an element of the matrix.
  ## \param row The row of the element to get(0-2)
  ## \param column The column of the element to get(0-2)
  ## \return The element at the given postion.
  def getElement( row, column ):
    pass

  ## Set an element of the matrix.
  ## \param row The row of the element to set(0-2)
  ## \param column The column of the element to set(0-2)
  ## \param value The value to set. 
  def setElement( row, column, value ):
    pass

  ## Get the scale part of each axis assuming the matrix is a
  ## transformation matrix.
  ## \return A Vec3d with the scaling factors for each axis.
  def getScalePart():
    pass

  ## Get the transpose of the matrix.
  ## \return The transpose of the matrix.
  def transpose():
    pass

  ## Get the euler angles( yaw, pitch, roll ) representation of 
  ## the rotation matrix. Assumes that the Matrix3d is a rotation matrix.
  ## \return A Vec3d with each components (yaw, pitch, roll) in radians.
  def toEulerAngles():
    pass



## The Matrix4f class represents a 3x3 matrix type using single
## precision floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>m1 + m2</td><td>Component-wise addition. </td></tr>
## <tr><td>m1 - m2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>m1 * m2 </td><td>Matrix multiplication. </td></tr>
## <tr><td>m1 * v </td><td>Matrix multiplication with column vector. v can be Vec3f, Vec3d, Vec4f or Vec4d. 3d vectors are assumed to have 1 in the fourth compnent.</td></tr>
## <tr><td>m1 * x </td><td>Component-wise multiplication where x is a scalar value.</td></tr>
## <tr><td>m1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## </table>
class Matrix4f:

  ## Constructs a new idenity Matrix4f.
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Matrix4f with values copied from another instance 
  ## of Matrix4f or Matrix4d.
  ## \param m The Matrix4f or Matrix4d to copy.
  def __init__( self, m ):
    pass

  ## Casting constructor.
  ## Constructs a new Matrix4f rotation matrix from another rotation
  ## representation. 
  ## \param r A Rotation or Quaternion instance to create a rotation matrix for.
  def __init__( self, r ):
    pass

  ## Constructs a new Matrix4f with its values set to 
  ## ( m00, m01, m02, m03
  ##   m10, m11, m12, m13
  ##   m20, m21, m22, m23,
  ##   m30, m31, m32, m33 )
  ##
  ## \param m00 The value at position (0,0)
  ## \param m01 The value at position (0,1)
  ## \param m02 The value at position (0,2)
  ## \param m03 The value at position (0,3)
  ## \param m10 The value at position (1,0)
  ## \param m11 The value at position (1,1)
  ## \param m12 The value at position (1,2)
  ## \param m13 The value at position (1,3)
  ## \param m20 The value at position (2,0)
  ## \param m21 The value at position (2,1)
  ## \param m22 The value at position (2,2)
  ## \param m23 The value at position (2,3)
  ## \param m30 The value at position (3,0)
  ## \param m31 The value at position (3,1)
  ## \param m32 The value at position (3,2)
  ## \param m33 The value at position (3,3)
  def __init__( self, m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 ):
    pass
    
  ## Transformation constructor.
  ## Creates a transformation matrix from translation and optional rotation and scale parameters.
  ## \param translation A Vec3f describing the translation.
  ## \param rotation A Rotation describing the rotation.
  ## \param scale A Vec3f describing the scale.
  def __init__( self, translation, rotation= None, scale= None ):
    pass

  ## Set this matrix to the identity matrix.
  def setToIdentity():
    pass

  ## Get the inverse of the matrix under the assumption that the matrix
  ## is a transformation matrix. This is faster than the inverse method.
  ## \return The inverse of the matrix. If the matrix is not a transformation
  ## matrix the result is undefined.
  def transformInverse():
    pass

  ## Get the inverse of the matrix.
  ## \return The inverse of the matrix.
  def inverse():
    pass

  ## Get a row of the matrix.
  ## \param row The row to get(0-3).
  ## \return A Vec4f with the values from the row.
  def getRow( row ):
    pass

  ## Get a column of the matrix.
  ## \param column The column to get(0-3).
  ## \return A Vec4f with the values from the row.
  def getColumn( column ):
    pass

  ## Get an element of the matrix.
  ## \param row The row of the element to get(0-3)
  ## \param column The column of the element to get(0-3)
  ## \return The element at the given postion.
  def getElement( row, column ):
    pass

  ## Set an element of the matrix.
  ## \param row The row of the element to set(0-3)
  ## \param column The column of the element to set(0-3)
  ## \param value The value to set. 
  def setElement( row, column, value ):
    pass

  ## Get the scale part of each axis assuming the matrix is a
  ## transformation matrix.
  ## \return A Vec3d with the scaling factors for each axis.
  def getScalePart():
    pass

  ## Get the rotation part of the matrix is a
  ## transformation matrix.
  ## \return A Matrix3d rotation matrix. 
  def getRotationPart():
    pass

  ## Get the part of the matrix that has nothing to do with translation.
  ## \return A Matrix3d with the upper-left 3x3 elements of the 4x4 matrix.
  def getScaleRotationPart():
    pass

  ## Get the translation from a transformation matrix.
  ## \return A Vec3d with the translation in each axis.
  def getTranslationPart():
    pass

  ## Get the transpose of the matrix.
  ## \return The transpose of the matrix.
  def transpose():
    pass


## The Matrix4d class represents a 3x3 matrix type using double
## precision floating point values for its components.
## 
## Operators supported:
## <table>
## <tr><td>m1 + m2</td><td>Component-wise addition. </td></tr>
## <tr><td>m1 - m2</td><td>Component-wise subtraction. </td></tr>
## <tr><td>m1 * m2 </td><td>Matrix multiplication. </td></tr>
## <tr><td>m1 * v </td><td>Matrix multiplication with column vector. v can be Vec3f, Vec3d, Vec4f or Vec4d. 3d vectors are assumed to have 1 in the fourth compnent.</td></tr>
## <tr><td>m1 * x </td><td>Component-wise multiplication where x is a scalar value.</td></tr>
## <tr><td>m1 / x </td><td>Component-wise division where x is a scalar value. </td></tr>
## </table>
class Matrix4d:

  ## Constructs a new idenity Matrix4d.
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Matrix4d with values copied from another instance 
  ## of Matrix4f or Matrix4d.
  ## \param m The Matrix4f or Matrix4d to copy.
  def __init__( self, m ):
    pass

  ## Casting constructor.
  ## Constructs a new Matrix4d rotation matrix from another rotation
  ## representation. 
  ## \param r A Rotation or Quaternion instance to create a rotation matrix for.
  def __init__( self, r ):
    pass

  ## Constructs a new Matrix4d with its values set to 
  ## ( m00, m01, m02, m03
  ##   m10, m11, m12, m13
  ##   m20, m21, m22, m23,
  ##   m30, m31, m32, m33 )
  ##
  ## \param m00 The value at position (0,0)
  ## \param m01 The value at position (0,1)
  ## \param m02 The value at position (0,2)
  ## \param m03 The value at position (0,3)
  ## \param m10 The value at position (1,0)
  ## \param m11 The value at position (1,1)
  ## \param m12 The value at position (1,2)
  ## \param m13 The value at position (1,3)
  ## \param m20 The value at position (2,0)
  ## \param m21 The value at position (2,1)
  ## \param m22 The value at position (2,2)
  ## \param m23 The value at position (2,3)
  ## \param m30 The value at position (3,0)
  ## \param m31 The value at position (3,1)
  ## \param m32 The value at position (3,2)
  ## \param m33 The value at position (3,3)
  def __init__( self, m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 ):
    pass
    
  ## Transformation constructor.
  ## Creates a transformation matrix from translation and optional rotation and scale parameters.
  ## \param translation A Vec3d describing the translation.
  ## \param rotation A Rotation describing the rotation (optional).
  ## \param scale A Vec3d describing the scale (optional).
  def __init__( self, translation, rotation= None, scale= None ):
    pass

  ## Set this matrix to the identity matrix.
  def setToIdentity():
    pass

  ## Get the inverse of the matrix under the assumption that the matrix
  ## is a transformation matrix. This is faster than the inverse method.
  ## \return The inverse of the matrix. If the matrix is not a transformation
  ## matrix the result is undefined.
  def transformInverse():
    pass

  ## Get the inverse of the matrix.
  ## \return The inverse of the matrix.
  def inverse():
    pass

  ## Get a row of the matrix.
  ## \param row The row to get(0-3).
  ## \return A Vec4d with the values from the row.
  def getRow( row ):
    pass

  ## Get a column of the matrix.
  ## \param column The column to get(0-3).
  ## \return A Vec4d with the values from the row.
  def getColumn( column ):
    pass

  ## Get an element of the matrix.
  ## \param row The row of the element to get(0-3)
  ## \param column The column of the element to get(0-3)
  ## \return The element at the given postion.
  def getElement( row, column ):
    pass

  ## Set an element of the matrix.
  ## \param row The row of the element to set(0-3)
  ## \param column The column of the element to set(0-3)
  ## \param value The value to set. 
  def setElement( row, column, value ):
    pass

  ## Get the scale part of each axis assuming the matrix is a
  ## transformation matrix.
  ## \return A Vec3d with the scaling factors for each axis.
  def getScalePart():
    pass

  ## Get the rotation part of the matrix is a
  ## transformation matrix.
  ## \return A Matrix3d rotation matrix. 
  def getRotationPart():
    pass

  ## Get the part of the matrix that has nothing to do with translation.
  ## \return A Matrix3d with the upper-left 3x3 elements of the 4x4 matrix.
  def getScaleRotationPart():
    pass

  ## Get the translation from a transformation matrix.
  ## \return A Vec3d with the translation in each axis.
  def getTranslationPart():
    pass

  ## Get the transpose of the matrix.
  ## \return The transpose of the matrix.
  def transpose():
    pass


## The Rotation class describes an arbitrary rotation. It specifies
## an axis to rotate around and the angle to rotate.
## 
## Operators supported:
## <table>
## <tr><td>r1 * r2 </td><td>Concatenation of rotations. </td></tr>
## <tr><td>r1 * v </td><td>Rotation of vector. v can be Vec3f or Vec3d</td></tr>
## <tr><td>r1 * x </td><td>Angle is multiplied by x. x is a scalar value.</td></tr>
## <tr><td>r1 / x </td><td>Angle is divided by x. x is a scalar value. </td></tr>
## <tr><td>-v1</td><td>Angle is negated. </td></tr>
## </table>
class Rotation:

  ## Constructs a new zero Rotation (1,0,0,0)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Rotation with values copied from another instance 
  ## of Rotation or Matrix3d.
  ## \param m The Rotation or Matrix3d to copy.
  def __init__( self, m ):
    pass

  ## Casting constructor.
  ## Constructs a new Rotation instance from another rotation
  ## representation. 
  ## \param r A Rotation or Matrix3f rotation matrix instance. Also 
  ## Vec3f and Vec3d instances representing euler angles are allowed.
  def __init__( self, r ):
    pass

  ## Constructs a new Rotation instance that is the closest rotation path
  ## between two normalized 3d vectors.
  ## The resulting Rotation is the Rotation object with the least angle
  ## that fulfills r * v1 = v2
  ## \param v1 Normalized Vec3f instance.
  ## \param v2 Normalized Vec3f instance.
  def __init__( self, v1, v2 ):
    pass

  ## Constructs a new Rotation instance.
  ##
  ## \param x The x-component of the rotation axis.
  ## \param y The y-component of the rotation axis.
  ## \param z The z-component of the rotation axis.
  ## \param angle The angle to rotate in radians.
  def __init__( self, x, y, z, angle ):
    ## The x-component of the rotation axis.
    self.x = x

    ## The y-component of the rotation axis.
    self.y = y

    ## The z-component of the rotation axis.
    self.z = z

    ## The angle to rotate in radians.
    self.angle = angle

    ## \deprecated Use angle instead.
    self.a = angle
    pass

  ## Spherical linear interpolation between two Rotation instances.
  ## 
  ## \param r Ending Rotation
  ## \param t Interpolation value between 0 and 1.
  ## \return The interpolated Rotation value.
  def slerp( r, t ):
    pass

  ## Get the euler angles( yaw, pitch, roll ) representation of 
  ## the Rotation. 
  ## \return A Vec3f with each components (yaw, pitch, roll) in radians.
  def toEulerAngles():
    pass




## The Quaternion class represents quaternion objects that are used
## for rotation operations.
## 
## Operators supported:
## <table>
## <tr><td>q1 + q2 </td><td>Quaternion addition. </td></tr>
## <tr><td>q1 - q2 </td><td>Quaternion subtraction. </td></tr>
## <tr><td>q1 * v </td><td>Rotation of vector(assumes q1 is normalized). v can be Vec3f or Vec3d</td></tr>
## <tr><td>q1 * x </td><td>Multiplication with scalar value.</td></tr>
## <tr><td>q1 / x </td><td>Division with scalar value. </td></tr>
## <tr><td>-q1</td><td>Negation. </td></tr>
## </table>
class Quaternion:

  ## Constructs a new zero Quaternion (0,0,0,0)
  def __init__( self ):
    pass
  
  ## Copy constructor.
  ## Constructs a new Quaternion with values copied from another instance 
  ## of Quaternion.
  ## \param q The Quaternion to copy.
  def __init__( self, q ):
    pass

  ## Casting constructor.
  ## Constructs a new Quaternion instance from another rotation
  ## representation. 
  ## \param r A Rotation or Matrix3f rotation matrix instance. 
  def __init__( self, r ):
    pass

  ## Constructs a new Quaternion instance that is the closest rotation path
  ## between two normalized 3d vectors.
  ## The resulting Quaternion is the Quaternion object with the least angle
  ## that fulfills r * v1 = v2
  ## \param v1 Normalized Vec3f instance.
  ## \param v2 Normalized Vec3f instance.
  def __init__( self, v1, v2 ):
    pass

  ## Constructs a new Quaternion instance.
  ##
  ## \param x The x-component of the quaternion vector part.
  ## \param y The y-component of the quaternion vector part.
  ## \param z The z-component of the quaternion vector part.
  ## \param w The quaternion scalar part.
  def __init__( self, x, y, z, w ):
    ## The x-component of the quaternion vector part.
    self.x = x

    ## The y-component of the quaternion vector part.
    self.y = y

    ## The z-component of the quaternion vector part.
    self.z = z

    ##  The quaternion scalar part.
    self.w = w

  ## Spherical linear interpolation between two Quaternion instances.
  ## 
  ## \param r Ending Quaternion
  ## \param t Interpolation value between 0 and 1.
  ## \return The interpolated Quaternion value.
  def slerp( r, t ):
    pass

  ## Get the euler angles( yaw, pitch, roll ) representation of 
  ## the Quaternion. 
  ## \return A Vec3f with each components (yaw, pitch, roll) in radians.
  def toEulerAngles():
    pass

  ## Get the quaternion norm.
  ## \return The quaternion norm.
  def norm():
    pass

  ## Normalize the Quaternion, i.e. scale it so that the magnitude
  ## is 1.
  def normalize():
    pass

  ## Get the conjugate of the quaternion.
  ## \return Quaternion that is the conjugate of the quaternion instance.
  def conjugate():
    pass

  ## Get the inverse of the quaternion.
  ## \return Quaternion that is the inverse of the quaternion instance.
  def inverse():
    pass

  ## Quaternion dot product.
  ## \param q Quaternion instance to perform dot product with.
  ## \return The quaternion dot product, i.e. cos(angle) between
  ## two unit quaternions.
  def dotProduct( q ):
    pass


## The RGB class represents a RGB color with red, green and blue components.
class RGB:
  ## Constructs a new RGB with its values set to (0,0,0)
  def __init__( self ):
    pass
  
  ## Constructs a new RGB with its values set to (r,g,b)
  ## \param r The amount of red in the color(0-1)
  ## \param g The amount of green in the color(0-1)
  ## \param b The amount of blue in the color(0-1)
  def __init__( self, r, g, b ):
    ## The amount of red in the color(0-1)
    self.r = r
    ## The amount of green in the color(0-1)
    self.g = g
    ## The amount of blue in the color(0-1)
    self.b = b


## The RGBA class represents a RGBA color with red, green, blue and alpha
## components.
class RGBA:
  ## Constructs a new RGBA with its values set to (0,0,0,1)
  def __init__( self ):
    pass
  
  ## Constructs a new RGBA with its values set to (r,g,b,a)
  ## \param r The amount of red in the color(0-1)
  ## \param g The amount of green in the color(0-1)
  ## \param b The amount of blue in the color(0-1)
  ## \param a The alpha value (0-1)
  def __init__( self, r, g, b, a ):
    ## The amount of red in the color(0-1)
    self.r = r
    ## The amount of green in the color(0-1)
    self.g = g
    ## The amount of blue in the color(0-1)
    self.b = b

    ## The alpha value (0-1)
    self.a = a
