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
/// \file SpiderMonkeyTypes.h
/// \brief Header file for SAI ECMAScript implementation using the SpiderMonkey
/// javascript engine.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPIDERMONKEYTYPES_H__
#define __SPIDERMONKEYTYPES_H__

#include <H3D/SFVec3f.h>
#include <H3D/SFColor.h>
#include <H3D/SFNode.h>

#ifdef HAVE_SPIDERMONKEY

// defines needed for include of jsapi.h
#ifdef H3D_WINDOWS
#define XP_WIN
#else
#define XP_UNIX
#endif

#include <jsapi.h>

namespace H3D {
  namespace SpiderMonkey {

    /// Insert the H3D data types, such as SFFloat, MFInt32 etc
    /// into the context and object given.
    bool insertH3DTypes( JSContext *cx, JSObject *obj );

    /// Create a jsval object containing the value of the field(and possibly
    /// encapsulating the field). If the field is of a type that matches
    /// a simple type such as Boolean or Numberic in the X3D Ecmascript
    /// specification a new such value is returned. If it is more complicated
    /// type that uses a JSObject the value is represented by the field inside
    /// this object. In this case make_copy dtermines if the encapsulated field
    /// should be the field given to the function or a copy of it.
    /// The new hsval is returned. JSVAL_VOID is returned on failure.
    jsval jsvalFromField( JSContext* cx, Field *f, bool make_copy );

    /// Sets the field value to the value of the jsval v.
    /// Returns JS_TRUE on success, JS_FALSE otherwise.
    JSBool setFieldValueFromjsval( JSContext* cx, Field *f, jsval v );

    /// Returns JS_TRUE if the name is a function in the given object.
    JSBool haveFunction( JSContext* cx,
			 JSObject *obj, 
			 const char * name );

    /// Class used as the private data in field JSObject classes.
    class FieldObjectPrivate {
    public:
      /// Constructor.
      /// \params field The field the JSObject represents.
      /// \params _internal field If true, the field will be deleted
      /// when the FieldObjectPrivate object is destructed.
      FieldObjectPrivate( Field *field,
			  bool _internal_field 
			  ):
	f( field ),
	internal_field( _internal_field ) {
	
      }

      /// Destructor.
      ~FieldObjectPrivate() {
	if( f && internal_field ) {
	  delete f;
	}
      }

      /// Get the field the JSObject encapsulates.
      inline Field *getField() {
	return f;
      }
      

      /// Set the field the JSObject encapsulates
      inline void setField( Field *field ) {
	f = field;
      }
      
    protected:
      Field *f;
      bool internal_field;
    };

    //////////////////////////////////////////////
    /// SFVec3f 
    ///

    enum SFVec3fPropertyId {
      SFVEC3F_X, SFVEC3F_Y, SFVEC3F_Z
    };

    /// Returns a new SFVec3f object encapsulating a field.
    /// \params cx The context in which to create the object.
    /// \params field The field to encapsulate.
    /// \params internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec3f_newInstance( JSContext *cx,
				   SFVec3f *field,
				   bool internal_field );

    /// Callback setter function for properties of a SFVec3f
    /// object.
    JSBool SFVec3f_setProperty(JSContext *cx, 
			       JSObject *obj, 
			       jsval id, 
			       jsval *vp);

    /// Callback getter function for properties of a SFVec3f
    /// object.
    JSBool SFVec3f_getProperty(JSContext *cx, 
			       JSObject *obj, 
			       jsval id, 
			       jsval *vp);

    /// Construct callback function for creating a new instance
    /// of SFVec3f.
    JSBool SFVec3f_construct(JSContext *cx, JSObject *obj, 
			     uintN argc, jsval *argv,
			     jsval *rval);
    
    /// Destruct callback for when SFVec3f is destructed.
    void SFVec3f_finalize(JSContext *cx, JSObject *obj);

    // member functions
    JSBool SFVec3f_add(JSContext *cx, JSObject *obj, 
		       uintN argc, jsval *argv,
		       jsval *rval);
    JSBool SFVec3f_cross(JSContext *cx, JSObject *obj, 
			 uintN argc, jsval *argv,
			 jsval *rval);
    JSBool SFVec3f_divide(JSContext *cx, JSObject *obj, 
			  uintN argc, jsval *argv,
			  jsval *rval);
    JSBool SFVec3f_dot(JSContext *cx, JSObject *obj, 
		       uintN argc, jsval *argv,
		       jsval *rval);
    JSBool SFVec3f_length(JSContext *cx, JSObject *obj, 
			  uintN argc, jsval *argv,
			  jsval *rval);
    JSBool SFVec3f_multiple(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);
    JSBool SFVec3f_negate(JSContext *cx, JSObject *obj, 
			  uintN argc, jsval *argv,
			  jsval *rval);
    JSBool SFVec3f_normalize(JSContext *cx, JSObject *obj, 
			     uintN argc, jsval *argv,
			     jsval *rval);
    JSBool SFVec3f_subtract(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);
    JSBool SFVec3f_toString(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);

    // properties
    static JSPropertySpec SFVec3f_properties[] = {
      {"x", SFVEC3F_X, JSPROP_PERMANENT},
      {"y", SFVEC3F_Y, JSPROP_PERMANENT},
      {"z", SFVEC3F_Z, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFVec3f_functions[] = {
      {"add", SFVec3f_add, 1, 0, 0 },
      {"cross", SFVec3f_cross, 1, 0, 0 },
      {"divide", SFVec3f_divide, 1, 0, 0 },
      {"dot", SFVec3f_dot, 1, 0, 0 },
      {"length", SFVec3f_length, 0, 0, 0 },
      {"multiple", SFVec3f_multiple, 1, 0, 0 },
      {"negate", SFVec3f_negate, 0, 0, 0 },
      {"normalize", SFVec3f_normalize, 0, 0, 0 },
      {"subtract", SFVec3f_subtract, 1, 0, 0 },
      {"toString", SFVec3f_toString, 0, 0, 0 },
      {0}
    };
    
    static JSClass SFVec3fClass = {
      "SFVec3f",
      JSCLASS_HAS_PRIVATE,
      
      /* All of these can be replaced with the corresponding JS_*Stub
	 function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFVec3f_getProperty, // get property
      SpiderMonkey::SFVec3f_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      SFVec3f_finalize,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFVec3f_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    //////////////////////////////////////////////
    /// SFNode 
    ///


    // class functions
    JSObject *SFNode_newInstance( JSContext *cx,
				  SFNode *field,
				  bool internal_field );

    JSBool SFNode_setProperty(JSContext *cx, 
			      JSObject *obj, 
			      jsval id, 
			      jsval *vp);

    JSBool SFNode_getProperty(JSContext *cx, 
			      JSObject *obj, 
			      jsval id, 
			      jsval *vp);

    JSBool SFNode_construct(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);
    
    void SFNode_finalize(JSContext *cx, JSObject *obj);

    // member functions
    /*    JSBool SFNode_subtract(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);
    JSBool SFNode_toString(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);
    */
    static JSFunctionSpec SFNode_functions[] = {
      //      {"toString", SFNode_toString, 0, 0, 0 },
      {0}
    };
    
    static JSClass SFNodeClass = {
      "SFNode",
      JSCLASS_HAS_PRIVATE,
      
      /* All of these can be replaced with the corresponding JS_*Stub
	 function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFNode_getProperty, // get property
      SpiderMonkey::SFNode_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      SFNode_finalize,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFNode_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    //////////////////////////////////////////////
    /// SFColor 
    ///

    enum SFColorPropertyId {
      SFCOLOR_R, SFCOLOR_G, SFCOLOR_B
    };

    /// Returns a new SFColor object encapsulating a field.
    /// \params cx The context in which to create the object.
    /// \params field The field to encapsulate.
    /// \params internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFColor_newInstance( JSContext *cx,
				   SFColor *field,
				   bool internal_field );

    /// Callback setter function for properties of a SFColor
    /// object.
    JSBool SFColor_setProperty(JSContext *cx, 
			       JSObject *obj, 
			       jsval id, 
			       jsval *vp);

    /// Callback getter function for properties of a SFColor
    /// object.
    JSBool SFColor_getProperty(JSContext *cx, 
			       JSObject *obj, 
			       jsval id, 
			       jsval *vp);

    /// Construct callback function for creating a new instance
    /// of SFColor.
    JSBool SFColor_construct(JSContext *cx, JSObject *obj, 
			     uintN argc, jsval *argv,
			     jsval *rval);
    
    /// Destruct callback for when SFColor is destructed.
    void SFColor_finalize(JSContext *cx, JSObject *obj);

    // member functions
    JSBool SFColor_getHSV(JSContext *cx, JSObject *obj, 
			  uintN argc, jsval *argv,
			  jsval *rval);
    JSBool SFColor_setHSV(JSContext *cx, JSObject *obj, 
			  uintN argc, jsval *argv,
			  jsval *rval);
    JSBool SFColor_toString(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);

    // properties
    static JSPropertySpec SFColor_properties[] = {
      {"r", SFCOLOR_R, JSPROP_PERMANENT},
      {"g", SFCOLOR_G, JSPROP_PERMANENT},
      {"b", SFCOLOR_B, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFColor_functions[] = {
      {"getHSV", SFColor_getHSV, 1, 0, 0 },
      {"setHSV", SFColor_setHSV, 1, 0, 0 },
      {"toString", SFColor_toString, 0, 0, 0 },
      {0}
    };
    
    static JSClass SFColorClass = {
      "SFColor",
      JSCLASS_HAS_PRIVATE,
      
      /* All of these can be replaced with the corresponding JS_*Stub
	 function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFColor_getProperty, // get property
      SpiderMonkey::SFColor_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      SFColor_finalize,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFColor_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

  }
}

#endif // HAVE_SPIDERMONKEY

#endif
