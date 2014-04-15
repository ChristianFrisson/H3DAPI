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
/// \file TypedFieldTypesTmpl.h
/// \brief This file has been automatically generated from the
/// Util/TypedFieldTypesTmpl.py. It contains the definition of the Types 
/// template class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TYPESFIELDTYPESTEMPL_H__
#define __TYPESFIELDTYPESTEMPL_H__

#define H3D_TYPES_TEMPLATE_LIST class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36, class Type37, class Type38, class Type39

#define H3D_TYPES_CLASS Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, Type38, Type39 >

#define H3D_TYPES_TEMPLATE_SPEC class Type0 = void, class Type1 = void, class Type2 = void, class Type3 = void, class Type4 = void, class Type5 = void, class Type6 = void, class Type7 = void, class Type8 = void, class Type9 = void, class Type10 = void, class Type11 = void, class Type12 = void, class Type13 = void, class Type14 = void, class Type15 = void, class Type16 = void, class Type17 = void, class Type18 = void, class Type19 = void, class Type20 = void, class Type21 = void, class Type22 = void, class Type23 = void, class Type24 = void, class Type25 = void, class Type26 = void, class Type27 = void, class Type28 = void, class Type29 = void, class Type30 = void, class Type31 = void, class Type32 = void, class Type33 = void, class Type34 = void, class Type35 = void, class Type36 = void, class Type37 = void, class Type38 = void, class Type39 = void

namespace H3D {
  namespace TypedFieldTypeCheck {
    template< class Type > struct TypesType;

    /// The Types template can be used in the RequiredArgTypes or 
    /// OptionalArgTypes arguments of a TypedField template instantiation.
    /// It defines a list of types and the types of the fields routed
    /// must follow that order.
    /// A Types template can contain Field types and Any <> template
    /// instantiations. No AnyNumber <> template instantiations are
    /// allowed.
    ///
    /// \par Examples:
    /// Types< SFInt32, MFInt32 > means that the first routed field should 
    /// be SFInt32 and the second a MFInt32
    /// \par
    /// Types< Any< SFInt32, MFInt32 >, SFFloat > means that the first
    /// routed field should be either of type SFInt32 or MFInt32. The 
    /// second routed field should be SFFloat.
    ///   
    template< H3D_TYPES_TEMPLATE_SPEC > 
    struct Types {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
        TypesType< Type34 >::checkTemplateArguments();
        TypesType< Type35 >::checkTemplateArguments();
        TypesType< Type36 >::checkTemplateArguments();
        TypesType< Type37 >::checkTemplateArguments();
        TypesType< Type38 >::checkTemplateArguments();
        TypesType< Type39 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 40; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
        else if( index == 34 ) {
          TypesType< Type34 >::checkType( f );
        } 
        else if( index == 35 ) {
          TypesType< Type35 >::checkType( f );
        } 
        else if( index == 36 ) {
          TypesType< Type36 >::checkType( f );
        } 
        else if( index == 37 ) {
          TypesType< Type37 >::checkType( f );
        } 
        else if( index == 38 ) {
          TypesType< Type38 >::checkType( f );
        } 
        else if( index == 39 ) {
          TypesType< Type39 >::checkType( f );
        } 
      }
    };
    template< class Type0 >
    struct Types< Type0, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 1; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1 >
    struct Types< Type0, Type1, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 2; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2 >
    struct Types< Type0, Type1, Type2, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 3; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3 >
    struct Types< Type0, Type1, Type2, Type3, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 4; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4 >
    struct Types< Type0, Type1, Type2, Type3, Type4, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 5; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 6; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 7; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 8; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 9; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 10; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 11; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 12; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 13; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 14; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 15; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 16; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 17; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 18; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 19; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 20; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 21; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 22; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 23; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 24; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 25; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, void, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 26; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, void, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 27; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, void, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 28; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, void, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 29; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, void, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 30; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, void, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 31; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, void, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 32; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, void, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 33; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, void, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 34; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, void, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
        TypesType< Type34 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 35; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
        else if( index == 34 ) {
          TypesType< Type34 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, void, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
        TypesType< Type34 >::checkTemplateArguments();
        TypesType< Type35 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 36; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
        else if( index == 34 ) {
          TypesType< Type34 >::checkType( f );
        } 
        else if( index == 35 ) {
          TypesType< Type35 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, void, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
        TypesType< Type34 >::checkTemplateArguments();
        TypesType< Type35 >::checkTemplateArguments();
        TypesType< Type36 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 37; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
        else if( index == 34 ) {
          TypesType< Type34 >::checkType( f );
        } 
        else if( index == 35 ) {
          TypesType< Type35 >::checkType( f );
        } 
        else if( index == 36 ) {
          TypesType< Type36 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36, class Type37 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, void, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
        TypesType< Type34 >::checkTemplateArguments();
        TypesType< Type35 >::checkTemplateArguments();
        TypesType< Type36 >::checkTemplateArguments();
        TypesType< Type37 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 38; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
        else if( index == 34 ) {
          TypesType< Type34 >::checkType( f );
        } 
        else if( index == 35 ) {
          TypesType< Type35 >::checkType( f );
        } 
        else if( index == 36 ) {
          TypesType< Type36 >::checkType( f );
        } 
        else if( index == 37 ) {
          TypesType< Type37 >::checkType( f );
        } 
      }
    };
    template< class Type0, class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16, class Type17, class Type18, class Type19, class Type20, class Type21, class Type22, class Type23, class Type24, class Type25, class Type26, class Type27, class Type28, class Type29, class Type30, class Type31, class Type32, class Type33, class Type34, class Type35, class Type36, class Type37, class Type38 >
    struct Types< Type0, Type1, Type2, Type3, Type4, Type5, Type6, Type7, Type8, Type9, Type10, Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19, Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28, Type29, Type30, Type31, Type32, Type33, Type34, Type35, Type36, Type37, Type38, void > {
      /// Checks that the template arguments is of valid form. 
      /// \throws InvalidTemplateArguments
      ///
      static void checkTemplateArguments() {
        TypesType< Type0 >::checkTemplateArguments();
        TypesType< Type1 >::checkTemplateArguments();
        TypesType< Type2 >::checkTemplateArguments();
        TypesType< Type3 >::checkTemplateArguments();
        TypesType< Type4 >::checkTemplateArguments();
        TypesType< Type5 >::checkTemplateArguments();
        TypesType< Type6 >::checkTemplateArguments();
        TypesType< Type7 >::checkTemplateArguments();
        TypesType< Type8 >::checkTemplateArguments();
        TypesType< Type9 >::checkTemplateArguments();
        TypesType< Type10 >::checkTemplateArguments();
        TypesType< Type11 >::checkTemplateArguments();
        TypesType< Type12 >::checkTemplateArguments();
        TypesType< Type13 >::checkTemplateArguments();
        TypesType< Type14 >::checkTemplateArguments();
        TypesType< Type15 >::checkTemplateArguments();
        TypesType< Type16 >::checkTemplateArguments();
        TypesType< Type17 >::checkTemplateArguments();
        TypesType< Type18 >::checkTemplateArguments();
        TypesType< Type19 >::checkTemplateArguments();
        TypesType< Type20 >::checkTemplateArguments();
        TypesType< Type21 >::checkTemplateArguments();
        TypesType< Type22 >::checkTemplateArguments();
        TypesType< Type23 >::checkTemplateArguments();
        TypesType< Type24 >::checkTemplateArguments();
        TypesType< Type25 >::checkTemplateArguments();
        TypesType< Type26 >::checkTemplateArguments();
        TypesType< Type27 >::checkTemplateArguments();
        TypesType< Type28 >::checkTemplateArguments();
        TypesType< Type29 >::checkTemplateArguments();
        TypesType< Type30 >::checkTemplateArguments();
        TypesType< Type31 >::checkTemplateArguments();
        TypesType< Type32 >::checkTemplateArguments();
        TypesType< Type33 >::checkTemplateArguments();
        TypesType< Type34 >::checkTemplateArguments();
        TypesType< Type35 >::checkTemplateArguments();
        TypesType< Type36 >::checkTemplateArguments();
        TypesType< Type37 >::checkTemplateArguments();
        TypesType< Type38 >::checkTemplateArguments();
      }

      /// Returns the number of types specified as template arguments.
      /// \returns The number of types specified in the template.
      ///
      static int numberTypes() { return 39; }

      /// Check that a field is of the correct type. 
      /// \param f The field to check the type of
      /// \param index The index of the field in the template
      /// types specified in Type.
      /// \throws InvalidType
      ///
      static void checkType( Field *f, int index ) { 
        if( index == 0 ) {
          TypesType< Type0 >::checkType( f );
        } 
        else if( index == 1 ) {
          TypesType< Type1 >::checkType( f );
        } 
        else if( index == 2 ) {
          TypesType< Type2 >::checkType( f );
        } 
        else if( index == 3 ) {
          TypesType< Type3 >::checkType( f );
        } 
        else if( index == 4 ) {
          TypesType< Type4 >::checkType( f );
        } 
        else if( index == 5 ) {
          TypesType< Type5 >::checkType( f );
        } 
        else if( index == 6 ) {
          TypesType< Type6 >::checkType( f );
        } 
        else if( index == 7 ) {
          TypesType< Type7 >::checkType( f );
        } 
        else if( index == 8 ) {
          TypesType< Type8 >::checkType( f );
        } 
        else if( index == 9 ) {
          TypesType< Type9 >::checkType( f );
        } 
        else if( index == 10 ) {
          TypesType< Type10 >::checkType( f );
        } 
        else if( index == 11 ) {
          TypesType< Type11 >::checkType( f );
        } 
        else if( index == 12 ) {
          TypesType< Type12 >::checkType( f );
        } 
        else if( index == 13 ) {
          TypesType< Type13 >::checkType( f );
        } 
        else if( index == 14 ) {
          TypesType< Type14 >::checkType( f );
        } 
        else if( index == 15 ) {
          TypesType< Type15 >::checkType( f );
        } 
        else if( index == 16 ) {
          TypesType< Type16 >::checkType( f );
        } 
        else if( index == 17 ) {
          TypesType< Type17 >::checkType( f );
        } 
        else if( index == 18 ) {
          TypesType< Type18 >::checkType( f );
        } 
        else if( index == 19 ) {
          TypesType< Type19 >::checkType( f );
        } 
        else if( index == 20 ) {
          TypesType< Type20 >::checkType( f );
        } 
        else if( index == 21 ) {
          TypesType< Type21 >::checkType( f );
        } 
        else if( index == 22 ) {
          TypesType< Type22 >::checkType( f );
        } 
        else if( index == 23 ) {
          TypesType< Type23 >::checkType( f );
        } 
        else if( index == 24 ) {
          TypesType< Type24 >::checkType( f );
        } 
        else if( index == 25 ) {
          TypesType< Type25 >::checkType( f );
        } 
        else if( index == 26 ) {
          TypesType< Type26 >::checkType( f );
        } 
        else if( index == 27 ) {
          TypesType< Type27 >::checkType( f );
        } 
        else if( index == 28 ) {
          TypesType< Type28 >::checkType( f );
        } 
        else if( index == 29 ) {
          TypesType< Type29 >::checkType( f );
        } 
        else if( index == 30 ) {
          TypesType< Type30 >::checkType( f );
        } 
        else if( index == 31 ) {
          TypesType< Type31 >::checkType( f );
        } 
        else if( index == 32 ) {
          TypesType< Type32 >::checkType( f );
        } 
        else if( index == 33 ) {
          TypesType< Type33 >::checkType( f );
        } 
        else if( index == 34 ) {
          TypesType< Type34 >::checkType( f );
        } 
        else if( index == 35 ) {
          TypesType< Type35 >::checkType( f );
        } 
        else if( index == 36 ) {
          TypesType< Type36 >::checkType( f );
        } 
        else if( index == 37 ) {
          TypesType< Type37 >::checkType( f );
        } 
        else if( index == 38 ) {
          TypesType< Type38 >::checkType( f );
        } 
      }
    };

  }
}

#endif

