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
/// \file MField.h
/// \brief Contains base classes used to create multivalued fields.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFIELD_H__
#define __MFIELD_H__

#include <string>
#include <set>
#include <vector>
#include <algorithm>

#include <H3D/TypedField.h>
#include <H3D/Node.h>
#include <H3D/X3DFieldConversion.h>

using namespace std;

namespace H3D {

  class H3DAPI_API MFieldClass {
  public:
    // Virtual destructor.
    virtual ~MFieldClass() {};

    /// Set the value of the field given a pointer to where the value
    /// of the field is. 
    /// \param data A pointer to the data.
    /// \param nr_elements The number of values in the mfield.
    /// \param size The size in bytes of the each value stored in the data.
    /// \returns 0 if successful, -1 otherwise.
    virtual int setValueFromVoidPtr( void *data, unsigned int nr_elements, 
                                     unsigned int size, int id = 0 ) = 0;

    /// Get the value of the data copied into a memory buffer.
    /// \param data Buffer to copy the data into.
    /// \param nr_elements This parameter will be set to the nr of values
    /// in the mfield.
    /// \param size The size of the buffer.
    /// \returns If successful: The number of bytes that was copied into the 
    /// Otherwise -1.
    virtual int getValueAsVoidPtr( void *data, unsigned int &nr_elements,
                                   unsigned int size, int id = 0 ) = 0;

    /// Returns the size in bytes of the value type the mfield encapsulates.
    virtual unsigned int valueTypeSize() = 0;

    /// Returns the number of values that is stored in the mfield.
    virtual unsigned int size() = 0; 
  };


  /// \class MFieldBase
  /// \brief The common base class for MField types and MFNode.
  /// It defines the common interface between MFNode and MField <>.
  /// It encapsulates a vector of values of a certain type with the Field
  /// mechanisms.
  /// \param Type The type the values in the vector.
  /// \param VectorClass The actual vector type.
  /// \param BaseField The Field base class to inherit from.
  /// 
  template< class Type, 
            class VectorClass = std::vector< Type >, 
            class BaseField  = ParsableMField > 
  class MFieldBase: public TypedField< BaseField,
                                       void,
                                       AnyNumber< MFieldBase< Type, 
                                                              VectorClass, 
                                                              BaseField > > >,
                    public MFieldClass {
  public:
    /// The type of the value member. 
    typedef VectorClass vector_type;
    /// The return type of functions that return the value of the field.
    typedef VectorClass vector_return_type;
    /// The type of the values stored in the vector.
    typedef typename VectorClass::value_type value_type;
    /// Pointer to Type.
    typedef typename VectorClass::pointer pointer;
    /// Const reference to Type.
    typedef typename VectorClass::const_reference const_reference;
    /// An unsigned integral type.
    typedef typename VectorClass::size_type size_type;
    /// A signed integral type.
    typedef typename VectorClass::difference_type difference_type;  
    /// Const iterator used to iterate through a vector.
    typedef typename VectorClass::const_iterator const_iterator;
    /// Const iterator used to iterate backwards through a vector.
    typedef typename VectorClass::const_reverse_iterator 
    const_reverse_iterator;

    /// Returns a const_iterator pointing to the beginning of the vector.
    inline const_iterator begin( int id = 0 ) { 
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      this->upToDate();
      return value.begin();
    }
    
    /// Returns a const_iterator pointing to the end of the vector.
    inline const_iterator end( int id = 0 ) { 
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      this->upToDate();
      return value.end(); 
    }
    
    /// Returns a const_reverse_iterator pointing to the beginning of the
    /// reversed vector.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline const_reverse_iterator rbegin( int id = 0 ) { 
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      this->upToDate();
      return value.rbegin();
    }
    
    /// Returns a const_reverse_iterator pointing to the end of the reversed 
    /// vector.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline const_reverse_iterator rend( int id = 0 ) { 
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      this->upToDate();
      return value.rend(); 
    }

    /// Returns the size of the vector.
    inline unsigned int size() { 
      this->upToDate();
      return (unsigned int)value.size(); 
    }

    /// Returns the largest possible size of the vector.
    inline size_type max_size() {
      this->upToDate();
      return value.max_size();
    }
        
    /// Number of elements for which memory has been allocated. capacity() 
    /// is always greater than or equal to size().
    inline size_type capacity() { 
      this->upToDate();
      return value.capacity(); 
    }
        
    /// A request for allocation of additional memory. If s is less than
    /// or equal to capacity(), this call has no effect. 
    /// Otherwise, it is a request for allocation of additional memory. 
    /// If the request is successful, then capacity() is greater than or 
    /// equal to s; otherwise, capacity() is unchanged. In either case, 
    /// size() is unchanged.
    /// 
    inline void reserve( size_type s ) { 
      this->upToDate();
      value.reserve( s ); 
    }

    /// Inserts or erases elements at the end such that the size becomes n.
    inline void resize( size_type n, Type t = Type(), int id = 0 ) { 
      this->checkAccessTypeSet( id );
      this->upToDate();
      value.resize( n );
      this->startEvent();
    }

    /// true if the vector's size is 0.
    inline bool empty() { 
      this->upToDate();
      return value.empty();
    }

    /// Returns the n'th element.
    inline const_reference operator[](size_type n ) {
      // check that we have the correct access type
      this->checkAccessTypeGet( 0 );
      this->upToDate();
      return value[n];
    }

    /// Returns the first element.
    inline const_reference front( int id = 0 ) { 
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      this->upToDate();
      return value.front(); 
    }

    /// Returns the last element.
    inline const_reference back( int id = 0 ) {
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      this->upToDate();
      return value.back(); 
    }

    /// Swaps the contents of two vectors.
    inline void swap( VectorClass &x, int id = 0 ) {
      // check that we have the correct access type
      this->checkAccessTypeSet( id );
      this->checkAccessTypeGet( id );
      this->upToDate();
      this->value.swap( x );
      this->startEvent();
    }

    /// Inserts a new element at the end.
    inline void push_back( const Type &x, int id = 0 ) {
      // check that we have the correct access type
      this->checkAccessTypeSet( id );
      this->upToDate();    
      this->value.push_back( x );
      this->startEvent();
    }

    /// Removed the last element.
    void pop_back( int id = 0 ) {
      // check that we have the correct access type
      this->checkAccessTypeSet( id );
      this->upToDate();
      value.pop_back();
      this->startEvent();
    }

    /// Erases all of the elements.
    inline void clear( int id = 0 ) {
      // check that we have the correct access type
      this->checkAccessTypeSet( id );
      value.clear();
      this->startEvent();
    }

    /// Set the value of the field given a pointer to where the value
    /// of the field is. 
    /// \param data A pointer to the data.
    /// \param nr_elements The number of values in the mfield.
    /// \param len The size in bytes of the each value stored in the data.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    /// \returns 0 if successful, -1 otherwise.
    inline virtual int setValueFromVoidPtr( void *data, 
                                            unsigned int nr_elements, 
                                            unsigned int len, int id = 0 ) {
      this->checkAccessTypeSet( id );
          
      if( len != sizeof( value_type ) * nr_elements )
      return -1;
      
      std::vector< Type > new_data( nr_elements );
      for( unsigned int i = 0; i < nr_elements; ++i ) {
        new_data[i] = static_cast< value_type * >( data )[i];
      }
      this->value.swap( new_data );
      this->startEvent();
      return 0;
    }

    /// Get the value of the data copied into a memory buffer.
    /// \param data Buffer to copy the data into.
    /// \param nr_elements This parameter will be set to the nr of values
    /// in the mfield.
    /// \param len The size of the buffer.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    /// \returns If successful: The number of bytes that was copied into the 
    /// Otherwise -1.
    inline virtual int getValueAsVoidPtr( void *data, 
                                          unsigned int &nr_elements,
                                          unsigned int len, 
                                          int id = 0 ) {
      unsigned int sz = sizeof( value_type );
      nr_elements = (unsigned int) this->value.size();
      if( len < sz * nr_elements ) {
        return -1;
      }
      
      value_type *data_ptr = 
        static_cast< value_type * >( data );

      this->upToDate();

      for( unsigned int i = 0; i < nr_elements; ++i ) {
        data_ptr[i] = value[i];
      }
      return sz * nr_elements;
    } 

    /// Returns the size in bytes of the value type the mfield encapsulates.
    inline virtual unsigned int valueTypeSize() {
      return sizeof( value_type );
    }

    /// Default constructor. Creates an empty MField.
    MFieldBase() {};

    /// Creates a MField with space reserved for n elements.
    MFieldBase( size_type sz ) : value( sz ) {};

    /// Returns a string name for this field type e.g. MFInt32
    static string classTypeName() {
      return typeid( MFieldBase< Type, VectorClass, BaseField > ).name();
    }

  protected:
    /// The encapsulated vector.
    VectorClass value;
  };

  /// \ingroup FieldTemplateModifiers   
  /// Template class that adds the Field mechanism to a vector of values.
  /// 
  /// \param Type The type of the values in the vector.
  ///     
  template< class Type >
  class MField: public MFieldBase< Type, 
                                   std::vector< Type >, 
                                   ParsableMField > {
    typedef MFieldBase< Type, 
                        std::vector< Type >, 
                        ParsableMField > BaseMField;
     
  public:
    /// iterator used to iterate through a vector.
    typedef typename std::vector< Type >::iterator iterator;

    /// Thrown if the index given to getValueByIndex() is outside the 
    /// boundaries.
    H3D_VALUE_EXCEPTION( typename BaseMField::size_type, InvalidIndex );

    /// Default constructor. Creates an empty MField.
    MField() {}

    /// Creates an MField with space reserved for size nodes.
    MField( typename BaseMField::size_type sz ) : 
      BaseMField( sz ) {}
        
    /// Get the value of the MField.
    inline virtual const std::vector< Type > &getValue( int id = 0 );

    /// Get the value of an element of the MField.
    /// \param i The index of the element.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual typename std::vector<Type>::const_reference
    getValueByIndex( typename BaseMField::size_type i, int id = 0 ) {
#ifdef DEBUG
      Console(1) << "MField(" << this->name << ")::getValue()" << endl;
#endif
      // check that we have the correct access type
      this->checkAccessTypeGet( id );
      
      // check that the field is up-to-date first
      this->upToDate();
      // i < 0 is never true..
      if( /*i < 0 || */ i >= this->value.size() ) {
        stringstream s;
        s << "Trying to access value outside the bounds of field "
          << this->getFullName() << ". Field has size " << this->value.size()
          << ". ";
        throw InvalidIndex( i, s.str(), H3D_FULL_LOCATION );
      }
      return this->value[i];    
    }

    /// Set the value of the field.
    /// \param v The new value.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual void setValue( const std::vector< Type > &v, int id = 0  );

    /// Change the value of one element in the MField.
    /// \param i The index of the value to set.
    /// \param v The new value.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual void setValue( typename BaseMField::size_type i,
                                  const Type &v, int id = 0  ) {
#ifdef DEBUG
      Console(1) << "MField< " << typeid( Type ).name() 
           << " >(" << this->name << ")::setValue()" << endl;
#endif
      // check that we have the correct access type
      this->checkAccessTypeSet( id );
      this->value[i] = v; //.set( i, v );
      // reset the event pointer since we want to ignore any pending
      // events when the field is set to a new value.
      this->event.ptr = NULL;
      // generate an event.
      this->startEvent();                                  
    }

    /// Set the value of the field given a string. By default
    /// we try to parse the values according to the X3D/XML 
    /// specification.
    inline virtual void setValueFromString( const string &s ) {
      std::vector< Type > v;
      X3D::X3DStringToVector< std::vector< Type > >( s, v ); 
      setValue( v );
    }

    /// Add a new element to an MField from a string value.
    inline virtual void addElementFromString( const string &s ) {
      this->push_back(  X3D::X3DStringToValue< Type >( s )  );
    }

    /// Get the value of the field as a string. If the field contains
    /// multiple values the separator string is used between the values.
    inline virtual string getValueAsString( const string& separator = " " ) {
      stringstream s;
      const std::vector< Type > &v = getValue();
      
      if( v.size() == 0 )
        return "";
      unsigned int i;
      for( i = 0; i < v.size() - 1; ++i )
        s << v[i] << separator;
      s << v[i];
      return s.str();
    }

    /// Inserts x before pos.
    inline typename MField<Type>::const_iterator insert( typename MField<Type>::const_iterator pos,
                     const Type &x, 
                     int id = 0 ) {
      this->checkAccessTypeSet( id );
      this->upToDate();
      iterator i = this->value.insert( this->iteratorFromConst ( pos ), x );
      this->startEvent();
      return i;
    }
        
   /// Inserts the range [first, last) before pos.
   template <class InputIterator>
   void insert(typename MField<Type>::const_iterator pos,
               InputIterator first, 
               InputIterator last,
               int id = 0 ) {
     this->checkAccessTypeSet( id );
     this->upToDate();
     this->value.insert( this->iteratorFromConst ( pos ), first, last );
     this->startEvent();
   } 
            
    /// Inserts n copies of x before pos.
    inline void insert(typename MField<Type>::const_iterator pos, 
                       typename BaseMField::size_type n, const Type &x, int id = 0 ) {
      this->checkAccessTypeSet( id );
      this->upToDate();
      this->value.insert( this->iteratorFromConst ( pos ), n, x );
      this->startEvent();
    }
    
    /// Erases the element at position pos.
    inline virtual void erase( typename MField<Type>::const_iterator pos, int id = 0 ) { 
      this->checkAccessTypeSet( id );
      this->upToDate();
      this->value.erase( this->iteratorFromConst ( pos ) );
      this->startEvent();
    }
        
    /// Erases the range [first, last)
    inline virtual void erase( typename MField<Type>::const_iterator first, 
                               typename MField<Type>::const_iterator last, int id = 0 ) {
      this->checkAccessTypeSet( id );
      this->upToDate();
      this->value.erase( 
        this->iteratorFromConst ( first ), 
        this->iteratorFromConst ( last ) );
      this->startEvent();
    }
    
    
    /// Erase the first element equal to a.
    inline virtual void erase( const Type &a, int id = 0 ) {
      iterator i = std::find( this->value.begin(), this->value.end(), a );
      if( i != this->value.end() ) {
        this->value.erase( i );
      }
    } 

    /// Returns a string name for this field type e.g. MFInt32
    virtual string getTypeName() {
      return this->classTypeName();
    }

    /// Set the value of the field given a string. By default
    /// we try to parse the values according to the X3D/XML 
    /// specification.
    virtual size_t getSize( ) {
      const std::vector< Type > &v = getValue();
      return v.size();
    }

    /// Get the value of an element of the field as a string.
    inline virtual string getElementAsString( size_t element ) {
      stringstream s;
      const std::vector< Type > &v = getValue();
      
      if( element >= v.size() )
        throw InvalidIndex( element, "getElementAsString", H3D_FULL_LOCATION );
      
      s << v[element];
      return s.str();
    }


  protected:
    /// Make the field up to date given that an event has occured.
    inline virtual void update();

    /// Helper function to get an iterator from a const_iterator
    inline iterator iteratorFromConst ( typename MField<Type>::const_iterator pos ) {
      return this->value.begin()+(pos-this->value.begin());
    }
  };

  template< class Type  >
  void MField< Type >::update() {
#ifdef DEBUG
    Console(1) << "MField< " << typeid( Type ).name() 
         << " >(" << this->name << ")::update()" << endl;
#endif
    if( this->owner )
      this->value = 
        static_cast< MField<Type>* >
        (this->event.ptr)->getValue( this->owner->id );
    else 
      this->value = 
        static_cast< MField<Type>* >(this->event.ptr)->getValue();
  }

  template< class Type  >
  void MField< Type >::setValue( const std::vector< Type > &v, int id ) {
#ifdef DEBUG
    Console(1) << "MField< " << typeid( Type ).name() 
         << " >(" << this->name << ")::setValue()" << endl;
#endif
    // check that we have the correct access type
    this->checkAccessTypeSet( id );
    this->value = v;
    // reset the event pointer since we want to ignore any pending
    // events when the field is set to a new value.
    this->event.ptr = NULL;
    // generate an event.
    this->startEvent();
  }

  template< class Type >
  const std::vector< Type > &MField<Type >::getValue( int id ) {
#ifdef DEBUG
    Console(1) << "MField< " << typeid( Type ).name() 
         << " >(" << this->name << ")::getValue()" << endl;
#endif
    // check that we have the correct access type
    this->checkAccessTypeGet( id );

    // check that the field is up-to-date first
    this->upToDate();
    return this->value;
  }
    
}

#endif

