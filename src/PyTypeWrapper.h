
//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file PyTypeWrapper.h
/// \brief Header file for PyTypeWrapper. Contains template modifier
/// classes for creating new Python types inheriting from existing types. 
///
//
//////////////////////////////////////////////////////////////////////////////

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

  /// \class PyType
  /// \brief Base class for all Python C type wrappers.
  /// The base class is used in order to ensure that the PyObject_HEAD comes
  /// before the inherited class in each Python class. 
  struct PyType {
    PyObject_HEAD
  };

  /// PyTypeWrapper is a template wrapper class to create new Python types
  /// from types already existing in the H3DAPI (see PythonTypes.h) for 
  /// example usage. 
  /// \param Type The H3DAPI type to make a Python type of.
  /// \param TypeObject A PyTypeObject for that type.
  /// \param CheckFunc A function for checking that a PyObject * is of the
  /// new Python type.
  /// \param ValueFunc A function for extracting the value of the type that
  /// has been wrapped in the python type.
  /// \param NewFunc A function for creating a new instance of the Python
  /// class given an instance of Type.
  template< class Type, 
            PyTypeObject *TypeObject,
            string ( *NameFunc ) (),
            bool ( *CheckFunc )( PyObject * ),
            Type (*ValueFunc)( PyObject *),
            PyObject *(*NewFunc)( const Type &) >
  struct PyTypeWrapper: public PyType, Type {

    /// Install type in the given python module. Must be called if the
    /// type is to be used in a module.
    static void installType( PyObject* H3D_module ) {
      if (PyType_Ready( TypeObject ) < 0 )
        return; // THROW ERROR!?
      
      Py_INCREF( TypeObject );
      PyModule_AddObject( H3D_module, 
                          (char *)NameFunc().c_str(), 
                          (PyObject *)TypeObject );
    }
  
    /// create() a new instance of Type using Python to allocate
    /// the memory and initialise the PyObject headers - to be called
    /// by PythonScript's internals.
    static PyObject* create() {
      return PyType_GenericAlloc( TypeObject, 1 ); 
    } 
    
    /// Python type deallocation
    static void dealloc( PyObject *self ) {
      self->ob_type->tp_free( self );
    }

    /// Converts to a char* string
    static PyObject* repr( PyObject *myself, PyObject *args ) {
      if( CheckFunc( myself ) ) {
        ostringstream s;
        s << ValueFunc( myself );
        return PyString_FromString( s.str().c_str() );
      } else {
        ostringstream s;
        s << "PyObject * is not a " << NameFunc() << "*"; 
        throw Exception::H3DAPIException( s.str(), 
                                          H3D_FULL_LOCATION );
      }
    }


    /// Test if two PyTypes are equal or not.
    static int compare( PyObject *veca, PyObject *vecb ) {
      if( CheckFunc( veca ) && CheckFunc( vecb ) ){
        Type a = ValueFunc( veca );
        Type b = ValueFunc( vecb );
        return a != b;
      } else {
        ostringstream s;
        s << "PyObject * is not a " << NameFunc() << "*"; 
        throw Exception::H3DAPIException( s.str(),
                                          H3D_FULL_LOCATION );
      }
    }
  };


  /// PyNumberTypeWrapperBase is a template wrapper class to create new Python 
  /// types from types already existing in the H3DAPI (see PythonTypes.h) for 
  /// example usage. It is used especially on types that have arithmetical
  /// operator defined since these will be generated.
  ///  
  /// \param Type The H3DAPI type to make a Python type of.
  /// \param TypeObject A PyTypeObject for that type.
  /// \param CheckFunc A function for checking that a PyObject * is of the
  /// new Python type.
  /// \param ValueFunc A function for extracting the value of the type that
  /// has been wrapped in the python type.
  /// \param NewFunc A function for creating a new instance of the Python
  /// class given an instance of Type.
  template< class Type, 
            PyTypeObject *TypeObject,
            string ( *NameFunc ) (),
            bool ( *CheckFunc )( PyObject * ),
            Type (*ValueFunc)( PyObject *),
            PyObject *(*NewFunc)( const Type &) >
  struct PyNumberTypeWrapperBase: 
    public PyTypeWrapper< Type, 
                          TypeObject, 
                          NameFunc,
                          CheckFunc, 
                          ValueFunc, 
                          NewFunc > {

    /// Performs addition between two instances
    static PyObject* add( PyObject *veca, PyObject *vecb ) {
      if( CheckFunc( veca ) && CheckFunc( vecb ) ){
        Type c = ValueFunc( veca ) + ValueFunc( vecb );
        return NewFunc( c );
      }
      return Py_NotImplemented;
    }
    
    /// Performs subtraction between two instances
    static PyObject* sub( PyObject *veca, PyObject *vecb ) {
      if( CheckFunc( veca ) && CheckFunc( vecb ) ){
        Type c = ValueFunc( veca ) - ValueFunc( vecb );
        return NewFunc( c );
      }
      return Py_NotImplemented;
    }
    /// Performs multiplication between a instance of the 
    /// type and a float, long or int.
    static PyObject* mul( PyObject *veca, PyObject *vecb ) {
      if( CheckFunc( veca ) ) {
        Type a = ValueFunc( veca );
        if( PyFloat_Check( vecb ) ) {
          Type c = a * PyFloat_AsDouble( vecb );
          return NewFunc( c );
          // int type
        } else if( PyInt_Check( vecb ) ) {
          Type c = a * PyInt_AsLong( vecb );
          return NewFunc( c );
          // long type
        } else if( PyLong_Check( vecb ) ) {
          Type c = a * PyLong_AsLong( vecb );
          return NewFunc( c );
        }
      } else if( CheckFunc( vecb ) ) {
        Type b = ValueFunc( vecb );
        // float type
        if( PyFloat_Check( veca ) ) {
          Type c = PyFloat_AsDouble( veca ) * b;
          return NewFunc( c );
          // int type
        } else if( PyInt_Check( veca ) ) {
          Type c = PyInt_AsLong( veca ) * b;
          return NewFunc( c );
          // long type
        } else if( PyLong_Check( veca ) ) {
          Type c = PyLong_AsLong( veca ) * b;
          return NewFunc( c );
        } 
      }
      return Py_NotImplemented;
    }

    /// Performs division with a float, long or int instance
    static PyObject* div( PyObject *veca, PyObject *floatb ) {
      if( CheckFunc( veca ) ) {
        Type a = ValueFunc( veca );
        // float type
        if( PyFloat_Check( floatb ) ) {
          Type c = a / PyFloat_AsDouble( floatb );
          return NewFunc( c );
          // int type
        } else if( PyInt_Check( floatb ) ) {
          Type c = a / PyInt_AsLong( floatb );
          return NewFunc( c );
          // long type
        } else if( PyLong_Check( floatb ) ) {
          Type c = a / PyLong_AsLong( floatb );
          return NewFunc( c );
        }
      }
      return Py_NotImplemented;
    }

    /// Negation.
    static PyObject* neg( PyObject *vec ) {
      if( CheckFunc( vec ) ){
        Type c = -ValueFunc( vec );
        return NewFunc( c );
      }
      return Py_NotImplemented;
    }

  };

  /// PyVecTypeWrapper is the same as PyNumberTypeWrapperBase with the 
  /// difference that its mul function also specifies vector dot product
  /// between two instances.
  ///  
  /// \param Type The H3DAPI type to make a Python type of.
  /// \param TypeObject A PyTypeObject for that type.
  /// \param CheckFunc A function for checking that a PyObject * is of the
  /// new Python type.
  /// \param ValueFunc A function for extracting the value of the type that
  /// has been wrapped in the python type.
  /// \param NewFunc A function for creating a new instance of the Python
  /// class given an instance of Type.
  template< class Type, 
            PyTypeObject *TypeObject,
            string ( *NameFunc ) (),
            bool ( *CheckFunc )( PyObject * ),
            Type (*ValueFunc)( PyObject *),
            PyObject *(*NewFunc)( const Type &) >
  struct PyVecTypeWrapper: 
    public PyNumberTypeWrapperBase< Type, 
                                    TypeObject, 
                                    NameFunc,
                                    CheckFunc, 
                                    ValueFunc, 
                                    NewFunc > {
    /// Performs vector dot product of two vector instances
    static PyObject* mul( PyObject *veca, PyObject *vecb ) {
      if( CheckFunc( veca ) && CheckFunc( vecb )  ) {
        Type a = ValueFunc( veca );
        Type b = ValueFunc( vecb );
        return PyFloat_FromDouble( a * b );
      } else {
        return PyNumberTypeWrapperBase< Type, 
          TypeObject, 
          NameFunc,
          CheckFunc, 
          ValueFunc, 
          NewFunc >::mul( veca, vecb );
      }
    }
  };

  /// PyNumberTypeWrapper is the same as PyNumberTypeWrapperBase with 
  /// the difference that its mul function also specifies multiplication
  /// between two instances of the class.
  ///  
  /// \param Type The H3DAPI type to make a Python type of.
  /// \param TypeObject A PyTypeObject for that type.
  /// \param CheckFunc A function for checking that a PyObject * is of the
  /// new Python type.
  /// \param ValueFunc A function for extracting the value of the type that
  /// has been wrapped in the python type.
  /// \param NewFunc A function for creating a new instance of the Python
  /// class given an instance of Type.
  template< class Type, 
            PyTypeObject *TypeObject,
            string ( *NameFunc ) (),
            bool ( *CheckFunc )( PyObject * ),
            Type (*ValueFunc)( PyObject *),
            PyObject *(*NewFunc)( const Type &) >
  struct PyNumberTypeWrapper: 
    public PyNumberTypeWrapperBase< Type, 
                                    TypeObject, 
                                    NameFunc,
                                    CheckFunc, 
                                    ValueFunc, 
                                    NewFunc > {
    /// Performs multiplacation.
    static PyObject* mul( PyObject *veca, PyObject *vecb ) {
      if( CheckFunc( veca ) && CheckFunc( vecb )  ) {
        Type a = ValueFunc( veca );
        Type b = ValueFunc( vecb );
        return NewFunc( a * b );
      } else {
        return PyNumberTypeWrapperBase< Type, 
          TypeObject, 
          NameFunc,
          CheckFunc, 
          ValueFunc, 
          NewFunc >::mul( veca, vecb );
      }
    }
  };

}

#endif // HAVE_PYTHON
