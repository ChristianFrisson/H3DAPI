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
/// \file SFVec4f.h
/// \brief Contains the SFVec4f field class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SFVEC4F_H__
#define __SFVEC4F_H__

#include <H3D/SField.h>

namespace H3D {

  // template specializations in order to get nicer type printouts.
  template<>
  inline string SField< Vec4f >::classTypeName() { return "SFVec4f"; }

  /// \ingroup FieldTypes
  /// The SFVec4f field contains a Vec4f.
  class H3DAPI_API SFVec4f: public SField< Vec4f > {
  public:
    SFVec4f() {}
    SFVec4f( const Vec4f &_value ): SField< Vec4f >( _value ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::SFVEC4F; }
  };
}

#endif

