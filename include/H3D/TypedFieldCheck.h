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
/// \file TypedFieldCheck.h 
/// \brief Contains a lot of help template classes for
/// checking the types of a field given a TypedField template argument
/// specification.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TYPEDFIELDCHECK_H__
#define __TYPEDFIELDCHECK_H__

#include <string>
#include <H3D/Field.h>
#include <H3DUtil/Exception.h>
#include <H3D/TypedFieldTypesTmpl.h>
#include <H3D/TypedFieldAnyTmpl.h>

namespace H3D {
  /// 
  /// \class TypedFieldError
  /// An exception thrown when the field does not conform to 
  /// the type specification from a TypedField.
  /// 
  H3D_API_EXCEPTION( TypedFieldError );
    
  /// Contains template classes for doing the type checking
  /// for the TypedField template class.
  ///
  namespace TypedFieldTypeCheck {

    /// 
    /// InvalidTemplateArguments
    /// An exception thrown when the template argument list does not
    /// follow the rules defined in TypedField.  
    /// 
    H3D_API_EXCEPTION( InvalidTemplateArguments );

    /// The AnyNumber template can be used in the OptionalArgTypes argument
    /// of a TypedField template instantiation. It defines that there can be
    /// an unlimited number of routes of the type given to the template.
    /// An AnyNumber template can contain Field types,
    /// Any <> template and Types <> template instantiations.
    ///
    /// \par Examples:
    /// AnyNumber< SFInt32 > means any number of SFInt32 routes.
    /// \par
    /// AnyNumber< Any< SFInt32, MFInt32 > > means any number of SFInt32 or 
    /// MFInt32 routes. 
    /// 
    template< class Type >
    struct AnyNumber {
      /// Check that the template arguments are on the correct form,
      /// i.e. only contains the a single type or valid Any <> template or 
      /// Types <> template instantiations.
      /// \throws InvalidTemplateArguments
      static void checkTemplateArguments() {
        TypesType< Type >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      /// 
      static void checkType( Field *f, int index ) {
        TypesType< Type >::checkType( f );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the AnyNumber 
    /// template contains a Types template class instantiation.
    ///
    template<H3D_TYPES_TEMPLATE_LIST>
    struct AnyNumber< H3D_TYPES_CLASS > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        H3D_TYPES_CLASS::checkTemplateArguments();
      }

      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) {
        int nr_types =  H3D_TYPES_CLASS::numberTypes();
        H3D_TYPES_CLASS::checkType( f, index % nr_types );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the AnyNumber 
    /// template contains a AnyNumber template class instantiation. This 
    /// is not allowed so an InvalidTemplateArguments exception
    /// is thrown in all functions.
    ///
    template< class Type >
    struct AnyNumber< AnyNumber< Type > > {
      /// AnyNumber <> instance in AnyNumber template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s="No AnyNumber statements allowed in AnyNumber statement.";
        throw InvalidTemplateArguments( s ); 
      }
      /// AnyNumber template instantiation is not allowed in a 
      /// AnyNumber template so we throw a 
      /// InvalidTemplateArguments exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkType( Field *f, int index ) {
        string s="No AnyNumber statements allowed in AnyNumber statement.";
        throw InvalidTemplateArguments( s ); 
      }
    };

    /// \internal
    /// AnyType is a help class for checking template arguments given to
    /// a Any <> template. Only normal types are allowed in the Any <> 
    /// template, i.e. no Any <>, Types <> or AnyNumber <> instances.
    ///
    template< class Type >
    struct AnyType {
      /// Check that the template argument list is correct. 
      /// \throws InvalidTemplateArguments
      //  It is correct in this case since it is a normal type
      //  (all illegal types are handled as template specialisations ).
      static void checkTemplateArguments() {}

      /// Check that the field is of the type specified in the template.
      /// \throws InvalidType
      static bool checkType( Field *f ) {
        return dynamic_cast< Type * >( f ) != NULL;
      }
    };

    /// \internal
    /// Template specialization to handle the case when the AnyType template
    /// contains a Any template class instantiation. AnyType does not allow 
    /// Any instances so InvalidTemplateArguments is thrown in
    /// all functions.
    ///
    template<H3D_ANY_TEMPLATE_LIST>
    struct AnyType< H3D_ANY_CLASS > {
      /// Any <> instance in AnyType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s = "No Any statements allowed in Any statement.";
        throw InvalidTemplateArguments( s );
      }

      /// Any <> instance in AnyType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static bool checkType( Field *f ) {
        string s = "No Any statements allowed in Any statement.";
        throw InvalidTemplateArguments( s );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the AnyType template
    /// contains a Types template class instantiation. AnyType does not 
    /// allow Types instances so InvalidTemplateArguments is 
    /// thrown in all functions.
    ///
    template<H3D_TYPES_TEMPLATE_LIST>
    struct AnyType< H3D_TYPES_CLASS > {

      /// Types <> instance in AnyType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s = "No Types statements allowed in Any statement.";
        throw InvalidTemplateArguments( s );
      }

      /// Types <> instance in AnyType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static bool checkType( Field *f ) {
        string s = "No Types statements allowed in Any statement.";
        throw InvalidTemplateArguments( s );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the AnyType template
    /// contains a AnyNumber template class instantiation. AnyType does not
    /// allow AnyNumber instances so InvalidTemplateArguments 
    /// is thrown in all functions.
    ///
    template< class Type >
    struct AnyType< AnyNumber< Type > > {

      /// AnyNumber <> instance in AnyType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s = "No AnyNumber statements allowed in Any statement.";
        throw InvalidTemplateArguments( s );
      }

      /// AnyNumber <> instance in AnyType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static bool checkType( Field *f ) {
        string s = "No AnyNumber statements allowed in Any statement.";
        throw InvalidTemplateArguments( s );
      }
    };

    /// \internal
    /// TypesType is a help class for checking template arguments given to
    /// a Types <> template. Only normal types and Any <> instances are 
    /// allowed i.e. no Types <> or AnyNumber <> instances.
    ///
    template< class Type >
    struct TypesType {
      /// Check that the template argument list is correct.
      /// \throws InvalidTemplateArguments
      //  It is correct in this case since it is a normal types 
      //  (all illegal types are handled as template specialisations ).
      static void checkTemplateArguments() {}

      /// Check that the field is of the type specified in the template.
      /// \throws InvalidType
      static void checkType( Field *f ) {
        if( !dynamic_cast< Type * >( f ) ) {
          throw InvalidType( f->getTypeName(),
                             Type::classTypeName() );
        }
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypesType 
    /// template contains a Any template class instantiation. 
    ///
    template< H3D_ANY_TEMPLATE_LIST >
    struct TypesType< H3D_ANY_CLASS > {
      /// Check that the template argument list is correct.
      /// \throws InvalidTemplateArguments
      static void checkTemplateArguments() {
        H3D_ANY_CLASS::checkTemplateArguments();
      }

      /// Check that the field is of the type specified in the template.
      /// \throws InvalidType
      static void checkType( Field *f ) {
        H3D_ANY_CLASS::checkType( f );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypesType 
    /// template contains a Types template class instantiation. AnyType
    /// does not allow Types instances so 
    /// InvalidTemplateArguments is thrown in all functions.
    ///
    template< H3D_TYPES_TEMPLATE_LIST >
    struct TypesType< H3D_TYPES_CLASS > {
      /// Types <> instance in TypesType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s = "No Types statements allowed in another Types statement.";
        throw InvalidTemplateArguments( s );
      }

      /// Types <> instance in TypesType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkType( Field *f ) {
        string s = "No Types statements allowed in another Types statement.";
        throw InvalidTemplateArguments( s );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypesType 
    /// template contains a AnyNumber template class instantiation. 
    /// TypesType does not allow AnyNumber instances so 
    /// InvalidTemplateArguments is thrown in
    /// all functions.
    ///
    template< class Type >
    struct TypesType< AnyNumber< Type > > {

      /// AnyNumber <> instance in TypesType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s ="No AnyNumber statements allowed in Types statement.";
        throw InvalidTemplateArguments( s );
      }

      /// AnyNumber <> instance in TypesType template not allowed, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static bool checkType( Field *f ) {
        string s ="No AnyNumber statements allowed in Types statement.";
        throw InvalidTemplateArguments( s );
      }
    };

    /// \internal
    /// Help class for checking that a field is of a correct type 
    /// given type specification according to the TypedField 
    /// template arguments.
    ///
    template< class RequiredArgType,
              class OptionalArgTypes >
    struct TypedFieldCheck {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType<RequiredArgType>::checkTemplateArguments();
        TypedFieldCheck< void,  
          OptionalArgTypes >::checkTemplateArguments();
      }
        
      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param field_index The index of the field in the template
      /// types specified in Type.
      /// \param type_index Is the index of the current type we are
      /// looking at. 
      /// \throws H3D::TypedFieldError
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        if( field_index == type_index ) {
          try {
            TypesType<RequiredArgType>::checkType( f );
          } catch( ... ) {
            stringstream s;
            s << "Route " << field_index << " has type " 
              << f->getTypeName() <<". Expecting "
              << RequiredArgType::classTypeName() << ".";
            throw H3D::TypedFieldError( s.str() );
          }
        } else {
          TypedFieldCheck< void,
            OptionalArgTypes >::checkFieldType( f, 
                                                field_index, 
                                                type_index + 1 );
        }
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypedFieldCheck 
    /// template contains a Types <> template class instantiation as 
    /// RequiredArgTypes template argument. 
    ///
    template< H3D_TYPES_TEMPLATE_LIST, class OptionalArgTypes >
    struct TypedFieldCheck< H3D_TYPES_CLASS, OptionalArgTypes >{
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        H3D_TYPES_CLASS::checkTemplateArguments();
        TypedFieldCheck< void, 
          OptionalArgTypes >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param field_index The index of the field in the template
      /// types specified in Type.
      /// \param type_index Is the index of the current type we are
      /// looking at. 
      /// \throws H3D::TypedFieldError
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        int nr_types = H3D_TYPES_CLASS::numberTypes();
        if( field_index < type_index + nr_types ) { 
          try {
            H3D_TYPES_CLASS::checkType( f, 
                                        field_index - type_index );
          } catch( const InvalidType &e ) {
            stringstream s;
            s << "Route " << field_index << " has type " 
              << e.value <<". Expecting "
              << e.message;
            throw H3D::TypedFieldError( s.str() );
          }
        } else {
          TypedFieldCheck< void, 
            OptionalArgTypes >::checkFieldType( f,
                                                field_index,
                                                type_index + 
                                                nr_types );
        }
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypedFieldCheck 
    /// template contains a AnyNumber <> template class instantiation as 
    /// RequiredArgTypes template argument. This is not allowed so we 
    /// throw an InvalidTemplateArguments in all functions.
    ///
    template< class Type, class OptionalArgTypes >
    struct TypedFieldCheck< AnyNumber< Type >, OptionalArgTypes >{
      /// AnyNumber <> instance not allowed as RequiredArgTypes, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        string s = "AnyNumber statements not allowed as RequiredArgTypes";
        throw InvalidTemplateArguments( s );
      }

      /// AnyNumber <> instance not allowed as RequiredArgTypes, so
      /// we throw a InvalidTemplateArguments
      /// exception.
      /// \throws InvalidTemplateArguments
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        string s = "AnyNumber statements not allowed as RequiredArgTypes";
        throw InvalidTemplateArguments( s );
      }
    };
    
    /// \internal
    /// Template specialization to handle the case when the TypedFieldCheck 
    /// template gets no types.
    ///
    template<>
    struct TypedFieldCheck< void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///       
      static void checkTemplateArguments() {}

      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param field_index The index of the field in the template
      /// types specified in Type.
      /// \param type_index Is the index of the current type we are
      /// looking at. 
      /// \throws H3D::TypedFieldError
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        stringstream s;
        s << "Too many routes. Expecing a maximum of " 
          << type_index  << " routes.";
        throw TypedFieldError( s.str() );
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypedFieldCheck 
    /// template contains void as RequiredArgTypes template argument and
    /// has OptionalArgTypes.
    ///
    template< class OptionalArgType >
    struct TypedFieldCheck< void, OptionalArgType > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< OptionalArgType >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param field_index The index of the field in the template
      /// types specified in Type.
      /// \param type_index Is the index of the current type we are
      /// looking at. 
      /// \throws H3D::TypedFieldError
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        if( field_index == type_index ) {
          try {
            TypesType<OptionalArgType>::checkType( f );
          } catch( ... ) {
            stringstream s;
            s << "Route " << field_index << " has type " 
              << f->getTypeName() <<". Expecting "
              << OptionalArgType::classTypeName() 
              << ".";
            throw H3D::TypedFieldError( s.str() );
          }
        } else {
          stringstream s;
          s << "Too many routes. Expecing a maximum of " 
            << type_index << " routes.";
          throw H3D::TypedFieldError( s.str() );
        }
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypedFieldCheck 
    /// template contains void as RequiredArgTypes template argument and
    /// a Types <> template class instance a OptionalArgTypes.
    ///
    template< H3D_TYPES_TEMPLATE_LIST >
    struct TypedFieldCheck< void, H3D_TYPES_CLASS >{
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        H3D_TYPES_CLASS::checkTemplateArguments();
      }

      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param field_index The index of the field in the template
      /// types specified in Type.
      /// \param type_index Is the index of the current type we are
      /// looking at. 
      /// \throws H3D::TypedFieldError
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        int nr_types = H3D_TYPES_CLASS::numberTypes();
        if( field_index < type_index + nr_types ) { 
          H3D_TYPES_CLASS::checkType( f, 
                                      field_index - type_index );
        } else {
          stringstream s;
          s << "Too many routes. Expecting a maximum of " 
            << type_index << " routes.";
          throw H3D::TypedFieldError( s.str() );
        }
      }
    };

    /// \internal
    /// Template specialization to handle the case when the TypedFieldCheck 
    /// template contains void as RequiredArgTypes template argument and
    /// a AnyNumber <> template class instance a OptionalArgTypes.
    ///
    template< class Type >
    struct TypedFieldCheck< void, AnyNumber< Type > >{
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyNumber<Type>::checkTemplateArguments();
      }
        
      /// Check that a field is of the correct type.
      /// \param f The field to check the type of
      /// \param field_index The index of the field in the template
      /// types specified in Type.
      /// \param type_index Is the index of the current type we are
      /// looking at. 
      /// \throws H3D::TypedFieldError
      ///
      static void checkFieldType( Field *f, 
                                  int field_index, 
                                  int type_index ) {
        try {
          AnyNumber< Type >::checkType( f, field_index - type_index );
        } catch( const InvalidType &e ) {
          stringstream s;
          s << "Route " << field_index << " has type " 
            << e.value <<". Expecting "
            << e.message << ".";
          throw H3D::TypedFieldError( s.str() );
        }
      }
    };
  }

}

#endif
