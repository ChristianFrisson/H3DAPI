////////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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

#include <H3D/PythonMethods.h>
#include <H3D/DeviceInfo.h>
#include <H3D/X3DViewpointNode.h>
#include <H3D/NavigationInfo.h>
#include <H3D/StereoInfo.h>
#include <H3D/Fog.h>
#include <H3D/GlobalSettings.h>
#include <H3D/X3DSAX2Handlers.h>
#include <H3D/X3DBackgroundNode.h> 
#include <H3D/X3DTypeFunctions.h>
#include <H3D/PythonTypes.h>
#include <H3D/MFNode.h>
#include <H3D/X3D.h>
#include <H3D/VrmlParser.h>
#include <H3D/Scene.h>
#include <H3D/ResourceResolver.h>
#include <H3D/Field.h>
#include "H3DInterface.py.h"

#include <sstream>
#include <cctype>

#ifdef HAVE_PYTHON
// DEV WARNING, never use PyBool_Check without also using PyInt_Check, the reason
// is that pythonfieldGetValue returns an integer for the SFBool type. Until that is
// changed this PyInt_Check also have to be used.

using namespace H3D;
using namespace PythonInternals;

namespace H3D {
  namespace PythonInternals {

    class PythonAutoRef{
    private:
      PyObject* ap;    // refers to the actual owned object (if any)
    public:
      typedef PyObject element_type;

      // constructor
      explicit PythonAutoRef (PyObject* ptr = 0, bool borrowed_ref = false ) {
        ap=ptr;
        // If the object is a borrowed reference then use Py_INCREF to make
        // sure that the PythonAutoRef actually contains a reference to the
        // python object and that Py_DECREF can be called no matter what when
        // the ap pointer should change.
        if( ap && borrowed_ref )
          Py_INCREF( ap );
      }
      
      // destructor
      ~PythonAutoRef() throw(){
        if( ap ) {
          // ensure we have the GIL lock to work with multiple python threads.
          PyGILState_STATE state = PyGILState_Ensure();
          Py_DECREF( ap );
          PyGILState_Release(state);
        }
      }

      // value access
      PyObject* get() const throw(){
        return ap;
      }
      PyObject& operator*() const throw(){
        return *ap;
      }
      PyObject* operator->() const throw(){
        return ap;
      }

      // reset value
      void reset( PyObject* ptr=0, bool borrowed_ref = false ) throw() {
        if( ap != ptr ){
          if( ap ) {
            // ensure we have the GIL lock to work with multiple python threads.
            PyGILState_STATE state = PyGILState_Ensure();
            Py_DECREF( ap );
            PyGILState_Release(state);
          }
          ap = ptr;
          // If the object is a borrowed reference then use Py_INCREF to make
          // sure that the PythonAutoRef actually contains a reference to the
          // python object and that Py_DECREF can be called no matter what when
          // the ap pointer should change.
          if( ap && borrowed_ref )
            Py_INCREF( ap );
        }
      }
    };



    H3D_VALUE_EXCEPTION( string, UnableToCreatePythonField );

    PythonAutoRef H3D_module( NULL );
    PythonAutoRef H3DInterface_module( NULL );

    // borrowed references so do not need autoref
    PyObject * H3DInterface_dict = NULL;
    PyObject * H3D_dict = NULL;

#ifndef H3DINTERFACE_AS_FILE
    // H3DUtils when not loading the module as a file.
    PythonAutoRef H3DUtils_module( NULL );
    // borrowed references so do not need autoref
    PyObject * H3DUtils_dict = NULL;
#endif

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


    // Macro used by pythonFieldGetValue to get the value of a SField.
#define GET_SFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
    value_type va; \
    PyThreadState *_save; \
    try { \
      Py_UNBLOCK_THREADS \
      va = static_cast< field_type * >( field )->getValue();    \
      Py_BLOCK_THREADS                                          \
    }                                                           \
    catch ( H3D::Exception::H3DException &e ) {                 \
      Py_BLOCK_THREADS                                          \
      ostringstream errstr;                                     \
      errstr << e;                                                   \
      PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );     \
      return 0;                                                      \
    } catch( ... ) {                                                 \
      Py_BLOCK_THREADS                                               \
      throw;                                                         \
    }                                                                \
    return from_func( va );

    // Macro used by pythonFieldGetValue to get the value of a MField.
#define GET_MFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
  field_type *mfield = static_cast< field_type* >( field ); \
  PyObject *list = PyList_New( (int)mfield->size() ); \
  for( size_t i = 0; i < mfield->size(); ++i ) { \
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

    // Macro used by pythonMFieldPushBack to push_back a value
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

    // Macro used by pythonMFieldErase to push_back a value
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
 
 // Macro used by pythonMFieldSize
#define MFIELD_SIZE( check_func, value_func, from_func, \
                     value_type, field_type, \
                     field, value ) \
  return PyInt_FromLong ( static_cast< field_type * >( field )->size() );

    // Macro used by pythonFieldSetValue to set the value of a SField.
#define SET_SFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
if( check_func( value ) ) { \
  PyThreadState *_save; \
  try { \
    Py_UNBLOCK_THREADS \
    static_cast<field_type*>(field)->setValue( (value_type) value_func( value ) ); \
    Py_BLOCK_THREADS \
  } \
  catch ( H3D::Exception::H3DException &e ) { \
    Py_BLOCK_THREADS \
    ostringstream errstr; \
    errstr << e; \
    PyErr_SetString( PyExc_ValueError, errstr.str().c_str() ); \
    return 0; \
  } catch( ... ) { \
    Py_BLOCK_THREADS \
    throw; \
  } \
} else {                                                            \
  PyErr_SetString( PyExc_ValueError,                                \
                   "Invalid argument type to setValue() function " );            \
  return 0;                                                         \
} 

    // Macro used by pythonFieldSetValue to set the value of a SField.
#define SET_MFIELD( check_func, value_func, from_func, \
                       value_type, field_type, \
                       field, value ) \
  if( ! value || ! PyList_Check( value ) ) {                     \
    PyErr_SetString( PyExc_ValueError,                         \
  "Argument type must be a list of values in MField.setValue()" );      \
    return 0;                                                  \
  }                                                            \
  Py_ssize_t n = PyList_GET_SIZE( value );                     \
  vector< value_type > fv;                                     \
  fv.resize( n );                                              \
  for ( int i=0; i < n; ++i ) {                                \
    if ( check_func( PyList_GET_ITEM( value, i ) ) ) {         \
              fv[i] = (value_type)value_func( PyList_GET_ITEM( value, i ) ); \
    } else { \
      PyErr_SetString( PyExc_ValueError, \
                       "Invalid argument type to setValue() function " ); \
      return 0;                                                \
    }                                                          \
  }                                                          \
  static_cast<field_type *>(field)->setValue(fv);                  

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
      for( int i = 0; i <= X3DTypes::UNKNOWN_X3D_TYPE; ++i ) {
        // insert each type from the enumerated list into Python
        string str = X3DTypes::typeToString( (X3DTypes::X3DType)i );
        for( string::iterator x = str.begin(); x != str.end(); ++x )
          *x = std::toupper( *x );
        PyDict_SetItem( dict, 
                        PyString_FromString( str.c_str() ), 
                        PyInt_FromLong( i ) );
      }
      
    }
    
    // Convert a Field::AccessType constant to a string name
    std::string accessTypeToString ( Field::AccessType accessType ) {
      switch ( accessType ) {
        case Field::INITIALIZE_ONLY:
          return "INITIALIZE_ONLY";
        case Field::OUTPUT_ONLY:
          return "OUTPUT_ONLY";
        case Field::INPUT_ONLY:
          return "INPUT_ONLY";
        case Field::INPUT_OUTPUT:
          return "INPUT_OUTPUT";
      }
      return "";
    }
    
    // Insert all the Field::AccessType constants in the given Python dictionary.
    void insertFieldAccessTypes( PyObject *dict ) {
      for( int i = 0; i <= Field::INPUT_OUTPUT; ++i ) {
        // insert each type from the enumerated list into Python
        string str = accessTypeToString( (Field::AccessType)i );
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
           i != dn->end(); ++i ) {
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
      { "fieldSetValue", pythonFieldSetValue, 0 },
      { "fieldGetValue", pythonFieldGetValue, 0 },
      { "fieldSetAccessType", pythonFieldSetAccessType, 0 },
      { "fieldGetAccessType", pythonFieldGetAccessType, 0 },
      { "fieldSetAccessCheck", pythonFieldSetAccessCheck, 0 },
      { "fieldIsAccessCheckOn", pythonFieldIsAccessCheckOn, 0 },
      { "fieldRoute", pythonFieldRoute, 0 },
      { "fieldRouteNoEvent", pythonFieldRouteNoEvent, 0 },
      { "fieldUnroute", pythonFieldUnroute, 0 },
      { "fieldReplaceRoute", pythonFieldReplaceRoute, 0 },
      { "fieldReplaceRouteNoEvent", pythonFieldReplaceRouteNoEvent, 0 },
      { "fieldUnrouteAll", pythonFieldUnrouteAll, 0 },
      { "getCPtr", pythonGetCPtr, 0 },
      { "writeNodeAsX3D", pythonWriteNodeAsX3D, 0 },
      { "createX3DFromURL", pythonCreateX3DFromURL, 0 },
      { "createX3DFromString", pythonCreateX3DFromString, 0 },
      { "createX3DNodeFromURL", pythonCreateX3DNodeFromURL, 0 },
      { "createX3DNodeFromString", pythonCreateX3DNodeFromString, 0 },
      { "createVRMLFromURL", pythonCreateVRMLFromURL, 0 },
      { "createVRMLFromString", pythonCreateVRMLFromString, 0 },
      { "createVRMLNodeFromURL", pythonCreateVRMLNodeFromURL, 0 },
      { "createVRMLNodeFromString", pythonCreateVRMLNodeFromString, 0 },
      { "fieldRoutesTo", pythonFieldRoutesTo, 0 },
      { "fieldHasRouteFrom", pythonFieldHasRouteFrom, 0 },
      { "fieldGetRoutesIn", pythonFieldGetRoutesIn, 0 },
      { "fieldGetRoutesOut", pythonFieldGetRoutesOut, 0 },
      { "getCurrentScenes", pythonGetCurrentScenes, 0 },
      { "getActiveDeviceInfo", pythonGetActiveDeviceInfo, 0 },
      { "getActiveViewpoint", pythonGetActiveViewpoint, 0 },
      { "getActiveBindableNode", pythonGetActiveBindableNode, 0 },
      { "getActiveNavigationInfo", pythonGetActiveNavigationInfo, 0 },
      { "getActiveFog", pythonGetActiveFog, 0 },
      { "getActiveGlobalSettings", pythonGetActiveGlobalSettings, 0 },
      { "getActiveStereoInfo", pythonGetActiveStereoInfo, 0 },
      { "getActiveBackground", pythonGetActiveBackground, 0 },
      { "MFieldErase", pythonMFieldErase, 0 },
      { "MFieldPushBack", pythonMFieldPushBack, 0 },
      { "MFieldClear", pythonMFieldClear, 0 },
      { "MFieldBack", pythonMFieldBack, 0 },
      { "MFieldFront", pythonMFieldFront, 0 },
      { "MFieldEmpty", pythonMFieldEmpty, 0 },
      { "MFieldPopBack", pythonMFieldPopBack, 0 },
      { "MFieldSize", pythonMFieldSize, 0 },
      { "fieldTouch", pythonFieldTouch, 0 },
      { "resolveURLAsFile", pythonResolveURLAsFile, 0 },
      { "resolveURLAsFolder", pythonResolveURLAsFolder, 0 },
      { "throwQuitAPIException", throwQuitAPIException, 0 },
      { "createNode", pythonCreateNode, 0 },
      { "getHapticsDevice", pythonGetHapticsDevice, 0 },
      { "getNrHapticsDevices", pythonGetNrHapticsDevices, 0 },
      { "getNamedNode", pythonGetNamedNode, 0 },
      { "fieldSetName", pythonFieldSetName, 0 },
      { "fieldGetName", pythonFieldGetName, 0 },
      { "fieldGetFullName", pythonFieldGetFullName, 0 },
      { "fieldGetTypeName", pythonFieldGetTypeName, 0 },
      { "fieldGetOwner", pythonFieldGetOwner, 0 },
      { "fieldSetOwner", pythonFieldSetOwner, 0 },
      { "fieldSetValueFromString", pythonFieldSetValueFromString, 0 },
      { "fieldGetValueAsString", pythonFieldGetValueAsString, 0 },
      { "fieldUpToDate", pythonFieldUpToDate, 0 },
      { "fieldIsUpToDate", pythonFieldIsUpToDate, 0 },
      { "addProgramSetting", pythonAddProgramSetting, 0 },
      { "findNodes", pythonFindNodes, 0 },
      { "takeScreenshot", pythonTakeScreenshot, 0 },
      { NULL, NULL }      
    };
    
    PyMODINIT_FUNC initH3D() {
      if( H3D_module.get() )
        return;

      H3D_module.reset( Py_InitModule( "H3D", H3DMethods ), true );
      H3D_dict = PyModule_GetDict( H3D_module.get() );


      PyNode::installType( H3D_module.get() );     
      PyVec2f::installType( H3D_module.get() );     
      PyVec2d::installType( H3D_module.get() );     
      PyVec3f::installType( H3D_module.get() );     
      PyVec3d::installType( H3D_module.get() );     
      PyVec4f::installType( H3D_module.get() );     
      PyVec4d::installType( H3D_module.get() );     
      PyRGB::installType( H3D_module.get() );     
      PyRGBA::installType( H3D_module.get() );     
      PyRotation::installType( H3D_module.get() );      
      PyQuaternion::installType( H3D_module.get() );
      PyMatrix3f::installType( H3D_module.get() );     
      PyMatrix4f::installType( H3D_module.get() );
      PyMatrix3d::installType( H3D_module.get() );     
      PyMatrix4d::installType( H3D_module.get() );
      PyConsole::installType( H3D_module.get() );


      PythonInternals::insertFieldTypes( H3D_dict );
      PythonInternals::insertFieldAccessTypes( H3D_dict );

#ifdef H3DINTERFACE_AS_FILE
      PythonInternals::H3DInterface_module.reset( 
        PyImport_ImportModule( "H3DInterface" ) );
      if ( PythonInternals::H3DInterface_module.get() == NULL ) {
        PyErr_Print();
        Console(4) << "PythonScript::initialiseParser() - ";
        Console(4) << "  Error importing H3DInterface module, check that you have a valid PYTHONPATH environment variable and try again." << endl;
        exit(-1); // SHOULD THROW AN ERROR!
      }
      PythonInternals::H3DInterface_dict = 
        PyModule_GetDict( PythonInternals::H3DInterface_module.get() );
#else
      if ( !PythonInternals::H3DInterface_module.get() ) {
        PythonInternals::H3DInterface_module.reset( 
          PyImport_AddModule( "H3DInterface" ), true ); 
        PythonInternals::H3DInterface_dict = 
          PyModule_GetDict( PythonInternals::H3DInterface_module.get() );
        // install the buildins in the module
        if( PythonInternals::H3DInterface_dict != NULL ) {
          if (PyDict_GetItemString(  PythonInternals::H3DInterface_dict, 
            "__builtins__") == NULL) {
              if (PyDict_SetItemString(  PythonInternals::H3DInterface_dict, 
                "__builtins__",
                PyEval_GetBuiltins()) != 0)
                Console(3) << "Warning: PyEval_GetBuiltins() could not be installed in module dictionary!" << endl;
          }
        }

        string a = H3DInterface::H3DInterface_string;
        PyObject *r = PyRun_String( H3DInterface::H3DInterface_string.c_str(), 
          Py_file_input,
          PythonInternals::H3DInterface_dict,
          PythonInternals::H3DInterface_dict );
        if ( r == NULL ) {
          Console( 3 ) << "Python error in file H3DInterface.py.h:" << endl;
          PyErr_Print();
        }
      }

#endif

      if ( PythonInternals::H3DInterface_dict == NULL )
        PyErr_Print();
      PyObject *time = (PyObject*)fieldAsPythonObject( Scene::time.get(), false );
      //PyObject *time = (PyObject*)fieldAsPythonObject( Scene::time, false );
      PyDict_SetItem( PythonInternals::H3DInterface_dict, 
                      PyString_FromString( "time" ), 
                      time );
      Py_DECREF( time );
      PyObject *event_sink = (PyObject*)fieldAsPythonObject( Scene::eventSink.get(), false );
      //PyObject *event_sink = (PyObject*)fieldAsPythonObject( Scene::eventSink, false );
      PyDict_SetItem( PythonInternals::H3DInterface_dict, 
                      PyString_FromString( "eventSink" ), 
                      event_sink );
      Py_DECREF( event_sink );

#ifndef H3DINTERFACE_AS_FILE
      if( !PythonInternals::H3DUtils_module.get() ) {
        PythonInternals::H3DUtils_module.reset( 
          PyImport_AddModule( "H3DUtils" ), true ); 
        PythonInternals::H3DUtils_dict = 
          PyModule_GetDict( PythonInternals::H3DUtils_module.get() );
        // install the buildins in the module
        if( PythonInternals::H3DUtils_dict != NULL ) {
          if (PyDict_GetItemString(  PythonInternals::H3DUtils_dict, 
            "__builtins__") == NULL) {
              if (PyDict_SetItemString(  PythonInternals::H3DUtils_dict, 
                "__builtins__",
                PyEval_GetBuiltins()) != 0)
                Console(3) << "Warning: PyEval_GetBuiltins() could not be installed in module dictionary!" << endl;
          }
        }

        string a = H3DInterface::H3DUtils_string;
        PyObject *r = PyRun_String( H3DInterface::H3DUtils_string.c_str(), 
          Py_file_input,
          PythonInternals::H3DUtils_dict,
          PythonInternals::H3DUtils_dict );
        if ( r == NULL ) {
          Console( 3 ) << "Python error in file H3DInterface.py.h:" << endl;
          PyErr_Print();
        }
      }
#endif
    };
  
    void fieldDestructor( void *f ) {
      Field *field = static_cast<Field*>(f);
      if ( field ) {
        delete field;
      }
    }

    PyObject *fieldAsPythonObject( Field * f, bool destruct ) {
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
      PyObject *field_ptr;
      if ( destruct )
          field_ptr = PyCObject_FromVoidPtr( f, fieldDestructor );
      else
          field_ptr = PyCObject_FromVoidPtr( f, NULL );
      PyObject_SetAttrString( py_field, "__fieldptr__", field_ptr );
      Py_DECREF( field_ptr);
      return py_field;      
    }

    // Methods for the H3D Module:
    
    PyObject *pythonCreateField( PyObject *self, PyObject *args ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createField( Field, int auto_update, name )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return NULL;
      }
      PyObject *field = PyTuple_GetItem( args, 0 );
      PyObject *autoupdateobj = PyTuple_GetItem( args, 1 );
      PyObject *name = PyTuple_GetItem( args, 2 );

      bool autoupdate = PyInt_AsLong( autoupdateobj ) != 0;
      
      if( field && PyInstance_Check( field ) ) {
        //PyObject_SetAttrString( field, "__fieldtype__", field );

        PyObject *fieldtype = PyObject_GetAttrString( field, "type" );
        int field_type = PyInt_AsLong( fieldtype );
        Py_DECREF( fieldtype );
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
        
        PyObject *pfield = PyCObject_FromVoidPtr( f, fieldDestructor );
        PyObject_SetAttrString( field, "__fieldptr__", pfield );
        // field now holds a reference to pfield so we can remove the extra reference
        // from the all to PyCObject_FromVoidPtr()
        Py_DECREF( pfield );
        PyObject *update = PyObject_GetAttrString( field, "update" );
        if( update ) {
          if( PyMethod_Check( update ) ) {
            // Set flag to indicate that this attribute exists
            // and the method should be run when updating.
            dynamic_cast< PythonFieldBase* >(f)->have_update = true;
          } else {
            Py_DECREF( update );
            PyErr_SetString( PyExc_ValueError, 
                             "Symbol 'update' must be a method!" );
            return NULL;
          }
          Py_DECREF( update );
        } else PyErr_Clear();
  
        PyObject *typeinfo = PyObject_GetAttrString( field, "__type_info__" );
        if( typeinfo ) {
          if( PyTuple_Check( typeinfo ) ) {
            // Set flag to indicate that this attribute exists.
            // Used by PythonFieldBase::checkFieldType.
            dynamic_cast< PythonFieldBase* >(f)->have_type_info = true;
          } else {
            Py_DECREF( typeinfo );
            PyErr_SetString( PyExc_ValueError, 
                             "Symbol '__type_info__' must be a tuple!" );
            return NULL;
          }
          Py_DECREF( typeinfo );
        } else PyErr_Clear();

        PyObject *opttypeinfo = 
          PyObject_GetAttrString( field, "__opt_type_info__" );
        if( opttypeinfo ) {
          if( PyTuple_Check( opttypeinfo ) ) {
            // Set flag to indicate that this attribute exists.
            // Used by PythonFieldBase::checkFieldType.
            dynamic_cast< PythonFieldBase* >(f)->have_opt_type_info = true;
          } else {
            Py_DECREF( opttypeinfo );
            PyErr_SetString( PyExc_ValueError, 
                             "Symbol '__opt_type_info__' must be a tuple!" );
            return NULL;
          }
          Py_DECREF( opttypeinfo );
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

    PyObject *pythonFieldSetValue( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldSetValue( self, value )" );
        return 0;
      }
  
      PyObject *field = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid Field type given as argument to H3D.fieldSetValue( self, value )" );
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
      Py_DECREF( py_field_ptr );

      PyObject *v = PyTuple_GetItem( args, 1 );
      return PythonInternals::pythonSetFieldValueFromObject( field_ptr, v );
    }
    
    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldGetValue( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldGetValue( self )" );
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
      
      Py_DECREF( py_field_ptr );

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

    PyObject *pythonFieldGetAccessType( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldGetAccessType( self )" );
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
      
      Py_DECREF( py_field_ptr );

      if( field_ptr ) { 
        return PyInt_FromLong( field_ptr->getAccessType() );
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldSetAccessType( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyTuple_Check( arg ) || PyTuple_Size( arg ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldSetAccessType( self, access_type )" );
        return 0;
      }
  
      PyObject *field = PyTuple_GetItem( arg, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid Field type given as argument to H3D.fieldSetAccessType( self, access_type )" );
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
      Py_DECREF( py_field_ptr );

      if( field_ptr ) {
      
        PyObject *at_py = PyTuple_GetItem( arg, 1 );      
        long at = PyInt_AsLong( at_py );
        Field::AccessType access_type;
        if ( at == 0 )
          access_type = Field::INITIALIZE_ONLY;
        else if ( at == 1 )
          access_type = Field::OUTPUT_ONLY;
        else if ( at == 2 )
          access_type = Field::INPUT_ONLY;
        else if ( at == 3 )
          access_type = Field::INPUT_OUTPUT;
        else {
          PyErr_SetString( PyExc_ValueError,
              "Error: Access type is not valid in H3D.fieldSetAccessType( self, access_type )" );
          return 0;
        }
        field_ptr->setAccessType( access_type );
        Py_INCREF(Py_None);
        return Py_None; 
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldSetAccessCheck( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyTuple_Check( arg ) || PyTuple_Size( arg ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldSetAccessCheck( self, access_check )" );  
        return 0;
      }

      PyObject *py_field_obj = PyTuple_GetItem( arg, 0 );
      if( ! PyInstance_Check( py_field_obj ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.fieldSetAccessCheck( self, access_check )" );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( py_field_obj, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }

      PyObject *py_access_check = PyTuple_GetItem( arg, 1 );
      if( !PyInt_Check( py_access_check ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument type. Expecting boolean" );
        return 0;
      }

      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      Py_DECREF( py_field_ptr );
      bool access_check = PyInt_AsLong( py_access_check );
      
      if( field_ptr ) { 
        field_ptr->setAccessCheck( access_check );
        Py_INCREF(Py_None);
        return Py_None;
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldIsAccessCheckOn( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldGetAccessType( self )" );
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
      Py_DECREF( py_field_ptr );

      if( field_ptr ) {
        if(field_ptr->isAccessCheckOn())
          Py_RETURN_TRUE;
        Py_RETURN_FALSE;
      }
      PyErr_SetString( PyExc_ValueError, 
                       "Error: Field NULL pointer" );
      return 0;  
    }

    /////////////////////////////////////////////////////////////////////////

    // help function for pythonFieldRoute and pythonFieldRouteNoEvent.
    PyObject *pythonFieldUnroute( PyObject *self, 
                                  PyObject *args  ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldUnroute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *from_field = PyTuple_GetItem( args, 0 );
      PyObject *to_field = PyTuple_GetItem( args, 1 );
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.fieldUnroute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( to_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.fieldUnroute( fromField, toField )";
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
      
      Py_DECREF( py_to_field_ptr );
      Py_DECREF( py_from_field_ptr );

      if( from_field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.fieldUnroute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( to_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.fieldUnroute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      try {
        from_field_ptr->unroute( to_field_ptr );
      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }
      
      Py_INCREF(Py_None);
      return Py_None; 
    }


    // help function for pythonFieldRoute and pythonFieldRouteNoEvent.
    PyObject *pythonGetCPtr( PyObject *self, 
                             PyObject *arg  ) {
      if( !arg || ! PyInstance_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getCPtr( Field f )."
            << " Requires one argument of type Field. ";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( arg, 
                                                            "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );

      Py_DECREF( py_field_ptr );
      
      return PyInt_FromLong( (long)field_ptr );
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

    // help function for pythonFieldRoute and pythonFieldRouteNoEvent.
    PyObject *pythonRouteFieldHelp( PyObject *self, 
                                    PyObject *args, 
                                    bool send_event  ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldRoute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *from_field = PyTuple_GetItem( args, 0 );
      PyObject *to_field = PyTuple_GetItem( args, 1 );
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.fieldRoute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( to_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.fieldRoute( fromField, toField )";
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
        err << "Source not a Field class in H3D.fieldRoute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( to_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.fieldRoute( fromField, toField )";
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
        errstr << e;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }

      Py_DECREF( py_to_field_ptr );
      Py_DECREF( py_from_field_ptr );
      Py_INCREF(Py_None);
      return Py_None; 
    }
    
    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateX3DFromURL( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromURL( filename )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *filename = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try{
        Node *n = X3D::createX3DFromURL( filename, &dm );
        return PythonInternals::createX3DHelp( n, &dm );
      }
      catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D from URL: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateX3DFromString( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromString( s )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *s = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try{
        Node *n = X3D::createX3DFromString( s, &dm );
        return PythonInternals::createX3DHelp( n, &dm );
      }
      catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D from string: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonWriteNodeAsX3D( PyObject *self, PyObject *arg ) {
      if( !arg || !PyNode_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.writeNodeAsX3D( node )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      Node *n = PyNode_AsNode( arg );
      stringstream s;
      X3D::writeNodeAsX3D( s, n );

      PyObject *pstring = PyString_FromString( s.str().c_str() );
      return pstring;
      
    }


    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateX3DNodeFromURL( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromURL( filename )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *filename = PyString_AsString( arg );
      X3D::DEFNodes dm;

       PyThreadState *_save;
       // release the interpreter lock to let other python threads execute while setting
       // field value. Need to make sure here that that if any exception is thrown we 
       // reaquire the lock with Py_BLOCK_THREADS

      try{
        AutoRef< Node > n;
        Py_UNBLOCK_THREADS
        n= X3D::createX3DNodeFromURL( filename, &dm );
        Py_BLOCK_THREADS
        return PythonInternals::createX3DHelp( n.get(), &dm );
      }
      catch(H3D::X3D::XMLParseError &e){
        Py_BLOCK_THREADS
        ostringstream err;
        err << "Error creating X3D Node from URL: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
      catch ( H3D::Exception::H3DException &e ) {
        // H3D error are set as Python exceptions
        Py_BLOCK_THREADS
        ostringstream errstr;
        errstr << e;
        PyErr_SetString( PyExc_RuntimeError, errstr.str().c_str() );
        return 0;
      } catch( ... ) {
        // rethrow all other exceptions
        Py_BLOCK_THREADS
        throw;
      }
    }

    PyObject* pythonCreateX3DNodeFromString( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DNodeFromString( s )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *s = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try{
        AutoRef< Node > n = X3D::createX3DNodeFromString( s, &dm );
        return PythonInternals::createX3DHelp( n.get(), &dm );
      }
      catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D Node from string: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateVRMLFromURL( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createX3DFromURL( filename )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *filename = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try {
        Node *n = X3D::createX3DFromURL( filename, &dm );
        return PythonInternals::createX3DHelp( n, &dm );
      } catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D Node from string: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateVRMLFromString( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createVRMLFromString( s )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *s = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try {
        Node *n = X3D::createVRMLFromString( s, &dm );
        return PythonInternals::createX3DHelp( n, &dm );
      } catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D Node from string: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonCreateVRMLNodeFromURL( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createVRMLFromURL( filename )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *filename = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try {
        AutoRef< Node > n = X3D::createVRMLNodeFromURL( filename, &dm );
        return PythonInternals::createX3DHelp( n.get(), &dm );
      } catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D Node from string: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    PyObject* pythonCreateVRMLNodeFromString( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.createVRMLFromString( s )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      char *s = PyString_AsString( arg );
      X3D::DEFNodes dm;
      try {
        AutoRef< Node > n = X3D::createVRMLNodeFromString( s, &dm );
        return PythonInternals::createX3DHelp( n.get(), &dm );
      } catch(H3D::X3D::XMLParseError &e){
        ostringstream err;
        err << "Error creating X3D Node from string: " << e.message;
        PyErr_SetString( PyExc_RuntimeError, err.str().c_str() );
        return 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonFieldRoutesTo( PyObject *self, PyObject *arg ) {
      if( !arg || !PyTuple_Check( arg ) || PyTuple_Size( arg ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldRoutesTo( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *from_field = PyTuple_GetItem( arg, 0 );
      PyObject *to_field = PyTuple_GetItem( arg, 1 );
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.fieldRoutesTo( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( to_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.fieldRoutesTo( f )";
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
      
      Py_DECREF( py_to_field_ptr );
      Py_DECREF( py_from_field_ptr );

      if( from_field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.fieldRoutesTo( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( to_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.fieldRoutesTo( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      try {

        if ( from_field_ptr->routesTo( to_field_ptr ) )
          Py_RETURN_TRUE;
        Py_RETURN_FALSE;

      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonFieldHasRouteFrom( PyObject *self, PyObject *arg ) {
      if( !arg || !PyTuple_Check( arg ) || PyTuple_Size( arg ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldHasRouteFrom( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *field = PyTuple_GetItem( arg, 0 );
      PyObject *from_field = PyTuple_GetItem( arg, 1 );
      if( !PyInstance_Check( field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.fieldHasRouteFrom( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.fieldHasRouteFrom( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      PyObject *py_field_ptr = PyObject_GetAttrString( field, 
                                                            "__fieldptr__" );
      PyObject *py_from_field_ptr = PyObject_GetAttrString( from_field, 
                                                          "__fieldptr__" );
      if( !py_field_ptr || !py_from_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      Field *from_field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_from_field_ptr ) );
      
      Py_DECREF( py_from_field_ptr );
      Py_DECREF( py_field_ptr );

      if( field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.fieldHasRouteFrom( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( from_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.fieldHasRouteFrom( f )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      try {

        if ( field_ptr->hasRouteFrom( from_field_ptr ) )
          Py_RETURN_TRUE;
        Py_RETURN_FALSE;

      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonFieldGetRoutesIn( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyInstance_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldGetRoutesIn( Field f )."
            << " Requires one argument of type Field. ";
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
      Field::FieldVector routes_in =  field_ptr->getRoutesIn();
      PyObject *retval = PyTuple_New( routes_in.size() );

      for( unsigned int i = 0; i < routes_in.size() ; ++i ) {
        PyObject *t = (PyObject *)PythonInternals::fieldAsPythonObject( routes_in[i] );
        PyTuple_SetItem( retval, i, t );
      }
      Py_DECREF( py_field_ptr );
      return retval;
    }

    /////////////////////////////////////////////////////////////////////////

    // function for returning a tuple of the Fields that the field given
    // as arg is routed to.
    PyObject* pythonFieldGetRoutesOut( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyInstance_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldGetRoutesOut( Field f )."
            << " Requires one argument of type Field. ";
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
      for( ; fi != routes_out.end(); ++i,++fi ) {
      PyObject *t = (PyObject *) PythonInternals::fieldAsPythonObject( *fi );
        PyTuple_SetItem( retval, i, t );
      }
      Py_DECREF( py_field_ptr );
      return retval;
    }

    PyObject *pythonFieldRoute( PyObject *self, 
                                PyObject *args ) {
      return pythonRouteFieldHelp( self, args, true );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldRouteNoEvent( PyObject *self, 
                                       PyObject *args ) {
      return pythonRouteFieldHelp( self, args, false );
    }

    /////////////////////////////////////////////////////////////////////////
    PyObject *pythonFieldUnrouteAll( PyObject *self, PyObject *arg  ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldUnrouteAll( self )" );
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
      Py_DECREF( py_field_ptr );

      if( field_ptr ) { 
        string type_name;
        PyThreadState *_save;
        // release the interpreter lock to let other python threads execute while updating
        // field value. Need to make sure here that that if any exception is thrown we 
        // reaquire the lock with Py_BLOCK_THREADS
        try {
          Py_UNBLOCK_THREADS
          field_ptr->unrouteAll();
          Py_BLOCK_THREADS
          Py_INCREF(Py_None);
          return Py_None; 
        } catch ( H3D::Exception::H3DException &e ) {
          Py_BLOCK_THREADS
          ostringstream errstr;
          errstr << e;
          PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
          return 0;
        } catch( ... ) {
          // rethrow all other exceptions
          Py_BLOCK_THREADS
          throw;
        }        
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }      
    }

    /////////////////////////////////////////////////////////////////////////
    
    // help function for pythonFieldReplaceRoute and pythonFieldReplaceRouteNoEvent.
    PyObject *pythonFieldReplaceRouteHelp( PyObject *self, 
                                    PyObject *args, 
                                    bool send_event  ) {
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldReplaceRoute( fromField, toField )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      
      PyObject *from_field = PyTuple_GetItem( args, 0 );
      PyObject *to_field = PyTuple_GetItem( args, 1 );
      PyObject *route_id = PyTuple_GetItem( args, 2 );
      
      if( !PyInstance_Check( from_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.fieldReplaceRoute( )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      if( !PyInstance_Check( to_field ) ) {
        ostringstream err;
        err << "Invalid Field type given as toField argument to H3D.fieldReplaceRoute( )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      
      if( !PyInt_Check( route_id ) ) {
      
        ostringstream err;
        err << "Invalid index argument to function H3D.fieldReplaceRoute(  ).";
        err << " index should be of int type.";
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

      Py_DECREF( py_to_field_ptr );      
      Py_DECREF( py_from_field_ptr );

      if( from_field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.fieldReplaceRoute( )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      if( to_field_ptr == 0 ) {
        ostringstream err;
        err << "Destination not a Field class in call to H3D.fieldReplaceRoute( )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      try {

        Field *replaced_field;
        int route_index = (int)( PyInt_AsLong( route_id ) );
        if( send_event )
          replaced_field = from_field_ptr->replaceRoute( to_field_ptr, route_index );
        else
          replaced_field = from_field_ptr->replaceRouteNoEvent( to_field_ptr, route_index );
        
        return ( PyObject * ) PythonInternals::fieldAsPythonObject( replaced_field, true );        

      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return NULL;
      }

    }
    
    /////////////////////////////////////////////////////////////////////////
    PyObject *pythonFieldReplaceRoute( PyObject *self, PyObject *args ) {
      return pythonFieldReplaceRouteHelp( self, args, true );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldReplaceRouteNoEvent( PyObject *self, PyObject *args ) {
      return pythonFieldReplaceRouteHelp( self, args, false );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetCurrentScenes( PyObject *self, PyObject *arg ) {
      if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getCurrentScenes()."
            << "Function does not take any arguments.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      PyObject *scenes = PyTuple_New( Scene::scenes.size() );
      unsigned int counter = 0;
      for( set< Scene * >::const_iterator i = Scene::scenes.begin(); 
           i != Scene::scenes.end(); ++i, ++counter ) {
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
            << "Function does not take any arguments.";
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
            << "Function does not take any arguments.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( X3DViewpointNode::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveBindableNode( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveBindableNode( bindable )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      char *bindable = PyString_AsString( arg );
      return PyNode_FromNode( X3DBindableNode::getActive( bindable ) );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveFog( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveFog()."
            << "Function does not take any arguments.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( Fog::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveGlobalSettings( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveGlobalSettings()."
            << "Function does not take any arguments.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( GlobalSettings::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonGetActiveNavigationInfo( PyObject *self, PyObject *arg ) {
     if( arg ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getActiveNavigationInfo()."
            << "Function does not take any arguments.";
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
            << "Function does not take any arguments.";
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
            << "Function does not take any arguments.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      return PyNode_FromNode( X3DBackgroundNode::getActive() );
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject* pythonMFieldErase( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
       "Invalid argument(s) to function H3D.MFieldErase( self, value )" );  
        return 0;
      }

      PyObject *field = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.MFieldErase( self, value )" );
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
      Py_DECREF( py_field_ptr );
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

   PyObject* pythonMFieldPushBack( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
    "Invalid argument(s) to function H3D.MFieldPushBack( self, value )" );  
        return 0;
      }

      PyObject *field = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( field ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.MFieldPushBack( self, value )" );
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
      Py_DECREF( py_field_ptr );
      Py_INCREF(Py_None);
      return Py_None; 
    }

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonFieldTouch( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldTouch( self )" );
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

      Py_DECREF( py_field_ptr );
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
      Py_DECREF( py_field_ptr );
      
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
      Py_DECREF( py_field_ptr );
      
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
      Py_DECREF( py_field_ptr );
      
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
      Py_DECREF( py_field_ptr );
      
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
      Py_DECREF( py_field_ptr );
      
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

    /////////////////////////////////////////////////////////////////////////

    PyObject *pythonMFieldSize( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.MFieldSize( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        bool success;
        APPLY_MFIELD_MACRO( field_ptr, field_ptr->getX3DType(), 0, 
                            MFIELD_SIZE, success );
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

    PyObject *pythonFieldGetValueAsString( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldGetValueAsString( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        ParsableField *parsable_field = dynamic_cast< ParsableField * >( field_ptr );
        if( parsable_field ) {
          string type_name;
                  PyThreadState *_save;
          // release the interpreter lock to let other python threads execute while setting
          // field value. Need to make sure here that that if any exception is thrown we 
          // reaquire the lock with Py_BLOCK_THREADS
          try {
            Py_UNBLOCK_THREADS
            type_name = parsable_field->getValueAsString();
            Py_BLOCK_THREADS
          }
          catch ( H3D::Exception::H3DException &e ) {
            // H3D error are set as Python exceptions
            Py_BLOCK_THREADS
            ostringstream errstr;
            errstr << e;
            PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
            return 0;
          } catch( ... ) {
            // rethrow all other exceptions
            Py_BLOCK_THREADS
            throw;
          }
          return PyString_FromString( type_name.c_str() );      
        } else {
          PyErr_SetString( PyExc_ValueError, 
                           "Field not a ParsableField." );
          return 0; 
        }
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }
    }

    PyObject *pythonFieldSetValueFromString( PyObject *self, PyObject *args ) {
      if( !args || ! PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldSetValueFromString( self, value )" );  
        return 0;
      }

      PyObject *py_field_obj = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( py_field_obj ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.fieldSetValueFromString( self, value )" );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( py_field_obj, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }

      PyObject *py_value_string = PyTuple_GetItem( args, 1 );
      if( !PyString_Check( py_value_string ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument type. Expecting string" );
        return 0;
      }
      
      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      string value_str( PyString_AsString( py_value_string ) );
      Py_DECREF( py_field_ptr );
      if( field_ptr ) {  
        ParsableField *parsable_field = dynamic_cast< ParsableField * >( field_ptr );
        if( parsable_field ) {
          PyThreadState *_save;
          // release the interpreter lock to let other python threads execute while setting
          // field value. Need to make sure here that that if any exception is thrown we 
          // reaquire the lock with Py_BLOCK_THREADS

          try {
            Py_UNBLOCK_THREADS
            parsable_field->setValueFromString( value_str );
            Py_BLOCK_THREADS
          } catch ( H3D::Exception::H3DException &e ) {
            // H3D error are set as Python exceptions
            Py_BLOCK_THREADS
            ostringstream errstr;
            errstr << e;
            PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
            return 0;
          } catch( ... ) {
            // rethrow all other exceptions
            Py_BLOCK_THREADS
            throw;
          }
        } else {
          PyErr_SetString( PyExc_ValueError, 
                           "Field not a ParsableField." );
          return 0; 
        }
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }

      Py_INCREF(Py_None);
      return Py_None; 
       //Console(4) <<"11" << endl;
    }

    PyObject *pythonFieldSetName( PyObject *self, PyObject *arg ) {
      if( !arg || ! PyTuple_Check( arg ) || PyTuple_Size( arg ) != 2  ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldSetValueFromString( self, value )" );  
        return 0;
      }

      PyObject *py_field_obj = PyTuple_GetItem( arg, 0 );
      if( ! PyInstance_Check( py_field_obj ) ) {
        PyErr_SetString( PyExc_ValueError, 
 "Invalid Field type given as argument to H3D.fieldSetName( self, value )" );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( py_field_obj, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }

      PyObject *py_value_string = PyTuple_GetItem( arg, 1 );
      if( !PyString_Check( py_value_string ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument type. Expecting string" );
        return 0;
      }

      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      string value_str( PyString_AsString( py_value_string ) );
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) {
        field_ptr->setName( value_str );
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;
      }
      Py_INCREF(Py_None);
      return Py_None;
    }

    PyObject *pythonFieldGetName( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldGetName( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        string type_name = field_ptr->getName();
        return PyString_FromString( type_name.c_str() );        
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }
    }

    PyObject *pythonFieldGetFullName( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldGetFullName( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        string type_name = field_ptr->getFullName();
        return PyString_FromString( type_name.c_str() );        
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }
    }

    PyObject *pythonFieldGetTypeName( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldGetTypeName( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        string type_name = field_ptr->getTypeName();
        return PyString_FromString( type_name.c_str() );        
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }
    }

    PyObject *pythonFieldGetOwner( PyObject *self, PyObject *arg ) {
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldGetOwner( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        return PyNode_FromNode( field_ptr->getOwner() );
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }
    }

    PyObject *pythonFieldSetOwner( PyObject *self, 
                                  PyObject *args  ) {
      
      if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.fieldSetOwner( n )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *py_field_obj = PyTuple_GetItem( args, 0 );
      if( ! PyInstance_Check( py_field_obj ) ) {
        ostringstream err;
        err << "Invalid Field type given as fromField argument to H3D.fieldSetOwner( n )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      PyObject *py_field_ptr = PyObject_GetAttrString( py_field_obj, "__fieldptr__" );
      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Python object not a Field type. Make sure that if you \
have defined an __init__ function in a specialized field class, you \
call the base class __init__ function." );
        return 0;
      }

      Field *field_ptr = static_cast< Field * >
        ( PyCObject_AsVoidPtr( py_field_ptr ) );
      Py_DECREF( py_field_ptr );

      if( field_ptr == 0 ) {
        ostringstream err;
        err << "Source not a Field class in H3D.fieldSetOwner( n )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;       
      }
      
      try {

        PyObject *owner_node = PyTuple_GetItem( args, 1 );
        field_ptr->setOwner( PyNode_AsNode( owner_node ) );
        Py_INCREF(Py_None);
        return Py_None; 

      } catch ( H3D::Exception::H3DException &e ) {
        ostringstream errstr;
        errstr << e;
        PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
        return 0;
      }     
    }

    PyObject *pythonFieldUpToDate( PyObject *self, PyObject *arg ) {
      
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                 "Invalid argument(s) to function H3D.fieldUpToDate( self )" );
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
      Py_DECREF( py_field_ptr );
      
      if( field_ptr ) { 
        string type_name;
        PyThreadState *_save;
        // release the interpreter lock to let other python threads execute while updating
        // field value. Need to make sure here that that if any exception is thrown we 
        // reaquire the lock with Py_BLOCK_THREADS
        try {
          Py_UNBLOCK_THREADS
          field_ptr->upToDate();
          Py_BLOCK_THREADS
          Py_INCREF(Py_None);
          return Py_None; 
        }
        catch ( H3D::Exception::H3DException &e ) {
          // H3D error are set as Python exceptions
          Py_BLOCK_THREADS
          ostringstream errstr;
          errstr << e;
          PyErr_SetString( PyExc_ValueError, errstr.str().c_str() );
          return 0;
        } catch( ... ) {
          // rethrow all other exceptions
          Py_BLOCK_THREADS
          throw;
        }        
      } else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;  
      }
      return 0;
    }

    PyObject *pythonFieldIsUpToDate( PyObject *self, PyObject *arg ) {
      
      if(!arg || ! PyInstance_Check( arg ) ) {
        PyErr_SetString( PyExc_ValueError, 
                         "Invalid argument(s) to function H3D.fieldTouch( self )" );
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
      if( field_ptr ){
        if ( field_ptr->isUpToDate() )
          Py_RETURN_TRUE;
        Py_RETURN_FALSE;
      }
      else {
        PyErr_SetString( PyExc_ValueError, 
                         "Error: Field NULL pointer" );
        return 0;
      }
    }

    PyObject *pythonResolveURLAsFile( PyObject *self, PyObject *args ) {

      if( !args || !PyString_Check( args ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.resolveURLAsFile( url )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      char *url = PyString_AsString( args );
      string resolved_url = ResourceResolver::resolveURLAsFile( url );
      return PyString_FromString( resolved_url.c_str() );
    }

    PyObject *pythonResolveURLAsFolder( PyObject *self, PyObject *args ) {

      if( !args || !PyString_Check( args ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.resolveURLAsFolder( url )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      char *url = PyString_AsString( args );
      string resolved_url = ResourceResolver::resolveURLAsFolder( url );
      return PyString_FromString( resolved_url.c_str() );
    }

    PyObject *throwQuitAPIException( PyObject *self, PyObject *args ) {
      throw Exception::QuitAPI();
      Py_INCREF(Py_None);
      return Py_None; 
    }

    PyObject* pythonCreateNode( PyObject *self, PyObject *args ) {
      //if( !arg || !PyString_Check( arg ) ) {
      //  ostringstream err;
      //  err << "Invalid argument(s) to function H3D.createNode( node_name ).";
      //  err << " node_name should be of string type.";
      //  PyErr_SetString( PyExc_ValueError, err.str().c_str() );
      //  return 0;
      //}

      //char *node_name = PyString_AsString( arg );
      //H3D::Node *node =  H3DNodeDatabase::createNode( node_name );
      //return PyNode_FromNode( H3DNodeDatabase::createNode( node_name ) );

      unsigned int nr_args = 1;     

      if( PyTuple_Check( args ) ) {
        nr_args =  PyTuple_Size( args );
        if( nr_args > 2 || nr_args < 1  ) {
          PyErr_SetString( PyExc_ValueError, 
            "Invalid argument(s) to function CreateNode( node_type, name )" );
          return NULL;
        } 
      }

      string node_name = "";
      if( nr_args > 1 ) {
        PyObject *py_name = PyTuple_GetItem( args, 0 ); // borrowed ref?
        if( !py_name || !PyString_Check( py_name ) ) {
          ostringstream err;
          err << "Invalid argument 0 to function H3D.CreateNode. Should be string with node type.";
          PyErr_SetString( PyExc_ValueError, err.str().c_str() );
          return 0;
        }
        node_name = PyString_AsString( py_name );
      } else {
        if( !args || !PyString_Check( args ) ) {
          ostringstream err;
          err << "Invalid argument(s) to function H3D.createNode( node_name ).";
          err << " node_name should be of string type.";
          PyErr_SetString( PyExc_ValueError, err.str().c_str() );
          return 0;
        }
        node_name = PyString_AsString( args );
      }
      H3D::Node *node =  H3DNodeDatabase::createNode( node_name );

      string name = "";
      if( nr_args > 1 ) {
        PyObject *py_name2 = PyTuple_GetItem( args, 1 ); // borrowed ref?
        if( !py_name2 || !PyString_Check( py_name2 ) ) {
          ostringstream err;
          err << "Invalid argument 1 to function H3D.CreateNode. Should be string.";
          PyErr_SetString( PyExc_ValueError, err.str().c_str() );
          return 0;
        }
        name = PyString_AsString( py_name2 );
      }

      if (name!="")
        node->setName(name);
      return PyNode_FromNode( node );
    }

    PyObject* pythonGetNrHapticsDevices( PyObject *self, PyObject *arg ) {
      DeviceInfo *di = DeviceInfo::getActive();
      size_t nr_devices = 0;
      if( di ) nr_devices = di->device->size();
      return PyInt_FromSize_t( nr_devices );
    }

    PyObject* pythonGetHapticsDevice( PyObject *self, PyObject *arg ) {
      if( !arg || !PyInt_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getHapticsDevice( index ).";
        err << " index should be of int type.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
      
      long index = PyInt_AsLong( arg );
      
      DeviceInfo *di = DeviceInfo::getActive();
      H3DHapticsDevice *hdev = NULL;
      if( di ) {
        size_t nr_devices = di->device->size();
        if( index >=0 && index < nr_devices ) {
          hdev = di->device->getValueByIndex( index );
        }
      }
      return PyNode_FromNode( hdev );
    }

    PyObject* pythonGetNamedNode( PyObject *self, PyObject *arg ) {
      if( !arg || !PyString_Check( arg ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.getNamedNode( node_name ).";
        err << " node_name should be of string type.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }

      char *node_name = PyString_AsString( arg );
      PyObject *globals = PyEval_GetGlobals(); // borrowed ref
      Node *n = NULL;
      if( globals ) {
        PyObject *py_scriptnode = 
          PyDict_GetItemString( globals, "__scriptnode__"); // borrowed ref
        if( py_scriptnode ) {
          PythonScript *ps = (PythonScript *) PyNode_AsNode( py_scriptnode );
          n = ps->getNamedNode( node_name );
        } 
      }
      return PyNode_FromNode( n );
    }

    PyObject *pythonAddProgramSetting( PyObject *self, PyObject *args ) {
      // args are (field, setting_name = "", section_name = "" )
      unsigned int nr_args = 1;     
      Field *field = NULL;
      PyObject *py_field = args;

      if( PyTuple_Check( args ) ) {
        nr_args =  PyTuple_Size( args );
        py_field = PyTuple_GetItem( args, 0 ); // borrowed ref ?
        if( nr_args > 3 || nr_args < 1  ) {
          PyErr_SetString( PyExc_ValueError, 
            "Invalid argument(s) to function addProgramSettings( field, setting_name, section_name )" );
          return NULL;
        } 
      }

      PyObject *py_field_ptr = 
        PyObject_GetAttrString( py_field, "__fieldptr__" ); //ref

      if( !py_field_ptr ) {
        PyErr_SetString( PyExc_ValueError, 
          "Invalid argument 0 to function addProgramSettings. Expecting Field type" );
        return NULL;
      }

      field = static_cast< Field * >( PyCObject_AsVoidPtr( py_field_ptr ) );
      Py_DECREF( py_field_ptr );

      string setting_name = "";
      if( nr_args > 1 ) {
        PyObject *py_setting_name = PyTuple_GetItem( args, 1 ); // borrowed ref?
        if( !py_setting_name || !PyString_Check( py_setting_name ) ) {
          ostringstream err;
          err << "Invalid argument 1 to function H3D.addProgramSettings. Should be string.";
          PyErr_SetString( PyExc_ValueError, err.str().c_str() );
          return 0;
        }
        setting_name = PyString_AsString( py_setting_name );
      }

      string section_name = "Main settings";
      if( nr_args > 2 ) {
        PyObject *py_section_name = PyTuple_GetItem( args, 2 ); // borrowed ref?
        if( !py_section_name || !PyString_Check( py_section_name ) ) {
          ostringstream err;
          err << "Invalid argument 1 to function H3D.addProgramSettings. Should be string.";
          PyErr_SetString( PyExc_ValueError, err.str().c_str() );
          return 0;
        }
        section_name = PyString_AsString( py_section_name );
      }

      // TODEO
      Scene::addProgramSetting( field, setting_name, section_name );

      Py_INCREF(Py_None);
      return Py_None; 
    }

    PyObject *pythonFindNodes( PyObject *self, PyObject *args ) {
      // args are ( node, type_names, node_name= "", field_names= [], exactNodeName= true, verbose= false )
      // return value is [(found_node,(parent0,parent1,...))]
      unsigned int nr_args= 1;
      Node* node= NULL;
      Scene::StringVec type_names;
      std::string node_name;
      Scene::SearchFieldNameMap search_field_names;
      bool verbose= false;
      bool exact_node_name= true;

      if( PyTuple_Check ( args ) ) {
        nr_args =  PyTuple_Size( args );

        // node (arg 0)
        if ( nr_args > 0 ) {
          node= PyNode_AsNode ( PyTuple_GetItem( args, 0 ) );
          if ( !node ) {
            PyErr_SetString( PyExc_ValueError, 
              "Invalid argument(s) to function findNodes( node, type_names, node_name= "", field_names= [], exactNodeName= true, verbose= false ): Invalid node argument (1)." );
            return NULL;
          }
        }

        // node names (arg 1)
        if ( nr_args > 1 ) {
          // Node type names
          PyObject* py_type_names= PyTuple_GetItem( args, 1 );
          if ( PyString_Check ( py_type_names ) ) {
            // Single type name
            type_names.push_back ( PyString_AsString ( py_type_names ) );
          } else if ( PyList_Check ( py_type_names ) ) {
            // List of type names
            Py_ssize_t size= PyList_Size ( py_type_names );
            for ( Py_ssize_t i= 0; i < size; ++i ) {
              PyObject* py_type_name= PyList_GetItem ( py_type_names, i );
              if ( PyString_Check ( py_type_name ) ) {
                type_names.push_back ( PyString_AsString ( py_type_name ) );
              } else {
                PyErr_SetString( PyExc_ValueError, 
                  "Invalid argument(s) to function findNodes(): Invalid type_names argument (2)." );
                return NULL;
              }
            }
          } else {
            PyErr_SetString( PyExc_ValueError, 
              "Invalid argument(s) to function findNodes(): Invalid type_names argument (2)." );
            return NULL;
          }
        }

        // node name (arg 2)
        if ( nr_args > 2 ) {
          // Node name
          PyObject* py_node_name= PyTuple_GetItem( args, 2 );
          if ( PyString_Check ( py_node_name ) ) {
            node_name= PyString_AsString ( py_node_name );
          } else {
            PyErr_SetString( PyExc_ValueError, 
              "Invalid argument(s) to function findNodes(): Invalid node_name argument (3)." );
            return NULL;
          }
        }

        // field names (arg 3)
        if ( nr_args > 3 ) {
          // List of field names per node type
          PyObject* py_node_list= PyTuple_GetItem( args, 3 );
          if ( PyList_Check ( py_node_list ) ) {
            Py_ssize_t size= PyList_Size ( py_node_list );
            for ( Py_ssize_t i= 0; i < size; ++i ) {
              PyObject* py_field_list= PyList_GetItem ( py_node_list, i );
              if ( PyList_Check ( py_field_list ) ) {
                Py_ssize_t size1= PyList_Size ( py_field_list );
                if ( size1 > 1 ) {
                  std::string type_name;
                  for ( Py_ssize_t j= 0; j < size1; ++j ) {
                    PyObject* py_name= PyList_GetItem ( py_field_list, j );
                    if ( PyString_Check ( py_name ) ) {
                      if ( j == 0 ) {
                        type_name= PyString_AsString ( py_name );
                      } else {
                        search_field_names[type_name].push_back ( PyString_AsString ( py_name ) );
                      }
                    } else {
                      PyErr_SetString( PyExc_ValueError, 
                        "Invalid argument(s) to function findNodes(): Invalid field_names argument (4)." );
                      return NULL;
                    }
                  }
                } else {
                  PyErr_SetString( PyExc_ValueError, 
                    "Invalid argument(s) to function findNodes(): Invalid field_names argument (4)." );
                  return NULL;
                }
              } else {
                PyErr_SetString( PyExc_ValueError, 
                  "Invalid argument(s) to function findNodes(): Invalid field_names argument (4)." );
                return NULL;
              }
            }
          }
        }

        // verbose (arg 4)
        if ( nr_args > 4 ) {
          // Verbose
          PyObject* py_exact_node_name= PyTuple_GetItem( args, 4 );
          if ( PyBool_Check( py_exact_node_name ) || PyInt_Check( py_exact_node_name ) ) {
            exact_node_name= PyObject_IsTrue ( py_exact_node_name ) == 1;
          } else {
            PyErr_SetString( PyExc_ValueError, 
              "Invalid argument(s) to function findNodes(): Invalid exactNodeName argument (5)." );
            return NULL;
          }
        }

        // verbose (arg 5)
        if ( nr_args > 5 ) {
          // Verbose
          PyObject* py_verbose= PyTuple_GetItem( args, 5 );
          if ( PyBool_Check( py_verbose ) || PyInt_Check( py_verbose ) ) {
            verbose= PyObject_IsTrue ( py_verbose ) == 1;
          } else {
            PyErr_SetString( PyExc_ValueError, 
              "Invalid argument(s) to function findNodes(): Invalid verbose argument (6)." );
            return NULL;
          }
        }

       } else {
         PyErr_SetString( PyExc_ValueError, 
           "Invalid argument(s) to function findNodes ()" );
         return NULL;
       } 

       AutoRefVector<Node> result;
       Scene::NodeParentsMap parent_map;
       Scene::findNodes (
         *node, result, node_name,
         &parent_map,
         search_field_names.empty() ? NULL : &search_field_names,
         type_names.empty() ? NULL : &type_names,
         exact_node_name,
         verbose );

       // return value is [(found_node,(parent0,parent1,...))]
       PyObject* py_result= PyList_New(result.size()); // New ref that is returned (no need to decr)
       for ( size_t i= 0; i < result.size(); ++i ) {
         Node* child_node= result[i];
         AutoRefVector<Node>& parents= parent_map[child_node];

         // New ref that is stolen by PyTuple_SetItem() (no need to decr)
         PyObject* py_result_item= PyTuple_New ( parents.empty() ? 1 : 2 );

         PyTuple_SetItem ( py_result_item, 0, PyNode_FromNode ( child_node ) );

         if ( !parents.empty() ) {
           // New ref that is stolen by PyTuple_SetItem() (no need to decr)
           PyObject* py_parent_list= PyTuple_New ( parents.size() );
           for ( size_t j= 0; j < parents.size(); ++j ) {
             PyTuple_SetItem ( py_parent_list, j, PyNode_FromNode ( parents[j] ) );
           }
           PyTuple_SetItem ( py_result_item, 1, py_parent_list );
         }

         PyList_SetItem ( py_result, i, py_result_item );
       }

       return py_result; 
    }

    PyObject *pythonTakeScreenshot( PyObject *self, PyObject *args ) {
      if( !args || !PyString_Check( args ) ) {
        ostringstream err;
        err << "Invalid argument(s) to function H3D.takeScreenshot( url ).";
        err << " url should be of string type.";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return NULL;
      }

      string url( PyString_AsString( args ) );
      string success = H3DWindowNode::takeScreenShot( url );
      if( success != "" ) {
        PyErr_SetString( PyExc_ValueError, success.c_str() );
        return NULL;
      }
      Py_RETURN_TRUE;
    }
  }
}

#endif // HAVE_PYTHON
