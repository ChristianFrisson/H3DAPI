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
/// \file X3DTypes.h
/// \brief Header file containing all X3D types enumerated.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __X3DTYPES_H__
#define __X3DTYPES_H__

namespace H3D {
  namespace X3DTypes {
    enum X3DType {
      SFFLOAT,      MFFLOAT,
      SFDOUBLE,     MFDOUBLE,
      SFTIME,       MFTIME,
      SFINT32,      MFINT32,
      SFVEC2F,      MFVEC2F,
      SFVEC2D,      MFVEC2D,
      SFVEC3F,      MFVEC3F,
      SFVEC3D,      MFVEC3D,
      SFVEC4F,      MFVEC4F,
      SFVEC4D,      MFVEC4D,
      SFBOOL,       MFBOOL,
      SFSTRING,     MFSTRING,
      SFNODE,       MFNODE,
      SFCOLOR,      MFCOLOR,
      SFCOLORRGBA,  MFCOLORRGBA,
      SFROTATION,   MFROTATION,
      SFQUATERNION,   MFQUATERNION,
      SFMATRIX3F,   MFMATRIX3F,
      SFMATRIX4F,   MFMATRIX4F,
      SFMATRIX3D,   MFMATRIX3D,
      SFMATRIX4D,   MFMATRIX4D,
      SFIMAGE,      MFIMAGE,
      UNKNOWN_X3D_TYPE 
    };
  }
}

#endif
