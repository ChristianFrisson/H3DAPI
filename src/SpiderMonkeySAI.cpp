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
#include <H3D/SpiderMonkeyTypes.h>
#include <H3D/Scene.h>
#include <H3D/Script.h>

#ifdef HAVE_SPIDERMONKEY

using namespace H3D;
using namespace H3D::SpiderMonkey;

SpiderMonkeySAI::SpiderMonkeySAI():
  rt( NULL ),
  cx( NULL ),
  global( NULL ),
  script_node( NULL ),
  callbackFunctionDispatcher( new CallbackFunctionDispatcher ) {
  callbackFunctionDispatcher->setName( "callbackFunctionDispatcher" );
  callbackFunctionDispatcher->setOwner( (Node*) this );
}

JSBool SAIPrint(JSContext *cx, 
                JSObject *obj, uintN argc, jsval *argv, jsval *rval) {

  const char *s;
  if (!JS_ConvertArguments(cx, argc, argv, "s", &s))
    return JS_FALSE;

  SAI::browser.print( s );
  *rval = JSVAL_VOID;  /* return undefined */
  return JS_TRUE;
}


static JSFunctionSpec browser_functions[] = {
  {"println",   SAIPrint,   0, 0, 0 },
  {0}
};

////////////////////////////////////////////////////////////////////
// Browser object

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
    {"version", BROWSER_VERSION, JSPROP_READONLY},
    {"name",    BROWSER_NAME,    JSPROP_READONLY},
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
      // if we get here the property was not one of the ones defined in
      // browser_properties. It can be another property such as a function
      // so we check if the previous value of vp contains anything. On call 
      // of this function it contains the current value of the attribute.
      // If it is JSVAL_VOID the attribute does not exist.
      if( *vp == JSVAL_VOID ) {
	JSString *s = JSVAL_TO_STRING( id );
	JS_ReportError(cx, "Browser object does not have property \"%s\".", JS_GetStringBytes( s ) );
	return JS_FALSE;
      } else {
	return JS_TRUE;
      }
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


/////////////////////////////////////////////////////////////
// global object

/* The class of the global object. */
static JSClass global_class = {
  "global", JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, SFNode_getProperty, SFNode_setProperty,
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

bool SpiderMonkeySAI::initializeScriptEngine( Script *_script_node ) {
  cerr << "Init" << endl;
  script_node = _script_node;
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

  // set error reporter so that we get error printed
  // to Console.
  JS_SetErrorReporter(cx, reportError);
  
  /* Create the global object. */
  global = JS_NewObject(cx, &global_class, NULL, NULL);
  if (global == NULL)
    return false;
  
  // Set the private data of the global object to be the script node
  // in order for it to look up fields at global scope.
  SFNode *f = new SFNode;
  f->setValue(script_node );
  // TODO: fix better
  script_node->unref();
  JS_SetPrivate( cx, global, (void *) new FieldObjectPrivate( f ) );

  /* Populate the global object with the standard globals,
     like Object and Array. */
  if (!JS_InitStandardClasses(cx, global))
    return false;

  // if (!JS_DefineFunctions(cx, global, sai_global_functions))
  //    return JS_FALSE;
    
  //TODO: what to with the object? When delete it?
  JSObject *browser = 
    JS_DefineObject( cx, global, "Browser", &BrowserClass, 0, JSPROP_READONLY | JSPROP_PERMANENT );  
  JS_DefineProperties(cx, browser, browser_properties );
  JS_DefineFunctions(cx, browser, browser_functions);

  insertH3DTypes( cx, global );

  for( H3DDynamicFieldsObject::field_iterator i = script_node->firstField();
       i != script_node->endField(); i++ ) {
    addField( *i );
  }
  
  return true;
}

string SpiderMonkeySAI::loadScript( const string &script, const string &filename ) {
  JSString *str; 
  JSBool ok; 

  jsval rval;
  ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(), 
                         filename.c_str(), 1, &rval); 
  if( ok ) {                       

    // check if the script loaded a callback function for any of the fields.
    // if it has, the global object will contain a property with the same
    // name as a field that is a function object (before evaluate script it
    // they were all of their respective field type). If it has changed
    // to a function we rename it to the callback function name and
    // and restore a field propert to the name.
    for( H3DDynamicFieldsObject::field_iterator i = script_node->firstField();
	 i != script_node->endField(); i++ ) {

      string field_name = (*i)->getName();
      jsval res;
      // get the property
      JSBool has_property = JS_GetProperty( cx,
					    global,
					    field_name.c_str(),
					    &res );
      if( has_property && JSVAL_IS_OBJECT( res ) ) {
	JSObject *fun = JSVAL_TO_OBJECT( res );
	if( JS_ObjectIsFunction( cx, fun ) ) {
	  
	  // a callback function has been defined, change the
	  // name of it in order let the property be the field
	  JS_DefineProperty( cx, global, 
			     fieldNameToCallbackName( field_name ).c_str(), 
			     res,
			     JS_PropertyStub, JS_PropertyStub,
			     0 );
	  // restore the field to the name
	  addField( *i );
	}
      }
    }
    str = JS_ValueToString(cx, rval); 
    return JS_GetStringBytes(str);
  }
  return "";
}

string SpiderMonkeySAI::fieldNameToCallbackName( const string &field_name ) {
  return string("H3DEventCallbackFunction_") + field_name;
}

void SpiderMonkeySAI::uninitializeScriptEngine( ) {
  /* Cleanup. */
  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  //JS_ShutDown();
}

bool SpiderMonkeySAI::addField( Field *field ) {
  if( !isInitialized() ) return false;

  cerr << "addField: " << field->getName() << endl;
  // TODO: when delete?
  string name = field->getName();
  jsval js_field = jsvalFromField( cx, field, false );

  if( js_field != JSVAL_VOID ) //&&
    //      JS_HasProperty( cx, global, name.c_str(), &found ) &&
    //!found ) { 
    {
    cerr << "Defining property: " << name << endl;
    JS_DefineProperty( cx, global, name.c_str(), 
		       js_field,
		       SFNode_getProperty, SFNode_setProperty,
		       0 );
  }
  if( field->getAccessType() == Field::INPUT_OUTPUT ) {
   
    /*    JS_DefineProperty( cx, global, (name + "_changed").c_str(), 
		       OBJECT_TO_JSVAL( js_field ),
		       JS_PropertyStub, JS_PropertyStub,
		       JSPROP_READONLY | JSPROP_PERMANENT );  
    JS_DefineProperty( cx, global, ( string("set_" )+ name).c_str(), 
		       OBJECT_TO_JSVAL( js_field ),
		       JS_PropertyStub, JS_PropertyStub,
		       JSPROP_READONLY | JSPROP_PERMANENT );
    */
    
    } 

  bool access_allowed = field->isAccessCheckOn();
  field->setAccessCheck( false );
  field->routeNoEvent( callbackFunctionDispatcher );
  field->setAccessCheck( access_allowed );
  return true;
}

void SpiderMonkeySAI::CallbackFunctionDispatcher::update() {
  SpiderMonkeySAI *sai = (SpiderMonkeySAI *)( getOwner() );
  for( unsigned int i = 0; i < routes_in.size(); i++ ) {
    if( hasCausedEvent( routes_in[i] ) ) {
	string field_name = routes_in[i]->getName();
	string function_name = sai->fieldNameToCallbackName( field_name );
	// the callback function has the same name as the field
	// try to find it in the script and execute it.
	jsval res;
	JSBool has_property = JS_GetProperty( sai->cx,
					  sai->global,
					      function_name.c_str(),
					      &res );
	if( has_property && JSVAL_IS_OBJECT( res ) ) {
	  JSObject *fun = JSVAL_TO_OBJECT( res );
	  if( JS_ObjectIsFunction( sai->cx, fun ) ) {
	    cerr << "Run function: " << field_name << endl;
	    //	  JS_GetFunctionArity( fun ) ) {
	    jsval fun_res;
	    
	    // TODO: memory handling
	    jsval arg[] = { 
	      jsvalFromField( sai->cx, routes_in[i], true ),
	      jsvalFromField( sai->cx, Scene::time.get(), true ) 
	    };

	    cerr << "Calling function" << endl;
	    JS_CallFunctionName( sai->cx, sai->global,
				 function_name.c_str(),
				 2,
				 arg,
				 &fun_res );
	  }
	}
    }
    
  }
  
}

#endif // HAVE_SPIDERMONKEY

