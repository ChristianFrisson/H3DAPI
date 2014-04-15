//////////////////////////////////////////////////////////////////////////////
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
/// \file PythonMethods.h
/// \brief Header file for methods used in the H3D module of the 
/// PythonScript node.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PYTHONMETHODS_H__
#define __PYTHONMETHODS_H__
#include <H3D/Field.h>
#include <H3D/X3DTypes.h>
#include <H3D/X3DFieldConversion.h>
#include <H3D/X3DTypeFunctions.h>
#include <H3D/PythonScript.h>


#ifdef HAVE_PYTHON
#if defined(_MSC_VER)
// undefine _DEBUG since we want to always link to the release version of
// python and pyconfig.h automatically links debug version if _DEBUG is
// defined.
#ifdef _DEBUG
#define _DEBUG_UNDEFED
#undef _DEBUG
#endif
#endif
#if defined(__APPLE__) && defined(__MACH__)
#include <Python/Python.h>
#else
#include <Python.h>
#endif
#if defined(_MSC_VER)
// redefine _DEBUG if it was undefed
#ifdef _DEBUG_UNDEFED
#define _DEBUG
#endif
#endif

namespace H3D {
  namespace PythonInternals {
    PyMODINIT_FUNC initH3D();
    
    double pyObjectToDouble( PyObject *v );

    void fieldDestructor( void *f );

    PyObject *fieldAsPythonObject( Field * f, bool destruct=false );

    PyObject *pythonCreateField( PyObject *self, PyObject *args );
    
    PyObject *pythonSetFieldValueFromObject( Field *field_ptr, PyObject *v );

    PyObject *pythonFieldSetValue( PyObject *self, PyObject *args );

    PyObject *pythonFieldGetValue( PyObject *self, PyObject *arg );

    PyObject *pythonFieldSetAccessType( PyObject *self, PyObject *arg );

    PyObject *pythonFieldGetAccessType( PyObject *self, PyObject *arg );

    PyObject *pythonFieldSetAccessCheck( PyObject *self, PyObject *arg );

    PyObject *pythonFieldIsAccessCheckOn( PyObject *self, PyObject *arg );

    PyObject *pythonFieldRoute( PyObject *self, PyObject *args );

    PyObject *pythonFieldRouteNoEvent( PyObject *self, PyObject *args );

    PyObject *pythonFieldUnroute( PyObject *self, PyObject *args );

    PyObject *pythonFieldReplaceRoute( PyObject *self, PyObject *args );
    
    PyObject *pythonFieldReplaceRouteNoEvent( PyObject *self, PyObject *args );
    
    PyObject *pythonFieldUnrouteAll( PyObject *self, PyObject *arg );

    PyObject *pythonGetCPtr( PyObject *self, PyObject *arg  );

    PyObject* pythonCreateX3DFromURL( PyObject *self, PyObject *arg );

    PyObject* pythonCreateX3DFromString( PyObject *self, PyObject *arg );

    PyObject* pythonCreateX3DNodeFromURL( PyObject *self, PyObject *arg );

    PyObject* pythonCreateX3DNodeFromString( PyObject *self, PyObject *arg );

    PyObject* pythonCreateVRMLFromURL( PyObject *self, PyObject *arg );

    PyObject* pythonCreateVRMLFromString( PyObject *self, PyObject *arg );

    PyObject* pythonWriteNodeAsX3D( PyObject *self, PyObject *arg );

    PyObject* pythonCreateVRMLNodeFromURL( PyObject *self, PyObject *arg );

    PyObject* pythonCreateVRMLNodeFromString( PyObject *self, PyObject *arg );

    PyObject* pythonFieldRoutesTo( PyObject *self, PyObject *arg );

    PyObject* pythonFieldHasRouteFrom( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetRoutesIn( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetRoutesOut( PyObject *self, PyObject *arg );

    PyObject* pythonGetCurrentScenes( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveDeviceInfo( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveBindableNode( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveViewpoint( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveNavigationInfo( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveStereoInfo( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveBackground( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveFog( PyObject *self, PyObject *arg );

    PyObject* pythonGetActiveGlobalSettings( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldErase( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldPushBack( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldClear( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldBack( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldFront( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldEmpty( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldPopBack( PyObject *self, PyObject *arg );

    PyObject* pythonMFieldSize( PyObject *self, PyObject *arg );

    PyObject* pythonFieldTouch( PyObject *self, PyObject *arg );

    PyObject* pythonResolveURLAsFile( PyObject *self, PyObject *arg );

    PyObject* pythonResolveURLAsFolder( PyObject *self, PyObject *arg );

    PyObject* throwQuitAPIException( PyObject *self, PyObject *arg );

    PyObject* pythonCreateNode( PyObject *self, PyObject *arg );

    PyObject* pythonGetHapticsDevice( PyObject *self, PyObject *arg );

    PyObject* pythonGetNrHapticsDevices( PyObject *self, PyObject *arg );

    PyObject* pythonGetNamedNode( PyObject *self, PyObject *arg );

    PyObject* pythonFieldSetName( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetName( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetFullName( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetTypeName( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetOwner( PyObject *self, PyObject *arg );
    
    PyObject* pythonFieldSetOwner( PyObject *self, PyObject *arg );

    PyObject* pythonFieldGetValueAsString( PyObject *self, PyObject *arg );

    PyObject* pythonFieldSetValueFromString( PyObject *self, PyObject *arg );

    PyObject* pythonFieldUpToDate( PyObject *self, PyObject *arg );

    PyObject* pythonFieldIsUpToDate( PyObject *self, PyObject *arg );

    /// addProgramSettings( field, setting_name = "", section = "" )
    PyObject* pythonAddProgramSetting( PyObject *self, PyObject *arg );
/*
    PyObject* pythonRemoveProgramSetting( PyObject *self, PyObject *arg );

    PyObject* pythonClearProgramSettings( PyObject *self, PyObject *arg );

    PyObject* pythonGetProgramSetting( PyObject *self, PyObject *arg );

    PyObject* pythonGetProgramSettings( PyObject *self, PyObject *arg );
*/

    PyObject* pythonFindNodes( PyObject *self, PyObject *arg );

    PyObject* pythonTakeScreenshot( PyObject *self, PyObject *arg );
  }

    
  /// 
  /// An exception thrown when a field is of the wrong type
  /// when it is checked. Internal to the H3D::TypedFieldCheck classes.
  /// 
  H3D_VALUE_EXCEPTION( string, PythonInvalidFieldType );

  struct PythonFieldBase {
    PythonFieldBase( void *_python_field ) :
      python_field( _python_field ),
      have_update( false ),
      have_type_info( false ),
      have_opt_type_info( false ) {}

    void *python_field;
    // Flags used to know if the "update", "__type_info_", "__opt_type_info__"
    // attributes of the python fields exists and should be used.
    // Note that the these attributes can not be added to the field after
    // pythonCreateField is called.
    // The reason for not simply storing a pointer to a c-version of those
    // attributes is because PyObject_GetAttrString increases the reference
    // count to the field and therefore we would have circular dependency
    // for destructing. Which means that reloading of new PythonScripts would
    // not work properly in browsers, for example H3DViewer.
    bool have_update;
    bool have_type_info;
    bool have_opt_type_info;
  };

  template< class F >
  struct PythonField : public F, PythonFieldBase {

    PythonField( void *_python_field ) : 
      PythonFieldBase( _python_field ) {
    }

    virtual void update() {
      // ensure we have the GIL lock to work with multiple python threads.
      PyGILState_STATE state = PyGILState_Ensure();
      if( have_update ) {
        PyObject *python_update = PyObject_GetAttrString(
          static_cast< PyObject * >(python_field), "update" );
        PyErr_Clear();
        if( python_update ) {
          PyObject *args = PyTuple_New(1);
          PyObject *f = PythonInternals::fieldAsPythonObject( this->event.ptr, false );
          PyTuple_SetItem( args, 0, f );
          PyObject *r = PyEval_CallObject( python_update,
                                           args );
          ////const char *value = PyString_AsString( PyObject_Repr( r ) );
          Py_DECREF( args );
          if( r == Py_None ) {
            Console(3) << "Warning: update()-function for Python defined field of type " 
                       << this->getFullName() << " does not return a value. "<< endl;
            Py_DECREF( r );
          } else if ( r ) {
            if( !PythonInternals::pythonSetFieldValueFromObject( this, r ) ) {
              Console(3) << "Warning: invalid return value from update()-function"
                         << " for Python defined field of type " 
                         << this->getFullName() << endl;
            }
            Py_DECREF( r );
          } else {
            PyErr_Print();
          }
          Py_DECREF( python_update );
        } else {
          F::update();
        }
      } else {
        F::update();
      }
      PyGILState_Release(state);
    }
    
    /// Function for checking that a field is of a correct type 
    /// given type specification according to the arguments specified
    /// during the construction of the Python field class.
    /// \param f The field to check the type of
    /// \param index The index of the field in the template
    /// type list.
    /// \throws PythonInvalidFieldType
    void checkFieldType( Field *f, int index ) {
      if( have_type_info == 0 && have_opt_type_info == 0 ) {
        F::checkFieldType( f, index );
        return;
      }

      // ensure we have the GIL lock to work with multiple python threads.
      PyGILState_STATE state = PyGILState_Ensure();

      PyObject *python_typeinfo = PyObject_GetAttrString(
        static_cast< PyObject * >(python_field), "__type_info__" );
      PyErr_Clear();

      int arg_size = -1;
      if( python_typeinfo )
        arg_size = (int)PyTuple_Size( static_cast< PyObject * >(python_typeinfo) );

      if ( index >= arg_size ) {
        PyObject *python_opttypeinfo = PyObject_GetAttrString(
          static_cast< PyObject * >(python_field), "__opt_type_info__" );
        PyErr_Clear();
        int opt_arg_size = -1;
        if( python_opttypeinfo )
          opt_arg_size = (int) PyTuple_Size( static_cast< PyObject * >(python_opttypeinfo) );
        if( opt_arg_size > 0 ) {
          PyObject *type_info = 
            PyTuple_GetItem( static_cast< PyObject * >(python_opttypeinfo),
                             0 );
          PyObject *type_id = PyObject_GetAttrString( type_info, "type" );
          int type_int = PyInt_AsLong( type_id );
          if ( f->getTypeName().compare( 
           X3DTypes::typeToString( (X3DTypes::X3DType)type_int ) ) != 0 ) {
            ostringstream err;
            err << "Bad input, expected " 
                << X3DTypes::typeToString( (X3DTypes::X3DType)type_int )  
                << " got " << f->getTypeName() << " for route" << index;
            PyGILState_Release(state);
            throw H3D::PythonInvalidFieldType( err.str(), "", 
                                               H3D_FULL_LOCATION );
          }
        } else {
          ostringstream err;
          err << "Too many inputs, expected " << arg_size+1;
          PyGILState_Release(state);
          throw H3D::PythonInvalidFieldType( err.str(), "", 
                                             H3D_FULL_LOCATION );
        }
      } else if( python_typeinfo ) {
        PyObject *type_info = 
          PyTuple_GetItem( static_cast< PyObject * >(python_typeinfo),
                           index );
        PyObject *type_id = PyObject_GetAttrString( type_info, "type" );
        int type_int = PyInt_AsLong( type_id );
        //if( ! t || ! PyInt_Check( t ) )

        // check that field type is of correct type. The comparison
        // with UNKNOWN_X3D_TYPE is to allow Python fields to accept
        // Field as input, thus allowing any field type to be routed
        // to it.
        if ( f->getTypeName().compare( 
             X3DTypes::typeToString( (X3DTypes::X3DType)type_int ) ) != 0 &&
             strcmp( X3DTypes::typeToString( (X3DTypes::X3DType)type_int ),
                     "UNKNOWN_X3D_TYPE" ) != 0 ) {
          ostringstream err;
          err << "Bad input, expected " 
              << X3DTypes::typeToString( (X3DTypes::X3DType)type_int )  
              << " got " << f->getTypeName() << " for route" << index;
          throw H3D::PythonInvalidFieldType( err.str(), "", 
                                             H3D_FULL_LOCATION );
        }
      }
      PyGILState_Release(state);
    }
  };
}

#endif // HAVE_PYTHON
#endif
