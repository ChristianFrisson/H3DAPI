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
/// \file SpiderMonkeySAI.cpp
/// \brief Source file for SAI ECMAScript implementation using the SpiderMonkey
/// javascript engine.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SpiderMonkeySAI.h>
#include <H3D/SAIFunctions.h>

#ifdef HAVE_SPIDERMONKEY

// defines needed for include of jsapi.h
#ifdef H3D_WINDOWS
#define XP_WIN
#else
#define XP_UNIX
#endif

#include <jsapi.h>

using namespace H3D;

JSBool SAIPrint(JSContext *cx, 
                JSObject *obj, uintN argc, jsval *argv, jsval *rval) {

  const char *s;
  if (!JS_ConvertArguments(cx, argc, argv, "s", &s))
    return JS_FALSE;

  SAI::browser.print( s );
  *rval = JSVAL_VOID;  /* return undefined */
  return JS_TRUE;
}


static JSFunctionSpec sai_global_functions[] = {
  {"println",   SAIPrint,   0, 0, 0 },
  {0}
};

/*
 * Define a bunch of properties with a JSPropertySpec array statically
 * initialized and terminated with a null-name entry.  Besides its name,
 * each property has a "tiny" identifier (MY_COLOR, e.g.) that can be used
 * in switch statements (in a common my_getProperty function, for example).
 */
enum BrowserPropertyId {
    BROWSER_NAME, BROWSER_VERSION
};

static JSPropertySpec browser_properties[] = {
    {"version", BROWSER_NAME,       JSPROP_READONLY},
    {"name",    BROWSER_VERSION,    JSPROP_READONLY},
    {0}
};

static JSBool
BrowserGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    if (JSVAL_IS_INT(id)) {
        switch (JSVAL_TO_INT(id)) {
        case BROWSER_VERSION: {
          string version = SAI::browser.getVersion();
          *vp = STRING_TO_JSVAL( JS_NewStringCopyN( cx,
                                                    (char *)version.c_str(),
                                                    version.length() ) );
          break;
        }
        case BROWSER_NAME: {
          string name = SAI::browser.getName();
          *vp = STRING_TO_JSVAL( JS_NewStringCopyN( cx, 
                                                   (char * )name.c_str(), 
                                                    name.length() ) );
          break;
        }
        }
        return JS_TRUE;
    } else {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Browser object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    }
}

static JSClass BrowserClass = {
    "Browser",
    0,

    /* All of these can be replaced with the corresponding JS_*Stub
       function pointers. */
    JS_PropertyStub,  // add property
    JS_PropertyStub,  // del property
    BrowserGetProperty, // get property
    JS_PropertyStub,  // set property
    JS_EnumerateStub, // enumerate
    JS_ResolveStub,   // resolve
    JS_ConvertStub,   // convert
    JS_FinalizeStub,  // finalize
    JSCLASS_NO_OPTIONAL_MEMBERS
};


/* The class of the global object. */
static JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};



/* The error reporter callback. */
void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
  Console(4) << report->filename ? report->filename : "<no filename>" ;
  Console(4) << ":"
             << (unsigned int) report->lineno
             << ":"
             << message << endl;
}

bool SpiderMonkeySAI::initializeScriptEngine() {
  
  /* Create a JS runtime. */
  rt = JS_NewRuntime(8L * 1024L * 1024L);
  if (rt == NULL)
    return false;
  
  /* Create a context. */
  cx = JS_NewContext(rt, 8192);
  if (cx == NULL)
    return false;
  JS_SetOptions(cx, JSOPTION_VAROBJFIX);
  JS_SetVersion(cx, JSVERSION_ECMA_3);
  JS_SetErrorReporter(cx, reportError);
  
  /* Create the global object. */
  global = JS_NewObject(cx, &global_class, NULL, NULL);
  if (global == NULL)
    return false;
  
  /* Populate the global object with the standard globals,
     like Object and Array. */
  if (!JS_InitStandardClasses(cx, global))
    return false;

  if (!JS_DefineFunctions(cx, global, sai_global_functions))
    return JS_FALSE;
    
  //TODO: what to with the object? When delete it?
  JSObject *browser = 
    JS_DefineObject( cx, global, "Browser", &BrowserClass, 0, JSPROP_READONLY | JSPROP_PERMANENT );  
  JS_DefineProperties(cx, browser, browser_properties );
  
  return true;
}

string SpiderMonkeySAI::loadScript( const string &script, const string &filename ) {
  JSString *str; 
  JSBool ok; 

  jsval rval;
  ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(), 
                         filename.c_str(), 1, &rval); 
  if( ok ) {                       
    str = JS_ValueToString(cx, rval); 
    return JS_GetStringBytes(str);
  }
  return "";
}

void SpiderMonkeySAI::uninitializeScriptEngine() {
  /* Cleanup. */
  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  //JS_ShutDown();
}

#endif // HAVE_SPIDERMONKEY
