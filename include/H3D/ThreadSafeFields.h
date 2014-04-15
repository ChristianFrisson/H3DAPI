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
/// \file ThreadSafeFields.h
/// \brief Header file for ThreadSafeFields, template field modifier
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __THREAD_SAFE_FIELDS_H__
#define __THREAD_SAFE_FIELDS_H__

#include <assert.h>
#include <H3D/PeriodicUpdate.h>
#include <H3DUtil/Threads.h>

namespace H3D {
  
  /// \ingroup FieldTemplateModifiers    
  /// \brief A template modifier to add thread safety to an SField 
  /// type.
  /// The field network should never be accessed from anything else 
  /// than the main thread. Often though values provided by fields
  /// control behaviour of things happening in the haptic threads.
  /// Therefore separate copies of the field values have to be maintained
  /// for usage in the haptics thread. This field template modifies
  /// a normal SField class to contain such a copy. The getValue() and
  /// setValue() functions can then be used by both the haptics thread
  /// and the main thread and depending on from which thread it is called
  /// the copy or the field value is returned. Once per scene-graph loop
  /// it is checked whether either the copy or the field network value 
  /// has changed since last loop and if it has the values are updated 
  /// in a thread safe manner to contain the same value.
  template< class BaseField >
  class ThreadSafeSField: public PeriodicUpdate< BaseField > {
  public:
    /// Constructor.
    ThreadSafeSField(): rt_value_changed( false ) {}
      
    /// Set the value of the field. 
    inline virtual void setValue( const typename BaseField::value_type &v,
                                  int id = 0 ) {
      if( H3DUtil::HapticThread::inHapticThread() ) {
        rt_value = v;
        rt_value_changed = true;
      } else {
        assert( H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::setValue( v, id );;
        void * param[] = { &this->value, &rt_value };
        H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Get the value of the field.
    inline virtual const typename BaseField::value_type &getValue( int id = 0 ) {
      if( H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value;
      } else {
        assert( H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::getValue( id );
      }
    }
    
    /// Make sure that the field is up-to-date. upToDate() is specialized to 
    /// transfer the rt_value to the field if it has been changed.
    virtual void upToDate() {
      assert( H3DUtil::ThreadBase::inMainThread() );
      
      if( rt_value_changed ) {
        void * param[] = { &rt_value, &this->value };
        H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
        this->startEvent();
      } else {
        PeriodicUpdate< BaseField >::upToDate();
      }
    }

  protected:
    /// Callback function to transfer copy a value between two
    /// pointers of the same type.
    static H3DUtil::PeriodicThread::CallbackCode transferValue( void * _data ) {
      void * * data = static_cast< void * * >( _data );
      typename BaseField::value_type *new_value = 
        static_cast< typename BaseField::value_type * >( data[0] );
      typename BaseField::value_type *rt_value = 
        static_cast< typename BaseField::value_type * >( data[1] );
      *rt_value = *new_value;
      return H3DUtil::PeriodicThread::CALLBACK_DONE;
    }

    /// The update function is specialized to synchronize with the
    /// haptics threads and copy the new value of the field to the
    /// rt_value member in a thread safe way.
    inline virtual void update() {
      assert( H3DUtil::ThreadBase::inMainThread() );
      PeriodicUpdate< BaseField >::update();;
      void * param[] = { &this->value, &rt_value };
      H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
    }
    
    /// The copy of the field value to be used in the haptics threads.
    typename BaseField::value_type rt_value;

    /// Flag indicating if the rt_value has been changed by a haptics 
    /// thread.
    bool rt_value_changed;
  };
  
  /// \ingroup FieldTemplateModifiers    
  /// \brief A template modifier to add thread safety to an RefCountedSField
  /// type.
  /// The field network should never be accessed from anything else 
  /// than the main thread. Often though values provided by fields
  /// control behaviour of things happening in the haptic threads.
  /// Therefore separate copies of the field values have to be maintained
  /// for usage in the haptics thread. This field template modifies
  /// a normal RefCountedSField class to contain such a copy. 
  /// The getValue() and settValue() functions can then be used by both
  /// the haptics thread and the main thread and depending on from which
  /// thread it is called the copy or the field value is returned. 
  /// Once per scene-graph loop it is checked whether either the copy 
  /// or the field network value has changed since last loop and if it
  /// has the values are updated in a thread safe manner to contain the
  /// same value.
  template< class BaseField > 
  class ThreadSafeRefSField:  public PeriodicUpdate< BaseField > {
  public:
    /// Constructor.
    ThreadSafeRefSField(): rt_value_changed( false ) {}

    /// Set the value of the field. 
    inline virtual void setValue( typename BaseField::value_type v,
                                  int id = 0 ) {
      if( H3DUtil::HapticThread::inHapticThread() ) {
        rt_value.reset( v );
        rt_value_changed = true;
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::setValue( v, id );
        void * param[] = { v, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Get the value of the field.
    virtual typename BaseField::typed_value_type getValue( int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return static_cast< typename BaseField::typed_value_type >( rt_value.get() );
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::getValue( id );
      }
    }
    
    /// Make sure that the field is up-to-date. upToDate() is specialized to 
    /// transfer the rt_value to the field if it has been changed.
    virtual void upToDate() {
      assert(  H3DUtil::ThreadBase::inMainThread() );
      
      if( rt_value_changed ) {
        void * param[] = { rt_value.get(), &this->value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
        this->startEvent();
      } else {
        PeriodicUpdate< BaseField >::upToDate();
      }
    }

  protected:
    static H3DUtil::PeriodicThread::CallbackCode transferValue( void * _data ) {
      void * * data = static_cast< void * * >( _data );
      typename BaseField::value_type new_value = 
        static_cast< typename BaseField::value_type >( data[0] );
      AutoRef< typename BaseField::class_type > *rt_value = 
        static_cast< AutoRef< typename BaseField::class_type> * >( data[1] );
      rt_value->reset( new_value );
      return H3DUtil::PeriodicThread::CALLBACK_DONE;
  }

    /// onAdd is extended to change the rt_image value in a thread safe way.
    virtual void onAdd( typename BaseField::value_type i ) {
      assert(  H3DUtil::ThreadBase::inMainThread() );
      PeriodicUpdate< BaseField >::onAdd( i );
      void * param[] = { i, &rt_value };
       H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
    }
    
    /// The reference for the haptics loop.
    AutoRef< typename BaseField::class_type > rt_value;

    /// Flag indicating if the rt_value has been changed by a haptics 
    /// thread.
    bool rt_value_changed;
  };

  /// \ingroup FieldTemplateModifiers    
  /// \brief A template modifier to add thread safety to an MField 
  /// type.
  /// The field network should never be accessed from anything else 
  /// than the main thread. Often though values provided by fields
  /// control behaviour of things happening in the haptic threads.
  /// Therefore separate copies of the field values have to be maintained
  /// for usage in the haptics thread. This field template modifies
  /// a normal MField class to contain such a copy. The getValue() and
  /// setValue() functions can then be used by both the haptics thread
  /// and the main thread and depending on from which thread it is called
  /// the copy or the field value is returned. Once per scene-graph loop
  /// it is checked whether either the copy or the field network value 
  /// has changed since last loop and if it has the values are updated 
  /// in a thread safe manner to contain the same value.
  template< class BaseField >
  class ThreadSafeMField: public PeriodicUpdate< BaseField > {
  public:
    /// Constructor.
    ThreadSafeMField(): rt_value_changed( false ) {}

    /// Set the value of the field.
    /// \param v The new value.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual void setValue( 
                 const vector< typename BaseField::value_type > &v,
                 int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        rt_value = v;
        rt_value_changed = true;
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::setValue( v, id );
        void * param[] = { &this->value, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Change the value of one element in the MField.
    /// \param i The index of the value to set.
    /// \param t The new value.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual void setValue( typename BaseField::size_type i,
                                  const typename BaseField::value_type &t,
                                  int id = 0  ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        rt_value[i] = t;
        rt_value_changed = true;
      } else {
        // TODO: don't copy the entire rt_value vector, just change the current one.
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::setValue( i, t, id );
        void * param[] = { &this->value, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Swaps the contents of two vectors.
    inline virtual void swap( typename BaseField::vector_type &x, int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        rt_value.swap( x );
        rt_value_changed = true;
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::swap( x, id );
        void * param[] = { &this->value, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Inserts a new element at the end.
    inline virtual void push_back( const typename BaseField::value_type &x,
                                   int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        rt_value.push_back( x );
        rt_value_changed = true;
      } else {
        // TODO: don't copy the entire rt_value vector, just change the current one.
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::push_back( x, id );
        void * param[] = { &this->value, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Removed the last element.
    void pop_back( int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        rt_value.pop_back();
        rt_value_changed = true;
      } else {
        // TODO: don't copy the entire rt_value vector, just change the current one.
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::pop_back( id );
        void * param[] = { &this->value, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }
    
    /// Erases all of the elements.
    inline virtual void clear( int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        rt_value.clear();
        rt_value_changed = true;
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::clear( id );
        void * param[] = { &this->value, &rt_value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
      }
    }

    /// Returns a const_iterator pointing to the beginning of the vector.
    inline virtual typename BaseField::const_iterator begin( int id = 0 ) { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.begin();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::begin( id );
      }
    }
    
    /// Returns a const_iterator pointing to the end of the vector.
    inline virtual typename BaseField::const_iterator end( int id = 0 ) { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.end();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::end( id );
      }
    }
    
    /// Returns a const_reverse_iterator pointing to the beginning of the
    /// reversed vector.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual typename BaseField::const_reverse_iterator rbegin( int id = 0 ) { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.rbegin();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::rbegin( id );
      }
    }
    /// Returns a const_reverse_iterator pointing to the end of the reversed 
    /// vector.
    /// \param id Id of the node calling this function. Used to check 
    /// access type.
    inline virtual typename BaseField::const_reverse_iterator rend( int id = 0 ) { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.rend();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::rend( id );
      }
    }

    /// Returns the size of the vector.
    inline virtual unsigned int size() { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.size();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::size();
      }
    }

    /// Returns the largest possible size of the vector.
    inline virtual typename BaseField::size_type max_size() {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.max_size();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::max_size();
      }
    }
        
    /// Number of elements for which memory has been allocated. capacity() 
    /// is always greater than or equal to size().
    inline virtual typename BaseField::size_type capacity() {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.capacity();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::capacity();
      }
    }

    /// true if the vector's size is 0.
    inline virtual bool empty() { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.empty();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::empty();
      }
    }
    /// Returns the n'th element.
    inline virtual typename BaseField::const_reference operator[]( typename BaseField::size_type n ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value[n];
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::operator[]( n );
      }
    }

    /// Returns the first element.
    inline virtual typename BaseField::const_reference front( int id = 0 ) { 
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.front();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::front( id );
      }
    }

    /// Returns the last element.
    inline virtual typename BaseField::const_reference back( int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value.back();
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::back( id );
      }
    }

    /// Gets the value of the field.
    inline virtual const typename BaseField::vector_type &getValue( int id = 0 ) {
      if(  H3DUtil::HapticThread::inHapticThread() ) {
        return rt_value;
      } else {
        assert(  H3DUtil::ThreadBase::inMainThread() );
        return PeriodicUpdate< BaseField >::getValue( id );
      }
    }

    /// Make sure that the field is up-to-date. upToDate() is specialized to 
    /// transfer the rt_value to the field if it has been changed.
    virtual void upToDate() {
      assert(  H3DUtil::ThreadBase::inMainThread() );
      
      if( rt_value_changed ) {
        void * param[] = { &rt_value, &this->value };
         H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
        this->startEvent();
      } else {
        PeriodicUpdate< BaseField >::upToDate();
      }
    }

  protected:
    /// Callback function to transfer copy a value between two
    /// pointers of the same type.
    static H3DUtil::PeriodicThread::CallbackCode transferValue( void * _data ) {
      void * * data = static_cast< void * * >( _data );
      typename BaseField::vector_type *new_value = 
        static_cast< typename BaseField::vector_type * >( data[0] );
      typename BaseField::vector_type *rt_value = 
        static_cast< typename BaseField::vector_type * >( data[1] );
      *rt_value = *new_value;
      return H3DUtil::PeriodicThread::CALLBACK_DONE;
    }

    /// The update function is specialized to synchronize with the
    /// haptics threads and copy the new value of the field to the
    /// rt_value member in a thread safe way.
    inline virtual void update() {
      assert(  H3DUtil::ThreadBase::inMainThread() );
      PeriodicUpdate< BaseField >::update();;
      void * param[] = { &this->value, &rt_value };
       H3DUtil::HapticThread::synchronousHapticCB( transferValue, param );
    }
            
    /// The copy of the field value to be used in the haptics threads.
    typename BaseField::vector_type rt_value;

    /// Flag indicating if the rt_value has been changed by a haptics 
    /// thread.
    bool rt_value_changed;
  };
    
}

#endif







