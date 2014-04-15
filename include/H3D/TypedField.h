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
/// \file TypedField.h Contains the TypedField template class.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TYPEDFIELD_H__
#define __TYPEDFIELD_H__

#include <sstream>
#include <H3D/TypedFieldCheck.h>

namespace H3D {
  using namespace TypedFieldTypeCheck;
  /// A template modifier class for adding type checking on the
  /// routes to any Field class.
  ///
  /// \param BaseField The Field type base class.  
  ///
  /// \param RequiredArgTypes The types of the routes that are required to be
  /// there. If these routes are not present when the evaluate() function is
  /// called then an exception is thrown. The types can be specified with the
  /// \link TypedFieldTypeCheck::Types
  /// Types
  /// \endlink 
  /// and 
  /// \link TypedFieldTypeCheck::Any
  /// Any
  /// \endlink 
  /// templates. 
  /// The Types template specifies a list of types and
  /// the Any template defines the type to be any of the types given in it.
  /// void can be used to specify that there are no required routes.
  ///
  /// \param OptionalArgTypes Types of optional routes. These do not have to 
  /// be there in order for the field to be able to evaluate. In addition to 
  /// the templates Any and Types used for RequiredArgTypes the template
  /// \link TypedFieldTypeCheck::AnyNumber
  /// AnyNumber 
  /// \endlink 
  /// can be used to specify that any number
  /// of the route types specified in the template are valid. 
  /// void can be used to specify that there are no required routes.
  ///
  /// \par Examples:
  /// TypedField< SFInt32, SFInt32 > defines an SFInt32 field that requires 
  /// one and only one DFInt32 field routed to it.
  ///
  /// \par
  /// TypedField< MFFloat, void, AnyNumber<SFFloat> > defines an MFFloat 
  /// that can have any number of SFFloat fields routed to it.
  /// 
  /// \par
  /// TypedField< SFInt32, Types< SFFloat, SFFloat >, Types< Any< SFInt32, MFInt32 > > >
  /// defines a SFInt32 field that requires two SFFloat routes and can
  /// optionally have an extra SFInt32 or MFInt32 route.
  ///
  /// \ingroup FieldTemplateModifiers    

  template< class BaseField, 
            class RequiredArgTypes, 
            class OptionalArgTypes = void >
  class TypedField : public BaseField {
  public:
    TypedField() {
      try {
        checkTemplateArguments();
      } catch( const InvalidTemplateArguments &e ) {
        stringstream s;
        s << e << " in definition of field named \"" << this->getFullName() 
          << "\". ";
        throw TypedFieldError( s.str() );
      }
    }
        
  protected:
    /// Function for checking that a field is of a correct type 
    /// given type specification according to the TypedField 
    /// template arguments.
    /// \param f The field to check the type of
    /// \param index The index of the field in the template
    /// type list.
    /// \throws TypedFieldCheck::InvalidType
    /// 
    void checkFieldType( Field *f, int index ) {
      try {
        TypedFieldCheck< RequiredArgTypes,
          OptionalArgTypes >::checkFieldType( f, index, 0 );
      } catch( TypedFieldError &e ) {
        stringstream s;
        s << e.message << " Source field: \"" << f->getFullName() 
          << "\". Destination field: \"" << this->getFullName()
          << "\".";
        e.message = s.str();
        throw e;
      }
    }

    /// Function for checking that a field given TypedField type 
    /// specification is valid.
    /// \throws TypedFieldCheck::InvalidTemplateArguments
    /// 
    void checkTemplateArguments() {
      TypedFieldCheck< RequiredArgTypes,
        OptionalArgTypes >::checkTemplateArguments();
    }
  }; 
}

#endif
