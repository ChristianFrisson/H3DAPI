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
/// \file FieldTemplates.h
/// \brief Contains different templates to modify field behaviour.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FIELDTEMPLATES_H__
#define __FIELDTEMPLATES_H__

#include <string>
#include <set>
#include <vector>

#include <H3D/SField.h>
#include <H3D/MField.h>
#include <H3D/TypedField.h>

using namespace std;

namespace H3D {

  H3D_VALUE_EXCEPTION( string, InvalidNodeType );

  /// \ingroup FieldTemplateModifiers
  /// Template for adding the virtual function onValueChange that can be overridden
  /// in subclasses in order to perform actions when the value of the field changes
  /// in any way, i.e. it will only be called when the value is updated or set
  /// to a value that is different than its previous value.
  template < class SF >
  class OnValueChangeSField: public SF {
  public:
    virtual void setValue( const typename SF::value_type &v, int id = 0  ) {
      typename SF::value_type old_value = this->value;
      SF::setValue( v, id );
        if( this->value != old_value ) {
          onValueChange( this->value );
        } 
    }

  protected:
    /// This function is called when the value in the field has changed. 
    virtual void onValueChange( const typename SF::value_type &new_value ) = 0;

    virtual void update() {
      typename SF::value_type old_value = this->value;
      SF::update();
      if( this->value != old_value ) {
        onValueChange( this->value );
      } 
    }
  };

  /// \ingroup FieldTemplateModifiers
  /// Template for adding the virtual function onNewValue that can be overridden
  /// in subclasses in order to perform actions when the value is updated in any
  /// way( setValue or update ). The difference between this and 
  /// OnValueChangeSField is that the function is called any time the setValue
  /// or update function is called, even if the new value is the same as the old.
  template < class SF >
  class OnNewValueSField: public SF {
  public:
    virtual void setValue( const typename SF::value_type &v, int id = 0 ) {
      SF::setValue( v, id );
      onNewValue( this->value );
    }

  protected:
    /// This function is called when the field is updated to a value.
    virtual void onNewValue( const typename SF::value_type &new_value ) = 0;

    virtual void update() {
      SF::update();
      onNewValue( this->value );
    }
  };

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
        s << "Expecting " << typeid( RefCountedType ).name();
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

  /// The EventCollection field collects all fields routed to it that
  /// generates event between calls to the update function of the field.
  /// Good to use if you have several fields and want to know which ones
  /// of them that has generated an event since last update.
  template< class FieldType >
  class EventCollectingField : public FieldType {
  protected:
    
    virtual void update() {
      FieldType::update();
      event_fields.clear();
    }
    
    /// When the event is propagated the field that created the event is
    /// saved
    virtual void propagateEvent( Field::Event e ) {
      FieldType::propagateEvent( e );
      event_fields.insert( e.ptr );
    }
    
  public:
    /// Returns true if the Field given has generated an event to this
    /// field since the last call to the update() function.
    inline bool hasCausedEvent( Field *f ) {
      return event_fields.find( f ) != event_fields.end();
    }
    
    /// Returns true if the Field given has generated an event to this
    /// field since the last call to the update() function.
    template< class FieldType2 >
    inline bool hasCausedEvent( auto_ptr< FieldType2 > &f ) {
      return hasCausedEvent( f.get() );
    }
    
  private:
    /// The fields that has generated an event since the last call to
    /// update()
    set< Field * > event_fields;
  };

}

#endif

