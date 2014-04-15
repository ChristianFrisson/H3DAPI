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
#include <H3DUtil/PixelImage.h>
#include<typeinfo>
#include<algorithm>
#include<sstream>
#include<H3D/SFMatrix3f.h>
#include<H3D/SFMatrix4f.h>
#include<H3D/Transform.h>



#ifdef HAVE_SPIDERMONKEY

using namespace H3D;
using namespace H3D::SpiderMonkey;


/// change dimension and preserve old spaces, or fill new spaces with zero
void changePixelImageDimension(PixelImage* img, unsigned int nw, unsigned int nh) {
  unsigned int ow = img->width(); unsigned int oh = img->height();
  if (nw == -1) nw = ow;
  if (nh == -1) nh = oh;
  unsigned char* dataOld = (unsigned char*) img->getImageData();
  unsigned char bytes_per_pixel = img->bitsPerPixel() / 8;
  unsigned char bytes_in_image = nw * nh * bytes_per_pixel;
  unsigned char* dataNew = new unsigned char[bytes_in_image];
  memset(dataNew, 0, bytes_in_image );
  for (int x = 0; x < std::min<int>( nw, ow); ++x) {
    for (int y = 0; y < std::min<int>(nh, oh); ++y) {
      memcpy( (void*)(dataNew + (y*nw+x)*bytes_per_pixel),
              (dataOld + (y * ow + x)*bytes_per_pixel), bytes_per_pixel);
    }
  }
  img->setImageData(dataNew, false);
  img->setWidth(nw);
  img->setHeight(nh);
}

/// when changed comp type, create a new data array
void changePixelImageComponent(PixelImage* img, int comp) {
  std::cout<< img->bitsPerPixel();
  img->setPixelType( (H3DUtil::Image::PixelType)(comp-1) );
  img->setbitsPerPixel(comp * 8);
  unsigned char* data = new unsigned char[img->width() * img->height() * comp ];
  img->setImageData(data, false);
}

template< class FieldType >
void setValueNoAccessCheck( FieldType *field, 
          const typename FieldType::value_type &v ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  field->setValue(v);
  field->setAccessCheck( access );
}

/// return the actual value wrapped inside a JS object
/// Use this function because we currently have 2 ways to represent SField: (SField) or (MField, index)
template<class FieldType>
JSBool getValueInJSObject(JSContext* cx, JSObject* obj, typename FieldType::value_type* rval) {
  typedef MField<typename FieldType::value_type> MFieldType;

  FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, obj ) );

  if ( this_private_data->getArrayIndex() == -1 ) {
    FieldType* sfield = dynamic_cast<FieldType*>( this_private_data->getPointer() );
    *rval = sfield->getValue();
    return JS_TRUE;
  } else {
    MFieldType* mf = dynamic_cast<MFieldType*>( this_private_data->getPointer() );
    *rval = mf->getValue()[ this_private_data->getArrayIndex() ];
    return JS_TRUE;
  }
}

template<class FieldType>
JSBool setValueInJSObject(JSContext* cx, JSObject* obj, typename FieldType::value_type& v) {
  typedef MField<typename FieldType::value_type> MFieldType;
  FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, obj ) );

  if ( this_private_data->getArrayIndex() == -1 ) {
    FieldType* sfield = dynamic_cast<FieldType*>( this_private_data->getPointer() );
    setValueNoAccessCheck( sfield, v );
  } else {
    MFieldType* mf = dynamic_cast<MFieldType*>( this_private_data->getPointer() );
    mf->setValue( this_private_data->getArrayIndex(), v );
  }
  return JS_TRUE;
}

template< class FieldType >
const typename FieldType::value_type getValueNoAccessCheck( FieldType *field ) {
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
void MField_setValueNoAccessCheck( FieldType *field, 
           const typename FieldType::vector_return_type &v ) {
  bool access = field->isAccessCheckOn();
  field->setAccessCheck( false );
  field->setValue( v);
  field->setAccessCheck( access );
}

JSBool SpiderMonkey::FieldObject_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {

  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  if (private_data->getArrayIndex() == -1){ 
    Field* this_field = static_cast<Field *>(private_data->getPointer());
    // make sure we have a value
    if (!this_field ) return JS_FALSE;
  
    ParsableField *pfield = dynamic_cast< ParsableField * >( this_field );
    if( !pfield ) return JSVAL_FALSE;

    // return string representation of the Vec3f
    string s = pfield->getValueAsString();
    *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)s.c_str(), s.length() ) );
    return JS_TRUE;
  } else {
    return JS_FALSE;
  }
}

template<class FieldType>
JSBool SpiderMonkey::FieldObject_toString2(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  if (private_data->getArrayIndex() == -1){ 
    FieldType* this_field = static_cast<FieldType*>(private_data->getPointer());
  
    // return string representation of the Vec3f
    string s = this_field->getValueAsString();
    *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)s.c_str(), s.length() ) );
    return JS_TRUE;
  } else {
    typename FieldType::value_type v;
    if ( !getValueInJSObject<FieldType>( cx, obj, &v ) ) return JS_FALSE;

    FieldType* f = new FieldType( v );
    string s = f->getValueAsString();
    delete f;
    *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)s.c_str(), s.length() ) );
    return JS_TRUE;
  }
} 


/// Callback getter function for properties of a JS_MField
/// object.
template< class MFieldType, class ElementType >
JSBool SpiderMonkey::JS_MField<MFieldType, ElementType>::getProperty(JSContext *cx, JSObject *obj, 
                                                         jsval id, jsval *vp) {

  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate*>(JS_GetPrivate(cx,obj));

  if (private_data->getPointer()->getX3DType() == X3DTypes::SFIMAGE) {
    SFImage* sfimg = dynamic_cast<SFImage*>( private_data->getPointer() );
    PixelImage* img = dynamic_cast<PixelImage*>( sfimg->getValue() );

    if ( JSVAL_IS_INT(id) ) {
      int index = JSVAL_TO_INT(id);

      int intval;
      if (index == 0) intval = img->width();
      else if (index == 1) intval = img->height();
      else if (index == 2) intval = (int)img->pixelType() + 1;
      else {
        index = index - 3;
        int ix = index % img->width();
        int iy = index / img->width();
        RGBA color = img->getPixel( ix, iy );
        unsigned char bytes_per_pixel = img->pixelType() + 1;
        unsigned char* data = new unsigned char[ bytes_per_pixel ];
        img->RGBAToImageValue(color, (void*) data);
        intval = 0;
        for (int i = 0; i < bytes_per_pixel; ++i) {
          intval = intval << 8;
          intval = intval | data[i];
        }
        delete data;
      }
      jsval aa;
      JS_NewNumberValue( cx, intval, &aa);
      *vp = aa;

      return JS_TRUE;
    }
    return JS_FALSE;
  }

  MFieldType* mfield = dynamic_cast<MFieldType *>(private_data->getPointer());

  if (JSVAL_IS_INT(id)) {
    // value is integer so it is an array index
    JSBool res = JS_TRUE;
    int index = JSVAL_TO_INT(id);

    if( index < (int)mfield->size() ) {
      ElementType sfield;
      *vp = jsvalFromField(cx, mfield, false, index, sfield.getX3DType() );
    } else {
      return JS_FALSE;
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
JSBool SpiderMonkey::JS_MField< MFieldType, ElementType >::construct(JSContext *cx, 
                                           JSObject *obj, uintN argc, 
                                           jsval *argv, jsval *rval) {

  // the constructor arguments should be a sequence of jsval representing
  // a number of ElementType values. 

  MFieldType *mfield = new MFieldType;
  auto_ptr< ElementType > element( new ElementType );
  for( unsigned int i = 0; i < argc; ++i ) {
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
  *rval = OBJECT_TO_JSVAL( (JS_MField< MFieldType, ElementType >::newInstance( cx, mfield, true )) ); 
  return JS_TRUE;
}
  


/// Callback setter function for properties of a JS_MField
/// object.
template< class MFieldType, class ElementType >
JSBool SpiderMonkey::JS_MField< MFieldType, ElementType >::setProperty(JSContext *cx, JSObject *obj, 
                                                         jsval id, jsval *vp) {
  //      cerr << "MFNode setProperty: "<< endl;
  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));

  
  if (private_data->getPointer()->getX3DType() == X3DTypes::SFIMAGE) {

    SFImage* sfimg = dynamic_cast<SFImage*>( private_data->getPointer() );
    PixelImage* img = (PixelImage*) sfimg->getValue();

    int intval = JSVAL_TO_INT( *vp );

    if ( JSVAL_IS_INT(id) ) {
      int index = JSVAL_TO_INT(id);
      
      if (index == 0) changePixelImageDimension(img, intval, -1);
      else if (index == 1) changePixelImageDimension(img, -1, intval);
      else if (index == 2) changePixelImageComponent(img, intval );
      else if( index < 0 ) {
        stringstream s;
        s << "Index out of SFImage.array bounds. Index value is" << index;
        JS_ReportError(cx, s.str().c_str() );
        return JS_FALSE;
      } else {
        index = index - 3;
        if( index >= (int)(img->width() * img->height()) ) {
          stringstream s;
        s << "Index out of SFImage.array bounds. Index value is" << index + 3;
        JS_ReportError(cx, s.str().c_str() );
        return JS_FALSE;
        }
        int ix = index % img->width();
        int iy = index / img->width();
        unsigned char bytes_per_pixel = img->pixelType() + 1;
        unsigned char* data = new unsigned char[ bytes_per_pixel ];
        for (int i = bytes_per_pixel-1; i >= 0; --i) {
          data[i] = intval & 0x0000FF;
          intval = intval >> 8;
        }
        RGBA color = img->imageValueToRGBA((void*) data);
        delete data;
        //sfimg->beginEditing();
        img->setPixel(color, ix, iy);
        //sfimg->endEditing();
        sfimg->touch();
      }
      //sfimg->touch();
      //std::cout<< "touched!" << std::endl;

      return JS_TRUE;
    }
    return JS_FALSE;
  }

  MFieldType* mfield = static_cast<MFieldType *>(private_data->getPointer());
  if (JSVAL_IS_INT(id)) {
    // value is integer so it is an array index
    JSBool res = JS_TRUE;
    int index = JSVAL_TO_INT(id);

    // resize mfield if it is not big enough.
    if( index >= (int)mfield->size() ) {
      mfield->resize( index+1 );
    }

    //// retrieve the value to n
    auto_ptr< ElementType > n( new ElementType );

    if (setFieldValueFromjsval( cx, n.get(), *vp ) ) {
      mfield->setValue( index, n->getValue() );  
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
    delete rp;
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
  SFVec2f* rp = new SFVec2f();
  if ( !helper_calculate2_TTT<SFVec2f>( cx, obj, &argv[0], rp, O_SUBTRACT )) {
    delete rp;
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2f_newInstance( cx, rp, true ) );
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
    delete rp;
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
    delete rp;
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
  //cerr << "Get Property SFVec2f" << endl;
  if (JSVAL_IS_INT(id)) {
    Vec2f v;
    if ( !getValueInJSObject<SFVec2f>( cx, obj, &v ) ) return JS_FALSE;

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
  if (JSVAL_IS_INT(id)) {
    Vec2f v;
    if ( !getValueInJSObject<SFVec2f>( cx, obj, &v ) )
      return JS_FALSE;

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
      if ( !setValueInJSObject<SFVec2f>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFVec2f_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFVec2fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec2f_properties );
  JS_DefineFunctions(cx, js_field, SFVec2f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                     internal_field, array_index ) );
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
    delete rp;
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
  SFVec2d* rp = new SFVec2d();
  if ( !helper_calculate2_TTT<SFVec2d>( cx, obj, &argv[0], rp, O_SUBTRACT )) {
    delete rp;
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec2d_newInstance( cx, rp, true ) );
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
    delete rp;
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
    delete rp;
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
  //cerr << "Get Property SFVec2d" << endl;
  if (JSVAL_IS_INT(id)) {
    Vec2d v;
    if ( !getValueInJSObject<SFVec2d>( cx, obj, &v ) ) return JS_FALSE;

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
  if (JSVAL_IS_INT(id)) {
    Vec2d v;
    if ( !getValueInJSObject<SFVec2d>( cx, obj, &v ) )
      return JS_FALSE;

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
      if ( !setValueInJSObject<SFVec2d>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFVec2d_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
         &SFVec2dClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec2d_properties );
  JS_DefineFunctions(cx, js_field, SFVec2d_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                     internal_field, array_index ) );
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
    delete rp;
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
  SFVec3f* rp = new SFVec3f();
  if ( !helper_calculate2_TTT<SFVec3f>( cx, obj, &argv[0], rp, O_SUBTRACT )) {
    delete rp;
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3f_newInstance( cx, rp, true ) );
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
    delete rp;
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
    delete rp;
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
  //cerr << "Get Property SFVec3f" << endl;
  if (JSVAL_IS_INT(id)) {
    Vec3f v;
    if ( !getValueInJSObject<SFVec3f>( cx, obj, &v ) ) return JS_FALSE;

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
  if (JSVAL_IS_INT(id)) {
    Vec3f v;
    if ( !getValueInJSObject<SFVec3f>( cx, obj, &v ) )
      return JS_FALSE;

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
      if ( !setValueInJSObject<SFVec3f>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFVec3f_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
         &SFVec3fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec3f_properties );
  JS_DefineFunctions(cx, js_field, SFVec3f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                     internal_field,  array_index ) );
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
    delete rp;
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
  SFVec3d* rp = new SFVec3d();
  if ( !helper_calculate2_TTT<SFVec3d>( cx, obj, &argv[0], rp, O_SUBTRACT )) {
    delete rp;
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec3d_newInstance( cx, rp, true ) );
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
    delete rp;
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
    delete rp;
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
  //cerr << "Get Property SFVec3d" << endl;
  if (JSVAL_IS_INT(id)) {
    Vec3d v;
    if ( !getValueInJSObject<SFVec3d>( cx, obj, &v ) ) return JS_FALSE;

    
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

  if (JSVAL_IS_INT(id)) {
    Vec3d v;
    if ( !getValueInJSObject<SFVec3d>( cx, obj, &v ) )
      return JS_FALSE;

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
      if ( !setValueInJSObject<SFVec3d>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFVec3d_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, 
         &SFVec3dClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec3d_properties );
  JS_DefineFunctions(cx, js_field, SFVec3d_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                     internal_field ,  array_index) );
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
    delete rp;
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
  SFVec4f* rp = new SFVec4f();
  if ( !helper_calculate2_TTT<SFVec4f>( cx, obj, &argv[0], rp, O_SUBTRACT )) {
    delete rp;
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4f_newInstance( cx, rp, true ) );
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
    delete rp;
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
    delete rp;
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
  if (JSVAL_IS_INT(id)) {
    Vec4f v;
    if ( !getValueInJSObject<SFVec4f>( cx, obj, &v ) ) return JS_FALSE;

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
  if (JSVAL_IS_INT(id)) {
    Vec4f v;
    if ( !getValueInJSObject<SFVec4f>( cx, obj, &v ) )
      return JS_FALSE;

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
      if ( !setValueInJSObject<SFVec4f>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFVec4f_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFVec4fClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec4f_properties );
  JS_DefineFunctions(cx, js_field, SFVec4f_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                     internal_field,  array_index ) );
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
    delete rp;
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
  SFVec4d* rp = new SFVec4d();
  if ( !helper_calculate2_TTT<SFVec4d>( cx, obj, &argv[0], rp, O_SUBTRACT )) {
    delete rp;
    return JS_FALSE;
  }
  // now convert to jsval and return it
  *rval = OBJECT_TO_JSVAL( SFVec4d_newInstance( cx, rp, true ) );
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
    delete rp;
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
    delete rp;
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
  //cerr << "Get Property SFVec4d" << endl;
  if (JSVAL_IS_INT(id)) {
    Vec4d v;
    if ( !getValueInJSObject<SFVec4d>( cx, obj, &v ) ) return JS_FALSE;

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
  if (JSVAL_IS_INT(id)) {
    Vec4d v;
    if ( !getValueInJSObject<SFVec4d>( cx, obj, &v ) )
      return JS_FALSE;

    JSBool res = JS_TRUE;
    switch (JSVAL_TO_INT(id)) {
    case SFVEC4D_X: {
      jsdouble x;
      res = JS_ValueToNumber( cx, *vp, &x );
      v.x = x;
      break;
    }
    case SFVEC4D_Y: {
      jsdouble y;
      res = JS_ValueToNumber( cx, *vp, &y );
      v.y = y;
      break;
    }
    case SFVEC4D_Z: {
      jsdouble z;
      res = JS_ValueToNumber( cx, *vp, &z );
      v.z = z;
      break;
    }
    case SFVEC4D_W: {
      jsdouble w;
      res = JS_ValueToNumber( cx, *vp, &w );
      v.w = w;
      break;
    }
    }
    if( !res ) {
      JS_ReportError(cx, "SFVec4d attribute must be convertable to Number" );
      return JS_FALSE;
    } else {
      if ( !setValueInJSObject<SFVec4d>( cx, obj, v ) ) return JS_FALSE;
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
      JS_ReportError( cx, "Field object does not have property \"%s\".",
                      JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFVec4d_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFVec4dClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFVec4d_properties );
  JS_DefineFunctions(cx, js_field, SFVec4d_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, 
                     internal_field, array_index ) );
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
    Rotation v;
    if ( !getValueInJSObject<SFRotation>( cx, obj, &v ) )
      return JS_FALSE;    
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
      if ( !setValueInJSObject< SFRotation>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFRotation_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFRotationClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFRotation_properties );
  JS_DefineFunctions(cx, js_field, SFRotation_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field , array_index) );
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

    //if (field_name != "prepareEvents") {
    //  cerr << "Get Property SFNode: " << field_name << " " << obj << endl;
    //}
    //if (field_name == "image") {
    //  cerr<< "";
    //}

    SFNode* node_field = static_cast<SFNode *>(private_data->getPointer());
    Node *n = node_field->getValue();
    Field *f = NULL;
    if( n ) f = n->getField( field_name );
    if( f ) *vp = jsvalFromField( cx, f, false );
  }

  return JS_TRUE;
}

JSBool SpiderMonkey::SFNode_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  // TODO: if we use MFNode in anycase?

  FieldObjectPrivate *private_data = static_cast<FieldObjectPrivate *>(JS_GetPrivate(cx,obj));
  
  // make sure the object have not been manually disposed by the user.
  if( private_data->isDisposed()  ) {
    setJSException( cx, SAI::SAIError::SAI_DISPOSED );
    return JS_FALSE;
  }

  if( JSVAL_IS_STRING( id ) ) {
    JSString *s = JSVAL_TO_STRING( id );
    string field_name = JS_GetStringBytes( s );

    //std::cout << "Set Property SFNode: " << field_name << " " << obj << endl;
    SFNode* node_field = static_cast<SFNode *>(private_data->getPointer());
    Node *n = node_field->getValue();
    Field *f = NULL;
    if( n ) f = n->getField( field_name );
    if( f ) return setFieldValueFromjsval( cx, f, *vp );
  }
  return JS_TRUE;
}


JSObject *SpiderMonkey::SFNode_newInstance( JSContext *cx,Field *field,bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFNodeClass, NULL, NULL );  

  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field,
                     internal_field, array_index) );
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

JSBool SpiderMonkey::SFColor_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  //FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, obj ) );
  //if ( this_private_data->isDisposed() ) {
  //  setJSException( cx, SAI::SAIError::SAI_DISPOSED );
  //  return 0;
  //}

  if (JSVAL_IS_INT(id)) {
    RGB rgb;
    if ( !getValueInJSObject<SFColor>( cx, obj, &rgb ) ) return JS_FALSE;

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
  if (JSVAL_IS_INT(id))
  {
    RGB v;
    if ( !getValueInJSObject<SFColor>( cx, obj, &v ) )
      return JS_FALSE;

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
      if ( !setValueInJSObject<SFColor>( cx, obj, v ) ) return JS_FALSE;
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

JSObject *SpiderMonkey::SFColor_newInstance( JSContext *cx, Field *field, bool internal_field, int array_index ) {
  JSObject *js_field;

  js_field = JS_NewObject( cx, &SFColorClass, NULL, NULL );  

  JS_DefineProperties(cx, js_field, SFColor_properties );
  JS_DefineFunctions(cx, js_field, SFColor_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field,
                     internal_field, array_index) );
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

  // make the string
  MFInt32* arr_obj = helper_extractPrivateObject<MFInt32>( cx, JSVAL_TO_OBJECT(argv[3]) );
  vector<int> arr_v = arr_obj->getValue();
  stringstream ss;
  ss<< x << " " << y << " " << comp;
  for (size_t i = 0; i < arr_v.size(); ++i) {
    ss<< " " << arr_v[i];
  }

  SFImage* sfimg = new SFImage();
  sfimg->setValueFromString( ss.str() ) ;


  *rval = OBJECT_TO_JSVAL( SFImage_newInstance( cx,  sfimg, true ) ); 

  return JS_TRUE;
}

JSBool SpiderMonkey::SFImage_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  SFImage* sfimg = helper_extractPrivateObject<SFImage>( cx, obj );
  // PixelImage is child of Image class.
  PixelImage* img = dynamic_cast<PixelImage*> ( sfimg->getValue() );

  if (sfimg == 0) return JS_FALSE;

  if (JSVAL_IS_INT(id)) {
    switch (JSVAL_TO_INT(id))
    {
      case SFImage_WIDTH:
        *vp = INT_TO_JSVAL(img->width());
        return JS_TRUE;
      case SFImage_HEIGHT:
        *vp = INT_TO_JSVAL(img->height());
        return JS_TRUE;
      case SFImage_COMP:
        *vp = INT_TO_JSVAL( ((int)img->pixelType()) + 1 );
        return JS_TRUE;
      case SFImage_ARRAY:

        JSObject* r_jsobj = JS_NewObject(cx, &JS_MFInt32::js_class, NULL, NULL);
        JS_SetPrivate(cx, r_jsobj, (void *) new FieldObjectPrivate( sfimg , false) );
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
  SFImage* sfimg = helper_extractPrivateObject<SFImage>( cx, obj );
  PixelImage* img = dynamic_cast<PixelImage*>( sfimg->getValue() );

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
        changePixelImageDimension(img, i, -1);
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
        changePixelImageDimension(img, -1, i);
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
        changePixelImageComponent(img, i );
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

        stringstream ss;
        ss<< img->width() << "" << img->height() << " " << (int)img->pixelType() + 1;
        for( unsigned int i = 0; i < vals.size(); ++i ) {
          ss<< " " << vals[i];
        }
        string s = ss.str();
        sfimg->setValueFromString( s );

        break;
    }
    sfimg->touch();
    //setValueNoAccessCheck(sfimg, ip);
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

JSObject *SpiderMonkey::SFImage_newInstance( JSContext *cx, /*SFImage*/ Field *field, bool internal_field, int array_index ) {
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

  
  if (JSVAL_IS_OBJECT( argv[0] )) {
    JSObject* print_obj = JSVAL_TO_OBJECT(argv[0]);
    if (JS_InstanceOf(cx, print_obj, &SFImageClass, argv)) {
      std::cout<<"sfimage";
    }
  }

  //FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, print_obj ) );
  //Field* sfield = dynamic_cast<Field*>( this_private_data->getPointer() );

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
    c->clean_up = true;
    *vp = OBJECT_TO_JSVAL(X3DExecutionContext_newInstance( cx, c, true ));
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

  JS_InitClass( cx, obj, NULL, &X3DMatrix3Class, X3DMatrix3_construct, 3,
    X3DMatrix3_properties, X3DMatrix3_functions, NULL, NULL);

  JS_InitClass( cx, obj, NULL, &SFMatrix3fRowClass, SFMatrix3fRow_construct, 3,
    SFMatrix3fRow_properties, SFMatrix3fRow_functions, NULL, NULL);

  JS_InitClass( cx, obj, NULL, &X3DMatrix4Class, X3DMatrix4_construct, 3,
    X3DMatrix4_properties, X3DMatrix4_functions, NULL, NULL);

  JS_InitClass( cx, obj, NULL, &SFMatrix4fRowClass, SFMatrix4fRow_construct, 3,
    SFMatrix4fRow_properties, SFMatrix4fRow_functions, NULL, NULL);

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
  //JS_MFImage::initClass( cx, obj, "MFImage" );

  return true;
}

template< class FieldType, JSObject *(*NewInstanceFunc)( JSContext *, Field *, bool, int ) >
jsval newInstanceFromField( JSContext *cx, Field *field, bool make_copy, int array_index) {
  FieldType *f = static_cast< FieldType * >( field );
  if( make_copy ) { 
    f = new FieldType;
    f->setValue( (static_cast< FieldType * >(field))->getValue() );
  }
  return OBJECT_TO_JSVAL( NewInstanceFunc( cx, f, make_copy, array_index ));
}

template<class FieldType>
jsval newNumberInstanceFromField( JSContext *cx, Field *field, int array_index) {
  // make_copy doesn't apply here!
  if (array_index != -1) {
    MField<typename FieldType::value_type>* mf = 
      dynamic_cast< MField<typename FieldType::value_type>* >( field );
    jsdouble v = mf->getValue()[array_index];
    jsval rval;
    JS_NewNumberValue( cx, v, &rval);
    return rval;
  } else {
    FieldType *f = static_cast< FieldType * >( field );
    jsdouble v = getValueNoAccessCheck( f );
    jsval rval;
    JS_NewNumberValue( cx, v, &rval);
    return rval;
  }
}

jsval SpiderMonkey::jsvalFromField( JSContext *cx, Field *field, bool make_copy, 
                                   int array_index, X3DTypes::X3DType x3dt ) {

  X3DTypes::X3DType x3d_type = field->getX3DType();

  if (array_index != -1) {
    x3d_type = x3dt;
  }

  switch( x3d_type ) { 
  case X3DTypes::SFFLOAT: { 
    return newNumberInstanceFromField<SFFloat>( cx, field, array_index );
  } 
  case X3DTypes::SFDOUBLE: { 
    //return a;
    return newNumberInstanceFromField<SFDouble>( cx, field, array_index );
  } 
  case X3DTypes::SFTIME: { 
    //return a;
    return newNumberInstanceFromField<SFTime>( cx, field, array_index );
  } 
  case X3DTypes::SFINT32: { 
    //return a;
    return newNumberInstanceFromField<SFInt32>( cx, field, array_index );
  } 
  case X3DTypes::SFVEC2F: { 
    return newInstanceFromField< SFVec2f, SFVec2f_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFVEC2D: { 
    return newInstanceFromField< SFVec2d, SFVec2d_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFVEC3F: { 
    return newInstanceFromField< SFVec3f, SFVec3f_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFVEC3D: { 
    return newInstanceFromField< SFVec3d, SFVec3d_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFVEC4F: { 
    return newInstanceFromField< SFVec4f, SFVec4f_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFVEC4D: { 
    return newInstanceFromField< SFVec4d, SFVec4d_newInstance >( cx, field, make_copy, array_index );
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
    return newInstanceFromField< SFNode, SFNode_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFCOLOR: { 
     return newInstanceFromField< SFColor, SFColor_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFCOLORRGBA: { 
  } 
  case X3DTypes::SFROTATION: { 
    return newInstanceFromField< SFRotation, SFRotation_newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::SFIMAGE: { 
    return newInstanceFromField< SFImage, SFImage_newInstance>( cx, field, make_copy, array_index );
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
    return newInstanceFromField< MFFloat, JS_MFFloat::newInstance >( cx, field, make_copy, array_index );
  } 
  case X3DTypes::MFDOUBLE: { 
    return newInstanceFromField< MFDouble, JS_MFDouble::newInstance >( cx, field, make_copy, array_index );
 
  } 
  case X3DTypes::MFTIME: { 
    return newInstanceFromField< MFTime, JS_MFTime::newInstance >( cx, field, make_copy, array_index  );
 
  } 
  case X3DTypes::MFINT32: { 
    return newInstanceFromField< MFInt32, JS_MFInt32::newInstance >( cx, field, make_copy, array_index  );
 
  } 
  case X3DTypes::MFVEC2F: { 
    return newInstanceFromField< MFVec2f, JS_MFVec2f::newInstance >( cx, field, make_copy, array_index  );
  } 
  case X3DTypes::MFVEC2D: { 
    return newInstanceFromField< MFVec2d, JS_MFVec2d::newInstance >( cx, field, make_copy, array_index  );
  } 
  case X3DTypes::MFVEC3F: { 
    return newInstanceFromField< MFVec3f, JS_MFVec3f::newInstance >( cx, field, make_copy, array_index  );
  } 
  case X3DTypes::MFVEC3D: { 
    return newInstanceFromField< MFVec3d, JS_MFVec3d::newInstance >( cx, field, make_copy , array_index );
  }
  case X3DTypes::MFVEC4F: { 
    return newInstanceFromField< MFVec4f, JS_MFVec4f::newInstance >( cx, field, make_copy, array_index  );
  } 
  case X3DTypes::MFVEC4D: { 
    return newInstanceFromField< MFVec4d, JS_MFVec4d::newInstance >( cx, field, make_copy, array_index  );
  } 
  case X3DTypes::MFBOOL: { 
    return newInstanceFromField< MFBool, JS_MFBool::newInstance >( cx, field, make_copy , array_index );
  }  
  case X3DTypes::MFSTRING: { 
    return newInstanceFromField< MFString, JS_MFString::newInstance >( cx, field, make_copy , array_index );
 
  } 
  case X3DTypes::MFNODE: { 
    return newInstanceFromField< MFNode, JS_MFNode::newInstance >( cx, field, make_copy, array_index  );
  } 
  case X3DTypes::MFCOLOR:
    return newInstanceFromField< MFColor, JS_MFColor::newInstance >( cx, field, make_copy , array_index );
  case X3DTypes::MFROTATION:
    return newInstanceFromField< MFRotation, JS_MFRotation::newInstance >( cx, field, make_copy , array_index );
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
  case X3DTypes::MFIMAGE: {
  }
  case X3DTypes::UNKNOWN_X3D_TYPE: {
  }
  } 
  return JSVAL_VOID;
}


template< class FieldType, X3DTypes::X3DType x3d_type >
JSBool setFieldValueFromjsvalTmpl( JSContext *cx, Field *field, jsval value ) {
  FieldType *f = static_cast< FieldType * >( field );

  if( JSVAL_IS_OBJECT( value ) ) {

    JSObject *js_object = JSVAL_TO_OBJECT( value );
    typename FieldType::value_type v;
    if ( !getValueInJSObject<FieldType>(cx, js_object, &v) ) {
      return JS_FALSE;
    }
    setValueNoAccessCheck<FieldType>( f, v );
    return JS_TRUE;

   // Field *value_field = private_data->getPointer();
   // if( value_field->getX3DType() == x3d_type ) {
   //   const typename FieldType::value_type &c = 
    //getValueNoAccessCheck( static_cast< FieldType * >( value_field ) );
   //   setValueNoAccessCheck( f, c ); 
   //   return JS_TRUE;
   // }
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


template< class FieldType, class JSType, 
          JSBool (*ConvertFunc)( JSContext *, jsval, JSType *) >
JSBool setFieldValueFromjsvalSimple( JSContext *cx, Field *field, jsval value ) {
  // check if value is JSObject, if it's JSObject still, then it must be (mf, index)
  if (JSVAL_IS_OBJECT( value ) ) {
    typename FieldType::value_type v;
    if ( !getValueInJSObject<FieldType>(cx, JSVAL_TO_OBJECT(value), &v) ) {
      return JS_FALSE;
    }
    FieldType *f = static_cast< FieldType * >( field );
    setValueNoAccessCheck( f, v);
    return JS_TRUE;
  }

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
      } else if( value_field->getX3DType() == X3DTypes::MFNODE && private_data->getArrayIndex() != -1 ) {
        Node *n = MField_getValueNoAccessCheck( static_cast< MFNode * >( value_field ) )[private_data->getArrayIndex()];
        setValueNoAccessCheck( f, n ); 
        return JS_TRUE;
      }
    }

    stringstream s;
    s << "Invalid type in assignment. Expecting SFNode.";
    JS_ReportError(cx, s.str().c_str() );
    return setFieldValueFromjsvalTmpl< SFNode, X3DTypes::SFNODE >( cx, field, value ) ;
  } 
  case X3DTypes::SFCOLOR: { 
    return setFieldValueFromjsvalTmpl< SFColor, X3DTypes::SFCOLOR >( cx, field, value ) ;
  } 
  case X3DTypes::SFIMAGE: { 
    return setFieldValueFromjsvalTmpl< SFImage, X3DTypes::SFIMAGE >( cx, field, value ) ;
  } 
  case X3DTypes::SFCOLORRGBA: { 
  } 
  case X3DTypes::SFROTATION:
    return setFieldValueFromjsvalTmpl< SFRotation, X3DTypes::SFROTATION>( cx, field, value ) ;
  



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
  case X3DTypes::MFIMAGE: {
  }
  case X3DTypes::UNKNOWN_X3D_TYPE: {
  }  }

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
  for (uintN i = 0; i < argc; ++i) {
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
    if ( num == 0 ) {
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
T* SpiderMonkey::helper_extractPrivateObject( JSContext* cx, JSObject* jsobj) {
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

JSBool SpiderMonkey::SFColor_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFColor>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFRotation_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFRotation>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFVec2f_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFVec2f>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFVec2d_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFVec2d>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFVec3f_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFVec3f>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFVec3d_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFVec3d>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFVec4f_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFVec4f>( cx, obj, argc, argv, rval);
}
JSBool SpiderMonkey::SFVec4d_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString2<SFVec4d>( cx, obj, argc, argv, rval);
}

JSBool SpiderMonkey::SFImage_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return FieldObject_toString( cx, obj, argc, argv, rval);
}


//======================================================
/// X3DMATRIX3
//======================================================


JSBool SpiderMonkey::X3DMatrix3_getTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  JS_ReportError(cx, "getTransform is not implemented.");
  return JS_FALSE;
}

JSBool SpiderMonkey::X3DMatrix3_setTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  JS_ReportError(cx, "setTransform is not implemented.");
  return JS_FALSE;
}

JSBool SpiderMonkey::X3DMatrix3_inverse(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &X3DMatrix3Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }
  SFMatrix3f* sfmatrix = helper_extractPrivateObject<SFMatrix3f>( cx, obj );
  SFMatrix3f* sfrval = new SFMatrix3f( sfmatrix->getValue().inverse() );
  *rval = OBJECT_TO_JSVAL(X3DMatrix3_newInstance(cx, sfrval , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_transpose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {

  // expecting 0 argument
  JSClass* type_classes[] = { &X3DMatrix3Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFMatrix3f* sfmatrix = helper_extractPrivateObject<SFMatrix3f>( cx, obj );
  SFMatrix3f* sfrval = new SFMatrix3f( sfmatrix->getValue().transpose() );
  *rval = OBJECT_TO_JSVAL(X3DMatrix3_newInstance(cx, sfrval , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_multLeft(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // arg * obj

  // expecting 1 argument of class X3DMatrix3
  JSClass* type_classes[] = { &X3DMatrix3Class, &X3DMatrix3Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFMatrix3f* sfobj = helper_extractPrivateObject<SFMatrix3f>( cx, obj );
  SFMatrix3f* sfarg = helper_extractPrivateObject<SFMatrix3f>( cx, JSVAL_TO_OBJECT(argv[0]) );
  SFMatrix3f* sfans = new SFMatrix3f(sfarg->getValue() * sfobj->getValue());

  *rval = OBJECT_TO_JSVAL(X3DMatrix3_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_multRight(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // obj * arg

  // expecting 1 argument of class X3DMatrix3
  JSClass* type_classes[] = { &X3DMatrix3Class, &X3DMatrix3Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFMatrix3f* sfobj = helper_extractPrivateObject<SFMatrix3f>( cx, obj );
  SFMatrix3f* sfarg = helper_extractPrivateObject<SFMatrix3f>( cx, JSVAL_TO_OBJECT(argv[0]) );
  SFMatrix3f* sfans = new SFMatrix3f(sfobj->getValue() * sfarg->getValue());

  *rval = OBJECT_TO_JSVAL(X3DMatrix3_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_multVecMatrix(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class SFVec3f
  JSClass* type_classes[] = { &X3DMatrix3Class, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // row vector
  SFVec3f* sfarg = helper_extractPrivateObject<SFVec3f>(cx, JSVAL_TO_OBJECT(argv[0]));
  SFMatrix3f* sfobj = helper_extractPrivateObject<SFMatrix3f>(cx, obj);

  // row_vec * matrix = 1x3matrix
  Matrix3f m = sfobj->getValue();
  Vec3f v = sfarg->getValue();
  Vec3f ans(v * m.getColumn(0), v * m.getColumn(1), v * m.getColumn(2) );
  SFVec3f* sfans = new SFVec3f(ans);

  *rval = OBJECT_TO_JSVAL(SFVec3f_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_multMatrixVec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
 // expecting 1 argument of class SFVec3f
  JSClass* type_classes[] = { &X3DMatrix3Class, &SFVec3fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // col vector
  SFVec3f* sfarg = helper_extractPrivateObject<SFVec3f>(cx, JSVAL_TO_OBJECT(argv[0]));
  SFMatrix3f* sfobj = helper_extractPrivateObject<SFMatrix3f>(cx, obj);

  // matrix * col vector = 3x1matrix
  Matrix3f m = sfobj->getValue();
  Vec3f v = sfarg->getValue();
  Vec3f ans(m.getRow(0) * v, m.getRow(1) * v, m.getRow(2) * v);
  SFVec3f* sfans = new SFVec3f(ans);

  *rval = OBJECT_TO_JSVAL(SFVec3f_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_toString(JSContext *cx, JSObject *obj,
                                         uintN argc, jsval *argv, jsval *rval) {
  SFMatrix3f* sfmatrix = helper_extractPrivateObject<SFMatrix3f>( cx, obj );
  string s = sfmatrix->getValueAsString();
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)s.c_str(), s.length() ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    int row = JSVAL_TO_INT(id);
    SFMatrix3f* sfobj = helper_extractPrivateObject<SFMatrix3f>( cx, obj );
    if (row < 0 || row > 2) {
      JS_ReportError(cx, "Index out of bound.");
      return JS_FALSE;
    }
    SFMatrix3fRow* sfans = new SFMatrix3fRow(sfobj, row);
    *vp = OBJECT_TO_JSVAL(SFMatrix3fRow_newInstance(cx, sfans, true) );
    return JS_TRUE;
  } else {
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}
JSBool SpiderMonkey::X3DMatrix3_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  // TODO
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix3_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  //check that we have enough arguments and that they are of
  //correct type.
  if (argc != 9) {
    return JS_FALSE;
  }
  float a[9];
  for (size_t i = 0; i < argc; ++i) {
    jsdouble r;
    if (!JS_ValueToNumber( cx, argv[i], &r) ) {
      std::cout<< "failed in construct" << std::endl;
      return JS_FALSE;
    }
    a[i] = (H3DFloat) r;
  }
  Matrix3f m( a[0], a[1], a[2],
              a[3], a[4], a[5],
              a[6], a[7], a[8] );
  SFMatrix3f* sfm = new SFMatrix3f(m);
  *rval = OBJECT_TO_JSVAL( X3DMatrix3_newInstance( cx, sfm, true ) ); 
  return JS_TRUE;
}

JSObject *SpiderMonkey::X3DMatrix3_newInstance( JSContext *cx, Field *field, bool internal_field) {
  JSObject *js_field;
  js_field = JS_NewObject( cx, &X3DMatrix3Class, NULL, NULL );  
  JS_DefineProperties(cx, js_field, X3DMatrix3_properties );
  JS_DefineFunctions(cx, js_field, X3DMatrix3_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field ) );
  return js_field; 
}

JSBool SpiderMonkey::SFMatrix3fRow_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // not supposed to implement this because SFMatrix4fRow type
  // is assumed to be hidden

  return JS_TRUE;
}

JSBool SpiderMonkey::SFMatrix3fRow_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    int col = JSVAL_TO_INT(id);
    SFMatrix3fRow* sfobj = helper_extractPrivateObject<SFMatrix3fRow>( cx, obj );
    if (col < 0 || col > 2) {
      JS_ReportError(cx, "Column index out of bound.");
      return JS_FALSE;
    }
    jsdouble jsd; JSBool res = JS_ValueToNumber( cx, *vp, &jsd );
    float f = (H3DFloat) jsd;
    Matrix3f m = sfobj->sfmatrix->getValue();
    m.setElement(sfobj->row, col, f);
    sfobj->sfmatrix->setValue(m);
    return JS_TRUE;
  } else {
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFMatrix3fRow_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    int col = JSVAL_TO_INT(id);
    SFMatrix3fRow* sfobj = helper_extractPrivateObject<SFMatrix3fRow>( cx, obj );
    if (col < 0 || col > 2) {
      JS_ReportError(cx, "Column index out of bound.");
      return JS_FALSE;
    }
    float r = sfobj->sfmatrix->getValue().getElement(sfobj->row, col);
    JS_NewNumberValue( cx, r, vp ); 
    return JS_TRUE;
  } else {
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFMatrix3fRow_newInstance( JSContext *cx, Field *field, bool internal_field ) {
  JSObject *js_field;
  js_field = JS_NewObject( cx, &SFMatrix3fRowClass, NULL, NULL );  
  JS_DefineProperties(cx, js_field, SFMatrix3fRow_properties );
  JS_DefineFunctions(cx, js_field, SFMatrix3fRow_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field ) );
  return js_field; 
}

JSBool SpiderMonkey::SFMatrix3fRow_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return JS_TRUE;
}


//======================================================
/// X3DMatrix4
//======================================================


JSBool SpiderMonkey::X3DMatrix4_getTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
  Matrix4f mobj = sfobj->getValue();

  // translation
  if (argc > 0) {
    if (JSVAL_IS_OBJECT(argv[0])) {
      FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, JSVAL_TO_OBJECT(argv[0])) );
      if (this_private_data) {
        SFVec3f* sftranslation = dynamic_cast<SFVec3f*>(this_private_data->getPointer());
        if (sftranslation) {  // TO CHECK
          sftranslation->setValue( mobj.getTranslationPart() ); 
        }
      }
    }
  }

  if (argc > 1) {
    if (JSVAL_IS_OBJECT(argv[1])) {
      FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, JSVAL_TO_OBJECT(argv[1])) );
      if (this_private_data) {
        SFRotation* sfrotation = dynamic_cast<SFRotation*>( this_private_data->getPointer() );
        if (sfrotation) {  // TO CHECK
          Rotation r( mobj.getRotationPart() );
          sfrotation->setValue( r ); 
        }
      }
    }
  }

  if (argc > 2) {
    if (JSVAL_IS_OBJECT(argv[2])) {
      FieldObjectPrivate *this_private_data = static_cast<FieldObjectPrivate *>( JS_GetPrivate( cx, JSVAL_TO_OBJECT(argv[2])) );
      if (this_private_data) {
        SFVec3f* sfscale = dynamic_cast<SFVec3f*>( this_private_data->getPointer() );
        if (sfscale) {
          sfscale->setValue( mobj.getScalePart() );
        }
      }
    }
  }

  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_setTransform(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>( cx, obj );

  Transform::Matrix* transform_matrix = new Transform::Matrix();

  SFVec3f* sftranslation  = (argc < 1 ? new SFVec3f(Vec3f(0, 0, 0)) : 
    helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT(argv[0]) ) );
  SFRotation* sfrotation  = (argc < 2 ? new SFRotation(Rotation(0, 0, 1, 0)) : 
    helper_extractPrivateObject<SFRotation>( cx, JSVAL_TO_OBJECT(argv[1]) ) );
  SFVec3f* sfscale        = (argc < 3 ? new SFVec3f(Vec3f(1, 1, 1)) : 
    helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT(argv[2]) ) );
  SFRotation* sfso        = (argc < 4 ? new SFRotation(Rotation( 0, 0, 1, 0 )) : helper_extractPrivateObject<SFRotation>( cx, JSVAL_TO_OBJECT(argv[3]) ) );
  SFVec3f* sfcenter       = (argc < 5 ? new SFVec3f(Vec3f(0, 0, 0)) : 
    helper_extractPrivateObject<SFVec3f>( cx, JSVAL_TO_OBJECT(argv[4]) ) );

      /// routes_in[0] center field
    /// routes_in[1] rotation field
    /// routes_in[2] scale field
    /// routes_in[3] scaleOrientation field
    /// routes_in[4] translation field
  sfcenter->routeNoEvent(transform_matrix);
  sfrotation->routeNoEvent(transform_matrix);
  sfscale->routeNoEvent(transform_matrix);
  sfso->routeNoEvent(transform_matrix);
  sftranslation->route(transform_matrix);
   
  sfobj->setValue(transform_matrix->getValue());
  delete transform_matrix;

  if (argc < 5) delete sfcenter;
  if (argc < 4) delete sfso;
  if (argc < 3) delete sfscale;
  if (argc < 2) delete sfrotation;
  if (argc < 1) delete sftranslation;

  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_inverse(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 0 argument
  JSClass* type_classes[] = { &X3DMatrix4Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }
  SFMatrix4f* sfmatrix = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
  SFMatrix4f* sfrval = new SFMatrix4f( sfmatrix->getValue().inverse() );
  *rval = OBJECT_TO_JSVAL(X3DMatrix4_newInstance(cx, sfrval , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_transpose(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {

  // expecting 0 argument
  JSClass* type_classes[] = { &X3DMatrix4Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 0, type_classes ) ) {
    return JS_FALSE;
  }

  SFMatrix4f* sfmatrix = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
  SFMatrix4f* sfrval = new SFMatrix4f( sfmatrix->getValue().transpose() );
  *rval = OBJECT_TO_JSVAL(X3DMatrix4_newInstance(cx, sfrval , true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_multLeft(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // arg * obj

  // expecting 1 argument of class X3DMatrix4
  JSClass* type_classes[] = { &X3DMatrix4Class, &X3DMatrix4Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
  SFMatrix4f* sfarg = helper_extractPrivateObject<SFMatrix4f>( cx, JSVAL_TO_OBJECT(argv[0]) );
  SFMatrix4f* sfans = new SFMatrix4f(sfarg->getValue() * sfobj->getValue());

  *rval = OBJECT_TO_JSVAL(X3DMatrix4_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_multRight(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // obj * arg

  // expecting 1 argument of class X3DMatrix4
  JSClass* type_classes[] = { &X3DMatrix4Class, &X3DMatrix4Class };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }

  SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
  SFMatrix4f* sfarg = helper_extractPrivateObject<SFMatrix4f>( cx, JSVAL_TO_OBJECT(argv[0]) );
  SFMatrix4f* sfans = new SFMatrix4f(sfobj->getValue() * sfarg->getValue());

  *rval = OBJECT_TO_JSVAL(X3DMatrix4_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_multVecMatrix(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // expecting 1 argument of class SFVec3f
  JSClass* type_classes[] = { &X3DMatrix4Class, &SFVec4fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // row vector
  SFVec4f* sfarg = helper_extractPrivateObject<SFVec4f>(cx, JSVAL_TO_OBJECT(argv[0]));
  SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>(cx, obj);

  // row_vec * matrix = 1x3matrix
  Matrix4f m = sfobj->getValue();
  Vec4f v = sfarg->getValue();
  Vec4f ans(v * m.getColumn(0), v * m.getColumn(1), v * m.getColumn(2), v * m.getColumn(3) );
  SFVec4f* sfans = new SFVec4f(ans);

  *rval = OBJECT_TO_JSVAL(SFVec4f_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_multMatrixVec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
 // expecting 1 argument of class SFVec3f
  JSClass* type_classes[] = { &X3DMatrix4Class, &SFVec4fClass };
  if ( !helper_sanityCheck(cx, obj, argc, argv, 1, type_classes ) ) {
    return JS_FALSE;
  }
  // col vector
  SFVec4f* sfarg = helper_extractPrivateObject<SFVec4f>(cx, JSVAL_TO_OBJECT(argv[0]));
  SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>(cx, obj);

  // matrix * col vector = 4x1matrix
  Matrix4f m = sfobj->getValue();
  Vec4f v = sfarg->getValue();
  Vec4f ans(m.getRow(0) * v, m.getRow(1) * v, m.getRow(2) * v, m.getRow(3) * v);
  SFVec4f* sfans = new SFVec4f(ans);

  *rval = OBJECT_TO_JSVAL(SFVec4f_newInstance(cx, sfans, true) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_toString(JSContext *cx, JSObject *obj,
                                         uintN argc, jsval *argv, jsval *rval) {
  SFMatrix4f* sfmatrix = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
  string s = sfmatrix->getValueAsString();
  *rval = STRING_TO_JSVAL( JS_NewStringCopyN( cx, (char *)s.c_str(), s.length() ) );
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    int row = JSVAL_TO_INT(id);
    SFMatrix4f* sfobj = helper_extractPrivateObject<SFMatrix4f>( cx, obj );
    if (row < 0 || row > 3) {
      JS_ReportError(cx, "Index out of bound.");
      return JS_FALSE;
    }
    SFMatrix4fRow* sfans = new SFMatrix4fRow(sfobj, row);
    *vp = OBJECT_TO_JSVAL(SFMatrix4fRow_newInstance(cx, sfans, true) );
    return JS_TRUE;
  } else {
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}
JSBool SpiderMonkey::X3DMatrix4_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  // TODO
  return JS_TRUE;
}

JSBool SpiderMonkey::X3DMatrix4_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  //check that we have enough arguments and that they are of
  //correct type.
  if (argc != 16) {
    return JS_FALSE;
  }
  float a[16];
  for (size_t i = 0; i < argc; ++i) {
    jsdouble r;
    if (!JS_ValueToNumber( cx, argv[i], &r) ) {
      std::cout<< "failed in construct" << std::endl;
      return JS_FALSE;
    }
    a[i] = (H3DFloat) r;
  }
  Matrix4f m( a[0], a[1], a[2], a[3],
              a[4], a[5], a[6], a[7],
              a[8], a[9], a[10], a[11], 
              a[12], a[13], a[14], a[15] );
  SFMatrix4f* sfm = new SFMatrix4f(m);
  *rval = OBJECT_TO_JSVAL( X3DMatrix4_newInstance( cx, sfm, true ) ); 
  return JS_TRUE;
}

JSObject *SpiderMonkey::X3DMatrix4_newInstance( JSContext *cx, Field *field, bool internal_field) {
  JSObject *js_field;
  js_field = JS_NewObject( cx, &X3DMatrix4Class, NULL, NULL );  
  JS_DefineProperties(cx, js_field, X3DMatrix4_properties );
  JS_DefineFunctions(cx, js_field, X3DMatrix4_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field ) );
  return js_field; 
}

JSBool SpiderMonkey::SFMatrix4fRow_toString(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  // not supposed to implement this because SFMatrix4fRow type
  // is assumed to be hidden
  return JS_TRUE;
}

JSBool SpiderMonkey::SFMatrix4fRow_setProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    int col = JSVAL_TO_INT(id);
    SFMatrix4fRow* sfobj = helper_extractPrivateObject<SFMatrix4fRow>( cx, obj );
    if (col < 0 || col > 3) {
      JS_ReportError(cx, "Column index out of bound.");
      return JS_FALSE;
    }
    jsdouble jsd; JSBool res = JS_ValueToNumber( cx, *vp, &jsd );
    float f = (H3DFloat) jsd;
    Matrix4f m = sfobj->sfmatrix->getValue();
    m.setElement(sfobj->row, col, f);
    sfobj->sfmatrix->setValue(m);
    return JS_TRUE;
  } else {
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSBool SpiderMonkey::SFMatrix4fRow_getProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
  if (JSVAL_IS_INT(id)) {
    int col = JSVAL_TO_INT(id);
    SFMatrix4fRow* sfobj = helper_extractPrivateObject<SFMatrix4fRow>( cx, obj );
    if (col < 0 || col > 3) {
      JS_ReportError(cx, "Column index out of bound.");
      return JS_FALSE;
    }
    float r = sfobj->sfmatrix->getValue().getElement(sfobj->row, col);
    JS_NewNumberValue( cx, r, vp ); 
    return JS_TRUE;
  } else {
    if( *vp == JSVAL_VOID ) {
      JSString *s = JSVAL_TO_STRING( id );
      JS_ReportError(cx, "Field object does not have property \"%s\".", JS_GetStringBytes( s ) );
      return JS_FALSE;
    } else {
      return JS_TRUE;
    }
  }
}

JSObject *SpiderMonkey::SFMatrix4fRow_newInstance( JSContext *cx, Field *field, bool internal_field ) {
  JSObject *js_field;
  js_field = JS_NewObject( cx, &SFMatrix4fRowClass, NULL, NULL );  
  JS_DefineProperties(cx, js_field, SFMatrix4fRow_properties );
  JS_DefineFunctions(cx, js_field, SFMatrix4fRow_functions );
  JS_SetPrivate(cx, js_field, (void *) new FieldObjectPrivate( field, internal_field ) );
  return js_field; 
}

JSBool SpiderMonkey::SFMatrix4fRow_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval) {
  return JS_TRUE;
}


#endif // HAVE_SPIDERMONKEY
