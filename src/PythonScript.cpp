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
#include <H3D/GlobalSettings.h>

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

  // The saved thread state when disallowMainThreadPython is called.
  // Will be restored upon allowMainThreadPyton call.
  PyThreadState *main_python_thread_state = NULL;

  /// The normal PyRun_String method does not allow a filename to
  /// be passed along for better error messages and trace backs.
  /// This function does the same as PyRun_String but 
  /// also takes the filename where the string originated from.
  inline PyObject *PyRun_StringFilename( const char *str, 
                                         const char *filename,
                                         int start, 
                                         PyObject *globals, 
                                         PyObject *locals) {
    PyCodeObject *co = (PyCodeObject *)Py_CompileString(str, filename, start);
    if (co == NULL)
      return NULL;
    PyObject *v = PyEval_EvalCode(co, globals, locals);
    Py_DECREF(co);
    return v;
  }
}

void PythonScript::setargv( int _argc, char *_argv[] ) {
  argc = _argc;
  argv = _argv;
}

void PythonScript::allowMainThreadPython() {
  using namespace PythonScriptInternals;
  if( main_python_thread_state ) {
    PyEval_RestoreThread( main_python_thread_state );
    main_python_thread_state = NULL;
  }
}

bool PythonScript::mainThreadPythonAllowed() {
  return PythonScriptInternals::main_python_thread_state == NULL;
}

void PythonScript::disallowMainThreadPython() {
  using namespace PythonScriptInternals;
  if( !main_python_thread_state ) {
    main_python_thread_state = PyEval_SaveThread();
  }
}

PyObject *PythonScript::getPythonAttribute( const string &name ) {
  if( module_dict ) {
    // ensure we have the GIL lock to work with multiple python threads.
    PyGILState_STATE state = PyGILState_Ensure();
    PyObject *fname = 
      PyDict_GetItemString( static_cast< PyObject * >( module_dict ), 
                            name.c_str() );
    PyGILState_Release(state);
    return fname;
  }
  return NULL;
}

Field *PythonScript::lookupField( const string &name ) const {
  if( module_dict ) {
    // ensure we have the GIL lock to work with multiple python threads.
    PyGILState_STATE state = PyGILState_Ensure();
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
        PyGILState_Release(state);
        return f;
      }
    } 
    PyGILState_Release(state);
  }
  return NULL;
}


void PythonScript::getTopLevelFields( vector< pair< string, Field *> > &fields ) {
  if( module_dict ) {
    // ensure we have the GIL lock to work with multiple python threads.
    PyGILState_STATE state = PyGILState_Ensure();

    PyObject *key, *value;
    Py_ssize_t pos = 0;
    
    // add all fields from the dictionary.
    while (PyDict_Next(static_cast< PyObject * >( module_dict ), &pos, &key, &value)) {
      // key and value are borrowed references

      // new reference
      PyObject *fieldptr = PyObject_GetAttrString( value, "__fieldptr__" );

      if ( fieldptr ) {
        if( PyCObject_Check( fieldptr ) && PyString_Check( key ) ) {
          Field *f = static_cast< Field* >( PyCObject_AsVoidPtr( fieldptr ) );
          string name = PyString_AsString( key );
          fields.push_back( make_pair( name, f ) );
        }
        Py_DECREF( fieldptr );
      }
    }

    PyGILState_Release(state);
  }
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
    
    // Decide if we should initialize multi-threaded C API usage
    bool multi_threaded= false;
    if ( GlobalSettings* gs= GlobalSettings::getActive() ) {
      multi_threaded= gs->multiThreadedPython->getValue();
    }
    if ( multi_threaded ) {
      PyEval_InitThreads();
    }
    
    if( argv )
      PySys_SetArgv(argc,argv);
    disallowMainThreadPython();
  }

  // ensure we have the GIL lock to work with multiple python threads.
  PyGILState_STATE state = PyGILState_Ensure();
  initialiseParser();
  PyGILState_Release(state);
}

// Find python modules in dictionary and store them and the python reference counter
// in module_names.
void findModulesInDict( void * _dict, list< pair< string, Py_ssize_t > > &module_names ) {
  PyObject *key, *value;
  Py_ssize_t pos = 0;
  while( PyDict_Next( static_cast< PyObject * >( _dict ),
                      &pos, &key, &value ) ) {

    if( PyString_Check( key ) && PyModule_Check( value ) ) {
      // The name is a module. add it to imported_module_names.
      string mod_name = PyString_AsString( key );
      list< pair< string, Py_ssize_t > >::iterator i = module_names.begin();
      for( ; i != module_names.end(); ++i )
        if( (*i).first == mod_name )
          break;
      if( i == module_names.end() && value->ob_refcnt > 1 ) {
        pair< string, Py_ssize_t > data;
        data.first = mod_name;
        data.second = value->ob_refcnt;
        module_names.push_back( data );
      }
    }
  }
}

PythonScript::~PythonScript() {
 // ensure we have the GIL lock to work with multiple python threads.
  PyGILState_STATE state = PyGILState_Ensure();

  if( module_dict ) {

    PyObject *temp_sys_module_dict = PyImport_GetModuleDict();
    // Find all modules with a reference count above 1. Use the
    // imported_module_names vector later to remove python modules included
    // by this PythonScript.
    list< pair< string, Py_ssize_t > > imported_module_names;
    findModulesInDict( temp_sys_module_dict, imported_module_names );

    // Setting module_dict to null just to be on the safe side. It should not
    // really be needed.
    module_dict = NULL;

    // Removing the PythonScript module module_name from database.
    // If it is already removed then there are two PythonScripts in the
    // scene using the same module_name ( or DEF ).
    if( PyDict_DelItemString( temp_sys_module_dict,
                              (char*)module_name.c_str() ) == -1 ) {
      Console(4) << "Could not remove the python module " << module_name
                 << " from the sys.modules database. " << endl;
    }

    // Go through imported_module_names. Those modules whose reference count
    // has decreased to 1 only belonged to this PythonScript and should be
    // removed from the global module list. Note that this
    // code will not take care of circular imports. That is, if module A
    // imports module B while module B imports module A.
    list< pair< string, Py_ssize_t > >::iterator i = imported_module_names.begin();
    while( i != imported_module_names.end() ) {
      PyObject *module_to_check = PyDict_GetItemString( temp_sys_module_dict,
                                                        (char*)(*i).first.c_str() );
      if( module_to_check && PyModule_Check( module_to_check ) ) {
        if( module_to_check->ob_refcnt < (*i).second &&
            module_to_check->ob_refcnt == 1 ) {
          // Remove module.
          PyDict_DelItemString( temp_sys_module_dict,
                                (*i).first.c_str() );
          imported_module_names.erase( i );
          // There might still be modules left, restart the loop since the
          // removal of this module might have lowered the reference count for
          // other module.
          if( imported_module_names.empty() )
            break;
          i = imported_module_names.begin();
        } else
          ++i;
      } else {
        // This module does not exist anymore, simply remove it from the list.
        list< pair< string, Py_ssize_t > >::iterator to_erase = i;
        ++i;
        imported_module_names.erase( to_erase );
      }
    }
  }

  PyGILState_Release(state);
}

void PythonScript::initialiseParser() {
  H3DScriptNode::initialiseParser();
  PythonInternals::initH3D();
}

void PythonScript::loadScript( const string &script_filename, const string &script_content ) {
  // ensure we have the GIL lock to work with multiple python threads.
  PyGILState_STATE state = PyGILState_Ensure();
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
  
  if ( script_content != "" ) {
    PyErr_Clear();
    PyObject *r = 
      PythonScriptInternals::PyRun_StringFilename( script_content.c_str(), script_filename.c_str(),
                                                   Py_file_input,
                                                   static_cast< PyObject * >(module_dict), 
                                                   static_cast< PyObject * >(module_dict) );

    if ( r == NULL ) {
      Console( 3 ) << "Python error in file \"" << script_filename << "\":" << endl;
      PyErr_Print();
    }
  } else {
#ifdef WIN32
    // have to read the script into a buffer instead of using FILE *
    // since it is unsafe to use FILE * sent over DLL boundaries.
    ifstream is( script_filename.c_str() );
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
      PyObject *r = 
        PythonScriptInternals::PyRun_StringFilename( buffer, script_filename.c_str(),
                                               Py_file_input,
                                               static_cast< PyObject * >(module_dict), 
                                               static_cast< PyObject * >(module_dict) );

      if ( r == NULL ) {
        Console( 3 ) << "Python error in file \"" << script_filename << "\":" << endl;
        PyErr_Print();
      }
      delete[] buffer;
    }
#else
    FILE *f = fopen( script_filename.c_str(), "r" );
    if ( f ) {
      PyErr_Clear();
      PyObject *r = PyRun_FileEx( f, script_filename.c_str(), Py_file_input,
                                  static_cast< PyObject * >(module_dict), 
                                  static_cast< PyObject * >(module_dict),
                                  true );
      if ( r == NULL )
        PyErr_Print();

    
    }
  
#endif
    else {
      Console(4) << "Could not open \""<< script_filename << endl;
    }
  }

  PyGILState_Release(state);
}


// Traverse the scenegraph. Used in PythonScript to call a function
// in python once per scene graph loop.
void PythonScript::traverseSG( TraverseInfo &ti ) {
  // ensure we have the GIL lock to work with multiple python threads.
  PyGILState_STATE state = PyGILState_Ensure();

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

  PyGILState_Release(state);
}

void PythonScript::initialize() {

  H3DScriptNode::initialize();

  module_name = moduleName->getValue();

  // if no module name set by user, use the instance name which is
  // a unique identifier for each PythonScript instance.
  if( module_name == "" )
    module_name = getInstanceName();

  // ensure we have the GIL lock to work with multiple python threads.
  PyGILState_STATE state = PyGILState_Ensure();

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

  // Add __scriptnode__ to module dictionary which is the node the script resides in. 
  // Do not let the PyNode hold a reference count for the PythonScript node, otherwise
  // it could never be deleted.
  PyObject *scriptnode = PyNode_FromNode( this, false );

  PyDict_SetItem( (PyObject *) module_dict, 
                  PyString_FromString( "__scriptnode__" ), 
                  scriptnode);
  Py_DECREF( scriptnode );

  bool script_loaded = false;
  for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) {
    // First try to resolve URL to file contents, if that is not supported
    // by the resolvers then fallback to resolve as local filename
    string url_contents= resolveURLAsString( *i );
    bool is_tmp_file= false;
    string url;
    if ( url_contents == "" ) {
      url= resolveURLAsFile( *i, &is_tmp_file );
    }
    if( url != "" || url_contents != "" ) {
      loadScript( url, url_contents );
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

  PyGILState_Release(state);
}

#endif // HAVE_PYTHON
