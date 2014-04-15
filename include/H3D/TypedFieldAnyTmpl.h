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
/// \file TypedFieldAnyTmpl.h
/// \brief This file has been automatically generated from the
/// Util/TypedFieldAnyTmpl.py. It contains the definition of the Any 
/// template class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TYPESFIELDANYTEMPL_H__
#define __TYPESFIELDANYTEMPL_H__

#include <typeinfo>

#define H3D_ANY_TEMPLATE_LIST class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36, class Type37, class Type38, class Type39

#define H3D_ANY_CLASS Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, Type38, Type39 >

#define H3D_ANY_TEMPLATE_SPEC class Type0 = void, class Type1 = void, class Type2 = void, class Type3 = void, class Type4 = void, class Type5 = void, class Type6 = void, class Type7 = void, class Type8 = void, class Type9 = void, class Type10 = void, class Type11 = void, class Type12 = void, class Type13 = void, class Type14 = void, class Type15 = void, class Type16 = void, class Type17 = void, class Type18 = void, class Type19 = void, class Type20 = void, class Type21 = void, class Type22 = void, class Type23 = void, class Type24 = void, class Type25 = void, class Type26 = void, class Type27 = void, class Type28 = void, class Type29 = void, class Type30 = void, class Type31 = void, class Type32 = void, class Type33 = void, class Type34 = void, class Type35 = void, class Type36 = void, class Type37 = void, class Type38 = void, class Type39 = void

namespace H3D {
  namespace TypedFieldTypeCheck {
    template< class Type > struct AnyType;
        
    /// 
    /// An exception thrown when a field is of the wrong type
    /// when it is checked. Internal to the H3D::TypedFieldCheck classes.
    /// 
    H3D_VALUE_EXCEPTION( string, InvalidType );

    /// The Any template can be used in the RequiredArgTypes or 
    /// OptionalArgTypes arguments of a TypedField template instantiation.
    /// It defines that a routed Field can be any of the types specified 
    /// in the template.
    /// An Any template can contain Field types,
    /// No AnyNumber <> template and Types <> template instantiations are
    /// allowed.
    ///
    /// \par Examples:
    /// Any< SFInt32, MFInt32 > means that the route should be either a 
    /// SFInt32 or a MFInt32
    /// \par
    /// Any< SFInt32, MFInt32, SFFloat, MFFloat > means that the route
    /// should have any of the Field types listed
    ///   
    template< H3D_ANY_TEMPLATE_SPEC > 
    struct Any {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
        AnyType< Type34 >::checkTemplateArguments();
        AnyType< Type35 >::checkTemplateArguments();
        AnyType< Type36 >::checkTemplateArguments();
        AnyType< Type37 >::checkTemplateArguments();
        AnyType< Type38 >::checkTemplateArguments();
        AnyType< Type39 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f ) &&
           !AnyType< Type34 >::checkType( f ) &&
           !AnyType< Type35 >::checkType( f ) &&
           !AnyType< Type36 >::checkType( f ) &&
           !AnyType< Type37 >::checkType( f ) &&
           !AnyType< Type38 >::checkType( f ) &&
           !AnyType< Type39 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, Type38, Type39> ).name() ); 
        }
      }
    };
    template< class Type0 >
    struct Any< Type0, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1 >
    struct Any< Type0, Type1, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2 >
    struct Any< Type0, Type1, Type2, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3 >
    struct Any< Type0, Type1, Type2, Type3, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4 >
    struct Any< Type0, Type1, Type2, Type3, Type4, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
        AnyType< Type34 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f ) &&
           !AnyType< Type34 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
        AnyType< Type34 >::checkTemplateArguments();
        AnyType< Type35 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f ) &&
           !AnyType< Type34 >::checkType( f ) &&
           !AnyType< Type35 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
        AnyType< Type34 >::checkTemplateArguments();
        AnyType< Type35 >::checkTemplateArguments();
        AnyType< Type36 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f ) &&
           !AnyType< Type34 >::checkType( f ) &&
           !AnyType< Type35 >::checkType( f ) &&
           !AnyType< Type36 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36, class Type37 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
        AnyType< Type34 >::checkTemplateArguments();
        AnyType< Type35 >::checkTemplateArguments();
        AnyType< Type36 >::checkTemplateArguments();
        AnyType< Type37 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f ) &&
           !AnyType< Type34 >::checkType( f ) &&
           !AnyType< Type35 >::checkType( f ) &&
           !AnyType< Type36 >::checkType( f ) &&
           !AnyType< Type37 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37> ).name() ); 
        }
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36, class Type37, class Type38 >
    struct Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, Type38, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        AnyType< Type0 >::checkTemplateArguments();
        AnyType< Type1 >::checkTemplateArguments();
        AnyType< Type2 >::checkTemplateArguments();
        AnyType< Type3 >::checkTemplateArguments();
        AnyType< Type4 >::checkTemplateArguments();
        AnyType< Type5 >::checkTemplateArguments();
        AnyType< Type6 >::checkTemplateArguments();
        AnyType< Type7 >::checkTemplateArguments();
        AnyType< Type8 >::checkTemplateArguments();
        AnyType< Type9 >::checkTemplateArguments();
        AnyType< Type10 >::checkTemplateArguments();
        AnyType< Type11 >::checkTemplateArguments();
        AnyType< Type12 >::checkTemplateArguments();
        AnyType< Type13 >::checkTemplateArguments();
        AnyType< Type14 >::checkTemplateArguments();
        AnyType< Type15 >::checkTemplateArguments();
        AnyType< Type16 >::checkTemplateArguments();
        AnyType< Type17 >::checkTemplateArguments();
        AnyType< Type18 >::checkTemplateArguments();
        AnyType< Type19 >::checkTemplateArguments();
        AnyType< Type20 >::checkTemplateArguments();
        AnyType< Type21 >::checkTemplateArguments();
        AnyType< Type22 >::checkTemplateArguments();
        AnyType< Type23 >::checkTemplateArguments();
        AnyType< Type24 >::checkTemplateArguments();
        AnyType< Type25 >::checkTemplateArguments();
        AnyType< Type26 >::checkTemplateArguments();
        AnyType< Type27 >::checkTemplateArguments();
        AnyType< Type28 >::checkTemplateArguments();
        AnyType< Type29 >::checkTemplateArguments();
        AnyType< Type30 >::checkTemplateArguments();
        AnyType< Type31 >::checkTemplateArguments();
        AnyType< Type32 >::checkTemplateArguments();
        AnyType< Type33 >::checkTemplateArguments();
        AnyType< Type34 >::checkTemplateArguments();
        AnyType< Type35 >::checkTemplateArguments();
        AnyType< Type36 >::checkTemplateArguments();
        AnyType< Type37 >::checkTemplateArguments();
        AnyType< Type38 >::checkTemplateArguments();
      }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f ) { 
        if( 
           !AnyType< Type0 >::checkType( f ) &&
           !AnyType< Type1 >::checkType( f ) &&
           !AnyType< Type2 >::checkType( f ) &&
           !AnyType< Type3 >::checkType( f ) &&
           !AnyType< Type4 >::checkType( f ) &&
           !AnyType< Type5 >::checkType( f ) &&
           !AnyType< Type6 >::checkType( f ) &&
           !AnyType< Type7 >::checkType( f ) &&
           !AnyType< Type8 >::checkType( f ) &&
           !AnyType< Type9 >::checkType( f ) &&
           !AnyType< Type10 >::checkType( f ) &&
           !AnyType< Type11 >::checkType( f ) &&
           !AnyType< Type12 >::checkType( f ) &&
           !AnyType< Type13 >::checkType( f ) &&
           !AnyType< Type14 >::checkType( f ) &&
           !AnyType< Type15 >::checkType( f ) &&
           !AnyType< Type16 >::checkType( f ) &&
           !AnyType< Type17 >::checkType( f ) &&
           !AnyType< Type18 >::checkType( f ) &&
           !AnyType< Type19 >::checkType( f ) &&
           !AnyType< Type20 >::checkType( f ) &&
           !AnyType< Type21 >::checkType( f ) &&
           !AnyType< Type22 >::checkType( f ) &&
           !AnyType< Type23 >::checkType( f ) &&
           !AnyType< Type24 >::checkType( f ) &&
           !AnyType< Type25 >::checkType( f ) &&
           !AnyType< Type26 >::checkType( f ) &&
           !AnyType< Type27 >::checkType( f ) &&
           !AnyType< Type28 >::checkType( f ) &&
           !AnyType< Type29 >::checkType( f ) &&
           !AnyType< Type30 >::checkType( f ) &&
           !AnyType< Type31 >::checkType( f ) &&
           !AnyType< Type32 >::checkType( f ) &&
           !AnyType< Type33 >::checkType( f ) &&
           !AnyType< Type34 >::checkType( f ) &&
           !AnyType< Type35 >::checkType( f ) &&
           !AnyType< Type36 >::checkType( f ) &&
           !AnyType< Type37 >::checkType( f ) &&
           !AnyType< Type38 >::checkType( f )        ) { 
          throw InvalidType( typeid( f ).name(),
                             typeid( Any< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, Type38> ).name() ); 
        }
      }
    };

  }
}

#endif

