////////////////////////////////////////////////////////////////////////////////
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
/// \file PythonScript.cpp
/// \brief cpp file for PythonScript
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PythonScript.h>
#include <H3D/X3DTypes.h>
#include <H3D/X3DFieldConversion.h>
#include <H3D/PythonTypes.h>
#include <H3D/Scene.h>
#include <H3D/X3D.h>
#include <H3D/X3DSAX2Handlers.h>
#include <H3D/MFNode.h>
#include <H3D/PythonMethods.h>
#include <H3D/ResourceResolver.h>

#ifdef HAVE_PYTHON

#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
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


int PythonScript::argc;
char** PythonScript::argv;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PythonScript::database( 
                                       "PythonScript", 
                                       &(newInstance<PythonScript>), 
                                       typeid( PythonScript ),
                                       &H3DScriptNode::database );

namespace PythonScriptInternals {
  FIELDDB_ELEMENT( PythonScript, references, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( PythonScript, moduleName, INITIALIZE_ONLY );
}

void PythonScript::setargv( int _argc, char *_argv[] ) {
  argc = _argc;
  argv = _argv;
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
      if ( fieldptr && PyCObject_Check( fieldptr ) ) {
        Field *f = static_cast< Field* >( PyCObject_AsVoidPtr( fieldptr ) );
        Py_DECREF( fieldptr );
        return f;
      }
    } 
  }
  return NULL;
}


PythonScript::PythonScript( Inst< MFString > _url,
                            Inst< MFNode    > _references,
                            Inst< SFString > _moduleName ) : 
  H3DScriptNode( _url ),
  references( _references ),
  moduleName( _moduleName ),
  module( NULL ),
  module_dict( NULL ) {
  type_name = "PythonScript";
  database.initFields( this );

  addInlinePrefix( "python" );

  // Py_Initialize really should be done in the DLL loader function:
  if ( !Py_IsInitialized() ) {
    Py_Initialize();  
    if( argv )
      PySys_SetArgv(argc,argv);
  }
  initialiseParser();
}

PythonScript::~PythonScript() {
  if( module_dict ) {
    // Clearing the dictionary
    PyDict_Clear( static_cast< PyObject * >(module_dict) );
    
    // Removing the PythonScript module module_name from database.
    // If it is already removed then there are two PythonScripts in the
    // scene using the same module_name ( or DEF ).
    PyObject *temp_sys_module_dict = PyImport_GetModuleDict();
    if( PyDict_DelItemString( temp_sys_module_dict,
                              (char*)module_name.c_str() ) == -1 ) {
      Console(4) << "Could not remove the python module " << module_name
                 << " from the sys.modules database. " << endl;
    }
  }
}

void PythonScript::initialiseParser() {
  H3DScriptNode::initialiseParser();
  PythonInternals::initH3D();
}

void PythonScript::loadScript( const string &script ) {
  PyObject *ref = (PyObject*)PythonInternals::fieldAsPythonObject( references.get(), false );
  PyDict_SetItem( (PyObject *)module_dict, 
                  PyString_FromString( "references" ), 
                  ref );
  Py_DECREF( ref );

  if (PyDict_GetItemString( static_cast< PyObject * >(module_dict), "__builtins__") == NULL) {
    if (PyDict_SetItemString( static_cast< PyObject * >(module_dict), "__builtins__",
                             PyEval_GetBuiltins()) != 0)
      Console(3) << "Warning: PyEval_GetBuiltins() could not be installed in module dictionary!" << endl;
  }  
  
#ifdef WIN32
  // have to read the script into a buffer instead of using FILE *
  // since it is unsafe to use FILE * sent over DLL boundaries.
  ifstream is( script.c_str() );
  if( is.good() ) {
    int length;
    char * buffer;
    
    // get length of file:
    is.seekg (0, ios::end);
    length = is.tellg();
    is.seekg (0, ios::beg);
    
    // allocate memory:
    buffer = new char [length + 1];
    // read data as a block:
    is.read (buffer,length);
    length = is.gcount();
    is.close();
    buffer[length ] = '\0';
    
    PyErr_Clear();
    PyObject *r = PyRun_String( buffer, Py_file_input,
                                static_cast< PyObject * >(module_dict), 
                                static_cast< PyObject * >(module_dict) );
    if ( r == NULL ) {
      Console( 3 ) << "Python error in file \"" << script << "\":" << endl;
      PyErr_Print();
    }
    delete[] buffer;
  }
#else
  FILE *f = fopen( script.c_str(), "r" );
  if ( f ) {
    PyErr_Clear();
    PyObject *r = PyRun_FileEx( f, script.c_str(), Py_file_input,
                                static_cast< PyObject * >(module_dict), 
                                static_cast< PyObject * >(module_dict),
                                true );
    if ( r == NULL )
      PyErr_Print();

    
  }
  
#endif

  else {
    Console(4) << "Could not open \""<< script << endl;
  }
}


// Traverse the scenegraph. Used in PythonScript to call a function
// in python once per scene graph loop.
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

  module_name = moduleName->getValue();

  // if no module name set by user, use the instance name which is
  // a unique identifier for each PythonScript instance.
  if( module_name == "" )
    module_name = getInstanceName();

  PyObject *temp_sys_module_dict = PyImport_GetModuleDict(); // borrowed ref
  if( PyDict_GetItemString( temp_sys_module_dict,
                            (char*)module_name.c_str() ) ) { // borrowed ref
    Console(4) << "The module " << module_name << " already exists. "
               << "It will be overridden which might cause strange behaviour. "
               << "Check the moduleName field of PythonScript "
               << "in the scene if this behaviour is undesired." << endl;
  }


  module = PyImport_AddModule( (char*)module_name.c_str() ); // borrowed ref
  module_dict = 
    PyModule_GetDict( static_cast< PyObject * >( module ) ); // borrowed ref


  bool script_loaded = false;
  for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) {
    bool is_tmp_file;
    string url = resolveURLAsFile( *i, &is_tmp_file );
    if( url != "" ) {
      loadScript( url );
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
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
    Console(4) << "Warning: None of the urls in the PythonScript node \"" 
         << getName() << "\" with url [";
    for( MFString::const_iterator i = url->begin(); 
         i != url->end(); ++i ) {  
      Console(4) << " \"" << *i << "\"";
    }
    Console(4) << "] could be found. " << endl;
  }
}

#endif // HAVE_PYTHON
