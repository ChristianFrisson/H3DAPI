//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
//
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FIELDTEMPLATES_H__
#define __FIELDTEMPLATES_H__

#include <string>
#include <set>
#include <vector>

#include "SField.h"
#include "MField.h"
#include "TypedField.h"

using namespace std;

namespace H3D {

  H3D_VALUE_EXCEPTION( string, InvalidNodeType );

  // template specializations in order to get nicer type printouts.
  template<>
  inline string SField< H3DFloat >::classTypeName() { return "SFFloat"; }

  template<>
  inline string SField< H3DDouble >::classTypeName() { return "SFDouble"; }

  template<>
  inline string SField< H3DInt32 >::classTypeName() { return "SFInt32"; }

  template<>
  inline string SField< Vec2f >::classTypeName() { return "SFVec2f"; }

  template<>
  inline string SField< Vec3f >::classTypeName() { return "SFVec3f"; }

  template<>
  inline string SField< Vec4f >::classTypeName() { return "SFVec4f"; }

  template<>
  inline string SField< Vec2d >::classTypeName() { return "SFVec2d"; }

  template<>
  inline string SField< Vec3d >::classTypeName() { return "SFVec3d"; }

  template<>
  inline string SField< Vec4d >::classTypeName() { return "SFVec4d"; }

  template<>
  inline string SField< bool >::classTypeName() { return "SFBool"; }

  template<>
  inline string SField< string >::classTypeName() { return "SFString"; }

  template<>
  inline string SField< RGB >::classTypeName() { return "SFColor"; }

  template<>
  inline string SField< RGBA >::classTypeName() { return "SFColorRGBA"; }

  template<>
  inline string SField< Rotation >::classTypeName() { return "SFRotation"; }

  template<>
  inline string SField< Quaternion >::classTypeName() { return "SFQuaternion";}

  template<>
  inline string SField< Matrix3f >::classTypeName() { return "SFMatrix3f"; }

  template<>
  inline string SField< Matrix4f >::classTypeName() { return "SFMatrix4f"; }

  template<>
  inline string SField< Matrix3d >::classTypeName() { return "SFMatrix3d"; }

  template<>
  inline string SField< Matrix4d >::classTypeName() { return "SFMatrix4d"; }

  template<>
  inline string MFieldBase< H3DFloat >::classTypeName() { return "MFFloat"; }

  template<>
  inline string MFieldBase< H3DDouble >::classTypeName() { return "MFDouble"; }

  template<>
  inline string MFieldBase< H3DInt32 >::classTypeName() { return "MFInt32"; }

  template<>
  inline string MFieldBase< Vec2f >::classTypeName() { return "MFVec2f"; }

  template<>
  inline string MFieldBase< Vec3f >::classTypeName() { return "MFVec3f"; }

  template<>
  inline string MFieldBase< Vec4f >::classTypeName() { return "MFVec4f"; }

  template<>
  inline string MFieldBase< Vec2d >::classTypeName() { return "MFVec2d"; }

  template<>
  inline string MFieldBase< Vec3d >::classTypeName() { return "MFVec3d"; }

  template<>
  inline string MFieldBase< Vec4d >::classTypeName() { return "MFVec4d"; }

  template<>
  inline string MFieldBase< bool >::classTypeName() { return "MFBool"; }

  template<>
  inline string MFieldBase< string >::classTypeName() { return "MFString"; }

  template<>
  inline string MFieldBase< RGB >::classTypeName() { return "MFColor"; }

  template<>
  inline string MFieldBase< RGBA >::classTypeName() { return "MFColorRGBA"; }

  template<>
  inline string MFieldBase< Rotation >::classTypeName() 
  { return "MFRotation"; }

  template<>
  inline string MFieldBase< Quaternion >::classTypeName() 
  { return "MFQuaternion"; }

  template<>
  inline string MFieldBase< Matrix3f >::classTypeName() 
  { return "MFMatrix3f"; }

  template<>
  inline string MFieldBase< Matrix4f >::classTypeName() 
  { return "MFMatrix4f"; }

  template<>
  inline string MFieldBase< Matrix3d >::classTypeName() 
  { return "MFMatrix3d"; }

  template<>
  inline string MFieldBase< Matrix4d >::classTypeName() 
  { return "MFMatrix4d"; }

  /// \defgroup FieldTypes Basic Field types.
  /// The basic Field types of H3D API.
  /// \{
    
  /// The SFFloat field contains one single-precision floating point number.
  class H3DAPI_API SFFloat: public SField< H3DFloat > {
  public:
    SFFloat() {}
    SFFloat( const H3DFloat &_value ): SField< H3DFloat >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFFLOAT; }
  };

  /// The SFDouble field contains one high-precision floating point number.
  class H3DAPI_API SFDouble: public SField< H3DDouble > {
  public:
    SFDouble() {}
    SFDouble( const H3DDouble &_value ): SField< H3DDouble >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFDOUBLE; }
  };

  /// The SFInt32 field contains one 32-bit integer.
  class H3DAPI_API SFInt32: public SField< H3DInt32 > {
  public:
    SFInt32() {}
    SFInt32( const H3DInt32 &_value ): SField< H3DInt32 >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFINT32; }
  };

  /// The SFVec2f field contains a Vec2f.
  class H3DAPI_API SFVec2f: public SField< Vec2f > {
  public:
    SFVec2f() {}
    SFVec2f( const Vec2f &_value ): SField< Vec2f >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC2F; }
  };

  /// The SFVec3f field contains a Vec3f.
  class H3DAPI_API SFVec3f: public SField< Vec3f > {
  public:
    SFVec3f() {}
    SFVec3f( const Vec3f &_value ): SField< Vec3f >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC3F; }
  };

  /// The SFVec4f field contains a Vec4f.
  class H3DAPI_API SFVec4f: public SField< Vec4f > {
  public:
    SFVec4f() {}
    SFVec4f( const Vec4f &_value ): SField< Vec4f >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC4F; }
  };

  /// The SFVec2d field contains a Vec2d.
  class H3DAPI_API SFVec2d: public SField< Vec2d > {
  public:
    SFVec2d() {}
    SFVec2d( const Vec2d &_value ): SField< Vec2d >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC2D; }
  };

  /// The SFVec3d field contains a Vec3d.
  class H3DAPI_API SFVec3d: public SField< Vec3d > {
  public:
    SFVec3d() {}
    SFVec3d( const Vec3d &_value ): SField< Vec3d >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC3D; }
  };

  /// The SFVec4d field contains a Vec4d.
  class H3DAPI_API SFVec4d: public SField< Vec4d > {
  public:
    SFVec4d() {}
    SFVec4d( const Vec4d &_value ): SField< Vec4d >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC4D; }
  };

  /// The SFBool field contains a single boolean value.
  class H3DAPI_API SFBool: public SField< bool > {
  public:
    SFBool() {}
    SFBool( const bool &_value ): SField< bool >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFBOOL; }
    
    /// Get the value of the field as a string.
    inline virtual string getValueAsString( const string& separator = " " ) {
      if( getValue() )
        return "true";
      else
        return "false";
    }
  };

  /// The SFTime field contains a single time value.
  class H3DAPI_API SFTime: public TypedField< SFDouble, void, 
                                              AnyNumber< SFTime > > {
  public:
    SFTime() {}
	SFTime( const H3DTime &_value ) { value = _value; }
    virtual string getTypeName() { return classTypeName(); }
    static string classTypeName() { return "SFTime"; }
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFTIME; }
  };

  /// The SFString field contains a string.
  class H3DAPI_API SFString: public SField< string > {
  public:
    SFString() {}
    SFString( const string &_value ): SField< string >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFSTRING; }
  };

  /// The SFColor field contains one RGB (red-green-blue) colour triple.
  class H3DAPI_API SFColor: public SField< RGB > {
  public:
    SFColor() {}
    SFColor( const RGB &_value ): SField< RGB >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFCOLOR; }
  };
  
  /// The SFColorRGBA field contains one RGBA (red-green-blue-alpha) 
  /// colour 4-tuple.
  class H3DAPI_API SFColorRGBA: public SField< RGBA > {
  public:
    SFColorRGBA() {}
    SFColorRGBA( const RGBA &_value ): SField< RGBA >( _value ){}
    virtual string getTypeName() { return classTypeName(); }
    static string classTypeName() { return "SFColorRGBA"; }
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFCOLORRGBA; }
  };

  /// The SFRotation field contains one arbitrary Rotation.
  class H3DAPI_API SFRotation: public SField< Rotation > {
  public:
    SFRotation() {}
    SFRotation( const Rotation &_value ): SField< Rotation >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFROTATION; }
  };

  /// The SFQuaternion field contains a Quaternion.
  class H3DAPI_API SFQuaternion: public SField< Quaternion > {
  public:
    SFQuaternion() {}
    SFQuaternion( const Quaternion &_value ): SField< Quaternion >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFQUATERNION; }
  };

  /// The SFMatrix3f field contains a Matrix3f.
  class H3DAPI_API SFMatrix3f: public SField< Matrix3f > {
  public:
    SFMatrix3f() {}
    SFMatrix3f( const Matrix3f &_value ): SField< Matrix3f >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFMATRIX3F; }
  };
    
  /// The SFMatrix4f field contains a Matrix4f.
  class H3DAPI_API SFMatrix4f: public SField< Matrix4f > {
  public:
    SFMatrix4f() {}
    SFMatrix4f( const Matrix4f &_value ): SField< Matrix4f >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFMATRIX4F; }
  };

  /// The SFMatrix3d field contains a Matrix3d.
  class H3DAPI_API SFMatrix3d: public SField< Matrix3d > {
  public:
    SFMatrix3d() {}
    SFMatrix3d( const Matrix3d &_value ): SField< Matrix3d >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFMATRIX3D; }
  };
    
  /// The SFMatrix4d field contains a Matrix4d.
  class H3DAPI_API SFMatrix4d: public SField< Matrix4d > {
  public:
    SFMatrix4d() {}
    SFMatrix4d( const Matrix4d &_value ): SField< Matrix4d >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFMATRIX4D; }
  };

  /// The MFFloat field contains a vector of single-precision floating 
  /// point numbers.
  class H3DAPI_API MFFloat: public MField< H3DFloat > {
  public:
    MFFloat(){}
    MFFloat( size_type sz ): MField< H3DFloat >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFFLOAT; }
  };

  /// The MFDouble field contains a vector of high-precision floating 
  /// point numbers.
  class H3DAPI_API MFDouble: public MField< H3DDouble > {
  public:
    MFDouble(){}
    MFDouble( size_type sz ): MField< H3DDouble >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFDOUBLE; }
  };

  /// The MFInt32 field contains a vector of 32-bit integers.
  class H3DAPI_API MFInt32: public MField< H3DInt32 > {
  public:
    MFInt32(){}
    MFInt32( size_type sz ): MField< H3DInt32 >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFINT32; }
  };

  /// The MFVec2f field contains a vector of Vec2f.
  class H3DAPI_API MFVec2f: public MField< Vec2f > {
  public:
    MFVec2f(){}
    MFVec2f( size_type sz ): MField< Vec2f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC2F; }
  };

  /// The MFVec3f field contains a vector of Vec3f.
  class H3DAPI_API MFVec3f: public MField< Vec3f > {
  public:
    MFVec3f(){}
    MFVec3f( size_type sz ): MField< Vec3f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC3F; }
  };

  /// The MFVec4f field contains a vector of Vec4f.
  class H3DAPI_API MFVec4f: public MField< Vec4f > {
  public:
    MFVec4f(){}
    MFVec4f( size_type sz ): MField< Vec4f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC4F; }
  };

  /// The MFVec2d field contains a vector of Vec2d.
  class H3DAPI_API MFVec2d: public MField< Vec2d > {
  public:
    MFVec2d(){}
    MFVec2d( size_type sz ): MField< Vec2d >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC2D; }
  };

  /// The MFVec3d field contains a vector of Vec3d.
  class H3DAPI_API MFVec3d: public MField< Vec3d > {
  public:
    MFVec3d(){}
    MFVec3d( size_type sz ): MField< Vec3d >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC3D; }
  };

  /// The MFVec4d field contains a vector of Vec4d.
  class H3DAPI_API MFVec4d: public MField< Vec4d > {
  public:
    MFVec4d(){}
    MFVec4d( size_type sz ): MField< Vec4d >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFVEC4D; }
  };

  /// The MFBool field contains a vector of boolean values.
  class H3DAPI_API MFBool: public MField< bool > {
  public:
    MFBool(){}
    MFBool( size_type sz ): MField< bool >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFBOOL; }
    /// Get the value of the field as a string. If the field contains
    /// multiple values the separator string is used between the values.
    inline virtual string getValueAsString( const string& separator = " " ) {
      stringstream s;
      const vector< bool > &v = getValue();
      
      if( v.size() == 0 )
        return "";
      unsigned int i;
      for( i = 0; i < v.size() - 1; i++ ) {
        if( v[i] ) s << "true";
        else s << "false";
        s << separator;
        
      }
      if( v[i] ) s << "true";
      else s << "false";
      return s.str();
    }
  };

  /// The MFTime field contains a vector of time values.
  class H3DAPI_API MFTime: public TypedField< MFDouble, 
                                              void,
                                              AnyNumber< MFTime > > {
  public:
    MFTime(){}
    MFTime( size_type sz ) {
	  value.resize( sz );	
	}
    virtual string getTypeName() { return classTypeName(); }
    static string classTypeName() { return "MFTime"; }
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFTIME; }
  };
  
  /// The MFString field conatins a vector of strings.
  class H3DAPI_API MFString: public MField< string > {
  public:
    MFString(){}
    MFString( size_type sz ): MField< string >( sz ){}
    inline virtual string getValueAsString( const string& separator = " " ) {
      stringstream s;
      const vector< string > &v = getValue();
      if( v.size() == 0 )
        return "";
      unsigned int i;
      for( i = 0; i < v.size() - 1; i++ )
        s << "\"" << v[i] << "\"" << separator;
      s << "\"" << v[i] << "\"";
      return s.str();
    }
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFSTRING; }
  };

  /// The MFColor field contains a vector of RGB (red-green-blue) 
  /// colour triples.
  class H3DAPI_API MFColor: public MField< RGB > {
  public:
    MFColor(){}
    MFColor( size_type sz ): MField< RGB >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFCOLOR; }
  };

  /// The MFColorRGBA field contains a vector of RGBA (red-green-blue-alpha) 
  /// colour 4-tuples.
  class H3DAPI_API MFColorRGBA: public MField< RGBA > {
  public:
    MFColorRGBA(){}
    MFColorRGBA( size_type sz ): MField< RGBA >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFCOLORRGBA; }
  };

  /// The MFRotation field contains a vector of arbitrary Rotation objects.
  class H3DAPI_API MFRotation: public MField< Rotation > {
  public:
    MFRotation(){}
    MFRotation( size_type sz ): MField< Rotation >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFROTATION; }
  };

  /// The MFQuaternion field contains a vector of arbitrary Quaternion objects.
  class H3DAPI_API MFQuaternion: public MField< Quaternion > {
  public:
    MFQuaternion(){}
    MFQuaternion( size_type sz ): MField< Quaternion >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFQUATERNION; }
  };

  /// The MFMatrix3f field contains a vector of Matrix3f instances.
  class H3DAPI_API MFMatrix3f: public MField< Matrix3f > {
  public:
    MFMatrix3f(){}
    MFMatrix3f( size_type sz ): MField< Matrix3f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFMATRIX3F; }
  };

  /// The MFMatrix4f field contains a vector of Matrix4f instances.
  class H3DAPI_API MFMatrix4f: public MField< Matrix4f > {
  public:
    MFMatrix4f(){}
    MFMatrix4f( size_type sz ): MField< Matrix4f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFMATRIX4F; }
  };

  /// The MFMatrix3d field contains a vector of Matrix3d instances.
  class H3DAPI_API MFMatrix3d: public MField< Matrix3d > {
  public:
    MFMatrix3d(){}
    MFMatrix3d( size_type sz ): MField< Matrix3d >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFMATRIX3D; }
  };

  /// The MFMatrix4d field contains a vector of Matrix4d instances.
  class H3DAPI_API MFMatrix4d: public MField< Matrix4d > {
  public:
    MFMatrix4d(){}
    MFMatrix4d( size_type sz ): MField< Matrix4d >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFMATRIX4D; }
  };

  /// \}

  /// Template to make sure that the Node that is set in a SFNode is
  /// of a specified Node type.
  /// \param RefCountedTyoe The expected RefcountedClass type. 
  /// \param BaseRefCountType The base field type.
  /// \ingroup FieldTemplateModifiers
  template< class RefCountedType, class BaseRefCountType >
  class TypedRefCountField: public BaseRefCountType {
  protected:
    /// We check that the type of the Node is of the correct type.
    void onAdd( typename BaseRefCountType::value_type n) {
      if( !dynamic_cast< RefCountedType * >( n ) ) {
        stringstream s;
        s << "Expecting " << typeid( RefCountedType ).name() << ends;
        throw InvalidNodeType( typeid( n ).name(),
                               s.str(),
                               H3D_FULL_LOCATION );
  
      } else {
        BaseRefCountType::onAdd(  n );
      }
    }
  public:
    /// Get the value casted to the RefCountType.
    virtual RefCountedType *getValue() {
      return static_cast< RefCountedType * >( BaseRefCountType::getValue() );
    }
    
  };

  /// The AutoUpdate field is a template to force the BaseField to update 
  /// itself as soon as an event is received. Normally the value of the 
  /// field has to be requested in order for an update to take place.
  /// \ingroup FieldTemplateModifiers
  template< class BaseFieldType >
  struct AutoUpdate: public BaseFieldType {
    /// Receive an event from another field. We call the upToDate() 
    /// function to update the field.
    ///
    virtual void propagateEvent( Field::Event e ) {
      BaseFieldType::propagateEvent( e );
      this->upToDate();
    }
  };
}

#endif

