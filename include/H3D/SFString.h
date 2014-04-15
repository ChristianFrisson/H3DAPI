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
/// \file SFString.h
/// \brief Contains the SFString field class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SFSTRING_H__
#define __SFSTRING_H__

#include <H3D/SField.h>

namespace H3D {

  // template specializations in order to get nicer type printouts.
  template<>
  inline string SField< string >::classTypeName() { return "SFString"; }

  /// \ingroup FieldTypes
  /// The SFString field contains a string.
  class H3DAPI_API SFString: public SField< string > {
  public:
    SFString() {}
    SFString( const string &_value ): SField< string >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFSTRING; }

    /// Add a valid value for the string. valid values is a set of values
    /// that the field can be set to to make sense. It is not forced to
    /// be any of these values but they provide an indication of what 
    /// values to use.
    inline void addValidValue( const string &s ) {
      valid_values.insert( s );
    }

    /// Add valid values for the string. valid values is a set of values
    /// that the field can be set to to make sense. It is not forced to
    /// be any of these values but they provide an indication of what 
    /// values to use.
    template< class InputIterator >
    inline void addValidValues( InputIterator begin, InputIterator end ) {
      valid_values.insert( begin, end );
    }

    /// Returns true if the given string is one of the specified valid
    /// values for the field.
    inline bool isValidValue( const string &s ) {
      return valid_values.find( s ) != valid_values.end();
    }

    /// Returns true if any valid values have been specified for the 
    /// field.
    inline bool hasValidValues() {
      return valid_values.size() > 0;
    }

    /// Returns the valid values for the field.
    const set< string > &getValidValues() {
      return valid_values;
    }

    /// Clears the valid values for the field.
    void clearValidValues() {
      valid_values.clear();
    }
    
  protected:
    set< string > valid_values;
  };
}

#endif

