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
/// \file SField.h
/// \brief Contains the SField template class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SFIELD_H__
#define __SFIELD_H__

#include <H3D/X3DFieldConversion.h>
#include <H3D/TypedField.h>
#include <H3D/Node.h>

namespace H3D {

  /// \defgroup FieldTemplateModifiers Field template modifiers.
  /// Template classes for modifying the behaviour of Field classes.
 
  /// Base class for all fields created with the SField template.
  class H3DAPI_API SFieldClass {
  public:
    // Virtual destructor.
    virtual ~SFieldClass() {};

    /// Set the value of the field given a pointer to where the value
    /// of the field is. 
    /// \param data A pointer to the data.
    /// \param size The size in bytes of the value stored in data.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    /// \returns 0 if successful, -1 otherwise.
    virtual int setValueFromVoidPtr( void *data, unsigned int size, 
                                     int id = 0 ) = 0;

    /// Get the value of the data copied into a memory buffer.
    /// \param data Buffer to copy the data into.
    /// \param size The size of the buffer.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    /// \returns If successful: The number of bytes that was copied into the 
    /// Otherwise -1.
    ///
    virtual int getValueAsVoidPtr( void *data, unsigned int size, 
                                   int id = 0 ) = 0;

    /// Returns the size in bytes of the value type the sfield encapsulates.
    virtual unsigned int valueTypeSize() = 0;
  };


  /// Template class that adds the Field mechanisms to an encapsulated value
  /// of specified type.
  /// 
  /// \param Type The type of the value.
  /// \ingroup FieldTemplateModifiers
  template< class Type > 
  class SField: public TypedField< ParsableField,
                                   void,
                                   AnyNumber< SField< Type> > >,
                public SFieldClass {
  public:   
    /// The type of the value member. 
    typedef Type value_type;

    /// Constructor.
    SField() {}
        
    /// Constructor.
    SField( const Type &_value ) {
      value = _value;
    }

    /// Set the value of the field given a pointer to where the value
    /// of the field is. 
    /// \param data A pointer to the data.
    /// \param len The size in bytes of the value stored in data.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    /// \returns 0 if successful, -1 otherwise.
    inline virtual int setValueFromVoidPtr( void *data, unsigned int len, 
                                            int id = 0 ) {
      if( len != sizeof( value_type ) )
        return -1;
      setValue( *( static_cast< Type * >( data ) ), id );
      return 0;
    }

    /// Get the value of the data copied into a memory buffer.
    /// \param data Buffer to copy the data into.
    /// \param len The size of the buffer.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    /// \returns If successful: The number of bytes that was copied into the 
    /// Otherwise -1.
    ///
    inline virtual int getValueAsVoidPtr( void *data, unsigned int len, 
                                          int id = 0 ) {
      unsigned int size = sizeof( value_type );
      if( len < size ) {
        return -1;
      }
      *static_cast< Type * >( data ) = getValue( id );
      return size;
    } 

    /// Returns the size in bytes of the value type the sfield encapsulates.
    inline virtual unsigned int valueTypeSize() {
      return sizeof( value_type );
    }

    /// Set the value of the field.
    inline virtual void setValue( const Type &v, int id = 0 );
    /// Get the value of the field.
    inline virtual const Type &getValue( int id = 0 );
        
    /// Set the value of the field given a string. By default
    /// we try to parse the values according to the X3D/XML 
    /// specification.
    ///
    inline virtual void setValueFromString( const string &s ) {
      this->setValue( X3D::X3DStringToValue< Type >( s ) ); 
    }

    /// Get the value of the field as a string.
    inline virtual string getValueAsString( const string& separator = " " ) {
      stringstream s;
      s << getValue();
      return s.str();
    }
  
    /// Returns a string name for this field type e.g. SFInt32
    virtual string getTypeName() {
      return classTypeName();
    }

    /// Returns a string name for this field type e.g. SFInt32
  static string classTypeName() { return typeid( SField< Type > ).name(); }

  protected:
    /// Make the field up to date given that an event has occured.
    inline virtual void update();

    /// The encapsulated value.
    Type value;

  };

  template< class Type  >
  void SField< Type >::update() {
#ifdef DEBUG
    Console(1) << "SField< " << typeid( Type ).name() 
               << " >(" << this->name << ")::update()" << endl;
#endif
    if( this->owner )
      this->value = 
        static_cast< SField<Type>* >
        (this->event.ptr)->getValue( this->owner->id );
    else 
      this->value = 
        static_cast< SField<Type>* >(this->event.ptr)->getValue();
  }

  template< class Type >
  void SField<Type>::setValue( const Type &v, int id ) {
#ifdef DEBUG
    Console(1) << "SField< " << typeid( Type ).name() 
               << " >(" << this->name << ")::setValue()" << endl;
#endif
    // check that we have the correct access type
    this->checkAccessTypeSet( id );
    
    value = v;
    // reset the event pointer since we want to ignore any pending
    // events when the field is set to a new value.
    this->event.ptr = NULL;
    // generate an event.
    this->startEvent();
  }

  template< class Type >
  const Type &SField<Type>::getValue( int id ) {
#ifdef DEBUG
    Console(1) << "SField< " << typeid( Type ).name() 
               << " >(" <<  this->name << ")::getValue()" << endl;
#endif
    // check that we have the correct access type
    this->checkAccessTypeGet( id );

    // check that the field is up-to-date first
    this->upToDate();
    return value;
  }
}
#endif
