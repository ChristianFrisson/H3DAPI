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
/// \file PythonScript.h
/// \brief Script node for python scripting
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PYTHONSCRIPT_H__
#define __PYTHONSCRIPT_H__

#include <H3DUtil/Exception.h>
#include <H3D/H3DScriptNode.h>
#include <H3D/MFNode.h>
#include <H3D/SFString.h>

#ifdef HAVE_PYTHON

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

namespace H3D {
  
  /// \ingroup X3DNodes 
  /// \class PythonScript
  /// \brief Interface to the dynamic Python library for executing script
  ///  code as part of the API
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PythonScript.x3d">PythonScript.x3d</a>
  ///     ( <a href="examples/PythonScript.x3d.html">Source</a> )
  struct H3DAPI_API PythonScript : public H3DScriptNode {
    ///Constructor.
    PythonScript( Inst< MFString > _url = 0,
                  Inst< MFNode    > _references = 0,
                  Inst< SFString > _moduleName = 0 );
    
    virtual Field * getField( const string&name ) const {
      Field *f = H3DScriptNode::getField( name );
      if( f ) return f;
      if ( name == "references" )
        return references.get();
      else
        return lookupField( name );
    }
    
     /// Destructor.
    ~PythonScript();
    
    /// lookupField() returns the field pointer to the field with the
    /// specified string field name, used when creating nodes via
    /// the H3DNodeDatabase service.
    virtual Field *lookupField( const string &name ) const;

    /// Returns the python object with the specified name from the
    /// python module used by the PythonScript node. NULL if no
    /// object with the specified name is found. The return value is a new
    /// reference to the python object.
    virtual PyObject *getPythonAttribute( const string &name ); 

    /// Adds the fields at the top level of the PythonScript
    /// node and its Python variable name to the given vector.
    virtual void getTopLevelFields( vector< pair< string, Field * > > &fields );

    /// Traverse the scenegraph. Used in PythonScript to call a function
    /// in python once per scene graph loop.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );
    
    virtual void initialiseParser();

    virtual string getInstanceName() {
      ostringstream namestr;
      namestr << "PythonScript_" << this; 
      return namestr.str();   
    }
    
    virtual void loadScript( const string &script_filename, const string &script_content= "" );

    static void setargv( int _argc, char *_argv[] );

    /// Initialize is called once upon the first reference of the 
    /// RefCountedClass.
    virtual void initialize();

    /// Through this field one can pass references to Nodes in the scene
    /// graph that can then be access through the 'references' variable
    /// in the Python script.
    ///
    /// <b>Access type:</b> initializeOnly \n
    auto_ptr< MFNode > references;

    /// Specify a specific moduleName for the pythonscript. This name should
    /// be unique among all PythonScript nodes in the scene. It should also
    /// differ from previously existing python modules. If this field is empty
    /// an automatically generated name will be used. This field should be used
    /// if one python script should import the functionality from another python
    /// script. See the H3DAPI manual for an example.
    ///
    /// <b>Access type:</b> initializeOnly \n
    auto_ptr< SFString > moduleName;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// Allow python code to run in the main thread. This must be called before
    /// any calls to python functions from the main thread. 
    ///
    /// The reason for this is to be able to start and run other Python 
    /// threads from within the script. Python handles tread safety by 
    /// only allowing one python thread to execute at any one time. It
    /// lets on thread run for a certain number of instructions then switch
    /// to the next thread for the same number of instructions and so on.
    /// The problem is that the main python script in H3D most of the time
    /// does not execute anything. Things only happen on events by fields 
    /// and in the traverseSG function which causes the other python threads
    /// to lock up since the main thread never executes the number of 
    /// instructions required to move on to the next thread. In order to 
    /// let these other threads run when the main python thread does nothing
    /// we encapsulate all python code that is executed in main thread 
    /// within allowMainThreadPython/disallowMainThreadPython calls. 
    /// After disallowMainThreadPython is called all other python threads 
    /// can run without waiting for the main thread. However this means that
    /// we must call allowMainThreadPython before any python code that
    /// runs in the main thread or the program will crash.
    static void allowMainThreadPython();

    /// Don't allow python code to run in the main thread, instead letting
    /// other python threads run without disruptions(see allowMainThreadPython 
    /// for more info).
    static void disallowMainThreadPython();

    /// Returns true if python code is allowed to run in main thread at the 
    /// moment.
    static bool mainThreadPythonAllowed();

  private:
    void * module;
    void * module_dict;
    static int argc;
    static char** argv;
    string module_name;
 };
}

#endif // HAVE_PYTHON
#endif
