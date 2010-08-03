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
/// \file SpiderMonkeySAI.h
/// \brief Header file for SAI ECMAScript implementation using the SpiderMonkey
/// javascript engine.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPIDERMONKEYSAI_H__
#define __SPIDERMONKEYSAI_H__
#include <H3D/Field.h>
#include <H3D/X3DTypes.h>
#include <H3D/X3DFieldConversion.h>
#include <H3D/X3DTypeFunctions.h>
#include <H3D/PeriodicUpdate.h>

#ifdef HAVE_SPIDERMONKEY

// forward declarations
struct JSRuntime;
struct JSContext;
struct JSObject;

namespace H3D {
  // forward declaration
  class Script;

  /// The SpiderMonkeySAI class is an implementation of the X3D Ecmascript
  /// SAI specification using SpiderMonkey(the Javascript engine used by 
  /// Firefox)
  class SpiderMonkeySAI {
  public:
    /// Constructor.
    SpiderMonkeySAI();

    /// Initialize the script engine for the given script, adding 
    /// all types, attributes, functions etc . Needs to be called
    /// before any other function are called.
    bool initializeScriptEngine( Script *s );

    /// Run the script specified by s. 
    /// \param s A string with the script code to run.
    /// \param filename A filename/identifier identifying the
    /// source of the string. Used for error messages.
    string loadScript( const string &s, const string &filename );

    /// Release all resources allocated for the script engine.
    void uninitializeScriptEngine( );

    /// Returns true if the script engine has been initialized.
    inline bool isInitialized() { return rt != NULL; }

    /// Add an interface to a field to the script engine. It will be
    /// added to the global namespace with the property name equal
    /// to the name of the field.
    virtual bool addField( Field *field );

  protected:
    /// The script engine runtime used.
    JSRuntime *rt;

    /// The script engine context used.
    JSContext *cx;

    /// The global object including all global properties and 
    /// functions.
    JSObject  *global;

    /// The Script node this script engine is used for,
    Script *script_node;

    /// The CallbackFunctionDispatcher class handles the calls
    /// of the ecmascript callback functions for the fields.
    class CallbackFunctionDispatcher: 
	public PeriodicUpdate< EventCollectingField<Field> > {
      virtual void update();
    };

    /// Given a field name it returns the name used for the callback
    /// function in the global namespace. According to the Ecmascript
    /// X3D specification the callback function and the field property
    /// name should be the same. This is not possible in SpiderMonkey
    /// so we rename the callback function to the name returned by this
    /// function.
    string fieldNameToCallbackName( const string &n );
      
    /// Field that handles calling of callback functions as
    /// events are received.
    auto_ptr< CallbackFunctionDispatcher > callbackFunctionDispatcher;
  };
}

#endif // HAVE_SPIDERMONKEY
#endif
