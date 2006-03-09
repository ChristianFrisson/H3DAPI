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
#ifndef __MFMATRIX3F_H__
#define __MFMATRIX3F_H__

#include "MField.h"

namespace H3D {

  // template specializations in order to get nicer type printouts.
  template<>
  inline string MFieldBase< Matrix3f >::classTypeName() 
  { return "MFMatrix3f"; }

  /// \ingroup FieldTypes
  /// The MFMatrix3f field contains a vector of Matrix3f instances.
  class H3DAPI_API MFMatrix3f: public MField< Matrix3f > {
  public:
    MFMatrix3f(){}
    MFMatrix3f( size_type sz ): MField< Matrix3f >( sz ){}
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::MFMATRIX3F; }
  };
}

#endif

