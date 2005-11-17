////////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file PythonMethods.cpp
/// \brief Definitions of the methods used in the H3D module of the 
/// PythonScript node.
///
//
//////////////////////////////////////////////////////////////////////////////

#include "PythonMethods.h"
#include "DeviceInfo.h"
#include "Viewpoint.h"
#include "NavigationInfo.h"
#include "StereoInfo.h"
#include "X3DBackgroundNode.h" 
#include "X3DTypeFunctions.h"
#include "PythonTypes.h"
#include "MFNode.h"
#include "X3D.h"
#include "Scene.h"
#include <sstream>
#include <cctype>

#ifdef HAVE_PYTHON

using namespace H3D;
using namespace PythonInternals;

namespace H3D {
  namespace PythonInternals {




    H3D_VALUE_EXCEPTION( string, UnableToCreatePythonField );

    PyObject *H3D_module = 0;
    PyObject *H3D_dict = 0;
    PyObject *H3DInterface_module = 0;
    PyObject *H3DInterface_dict = 0;

    // Macro used by pythonCreateField to create a new PythonField depending
    // on the field type.
#define CREATE_FIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
f = new PythonField< field_type >( field );

    // Macro used by pythonCreateField to create a new PythonField with 
    // AutoUpdate depending on the field type.
    #define CREATE_AUTOUPDATE_FIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
f = new PythonField< AutoUpdate< field_type > >( field );


    // Macro used by pythonGetFieldValue to get the value of a SField.
#define GET_SFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
return from_func( static_cast< field_type * >( field )->getValue() ); 

    // Macro used by pythonGetFieldValue to get the value of a MField.
#define GET_MFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
  field_type *mfield = static_cast< field_type* >( field ); \
  PyObject *list = PyList_New( (int)mfield->size() ); \
  for( size_t i = 0; i < mfield->size(); i++ ) { \
    PyObject *v = from_func( (*mfield)[i] ); \
    PyList_SetItem( list, i, v ); \
  } \
  return list;


    // Macro used by pythonMFieldEmpty.
#define MFIELD_EMPTY( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
  bool is_empty = static_cast< field_type * >( field )->empty(); \
  int v = is_empty ? 1 : 0; \
  return PyInt_FromLong( v );

  // Macro used by pythonMFieldFront.
#define MFIELD_FRONT( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
 if( static_cast< field_type * >( field )->empty() ) { \
    PyErr_SetString( PyExc_ValueError,  \
                     "Trying to call front() on empty MField" ); \
    return 0; \
  } \
  value_type front = static_cast< field_type * >( field )->front(); \
  return from_func( front );

 // Macro used by pythonMFieldBack.
#define MFIELD_BACK( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
  if( static_cast< field_type * >( field )->empty() ) { \
    PyErr_SetString( PyExc_ValueError,  \
                     "Trying to call back() on empty MField" ); \
    return 0; \
  } \
  value_type back = static_cast< field_type * >( field )->back(); \
  return from_func( back );

    // Macro used by pythonMFieldClear.
#define MFIELD_CLEAR( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
 static_cast< field_type *> \
    (field_ptr)->clear(); 

    // Macro used by pythonMFieldPopBack.
#define MFIELD_POPBACK( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
 static_cast< field_type *> \
    (field_ptr)->pop_back(); 

    // Macro used by pythonPushBackElementInMField to push_back a value
    // in a MField.
#define MFIELD_PUSH_BACK( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
 if( ! value || ! check_func( value ) ) { \
    PyErr_SetString( PyExc_ValueError,  \
                     "Invalid argument type to push_back() function" ); \
    return 0; \
 } \
 static_cast< field_type *> \
    (field_ptr)->push_back( (value_type)value_func( v ) ); 

    // Macro used by pythonEraseElementFromMField to push_back a value
    // in a MField.
#define MFIELD_ERASE( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
 if( ! value || ! check_func( value ) ) { \
    PyErr_SetString( PyExc_ValueError,  \
                     "Invalid argument type to erase() function"  ); \
    return 0; \
 } \
 static_cast< field_type *> \
    (field_ptr)->erase( (value_type)value_func( v ) ); 
 
    // Macro used by pythonSetFieldValue to set the value of a SField.
#define SET_SFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
if( check_func( value ) ) {                                         \
  static_cast<field_type*>(field)->setValue( (value_type) value_func( value ) ); \
} else {                                                            \
  PyErr_SetString( PyExc_ValueError,                                \
                   "Invalid argument type to setValue() function " );            \
  return 0;                                                         \
} 

    // Macro used by pythonSetFieldValue to set the value of a SField.
#define SET_MFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
  if( ! value || ! PyList_Check( value ) ) {                     \
    PyErr_SetString( PyExc_ValueError,                         \
  "Argument type must be a list of values in MField.setValue()" );      \
    return 0;                                                  \
  }                                                            \
  int n = PyList_GET_SIZE( value );                            \
  vector< value_type > fv;                                     \
  fv.resize( n );                                              \
  for ( int i=0; i < n; i++ ) {                                \
    if ( check_func( PyList_GET_ITEM( value, i ) ) ) {         \
              fv[i] = (value_type)value_func( PyList_GET_ITEM( value, i ) ); \
    } else { \
      PyErr_SetString( PyExc_ValueError, \
                       "Invalid argument type to setValue() function " ); \
      return 0;                                                \
    }                                                          \
  }                                                          \
  static_cast<field_type *>(field)->swap(fv);                  


    // This macro is used in order to apply some macro where the values
    // given to the macro depends on the X3DType of the field it is given.
    // 
    // field_ptr is a pointer to a field.
    // x3d_type is the X3DType of that field.
    // value is an optional argument that will be passed along to the macro
    // macro is the macro to apply. The macro must take arguments as in
    //
    // #define MACRO( check_func, value_func, from_func, value_type,
    //                field_type, field, value ) 
    // where check_func is a function to check if a PyObject is of the current
    // type. 
    // value_func is a function to get the value from a PyObject.
    // from_func is a function to crete a new PyObject from a value.
    // value_type is the type of the value encapsulated in the field.
    // field_type is the type of the field
    // field is a pointer to the field
    // value is an extra parameter that can be used to pass an extra argument.
    // 
    // The APPLY_SFIELD_MACRO will check what the type of the field is and
    // call the macro with the appropriate parameters.
    // success is a boolean variable that will be set to true on success
    // and false otherwise.
#define APPLY_SFIELD_MACRO( field_ptr, x3d_type, value, macro, success ) \
   { \
   success = true; \
      switch( x3d_type ) { \
      case X3DTypes::SFFLOAT: { \
        macro( PyFunctions::H3DPyDouble_Check, PyFunctions::H3DPyDouble_AsDouble, \
               PyFloat_FromDouble, H3DFloat, SFFloat, field_ptr, v ); break; \
      } \
      case X3DTypes::SFDOUBLE: { \
        macro( PyFunctions::H3DPyDouble_Check, PyFunctions::H3DPyDouble_AsDouble, \
               PyFloat_FromDouble, H3DDouble, SFDouble, field_ptr, v ); break; \
      } \
      case X3DTypes::SFTIME: { \
        macro( PyFunctions::H3DPyDouble_Check, PyFunctions::H3DPyDouble_AsDouble, \
               PyFloat_FromDouble, H3DTime, SFTime, field_ptr, v ); break; \
      } \
      case X3DTypes::SFINT32: { \
        macro( PyInt_Check, PyInt_AsLong, PyInt_FromLong,  \
               H3DInt32, SFInt32, field_ptr, v ); break; \
      } \
      case X3DTypes::SFVEC2F: { \
        macro( PyVec2f_Check, PyVec2f_AsVec2f, PyVec2f_FromVec2f, \
               Vec2f, SFVec2f, field_ptr, v ); break; \
      } \
      case X3DTypes::SFVEC2D: { \
        macro( PyVec2d_Check, PyVec2d_AsVec2d, PyVec2d_FromVec2d, \
               Vec2d, SFVec2d, field_ptr, v ); break; \
      } \
      case X3DTypes::SFVEC3F: { \
        macro( PyVec3f_Check, PyVec3f_AsVec3f, PyVec3f_FromVec3f, \
               Vec3f, SFVec3f, field_ptr, v ); break; \
      } \
      case X3DTypes::SFVEC3D: { \
        macro( PyVec3d_Check, PyVec3d_AsVec3d, PyVec3d_FromVec3d, \
               Vec3d, SFVec3d, field_ptr, v ); break; \
      } \
      case X3DTypes::SFVEC4F: { \
        macro( PyVec4f_Check, PyVec4f_AsVec4f, PyVec4f_FromVec4f, \
               Vec4f, SFVec4f, field_ptr, v ); break; \
      } \
      case X3DTypes::SFVEC4D: { \
        macro( PyVec4d_Check, PyVec4d_AsVec4d, PyVec4d_FromVec4d, \
               Vec4d, SFVec4d, field_ptr, v ); break; \
      } \
      case X3DTypes::SFBOOL: { \
        macro( PyInt_Check, PyInt_AsLong, PyInt_FromLong,  \
               bool, SFBool, field_ptr, v ); break; \
      } \
      case X3DTypes::SFSTRING: { \
        macro( PyString_Check, PyString_AsString, \
               PyFunctions::H3DPyString_FromString, \
               string, SFString, field_ptr, v ); break; \
      } \
      case X3DTypes::SFNODE: { \
        macro( PyNode_Check, PyNode_AsNode, PyNode_FromNode,  \
               Node *,SFNode, field_ptr, v ); break; \
      } \
      case X3DTypes::SFCOLOR: { \
        macro( PyRGB_Check, PyRGB_AsRGB, PyRGB_FromRGB,  \
               RGB, SFColor, field_ptr, v ); break; \
      } \
      case X3DTypes::SFCOLORRGBA: { \
        macro( PyRGBA_Check, PyRGBA_AsRGBA, PyRGBA_FromRGBA, \
               RGBA, SFColorRGBA, field_ptr, v ); break; \
      } \
      case X3DTypes::SFROTATION: { \
        macro( PyRotation_Check, PyRotation_AsRotation, PyRotation_FromRotation, \
               Rotation, SFRotation, field_ptr, v ); break; \
      } \
      case X3DTypes::SFQUATERNION: { \
        macro( PyQuaternion_Check, PyQuaternion_AsQuaternion, PyQuaternion_FromQuaternion, \
               Quaternion, SFQuaternion, field_ptr, v ); break; \
      } \
      case X3DTypes::SFMATRIX3F: { \
        macro( PyMatrix3f_Check, PyMatrix3f_AsMatrix3f, PyMatrix3f_FromMatrix3f, \
               Matrix3f, SFMatrix3f, field_ptr, v ); break; \
      } \
      case X3DTypes::SFMATRIX4F: { \
        macro( PyMatrix4f_Check, PyMatrix4f_AsMatrix4f, PyMatrix4f_FromMatrix4f, \
               Matrix4f, SFMatrix4f, field_ptr, v ); break; \
      } \
      case X3DTypes::SFMATRIX3D: { \
        macro( PyMatrix3d_Check, PyMatrix3d_AsMatrix3d, PyMatrix3d_FromMatrix3d, \
               Matrix3d, SFMatrix3d, field_ptr, v ); break; \
      } \
      case X3DTypes::SFMATRIX4D: { \
        macro( PyMatrix4d_Check, PyMatrix4d_AsMatrix4d, PyMatrix4d_FromMatrix4d, \
               Matrix4d, SFMatrix4d, field_ptr, v ); break; \
      } \
      default: success = false; \
      } \
  }

    // This macro is used in order to apply some macro where the values
    // given to the macro depends on the X3DType of the field it is given.
    // 
    // field_ptr is a pointer to a field.
    // x3d_type is the X3DType of that field.
    // value is an optional argument that will be passed along to the macro
    // macro is the macro to apply. The macro must take arguments as in
    //
    // #define MACRO( check_func, value_func, from_func, value_type,
    //                field_type, field, value ) 
    // where check_func is a function to check if a PyObject is of the current
    // type. 
    // value_func is a function to get the value from a PyObject.
    // from_func is a function to crete a new PyObject from a value.
    // value_type is the type of the value encapsulated in the field.
    // field_type is the type of the field
    // field is a pointer to the field
    // value is an extra parameter that can be used to pass an extra argument.
    // 
    // The APPLY_SFIELD_MACRO will check what the type of the field is and
    // call the macro with the appropriate parameters.
    // success is a boolean variable that will be set to true on success
    // and false otherwise.
#define APPLY_MFIELD_MACRO( field_ptr, x3d_type, value, macro, success  ) \
  {\
    success = true; \
      switch( x3d_type ) { \
      case X3DTypes::MFFLOAT: { \
        macro( PyFunctions::H3DPyDouble_Check, PyFunctions::H3DPyDouble_AsDouble, \
               PyFloat_FromDouble, H3DFloat, MFFloat, field_ptr, v ); break; \
      } \
      case X3DTypes::MFDOUBLE: { \
        macro( PyFunctions::H3DPyDouble_Check, PyFunctions::H3DPyDouble_AsDouble, \
               PyFloat_FromDouble, H3DDouble, MFDouble, field_ptr, v ); break; \
      } \
      case X3DTypes::MFTIME: { \
        macro( PyFunctions::H3DPyDouble_Check, PyFunctions::H3DPyDouble_AsDouble, \
               PyFloat_FromDouble, H3DTime, MFTime, field_ptr, v ); break; \
      } \
      case X3DTypes::MFINT32: { \
        macro( PyInt_Check, PyInt_AsLong,  PyInt_FromLong, \
               H3DInt32, MFInt32, field_ptr, v ); break; \
      } \
      case X3DTypes::MFVEC2F: { \
        macro( PyVec2f_Check, PyVec2f_AsVec2f, PyVec2f_FromVec2f, \
               Vec2f, MFVec2f, field_ptr, v ); break; \
      } \
      case X3DTypes::MFVEC2D: { \
        macro( PyVec2d_Check, PyVec2d_AsVec2d, PyVec2d_FromVec2d, \
               Vec2d, MFVec2d, field_ptr, v ); break; \
      } \
      case X3DTypes::MFVEC3F: { \
        macro( PyVec3f_Check, PyVec3f_AsVec3f, PyVec3f_FromVec3f, \
               Vec3f, MFVec3f, field_ptr, v ); break; \
      } \
      case X3DTypes::MFVEC3D: { \
        macro( PyVec3d_Check, PyVec3d_AsVec3d, PyVec3d_FromVec3d, \
               Vec3d, MFVec3d, field_ptr, v ); break; \
      } \
      case X3DTypes::MFVEC4F: { \
        macro( PyVec4f_Check, PyVec4f_AsVec4f, PyVec4f_FromVec4f, \
               Vec4f, MFVec4f, field_ptr, v ); break; \
      } \
      case X3DTypes::MFVEC4D: { \
        macro( PyVec4d_Check, PyVec4d_AsVec4d, PyVec4d_FromVec4d, \
               Vec4d, MFVec4d, field_ptr, v ); break; \
      } \
      case X3DTypes::MFBOOL: { \
        macro( PyInt_Check, PyInt_AsLong, PyInt_FromLong, bool,  \
               MFBool, field_ptr, v ); break; \
      } \
      case X3DTypes::MFSTRING: { \
        macro( PyString_Check, PyString_AsString, \
               PyFunctions::H3DPyString_FromString, \
               string, MFString, field_ptr, v ); break; \
      } \
      case X3DTypes::MFNODE: { \
        macro( PyNode_Check, PyNode_AsNode, PyNode_FromNode,  \
                    Node *, MFNode, field_ptr, v ); break; \
      } \
      case X3DTypes::MFCOLOR: { \
        macro( PyRGB_Check, PyRGB_AsRGB, PyRGB_FromRGB, \
               RGB, MFColor, field_ptr, v ); break; \
      } \
      case X3DTypes::MFCOLORRGBA: { \
        macro( PyRGBA_Check, PyRGBA_AsRGBA, PyRGBA_FromRGBA, \
               RGBA, MFColorRGBA, field_ptr, v ); break; \
      } \
      case X3DTypes::MFROTATION: { \
        macro( PyRotation_Check, PyRotation_AsRotation, PyRotation_FromRotation, \
               Rotation, MFRotation, field_ptr, v ); break; \
      } \
      case X3DTypes::MFQUATERNION: { \
        macro( PyQuaternion_Check, PyQuaternion_AsQuaternion, PyQuaternion_FromQuaternion, \
               Quaternion, MFQuaternion, field_ptr, v ); break; \
      } \
      case X3DTypes::MFMATRIX3F: { \
        macro( PyMatrix3f_Check, PyMatrix3f_AsMatrix3f, PyMatrix3f_FromMatrix3f, \
               Matrix3f, MFMatrix3f, field_ptr, v ); break; \
      } \
      case X3DTypes::MFMATRIX4F: { \
        macro( PyMatrix4f_Check, PyMatrix4f_AsMatrix4f, PyMatrix4f_FromMatrix4f, \
               Matrix4f, MFMatrix4f, field_ptr, v ); break; \
      } \
      case X3DTypes::MFMATRIX3D: { \
        macro( PyMatrix3d_Check, PyMatrix3d_AsMatrix3d, PyMatrix3d_FromMatrix3d, \
               Matrix3d, MFMatrix3d, field_ptr, v ); break; \
      } \
      case X3DTypes::MFMATRIX4D: { \
        macro( PyMatrix4d_Check, PyMatrix4d_AsMatrix4d, PyMatrix4d_FromMatrix4d, \
               Matrix4d, MFMatrix4d, field_ptr, v ); break; \
      } \
      default: success = false; \
    } \
  }


    
    // Utility functions

    // Insert all the X3DTypes in the given Python dictionary.
    void insertFieldTypes( PyObject *dict ) {
      for( int i = 0; i <= X3DTypes::UNKNOWN_X3D_TYPE; i++ ) {
        // insert each type from the enumerated list into Python
        //cerr << X3DTypes::typeToString( (X3DTypes::X3DType)i ) <<  endl;
        string str = X3DTypes::typeToString( (X3DTypes::X3DType)i );
        for( string::iterator x = str.begin(); x != str.end(); x++ )
          *x = std::toupper( *x );
        PyDict_SetItem( dict, 
                        PyString_FromString( str.c_str() ), 
                        PyInt_FromLong( i ) );
      }
      
    }

    // returns a PyTuple containing python versions of the
    // given node and DEFNodes.
    PyObject *createX3DHelp( Node *n, X3D::DEFNodes *dn ) {
      // make a PyNode from the created node.
      PyObject* python_node = PyNode_FromNode( n );

      // make a python dict out of the DEFNodes.
      PyObject *py_def_map = PyDict_New();
      for( X3D::DEFNodes::const_iterator i = dn->begin(); 
           i != dn->end(); i++ ) {
        PyObject *node = PyNode_FromNode( (*i).second );
        PyDict_SetItemString( py_def_map, 
                              (*i).first.c_str(), 
                              node);
        Py_DECREF( node );
      }
      // create the tuple to return
      PyObject *res = Py_BuildValue( "(OO)", python_node, py_def_map );
      Py_DECREF( py_def_map );
      Py_DECREF( python_node );
      return res;
    }
    
    static PyMethodDef H3DMethods[] = {
      { "createField", pythonCreateField, 0 },
      { "setFieldValue", pythonSetFieldValue, 0 },
      { "getFieldValue", pythonGetFieldValue, 0 },
      { "routeField", pythonRouteField, 0 },
      { "routeFieldNoEvent", pythonRouteFieldNoEvent, 0 },
      { "unrouteField", pythonUnrouteField, 0 },
      { "createX3DFromURL", pythonCreateX3DFromURL, 0 },
      { "createX3DFromString", pythonCreateX3DFromString, 0 },
      { "createX3DNodeFromURL", pythonCreateX3DNodeFromURL, 0 },
      { "createX3DNodeFromString", pythonCreateX3DNodeFromString, 0 },
      { "getRoutesIn", pythonGetRoutesIn, 0 },
      { "getRoutesOut", pythonGetRoutesOut, 0 },
      { "getCurrentScenes", pythonGetCurrentScenes, 0 },
      { "getActiveDeviceInfo", pythonGetActiveDeviceInfo, 0 },
      { "getActiveViewpoint", pythonGetActiveViewpoint, 0 },
      { "getActiveNavigationInfo", pythonGetActiveNavigationInfo, 0 },
      { "getActiveStereoInfo", pythonGetActiveStereoInfo, 0 },
      { "getActiveBackground", pythonGetActiveBackground, 0 },
      { "eraseElementFromMField", pythonEraseElementFromMField, 0 },
      { "pushBackElementInMField", pythonPushBackElementInMField, 0 },
      { "pushBackElementInMField", pythonPushBackElementInMField, 0 },
      { "MFieldClear", pythonMFieldClear, 0 },
      { "MFieldBack", pythonMFieldBack, 0 },
      { "MFieldFront", pythonMFieldFront, 0 },
      { "MFieldEmpty", pythonMFieldEmpty, 0 },
      { "MFieldPopBack", pythonMFieldPopBack, 0 },
      { "touchField", pythonTouchField, 0 },
      { NULL, NULL }      
    };
    
    PyMODINIT_FUNC initH3D() {
      H3D_module = Py_InitModule( "H3D", H3DMethods );
      H3D_dict = PyModule_GetDict( H3D_module );

      PyNode::installType( H3D_module );     
      PyVec2f::installType( H3D_module );     
      PyVec2d::installType( H3D_module );     
      PyVec3f::installType( H3D_module );     
      PyVec3d::installType( H3D_module );     
      PyVec4f::installType( H3D_module );     
      PyVec4d::installType( H3D_module );     
      PyRGB::installType( H3D_module );     
      PyRGBA::installType( H3D_module );     
      PyRotation::installType( H3D_module );      
      PyQuaternion::installType( H3D_module );
      PyMatrix3f::installType( H3D_module );     
      PyMatrix4f::installType( H3D_module );
      PyMatrix3d::installType( H3D_module );     
      PyMatrix4d::installType( H3D_module );
      PythonInternals::insertFieldTypes( H3D_dict );

      PythonInternals::H3DInterface_module = 
        PyImport_ImportModule( "H3DInterface" );
      if ( PythonInternals::H3DInterface_module == NULL ) {
        PyErr_Print();
        cerr << "PythonScript::initialiseParser() ERROR importing H3DInterface, check your PYTHONPATH" << endl;
        exit(-1); // SHOULD THROW AN ERROR!
      }

      PythonInternals::H3DInterface_dict = 
        PyModule_GetDict( PythonInternals::H3DInterface_module );

      if ( PythonInternals::H3DInterface_dict == NULL )
        PyErr_Print();
      PyObject *time = (PyObject*)fieldAsPythonObject( Scene::time );
      PyDict_SetItem( PythonInternals::H3DInterface_dict, 
                      PyString_FromString( "time" ), 
                      time );
      PyObject *event_sink = (PyObject*)fieldAsPythonObject( Scene::eventSink );
      PyDict_SetItem( PythonInternals::H3DInterface_dict, 
                      PyString_FromString( "eventSink" ), 
                      event_sink );
      
    };
  


    void *fieldAsPythonObject( Field * f ) {
      PyObject *obj = PyDict_GetItemString( PythonInternals::H3DInterface_dict, 
                                            f->getTypeName().c_str() );
      if( ! obj || ! PyClass_Check( obj ) ) {
        PyErr_Print();
        throw UnableToCreatePythonField( "fieldAsPythonObject()", 
                                         f->classTypeName().c_str() );
      }
      // create the field instance with the owner flag set to zero.
      PyObject *py_field = PyInstance_New( obj, NULL, NULL );
      if ( !py_field ) {
        PyErr_Print();
        throw UnableToCreatePythonField( "fieldAsPythonObject()", "" );
      }
      PyObject *field_ptr = PyCObject_FromVoidPtr( f, NULL );
      PyObject_SetAttrString( py_field, "__fieldptr__", field_ptr );
      Py_DECREF( field_ptr);
      return (void*)py_field;      
    }

    // Methods for the H3D Module:
    
    PyObject *pythonCreateField( PyObject *self, PyObject *args ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createField( Field, int auto_update, name )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      PyObject *field = PyTuple_GetItem( args, 0 );
      PyObject *autoupdateobj = PyTuple_GetItem( args, 1 );
      PyObject *name = PyTuple_GetItem( args, 2 );

      bool autoupdate = PyInt_AsLong( autoupdateobj ) != 0;
      
      if( field && PyInstance_Check( field ) ) {
        //PyObject_SetAttrString( field, "__fieldtype__", field );

        PyObject *fieldtype = PyObject_GetAttrString( field, "type" );
        int field_type = PyInt_AsLong( fieldtype );
        Field *f;

        if ( autoupdate ) {
          bool success;
          APPLY_SFIELD_MACRO( field, field_type, 0, CREATE_AUTOUPDATE_FIELD, success );
          if( !success )
            APPLY_MFIELD_MACRO( field, field_type, 0, 
                                CREATE_AUTOUPDATE_FIELD, success );
          if( !success ) {
            f = 0; /*
            PyErr_SetString( PyExc_ValueError, 
                             "Error: not a valid Field instance" );
                             return 0;*/  
          }
        } else {
          bool success;
          APPLY_SFIELD_MACRO( field, field_type, 0, CREATE_FIELD, success );
          if( !success )
            APPLY_MFIELD_MACRO( field, field_type, 0, 
                                CREATE_FIELD, success );
          if( !success ) {
            f = 0 ; /*PyErr_SetString( PyExc_ValueError, 
                             "Error: not a valid Field instance" );
                             return 0; */ 
          }
        }

        if( f ) f->setName( PyString_AsString( name ) );
        
        PyObject *pfield = PyCObject_FromVoidPtr( f, 0 );
        PyObject_SetAttrString( field, "__fieldptr__", pfield );
        // field now holds a reference to pfield so we can remove the extra reference
        // from the all to PyCObject_FromVoidPtr()
        Py_DECREF( pfield );
        PyObject *update = PyObject_GetAttrString( field, "update" );
        if( update ) {
          if( PyMethod_Check( update ) ) {
            dynamic_cast< PythonFieldBase* >(f)->python_update = update;
          } else {
            PyErr_SetString( PyExc_ValueError, 
                             "Symbol 'update' must be a method!" );
            return 0;
          }
        } else PyErr_Clear();
  
        PyObject *typeinfo = PyObject_GetAttrString( field, "__type_info__" );
        if( typeinfo ) {
          if( PyTuple_Check( typeinfo ) ) {
            dynamic_cast< PythonFieldBase* >(f)->python_typeinfo = typeinfo;
          } else {
            PyErr_SetString( PyExc_ValueError, 
                             "Symbol '__type_info__' must be a tuple!" );
            return NULL;
          }
        } else PyErr_Clear();
        
        //initField( (PyInstanceObject*) field );
        Py_INCREF( Py_None );
        return Py_None;
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "H3D.createField() failed due to invalid input!" );
        return NULL;
        // THROW APPROPRIATE ERROR
      }
      Py_INCREF(Py_None);
      return Py_None;
    }

    namespace PyFunctions {
      int H3DPyDouble_Check( PyObject *v ) {
        return PyFloat_Check( v ) || PyInt_Check( v ) || PyLong_Check( v );
      }

      double H3DPyDouble_AsDouble( PyObject *v ) {
        if( PyFloat_Check( v ) )
          return PyFloat_AsDouble( v );
        else if( PyInt_Check( v ) ) 
          return (double)PyInt_AsLong( v );
         else if( PyLong_Check( v ) ) 
          return PyLong_AsDouble( v );
        return 0.0;
      }

      PyObject *H3DPyString_FromString( const string &s ) {
        return PyString_FromString( s.c_str() );
      }

    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonSetFieldValueFromObject( Field *field_ptr, PyObject *v ) {
      if( field_ptr ) { 
        bool success;
        APPLY_SFIELD_MACRO( field_ptr, field_ptr->getX3DType(), v, SET_SFIELD, success );
        if( !success )
          APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), v, SET_MFIELD, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
      }
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonSetFieldValue( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.setFieldValue( self, value )" );
        return 0;
      }
  
      PyObject *field = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid Field type given as argument to H3D.setFieldValue( self, value )" );
        return 0;
      }
      PyObject *py_field_ptr = PyObject_GetAttrString( field, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );

      PyObject *v = PyTuple_GetItem( args, 1 );
      //const char *value = PyString_AsString( PyObject_Repr( v ) );
      //ParsableField *pf = static_cast< ParsableField* >(field_ptr);
      return PythonInternals::pythonSetFieldValueFromObject( field_ptr, v );
    }
    
    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonGetFieldValue( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.getFieldValue( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      //cerr << "pythonGetFieldValue() ptr=" << py_field_ptr << endl;
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      
      if( field_ptr ) { 
        bool success;
        APPLY_SFIELD_MACRO( field_ptr, field_ptr->getX3DType(), v, GET_SFIELD, success );
        if( !success )
          APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), v, GET_MFIELD, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    // help function for pythonRouteField and pythonRouteFieldNoEvent.
    PyObject *pythonUnrouteField( PyObject *self, 
                                  PyObject *args  ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.unrouteField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *from_field = PyTuple_GetItem( args, 0 );
      PyObject *to_field = PyTuple_GetItem( args, 1 );
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.unrouteField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( to_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.unrouteField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      PyObject *py_from_field_ptr = PyObject_GetAttrString( from_field, 
                                                            "__fieldptr__" );
      PyObject *py_to_field_ptr = PyObject_GetAttrString( to_field, 
                                                          "__fieldptr__" );
      if( !py_from_field_ptr || !py_to_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *from_field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_from_field_ptr ) );
      Field *to_field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_to_field_ptr ) );
      
      if( from_field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.unrouteField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( to_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.unrouteField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      try {
        from_field_ptr->unroute( to_field_ptr );
      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e << ends;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }
      
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

    // help function for pythonRouteField and pythonRouteFieldNoEvent.
    PyObject *pythonRouteFieldHelp( PyObject *self, 
                                    PyObject *args, 
                                    bool send_event  ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.routeField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *from_field = PyTuple_GetItem( args, 0 );
      PyObject *to_field = PyTuple_GetItem( args, 1 );
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.routeField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( to_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.routeField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      PyObject *py_from_field_ptr = PyObject_GetAttrString( from_field, 
                                                            "__fieldptr__" );
      PyObject *py_to_field_ptr = PyObject_GetAttrString( to_field, 
                                                          "__fieldptr__" );
      if( !py_from_field_ptr || !py_to_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *from_field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_from_field_ptr ) );
      Field *to_field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_to_field_ptr ) );
      
      if( from_field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.routeField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( to_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.routeField( fromField, toField )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      try {
        if( send_event )
          from_field_ptr->route( to_field_ptr );
        else
          from_field_ptr->routeNoEvent( to_field_ptr );
      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e << ends;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }
      
      Py_INCREF(Py_None);
      return Py_None; 
    }
    
    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateX3DFromURL( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromURL( filename )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *filename = PyString_AsString( arg );
      X3D::DEFNodes dm;
      Node *n = X3D::createX3DFromURL( filename, &dm );
      return PythonInternals::createX3DHelp( n, &dm );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateX3DFromString( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromString( s )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *s = PyString_AsString( arg );
      X3D::DEFNodes dm;
      Node *n = X3D::createX3DFromString( s, &dm );
      return PythonInternals::createX3DHelp( n, &dm );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateX3DNodeFromURL( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromURL( filename )" << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *filename = PyString_AsString( arg );
      X3D::DEFNodes dm;
      AutoRef< Node > n = X3D::createX3DNodeFromURL( filename, &dm );
      return PythonInternals::createX3DHelp( n.get(), &dm );
    }

    PyObject* pythonCreateX3DNodeFromString( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromString( s )"
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *s = PyString_AsString( arg );
      X3D::DEFNodes dm;
      AutoRef< Node > n = X3D::createX3DNodeFromString( s, &dm );
      return PythonInternals::createX3DHelp( n.get(), &dm );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetRoutesIn( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyInstance_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getRoutesIn( Field f )."
            << " Requires one argument of type Field. "
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
             "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      //cerr << "pythonGetFieldValue() ptr=" << py_field_ptr << endl;
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      Field::FieldVector routes_in =  field_ptr->getRoutesIn();
      PyObject *retval = PyTuple_New( routes_in.size() );

      for( unsigned int i = 0; i < routes_in.size() ; i++ ) {
        PyTuple_SetItem( retval, i, 
           (PyObject *)PythonInternals::fieldAsPythonObject( routes_in[i] ) );
      }
      return retval;
    }

    /////////////////////////////////////////////////////////////////////////

    // function for returning a tuple of the Fields that the field given
    // as arg is routed to.
    PyObject* pythonGetRoutesOut( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyInstance_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getRoutesOut( Field f )."
            << " Requires one argument of type Field. "
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
             "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }

      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      Field::FieldSet routes_out =  field_ptr->getRoutesOut();
      PyObject *retval = PyTuple_New( routes_out.size() );

      Field::FieldSet::iterator fi = routes_out.begin();
      unsigned int i = 0;
      for( ; fi != routes_out.end(); i++,fi++ ) {
        PyTuple_SetItem( retval, i, 
            (PyObject *) PythonInternals::fieldAsPythonObject( *fi ) );
      }
      return retval;
    }

    PyObject *pythonRouteField( PyObject *self, 
                                PyObject *args ) {
      return pythonRouteFieldHelp( self, args, true );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonRouteFieldNoEvent( PyObject *self, 
                                       PyObject *args ) {
      return pythonRouteFieldHelp( self, args, false );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetCurrentScenes( PyObject *self, PyObject *arg ) {
      if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getCurrentScenes()."
            << "Function does not take any arguments."
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      PyObject *scenes = PyTuple_New( Scene::scenes.size() );
      unsigned int counter = 0;
      for( set< Scene * >::const_iterator i = Scene::scenes.begin(); 
           i != Scene::scenes.end(); i++, counter++ ) {
        PyObject *node = PyNode_FromNode( *i );
        PyTuple_SetItem( scenes, 
                         counter, 
                         node );
      }      
      return scenes;
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveDeviceInfo( PyObject *self, PyObject *arg ) {
      if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveDeviceInfo()."
            << "Function does not take any arguments."
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( DeviceInfo::getActive() );
    }
    
    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveViewpoint( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveViewpoint()."
            << "Function does not take any arguments."
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( Viewpoint::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveNavigationInfo( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveNavigationInfo()."
            << "Function does not take any arguments."
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( NavigationInfo::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveStereoInfo( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveStereoInfo()."
            << "Function does not take any arguments."
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( StereoInfo::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveBackground( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveBackground()."
            << "Function does not take any arguments."
            << ends;
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( X3DBackgroundNode::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonEraseElementFromMField( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
       "Invalid argument(s) to function H3D.eraseElementFromMField( self, value )" );  
        return 0;
      }

      PyObject *field = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.eraseElementFromMField( self, value )" );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( field, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );


      if( field_ptr ) {
        PyObject *v = PyTuple_GetItem( args, 1 );
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 
                            v, MFIELD_ERASE, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid MField instance" );
          return 0;  
        }
      }
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

   PyObject* pythonPushBackElementInMField( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
    "Invalid argument(s) to function H3D.pushBackElementInMField( self, value )" );  
        return 0;
      }

      PyObject *field = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.pushBackElementInMField( self, value )" );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( field, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );


      if( field_ptr ) {
        PyObject *v = PyTuple_GetItem( args, 1 );
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 
                            v, MFIELD_PUSH_BACK, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid MField instance" );
          return 0;  
        }
      }
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonTouchField( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.touchField( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      if( field_ptr ) field_ptr->touch();

      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonMFieldClear( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.MFieldClear( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      
      if( field_ptr ) { 
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 0, 
                            MFIELD_CLEAR, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
        Py_INCREF(Py_None);
        return Py_None; 
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }
    
    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonMFieldPopBack( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.MFieldPopBack( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      
      if( field_ptr ) { 
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 0, 
                            MFIELD_POPBACK, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
        Py_INCREF(Py_None);
        return Py_None; 
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonMFieldEmpty( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.MFieldEmpty( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      
      if( field_ptr ) { 
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 0, 
                            MFIELD_EMPTY, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonMFieldFront( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.MFieldFront( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      
      if( field_ptr ) { 
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 0, 
                            MFIELD_FRONT, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonMFieldBack( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.MFieldBack( self )" );
        return 0;
      }
      
      PyObject *py_field_ptr = PyObject_GetAttrString( arg, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      
      if( field_ptr ) { 
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 0, 
                            MFIELD_BACK, success );
        if( !success ) {
          PyErr_SetString( PyExc_ValueError, 
                           "Error: not a valid Field instance" );
          return 0;  
        }
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }


  }

 
}

#endif // HAVE_PYTHON
