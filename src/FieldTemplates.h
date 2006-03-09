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

