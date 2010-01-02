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
/// \brief Header file for type definitions for SAI ECMAScript implementation
/// using the SpiderMonkey javascript engine.
///
/// This file defines the SpiderMonkey types for all field types such as
/// SFInt32, SFVec3f, MFBool and so on and other types defined in the 
/// SAI Ecmascript specification. This makes it possible to use these
/// types in the scripts that are used. To add the X3D field types to a
/// JS API object use the insertH#DTypes method. 
/// 
/// Implementation nodes: All field types that map to an JSObject type have
/// an encapsulated Field pointer. The field type in the spec. also 
/// represents the field value itself, allowing access to individual 
/// properties and functions of the field value through the field type. 
/// We distiguish between internal fields and external fields where 
/// internal fields are encapsulated fields which are not a field part of
/// any node, but just a temporary holder of a field value, such as the return
/// value of an addition between two SFVec3f types.
///
/// Field types that map directly to a simple Ecmascript type such as SFBool
/// and SFFloat do not have an encapsulated field pointer. Instead setting
/// and getting the value of these types are handled by the setter/getter
/// function of SFNodeClass. 
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPIDERMONKEYTYPES_H__
#define __SPIDERMONKEYTYPES_H__

#include <H3D/SFFloat.h>
#include <H3D/SFDouble.h>
#include <H3D/SFTime.h>
#include <H3D/SFInt32.h>
#include <H3D/SFString.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFColor.h>
#include <H3D/SFNode.h>
#include <H3D/SFBool.h>

#include <H3D/MFFloat.h>
#include <H3D/MFDouble.h>
#include <H3D/MFTime.h>
#include <H3D/MFInt32.h>
#include <H3D/MFString.h>
#include <H3D/MFVec3f.h>
#include <H3D/MFColor.h>
#include <H3D/MFNode.h>
#include <H3D/MFBool.h>
#include <H3D/SAIFunctions.h>

#ifdef HAVE_SPIDERMONKEY

// defines needed for include of jsapi.h
#ifdef H3D_WINDOWS
#define XP_WIN
#else  // H3D_WINDOWS
#ifdef H3D_OSX
#define XP_WIN
#define X86_LINUX
#define PIC
#define HAVE_VA_COPY
#define VA_COPY va_copy
#else  // H3D_OSX
#define XP_UNIX
#endif // H3D_OSX
#endif // H3D_WINDOWS else

#include <jsapi.h>

namespace H3D {
  namespace SpiderMonkey {

    /// Set a JS API exception based on the c++ SAIError exception.
    void setJSException( JSContext *cx, const SAI::SAIError &e );

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
    template< class PointerType >
    class PointerPrivateData {
    public:
      /// Constructor.
      /// \params field The field the JSObject represents.
      /// \params _internal_pointer If true, the field will be deleted
      /// when the FieldObjectPrivate object is destructed.
      PointerPrivateData( PointerType *_ptr,
			  bool _internal_pointer 
			  ):
	ptr( _ptr ),
	internal_pointer( _internal_pointer ) {
	
      }

      /// Destructor.
      ~PointerPrivateData() {
	if( ptr && internal_pointer ) {
	  delete ptr;
	}
      }

      /// Get the pointer the JSObject encapsulates.
      inline PointerType *getPointer() {
	return ptr;
      }
      
    protected:
      PointerType *ptr;
      bool internal_pointer;
    };

    typedef PointerPrivateData< Field > FieldObjectPrivate;
    typedef PointerPrivateData< SAI::ExecutionContext > ExecutionContextPrivate;

    /// Destruct callback for JSObject with a private datamember. It converts
    /// the pointer in JS_GetPrivate to PointerType and deletes it.
    template< class PointerType >
    void PrivatePointer_finalize(JSContext *cx, JSObject *obj) {
      PointerType *private_data = 
	static_cast<PointerType *>(JS_GetPrivate(cx,obj));
      if( private_data ) {
	delete private_data;
      }  
    }
    
    /// Callback function for converting a JSObject with a FieldObjectPrivate
    /// private data member to a JSString. Only works for fields where the 
    /// encapsulated field is a ParsableField.
    JSBool FieldObject_toString(JSContext *cx, JSObject *obj, 
				uintN argc, jsval *argv,
				jsval *rval);

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
      {"toString", FieldObject_toString, 0, 0, 0 },
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
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
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


    /// Returns a new SFNode object encapsulating a field.
    /// \params cx The context in which to create the object.
    /// \params field The field to encapsulate.
    /// \params internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFNode_newInstance( JSContext *cx,
				  SFNode *field,
				  bool internal_field );

    /// Callback setter function for properties of a SFNode
    /// object.
    JSBool SFNode_setProperty(JSContext *cx, 
			      JSObject *obj, 
			      jsval id, 
			      jsval *vp);

    /// Callback getter function for properties of a SFNode
    /// object.
    JSBool SFNode_getProperty(JSContext *cx, 
			      JSObject *obj, 
			      jsval id, 
			      jsval *vp);

    /// Construct callback function for creating a new instance
    /// of SFNode.
    JSBool SFNode_construct(JSContext *cx, JSObject *obj, 
			    uintN argc, jsval *argv,
			    jsval *rval);
    
    // member functions
    JSBool SFNode_getNodeName(JSContext *cx, JSObject *obj, 
			      uintN argc, jsval *argv,
			      jsval *rval);
    JSBool SFNode_getNodeType(JSContext *cx, JSObject *obj, 
			      uintN argc, jsval *argv,
			      jsval *rval);
    JSBool SFNode_getFieldDefinitions(JSContext *cx, JSObject *obj, 
				      uintN argc, jsval *argv,
				      jsval *rval);
    JSBool SFNode_toVRMLString(JSContext *cx, JSObject *obj, 
			       uintN argc, jsval *argv,
			       jsval *rval);
    JSBool SFNode_toX3DString(JSContext *cx, JSObject *obj, 
			      uintN argc, jsval *argv,
			      jsval *rval);
    

    static JSFunctionSpec SFNode_functions[] = {
      {"getNodeName", SFNode_getNodeName, 0, 0, 0 },
      {"getNodeType", SFNode_getNodeType, 0, 0, 0 },
      {"getFieldDefinitions", SFNode_getFieldDefinitions, 0, 0, 0 },
      {"toString", SFNode_toX3DString, 0, 0, 0 },
      {"toX3DString", SFNode_toX3DString, 0, 0, 0 },
      {"toVRMLString", SFNode_toVRMLString, 0, 0, 0 },
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
      PrivatePointer_finalize< FieldObjectPrivate >,  // finalize
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

    // member functions
    JSBool SFColor_getHSV(JSContext *cx, JSObject *obj, 
			  uintN argc, jsval *argv,
			  jsval *rval);
    JSBool SFColor_setHSV(JSContext *cx, JSObject *obj, 
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
      {"toString", FieldObject_toString, 0, 0, 0 },
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
      PrivatePointer_finalize< FieldObjectPrivate >,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFColor_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    //////////////////////////////////////////////
    /// X3DExecutionContext 
    ///

    enum X3DExecutionContextPropertyId {
      X3DEXECUTIONCONTEXT_ROOTNODES
    };

    /// Returns a new X3DExecutionContext object encapsulating a 
    /// SAI::ExecutionContext object.
    /// \params cx The context in which to create the object.
    /// \params field The field to encapsulate.
    /// \params internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *X3DExecutionContext_newInstance( JSContext *cx,
					       SAI::ExecutionContext *field,
					       bool internal_field );

    /// Callback setter function for properties of a X3DExecutionContext
    /// object.
    JSBool X3DExecutionContext_setProperty(JSContext *cx, 
					   JSObject *obj, 
					   jsval id, 
					   jsval *vp);

    /// Callback getter function for properties of a X3DExecutionContext
    /// object.
    JSBool X3DExecutionContext_getProperty(JSContext *cx, 
					   JSObject *obj, 
					   jsval id, 
					   jsval *vp);

    /// Construct callback function for creating a new instance
    /// of X3DExecutionContext.
    JSBool X3DExecutionContext_construct(JSContext *cx, JSObject *obj, 
					 uintN argc, jsval *argv,
					 jsval *rval);
    

    // member functions
    JSBool X3DExecutionContext_createNode(JSContext *cx, JSObject *obj, 
					  uintN argc, jsval *argv,
					  jsval *rval);

    // properties
    static JSPropertySpec X3DExecutionContext_properties[] = {
      {"rootNodes", X3DEXECUTIONCONTEXT_ROOTNODES, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec X3DExecutionContext_functions[] = {
      {"createNode", X3DExecutionContext_createNode, 0, 0, 0 },
      {0}
    };
    
    static JSClass X3DExecutionContextClass = {
      "X3DExecutionContext",
      JSCLASS_HAS_PRIVATE,
      
      /* All of these can be replaced with the corresponding JS_*Stub
	 function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::X3DExecutionContext_getProperty, // get property
      SpiderMonkey::X3DExecutionContext_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize< ExecutionContextPrivate >,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      NULL, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


  //////////////////////////////////////////////
  /// X3DScene 
  ///

    enum X3DScenePropertyId {
      X3DSCENE_ROOTNODES
    };

    /// Returns a new X3DScene object encapsulating a 
    /// SAI::ExecutionContext object.
    /// \params cx The context in which to create the object.
    /// \params field The field to encapsulate.
    /// \params internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *X3DScene_newInstance( JSContext *cx,
				    SAI::SAIScene *field,
				    bool internal_field );

    /// Callback setter function for properties of a X3DScene
    /// object.
    JSBool X3DScene_setProperty(JSContext *cx, 
				JSObject *obj, 
				jsval id, 
				jsval *vp);

    /// Callback getter function for properties of a X3DScene
    /// object.
    JSBool X3DScene_getProperty(JSContext *cx, 
				JSObject *obj, 
				jsval id, 
				jsval *vp);

    /// Construct callback function for creating a new instance
    /// of X3DScene.
    JSBool X3DScene_construct(JSContext *cx, JSObject *obj, 
			      uintN argc, jsval *argv,
			      jsval *rval);
    

    // member functions

    // properties
    static JSPropertySpec X3DScene_properties[] = {
      {"rootNodes", X3DSCENE_ROOTNODES, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec X3DScene_functions[] = {
      {"createNode", X3DExecutionContext_createNode, 0, 0, 0 },
      {0}
    };
    
    static JSClass X3DSceneClass = {
      "X3DScene",
      JSCLASS_HAS_PRIVATE,
      
      /* All of these can be replaced with the corresponding JS_*Stub
	 function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::X3DScene_getProperty, // get property
      SpiderMonkey::X3DScene_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize< ExecutionContextPrivate >,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      NULL, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };




    //////////////////////////////////////////////
    /// MField template 
    ///
    template< class MFieldType, class ElementType > 
    struct JS_MField {
	
      /// Returns a new SFNode object encapsulating a field.
      /// \params cx The context in which to create the object.
      /// \params field The field to encapsulate.
      /// \params internal_field If true, the encapsulated field
      /// will be deleted upon destruction of the JSObject 
      /// encapsulating it.
      static JSObject *newInstance( JSContext *cx,
				    MFieldType *field,
				    bool internal_field );

      /// Callback setter function for properties of a JS_MField
      /// object.
      static JSBool setProperty(JSContext *cx, 
				JSObject *obj, 
				jsval id, 
				jsval *vp);

      /// Callback getter function for properties of a JS_MField
      /// object.
      static JSBool getProperty(JSContext *cx, 
				JSObject *obj, 
				jsval id, 
				jsval *vp);
      
      /// Construct callback function for creating a new instance
      /// of JS_MField.
      static JSBool construct(JSContext *cx, JSObject *obj, 
			      uintN argc, jsval *argv,
			      jsval *rval);
    
      /// Member functions
      static JSFunctionSpec functions[2];
      static JSClass js_class;
      static JSPropertySpec properties[1];

      static JSBool initClass( JSContext *cx, JSObject *obj, const char *name ) {
	      
	JSPropertySpec p[] = {
	  {0} 
	};
	
	properties[0] = p[0];

	JSFunctionSpec f[2] = {
	  {"toString", FieldObject_toString, 0, 0, 0 },
	  {0}
	};

	functions[0] = f[0];
	functions[1] = f[1];

	JSClass c = {
	name,
	JSCLASS_HAS_PRIVATE,
      
	/* All of these can be replaced with the corresponding JS_*Stub
	   function pointers. */
	JS_PropertyStub,  // add property
	JS_PropertyStub,  // del property
	getProperty, // get property
	setProperty,  // set property
	JS_EnumerateStub, // enumerate
	JS_ResolveStub,   // resolve
	JS_ConvertStub,   // convert
	PrivatePointer_finalize< FieldObjectPrivate >,  // finalize
	NULL, // getObjectOps
	NULL, // checkAccess
	NULL, // call
	construct, // construct
	NULL, // xdrObject
	NULL, // hasInstance
	NULL, // mark
	NULL //reserveSlots
	};
	
	js_class = c;
	
	JSObject *proto = JS_InitClass( cx, obj, NULL, 
					&js_class, construct, 0, 
					properties, functions, NULL, NULL );
	return JS_TRUE;

      }
    };

    template< class MFieldType, class ElementType >
    JSFunctionSpec JS_MField< MFieldType, ElementType >::functions[2];

    template< class MFieldType, class ElementType >
    JSClass JS_MField< MFieldType, ElementType >::js_class;

    template< class MFieldType, class ElementType >
    JSPropertySpec JS_MField< MFieldType, ElementType >::properties[1];
    
    /// Returns a new JS_MField object encapsulating a field.
    /// \params cx The context in which to create the object.
    /// \params field The field to encapsulate.
    /// \params internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    template< class MFieldType, class ElementType >
    JSObject *JS_MField< MFieldType, ElementType >::newInstance( JSContext *cx,
								 MFieldType *field,
								 bool internal_field ) {
      //      cerr << "MFNode newInstance: " << field->getName() << endl;
      JSObject *js_field;
      
      js_field = JS_NewObject( cx, 
			       &js_class, NULL, NULL );  
      
      JS_DefineProperties(cx, js_field, 
			  JS_MField< MFieldType, ElementType >::properties );
      JS_DefineFunctions(cx, js_field, 
			 JS_MField< MFieldType, ElementType >::functions );
      JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
								   internal_field ) );
      return js_field;
    }
    
    /// Callback setter function for properties of a JS_MField
    /// object.
    template< class MFieldType, class ElementType >
    JSBool JS_MField< MFieldType, ElementType >::setProperty(JSContext *cx, 
							     JSObject *obj, 
							     jsval id, 
							     jsval *vp) {
      //      cerr << "MFNode setProperty: "<< endl;
      FieldObjectPrivate *private_data = 
	  static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
	MFieldType* mfield = 
	  static_cast<MFieldType *>(private_data->getPointer());
      if (JSVAL_IS_INT(id)) {
	// value is integer so it is an array index
	JSBool res = JS_TRUE;
	int index = JSVAL_TO_INT(id);

	// resize mfield if it is not big enough.
	if( index >= mfield->size() ) {
	  mfield->resize( index+1 );
	}

	auto_ptr< ElementType > n( new ElementType );

	if(setFieldValueFromjsval( cx, n.get(), *vp ) ) {
	  mfield->setValue( index, n->getValue() );  
	  //	  mfield->push_back( n->getValue() );
 	} else {
	  stringstream s;
	  s << mfield->getTypeName() << " element must be of type " 
	    << n->getValue();
	  JS_ReportError(cx, s.str().c_str() );
	  return JS_FALSE; 
	}

      } else if( JSVAL_IS_STRING( id ) ){
	// value is string, so it is a named property
	JSString *s = JSVAL_TO_STRING( id );
	string prop = JS_GetStringBytes( s );
	if( prop == "length" ) {
	  int32 size;
	  if( JS_ValueToInt32( cx, *vp, &size ) ) {
	    mfield->resize( size );
	  } else {
	    JS_ReportError(cx, "Could not convert to integer value." );
	    return JS_FALSE; 
	  }
	}
      }

      return JS_TRUE;
    }
    
    /// Callback getter function for properties of a JS_MField
    /// object.
    template< class MFieldType, class ElementType >
    JSBool JS_MField< MFieldType, ElementType >::getProperty(JSContext *cx, 
							     JSObject *obj, 
							     jsval id, 
							     jsval *vp) {
      
      //      cerr << "MFNode getProperty: "<< endl;
      FieldObjectPrivate *private_data = 
	static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
      MFieldType* mfield = 
	static_cast<MFieldType *>(private_data->getPointer());

      if (JSVAL_IS_INT(id)) {
	// value is integer so it is an array index
	JSBool res = JS_TRUE;
	int index = JSVAL_TO_INT(id);

	if( index < mfield->size() ) {
	  ElementType *sfield = new ElementType;
	  sfield->setValue( MField_getValueNoAccessCheck( mfield )[index] );
	  *vp = jsvalFromField( cx, sfield, false );
	}
      } else if( JSVAL_IS_STRING( id ) ){
	// value is string, so it is a named property
	JSString *s = JSVAL_TO_STRING( id );
	string prop = JS_GetStringBytes( s );
	if( prop == "length" ) {
	  *vp = INT_TO_JSVAL( mfield->size() );
	}
      }
      return JS_TRUE;
    }
    
    /// Construct callback function for creating a new instance
    /// of SFNode.
    template< class MFieldType, class ElementType >
    JSBool JS_MField< MFieldType, ElementType >::construct(JSContext *cx, JSObject *obj, 
							   uintN argc, jsval *argv,
							   jsval *rval) {

      // the constructor arguments should be a sequence of jsval representing
      // a number of ElementType values. 

      MFieldType *mfield = new MFieldType;
      auto_ptr< ElementType > element( new ElementType );
      for( unsigned int i = 0; i < argc; i++ ) {
	if( setFieldValueFromjsval( cx, element.get(), argv[i] ) ) {
	  mfield->push_back( element->getValue() );
	} else {
	  delete mfield;
	  stringstream s;
	  s << "Could not convert argument " << i << " to " << element->getTypeName();
	  JS_ReportError(cx, s.str().c_str() );
	  return JS_FALSE;
	}
      }
  
      // create return value
      *rval = OBJECT_TO_JSVAL( (JS_MField< MFieldType, ElementType >::newInstance( cx, 
										  mfield,
										   true )) ); 
      return JS_TRUE;
    }
    
    /// The JSAPI type encapsulating an MFFloat object.
    typedef JS_MField< MFFloat,  SFFloat  > JS_MFFloat;

    /// The JSAPI type encapsulating an MFDouble object.
    typedef JS_MField< MFDouble, SFDouble > JS_MFDouble;

    /// The JSAPI type encapsulating an MFTime object.
    typedef JS_MField< MFTime,   SFTime   > JS_MFTime;

    /// The JSAPI type encapsulating an MFInt32 object.
    typedef JS_MField< MFInt32,  SFInt32  > JS_MFInt32;

    /// The JSAPI type encapsulating an MFVec3f object.
    typedef JS_MField< MFVec3f,  SFVec3f  > JS_MFVec3f;

    /// The JSAPI type encapsulating an MFBool object.
    typedef JS_MField< MFBool,   SFBool   > JS_MFBool;

    /// The JSAPI type encapsulating an MFString object.
    typedef JS_MField< MFString, SFString > JS_MFString;

    /// The JSAPI type encapsulating an MFNode object.
    typedef JS_MField< MFNode,   SFNode   > JS_MFNode;

    /// The JSAPI type encapsulating an MFColor object.
    typedef JS_MField< MFColor,  SFColor > JS_MFColor;

  }
}

#endif // HAVE_SPIDERMONKEY

#endif
