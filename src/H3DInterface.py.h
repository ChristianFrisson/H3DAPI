////////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file H3DInterface.py.h
/// \brief header file containing H3DInterface python module strings.
///
//
//////////////////////////////////////////////////////////////////////////////

namespace H3D {

  namespace H3DInterface {
    // const string H3DInterface_string = "print \"TEST\" \n";

    const string H3DInterface_string = "\
from H3D import * \n\
\n\
import sys\n\
\n\
Console = H3DConsole()\n\
\n\
sys.stdout = Console\n\
sys.stderr = Console\n\
\n\
###################################################################\n\
#\n\
# Base types; Vec2f, Vec3f, Vec2d, Vec2d, etc, etc, etc\n\
#\n\
###################################################################\n\
\n\
###################################################################\n\
#\n\
# Field base class and SF* MF* class definitions\n\
#\n\
###################################################################\n\
class Field:\n\
  type = UNKNOWN_X3D_TYPE\n\
  def __init__( self, auto_update = 0 ):\n\
    module = self.__class__.__dict__[\"__module__\"]\n\
    createField( self, auto_update, module + \".\" + self.__class__.__name__ )\n\
\n\
  def setName(self, name):\n\
    return fieldSetName( self, name )\n\
\n\
  def getName(self):\n\
    return fieldGetName( self )\n\
\n\
  def getFullName(self):\n\
    return fieldGetFullName( self )\n\
\n\
  def getTypeName(self):\n\
    return fieldGetTypeName( self )\n\
\n\
  def route( self, dest ):\n\
    return routeField( self, dest )\n\
\n\
  def routeNoEvent( self, dest ):\n\
    return routeFieldNoEvent( self, dest )\n\
\n\
  def unroute( self, dest ):\n\
    return unrouteField( self, dest )\n\
\n\
#  def replaceRoute( self, dest, i ):\n\
#    return unrouteField( self, dest )\n\
\n\
#  def replaceRouteNoEvent( self, dest, i ):\n\
#    return unrouteField( self, dest, i )\n\
\n\
  def touch( self ):\n\
    return touchField( self )\n\
\n\
  def getRoutesIn( self ):\n\
    return getRoutesIn( self )\n\
\n\
  def getRoutesOut( self ):\n\
    return getRoutesOut( self )\n\
\n\
  def getAccessType( self ):\n\
    return getFieldAccessType( self )\n\
\n\
  def setValueFromString( self, value ):\n\
    return fieldSetValueFromString( self, value )\n\
\n\
  def getValueAsString( self ):\n\
    return fieldGetValueAsString( self )\n\
\n\
  def __cmp__( self, o ):\n\
    return getCPtr(self) - getCPtr(o)\n\
\n\
class SField( Field ):\n\
  type = UNKNOWN_X3D_TYPE\n\
  def setValue( self, value ):\n\
    setFieldValue( self, value )\n\
\n\
  def getValue( self ):\n\
    return getFieldValue( self )\n\
\n\
class MField( Field ):\n\
  type = UNKNOWN_X3D_TYPE\n\
  def setValue( self, value ):\n\
    setFieldValue( self, value )\n\
\n\
  def getValue( self ):\n\
    return getFieldValue( self )\n\
\n\
  def setValueFromString( self, value ):\n\
    setFieldValueFromString( self, value )\n\
\n\
  def getValueAsString( self ):\n\
    return getFieldValueAsString( self )\n\
\n\
  def push_back( self, v ):\n\
    pushBackElementInMField( self, v ) \n\
\n\
  def pop_back( self ):\n\
    MFieldPopBack( self )\n\
\n\
  def empty( self ):\n\
    return MFieldEmpty( self )\n\
\n\
  def front( self ):\n\
    return MFieldFront( self )\n\
\n\
  def back( self ):\n\
    return MFieldBack( self )\n\
\n"
"  def clear( self ):\n\
    MFieldClear( self )\n\
\n\
  def erase( self, v ):\n\
    eraseElementFromMField( self, v ) \n\
\n\
  def size( self ):\n\
    return MFieldSize( self ) \n\
\n\
\n\
# Install all built-in Field types:\n\
sfield_types = [ \n\
  ( SFFLOAT,    \"SFFloat\" ),\n\
  ( SFDOUBLE,   \"SFDouble\" ),\n\
  ( SFTIME,     \"SFTime\" ),\n\
  ( SFINT32,    \"SFInt32\" ),\n\
  ( SFVEC2F,    \"SFVec2f\" ),\n\
  ( SFVEC2D,    \"SFVec2d\" ),\n\
  ( SFVEC3F,    \"SFVec3f\" ),\n\
  ( SFVEC3D,    \"SFVec3d\" ),\n\
  ( SFVEC4F,    \"SFVec4f\" ),\n\
  ( SFVEC4D,    \"SFVec4d\" ),\n\
  ( SFBOOL,     \"SFBool\"  ),\n\
  ( SFSTRING,   \"SFString\" ),\n\
  ( SFCOLOR,    \"SFColor\" ),\n\
  ( SFCOLORRGBA,\"SFColorRGBA\" ),\n\
  ( SFROTATION, \"SFRotation\" ),\n\
  ( SFQUATERNION, \"SFQuaternion\" ),\n\
  ( SFMATRIX3F, \"SFMatrix3f\" ),\n\
  ( SFMATRIX4F, \"SFMatrix4f\" ),\n\
  ( SFMATRIX3D, \"SFMatrix3d\" ),\n\
  ( SFMATRIX4D, \"SFMatrix4d\" ),\n\
  ( SFNODE    , \"SFNode\"     ) ]\n\
\n\
mfield_types = [\n\
  ( MFFLOAT,    \"MFFloat\" ),\n\
  ( MFDOUBLE,   \"MFDouble\" ),\n\
  ( MFTIME,     \"MFTime\" ),\n\
  ( MFINT32,    \"MFInt32\" ),\n\
  ( MFVEC2F,    \"MFVec2f\" ),\n\
  ( MFVEC2D,    \"MFVec2d\" ),\n\
  ( MFVEC3F,    \"MFVec3f\" ),\n\
  ( MFVEC3D,    \"MFVec3d\" ),\n\
  ( MFVEC4F,    \"MFVec4f\" ),\n\
  ( MFVEC4D,    \"MFVec4d\" ),\n\
  ( MFBOOL,     \"MFBool\"  ),\n\
  ( MFSTRING,   \"MFString\" ),\n\
  ( MFCOLOR,    \"MFColor\" ),\n\
  ( MFCOLORRGBA,\"MFColorRGBA\" ),\n\
  ( MFROTATION, \"MFRotation\" ),\n\
  ( MFQUATERNION, \"MFQuaternion\" ),\n\
  ( MFMATRIX3F, \"MFMatrix3f\" ),\n\
  ( MFMATRIX4F, \"MFMatrix4f\" ),\n\
  ( MFMATRIX3D, \"MFMatrix3d\" ),\n\
  ( MFMATRIX4D, \"MFMatrix4d\" ),\n\
  ( MFNODE    , \"MFNode\"     )\n\
]\n\
\n\
for t in sfield_types:\n\
  exec \"\"\"\n\
class %s( SField ):\n\
  type = %s\n\
\"\"\" % (t[1], t[0] )\n\
\n\
for t in mfield_types:\n\
  exec \"\"\"\n\
class %s( MField ):\n\
  type = %s\n\
\"\"\" % (t[1], t[0] )\n\
\n\
\n\
typed_field_classes = {}\n\
\n\
def TypedField( base_class, type_info = None, opt_type_info = None ):\n\
  class TypedBase( base_class ):\n\
    pass\n\
  global typed_field_classes\n\
  if( typed_field_classes.has_key( (base_class, type_info, opt_type_info) ) ):\n\
    return typed_field_classes[(base_class, type_info, opt_type_info)]\n"
"\n\
  if type_info == None:\n\
    TypedBase.__type_info__ = ()\n\
  elif type( type_info ) != type(()):\n\
    TypedBase.__type_info__ = ( type_info, )\n\
  else:\n\
    TypedBase.__type_info__ = type_info\n\
\n\
  if opt_type_info == None:\n\
    TypedBase.__opt_type_info__ = ()\n\
  elif type( opt_type_info ) != type(()):\n\
    TypedBase.__opt_type_info__ = ( opt_type_info, )\n\
  else:\n\
    TypedBase.__opt_type_info__ = opt_type_info\n\
\n\
  typed_field_classes[(base_class, type_info, opt_type_info)] = TypedBase\n\
  return TypedBase\n\
\n\
\n\
auto_update_classes = {}\n\
\n\
\n\
# AutoUpdate \"template\" as in C++\n\
def AutoUpdate( base_class ):\n\
  class AutoUpdateBase( base_class ):\n\
    def __init__( self ):\n\
      base_class.__init__( self, 1 )\n\
\n\
  global auto_update_classes\n\
  if( auto_update_classes.has_key( base_class ) ):\n\
    return auto_update_classes[base_class]\n\
  else:\n\
    auto_update_classes[base_class] = AutoUpdateBase\n\
    return AutoUpdateBase\n\
\n\
periodic_update_classes = {}\n\
\n\
def PeriodicUpdate( base_class ):\n\
  class PeriodicUpdateBase( base_class ):\n\
    def __init__( self ):\n\
      base_class.__init__( self, 0 )\n\
      self.route( eventSink )\n\
  \n\
  global periodic_update_classes\n\
  if( periodic_update_classes.has_key( base_class ) ):\n\
    return periodic_update_classes[base_class]\n\
  else:\n\
    periodic_update_classes[base_class] = PeriodicUpdateBase\n\
    return PeriodicUpdateBase\n\
\n";

    const string H3DUtils_string ="from H3DInterface import *\n\
\n\
def PrintFieldValue( base_class ):\n\
  class PrintValueClass( AutoUpdate( base_class ) ):\n\
    def update( self, event ):\n\
      v = event.getValue()\n\
      print v\n\
      return v\n\
  return PrintValueClass()\n\
\n\
def FieldValue2String( base_class ):\n\
  class Value2StringClass( TypedField( SFString, base_class ) ):\n\
    def update( self, event ):\n\
      v = event.getValue()\n\
      return str( v )\n\
  return Value2StringClass()\n\
\n\
def FieldValue2StringList( base_class ):\n\
  class Value2StringListClass( TypedField( MFString, base_class ) ):\n\
    def update( self, event ):\n\
      v = event.getValue()\n\
      return [str( v )]\n\
  return Value2StringListClass()\n\
\n\
def FieldValue2Int( base_class ):\n\
  class Value2IntClass( TypedField( SFInt32, base_class ) ):\n\
    def update( self, event ):\n\
      v = event.getValue()\n\
      return int( v )\n\
  return Value2IntClass()\n\
\n\
def SField2MField( sfield, mfield ):\n\
  class SField2MFieldClass( TypedField( mfield, sfield ) ):\n\
    def update( self, event ):\n\
      v = event.getValue()\n\
      return [v]\n\
  return SField2MFieldClass()\n\
\n\
# The TimerCallback field is a field in which you can set callback functions\n\
# to be called at a later time that you specify.\n\
class TimerCallback( AutoUpdate( SFTime ) ):\n\
  def __init__( self ):\n\
    AutoUpdate( SFTime ).__init__( self )\n\
    self.callbacks = []\n\
    time.route( self )\n\
\n\
  def update( self, event ):\n\
    t = event.getValue()\n\
    cbs_to_remove = []\n\
    for cb in self.callbacks:\n\
      if t > cb[0]:\n\
        apply( cb[1], cb[2] )\n\
        cbs_to_remove.append( cb )\n\
    for cb in cbs_to_remove:\n\
      self.callbacks.remove( cb )\n\
\n\
    return event.getValue()\n\
\n\
  # add a callback function. The function will be called at the specified\n\
  # time with the given arguments.\n\
  def addCallback( self, time, func, args ):\n\
    self.callbacks.append( (time, func, args ) )\n\
\n\
  ## Remove a callback function before it has executed. \n\
  ## \\param cb The handle of the callback to remove. \n\
  def removeCallback( self, cb ): \n\
    try: \n\
      self.callbacks.remove( cb ) \n\
    except: \n\
      pass\n";

  }
}
