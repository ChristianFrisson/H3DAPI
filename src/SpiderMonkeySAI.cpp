//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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


/////////////////////////////////////////////////////////////
// global object

/* The class of the global object. */
static JSClass global_class = {
  "global", JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
    JS_PropertyStub, JS_PropertyStub, SFNode_getProperty, SFNode_setProperty,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, PrivatePointer_finalize<FieldObjectPrivate>,  // finalize,
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
  //  cerr << "Init" << endl;
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
  // The script node will get a reference by being
  // put into the SFNode. We remove this reference in order to make sure
	// that the Script nodes destructor is called. The scrip nodes destructor then
	// use a trick in order to delete this SFNode instance without accidently calling
	// the Script nodes own destructor again.
  script_node->unref();

	JS_SetPrivate( cx, global, (void *) new FieldObjectPrivate( f, true ) );

  /* Populate the global object with the standard globals,
     like Object and Array. */
  if (!JS_InitStandardClasses(cx, global))
    return false;

  // if (!JS_DefineFunctions(cx, global, sai_global_functions))
  //    return JS_FALSE;

  // create the Browser property
  JSObject *browser = 
    Browser_newInstance( cx, 
			 (*Scene::scenes.begin())->getSAIBrowser(), 
			 false );

  // add it as the name "Browser" to the global object.
  JS_DefineProperty( cx, global, 
		     "Browser", 
		     OBJECT_TO_JSVAL(browser),
		     JS_PropertyStub, JS_PropertyStub,
		     JSPROP_READONLY | JSPROP_PERMANENT  );

  // Add all H3D field types such as SFVec3f, MFBool, SFNode etc to 
  // the global object.
  insertH3DTypes( cx, global );

  // Add all fields specificed in the script to the global object in order
  // to be able to access them directly in the ecmascript source code.
  for( H3DDynamicFieldsObject::field_iterator i = script_node->firstField();
       i != script_node->endField(); ++i ) {
    addField( *i );
  }
  
  // Set up route to callbackFunctionDispatcher which controls the
  // calling of the special functions initialize, prepareEvents and
  // eventsProcessed.
  Scene::time->route( callbackFunctionDispatcher );

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
	 i != script_node->endField(); ++i ) {

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
     
    // run initialize function if it exists
    
    if( haveFunction( cx, global, "initialize" ) ) {
      //jsval arg[] = {};
      jsval res;
      JS_CallFunctionName( cx, global, "initialize", 0, NULL, &res ); 
    }

    str = JS_ValueToString(cx, rval); 
    return JS_GetStringBytes(str);
  } else {
    // Make sure top-level error is reported appropriately.
    if( JS_IsExceptionPending( cx ) ) {
      JS_ReportPendingException( cx );
    }
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

  //  cerr << "addField: " << field->getName() << endl;
  // TODO: when delete?
  string name = field->getName();
  jsval js_field = jsvalFromField( cx, field, false );

  if( js_field != JSVAL_VOID ) //&&
    //      JS_HasProperty( cx, global, name.c_str(), &found ) &&
    //!found ) { 
    {
      //    cerr << "Defining property: " << name << endl;
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
  bool event_processed = false;
  for( unsigned int i = 0; i < routes_in.size(); ++i ) {
    if( hasCausedEvent( routes_in[i] ) && routes_in[i] != Scene::time.get() ) {
      string field_name = routes_in[i]->getName();
      string function_name = sai->fieldNameToCallbackName( field_name );

      if( haveFunction( sai->cx, sai->global, function_name.c_str() ) ) {
	// the callback function has the same name as the field
	// try to find it in the script and execute it.
	jsval res;
	jsval arg[] = { 
	  jsvalFromField( sai->cx, routes_in[i], true ),
	  jsvalFromField( sai->cx, Scene::time.get(), true ) 
	};
	
	//cerr << "Calling function: " << function_name << endl;
	JS_CallFunctionName( sai->cx, sai->global,
			     function_name.c_str(),
			     2,
			     arg,
			     &res );
	
	event_processed = true;
      }
    }
  }

  // run initialize function if it exists
  //jsval arg[] = {};
  jsval res;

  // call eventsProcessed callback function if we have processed
  // and event.
  if( event_processed && 
      haveFunction( sai->cx, sai->global, "eventsProcessed" ) ) {
    JS_CallFunctionName( sai->cx, sai->global, 
			 "eventsProcessed", 0, NULL, &res ); 
  }

  // call prepareEvents callback function if the Scene::time has
  // advanced one tick.
  if( hasCausedEvent( Scene::time ) &&
      haveFunction( sai->cx, sai->global, "prepareEvents" )) {
    JS_CallFunctionName( sai->cx, sai->global, 
			 "prepareEvents", 0, NULL, &res ); 
  }

  // possibly run garbage collector to dispose of objects
  // no longer in use.
  JS_MaybeGC( sai->cx );

  // call base class
  PeriodicUpdate< EventCollectingField< Field > >::update();
}

#endif // HAVE_SPIDERMONKEY

