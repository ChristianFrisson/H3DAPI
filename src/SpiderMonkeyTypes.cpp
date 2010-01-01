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
const typename FieldType::vector_type &MField_getValueNoAccessCheck( FieldType *field ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  const typename FieldType::vector_type &b = field->getValue();
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
				   const typename FieldType::vector_type &v ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  field->setValue( v);
  field->setAccessCheck( access );
}

//////////////////////////////////////////////////////////
// SFVec3f object

JSBool SpiderMonkey::SFVec3f_construct(JSContext *cx, JSObject *obj, 
			 uintN argc, jsval *argv,
			 jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 3 ||
      ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
      ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
      ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
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
  SFVec3f *return_vec3f = new SFVec3f;
  return_vec3f->setValue( v );
  
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, 
						return_vec3f,
						true ) ); 
  return JS_TRUE;
}

void SpiderMonkey::SFVec3f_finalize(JSContext *cx, JSObject *obj) {
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  
  //  cerr << "Finalize SFVec3f: " << obj << " " << private_data << endl;
  
  // The prototype of SFColor does not have a private member
  if( private_data ) {
    delete private_data; 
  }
}


 JSBool SpiderMonkey::SFVec3f_add(JSContext *cx, JSObject *obj, 
				  uintN argc, jsval *argv,
				  jsval *rval) {
   // check that this object is a SFVec3f_class 
   if (!JS_InstanceOf(cx, obj, &SFVec3fClass, argv))
      return JS_FALSE;

   // check that we have enough arguments and that they are of
   // correct type.
   if( argc != 1 || 
       !JSVAL_IS_OBJECT( argv[0] ) ||
       !JS_InstanceOf( cx, JSVAL_TO_OBJECT( argv[0] ), &SFVec3fClass, argv ) ) {
     return JS_FALSE;
   }

   FieldObjectPrivate *this_private_data = 
     static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
   FieldObjectPrivate *arg_private_data = 
     static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx, JSVAL_TO_OBJECT(argv[0])));

   SFVec3f* this_vec3f = static_cast<SFVec3f *>(this_private_data->getPointer());
   SFVec3f* arg_vec3f =  static_cast<SFVec3f *>(arg_private_data->getPointer());

   // make sure we have a value
   if (!this_vec3f || !arg_vec3f ) return JS_FALSE;
   
   // create return value
   SFVec3f *return_vec3f = new SFVec3f;
   return_vec3f->setValue( this_vec3f->getValue() + arg_vec3f->getValue() );
   
   *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, 
						 return_vec3f,
						 true ) ); 
   return JS_TRUE;
 }

JSBool SpiderMonkey::SFVec3f_cross(JSContext *cx, JSObject *obj, 
				   uintN argc, jsval *argv,
				   jsval *rval) {
   cerr << "cross" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_divide(JSContext *cx, JSObject *obj, 
				    uintN argc, jsval *argv,
				    jsval *rval) {
   cerr << "divide" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_dot(JSContext *cx, JSObject *obj, 
				 uintN argc, jsval *argv,
				 jsval *rval) {
   cerr << "dot" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_length(JSContext *cx, JSObject *obj, 
				    uintN argc, jsval *argv,
				    jsval *rval) {
   cerr << "length" << endl;
   return JS_TRUE;
}
JSBool SpiderMonkey::SFVec3f_multiple(JSContext *cx, JSObject *obj, 
				      uintN argc, jsval *argv,
				      jsval *rval) {
   cerr << "multiple" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_negate(JSContext *cx, JSObject *obj, 
				    uintN argc, jsval *argv,
				    jsval *rval) {
   cerr << "negate" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_normalize(JSContext *cx, JSObject *obj, 
				       uintN argc, jsval *argv,
				       jsval *rval) {
   cerr << "normalize" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_subtract(JSContext *cx, JSObject *obj, 
				      uintN argc, jsval *argv,
				      jsval *rval) {
   cerr << "subtract" << endl;
   return JS_TRUE;
}

JSBool SpiderMonkey::SFVec3f_toString(JSContext *cx, JSObject *obj, 
				      uintN argc, jsval *argv,
				      jsval *rval) {
  // check that this object is a SFVec3f_class 
  if (!JS_InstanceOf(cx, obj, &SFVec3fClass, argv))
    return JS_FALSE;

 FieldObjectPrivate *private_data = 
   static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
 SFVec3f* this_vec3f = static_cast<SFVec3f *>(private_data->getPointer());

  // make sure we have a value
  if (!this_vec3f ) return JS_FALSE;

  // return string representation of the Vec3f
  const Vec3f &v = this_vec3f->getValue(); 
  stringstream s;
  s << v;
  string str = s.str();
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx,
					      (char *)str.c_str(),
					      str.length() ) );
  return JS_TRUE;
}

JSBool
SpiderMonkey::SFVec3f_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  //cerr << "Get Property SFVec3f" << endl;
  if (JSVAL_IS_INT(id)) {
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
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

JSBool
SpiderMonkey::SFVec3f_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  //cerr << "Set Property SFVec3f" << endl;
  if (JSVAL_IS_INT(id)) {
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
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


JSObject *SpiderMonkey::SFVec3f_newInstance( JSContext *cx,
					     SFVec3f *field,
					     bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &SFVec3fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec3f_properties );
  JS_DefineFunctions(cx, js_field, SFVec3f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
							       internal_field ) );
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

void SpiderMonkey::SFNode_finalize(JSContext *cx, JSObject *obj) {
  //  cerr << "Finalize SFNode" << endl;
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  if( private_data ) delete private_data;
  //    SFVec3f* f = static_cast<SFVec3f *>(private_data->getPointer());
  // TODO: delete only those that need deletion
  //    if (f) delete f;
}


JSBool
SpiderMonkey::SFNode_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{

  if( JSVAL_IS_STRING( id ) ) {
    JSString *s = JSVAL_TO_STRING( id );
    string field_name = JS_GetStringBytes( s );

    //cerr << "Get Property SFNode: " << field_name << " " << obj << endl;
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
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
  if( JSVAL_IS_STRING( id ) ) {
    JSString *s = JSVAL_TO_STRING( id );
    string field_name = JS_GetStringBytes( s );
    //    cerr << "Set Property SFNode: " << field_name << " " << obj << endl;
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
    SFNode* node_field = static_cast<SFNode *>(private_data->getPointer());
    Node *n = node_field->getValue();
    Field *f = NULL;
    if( n ) f = n->getField( field_name );
    if( f ) return setFieldValueFromjsval( cx, f, *vp );
  }
  return JS_TRUE;
}


JSObject *SpiderMonkey::SFNode_newInstance( JSContext *cx,
					    SFNode *field,
					    bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &SFNodeClass, NULL, NULL );  

  //  JS_DefineProperties(cx, js_field, SFVec3f_properties );
  //JS_DefineFunctions(cx, js_field, SFVec3f_functions );
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
  
  FieldObjectPrivate *this_private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

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

JSBool SpiderMonkey::SFNode_toVRMLString(JSContext *cx, JSObject *obj, 
					 uintN argc, jsval *argv,
					 jsval *rval) {
   cerr << "SFNode_toVRMLString" << endl;
   return JS_TRUE;
}


//////////////////////////////////////////////////////////
// SFColor object

JSBool SpiderMonkey::SFColor_construct(JSContext *cx, JSObject *obj, 
				       uintN argc, jsval *argv,
				       jsval *rval) {
  // check that we have enough arguments and that they are of
  // correct type.
  if( argc > 3 ||
      ( argc > 2 && !JSVAL_IS_NUMBER( argv[2] ) ) ||
      ( argc > 1 && !JSVAL_IS_NUMBER( argv[1] ) ) ||
      ( argc > 0 && !JSVAL_IS_NUMBER( argv[0] ) ) ) {
    return JS_FALSE;
  }

  RGB v( 0, 0, 0 );

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
  
  *rval = OBJECT_TO_JSVAL( SFColor_newInstance( cx, 
						return_color,
						true ) ); 
  return JS_TRUE;
} 

void SpiderMonkey::SFColor_finalize(JSContext *cx, JSObject *obj) {
  FieldObjectPrivate *private_data = 
    static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  //  cerr << "Finalize SFColor:" << obj << " " << private_data << endl;
  
  // The prototype of SFColor does not have a private member
  if( private_data ) {
    delete private_data;
  }
}


JSBool SpiderMonkey::SFColor_getHSV(JSContext *cx, JSObject *obj, 
				    uintN argc, jsval *argv,
				    jsval *rval) {
  cerr <<  "SFColor_getHSV" << endl;
  return JS_TRUE;
}

JSBool SpiderMonkey::SFColor_setHSV(JSContext *cx, JSObject *obj, 
				    uintN argc, jsval *argv,
				    jsval *rval) {
  cerr << "SFColor_setHSV" << endl;
  return JS_TRUE;
}

JSBool SpiderMonkey::SFColor_toString(JSContext *cx, JSObject *obj, 
				      uintN argc, jsval *argv,
				      jsval *rval) {
  // check that this object is a SFColor_class 
  if (!JS_InstanceOf(cx, obj, &SFColorClass, argv))
    return JS_FALSE;

 FieldObjectPrivate *private_data = 
   static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
 SFColor* this_color = static_cast<SFColor *>(private_data->getPointer());

  // make sure we have a value
  if (!this_color ) return JS_FALSE;

  // return string representation of the Vec3f
  const RGB &rgb = this_color->getValue(); 
  stringstream s;
  s << rgb;
  string str = s.str();
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx,
					      (char *)str.c_str(),
					      str.length() ) );
  return JS_TRUE; 
}

JSBool SpiderMonkey::SFColor_getProperty(JSContext *cx, 
					 JSObject *obj, 
					 jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
    SFColor* this_color = static_cast<SFColor *>(private_data->getPointer());

    const RGB &rgb = getValueNoAccessCheck( this_color );

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

JSBool
SpiderMonkey::SFColor_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  //  cerr << "Set Property SFColor" << endl;
  if (JSVAL_IS_INT(id)) {
    FieldObjectPrivate *private_data = 
      static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
    SFColor* this_color = static_cast<SFColor *>(private_data->getPointer());
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


JSObject *SpiderMonkey::SFColor_newInstance( JSContext *cx,
					     SFColor *field,
					     bool internal_field ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
			   &SFColorClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFColor_properties );
  JS_DefineFunctions(cx, js_field, SFColor_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field,
							       internal_field) );
  //  cerr << "SFColor_newInstance: " << js_field << " " << field->getName() << " " << JS_GetPrivate( cx, js_field ) <<endl;
  return js_field;
}


//////////////////////////////////////////////////////////
// X3DExecutionContext object

void SpiderMonkey::X3DExecutionContext_finalize(JSContext *cx, JSObject *obj) {
  ExecutionContextPrivate *private_data = 
    static_cast<ExecutionContextPrivate *>(JS_GetPrivate(cx,obj));
  
  // The prototype of SFColor does not have a private member
  if( private_data ) {
    delete private_data; 
  }
}


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
  
  SAI::ExecutionContext* context = private_data->getPointer();
  
  // make sure we have a value
  if (!context ) { 
    cerr << private_data << " " << context << endl;
    return JS_TRUE;
  }
  
  cerr << "FDASFDASFDSA" << endl;

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
  //cerr << "Get Property X3DExecutionContext" << endl;
  if (JSVAL_IS_INT(id)) {
    ExecutionContextPrivate *private_data = 
      static_cast<ExecutionContextPrivate *>(JS_GetPrivate(cx,obj));

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
      JS_ReportError(cx, "X3DExecutionObject object does not have property \"%s\".", JS_GetStringBytes( s ) );
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




























































bool SpiderMonkey::insertH3DTypes( JSContext *cx, JSObject *obj ) {
  // TODO: FIX!!
  JSObject *proto = JS_InitClass( cx, obj, NULL, 
				  &SFVec3fClass, SFVec3f_construct, 3, 
				  SFVec3f_properties, SFVec3f_functions, NULL, NULL );
  proto = JS_InitClass( cx, obj, NULL, 
			&SFNodeClass, SFNode_construct, 3, 
			NULL, SFNode_functions, NULL, NULL );
  proto = JS_InitClass( cx, obj, NULL, 
			&SFColorClass, SFColor_construct, 3, 
			SFColor_properties, SFColor_functions, NULL, NULL );

  JS_MFFloat::initClass( cx, obj, "MFFloat" );
  JS_MFDouble::initClass( cx, obj, "MFDouble" );
  JS_MFTime::initClass( cx, obj, "MFTime" );
  JS_MFInt32::initClass( cx, obj, "MFInt32" );
  JS_MFVec3f::initClass( cx, obj, "MFVec3f" );
  JS_MFBool::initClass( cx, obj, "MFBool" );
  JS_MFString::initClass( cx, obj, "MFString" );
  JS_MFNode::initClass( cx, obj, "MFNode" );
  JS_MFColor::initClass( cx, obj, "MFColor" );

  return true;
}

template< class FieldType, JSObject *(*NewInstanceFunc)( JSContext *,
						 	 FieldType *, bool ) >
jsval newInstanceFromField( JSContext *cx,
				Field *field,
				bool make_copy ) {
  FieldType *f = static_cast< FieldType * >( field );
  if( make_copy ) { 
    f = new FieldType;
    f->setValue( static_cast< FieldType * >(field)->getValue() );
  }
  return OBJECT_TO_JSVAL( NewInstanceFunc( cx, f, make_copy ));
}


jsval SpiderMonkey::jsvalFromField( JSContext *cx, 
				    Field *field, bool make_copy ) {

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
    break;
  } 
  case X3DTypes::SFVEC2D: { 
    break;
  } 
  case X3DTypes::SFVEC3F: { 
    return newInstanceFromField< SFVec3f, SFVec3f_newInstance >( cx, 
								 field, 
								 make_copy );
  } 
  case X3DTypes::SFVEC3D: { 
  } 
  case X3DTypes::SFVEC4F: { 
  } 
  case X3DTypes::SFVEC4D: { 
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
    return newInstanceFromField< SFNode, SFNode_newInstance >( cx, 
							       field, 
							       make_copy );
  } 
  case X3DTypes::SFCOLOR: { 
     return newInstanceFromField< SFColor, SFColor_newInstance >( cx, 
								  field, 
								  make_copy );
  } 
  case X3DTypes::SFCOLORRGBA: { 
  } 
  case X3DTypes::SFROTATION: { 
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
    return newInstanceFromField< MFFloat, JS_MFFloat::newInstance >( cx, 
								     field, 
								     make_copy );
  } 
  case X3DTypes::MFDOUBLE: { 
    return newInstanceFromField< MFDouble, JS_MFDouble::newInstance >( cx, 
								       field, 
								       make_copy );
 
  } 
  case X3DTypes::MFTIME: { 
    return newInstanceFromField< MFTime, JS_MFTime::newInstance >( cx, 
								   field, 
								   make_copy );
 
  } 
  case X3DTypes::MFINT32: { 
    return newInstanceFromField< MFInt32, JS_MFInt32::newInstance >( cx, 
								     field, 
								     make_copy );
 
  } 
  case X3DTypes::MFVEC2F: { 
  } 
  case X3DTypes::MFVEC2D: { 
  } 
  case X3DTypes::MFVEC3F: { 
    return newInstanceFromField< MFVec3f, JS_MFVec3f::newInstance >( cx, 
								     field, 
								     make_copy );
 
  } 
  case X3DTypes::MFVEC3D: { 
  }  
  case X3DTypes::MFVEC4F: { 
  } 
  case X3DTypes::MFVEC4D: { 
    break;
  } 
  case X3DTypes::MFBOOL: { 
    return newInstanceFromField< MFBool, JS_MFBool::newInstance >( cx, 
								   field, 
								   make_copy );
  }  
  case X3DTypes::MFSTRING: { 
    return newInstanceFromField< MFString, JS_MFString::newInstance >( cx, 
								       field, 
								       make_copy );
 
  } 
  case X3DTypes::MFNODE: { 
    return newInstanceFromField< MFNode, JS_MFNode::newInstance >( cx, 
								   field, 
								   make_copy );
  } 
  case X3DTypes::MFCOLOR: { 
    return newInstanceFromField< MFColor, JS_MFColor::newInstance >( cx, 
								     field, 
								     make_copy );
 
  } 
  case X3DTypes::MFCOLORRGBA: { 
  } 
  case X3DTypes::MFROTATION: { 
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
      const typename FieldType::vector_type &c = 
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

JSBool SpiderMonkey::setFieldValueFromjsval( JSContext *cx, 
					     Field *field, 
					     jsval value ) {
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
    break;
  } 
  case X3DTypes::SFVEC2D: { 
    break;
  } 
  case X3DTypes::SFVEC3F: { 
    return setFieldValueFromjsvalTmpl< SFVec3f, X3DTypes::SFVEC3F >( cx, field, value ) ;
  } 
  case X3DTypes::SFVEC3D: { 
  } 
  case X3DTypes::SFVEC4F: { 
  } 
  case X3DTypes::SFVEC4D: { 
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
    return MField_setFieldValueFromjsvalTmpl< MFFloat, X3DTypes::MFFLOAT >( cx, 
									    field, 
									    value ) ;
  } 
  case X3DTypes::MFDOUBLE: { 
    return MField_setFieldValueFromjsvalTmpl< MFDouble, X3DTypes::MFDOUBLE >( cx, 
									      field, 
									      value ) ;
  } 
  case X3DTypes::MFTIME: { 
    return MField_setFieldValueFromjsvalTmpl< MFTime, X3DTypes::MFTIME >( cx, 
									  field, 
									  value ) ;
  } 
  case X3DTypes::MFINT32: { 
    return MField_setFieldValueFromjsvalTmpl< MFInt32, X3DTypes::MFINT32 >( cx, 
									    field, 
									    value );
  } 
  case X3DTypes::MFVEC2F: { 
  } 
  case X3DTypes::MFVEC2D: { 
    break;
  } 
  case X3DTypes::MFVEC3F: { 
    return MField_setFieldValueFromjsvalTmpl< MFVec3f, X3DTypes::MFVEC3F >( cx, 
									    field, 
									    value );
  } 
  case X3DTypes::MFVEC3D: { 
  } 
  case X3DTypes::MFVEC4F: { 
  } 
  case X3DTypes::MFVEC4D: { 
    break;
  } 
  case X3DTypes::MFBOOL: { 
    return MField_setFieldValueFromjsvalTmpl< MFBool, X3DTypes::MFBOOL >( cx, field, value ) ;
   
  } 
  case X3DTypes::MFSTRING: { 
    return MField_setFieldValueFromjsvalTmpl< MFString, X3DTypes::MFSTRING >( cx, 
									      field, 
									      value );
  } 
  case X3DTypes::MFNODE: { 
    return MField_setFieldValueFromjsvalTmpl< MFNode, X3DTypes::MFNODE >( cx, field, value ) ;
  } 
  case X3DTypes::MFCOLOR: { 
    return MField_setFieldValueFromjsvalTmpl< MFColor, X3DTypes::MFCOLOR >( cx, field, value ) ;
  } 
  case X3DTypes::MFCOLORRGBA: { 
  } 
  case X3DTypes::MFROTATION: { 
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


JSBool SpiderMonkey::haveFunction( JSContext *cx, 
				   JSObject *obj,
				   const char * name ) { 			
  jsval res;
  // get the property
  JSBool has_property = JS_GetProperty( cx,
					obj,
					name,
					&res );
  
  if( has_property && JSVAL_IS_OBJECT( res ) ) {
    JSObject *fun = JSVAL_TO_OBJECT( res );
    if( JS_ObjectIsFunction( cx, fun ) ) {
      return JS_TRUE;
    }
  }
  return JS_FALSE;
}

#endif // HAVE_SPIDERMONKEY

