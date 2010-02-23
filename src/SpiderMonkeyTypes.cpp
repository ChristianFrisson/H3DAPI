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
/// \file SpiderMonkeyTypes.cpp
/// \brief Source file for SAI ECMAScript implementation using the SpiderMonkey
/// javascript engine.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SpiderMonkeyTypes.h>
#include <H3D/Field.h>
#include <H3D/X3DTypes.h>
#include <H3D/X3DFieldConversion.h>
#include <H3D/X3DTypeFunctions.h>
#include <H3D/SAIFunctions.h>
#include <H3D/X3D.h>
#include<typeinfo>

#include<sstream>



#ifdef HAVE_SPIDERMONKEY

using namespace H3D;
using namespace H3D::SpiderMonkey;

template< class FieldType >
const typename FieldType::value_type &getValueNoAccessCheck( FieldType *field ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  const typename FieldType::value_type &b = field->getValue();
  field->setAccessCheck( access );
  return b;
}

template< class FieldType >
const typename FieldType::vector_return_type &MField_getValueNoAccessCheck( FieldType *field ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  const typename FieldType::vector_return_type &b = field->getValue();
  field->setAccessCheck( access );
  return b;
}

template< class FieldType >
void setValueNoAccessCheck( FieldType *field, 
			    const typename FieldType::value_type &v ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  field->setValue( v);
  field->setAccessCheck( access );
}

template< class FieldType >
void MField_setValueNoAccessCheck( FieldType *field, 
				   const typename FieldType::vector_return_type &v ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  field->setValue( v);
  field->setAccessCheck( access );
}

JSBool SpiderMonkey::FieldObject_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {

  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  Field* this_field = static_cast<Field *>(private_data->getPointer());
  
   // make sure we have a value
  if (!this_field ) return JS_FALSE;
  
  ParsableField *pfield = dynamic_cast< ParsableField * >( this_field );
  if( !pfield ) return JSVAL_FALSE;
  
  // return string representation of the Vec3f
  string s = pfield->getValueAsString();
  
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)s.c_str(), s.length() ) );
  return JS_TRUE;
} 


//=======================================================
//=======================================================
// SFVEC2F
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFVec2f_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 2 || 
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  Vec2f v( 0, 0 );
  if( argc > 0 ) {
    jsdouble x;
    if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
    v.x = (H3DFloat) x;
  }
  if( argc > 1 ) {
    jsdouble y;
    if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
    v.y = (H3DFloat) y;
  }
  // create return value
  SFVec2f *return_f = new SFVec2f(v);
  *rval = OBJECT_TO_JSVAL( SFVec2f_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec2fClass, &SFVec2fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec2f* rp = new SFVec2f();
  if ( !helper_calculate2_TTT<SFVec2f>( cx, obj, &argv[0], rp, O_ADD )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2f_newInstance( cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_subtract(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec2fClass, &SFVec2fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec2f* r_sfv3f = new SFVec2f();
  if ( !helper_calculate2_TTT<SFVec2f>( cx, obj, &argv[0], r_sfv3f, O_SUBTRACT )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2f_newInstance( cx, r_sfv3f, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_divide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec2fClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2f* rp = new SFVec2f();
  if ( !helper_calculate2_TNT<SFVec2f, H3DFloat>( cx, obj, &argv[0], rp, O_DIVIDE )) {
    return JS_FALSE;
  }

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2f_newInstance(cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_dot(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type SFVec2f
  JSClass* type_classes[] = { &SFVec2fClass, &SFVec2fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  // calculate and return result
  if ( !helper_calculate2_TTN<SFVec2f, H3DFloat>( cx, obj, &argv[0], rval, O_DOT )) {
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_length(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec2fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2f* v = helper_extractPrivateObject<SFVec2f>( cx, obj );

  if ( !JS_NewNumberValue(cx, v->getValue().length(), rval) )
    return JS_FALSE; 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_multiple(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec2fClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2f* rp = new SFVec2f();
  if ( !helper_calculate2_TNT<SFVec2f, H3DFloat>( cx, obj, &argv[0], rp, O_MULTIPLE )) {
    return JS_FALSE;
  }

  *rval = OBJECT_TO_JSVAL(SFVec2f_newInstance(cx, rp, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_negate(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec2fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  // extract old value and calculate new value
  SFVec2f* oldv = helper_extractPrivateObject<SFVec2f>(cx, obj);
  SFVec2f* v = new SFVec2f( - oldv->getValue() );

  *rval = OBJECT_TO_JSVAL(SFVec2f_newInstance(cx, v , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_normalize(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec2fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2f* oldv = helper_extractPrivateObject<SFVec2f>( cx, obj );
  Vec2f value = oldv->getValue(); // copy constructor
  value.normalizeSafe();
  SFVec2f* newv = new SFVec2f(value);

  *rval = OBJECT_TO_JSVAL(SFVec2f_newInstance(cx, newv , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2f_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property SFVec2f" << endl;
  if (JSVAL_IS_INT(id)) {
    SFVec2f* this_vec3f = static_cast<SFVec2f *>(private_data->getPointer());
    const Vec2f &v = getValueNoAccessCheck( this_vec3f );

    switch (JSVAL_TO_INT(id)) {
    case SFVEC2F_X: {
      JS_NewNumberValue( cx, v.x, vp ); 
      break;
    }
    case SFVEC2F_Y: {
      JS_NewNumberValue( cx, v.y, vp ); 
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFVec2f_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if (JSVAL_IS_INT(id)) {
    SFVec2f* this_vec3f = static_cast<SFVec2f *>(private_data->getPointer());
    Vec2f v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC2F_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DFloat) x;
      break;
    }
    case SFVEC2F_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DFloat) y;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec2f attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec2f_newInstance( JSContext *cx, SFVec2f *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFVec2fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec2f_properties );
  JS_DefineFunctions(cx, js_field, SFVec2f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
  return js_field; 
}



//=======================================================
//=======================================================
// SFVEC2D
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFVec2d_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 2 || 
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  Vec2d v( 0, 0 );
  if( argc > 0 ) {
    jsdouble x;
    if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
    v.x = (H3DDouble) x;
  }
  if( argc > 1 ) {
    jsdouble y;
    if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
    v.y = (H3DDouble) y;
  }
  // create return value
  SFVec2d *return_f = new SFVec2d(v);
  *rval = OBJECT_TO_JSVAL( SFVec2d_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec2dClass, &SFVec2dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec2d* rp = new SFVec2d();
  if ( !helper_calculate2_TTT<SFVec2d>( cx, obj, &argv[0], rp, O_ADD )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2d_newInstance( cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_subtract(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec2dClass, &SFVec2dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec2d* r_sfv3f = new SFVec2d();
  if ( !helper_calculate2_TTT<SFVec2d>( cx, obj, &argv[0], r_sfv3f, O_SUBTRACT )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2d_newInstance( cx, r_sfv3f, true ) );
  return JS_TRUE;
}


JSBool SpiderMonkey::SFVec2d_divide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec2dClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2d* rp = new SFVec2d();
  if ( !helper_calculate2_TNT<SFVec2d, H3DDouble>( cx, obj, &argv[0], rp, O_DIVIDE )) {
    return JS_FALSE;
  }

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2d_newInstance(cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_dot(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type SFVec2d
  JSClass* type_classes[] = { &SFVec2dClass, &SFVec2dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  // calculate and return result
  if ( !helper_calculate2_TTN<SFVec2d, H3DDouble>( cx, obj, &argv[0], rval, O_DOT )) {
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_length(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec2dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2d* v = helper_extractPrivateObject<SFVec2d>( cx, obj );

  if ( !JS_NewNumberValue(cx, v->getValue().length(), rval) )
    return JS_FALSE; 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_multiple(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec2dClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2d* rp = new SFVec2d();
  if ( !helper_calculate2_TNT<SFVec2d, H3DDouble>( cx, obj, &argv[0], rp, O_MULTIPLE )) {
    return JS_FALSE;
  }

  *rval = OBJECT_TO_JSVAL(SFVec2d_newInstance(cx, rp, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_negate(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec2dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  // extract old value and calculate new value
  SFVec2d* oldv = helper_extractPrivateObject<SFVec2d>(cx, obj);
  SFVec2d* v = new SFVec2d( - oldv->getValue() );

  *rval = OBJECT_TO_JSVAL(SFVec2d_newInstance(cx, v , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_normalize(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec2dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec2d* oldv = helper_extractPrivateObject<SFVec2d>( cx, obj );
  Vec2d value = oldv->getValue(); // copy constructor
  value.normalizeSafe();
  SFVec2d* newv = new SFVec2d(value);

  *rval = OBJECT_TO_JSVAL(SFVec2d_newInstance(cx, newv , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec2d_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property SFVec2d" << endl;
  if (JSVAL_IS_INT(id)) {
    SFVec2d* this_vec3f = static_cast<SFVec2d *>(private_data->getPointer());
    const Vec2d &v = getValueNoAccessCheck( this_vec3f );

    switch (JSVAL_TO_INT(id)) {
    case SFVEC2D_X: {
      JS_NewNumberValue( cx, v.x, vp ); 
      break;
    }
    case SFVEC2D_Y: {
      JS_NewNumberValue( cx, v.y, vp ); 
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFVec2d_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if (JSVAL_IS_INT(id)) {
    SFVec2d* this_vec3f = static_cast<SFVec2d *>(private_data->getPointer());
    Vec2d v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC2D_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DDouble) x;
      break;
    }
    case SFVEC2D_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DDouble) y;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec2d attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec2d_newInstance( JSContext *cx, SFVec2d *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &SFVec2dClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec2d_properties );
  JS_DefineFunctions(cx, js_field, SFVec2d_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
  return js_field; 
}


//=======================================================
//=======================================================
// SFVEC3F
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFVec3f_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 3 || 
    ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  Vec3f v( 0, 0, 0 );
  if( argc > 0 ) {
    jsdouble x;
    if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
    v.x = (H3DFloat) x;
  }
  if( argc > 1 ) {
    jsdouble y;
    if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
    v.y = (H3DFloat) y;
  }
  if( argc > 2 ) {
    jsdouble z;
    if(!JS_ValueToNumber( cx, argv[2], &z) ) return JS_FALSE;
    v.z = (H3DFloat) z;
  }
  // create return value
  SFVec3f *return_f = new SFVec3f(v);
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec3fClass, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec3f* rp = new SFVec3f();
  if ( !helper_calculate2_TTT<SFVec3f>( cx, obj, &argv[0], rp, O_ADD )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_subtract(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec3fClass, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec3f* r_sfv3f = new SFVec3f();
  if ( !helper_calculate2_TTT<SFVec3f>( cx, obj, &argv[0], r_sfv3f, O_SUBTRACT )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, r_sfv3f, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_cross(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec3fClass, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* sf1 = helper_extractPrivateObject<SFVec3f>( cx, obj );
  SFVec3f* sf2 = helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT(argv[0]) );
  SFVec3f* r_sf = new SFVec3f(sf1->getValue() % sf2->getValue());

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, r_sf, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_divide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec3fClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* rp = new SFVec3f();
  if ( !helper_calculate2_TNT<SFVec3f, H3DFloat>( cx, obj, &argv[0], rp, O_DIVIDE )) {
    return JS_FALSE;
  }

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance(cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_dot(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type SFVec3f
  JSClass* type_classes[] = { &SFVec3fClass, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  // calculate and return result
  if ( !helper_calculate2_TTN<SFVec3f, H3DFloat>( cx, obj, &argv[0], rval, O_DOT )) {
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_length(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* v = helper_extractPrivateObject<SFVec3f>( cx, obj );

  if ( !JS_NewNumberValue(cx, v->getValue().length(), rval) )
    return JS_FALSE; 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_multiple(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec3fClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* rp = new SFVec3f();
  if ( !helper_calculate2_TNT<SFVec3f, H3DFloat>( cx, obj, &argv[0], rp, O_MULTIPLE )) {
    return JS_FALSE;
  }

  *rval = OBJECT_TO_JSVAL(SFVec3f_newInstance(cx, rp, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_negate(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  // extract old value and calculate new value
  SFVec3f* oldv = helper_extractPrivateObject<SFVec3f>(cx, obj);
  SFVec3f* v = new SFVec3f( - oldv->getValue() );

  *rval = OBJECT_TO_JSVAL(SFVec3f_newInstance(cx, v , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_normalize(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* oldv = helper_extractPrivateObject<SFVec3f>( cx, obj );
  Vec3f value = oldv->getValue(); // copy constructor
  value.normalizeSafe();
  SFVec3f* newv = new SFVec3f(value);

  *rval = OBJECT_TO_JSVAL(SFVec3f_newInstance(cx, newv , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property SFVec3f" << endl;
  if (JSVAL_IS_INT(id)) {
    SFVec3f* this_vec3f = static_cast<SFVec3f *>(private_data->getPointer());
    const Vec3f &v = getValueNoAccessCheck( this_vec3f );

    switch (JSVAL_TO_INT(id)) {
    case SFVEC3F_X: {
      JS_NewNumberValue( cx, v.x, vp ); 
      break;
    }
    case SFVEC3F_Y: {
      JS_NewNumberValue( cx, v.y, vp ); 
      break;
    }
    case SFVEC3F_Z: {
      JS_NewNumberValue( cx, v.z, vp ); 
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFVec3f_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if (JSVAL_IS_INT(id)) {
    SFVec3f* this_vec3f = static_cast<SFVec3f *>(private_data->getPointer());
    Vec3f v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC3F_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DFloat) x;
      break;
    }
    case SFVEC3F_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DFloat) y;
      break;
    }
    case SFVEC3F_Z: {
      jsdouble z;
      res = JS_ValueToNumber( cx, *vp, &z );
      v.z = (H3DFloat) z;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec3f attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec3f_newInstance( JSContext *cx, SFVec3f *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &SFVec3fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec3f_properties );
  JS_DefineFunctions(cx, js_field, SFVec3f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
  return js_field; 
}



//=======================================================
//=======================================================
// SFVEC3D
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFVec3d_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 3 || 
    ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  Vec3d v( 0, 0, 0 );
  if( argc > 0 ) {
    jsdouble x;
    if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
    v.x = (H3DDouble) x;
  }
  if( argc > 1 ) {
    jsdouble y;
    if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
    v.y = (H3DDouble) y;
  }
  if( argc > 2 ) {
    jsdouble z;
    if(!JS_ValueToNumber( cx, argv[2], &z) ) return JS_FALSE;
    v.z = (H3DDouble) z;
  }
  // create return value
  SFVec3d *return_f = new SFVec3d(v);
  *rval = OBJECT_TO_JSVAL( SFVec3d_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec3dClass, &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec3d* rp = new SFVec3d();
  if ( !helper_calculate2_TTT<SFVec3d>( cx, obj, &argv[0], rp, O_ADD )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3d_newInstance( cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_subtract(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec3dClass, &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec3d* r_sfv3f = new SFVec3d();
  if ( !helper_calculate2_TTT<SFVec3d>( cx, obj, &argv[0], r_sfv3f, O_SUBTRACT )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3d_newInstance( cx, r_sfv3f, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_cross(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec3dClass, &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3d* sf1 = helper_extractPrivateObject<SFVec3d>( cx, obj );
  SFVec3d* sf2 = helper_extractPrivateObject<SFVec3d>( cx, JSVAL_TO_OBJECT(argv[0]) );
  SFVec3d* r_sf = new SFVec3d(sf1->getValue() % sf2->getValue());

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3d_newInstance( cx, r_sf, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_divide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec3dClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3d* rp = new SFVec3d();
  if ( !helper_calculate2_TNT<SFVec3d, H3DDouble>( cx, obj, &argv[0], rp, O_DIVIDE )) {
    return JS_FALSE;
  }

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3d_newInstance(cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_dot(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type SFVec3d
  JSClass* type_classes[] = { &SFVec3dClass, &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  // calculate and return result
  if ( !helper_calculate2_TTN<SFVec3d, H3DDouble>( cx, obj, &argv[0], rval, O_DOT )) {
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_length(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3d* v = helper_extractPrivateObject<SFVec3d>( cx, obj );

  if ( !JS_NewNumberValue(cx, v->getValue().length(), rval) )
    return JS_FALSE; 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_multiple(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec3dClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3d* rp = new SFVec3d();
  if ( !helper_calculate2_TNT<SFVec3d, H3DDouble>( cx, obj, &argv[0], rp, O_MULTIPLE )) {
    return JS_FALSE;
  }

  *rval = OBJECT_TO_JSVAL(SFVec3d_newInstance(cx, rp, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_negate(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  // extract old value and calculate new value
  SFVec3d* oldv = helper_extractPrivateObject<SFVec3d>(cx, obj);
  SFVec3d* v = new SFVec3d( - oldv->getValue() );

  *rval = OBJECT_TO_JSVAL(SFVec3d_newInstance(cx, v , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_normalize(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec3dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec3d* oldv = helper_extractPrivateObject<SFVec3d>( cx, obj );
  Vec3d value = oldv->getValue(); // copy constructor
  value.normalizeSafe();
  SFVec3d* newv = new SFVec3d(value);

  *rval = OBJECT_TO_JSVAL(SFVec3d_newInstance(cx, newv , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3d_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property SFVec3d" << endl;
  if (JSVAL_IS_INT(id)) {
    SFVec3d* this_vec3f = static_cast<SFVec3d *>(private_data->getPointer());
    const Vec3d &v = getValueNoAccessCheck( this_vec3f );

    switch (JSVAL_TO_INT(id)) {
    case SFVEC3D_X: {
      JS_NewNumberValue( cx, v.x, vp ); 
      break;
    }
    case SFVEC3D_Y: {
      JS_NewNumberValue( cx, v.y, vp ); 
      break;
    }
    case SFVEC3D_Z: {
      JS_NewNumberValue( cx, v.z, vp ); 
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFVec3d_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if (JSVAL_IS_INT(id)) {
    SFVec3d* this_vec3f = static_cast<SFVec3d *>(private_data->getPointer());
    Vec3d v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC3D_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DDouble) x;
      break;
    }
    case SFVEC3D_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DDouble) y;
      break;
    }
    case SFVEC3D_Z: {
      jsdouble z;
      res = JS_ValueToNumber( cx, *vp, &z );
      v.z = (H3DDouble) z;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec3d attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec3d_newInstance( JSContext *cx, SFVec3d *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &SFVec3dClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec3d_properties );
  JS_DefineFunctions(cx, js_field, SFVec3d_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
  return js_field; 
}



//=======================================================
//=======================================================
// SFVEC4F
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFVec4f_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 4 || 
    ( argc > 3 && !JSVAL_IS_NUMBER( argv[3] ) ) ||
    ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  Vec4f v( 0, 0, 0, 0 );
  if( argc > 0 ) {
    jsdouble x;
    if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
    v.x = (H3DFloat) x;
  }
  if( argc > 1 ) {
    jsdouble y;
    if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
    v.y = (H3DFloat) y;
  }
  if( argc > 2 ) {
    jsdouble z;
    if(!JS_ValueToNumber( cx, argv[2], &z) ) return JS_FALSE;
    v.z = (H3DFloat) z;
  }
  if( argc > 3 ) {
    jsdouble w;
    if(!JS_ValueToNumber( cx, argv[3], &w) ) return JS_FALSE;
    v.w = (H3DFloat) w;
  }
  // create return value
  SFVec4f *return_f = new SFVec4f(v);
  *rval = OBJECT_TO_JSVAL( SFVec4f_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec4fClass, &SFVec4fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec4f* rp = new SFVec4f();
  if ( !helper_calculate2_TTT<SFVec4f>( cx, obj, &argv[0], rp, O_ADD )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4f_newInstance( cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_subtract(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec4fClass, &SFVec4fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec4f* r_sfv3f = new SFVec4f();
  if ( !helper_calculate2_TTT<SFVec4f>( cx, obj, &argv[0], r_sfv3f, O_SUBTRACT )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4f_newInstance( cx, r_sfv3f, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_divide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec4fClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec4f* rp = new SFVec4f();
  if ( !helper_calculate2_TNT<SFVec4f, H3DFloat>( cx, obj, &argv[0], rp, O_DIVIDE )) {
    return JS_FALSE;
  }

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4f_newInstance(cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_dot(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type SFVec4f
  JSClass* type_classes[] = { &SFVec4fClass, &SFVec4fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  // calculate and return result
  if ( !helper_calculate2_TTN<SFVec4f, H3DFloat>( cx, obj, &argv[0], rval, O_DOT )) {
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_multiple(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec4fClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec4f* rp = new SFVec4f();
  if ( !helper_calculate2_TNT<SFVec4f, H3DFloat>( cx, obj, &argv[0], rp, O_MULTIPLE )) {
    return JS_FALSE;
  }

  *rval = OBJECT_TO_JSVAL(SFVec4f_newInstance(cx, rp, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_negate(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec4fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  // extract old value and calculate new value
  SFVec4f* oldv = helper_extractPrivateObject<SFVec4f>(cx, obj);
  SFVec4f* v = new SFVec4f( - oldv->getValue() );

  *rval = OBJECT_TO_JSVAL(SFVec4f_newInstance(cx, v , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4f_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property SFVec4f" << endl;
  if (JSVAL_IS_INT(id)) {
    SFVec4f* this_vec3f = static_cast<SFVec4f *>(private_data->getPointer());
    const Vec4f &v = getValueNoAccessCheck( this_vec3f );

    switch (JSVAL_TO_INT(id)) {
    case SFVEC4F_X: {
      JS_NewNumberValue( cx, v.x, vp ); 
      break;
    }
    case SFVEC4F_Y: {
      JS_NewNumberValue( cx, v.y, vp ); 
      break;
    }
    case SFVEC4F_Z: {
      JS_NewNumberValue( cx, v.z, vp ); 
      break;
    }
    case SFVEC4F_W:
      JS_NewNumberValue( cx, v.w, vp ); 
      break;
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFVec4f_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if (JSVAL_IS_INT(id)) {
    SFVec4f* this_vec3f = static_cast<SFVec4f *>(private_data->getPointer());
    Vec4f v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC4F_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DFloat) x;
      break;
    }
    case SFVEC4F_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DFloat) y;
      break;
    }
    case SFVEC4F_Z: {
      jsdouble z;
      res = JS_ValueToNumber( cx, *vp, &z );
      v.z = (H3DFloat) z;
      break;
    }
    case SFVEC4F_W: {
      jsdouble w;
      res = JS_ValueToNumber( cx, *vp, &w );
      v.w = (H3DFloat) w;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec4f attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec4f_newInstance( JSContext *cx, SFVec4f *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFVec4fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec4f_properties );
  JS_DefineFunctions(cx, js_field, SFVec4f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
  return js_field; 
}

//=======================================================
//=======================================================
// SFVEC4D
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFVec4d_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 4 || 
    ( argc > 3 && !JSVAL_IS_NUMBER( argv[3] ) ) ||
    ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  Vec4d v( 0, 0, 0, 0 );
  if( argc > 0 ) {
    jsdouble x;
    if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
    v.x = (H3DDouble) x;
  }
  if( argc > 1 ) {
    jsdouble y;
    if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
    v.y = (H3DDouble) y;
  }
  if( argc > 2 ) {
    jsdouble z;
    if(!JS_ValueToNumber( cx, argv[2], &z) ) return JS_FALSE;
    v.z = (H3DDouble) z;
  }
  if( argc > 3 ) {
    jsdouble w;
    if(!JS_ValueToNumber( cx, argv[3], &w) ) return JS_FALSE;
    v.w = (H3DDouble) w;
  }
  // create return value
  SFVec4d *return_f = new SFVec4d(v);
  *rval = OBJECT_TO_JSVAL( SFVec4d_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec4dClass, &SFVec4dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec4d* rp = new SFVec4d();
  if ( !helper_calculate2_TTT<SFVec4d>( cx, obj, &argv[0], rp, O_ADD )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4d_newInstance( cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_subtract(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class sfvec3f
  JSClass* type_classes[] = { &SFVec4dClass, &SFVec4dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // calculate the result
  SFVec4d* r_sfv3f = new SFVec4d();
  if ( !helper_calculate2_TTT<SFVec4d>( cx, obj, &argv[0], r_sfv3f, O_SUBTRACT )) {
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4d_newInstance( cx, r_sfv3f, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_divide(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec4dClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec4d* rp = new SFVec4d();
  if ( !helper_calculate2_TNT<SFVec4d, H3DDouble>( cx, obj, &argv[0], rp, O_DIVIDE )) {
    return JS_FALSE;
  }

  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4d_newInstance(cx, rp, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_dot(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type SFVec4d
  JSClass* type_classes[] = { &SFVec4dClass, &SFVec4dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  // calculate and return result
  if ( !helper_calculate2_TTN<SFVec4d, H3DDouble>( cx, obj, &argv[0], rval, O_DOT )) {
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_multiple(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of type number
  JSClass* type_classes[] = { &SFVec4dClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFVec4d* rp = new SFVec4d();
  if ( !helper_calculate2_TNT<SFVec4d, H3DDouble>( cx, obj, &argv[0], rp, O_MULTIPLE )) {
    return JS_FALSE;
  }

  *rval = OBJECT_TO_JSVAL(SFVec4d_newInstance(cx, rp, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_negate(JSContext *cx, JSObject *obj,  uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &SFVec4dClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  // extract old value and calculate new value
  SFVec4d* oldv = helper_extractPrivateObject<SFVec4d>(cx, obj);
  SFVec4d* v = new SFVec4d( - oldv->getValue() );

  *rval = OBJECT_TO_JSVAL(SFVec4d_newInstance(cx, v , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFVec4d_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property SFVec4d" << endl;
  if (JSVAL_IS_INT(id)) {
    SFVec4d* this_vec3f = static_cast<SFVec4d *>(private_data->getPointer());
    const Vec4d &v = getValueNoAccessCheck( this_vec3f );

    switch (JSVAL_TO_INT(id)) {
    case SFVEC4D_X: {
      JS_NewNumberValue( cx, v.x, vp ); 
      break;
    }
    case SFVEC4D_Y: {
      JS_NewNumberValue( cx, v.y, vp ); 
      break;
    }
    case SFVEC4D_Z: {
      JS_NewNumberValue( cx, v.z, vp ); 
      break;
    }
    case SFVEC4D_W: {
      JS_NewNumberValue( cx, v.w, vp ); 
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFVec4d_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if (JSVAL_IS_INT(id)) {
    SFVec4d* this_vec3f = static_cast<SFVec4d *>(private_data->getPointer());
    Vec4d v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC4D_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DDouble) x;
      break;
    }
    case SFVEC4D_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DDouble) y;
      break;
    }
    case SFVEC4D_Z: {
      jsdouble z;
      res = JS_ValueToNumber( cx, *vp, &z );
      v.z = (H3DDouble) z;
      break;
    }
    case SFVEC4D_W: {
      jsdouble w;
      res = JS_ValueToNumber( cx, *vp, &w );
      v.w = (H3DDouble) w;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec4d attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec4d_newInstance( JSContext *cx, SFVec4d *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFVec4dClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec4d_properties );
  JS_DefineFunctions(cx, js_field, SFVec4d_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
  return js_field; 
}



//=======================================================
//=======================================================
// SFRotation
//=======================================================
//=======================================================

JSBool SpiderMonkey::SFRotation_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.

  auto_ptr<Rotation> vp;

  // 2 arguments, (Vec3f, Vec3f) -- 3rd case
  if ( argc == 2 
    && JSVAL_IS_OBJECT(argv[0]) && JS_InstanceOf(cx, JSVAL_TO_OBJECT(argv[0]), &SFVec3fClass, argv)
    && JSVAL_IS_OBJECT(argv[1]) && JS_InstanceOf(cx, JSVAL_TO_OBJECT(argv[1]), &SFVec3fClass, argv) )
  {
    SFVec3f* v1 = helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT( argv[0] ) );
    SFVec3f* v2 = helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT( argv[1] ) );

    if ( H3DAbs(v1->getValue().length()-1) > std::numeric_limits< H3DFloat >::epsilon()
      || H3DAbs(v2->getValue().length()-1) > std::numeric_limits< H3DFloat >::epsilon() )
    {
      JS_ReportError(cx, "Provided vectors are not normalized" );
      return JS_FALSE;
    }
    vp.reset( new Rotation( v1->getValue(), v2->getValue() ) );
  }
  // 2 arguments, (Vec3f, num) -- 2nd case
  else if ( argc == 2
    && JSVAL_IS_OBJECT(argv[0]) && JS_InstanceOf(cx, JSVAL_TO_OBJECT(argv[0]), &SFVec3fClass, argv)
    && JSVAL_IS_NUMBER( argv[1] ) )
  {
    SFVec3f* v = helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT( argv[0] ) );
    jsdouble jsd;
    JS_ValueToNumber(cx, argv[1], &jsd );
    vp.reset( new Rotation( v->getValue(), (H3DFloat)jsd ) );
  }
  // wrong arguments
  else if ( argc > 4 || 
    ( argc > 3 && !JSVAL_IS_NUMBER( argv[3] ) ) ||
    ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
    ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
    ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    JS_ReportError(cx, "Constructor: Invalid number provided.");
    return JS_FALSE;
  }
  else {
    // first case
    vp.reset( new Rotation( 0, 1, 0, 0 ) );
    if( argc > 0 ) {
      jsdouble x;
      if(!JS_ValueToNumber( cx, argv[0], &x) ) return JS_FALSE;
      vp->axis.x = (H3DFloat) x;
    }
    if( argc > 1 ) {
      jsdouble y;
      if(!JS_ValueToNumber( cx, argv[1], &y) ) return JS_FALSE;
      vp->axis.y = (H3DFloat) y;
    }
    if( argc > 2 ) {
      jsdouble z;
      if(!JS_ValueToNumber( cx, argv[2], &z) ) return JS_FALSE;
      vp->axis.z = (H3DFloat) z;
    }
    if( argc > 3 ) {
      jsdouble angle;
      if(!JS_ValueToNumber( cx, argv[3], &angle) ) return JS_FALSE;
      vp->angle = (H3DFloat) angle;
    }
  }

  // create return value
  SFRotation *return_f = new SFRotation( *vp );
  *rval = OBJECT_TO_JSVAL( SFRotation_newInstance( cx, return_f, true ) ); 
  return JS_TRUE;
}

// member functions
JSBool SpiderMonkey::SFRotation_getAxis( JSContext *cx, JSObject *obj, uintN argc, 
                                        jsval *argv, jsval *rval )
{
  JSClass* arg_classes[] = { &SFRotationClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, arg_classes ) ) {
    return JS_FALSE;
  }

  SFRotation* sfr_p = helper_extractPrivateObject<SFRotation>( cx, obj );
  
  SFVec3f* sfreturn_p = new SFVec3f( sfr_p->getValue().axis );
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, sfreturn_p, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFRotation_inverse( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval ) {
  JSClass* arg_classes[] = { &SFRotationClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, arg_classes ) ) {
    return JS_FALSE;
  }

  SFRotation* sfr_p = helper_extractPrivateObject<SFRotation>( cx, obj );
  
  SFRotation* sfreturn_p = new SFRotation( -sfr_p->getValue() );
  *rval = OBJECT_TO_JSVAL( SFRotation_newInstance( cx, sfreturn_p, true ) );
  return JS_TRUE;
}
JSBool SpiderMonkey::SFRotation_multiply( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval ) {
  JSClass* arg_classes[] = { &SFRotationClass, &SFRotationClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, arg_classes ) ) {
    return JS_FALSE;
  }

  SFRotation* sfr1 = helper_extractPrivateObject<SFRotation>( cx, JSVAL_TO_OBJECT( argv[0] ) );
  SFRotation* sfr_p = helper_extractPrivateObject<SFRotation>( cx, obj );

  SFRotation* sfreturn_p = new SFRotation( sfr_p->getValue() * sfr1->getValue() );
  *rval = OBJECT_TO_JSVAL( SFRotation_newInstance( cx, sfreturn_p, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFRotation_multiVec( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval ) {
  JSClass* arg_classes[] = { &SFRotationClass, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, arg_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* sfv = helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT( argv[0] ) );
  SFRotation* sfr_p = helper_extractPrivateObject<SFRotation>( cx, obj );

  SFVec3f* sfreturn_p = new SFVec3f( sfr_p->getValue() * sfv->getValue() );
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, sfreturn_p, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFRotation_setAxis( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval ) {
  JSClass* arg_classes[] = { &SFRotationClass, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, arg_classes ) ) {
    return JS_FALSE;
  }

  SFVec3f* sfv = helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT( argv[0] ) );
  SFRotation* sfr_p = helper_extractPrivateObject<SFRotation>( cx, obj );

  Rotation r = sfr_p->getValue();
  r.axis = sfv->getValue();
  sfr_p->setValue(r);

  return JS_TRUE;
}

JSBool SpiderMonkey::SFRotation_slerp( JSContext *cx, JSObject *obj, uintN argc, jsval *argv,jsval *rval ) {
  JSClass* arg_classes[] = { &SFRotationClass, &SFRotationClass, &SFNumberClass};
  if ( !helper_sanityCheck(cx, obj, argc, argv, 2, arg_classes ) ) {
    return JS_FALSE;
  }

  SFRotation* sfr_p = helper_extractPrivateObject<SFRotation>( cx, obj );
  SFRotation* sfr1 = helper_extractPrivateObject<SFRotation>( cx, JSVAL_TO_OBJECT( argv[0] ) );
  jsdouble num;
  JS_ValueToNumber( cx, argv[1], &num );

  SFRotation* sfreturn_p = new SFRotation( sfr_p->getValue().slerp( sfr1->getValue(), (H3DFloat)num ) );
  *rval = OBJECT_TO_JSVAL( SFRotation_newInstance( cx, sfreturn_p, true ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::SFRotation_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  if (JSVAL_IS_INT(id))
  {
    SFRotation* sf = helper_extractPrivateObject<SFRotation>( cx, obj );
    if (sf == 0) {
      return JS_FALSE;
    }
    const Rotation &v = getValueNoAccessCheck( sf );

    switch (JSVAL_TO_INT(id)) {
      case SFRotation_X:
        JS_NewNumberValue( cx, v.axis.x, vp ); 
        break;
        
      case SFRotation_Y:
        JS_NewNumberValue( cx, v.axis.y, vp ); 
        break;

      case SFRotation_Z:
        JS_NewNumberValue( cx, v.axis.z, vp ); 
        break;
      
      case SFRotation_ANGLE:
        JS_NewNumberValue( cx, v.angle, vp ); 
        break;
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if ( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFRotation_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{

  if (JSVAL_IS_INT(id))
  {
    SFRotation* sf = helper_extractPrivateObject<SFRotation>( cx, obj );
    if (sf == 0) return JS_FALSE;

    Rotation v = sf->getValue();
    JSBool res = JS_TRUE;

    switch (JSVAL_TO_INT(id))
    {
      case SFRotation_X:
        jsdouble x;
        res = JS_ValueToNumber( cx, *vp, &x );
        v.axis.x = (H3DFloat) x;
        break;
      case SFRotation_Y:
        jsdouble y;
        res = JS_ValueToNumber( cx, *vp, &y );
        v.axis.y = (H3DFloat) y;
        break;
      
      case SFRotation_Z: 
        jsdouble z;
        res = JS_ValueToNumber( cx, *vp, &z );
        v.axis.z = (H3DFloat) z;
        break;
      
      case SFRotation_ANGLE: 
        jsdouble angle;
        res = JS_ValueToNumber( cx, *vp, &angle );
        v.angle = (H3DFloat) angle;
        break;
    }

    if( !res ) {
      JS_ReportError(cx, "SFRotation attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( sf, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFRotation_newInstance( JSContext *cx, SFRotation *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFRotationClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFRotation_properties );
  JS_DefineFunctions(cx, js_field, SFRotation_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field ) );
  return js_field; 
}



//////////////////////////////////////////////////////////
// SFNode object

JSBool SpiderMonkey::SFNode_construct(JSContext *cx, JSObject *obj, 
                                      uintN argc, jsval *argv,
                                      jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc != 0 ) return JS_FALSE;

  // create return value
  SFNode *return_node = new SFNode;
  return_node->setValue( NULL );
  
  *rval = OBJECT_TO_JSVAL( SFNode_newInstance( cx, 
					       return_node,
					       true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFNode_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{

  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if( JSVAL_IS_STRING( id ) ) {
    JSString *s = JSVAL_TO_STRING( id );
    string field_name = JS_GetStringBytes( s );

    //cerr << "Get Property SFNode: " << field_name << " " << obj << endl;
    SFNode* node_field = static_cast<SFNode *>(private_data->getPointer());
    Node *n = node_field->getValue();
    Field *f = NULL;
    if( n ) f = n->getField( field_name );
    if( f ) *vp = jsvalFromField( cx, f, false );
  }

  return JS_TRUE;
}

JSBool
SpiderMonkey::SFNode_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if( JSVAL_IS_STRING( id ) ) {
    JSString *s = JSVAL_TO_STRING( id );
    string field_name = JS_GetStringBytes( s );
    //    cerr << "Set Property SFNode: " << field_name << " " << obj << endl;
    SFNode* node_field = static_cast<SFNode *>(private_data->getPointer());
    Node *n = node_field->getValue();
    Field *f = NULL;
    if( n ) f = n->getField( field_name );
    if( f ) return setFieldValueFromjsval( cx, f, *vp );
  }
  return JS_TRUE;
}


JSObject *SpiderMonkey::SFNode_newInstance( JSContext *cx,SFNode *field,bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFNodeClass, NULL, NULL );  

  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field,
							       internal_field) );
  return js_field;
}

JSBool SpiderMonkey::SFNode_getNodeName(JSContext *cx, JSObject *obj, 
					uintN argc, jsval *argv,
					jsval *rval) {
  // check that this object is a SFNode_class 
  if (!JS_InstanceOf(cx, obj, &SFNodeClass, argv))
    return JS_FALSE;

  // check that we have enough arguments and that they are of
  // correct type.
  if( argc != 0 )
    return JS_FALSE;
  
  FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( this_private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  SFNode* this_sfnode = static_cast<SFNode *>(this_private_data->getPointer());
  
  // make sure we have a value
  if (!this_sfnode ) return JS_FALSE;
   
  // create return value
  Node *n = this_sfnode->getValue();
  string type_name;
  if( n ) {
    type_name = n->getTypeName();
  } else {
    type_name = "NULL";
  }
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, 
					      type_name.c_str(), 
					      type_name.size()) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFNode_getNodeType(JSContext *cx, JSObject *obj, 
                                        uintN argc, jsval *argv,
                                        jsval *rval) {
  cerr << "SFNode_getNodeType" << endl;
  return JS_TRUE;
}

JSBool SpiderMonkey::SFNode_getFieldDefinitions(JSContext *cx, JSObject *obj, 
					uintN argc, jsval *argv,
					jsval *rval) {
   cerr << "SFNode_getFieldDefinitions" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFNode_toX3DString(JSContext *cx, JSObject *obj, 
                                        uintN argc, jsval *argv,
                                        jsval *rval) {
    // check that this object is a SFNode_class 
  if (!JS_InstanceOf(cx, obj, &SFNodeClass, argv))
    return JS_FALSE;

  // check that we have enough arguments and that they are of
  // correct type.
  if( argc != 0 )
    return JS_FALSE;
  
  FieldObjectPrivate *this_private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( this_private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  SFNode* this_sfnode = static_cast<SFNode *>(this_private_data->getPointer());
  
  // make sure we have a value
  if (!this_sfnode ) return JS_FALSE;
   
  // create return value
  stringstream s;
  X3D::writeNodeAsX3D( s, this_sfnode->getValue() );
  string x3d_string = s.str();
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, 
					      x3d_string.c_str(), 
					      x3d_string.size()) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFNode_toVRMLString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
   cerr << "SFNode_toVRMLString" << endl;
   return JS_TRUE;
}


//////////////////////////////////////////////////////////
// SFColor object

JSBool SpiderMonkey::SFColor_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 3 ||
      ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
      ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
      ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    return JS_FALSE;
  }

  H3DUtil::RGB v( 0, 0, 0 );

  if( argc > 0 ) {
    jsdouble r;
    if(!JS_ValueToNumber( cx, argv[0], &r) ) return JS_FALSE;
    v.r = (H3DFloat) r;
  }

  if( argc > 1 ) {
    jsdouble g;
    if(!JS_ValueToNumber( cx, argv[1], &g) ) return JS_FALSE;
    v.g = (H3DFloat) g;
  }

  if( argc > 2 ) {
    jsdouble b;
    if(!JS_ValueToNumber( cx, argv[2], &b) ) return JS_FALSE;
    v.b = (H3DFloat) b;
  }
  
  // create return value
  SFColor *return_color = new SFColor;
  return_color->setValue( v );
  
  *rval = OBJECT_TO_JSVAL( SFColor_newInstance( cx,  return_color, true ) ); 
  return JS_TRUE;
} 

JSBool SpiderMonkey::SFColor_getHSV(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  SFColor* sfc = helper_extractPrivateObject<SFColor>( cx, obj );
  if (sfc == 0) return JS_FALSE;

  H3DUtil::RGB rgb = sfc->getValue();
  Vec3f hsv = rgb.toHSV();
  JSObject* jshsv = JS_NewArrayObject(cx, 3, NULL);
  *rval = OBJECT_TO_JSVAL(jshsv);
  jsval a, b, c;
  JS_NewNumberValue(cx, hsv.x, &a);
  JS_SetElement(cx, jshsv, 0, &a);
  JS_NewNumberValue(cx, hsv.y, &b);
  JS_SetElement(cx, jshsv, 1, &b);
  JS_NewNumberValue(cx, hsv.z, &c);
  JS_SetElement(cx, jshsv, 2, &c);

  return JS_TRUE;
}

JSBool SpiderMonkey::SFColor_setHSV(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  JSClass* type_classes[] = { &SFColorClass, &SFNumberClass, &SFNumberClass, &SFNumberClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 3, type_classes ) ) {
    return JS_FALSE;
  }

  SFColor* sfc = helper_extractPrivateObject<SFColor>(cx, obj );
  if (sfc == 0)
    return JS_FALSE;

  Vec3f hsv;
  jsdouble x, y, z;
  JS_ValueToNumber(cx, argv[0], &x);
  hsv.x = (H3DFloat) x;
  JS_ValueToNumber(cx, argv[1], &y);
  hsv.y = (H3DFloat) y;
  JS_ValueToNumber(cx, argv[2], &z);
  hsv.z = (H3DFloat) z;
  H3DUtil::RGB rgb = sfc->getValue();
  rgb.fromHSV(hsv);
  sfc->setValue(rgb);

  return JS_TRUE;
}

JSBool SpiderMonkey::SFColor_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  SFColor* sfc = helper_extractPrivateObject<SFColor>( cx, obj );
  if (sfc == 0) return JS_FALSE;

  if (JSVAL_IS_INT(id)) {

    const RGB &rgb = getValueNoAccessCheck( sfc  );

    switch (JSVAL_TO_INT(id)) {
    case SFCOLOR_R: {
      JS_NewNumberValue( cx, rgb.r, vp ); 
      break;
    }
    case SFCOLOR_G: {
      JS_NewNumberValue( cx, rgb.g, vp ); 
      break;
    }
    case SFCOLOR_B: {
      JS_NewNumberValue( cx, rgb.b, vp ); 
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfcolor_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
	return JS_FALSE;
      } else {
	return JS_TRUE;
      }
    }
}

JSBool SpiderMonkey::SFColor_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  SFColor* this_color = helper_extractPrivateObject<SFColor>( cx, obj );
  if (JSVAL_IS_INT(id)) {
    RGB v = this_color->getValue();
    
    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFCOLOR_R: {
      jsdouble r;
      res = JS_ValueToNumber( cx, *vp, &r );
      v.r = (H3DFloat) r;
      break;
    }
    case SFCOLOR_G: {
      jsdouble g;
      res = JS_ValueToNumber( cx, *vp, &g );
      v.g = (H3DFloat) g;
      break;
    }
    case SFCOLOR_B: {
      jsdouble b;
      res = JS_ValueToNumber( cx, *vp, &b );
      v.b = (H3DFloat) b;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFColor attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_color, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // sfcolor_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFColor_newInstance( JSContext *cx, SFColor *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFColorClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFColor_properties );
  JS_DefineFunctions(cx, js_field, SFColor_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field,
							       internal_field) );
  return js_field;
}


//////////////////////////////////////////////////////////
// SFImage object

JSBool SpiderMonkey::SFImage_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if (! (argc == 4 && JSVAL_IS_NUMBER( argv[0] ) // width
                && JSVAL_IS_NUMBER( argv[1] ) // height
                && JSVAL_IS_INT( argv[2] )    // comp
                && JSVAL_IS_OBJECT( argv[3] ) && JS_InstanceOf(cx, JSVAL_TO_OBJECT(argv[3]), &JS_MFInt32::js_class, argv) ) ) {
    JS_ReportError(cx, "Wrong arguments");
    return JS_FALSE;
  }

  jsdouble dx, dy, dcomp;
  JS_ValueToNumber(cx, argv[0], &dx);
  JS_ValueToNumber(cx, argv[1], &dy);
  JS_ValueToNumber(cx, argv[2], &dcomp);
  int x = (int)dx;
  int y = (int)dy;
  int comp = (int)dcomp;

  MFInt32* arr_obj = helper_extractPrivateObject<MFInt32>( cx, JSVAL_TO_OBJECT(argv[3]) );
  ImagePrivate* ip = new ImagePrivate(x, y, comp, arr_obj->getValue());

  SFImagePrivate* sfimg = new SFImagePrivate(*ip);
  *rval = OBJECT_TO_JSVAL( SFImage_newInstance( cx, sfimg, true ) ); 
  return JS_TRUE;
}

JSBool SpiderMonkey::SFImage_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  SFImagePrivate* sfimg = helper_extractPrivateObject<SFImagePrivate>( cx, obj );
  ImagePrivate ip = sfimg->getValue();

  if (sfimg == 0) return JS_FALSE;

  if (JSVAL_IS_INT(id)) {
    switch (JSVAL_TO_INT(id))
    {
      case SFImage_WIDTH:
        *vp = INT_TO_JSVAL(ip.x());
        return JS_TRUE;
      case SFImage_HEIGHT:
        *vp = INT_TO_JSVAL(ip.y());
        return JS_TRUE;
      case SFImage_COMP:
        *vp = INT_TO_JSVAL( ip.comp() );
        return JS_TRUE;
      case SFImage_ARRAY:
        // create jsobject wrapping the mfint32 object
        MFInt32* mfint32 = new MFInt32();
        mfint32->setValue(ip.arr());
        JSObject* r_jsobj = JS_NewObject(cx, &JS_MFInt32::js_class, NULL, NULL);
        JS_SetPrivate(cx, r_jsobj, (void *) new FieldObjectPrivate( mfint32 , true) );
        *vp = OBJECT_TO_JSVAL( r_jsobj );

        return JS_TRUE;
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // SFImage_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
	return JS_FALSE;
      } else {
	return JS_TRUE;
      }
    }
}

JSBool SpiderMonkey::SFImage_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  SFImagePrivate* sfimg = helper_extractPrivateObject<SFImagePrivate>( cx, obj );
  ImagePrivate ip = sfimg->getValue();

  if (JSVAL_IS_INT(id)) {

    JSBool res = JS_TRUE;
    jsint i;
    switch (JSVAL_TO_INT(id))
    {
      case SFImage_WIDTH:
        if ( !JS_ValueToInt32(cx, *vp, &i) ) {
          JS_ReportError(cx, "Value must be number" );
          return JS_FALSE;
        }
        if ( i <= 0 ) {
          JS_ReportError(cx, "Value must be positive" );
          return JS_FALSE;
        }
        ip.setX(i);
        break;

      case SFImage_HEIGHT:

        if ( !JS_ValueToInt32(cx, *vp, &i) ) {
          JS_ReportError(cx, "Value must be number" );
          return JS_FALSE;
        }
        if ( i <= 0 ) {
          JS_ReportError(cx, "Value must be positive" );
          return JS_FALSE;
        }
        ip.setY(i);
        break;

      case SFImage_COMP:
        if ( !JS_ValueToInt32(cx, *vp, &i) ) {
          JS_ReportError(cx, "Value must be number" );
          return JS_FALSE;
        }
        // must be either 1, 2, 3 or 4
        if ( i <= 0 || i >= 5 ) {
          JS_ReportError(cx, "Value must be either 1, 2, 3 or 4" );
          return JS_FALSE;
        }
        ip.setComp(i);
        break;

      case SFImage_ARRAY:
        // sanity check
        if ( !JSVAL_IS_OBJECT(*vp) ) {
          JS_ReportError(cx, "RHS must be object.");
          return JS_FALSE;
        }
        if ( !JS_InstanceOf(cx, JSVAL_TO_OBJECT(*vp), &JS_MFInt32::js_class, 0) ) {
          JS_ReportError(cx, "RHS must be an int array");
          return JS_FALSE;
        }
        MFInt32* mfint32 = helper_extractPrivateObject<MFInt32>( cx, JSVAL_TO_OBJECT(*vp) );

        // check for array size
        vector<int> vals = mfint32->getValue();
        if ( vals.size() != vals[0] * vals[1] + 3 ) {
          JS_ReportError(cx, "Supplied int array has wrong number of arguments");
          return JS_FALSE;
        }

        // convert to vector
        ip.setArray(mfint32->getValue());

        break;
    }
    setValueNoAccessCheck(sfimg, ip);
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // SFImage_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFImage_newInstance( JSContext *cx, SFImagePrivate *field, bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFImageClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFImage_properties );
  JS_DefineFunctions(cx, js_field, SFImage_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field) );
  return js_field;
}

//////////////////////////////////////////////////////////
// X3DExecutionContext object

JSBool SpiderMonkey::X3DExecutionContext_createNode(JSContext *cx, 
						    JSObject *obj, 
						    uintN argc, jsval *argv,
						    jsval *rval) {
  // check that this object is a X3DExecutionContext_class 
  if (!JS_InstanceOf(cx, obj, &X3DExecutionContextClass, argv))
    return JS_FALSE;

  // check that we have enough arguments and that they are of
  // correct type.
  if( argc != 1 ) return JS_FALSE;

  JSString *s = JS_ValueToString( cx, argv[0] );
  if( !s ) {
    JS_ReportError(cx, "Value not convertable to String" );
    return JS_FALSE;
  }

  ExecutionContextPrivate *private_data = 
    static_cast<ExecutionContextPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }
  
  SAI::ExecutionContext* context = private_data->getPointer();
  
  // make sure we have a value
  if (!context ) { 
    return JS_TRUE;
  }
  
  // create return value

  // TODO: exception SAIInvalidName 
  SFNode *return_sfnode = new SFNode;
  Node *n = context->createNode( JS_GetStringBytes(s) );
  return_sfnode->setValue( n );
  
  *rval = OBJECT_TO_JSVAL( SFNode_newInstance( cx, 
					       return_sfnode,
					       true ) ); 
  return JS_TRUE;
}


JSBool
SpiderMonkey::X3DExecutionContext_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{

  ExecutionContextPrivate *private_data = 
    static_cast<ExecutionContextPrivate *>(JS_GetPrivate(cx,obj));
  
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property X3DExecutionContext" << endl;
  if (JSVAL_IS_INT(id)) {
    SAI::ExecutionContext* context = private_data->getPointer();

    switch (JSVAL_TO_INT(id)) {
    case X3DEXECUTIONCONTEXT_ROOTNODES: {
      *vp = OBJECT_TO_JSVAL(JS_MFNode::newInstance( cx, context->getRootNodes(), false ));
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "X3DExecutionContext object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool
SpiderMonkey::X3DExecutionContext_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  return JS_TRUE;
  /*
  //cerr << "Set Property X3DExecutionContext" << endl;
  if (JSVAL_IS_INT(id)) {
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
    X3DExecutionContext* this_vec3f = static_cast<X3DExecutionContext *>(private_data->getPointer());
    Vec3f v = this_vec3f->getValue();

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC3F_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = (H3DFloat) x;
      break;
    }
    case SFVEC3F_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = (H3DFloat) y;
      break;
    }
    case SFVEC3F_Z: {
      jsdouble z;
      res = JS_ValueToNumber( cx, *vp, &z );
      v.z = (H3DFloat) z;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "X3DExecutionContext attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      setValueNoAccessCheck( this_vec3f, v );
      return JS_TRUE;
    }
  } else {
    // if we get here the property was not one of the ones defined in
    // browser_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
    }*/
}


JSObject *SpiderMonkey::X3DExecutionContext_newInstance( JSContext *cx,
							 SAI::ExecutionContext *context,
							 bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &X3DExecutionContextClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, X3DExecutionContext_properties );
  JS_DefineFunctions(cx, js_field, X3DExecutionContext_functions );
  JS_SetPrivate(cx, js_field, (void *) new ExecutionContextPrivate( context, 
								    internal_field ) );
  return js_field;
}



JSBool
SpiderMonkey::X3DScene_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  ExecutionContextPrivate *private_data = 
    static_cast<ExecutionContextPrivate *>(JS_GetPrivate(cx,obj));
  
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  //cerr << "Get Property X3DScene" << endl;
  if (JSVAL_IS_INT(id)) {
    SAI::ExecutionContext* context = private_data->getPointer();

    switch (JSVAL_TO_INT(id)) {
    case X3DEXECUTIONCONTEXT_ROOTNODES: {
      *vp = OBJECT_TO_JSVAL(JS_MFNode::newInstance( cx, context->getRootNodes(), false ));
      break;
    }
    }
    return JS_TRUE;
  } else {
    // if we get here the property was not one of the ones defined in
    // sfvec3f_properties. It can be another property such as a function
    // so we check if the previous value of vp contains anything. On call 
    // of this function it contains the current value of the attribute.
    // If it is JSVAL_VOID the attribute does not exist.
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "X3DScene object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool
SpiderMonkey::X3DScene_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  return JS_TRUE;
}


JSObject *SpiderMonkey::X3DScene_newInstance( JSContext *cx,
					      SAI::SAIScene *context,
					      bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &X3DSceneClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, X3DScene_properties );
  JS_DefineFunctions(cx, js_field, X3DScene_functions );
  JS_SetPrivate(cx, js_field, (void *) new ExecutionContextPrivate( context, 
								    internal_field ) );
  return js_field;
}


JSObject *SpiderMonkey::Browser_newInstance( JSContext *cx, SAI::Browser *browser, bool internal_ptr ) {
  JSObject *js_browser;

  js_browser = JS_NewObject( cx, &BrowserClass, NULL, NULL );  
  JS_DefineProperties(cx, js_browser, browser_properties );
  JS_DefineFunctions(cx, js_browser, browser_functions );

  JS_SetPrivate(cx, js_browser, (void *) new BrowserPrivate( browser, internal_ptr ) );
  return js_browser; 
}

JSBool SpiderMonkey::Browser_print(JSContext *cx, 
                                   JSObject *obj, uintN argc, 
                                   jsval *argv, jsval *rval) {

  const char *s;
  if (!JS_ConvertArguments(cx, argc, argv, "s", &s))
    return JS_FALSE;

  BrowserPrivate *private_data = 
      static_cast<BrowserPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  SAI::Browser* browser = private_data->getPointer();
  browser->print( s );
  *rval = JSVAL_VOID;  /* return undefined */
  return JS_TRUE;
}

JSBool SpiderMonkey::Browser_println(JSContext *cx, 
				     JSObject *obj, uintN argc, 
				     jsval *argv, jsval *rval) {

  const char *s;
  if (!JS_ConvertArguments(cx, argc, argv, "s", &s))
    return JS_FALSE;

  BrowserPrivate *private_data = 
      static_cast<BrowserPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  SAI::Browser* browser = private_data->getPointer();
  browser->print( string(s) + "\n" );
  *rval = JSVAL_VOID;  /* return undefined */
  return JS_TRUE;
}

JSBool SpiderMonkey::Browser_createX3DFromString(JSContext *cx, JSObject *obj, 
                                                 uintN argc, jsval *argv, jsval *rval)
{

  try {
    if( argc != 1 ) {
      JS_ReportError(cx, "createX3DFromString requires one argument." );
      return JS_FALSE;
    }

    JSString *s = JSVAL_TO_STRING( argv[0] );
    if( !s ) {
      JS_ReportError(cx, "createX3DFromString argument must be convertable to String." );
      return JS_FALSE;
    }

    BrowserPrivate *private_data = 
      static_cast<BrowserPrivate *>(JS_GetPrivate(cx,obj));
    SAI::Browser* browser = private_data->getPointer();
    SAI::SAIScene *scene = 
      browser->createX3DFromString( JS_GetStringBytes( s ) );
    *rval = OBJECT_TO_JSVAL( X3DScene_newInstance( cx, scene, true ) );

  } catch( SAI::SAIError &e ) {
    setJSException( cx, e );
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::Browser_createX3DFromURL(JSContext *cx, 
					      JSObject *obj, uintN argc, 
					      jsval *argv, jsval *rval) {
  
  if( argc == 0 ) {
    JS_ReportError(cx, "createX3DFromURL requires at least one argument." );
    return JS_FALSE;
  }
  
  if (!JS_InstanceOf(cx, JSVAL_TO_OBJECT( argv[0] ), &JS_MFString::js_class, argv)) {
    JS_ReportError(cx, "arg 0 to createX3DFromURL must be of MFString type." );
    return JS_FALSE;
  }

  try {
    MFString urls;
    setFieldValueFromjsval( cx, &urls, argv[0] );
    BrowserPrivate *private_data = 
      static_cast<BrowserPrivate *>(JS_GetPrivate(cx,obj));
    
    // make sure the object have not been manually disposed by the user.
    if( private_data->isDisposed()  ) {
      setJSException( cx, SAI::SAIError::SAI_DISPOSED );
      return JS_FALSE;
    }

    SAI::Browser* browser = private_data->getPointer();
    SAI::SAIScene *scene = 
      browser->createX3DFromURL( &urls );
    *rval = OBJECT_TO_JSVAL( X3DScene_newInstance( cx, scene, true ) );
    return JS_TRUE;
  } catch( SAI::SAIError &e ) {
    setJSException( cx, e );
    return JS_FALSE;
  }
  return JS_TRUE;
}

JSBool SpiderMonkey::Browser_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  return JS_TRUE;
}

JSBool SpiderMonkey::Browser_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  BrowserPrivate *private_data = 
    static_cast<BrowserPrivate *>(JS_GetPrivate(cx,obj));

  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  SAI::Browser* browser = private_data->getPointer();
  
  if (JSVAL_IS_INT(id)) {
        switch (JSVAL_TO_INT(id)) {
        case BROWSER_VERSION: {
          string version = browser->getVersion();
          *vp = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)version.c_str(), version.length() ) );
          break;
        }
        case BROWSER_NAME: {
          string name = browser->getName();
          *vp = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char * )name.c_str(), name.length() ) );
          break;
        }
	case BROWSER_CURRENT_SCENE: {
	  // TODO: sometime should return scene instead of 
	  // execution context
	  SAI::ExecutionContext *c = browser->getExecutionContext();
	  *vp = OBJECT_TO_JSVAL(X3DExecutionContext_newInstance( cx, c, false ));
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





bool SpiderMonkey::insertH3DTypes( JSContext *cx, JSObject *obj ) {
  // TODO: FIX!!
  JS_InitClass( cx, obj, NULL, 
				  &SFVec2fClass, SFVec2f_construct, 3, 
				  SFVec2f_properties, SFVec2f_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, 
				  &SFVec2dClass, SFVec2d_construct, 3, 
				  SFVec2d_properties, SFVec2d_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, 
				  &SFVec3fClass, SFVec3f_construct, 3, 
				  SFVec3f_properties, SFVec3f_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, 
				  &SFVec3dClass, SFVec3d_construct, 3, 
				  SFVec3d_properties, SFVec3d_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, 
				  &SFVec4fClass, SFVec4f_construct, 3, 
				  SFVec4f_properties, SFVec4f_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, 
				  &SFVec4dClass, SFVec4d_construct, 3, 
				  SFVec4d_properties, SFVec4d_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, 
			&SFNodeClass, SFNode_construct, 3, 
			NULL, SFNode_functions, NULL, NULL );
  JS_InitClass( cx, obj, NULL, 
			&SFColorClass, SFColor_construct, 3, 
			SFColor_properties, SFColor_functions, NULL, NULL );

  JS_InitClass( cx, obj, NULL, &SFImageClass, SFImage_construct, 3,
    SFImage_properties, SFImage_functions, NULL, NULL);

  JS_InitClass( cx, obj, NULL, &SFRotationClass, SFRotation_construct, 3,
    SFRotation_properties, SFRotation_functions, NULL, NULL);


  JS_MFFloat::initClass( cx, obj, "MFFloat" );
  JS_MFDouble::initClass( cx, obj, "MFDouble" );
  JS_MFTime::initClass( cx, obj, "MFTime" );
  JS_MFInt32::initClass( cx, obj, "MFInt32" );
  JS_MFVec2f::initClass( cx, obj, "MFVec2f" );
  JS_MFVec2d::initClass( cx, obj, "MFVec2d" );
  JS_MFVec3f::initClass( cx, obj, "MFVec3f" );
  JS_MFVec3d::initClass( cx, obj, "MFVec3d" );
  JS_MFVec4f::initClass( cx, obj, "MFVec4f" );
  JS_MFVec4d::initClass( cx, obj, "MFVec4d" );
  JS_MFBool::initClass( cx, obj, "MFBool" );
  JS_MFString::initClass( cx, obj, "MFString" );
  JS_MFNode::initClass( cx, obj, "MFNode" );
  JS_MFColor::initClass( cx, obj, "MFColor" );
  JS_MFRotation::initClass( cx, obj, "MFRotation" );
//  JS_MFImage::initClass( cx, obj, "MFImage" );

  return true;
}

template< class FieldType, JSObject *(*NewInstanceFunc)( JSContext *, FieldType *, bool ) >
jsval newInstanceFromField( JSContext *cx, Field *field, bool make_copy ) {
  FieldType *f = static_cast< FieldType * >( field );
  if( make_copy ) { 
    f = new FieldType;
    f->setValue( static_cast< FieldType * >(field)->getValue() );
  }
  return OBJECT_TO_JSVAL( NewInstanceFunc( cx, f, make_copy ));
}

jsval SpiderMonkey::jsvalFromField( JSContext *cx, Field *field, bool make_copy ) {

  X3DTypes::X3DType x3d_type = field->getX3DType();
  switch( x3d_type ) { 
  case X3DTypes::SFFLOAT: { 
    SFFloat *f = static_cast< SFFloat * >( field );
    jsdouble v = getValueNoAccessCheck( f );
    jsval rval;
    JS_NewNumberValue( cx, v, &rval);
    return rval;
  } 
  case X3DTypes::SFDOUBLE: { 
    SFDouble *f = static_cast< SFDouble * >( field );
    jsdouble v = getValueNoAccessCheck( f );
    jsval rval;
    JS_NewNumberValue( cx, v, &rval);
    return rval;
  } 
  case X3DTypes::SFTIME: { 
    SFTime *f = static_cast< SFTime * >( field );
    jsdouble v = getValueNoAccessCheck( f );
    jsval rval;
    JS_NewNumberValue( cx, v, &rval);
    return rval;
  } 
  case X3DTypes::SFINT32: { 
    SFInt32 *f = static_cast< SFInt32 * >( field );
    jsdouble v = getValueNoAccessCheck( f );
    jsval rval;
    JS_NewNumberValue( cx, v, &rval);
    return rval;
  } 
  case X3DTypes::SFVEC2F: { 
    return newInstanceFromField< SFVec2f, SFVec2f_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFVEC2D: { 
    return newInstanceFromField< SFVec2d, SFVec2d_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFVEC3F: { 
    return newInstanceFromField< SFVec3f, SFVec3f_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFVEC3D: { 
    return newInstanceFromField< SFVec3d, SFVec3d_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFVEC4F: { 
    return newInstanceFromField< SFVec4f, SFVec4f_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFVEC4D: { 
    return newInstanceFromField< SFVec4d, SFVec4d_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFBOOL: { 
    SFBool *f = static_cast< SFBool * >( field );
    bool b = getValueNoAccessCheck( f );
    return BOOLEAN_TO_JSVAL( b );
  } 
  case X3DTypes::SFSTRING: { 
    SFString *f = static_cast< SFString * >( field );
    const string &s = getValueNoAccessCheck( f );
    JSString *js_s = JS_NewStringCopyN( cx, s.c_str(), s.size() );
    return STRING_TO_JSVAL( js_s );
  } 
  case X3DTypes::SFNODE: { 
    return newInstanceFromField< SFNode, SFNode_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFCOLOR: { 
     return newInstanceFromField< SFColor, SFColor_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFCOLORRGBA: { 
  } 
  case X3DTypes::SFROTATION: { 
    return newInstanceFromField< SFRotation, SFRotation_newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::SFQUATERNION: { 
  }
  case X3DTypes::SFMATRIX3F: { 
  } 
  case X3DTypes::SFMATRIX4F: { 
  } 
  case X3DTypes::SFMATRIX3D: { 
  } 
  case X3DTypes::SFMATRIX4D: { 
    break;
  } 
  case X3DTypes::MFFLOAT: { 
    return newInstanceFromField< MFFloat, JS_MFFloat::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFDOUBLE: { 
    return newInstanceFromField< MFDouble, JS_MFDouble::newInstance >( cx, field, make_copy );
 
  } 
  case X3DTypes::MFTIME: { 
    return newInstanceFromField< MFTime, JS_MFTime::newInstance >( cx, field, make_copy );
 
  } 
  case X3DTypes::MFINT32: { 
    return newInstanceFromField< MFInt32, JS_MFInt32::newInstance >( cx, field, make_copy );
 
  } 
  case X3DTypes::MFVEC2F: { 
    return newInstanceFromField< MFVec2f, JS_MFVec2f::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFVEC2D: { 
    return newInstanceFromField< MFVec2d, JS_MFVec2d::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFVEC3F: { 
    return newInstanceFromField< MFVec3f, JS_MFVec3f::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFVEC3D: { 
    return newInstanceFromField< MFVec3d, JS_MFVec3d::newInstance >( cx, field, make_copy );
  }
  case X3DTypes::MFVEC4F: { 
    return newInstanceFromField< MFVec4f, JS_MFVec4f::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFVEC4D: { 
    return newInstanceFromField< MFVec4d, JS_MFVec4d::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFBOOL: { 
    return newInstanceFromField< MFBool, JS_MFBool::newInstance >( cx, field, make_copy );
  }  
  case X3DTypes::MFSTRING: { 
    return newInstanceFromField< MFString, JS_MFString::newInstance >( cx, field, make_copy );
 
  } 
  case X3DTypes::MFNODE: { 
    return newInstanceFromField< MFNode, JS_MFNode::newInstance >( cx, field, make_copy );
  } 
  case X3DTypes::MFCOLOR:
    return newInstanceFromField< MFColor, JS_MFColor::newInstance >( cx, field, make_copy );
  case X3DTypes::MFROTATION:
    return newInstanceFromField< MFRotation, JS_MFRotation::newInstance >( cx, field, make_copy );
  case X3DTypes::MFCOLORRGBA: { 
  } 
  case X3DTypes::MFQUATERNION: { 
  } 
  case X3DTypes::MFMATRIX3F: { 
  } 
  case X3DTypes::MFMATRIX4F: { 
  } 
  case X3DTypes::MFMATRIX3D: { 
  }
  case X3DTypes::MFMATRIX4D: { 
  }
  } 
  return JSVAL_VOID;
}


template< class FieldType, X3DTypes::X3DType x3d_type >
JSBool setFieldValueFromjsvalTmpl( JSContext *cx, 
				   Field *field, 
				   jsval value ) {
  FieldType *f = static_cast< FieldType * >( field );

  if( JSVAL_IS_OBJECT( value ) ) {
    JSObject *js_object = JSVAL_TO_OBJECT( value );
      
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,js_object));

    Field *value_field = private_data->getPointer();
    if( value_field->getX3DType() == x3d_type ) {
      const typename FieldType::value_type &c = 
	  getValueNoAccessCheck( static_cast< FieldType * >( value_field ) );
      setValueNoAccessCheck( f, c ); 
      return JS_TRUE;
    } 
  }

  stringstream s;
  s << "Invalid type in assignment. Expecting " << field->getTypeName();
  JS_ReportError(cx, s.str().c_str() );
  return JS_FALSE;
}

template< class FieldType, X3DTypes::X3DType x3d_type >
JSBool MField_setFieldValueFromjsvalTmpl( JSContext *cx, 
					  Field *field, 
					  jsval value ) {
  FieldType *f = static_cast< FieldType * >( field );

  if( JSVAL_IS_OBJECT( value ) ) {
    JSObject *js_object = JSVAL_TO_OBJECT( value );
      
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,js_object));

    Field *value_field = private_data->getPointer();
    if( value_field->getX3DType() == x3d_type ) {
      const typename FieldType::vector_return_type &c = 
	  MField_getValueNoAccessCheck( static_cast< FieldType * >( value_field ) );
      MField_setValueNoAccessCheck( f, c ); 
      return JS_TRUE;
    } 
  }

  stringstream s;
  s << "Invalid type in assignment. Expecting " << field->getTypeName();
  JS_ReportError(cx, s.str().c_str() );
  return JS_FALSE;
}


template< class FieldType, 
	  class JSType,
	  JSBool (*ConvertFunc)( JSContext *,
				 jsval, JSType *) >
JSBool setFieldValueFromjsvalSimple( JSContext *cx, 
				     Field *field, 
				     jsval value ) {
  FieldType *f = static_cast< FieldType * >( field );
  JSType v;
  if( ConvertFunc( cx, value, &v ) ) {
    setValueNoAccessCheck( f, (typename FieldType::value_type) v);
    return JS_TRUE;
  } else {
    stringstream s;
    s <<  "Value not convertable to ";
    X3DTypes::X3DType type = field->getX3DType();
    if( type == X3DTypes::SFBOOL ) s << "Boolean.";
    else if( type == X3DTypes::SFSTRING ) s << "String";
    else s << "Numeric";
    JS_ReportError(cx, s.str().c_str() );
    return JS_FALSE;
  }
}

JSBool SpiderMonkey::setFieldValueFromjsval( JSContext *cx, Field *field, jsval value ) {
  X3DTypes::X3DType field_x3d_type = field->getX3DType();
  switch( field_x3d_type ) { 
  case X3DTypes::SFFLOAT: { 
   return setFieldValueFromjsvalSimple< SFFloat, jsdouble, JS_ValueToNumber >( cx, field, value ) ;
  } 
  case X3DTypes::SFDOUBLE: { 
    return setFieldValueFromjsvalSimple< SFDouble, jsdouble, JS_ValueToNumber >( cx, field, value ) ;
  } 
  case X3DTypes::SFTIME: { 
    return setFieldValueFromjsvalSimple< SFTime, jsdouble, JS_ValueToNumber >( cx, field, value ) ;
  } 
  case X3DTypes::SFINT32: { 
    return setFieldValueFromjsvalSimple< SFInt32, jsint, JS_ValueToInt32 >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC2F: { 
    return setFieldValueFromjsvalTmpl< SFVec2f, X3DTypes::SFVEC2F >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC2D: { 
    return setFieldValueFromjsvalTmpl< SFVec2d, X3DTypes::SFVEC2D >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC3F: { 
    return setFieldValueFromjsvalTmpl< SFVec3f, X3DTypes::SFVEC3F >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC3D: { 
    return setFieldValueFromjsvalTmpl< SFVec3d, X3DTypes::SFVEC3D >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC4F: { 
    return setFieldValueFromjsvalTmpl< SFVec4f, X3DTypes::SFVEC4F >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC4D: { 
    return setFieldValueFromjsvalTmpl< SFVec4d, X3DTypes::SFVEC4D >( cx, field, value ) ;
  } 
  case X3DTypes::SFBOOL: { 
    return setFieldValueFromjsvalSimple< SFBool, JSBool, JS_ValueToBoolean >( cx, field, value ) ;
  } 
  case X3DTypes::SFSTRING: { 
    SFString *f = static_cast< SFString * >( field );
    JSString *s = JS_ValueToString( cx, value );
    if( s ) {
      setValueNoAccessCheck( f, JS_GetStringBytes(s) );
      return JS_TRUE;
    } else {
      JS_ReportError(cx, "Value not convertable to String" );
      return JS_FALSE;
    }
  } 
  case X3DTypes::SFNODE: { 
    SFNode *f = static_cast< SFNode * >( field );

    if( JSVAL_IS_OBJECT( value ) ) {
      JSObject *js_object = JSVAL_TO_OBJECT( value );
      
      FieldObjectPrivate *private_data = 
	static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,js_object));

      Field *value_field = private_data->getPointer();
      if( value_field->getX3DType() == X3DTypes::SFNODE ) {
	Node *n = getValueNoAccessCheck( static_cast< SFNode * >( value_field ) );
	setValueNoAccessCheck( f, n ); 
	return JS_TRUE;
      } 
    }

    stringstream s;
    s << "Invalid type in assignment. Expecting SFNode.";
    JS_ReportError(cx, s.str().c_str() );
    return JS_FALSE;
    //    return setFieldValueFromjsvalTmpl< SFNode, X3DTypes::SFNODE >( cx, field, value ) ;
  } 
  case X3DTypes::SFCOLOR: { 
    return setFieldValueFromjsvalTmpl< SFColor, X3DTypes::SFCOLOR >( cx, field, value ) ;
  } 
  case X3DTypes::SFCOLORRGBA: { 
  } 
  case X3DTypes::SFROTATION: { 
    return setFieldValueFromjsvalTmpl< SFRotation, X3DTypes::SFROTATION>( cx, field, value ) ;
  } 
  case X3DTypes::SFQUATERNION: { 
  }
  case X3DTypes::SFMATRIX3F: { 
  } 
  case X3DTypes::SFMATRIX4F: { 
  } 
  case X3DTypes::SFMATRIX3D: { 
  } 
  case X3DTypes::SFMATRIX4D: { 
    break;
  } 
  case X3DTypes::MFFLOAT: { 
    return MField_setFieldValueFromjsvalTmpl< MFFloat, X3DTypes::MFFLOAT >( cx, field, value ) ;
  } 
  case X3DTypes::MFDOUBLE: { 
    return MField_setFieldValueFromjsvalTmpl< MFDouble, X3DTypes::MFDOUBLE >( cx, field, value ) ;
  } 
  case X3DTypes::MFTIME: { 
    return MField_setFieldValueFromjsvalTmpl< MFTime, X3DTypes::MFTIME >( cx, field, value ) ;
  } 
  case X3DTypes::MFINT32: { 
    return MField_setFieldValueFromjsvalTmpl< MFInt32, X3DTypes::MFINT32 >( cx, field, value );
  } 
  case X3DTypes::MFVEC2F: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec2f, X3DTypes::MFVEC2F >( cx, field, value );
  } 
  case X3DTypes::MFVEC2D: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec2d, X3DTypes::MFVEC2D >( cx, field, value );
  } 
  case X3DTypes::MFVEC3F: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec3f, X3DTypes::MFVEC3F >( cx, field, value );
  } 
  case X3DTypes::MFVEC3D: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec3d, X3DTypes::MFVEC3D >( cx, field, value );
  } 
  case X3DTypes::MFVEC4F: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec4f, X3DTypes::MFVEC4F >( cx, field, value );
  } 
  case X3DTypes::MFVEC4D: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec4d, X3DTypes::MFVEC4D >( cx, field, value );
  } 
  case X3DTypes::MFBOOL: { 
    return MField_setFieldValueFromjsvalTmpl< MFBool, X3DTypes::MFBOOL >( cx, field, value ) ;
   
  } 
  case X3DTypes::MFSTRING: { 
    return MField_setFieldValueFromjsvalTmpl< MFString, X3DTypes::MFSTRING >( cx, field, value );
  } 
  case X3DTypes::MFNODE: { 
    return MField_setFieldValueFromjsvalTmpl< MFNode, X3DTypes::MFNODE >( cx, field, value ) ;
  } 
  case X3DTypes::MFCOLOR: { 
    return MField_setFieldValueFromjsvalTmpl< MFColor, X3DTypes::MFCOLOR >( cx, field, value ) ;
  } 
  case X3DTypes::MFROTATION:
    return MField_setFieldValueFromjsvalTmpl< MFRotation, X3DTypes::MFROTATION>( cx, field, value ) ;
  case X3DTypes::MFCOLORRGBA: { 
  } 
  case X3DTypes::MFQUATERNION: { 
  } 
  case X3DTypes::MFMATRIX3F: { 
  } 
  case X3DTypes::MFMATRIX4F: { 
  } 
  case X3DTypes::MFMATRIX3D: { 
  }
  case X3DTypes::MFMATRIX4D: { 
  }
  }

  JS_ReportError(cx, "Field has invalid/unsupported X3DType." );
  return JS_FALSE;
}


JSBool SpiderMonkey::haveFunction( JSContext *cx, JSObject *obj, const char * name ) { 			
  jsval res;
  // get the property
  JSBool has_property = JS_GetProperty( cx, obj, name, &res );
  
  if( has_property && JSVAL_IS_OBJECT( res ) ) {
    JSObject *fun = JSVAL_TO_OBJECT( res );
    if( JS_ObjectIsFunction( cx, fun ) ) {
      return JS_TRUE;
    }
  }
  return JS_FALSE;
}

void SpiderMonkey::setJSException( JSContext *cx, const SAI::SAIError &e ) {
  switch( e.getErrorType() ) {
  case SAI::SAIError::SAI_INVALID_X3D: {
    string msg = string( "InvalidX3DException: " ) + e.getErrorMessage();
    JS_ReportError( cx, msg.c_str() );
    break;
  }
  case SAI::SAIError::SAI_INVALID_URL: {
    string msg = string( "InvalidURLException: " ) + e.getErrorMessage();
    JS_ReportError( cx, msg.c_str() );
    break;
  }
  case SAI::SAIError::SAI_URL_UNAVAILABLE: {
    string msg = string( "URLUnavailableException: " ) + e.getErrorMessage();
    JS_ReportError( cx, msg.c_str() );
    break;
  }
  case SAI::SAIError::SAI_DISPOSED: {
    // TODO: this is an incorrect error. Need to figure out how disposed
    // objects work.
    string msg = string( "ObjectDisposed: " ) + e.getErrorMessage();
    JS_ReportError( cx, msg.c_str() );
    break;
  }
  default:
    JS_ReportError( cx, e.getErrorMessage().c_str() );
  }
}


// e_argtypes[0] contains type of the object
// types of arguments starting from 1
bool SpiderMonkey::helper_sanityCheck(JSContext *cx, JSObject *obj, uintN argc, jsval *argv,
                               uintN e_argc, JSClass** e_argtypes) {
  // check that this object is a Type_class 
  if (!JS_InstanceOf(cx, obj, e_argtypes[0], argv)) {
    JS_ReportError(cx, "Object is of wrong type");
    return false;
  }

  // check number of arguments
  if (argc != e_argc) {
    JS_ReportError(cx, "Wrong number of arguments.");
    return false;
  }

  // check if the arguments are of correct type (number or object)
  for (uintN i = 0; i < argc; i++) {
    if (e_argtypes[i+1] == &SFNumberClass) {
      if ( !JSVAL_IS_NUMBER(argv[i]) ) {
        JS_ReportError(cx, "Argument %d is not number.", i);
        return false;
      }
    } else if ( !JSVAL_IS_OBJECT(argv[i]) ||
        !JS_InstanceOf( cx, JSVAL_TO_OBJECT(argv[i]), e_argtypes[i+1], argv )) {
      JS_ReportError(cx, "Argument of wrong type.");
      return false;
    }
  }

  return true;
}

template <class T>
bool SpiderMonkey::helper_calculate2_TTT(JSContext *cx, JSObject *this_obj, jsval *arg, T* const rval, BinaryOperator op) {
  T* obj1 = helper_extractPrivateObject<T>( cx, this_obj );
  T* obj2 = helper_extractPrivateObject<T>( cx, JSVAL_TO_OBJECT(*arg) );

  // calculate the result
  if (op == O_ADD) {
    rval->setValue( obj1->getValue() + obj2->getValue() );
  } else if (op == O_SUBTRACT) {
    rval->setValue( obj1->getValue() - obj2->getValue() );
  } else {
    return false;
  }

  return true;
}

template <class T, class N>
bool SpiderMonkey::helper_calculate2_TNT(JSContext *cx, JSObject *this_obj, jsval *arg, T* const rval, BinaryOperator op) {
  // convert lhs to object
  FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, this_obj ) );
  if ( this_private_data->isDisposed() ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return false;
  }
  T* obj1 = dynamic_cast<T*>( this_private_data->getPointer() );

  // convert rhs to number
  jsdouble d;
  if ( !JS_ValueToNumber(cx, *arg, &d) ) {
    JS_ReportError( cx, "Can't convert to number." );
    return false;
  }
  N num = (N) d;

  // calculate the result
  if (op == O_DIVIDE) {
    // division by zero check
    if ( abs(num) < 0.000000001 ) {
      JS_ReportError(cx, "Division by zero.");
      return false;
    }
    rval->setValue( obj1->getValue() / num );
  } else if (op == O_MULTIPLE) {
    rval->setValue( obj1->getValue() * num );
  } else {
    return false;
  }

  return true;
}


template <class T, class N>
bool SpiderMonkey::helper_calculate2_TTN(JSContext *cx, JSObject *this_obj, jsval *arg, jsval* const rjsval, BinaryOperator op) {
  T* obj1 = helper_extractPrivateObject<T>( cx, this_obj );
  T* obj2 = helper_extractPrivateObject<T>( cx, JSVAL_TO_OBJECT( *arg ) );

  if (obj1 == 0 || obj2 == 0)
    return false;

  N num;
  // calculate the result
  if (op == O_DOT) {
    num = obj1->getValue() * obj2->getValue(); 
  } else {
    return false;
  }

  JS_NewNumberValue(cx, num, rjsval);

  return true;
}


template<class T>
T* SpiderMonkey::helper_extractPrivateObject<SFVec3f>( JSContext* cx, JSObject* jsobj) {
  // convert lhs to object
  FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, jsobj ) );
  if ( this_private_data->isDisposed() ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return 0;
  }

  T* p = dynamic_cast<T*>( this_private_data->getPointer() );
  if ( !p ) {
    JS_ReportError(cx, "Internal value not found.");
    return 0;
  }
  return p;
}


#endif // HAVE_SPIDERMONKEY