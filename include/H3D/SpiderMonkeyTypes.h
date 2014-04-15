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

#include<iostream>

#include <H3D/SFMatrix3f.h>
#include <H3D/SFMatrix4f.h>
#include <H3D/SFFloat.h>
#include <H3D/SFDouble.h>
#include <H3D/SFTime.h>
#include <H3D/SFInt32.h>
#include <H3D/SFString.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFVec3d.h>
#include <H3D/SFVec4f.h>
#include <H3D/SFVec4d.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFVec2d.h>
#include <H3D/SFColor.h>
#include <H3D/SFRotation.h>
#include <H3D/SFNode.h>
#include <H3D/SFBool.h>
#include <H3D/X3DTexture2DNode.h>

#include <H3D/MFFloat.h>
#include <H3D/MFDouble.h>
#include <H3D/MFTime.h>
#include <H3D/MFInt32.h>
#include <H3D/MFString.h>
#include <H3D/MFVec3f.h>
#include <H3D/MFVec3d.h>
#include <H3D/MFVec2f.h>
#include <H3D/MFVec2d.h>
#include <H3D/MFVec4f.h>
#include <H3D/MFVec4d.h>
#include <H3D/MFColor.h>
#include <H3D/MFRotation.h>
#include <H3D/MFNode.h>
#include <H3D/MFBool.h>
#include <H3D/SAIFunctions.h>

#ifdef HAVE_SPIDERMONKEY

// defines needed for include of jsapi.h
#ifdef H3D_WINDOWS
#define XP_WIN
#else  // H3D_WINDOWS
#define XP_UNIX
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
    jsval jsvalFromField( JSContext* cx, Field *f, bool make_copy, 
      int array_index = -1, X3DTypes::X3DType x3dt = X3DTypes::UNKNOWN_X3D_TYPE );

    /// Sets the field value to the value of the jsval v.
    /// Returns JS_TRUE on success, JS_FALSE otherwise.
    JSBool setFieldValueFromjsval( JSContext* cx, Field *f, jsval v );

    /// Returns JS_TRUE if the name is a function in the given object.
    JSBool haveFunction( JSContext* cx, JSObject *obj, const char * name );

    /// Class used as the private data in field JSObject classes.
    template< class PointerType >
    class PointerPrivateData {
    public:
      /// Constructor.
      /// \param _ptr A pointer to the class for which this private data
      /// should be created.
      /// \param _internal_pointer If true, the field will be deleted
      /// when the FieldObjectPrivate object is destructed.
      /// \param _array_index Index into array if MF object.
      PointerPrivateData( PointerType *_ptr,
                          bool _internal_pointer,
                          int _array_index = -1
                          ):
        ptr( _ptr ),
        internal_pointer( _internal_pointer ),
        array_index( _array_index ),
        disposed( false ) {

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
      
      /// Returns true if this data object has been disposed.
      inline bool isDisposed() {
        return disposed;
      }

      /// Return the array index (in case we using (MF, index) to represent an SF object
      /// Return -1 if it IS an SF object.
      inline int getArrayIndex() {
        return array_index;
      }

      /// Dispose the object. Sets the dispose flag to true.
      inline void dispose() {
        disposed = true;
      }

    protected:
      PointerType *ptr;
      bool internal_pointer;
      bool disposed;
      int array_index;
    };

    typedef PointerPrivateData< Field > FieldObjectPrivate;
    typedef PointerPrivateData< SAI::ExecutionContext > ExecutionContextPrivate;
    typedef PointerPrivateData< SAI::Browser > BrowserPrivate;

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


    template<class FieldType>
    JSBool FieldObject_toString2(JSContext *cx, JSObject *obj, 
      uintN argc, jsval *argv, jsval *rval);


    //////////////////////////////////////////////
    /// SFVec2f 
    ///

    enum SFVec2fPropertyId {
      SFVEC2F_X, SFVEC2F_Y, SFVEC2F_Z
    };

    JSBool SFVec2f_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);


    /// Returns a new SFVec2f object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec2f_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFVec2f
    /// object.
    JSBool SFVec2f_setProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Callback getter function for properties of a SFVec2f
    /// object.
    JSBool SFVec2f_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFVec2f.
    JSBool SFVec2f_construct(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    
    // member functions
    JSBool SFVec2f_add(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec2f_divide(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec2f_dot(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec2f_length(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec2f_multiple(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    JSBool SFVec2f_negate(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec2f_normalize(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    JSBool SFVec2f_subtract(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    
    // properties
    static JSPropertySpec SFVec2f_properties[] = {
      {"x", SFVEC2F_X, JSPROP_PERMANENT},
      {"y", SFVEC2F_Y, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFVec2f_functions[] = {
      {"add", SFVec2f_add, 1, 0, 0 },
      {"divide", SFVec2f_divide, 1, 0, 0 },
      {"dot", SFVec2f_dot, 1, 0, 0 },
      {"length", SFVec2f_length, 0, 0, 0 },
      {"multiple", SFVec2f_multiple, 1, 0, 0 },
      {"negate", SFVec2f_negate, 0, 0, 0 },
      {"normalize", SFVec2f_normalize, 0, 0, 0 },
      {"subtract", SFVec2f_subtract, 1, 0, 0 },
      {"toString", SFVec2f_toString, 0, 0, 0 },
      {0}
    };

    
    static JSClass SFVec2fClass = {
      "SFVec2f",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFVec2f_getProperty, // get property
      SpiderMonkey::SFVec2f_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFVec2f_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    //////////////////////////////////////////////
    /// SFVec2d 
    ///

    enum SFVec2dPropertyId {
      SFVEC2D_X, SFVEC2D_Y, SFVEC2D_Z
    };

    JSBool SFVec2d_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);


    /// Returns a new SFVec2d object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec2d_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFVec2d
    /// object.
    JSBool SFVec2d_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    
    /// Callback getter function for properties of a SFVec2d
    /// object.
    JSBool SFVec2d_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFVec2d.
    JSBool SFVec2d_construct(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    
    // member functions
    JSBool SFVec2d_add(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec2d_divide(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec2d_dot(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec2d_length(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec2d_multiple(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    JSBool SFVec2d_negate(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec2d_normalize(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    JSBool SFVec2d_subtract(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    
    // properties
    static JSPropertySpec SFVec2d_properties[] = {
      {"x", SFVEC2D_X, JSPROP_PERMANENT},
      {"y", SFVEC2D_Y, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFVec2d_functions[] = {
      {"add", SFVec2d_add, 1, 0, 0 },
      {"divide", SFVec2d_divide, 1, 0, 0 },
      {"dot", SFVec2d_dot, 1, 0, 0 },
      {"length", SFVec2d_length, 0, 0, 0 },
      {"multiple", SFVec2d_multiple, 1, 0, 0 },
      {"negate", SFVec2d_negate, 0, 0, 0 },
      {"normalize", SFVec2d_normalize, 0, 0, 0 },
      {"subtract", SFVec2d_subtract, 1, 0, 0 },
      {"toString", SFVec2d_toString, 0, 0, 0 },
      {0}
    };

    
    static JSClass SFVec2dClass = {
      "SFVec2d",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFVec2d_getProperty, // get property
      SpiderMonkey::SFVec2d_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFVec2d_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };




    //////////////////////////////////////////////
    /// SFImage 
    ///

    typedef X3DTexture2DNode::SFImage SFImage;

    enum SFImagePropertyId {
      SFImage_WIDTH, SFImage_HEIGHT, SFImage_COMP, SFImage_ARRAY,
    };

    JSBool SFImage_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv, jsval *rval);


    /// Returns a new SFImage object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFImage_newInstance( JSContext *cx, /*SFImage*/ Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFImage
    /// object.
    JSBool SFImage_setProperty(JSContext *cx, JSObject *obj,
                               jsval id, jsval *vp);
    
    /// Callback getter function for properties of a SFImage
    /// object.
    JSBool SFImage_getProperty(JSContext *cx, JSObject *obj, 
                               jsval id, jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFImage.
    JSBool SFImage_construct(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);

    JSBool SFImage_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);


    // properties
    static JSPropertySpec SFImage_properties[] = {
      {"width", SFImage_WIDTH, JSPROP_PERMANENT},
      {"height", SFImage_HEIGHT, JSPROP_PERMANENT},
      {"comp", SFImage_COMP, JSPROP_PERMANENT},
      {"array", SFImage_ARRAY, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFImage_functions[] = {
      {"toString", SFImage_toString, 0, 0, 0 },
      {0}
    };

    
    static JSClass SFImageClass = {
      "SFImage",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFImage_getProperty, // get property
      SpiderMonkey::SFImage_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFImage_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


    //////////////////////////////////////////////
    /// SFVec3f 
    ///

    enum SFVec3fPropertyId {
      SFVEC3F_X, SFVEC3F_Y, SFVEC3F_Z
    };

    JSBool SFVec3f_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);


    /// Returns a new SFVec3f object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec3f_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
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
    /// X3DMatrix3 
    ///

    class SFMatrix3fRow : public Field {
    public:
      SFMatrix3fRow() { }
      SFMatrix3fRow(SFMatrix3f* _sfmatrix, int _row) : row(_row), sfmatrix(_sfmatrix) { }
      SFMatrix3f* sfmatrix;
      int row;
    };

    JSBool SFMatrix3fRow_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool SFMatrix3fRow_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool SFMatrix3fRow_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool SFMatrix3fRow_construct(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval);
    JSObject *SFMatrix3fRow_newInstance( JSContext *cx, Field *field, bool internal_field );

    enum SFMatrix3fRowPropertyId {
    };

    static JSPropertySpec SFMatrix3fRow_properties[] = {
      {0}
    };

    static JSFunctionSpec SFMatrix3fRow_functions[] = {
      {"toString", SFMatrix3fRow_toString , 0, 0, 0 },
      {0}
    };
    static JSClass SFMatrix3fRowClass = {
      "SFMatrix3fRow",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFMatrix3fRow_getProperty, // get property
      SpiderMonkey::SFMatrix3fRow_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      NULL, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    JSBool X3DMatrix3_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool X3DMatrix3_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool X3DMatrix3_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_setTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_getTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_inverse(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_transpose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_multLeft(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_multRight(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_multVecMatrix(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix3_multMatrixVec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSObject *X3DMatrix3_newInstance( JSContext *cx, Field *field, bool internal_field );

    enum X3DMatrix3PropertyId {
    };

    static JSPropertySpec X3DMatrix3_properties[] = {
      {0}
    };

    static JSFunctionSpec X3DMatrix3_functions[] = {
      {"inverse", X3DMatrix3_inverse, 0, 0, 0 },
      {"transpose", X3DMatrix3_transpose,   0, 0, 0 },
      {"setTransform", X3DMatrix3_setTransform,   5, 0, 0 },
      {"getTransform", X3DMatrix3_getTransform,   3, 0, 0 },
      {"multLeft", X3DMatrix3_multLeft,   1, 0, 0 },
      {"multRight", X3DMatrix3_multRight,   1, 0, 0 },
      {"multVecMatrix", X3DMatrix3_multVecMatrix,   1, 0, 0 },
      {"multMatrixVec", X3DMatrix3_multMatrixVec,   1, 0, 0 },
      {"toString", X3DMatrix3_toString , 0, 0, 0 },
      {0}
    };
    
    static JSClass X3DMatrix3Class = {
      "X3DMatrix3",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::X3DMatrix3_getProperty, // get property
      SpiderMonkey::X3DMatrix3_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      X3DMatrix3_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


    //////////////////////////////////////////////
    /// X3DMatrix4 
    ///

    class SFMatrix4fRow : public Field {
    public:
      SFMatrix4fRow() { }
      SFMatrix4fRow(SFMatrix4f* _sfmatrix, int _row) : row(_row), sfmatrix(_sfmatrix) { }
      SFMatrix4f* sfmatrix;
      int row;
    };

    JSBool SFMatrix4fRow_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool SFMatrix4fRow_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool SFMatrix4fRow_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool SFMatrix4fRow_construct(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval);
    JSObject *SFMatrix4fRow_newInstance( JSContext *cx, Field *field, bool internal_field );

    enum SFMatrix4fRowPropertyId {
    };

    static JSPropertySpec SFMatrix4fRow_properties[] = {
      {0}
    };

    static JSFunctionSpec SFMatrix4fRow_functions[] = {
      {"toString", SFMatrix4fRow_toString , 0, 0, 0 },
      {0}
    };
    static JSClass SFMatrix4fRowClass = {
      "SFMatrix4fRow",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFMatrix4fRow_getProperty, // get property
      SpiderMonkey::SFMatrix4fRow_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      NULL, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    JSBool X3DMatrix4_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool X3DMatrix4_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    JSBool X3DMatrix4_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_setTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_getTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_inverse(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_transpose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_multLeft(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_multRight(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_multVecMatrix(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSBool X3DMatrix4_multMatrixVec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    JSObject *X3DMatrix4_newInstance( JSContext *cx, Field *field, bool internal_field );

    enum X3DMatrix4PropertyId {
    };

    static JSPropertySpec X3DMatrix4_properties[] = {
      {0}
    };

    static JSFunctionSpec X3DMatrix4_functions[] = {
      {"inverse", X3DMatrix4_inverse, 0, 0, 0 },
      {"transpose", X3DMatrix4_transpose,   0, 0, 0 },
      {"setTransform", X3DMatrix4_setTransform,   5, 0, 0 },
      {"getTransform", X3DMatrix4_getTransform,   3, 0, 0 },
      {"multLeft", X3DMatrix4_multLeft,   1, 0, 0 },
      {"multRight", X3DMatrix4_multRight,   1, 0, 0 },
      {"multVecMatrix", X3DMatrix4_multVecMatrix,   1, 0, 0 },
      {"multMatrixVec", X3DMatrix4_multMatrixVec,   1, 0, 0 },
      {"toString", X3DMatrix4_toString , 0, 0, 0 },
      {0}
    };
    
    static JSClass X3DMatrix4Class = {
      "X3DMatrix4",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::X3DMatrix4_getProperty, // get property
      SpiderMonkey::X3DMatrix4_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      X3DMatrix4_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


    //////////////////////////////////////////////
    /// SFVec3d 
    ///

    enum SFVec3dPropertyId {
      SFVEC3D_X, SFVEC3D_Y, SFVEC3D_Z
    };

    JSBool SFVec3d_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);


    /// Returns a new SFVec3d object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec3d_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFVec3d
    /// object.
    JSBool SFVec3d_setProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Callback getter function for properties of a SFVec3d
    /// object.
    JSBool SFVec3d_getProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFVec3d.
    JSBool SFVec3d_construct(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    
    // member functions
    JSBool SFVec3d_add(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec3d_cross(JSContext *cx, JSObject *obj, 
                         uintN argc, jsval *argv,
                         jsval *rval);
    JSBool SFVec3d_divide(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec3d_dot(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec3d_length(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec3d_multiple(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    JSBool SFVec3d_negate(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec3d_normalize(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    JSBool SFVec3d_subtract(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    
    // properties
    static JSPropertySpec SFVec3d_properties[] = {
      {"x", SFVEC3D_X, JSPROP_PERMANENT},
      {"y", SFVEC3D_Y, JSPROP_PERMANENT},
      {"z", SFVEC3D_Z, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFVec3d_functions[] = {
      {"add", SFVec3d_add, 1, 0, 0 },
      {"cross", SFVec3d_cross, 1, 0, 0 },
      {"divide", SFVec3d_divide, 1, 0, 0 },
      {"dot", SFVec3d_dot, 1, 0, 0 },
      {"length", SFVec3d_length, 0, 0, 0 },
      {"multiple", SFVec3d_multiple, 1, 0, 0 },
      {"negate", SFVec3d_negate, 0, 0, 0 },
      {"normalize", SFVec3d_normalize, 0, 0, 0 },
      {"subtract", SFVec3d_subtract, 1, 0, 0 },
      {"toString", SFVec3d_toString, 0, 0, 0 },
      {0}
    };

    
    static JSClass SFVec3dClass = {
      "SFVec3d",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFVec3d_getProperty, // get property
      SpiderMonkey::SFVec3d_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFVec3d_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


    //////////////////////////////////////////////
    /// SFVec4f 
    ///

    enum SFVec4fPropertyId {
      SFVEC4F_X, SFVEC4F_Y, SFVEC4F_Z, SFVEC4F_W,
    };


        JSBool SFVec4f_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);


    /// Returns a new SFVec4f object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec4f_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFVec4f
    /// object.
    JSBool SFVec4f_setProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Callback getter function for properties of a SFVec4f
    /// object.
    JSBool SFVec4f_getProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFVec4f.
    JSBool SFVec4f_construct(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    
    // member functions
    JSBool SFVec4f_add(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec4f_divide(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec4f_dot(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec4f_multiple(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    JSBool SFVec4f_negate(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec4f_subtract(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    
    // properties
    static JSPropertySpec SFVec4f_properties[] = {
      {"x", SFVEC4F_X, JSPROP_PERMANENT},
      {"y", SFVEC4F_Y, JSPROP_PERMANENT},
      {"z", SFVEC4F_Z, JSPROP_PERMANENT},
      {"w", SFVEC4F_W, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFVec4f_functions[] = {
      {"add", SFVec4f_add, 1, 0, 0 },
      {"divide", SFVec4f_divide, 1, 0, 0 },
      {"dot", SFVec4f_dot, 1, 0, 0 },
      {"multiple", SFVec4f_multiple, 1, 0, 0 },
      {"negate", SFVec4f_negate, 0, 0, 0 },
      {"subtract", SFVec4f_subtract, 1, 0, 0 },
      {"toString", SFVec4f_toString, 0, 0, 0 },
      {0}
    };

    
    static JSClass SFVec4fClass = {
      "SFVec4f",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFVec4f_getProperty, // get property
      SpiderMonkey::SFVec4f_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFVec4f_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };

    //////////////////////////////////////////////
    /// SFVec4d 
    ///

    enum SFVec4dPropertyId {
      SFVEC4D_X, SFVEC4D_Y, SFVEC4D_Z, SFVEC4D_W,
    };

    JSBool SFVec4d_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);

    /// Returns a new SFVec4d object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFVec4d_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFVec4d
    /// object.
    JSBool SFVec4d_setProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Callback getter function for properties of a SFVec4d
    /// object.
    JSBool SFVec4d_getProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFVec4d.
    JSBool SFVec4d_construct(JSContext *cx, JSObject *obj, 
                             uintN argc, jsval *argv,
                             jsval *rval);
    
    // member functions
    JSBool SFVec4d_add(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec4d_divide(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec4d_dot(JSContext *cx, JSObject *obj, 
                       uintN argc, jsval *argv,
                       jsval *rval);
    JSBool SFVec4d_multiple(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    JSBool SFVec4d_negate(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);
    JSBool SFVec4d_subtract(JSContext *cx, JSObject *obj, 
                            uintN argc, jsval *argv,
                            jsval *rval);
    
    // properties
    static JSPropertySpec SFVec4d_properties[] = {
      {"x", SFVEC4D_X, JSPROP_PERMANENT},
      {"y", SFVEC4D_Y, JSPROP_PERMANENT},
      {"z", SFVEC4D_Z, JSPROP_PERMANENT},
      {"w", SFVEC4D_W, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFVec4d_functions[] = {
      {"add", SFVec4d_add, 1, 0, 0 },
      {"divide", SFVec4d_divide, 1, 0, 0 },
      {"dot", SFVec4d_dot, 1, 0, 0 },
      {"multiple", SFVec4d_multiple, 1, 0, 0 },
      {"negate", SFVec4d_negate, 0, 0, 0 },
      {"subtract", SFVec4d_subtract, 1, 0, 0 },
      {"toString", SFVec4d_toString, 0, 0, 0 },
      {0}
    };

    
    static JSClass SFVec4dClass = {
      "SFVec4d",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFVec4d_getProperty, // get property
      SpiderMonkey::SFVec4d_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFVec4d_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


    //////////////////////////////////////////////
    /// SFRotation
    ///

    enum SFRotationPropertyId {
      SFRotation_X, SFRotation_Y, SFRotation_Z, SFRotation_ANGLE,
    };

    JSBool SFRotation_toString(JSContext *cx, JSObject *obj, 
                          uintN argc, jsval *argv,
                          jsval *rval);

    /// Returns a new SFRotation object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFRotation_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index = -1 );
    
    /// Callback setter function for properties of a SFRotation
    /// object.
    JSBool SFRotation_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    
    /// Callback getter function for properties of a SFRotation
    /// object.
    JSBool SFRotation_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    
    /// Construct callback function for creating a new instance
    /// of SFRotation.
    JSBool SFRotation_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval);
    
    // member functions
    JSBool SFRotation_getAxis( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval );
    JSBool SFRotation_inverse( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval );
    JSBool SFRotation_multiply( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval );
    JSBool SFRotation_multiVec( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval );
    JSBool SFRotation_setAxis( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval );
    JSBool SFRotation_slerp( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval );

    // properties
    static JSPropertySpec SFRotation_properties[] = {
      {"x", SFRotation_X, JSPROP_PERMANENT},
      {"y", SFRotation_Y, JSPROP_PERMANENT},
      {"z", SFRotation_Z, JSPROP_PERMANENT},
      {"angle", SFRotation_ANGLE, JSPROP_PERMANENT},
      {0}
    };

    static JSFunctionSpec SFRotation_functions[] = {
      {"getAxis", SFRotation_getAxis, 0, 0, 0 },
      {"inverse", SFRotation_inverse, 0, 0, 0 },
      {"multiply", SFRotation_multiply, 1, 0, 0 },
      {"multiVec", SFRotation_multiVec, 1, 0, 0 },
      {"setAxis", SFRotation_setAxis, 1, 0, 0 },
      {"slerp", SFRotation_slerp, 2, 0, 0 },
      {"toString", SFRotation_toString, 0, 0, 0 },
      {0}
    };

    static JSClass SFRotationClass = {
      "SFRotation",
      JSCLASS_HAS_PRIVATE,

      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      SpiderMonkey::SFRotation_getProperty, // get property
      SpiderMonkey::SFRotation_setProperty,  // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
      NULL, // getObjectOps
      NULL, // checkAccess
      NULL, // call
      SFRotation_construct, // construct
      NULL, // xdrObject
      NULL, // hasInstance
      NULL, // mark
      NULL //reserveSlots
    };


    //////////////////////////////////////////////
    /// SFNode 
    ///


    /// Returns a new SFNode object encapsulating a field.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFNode_newInstance( JSContext *cx,
                                  Field *field,
                                  bool internal_field, int array_index = -1 );

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
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    JSObject *SFColor_newInstance( JSContext *cx,
                                   Field *field,
                                   bool internal_field, int array_index = -1 );

    /// Callback setter function for properties of a SFColor
    /// object.
    JSBool SFColor_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

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

    ////////////////////////////////////////////////
    ///// SFImage 
    /////

    //struct ImagePrivate {
    //  // Preclusion: sizeof(arr_v) = x * y
    //  ImagePrivate(int x, int y, int comp, const vector<int>& arr_v) {
    //    values.reserve(x * y + 3);
    //    values.push_back(x);
    //    values.push_back(y);
    //    values.push_back(comp);
    //    for ( int i = 0; i < x * y; ++i) {
    //      values.push_back( arr_v[i] );
    //    }
    //  }
    //  const int x() { return values[0]; };
    //  const int y() { return values[1]; };
    //  const int comp() { return values[2]; };
    //  const vector<int>& arr() { return values; }

    //  void setComp(int comp) { values[2] = comp; }

    //  // resize the dimension (fill the gap with zeros if needed)
    //  void setX(int newx) {
    //    values.resize(3 + newx * y(), 0);
    //    values[0] = newx;
    //  }

    //  // resize the dimension (fill the gap with zeros if needed)
    //  void setY(int newy) {
    //    values.resize(3 + x() * newy, 0);
    //    values[1] = newy;
    //  }

    //  // Preclusion: arr.size = arr[0] * arr[1] + 3;
    //  void setArray(const vector<int>& arr) {
    //    values = arr;
    //  }

    //  ImagePrivate() { }

    //private:
    //  // x, y, comp, values...
    //  vector<int> values;
    //};

    //inline ostream& operator<<(ostream& os, const ImagePrivate& ip) {
    //  return os;
    //}

    //class SFImagePrivate: public SField< ImagePrivate > {
    //public:
    //  SFImagePrivate() {}
    //  SFImagePrivate( const ImagePrivate &_value ): SField< ImagePrivate >( _value ){}

    //  /// Get the value of the field as a string.
    //  inline virtual string getValueAsString( const string& separator = " " ) {
    //    ImagePrivate ip = getValue();
    //    stringstream ss;
    //    ss<< ip.x() << " " << ip.y() << " " << ip.comp();
    //    for (int i = 0; i < ip.x() * ip.y(); ++i) {
    //      ss<< " " << ip.arr()[i + 3];
    //    }
    //    return ss.str();
    //  }

    //};

    //enum SFImagePropertyId {
    //  SFImage_WIDTH, SFImage_HEIGHT, SFImage_COMP, SFImage_ARRAY
    //};

    ///// Returns a new SFImage object encapsulating a field.
    ///// \param cx The context in which to create the object.
    ///// \param field The field to encapsulate.
    ///// \param internal_field If true, the encapsulated field
    ///// will be deleted upon destruction of the JSObject 
    ///// encapsulating it.
    //JSObject *SFImage_newInstance( JSContext *cx, SFImagePrivate *field, bool internal_field, int array_index = -1 );

    ///// Callback setter function for properties of a SFImage
    ///// object.
    //JSBool SFImage_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

    ///// Callback getter function for properties of a SFImage
    ///// object.
    //JSBool SFImage_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

    ///// Construct callback function for creating a new instance
    ///// of SFImage.
    //JSBool SFImage_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

    //// properties
    //static JSPropertySpec SFImage_properties[] = {
    //  {"width", SFImage_WIDTH, JSPROP_PERMANENT},
    //  {"height", SFImage_HEIGHT, JSPROP_PERMANENT},
    //  {"comp", SFImage_COMP, JSPROP_PERMANENT},
    //  {"array", SFImage_ARRAY, JSPROP_PERMANENT},
    //  {0}
    //};

    //static JSFunctionSpec SFImage_functions[] = {
    //  {"toString", FieldObject_toString, 0, 0, 0 },
    //  {0}
    //};
    //
    //static JSClass SFImageClass = {
    //  "SFImage",
    //  JSCLASS_HAS_PRIVATE,
    //  
    //  /* All of these can be replaced with the corresponding JS_*Stub
    //     function pointers. */
    //  JS_PropertyStub,  // add property
    //  JS_PropertyStub,  // del property
    //  SpiderMonkey::SFImage_getProperty, // get property
    //  SpiderMonkey::SFImage_setProperty,  // set property
    //  JS_EnumerateStub, // enumerate
    //  JS_ResolveStub,   // resolve
    //  JS_ConvertStub,   // convert
    //  PrivatePointer_finalize< FieldObjectPrivate >,  // finalize
    //  NULL, // getObjectOps
    //  NULL, // checkAccess
    //  NULL, // call
    //  SFImage_construct, // construct
    //  NULL, // xdrObject
    //  NULL, // hasInstance
    //  NULL, // mark
    //  NULL //reserveSlots
    //};

    ////class MFImage: public MField< ImagePrivate > {
    ////public:
    ////  MFImage(){}
    ////  MFImage( size_type sz ): MField< ImagePrivate >( sz ) { }
    ////};

    //////////////////////////////////////////////
    /// X3DExecutionContext 
    ///

    enum X3DExecutionContextPropertyId {
      X3DEXECUTIONCONTEXT_ROOTNODES
    };

    /// Returns a new X3DExecutionContext object encapsulating a 
    /// SAI::ExecutionContext object.
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
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
      {"createNode", X3DExecutionContext_createNode, 1, 0, 0 },
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
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
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
      {"createNode", X3DExecutionContext_createNode, 1, 0, 0 },
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
    /// Browser 
    ///

    JSBool Browser_getProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);

    JSBool Browser_setProperty(JSContext *cx, 
                               JSObject *obj, 
                               jsval id, 
                               jsval *vp);

    JSObject *Browser_newInstance( JSContext *cx,
                                   SAI::Browser *browser,
                                   bool internal_field );

    // Functions
    JSBool Browser_print(JSContext *cx, 
                         JSObject *obj, uintN argc, 
                         jsval *argv, jsval *rval);

    JSBool Browser_println(JSContext *cx, 
                           JSObject *obj, uintN argc, 
                           jsval *argv, jsval *rval);
    
    JSBool Browser_createX3DFromString(JSContext *cx, 
                                       JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval);

    JSBool Browser_createX3DFromURL(JSContext *cx, 
                                    JSObject *obj, uintN argc, 
                                    jsval *argv, jsval *rval);

    enum BrowserPropertyId {
      BROWSER_NAME, BROWSER_VERSION, BROWSER_CURRENT_SCENE
    };
    
    static JSPropertySpec browser_properties[] = {
      {"version",      BROWSER_VERSION,       JSPROP_READONLY},
      {"name",         BROWSER_NAME,          JSPROP_READONLY},
      {"currentScene", BROWSER_CURRENT_SCENE, JSPROP_READONLY},
      {0}
    };

    static JSFunctionSpec browser_functions[] = {
      {"println",   Browser_println,   0, 0, 0 },
      {"print",   Browser_print,   0, 0, 0 },
      {"createX3DFromString",   Browser_createX3DFromString,   0, 0, 0 },
      {"createX3DFromURL",   Browser_createX3DFromURL,   0, 0, 0 },
      {0}
    };

    static JSClass BrowserClass = {
      "Browser",
      JSCLASS_HAS_PRIVATE,
      
      /* All of these can be replaced with the corresponding JS_*Stub
         function pointers. */
      JS_PropertyStub,  // add property
      JS_PropertyStub,  // del property
      Browser_getProperty, // get property
      Browser_setProperty, // set property
      JS_EnumerateStub, // enumerate
      JS_ResolveStub,   // resolve
      JS_ConvertStub,   // convert
      PrivatePointer_finalize<BrowserPrivate>,  // finalize
      JSCLASS_NO_OPTIONAL_MEMBERS
    };

    //////////////////////////////////////////////
    /// X3DMatrix3 
    ///

    //JSBool X3DMatrix3_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    //JSBool X3DMatrix3_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    //JSObject* X3DMatrix3_newInstance( JSContext *cx, Field *field, bool internal_field);

    //// Members Functions
    //JSBool X3DMatrix3_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_setTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_getTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_inverse(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_transpose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_multLeft(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_multRight(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_multVecMatrix(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_multMatrixVec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
    //JSBool X3DMatrix3_length(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

    //enum X3DMatrix3PropertyId {
    //  X3DMatrix3_ZZ
    //};
    //
    //static JSPropertySpec X3DMatrix3_properties[] = {
    //  {"zz", X3DMatrix3_ZZ, JSPROP_PERMANENT},
    //  {0}
    //};

    //static JSFunctionSpec X3DMatrix3_functions[] = {
    //  {"length", X3DMatrix3_length, 0, 0, 0 },
    //  {"inverse", X3DMatrix3_inverse, 0, 0, 0 },
    //  {"transpose", X3DMatrix3_transpose,   0, 0, 0 },
    //  {"setTransform", X3DMatrix3_setTransform,   5, 0, 0 },
    //  {"getTransform", X3DMatrix3_getTransform,   3, 0, 0 },
    //  {"multLeft", X3DMatrix3_multLeft,   1, 0, 0 },
    //  {"multRight", X3DMatrix3_multRight,   1, 0, 0 },
    //  {"multVecMatrix", X3DMatrix3_multVecMatrix,   1, 0, 0 },
    //  {"multMatrixVec", X3DMatrix3_multMatrixVec,   1, 0, 0 },
    //  {"toString", X3DMatrix3_toString , 0, 0, 0 },
    //  {0}
    //};

    //static JSClass X3DMatrix3Class = {
    //  "X3DMatrix3",
    //  JSCLASS_HAS_PRIVATE,

    //  /* All of these can be replaced with the corresponding JS_*Stub
    //     function pointers. */
    //  JS_PropertyStub,  // add property
    //  JS_PropertyStub,  // del property
    //  X3DMatrix3_getProperty, // get property
    //  X3DMatrix3_setProperty,  // set property
    //  JS_EnumerateStub, // enumerate
    //  JS_ResolveStub,   // resolve
    //  JS_ConvertStub,   // convert
    //  PrivatePointer_finalize<FieldObjectPrivate>,  // finalize
    //  NULL, // getObjectOps
    //  NULL, // checkAccess
    //  NULL, // call
    //  X3DMatrix3_construct, // construct
    //  NULL, // xdrObject
    //  NULL, // hasInstance
    //  NULL, // mark
    //  NULL //reserveSlots
    //};

    //////////////////////////////////////////////
    /// MField template 
    ///
    template< class MFieldType, class ElementType > 
    struct JS_MField {

      /// Returns a new JSObject object encapsulating a field.
      /// \param cx The context in which to create the object.
      /// \param field The field to encapsulate.
      /// \param internal_field If true, the encapsulated field
      /// will be deleted upon destruction of the JSObject 
      /// encapsulating it.
      static JSObject *newInstance( JSContext *cx,
                                    //MFieldType *field,
                                    Field *field,
                                    bool internal_field, int array_index = -1 );

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
    /// \param cx The context in which to create the object.
    /// \param field The field to encapsulate.
    /// \param internal_field If true, the encapsulated field
    /// will be deleted upon destruction of the JSObject 
    /// encapsulating it.
    /// \param array_index Index into MF type.
    template< class MFieldType, class ElementType >
    JSObject *JS_MField< MFieldType, ElementType >::newInstance( JSContext *cx,
                                                                 //MFieldType *field,
                                                                 Field *field,
                                                                 bool internal_field,
                                                                 int array_index
                                                                 ) {
      //      cerr << "MFNode newInstance: " << field->getName() << endl;
      JSObject *js_field;
      
      js_field = JS_NewObject( cx, &js_class, NULL, NULL );  
      
      JS_DefineProperties(cx, js_field, 
                          JS_MField< MFieldType, ElementType >::properties );
      JS_DefineFunctions(cx, js_field, 
                         JS_MField< MFieldType, ElementType >::functions );
      JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                                                                   internal_field ) );
      return js_field;
    }
    

    /// The JSAPI type encapsulating an MFFloat object.
    typedef JS_MField< MFFloat,  SFFloat  > JS_MFFloat;

    /// The JSAPI type encapsulating an MFDouble object.
    typedef JS_MField< MFDouble, SFDouble > JS_MFDouble;
    
    /// The JSAPI type encapsulating an MFTime object.
    typedef JS_MField< MFTime,   SFTime   > JS_MFTime;
    
    /// The JSAPI type encapsulating an MFInt32 object.
    typedef JS_MField< MFInt32,  SFInt32  > JS_MFInt32;

    /// The JSAPI type encapsulating an MFVec2f object.
    typedef JS_MField< MFVec2f,  SFVec2f  > JS_MFVec2f;

    /// The JSAPI type encapsulating an MFVec2d object.
    typedef JS_MField< MFVec2d,  SFVec2d  > JS_MFVec2d;

    /// The JSAPI type encapsulating an MFVec3f object.
    typedef JS_MField< MFVec3f,  SFVec3f  > JS_MFVec3f;

    /// The JSAPI type encapsulating an MFVec3f object.
    typedef JS_MField< MFVec3d,  SFVec3d  > JS_MFVec3d;

    /// The JSAPI type encapsulating an MFVec4f object.
    typedef JS_MField< MFVec4f,  SFVec4f  > JS_MFVec4f;

    /// The JSAPI type encapsulating an MFVec4d object.
    typedef JS_MField< MFVec4d,  SFVec4d  > JS_MFVec4d;

    /// The JSAPI type encapsulating an MFBool object.
    typedef JS_MField< MFBool,   SFBool   > JS_MFBool;

    /// The JSAPI type encapsulating an MFString object.
    typedef JS_MField< MFString, SFString > JS_MFString;

    /// The JSAPI type encapsulating an MFNode object.
    typedef JS_MField< MFNode,   SFNode   > JS_MFNode;

    /// The JSAPI type encapsulating an MFColor object.
    typedef JS_MField< MFColor,  SFColor > JS_MFColor;

    /// The JSAPI type encapsulating an MFRotation object.
    typedef JS_MField< MFRotation,  SFRotation > JS_MFRotation;

    ///// The JSAPI type encapsulating an MFImage object.
    //typedef JS_MField< MFImage,  SFImage > JS_MFImage;

    typedef enum { O_ADD, O_SUBTRACT, O_CROSS, O_DOT, O_DIVIDE, O_MULTIPLE, } BinaryOperator;

    // get the private pointer from a JSObject
    template<class T>
    T* helper_extractPrivateObject( JSContext* cx, JSObject* jsobj);

    // templates for binary-operator functions

    template <class T>
    bool helper_calculate2_TTT(JSContext *cx, JSObject *this_obj, jsval *arg, T* const rval, BinaryOperator op);
    template <class T, class N>
    bool helper_calculate2_TNT(JSContext *cx, JSObject *this_obj, jsval *arg, T* const rval, BinaryOperator op);
    template <class T, class N>
    bool helper_calculate2_TTN(JSContext *cx, JSObject *this_obj, jsval *arg, jsval* const rjsval, BinaryOperator op);

    /// constant to help sanity check function
    static JSClass SFNumberClass = {
      "SFNumber",
      JSCLASS_HAS_PRIVATE,
      JS_PropertyStub, JS_PropertyStub, 
      JS_PropertyStub, JS_PropertyStub, 
      JS_EnumerateStub, JS_ResolveStub,  
      JS_ConvertStub, JS_FinalizeStub,
      NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };

    // sanity check the parameters against a particular set
    bool helper_sanityCheck(JSContext *cx, JSObject *obj, uintN argc, 
      jsval *argv, uintN e_argc, JSClass** e_argtypes);
  }
}

#endif // HAVE_SPIDERMONKEY

#endif
