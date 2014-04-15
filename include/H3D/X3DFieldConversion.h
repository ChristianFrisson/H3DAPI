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
/// \file X3DFieldConversion.h
/// \brief This file contains functions for convertion from a string to 
/// a value of an X3D field type.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __X3DFIELDCONVERSION_H__
#define __X3DFIELDCONVERSION_H__

#include <sstream>
#include <H3DUtil/Exception.h>
#include <H3D/H3DTypes.h>
#include <H3DUtil/PixelImage.h>

#include <typeinfo>

using namespace std;

namespace H3D {
  namespace X3D {
    /// The Convert namespace contains help functions for parsing a string 
    /// and converting it to a X3D field value. 
    /// 
    namespace Convert {
      /// 
      /// \class X3DFieldConversionError
      /// An exception for errors when string to convert to a field value
      /// is invalid for the field type. The 'value' should 
      /// be the name of the field value type we tried to convert to.
      ///
      H3D_VALUE_EXCEPTION( const string, X3DFieldConversionError );

      /// 
      /// \class UnimplementedConversionType
      /// An exception for errors when the conversion from a string 
      /// to the type that we try convert to is not implemented. Thrown
      /// by getValue.
      ///
      H3D_VALUE_EXCEPTION( const string, UnimplementedConversionType );

      /// Skip whitespaces at the beginning of a string.
      /// \param s The string to skip whitespace in
      /// \returns A pointer to the first character in the string that
      /// is not a whitespace
      ///
      inline const char *skipWhitespaces( const char *s ) {
        int i = 0; 
        while( isspace(s[i]) && s[i]!='\0' ) {
          ++i;
        }
        return s+i;
      }
      
      /// Skip whitespaces and commas at the beginning of a string.
      /// \param s The string to skip whitespace in
      /// \returns A pointer to the first character in the string that
      /// is not a whitespace
      ///
      inline const char *skipWhitespacesAndCommas( const char *s ) {
        int i = 0; 
        while( true ) {
          if( s[i] == ',' || isspace(s[i] ) ) {
            ++i;
          } else { 
            break;
          }
        }
        return s+i;
      }

      /// Works the same way as getValue<string>() but with the
      /// exception that the string given is within double quotes. The
      /// resulting value will be the string inside the quotes.
      inline string getQuoteEnclosedStringValue( const char *s, 
                                                 const char *&rest ) {
        int i = 0;
        if( s[0] != '\"' ) {
          throw X3DFieldConversionError( "string" ); 
        }
        ++i;
        while( s[i] !='\"' && s[i]!='\0' ) {
          ++i;
        }
        if( s[i] != '\"' ) {
          throw X3DFieldConversionError( "string" ); 
        }
        char *new_s = new char[i+1];
        strncpy( new_s, &s[1], i-1 );
        new_s[i-1] = '\0';
        string ret( new_s );
        delete [] new_s;
        rest = &s[ i + 1 ];
        return ret;
      }
      
      /// Function that reads characters from a char * and converts them
      /// to a given type. It will use as many characters from the char * as
      /// possible to build the value. Used by H3D::X3D::X3DStringToVector and
      /// H3D::X3D::X3DStringToValue to read values. The default implementation
      /// just returns an exception. Each type that is supported must have
      /// a template specialization handling it.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template< class Type >
      inline Type getValue( const char *s, const char *&rest ) {
        throw UnimplementedConversionType( typeid( Type ).name(),
                                           "" );
      }
      
      
      /// Template specialization to handle the type 'string'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline string getValue<string>( const char *s, const char *&rest ) {
        string str( s ); 
        rest = &s[ str.length() ];
        return str;
      }

      /// Template specialization to handle the type 'bool'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline bool getValue<bool>( const char *s, const char *&rest ) {
        int i = 0;
        //bool valid = false;
        while( isspace(s[i]) && s[i]!='\0' ) {
          ++i;
        }
        if( strncmp( s, "true", 4 ) == 0 ||
            strncmp( s, "TRUE", 4 ) == 0 ) {
          rest = &s[i+4];
          return true;
        }
        else if( strncmp( s, "false", 5 ) == 0 ||
                 strncmp( s, "FALSE", 5 ) == 0) {
          rest = &s[i+5];  
          return false;
        }
        throw X3DFieldConversionError( "bool" );
      }

      /// Template specialization to handle the type 'double'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline double getValue<double>( const char *s, const char *&rest ) {
        // make sure the string is on the form
        // [whitespace][sign][digits][.digits][ {d | D | e | E }[sign]digits]
        int i = 0;
        bool negative = false;
        unsigned long integer_part = 0;
        double fractional_part = 0;
        bool valid = false;
        while( isspace(s[i]) && s[i]!='\0' ) {
          ++i;
        }
        if( s[i] == '+' ) {
          ++i;
        } else if( s[i] == '-' ) {
          ++i;
          negative = true;
        }
        while( isdigit( s[i] ) && s[i]!='\0' ) {
          integer_part = 10 * integer_part + ( s[i] - '0' );
          ++i;
          valid = true;
        }
        if( s[i] == '.' ) {
          ++i;
          unsigned int divider = 10;
          while( isdigit( s[i] ) && s[i]!='\0' ) {
            fractional_part = fractional_part + (double)(  s[i] - '0' ) / divider;
            divider *= 10;
            valid = true;
            ++i;
          }
        }
        int saved_i = i;
        int exponent = 0;
        bool exponent_negative = false;
        if( s[i] == 'd' || s[i] == 'D' || s[i] == 'e' || s[i] == 'E' ) {
          ++i;
          if( s[i] == '+' ) {
            ++i;
          } else if ( s[i] == '-' ) {
            ++i;
            exponent_negative = true;
          }
          
          int before_digit = i;
          while( isdigit( s[i] ) && s[i]!='\0' ) {
            exponent = 10 * exponent + ( s[i] - '0' );
            ++i;
          }
          // make sure that we have at least one digit, otherwise this
          // part is invalid and only the previous part will be used.
          if( i == before_digit ) {
            i = saved_i;
            exponent = 0;
          } else {
            if( exponent_negative ) {
              exponent = -exponent;
            }
          }
        }
  
        if( !valid ) {
          throw X3DFieldConversionError( "double" );
        } else {
          rest = &s[i];
          double res = integer_part + fractional_part;
          if( negative ) res = -res;
          if( exponent != 0 ) res = res * H3DPow( 10, exponent ); 
          return res;
        }
      }

      /// Template specialization to handle the type 'float'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline float getValue<float>( const char *s, const char *&rest ) {
        return (float)getValue<double>( s, rest );
      }
      
      /// Template specialization to handle the type 'int'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline int getValue<int>( const char *s, const char *&rest ) {
        int i = 0;
        bool valid = false;
        // remove whitespaces
        while( isspace(s[i]) && s[i]!='\0' ) {
          ++i;
        }
        if( s[0] == '0' && s[1] == 'x' ) {
          i+=2;
          // hexadecimal number 0x????????
          int max_index = i + 8;
          int return_value = 0;
          int pos_value;
          for( ; i < max_index; ++i ) {
            if (s[i]=='\0')
              break;
            if (s[i] > 0x29 && s[i] < 0x40 )      //if 0 to 9
              pos_value = s[i] & 0x0f;            //convert to int
            else if (s[i] >='a' && s[i] <= 'f')   //if a to f
              pos_value= (s[i] & 0x0f) + 9;       //convert to int
            else if (s[i] >='A' && s[i] <= 'F')   //if A to F
              pos_value = (s[i] & 0x0f) + 9;      //convert to int
            else break;
      
            valid = true;
            return_value = return_value << 4;
            return_value = return_value | pos_value;
          }
          if( !valid ) {
            throw X3DFieldConversionError( "int" );
          } else {
            rest = &s[i];
            return return_value;
          }
        } else {
          // decimal number. Make sure it is on the form
          // [sign]digits
          bool negative = false;

          if( s[i] == '+' ) {
            ++i;
          } else if ( s[i] == '-' ) {
            negative = true;
            ++i;
          }
          
          int result = 0;
          while( isdigit( s[i] ) && s[i]!='\0' ) {
            result = 10 * result + ( s[i] - '0' );
            ++i;
            valid = true;
          }

          if( !valid ) {
            throw X3DFieldConversionError( "int" );
          } else {
            rest = &s[i];
            if( negative ) result = -result;
            return result;
          }
        }
      }
      
      /// Template specialization to handle the type 'RGB'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline RGB getValue<RGB>( const char *s, const char *&rest ) {
        RGB color;
        const char *t1, *t2;
        color.r = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "RGB" );
        }
        color.g = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "RGB" );
        }
        color.b = getValue<H3DFloat>( t2, rest );
        return color;
      }
      
      /// Template specialization to handle the type 'RGBA'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline RGBA getValue<RGBA>( const char *s, const char *&rest ) {
        RGBA color;
        const char *t1, *t2;
        color.r = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "RGBA" );
        }
        color.g = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "RGBA" );
        }
        color.b = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "RGBA" );
        }
        color.a = getValue<H3DFloat>( t1, rest );
        return color;
      }
      
      /// Template specialization to handle the type 'Rotation'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline Rotation getValue<Rotation>( const char *s, 
                                          const char *&rest ) {
        Rotation rot;
        const char *t1, *t2;
        rot.axis.x = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Rotation" );
        }
        rot.axis.y = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Rotation" );
        }
        rot.axis.z = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Rotation" );
        }
        rot.angle = getValue<H3DFloat>( t1, rest );
        return rot;
      }

      /// Template specialization to handle the type 'Quaternion'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline Quaternion getValue<Quaternion>( const char *s, 
                                              const char *&rest ) {
        Quaternion q;
        const char *t1, *t2;
        q.v.x = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Quaternion" );
        }
        q.v.y = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Quaternion" );
        }
        q.v.z = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Quaternion" );
        }
        q.w = getValue<H3DFloat>( t1, rest );
        return q;
      }
      
      /// Template specialization to handle the type 'Matrix4f'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Matrix4f getValue<Matrix4f>( const char *s, const char *&rest ) {
        Matrix4f m;
        const char *t1, *t2;
        m[0][0] = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[0][1] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[0][2] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[0][3] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }

        m[1][0] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[1][1] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[1][2] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[1][3] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        } 
  
        m[2][0] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[2][1] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[2][2] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[2][3] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        } 

        m[3][0] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[3][1] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[3][2] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4f" );
        }
        m[3][3] = getValue<H3DFloat>( t1, rest );

        return m;
      }

      /// Template specialization to handle the type 'Matrix3f'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Matrix3f getValue<Matrix3f>( const char *s, const char *&rest ) {
        Matrix3f m;
        const char *t1, *t2;
        m[0][0] = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[0][1] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[0][2] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[1][0] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[1][1] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[1][2] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[2][0] = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[2][1] = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3f" );
        }
        m[2][2] = getValue<H3DFloat>( t2, rest );

        return m;
      }

      /// Template specialization to handle the type 'Matrix4d'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Matrix4d getValue<Matrix4d>( const char *s, const char *&rest ) {
        Matrix4d m;
        const char *t1, *t2;
        m[0][0] = getValue<H3DDouble>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[0][1] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[0][2] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[0][3] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }

        m[1][0] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[1][1] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[1][2] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[1][3] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        } 
  
        m[2][0] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[2][1] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[2][2] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[2][3] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        } 

        m[3][0] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[3][1] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[3][2] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix4d" );
        }
        m[3][3] = getValue<H3DDouble>( t1, rest );

        return m;
      }

      /// Template specialization to handle the type 'Matrix3f'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Matrix3d getValue<Matrix3d>( const char *s, const char *&rest ) {
        Matrix3d m;
        const char *t1, *t2;
        m[0][0] = getValue<H3DDouble>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[0][1] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[0][2] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[1][0] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[1][1] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[1][2] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[2][0] = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[2][1] = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Matrix3d" );
        }
        m[2][2] = getValue<H3DDouble>( t2, rest );

        return m;
      }



      /// Template specialization to handle the type 'Vec4f'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Vec4f getValue<Vec4f>( const char *s, const char *&rest ) {
        Vec4f v;
        const char *t1, *t2;
        v.x = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec4f" );
        }
        v.y = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Vec4f" );
        }
        v.z = getValue<H3DFloat>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec4f" );
        }
        v.w = getValue<H3DFloat>( t1, rest );
        return v;
      }

      /// Template specialization to handle the type 'Vec4d'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline Vec4d getValue<Vec4d>( const char *s, const char *&rest ) {
        Vec4d v;
        const char *t1, *t2;
        v.x = getValue<H3DDouble>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec4d" );
        }
        v.y = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Vec4d" );
        }
        v.z = getValue<H3DDouble>( t2, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec4d" );
        }
        v.w = getValue<H3DDouble>( t1, rest );
        return v;
      }

      /// Template specialization to handle the type 'Vec3f'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Vec3f getValue<Vec3f>( const char *s, const char *&rest ) {
        Vec3f v;
        const char *t1, *t2;
        v.x = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec3f" );
        }
        v.y = getValue<H3DFloat>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Vec3f" );
        }
        v.z = getValue<H3DFloat>( t2, rest );
        return v;
      }

      /// Template specialization to handle the type 'Vec3d'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the
      /// string not used in the conversion.
      ///
      template<>
      inline Vec3d getValue<Vec3d>( const char *s, const char *&rest ) {
        Vec3d v;
        const char *t1, *t2;
        v.x = getValue<H3DDouble>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec3d" );
        }
        v.y = getValue<H3DDouble>( t1, t2 );
        if( !isspace(t2[0]) ) {
          throw X3DFieldConversionError( "Vec3d" );
        }
        v.z = getValue<H3DDouble>( t2, rest );
        return v;
      }

      /// Template specialization to handle the type 'Vec2f'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Vec2f getValue<Vec2f>( const char *s, const char *&rest ) {
        Vec2f v;
        const char *t1;
        v.x = getValue<H3DFloat>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec2f" );
        }
        v.y = getValue<H3DFloat>( t1, rest );
        return v;
      }

      /// Template specialization to handle the type 'Vec2d'.
      /// \param s The string to convert.
      /// \param rest Return value pointing at the first character in the 
      /// string not used in the conversion.
      ///
      template<>
      inline Vec2d getValue<Vec2d>( const char *s, const char *&rest ) {
        Vec2d v;
        const char *t1;
        v.x = getValue<H3DDouble>( s, t1 );
        if( !isspace(t1[0]) ) {
          throw X3DFieldConversionError( "Vec2d" );
        }
        v.y = getValue<H3DDouble>( t1, rest );
        return v;
      }
        
      unsigned char *readImageData( const char *s, 
                                    int width,
                                    int height,
                                    int depth,
                                    int nr_components );
    
      /// The same as standard atof function with the difference that the decimal point
      /// is always . regardless of locale settings.
      ///
      /// On success, the function returns the converted floating point number as a double value.
      /// If no valid conversion could be performed, the function returns zero (0.0).
      inline double atof( const char * s) {
        try {
          const char *pos;
          return X3D::Convert::getValue< double >( s, pos );
        } catch( X3D::Convert::X3DFieldConversionError & ) {
          return 0.0;
        }
      } 
    }
  
    /// Convert a string to a specified type according to the X3D/XML
    /// field encoding.
    /// \tparam Type The type to convert the string to.
    /// \param x3d_string The string to convert.
    /// \returns The converted value.
    /// \throws Convert::X3DFieldConversionError
    /// \ returns The string converted to class Type.
    template< class Type >
    Type X3DStringToValue( const string &x3d_string ) {
      typedef Convert::X3DFieldConversionError ConversionError;
      const char *s = x3d_string.c_str();
      const char *t1;
      Type value;
      try {
        s = Convert::skipWhitespaces( s );
        value = Convert::getValue<Type>( s, t1 );
        s = Convert::skipWhitespaces( t1 );
      } catch( const ConversionError & ) {
        throw ConversionError( typeid( Type ).name() );
      }
      if( s[0] != '\0' ) {
        throw ConversionError( typeid( Type ).name() );
      }
      return value;
    }

    /// Convert a string to a vector of a specified type according
    /// to the X3D/XML field encoding.
    /// \tparam VectorType The vector type to store the converted types in.
    /// \param x3d_string The string to convert.
    /// \param values The return vector.
    /// \throws Convert::X3DFieldConversionError
    ///
    template< class VectorType >
    inline void X3DStringToVector( const string &x3d_string, 
                                   VectorType &values ) {
      typedef Convert::X3DFieldConversionError ConversionError;
      const char *s = x3d_string.c_str();
      const char *t1;
      values.clear();
      try {
        s = Convert::skipWhitespaces( s );
        while( s[0] != '\0' ) {
          values.push_back( Convert::getValue< 
                            typename VectorType::value_type >
                            ( (const char*)s, t1 ) );
          s = Convert::skipWhitespacesAndCommas( t1 );
        }
      } catch( const ConversionError & ) {
        stringstream ss;
        ss << typeid( typename VectorType::value_type ).name() 
           << " vector";
        throw ConversionError( ss.str() );
      }
    }

    /// Template specializaion for handling the case of MFString where the
    /// the values are specified as e.g. '"WALK" "ANY"'
    /// \param x3d_string The string to convert.
    /// \param values The return vector.
    /// \throws Convert::X3DFieldConversionError
    ///
    template< >
    inline void X3DStringToVector< vector< string > >( 
                           const string &x3d_string, 
                           vector< string > &values ) {
      typedef Convert::X3DFieldConversionError ConversionError;
      const char *s = x3d_string.c_str();
      const char *t1;
      values.clear();
      try {
        s = Convert::skipWhitespaces( s );
        if( s[0] != '\"' )
          values.push_back( Convert::getValue< string >( s, t1 ) );
        else
          while( s[0] != '\0' ) {
            values.push_back( 
                 Convert::getQuoteEnclosedStringValue( (const char*)s, t1 ) 
            );
            s = Convert::skipWhitespacesAndCommas( t1 );
          }
      } catch( const ConversionError & ) {
        stringstream ss;
        ss << " string vector";
        throw ConversionError( ss.str() );
      }
    }

    /// Convert a string to a PixelImage according to the X3D/XML
    /// field encoding for SFImage.
    /// \param x3d_string The string to convert.
    /// \returns The converted value.
    /// \throws Convert::X3DFieldConversionError
    ///
    PixelImage *X3DStringTo2DImage( const string &x3d_string );

    /// Convert a string to a 3D PixelImage according to the X3D 
    /// spec for Pixel3DTexture.
    /// \param x3d_string The string to convert.
    /// \returns The converted value.
    /// \throws Convert::X3DFieldConversionError
    ///
    PixelImage *X3DStringTo3DImage( const string &x3d_string );
  } 
};

#endif
