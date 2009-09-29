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
    
    virtual Field* getField( const string&name ) {
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
    virtual Field *lookupField( const string &name );

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
    
    virtual void loadScript( const string &script );

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
    /// the DEF-name of the PythonScript will be used. An automatically
    /// generated name will be used if moduleName is empty and the node does
    /// not have a DEF name.
    ///
    /// <b>Access type:</b> initializeOnly \n
    auto_ptr< SFString > moduleName;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
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
