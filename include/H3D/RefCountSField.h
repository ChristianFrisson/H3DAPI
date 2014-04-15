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
/// \file RefCountSField.h
/// \brief Contains the RefCountSField class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __REFCOUNTSFIELD_H__
#define __REFCOUNTSFIELD_H__

#include <H3DUtil/AutoRef.h>
#include <H3DUtil/RefCountedClass.h>
#include <H3D/Field.h>

namespace H3D {

  /// RefCountSField is almost like any SField but it encapsulates a pointer
  /// to a RefCountedClass. Reference counting on the encapsulated 
  /// RefCountedClass is upheld when the value is changed in the field.  
  /// \ingroup FieldTemplateModifiers
  template< class RefClass, class BaseField = H3D::Field >
  class RefCountSField: public BaseField {
  public: 
    /// The same as class_type, but if e.g. TypedSFNode template
    /// modifier has been used the typed_class_type is the 
    /// type specified to that template.
    typedef RefClass typed_class_type;
    /// The same as value_type, but if e.g. TypedSFNode template
    /// modifier has been used the typed_class_type is the 
    /// type specified to that template.
    typedef RefClass * typed_value_type;
    /// The type of which this field contains a pointer to.
    typedef RefClass class_type;
    /// The type of the value in this field.
    typedef RefClass * value_type;
    
    /// Constructor. Default value of value is NULL.
    RefCountSField(): value( NULL ) {
      value.rc_field = this;
    };
      
    /// Constructor.
    RefCountSField( RefClass *_value ) : value( _value ) {
      value.rc_field = this;
    }
      
    /// Set the value of the field.
    inline virtual void setValue( const AutoRef< RefClass > &n, int id = 0 );
      
    /// Set the value of the field.
    inline virtual void setValue( RefClass *n, int id = 0 );
      
    /// Get the value of the field.
    inline virtual RefClass *getValue( int id = 0 );
      
  protected:
    /// Make the field up to date if an event has occured
    inline virtual void update();
      
    virtual RefClass *preOnAdd( RefClass *n ) {
      return n;
    }
    
    virtual RefClass *preOnRemove( RefClass *n ) {
      return n;
    }

    /// \internal
    /// The RefCountSFieldAutoRef is a AutoRef class used in 
    /// RefCountSField in order to call the onAdd and onRemove 
    /// functions when the value is changed.
    ///
    class RefCountSFieldAutoRef: public AutoRef< RefClass > {
    public :

      /// Constructor.   
      explicit RefCountSFieldAutoRef( RefClass* n = 0) throw() :
        AutoRef< RefClass >( n ) {
    
      }

      /// Assignment operator.
      RefCountSFieldAutoRef& operator=(RefClass* n) throw() {
        this->reset( n );
        return *this;
      }
  
      /// Assignment operator.
      template< class RefClassClass >
      RefCountSFieldAutoRef& operator=(const AutoRef<RefClassClass> &n) 
        throw() {
        this->reset( n.get() );
        return *this;
      }
  
      /// Destructor.
      ~RefCountSFieldAutoRef() throw() {
        this->reset( NULL );
      } 
  
      /// This function is called when a RefClass * is to be held by the 
      /// AutoRef.
      /// It increments the reference counter of the RefClass by calling
      /// the ref() function. Subclasses can override this function in
      /// order to get specialized behaviour.
      /// \param n The node that is to be held by the AutoRef
      ///
      inline virtual void ref( RefClass *n ) {
        if( rc_field && n ) {
          AutoRef< RefClass >::ref( n );
          rc_field->onAdd( rc_field->preOnAdd( n ) );
        }
      }
  
      /// This function is called when a RefClass * is released by the 
      /// AutoRef. It decrements the reference counter of the RefClass
      /// by calling the unref() function. Subclasses can override this
      /// function in order to get specialized behaviour.
      /// \param n The node being released by the AutoRef.
      /// 
      inline virtual void unref( RefClass *n ) {
        if( rc_field && n ) {
          rc_field->onRemove( rc_field->preOnRemove( n ) );
          AutoRef< RefClass >::unref( n );
        }
      }
      /// The RefCountSField that contains the instance.
      RefCountSField *rc_field;
    };
#ifdef __BORLANDC__
    friend class RefCountSFieldAutoRef;
#endif
    /// This function will be called when the value of RefCountSField
    /// changes. As soon as the value changes onRemove will
    /// be called on the old value and onAdd will be called
    /// on the new value. 
    /// \param n The new value.
    ///
    virtual void onAdd( RefClass *n ) {
#ifdef REF_COUNT_DEBUG
      Console(1) << "RefCountSField: Add node" << endl;
#endif
    }
      
    /// This function will be called when the value of RefCountSField
    /// changes. As soon as the value changes onRemove will
    /// be called on the old value and onAdd will be called
    /// on the new value. 
    /// \param n The old value.
    ///
    virtual void onRemove( RefClass *n ) {
#ifdef REF_COUNT_DEBUG
      Console(1) << "RefCountSField: Remove node" << endl;
#endif
    }
      
    /// The encapsulated value.
    RefCountSFieldAutoRef value;
  };
  
  template< class RefClass, class BaseField >
  void RefCountSField< RefClass, BaseField >::update() {
#ifdef REF_COUNT_DEBUG
    Console(1) << "RefCountSField(" << name << ")::update()   event_ptr = " 
               << event.ptr << endl;
#endif
    value = static_cast< RefCountSField* >(this->event.ptr)->getValue();
    
  }
  
  template< class RefClass, class BaseField >
  inline void RefCountSField< RefClass, BaseField >::setValue( RefClass *n,
                                                               int id ) {
    this->checkAccessTypeSet( id );
#ifdef REF_COUNT_DEBUG
    Console(1) << "RefCountSField(" << name << ")::setValue()" << endl;
#endif
    value = n;
    // generate an event.
    this->startEvent();
  }
  
  template< class RefClass, class BaseField >
  void RefCountSField< RefClass, BaseField >::setValue( 
                                             const AutoRef< RefClass > &n,
                                             int id ) {
    this->setValue( n.get(), id );
  }
  
  template< class RefClass, class BaseField >
  RefClass *RefCountSField< RefClass, BaseField >::getValue( int id ) {
#ifdef REF_COUNT_DEBUG
    Console(1) << "RefCountSField(" <<  name << ")::getValue()" << endl;
#endif
    this->checkAccessTypeGet( id );
    // check that the field is up-to-date first
    this->upToDate();
    return value.get();
  }
  
}

#endif
