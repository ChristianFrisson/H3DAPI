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
/// \file PythonScript.cpp
/// \brief cpp file for PythonScript
///
//
//////////////////////////////////////////////////////////////////////////////

#include "PythonScript.h"
#include "X3DTypes.h"
#include "X3DFieldConversion.h"
#include "PythonTypes.h"
#include "Scene.h"
#include "X3D.h"
#include "X3DSAX2Handlers.h"
#include "MFNode.h"
#include "PythonMethods.h"

#ifdef HAVE_PYTHON

#include <string>
#include <algorithm>
#include <cctype>
#define DONT_HAVE_SYS_STAT_H
#undef HAVE_STAT_H

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
#include <Python/pythonrun.h>
#include <Python/ceval.h>
#include <Python/moduleobject.h>
#include <Python/structmember.h>
#else
#include <Python.h>
#include <pythonrun.h>
#include <ceval.h>
#include <moduleobject.h>
#include <structmember.h>
#endif

#if defined(_MSC_VER)
// redefine _DEBUG if it was undefed
#ifdef _DEBUG_UNDEFED
#define _DEBUG
#endif
#endif

using namespace H3D;
using namespace X3D;


// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PythonScript::database( 
                                       "PythonScript", 
                                       &(newInstance<PythonScript>), 
                                       typeid( PythonScript ),
                                       &H3DScriptNode::database );

namespace PythonScriptInternals {
  FIELDDB_ELEMENT( PythonScript, references, INITIALIZE_ONLY );
}


Field *PythonScript::lookupField( const string &name ) {
	if( module_dict ) {
    PyObject *fname = 
      PyDict_GetItemString( static_cast< PyObject * >( module_dict ), 
                            name.c_str() );
    if ( fname ) {
      // it was a variable in the python script, so extract the C++ type
      // pointer and return it
      PyObject *fieldptr = PyObject_GetAttrString( fname, "__fieldptr__" );
      if ( fieldptr && PyCObject_Check( fieldptr ) )
        return static_cast< Field* >( PyCObject_AsVoidPtr( fieldptr ) );
    } 
	}
  return NULL;
}


PythonScript::PythonScript( Inst< MFString > _url,
                            Inst< MFNode    > _references ) : 
  H3DScriptNode( _url ),
  references( _references ),
  module( NULL ),
  module_dict( NULL ) {
  type_name = "PythonScript";
  database.initFields( this );

  // Py_Initialize really should be done in the DLL loader function:
  if ( !Py_IsInitialized() ) {
    Py_Initialize();  
  }
  initialiseParser();
}

PythonScript::~PythonScript() {
  if( module )
    Py_DECREF( (PyObject *)module );
}

void PythonScript::initialiseParser() {
  H3DScriptNode::initialiseParser();
  PythonInternals::initH3D();
}

void PythonScript::loadScript( const string &script ) {
  PyObject *ref = (PyObject*)PythonInternals::fieldAsPythonObject( references.get() );
  PyDict_SetItem( (PyObject *)module_dict, 
                  PyString_FromString( "references" ), 
                  ref );

  if (PyDict_GetItemString( static_cast< PyObject * >(module_dict), "__builtins__") == NULL) {
    if (PyDict_SetItemString( static_cast< PyObject * >(module_dict), "__builtins__",
                             PyEval_GetBuiltins()) != 0)
    cerr << "Warning: PyEval_GetBuiltins() could not be installed in module dictionary!" << endl;
  }  
  
  FILE *f = fopen( script.c_str(), "r" );
  if ( f ) {
    PyErr_Clear();
    PyObject *r = PyRun_File( f, script.c_str(), Py_file_input,
                              static_cast< PyObject * >(module_dict), 
                              static_cast< PyObject * >(module_dict) );
    if ( r == NULL )
      PyErr_Print();
    
  } else {
    cerr << "Could not open \""<< script << endl;
  }
}


/// Traverse the scenegraph. Used in PythonScript to call a function
/// in python once per scene graph loop.
void PythonScript::traverseSG( TraverseInfo &ti ) {
  PyObject *func = 
    PyDict_GetItemString( static_cast< PyObject * >( module_dict ), 
                          "traverseSG" );
  if( func && PyFunction_Check( func ) ) {
    PyObject *args = PyTuple_New(0);
    PyObject *result = PyEval_CallObject( func, args );
    if ( result == NULL )
      PyErr_Print();
    else 
      Py_DECREF( result );

    Py_DECREF( args );
  } 
}

void PythonScript::initialize() {
  H3DScriptNode::initialize();
  module = PyImport_AddModule( (char*)name.c_str() );
  module_dict = PyModule_GetDict( static_cast< PyObject * >( module ) );
  bool script_loaded = false;
  for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) {
    string url = resolveURLAsFile( *i );
    if( url != "" ) {
      loadScript( url );
      script_loaded = true;
      break;
    }
  }

  if( script_loaded ) {
    PyObject *func = 
      PyDict_GetItemString( static_cast< PyObject * >( module_dict ), "initialize" );
    if( func && PyFunction_Check( func ) ) {
      PyObject *args = PyTuple_New(0);
      PyObject *result = PyEval_CallObject( func, args );
      if ( result == NULL )
        PyErr_Print();
      else 
        Py_DECREF( result );
      
      Py_DECREF( args );
    }   
  } else {
    cerr << "Warning: None of the urls in the PythonScript node \"" 
         << getName() << "\" with url [";
    for( MFString::const_iterator i = url->begin(); 
         i != url->end(); ++i ) {  
      cerr << " \"" << *i << "\"";
    }
    cerr << "] could be found. " << endl;
  }
}

#endif // HAVE_PYTHON
