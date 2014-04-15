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
/// \file LibraryInfo.h
/// \brief Contains the LibraryInfo class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3D_LIBRARY_INFO_H__
#define __H3D_LIBRARY_INFO_H__

#include <H3D/H3DApi.h>

namespace H3D {
  /// Structure to contain information about a shared library of extra nodes
  /// outside. People who develop external libraries with new nodes can define
  /// a method in their dll 
  /// extern "C" MEDX3D_API LibraryInfo __cdecl getLibraryInfo();
  /// where they return information about their library. 
  struct LibraryInfo {
    static inline LibraryInfo createEmptyInfo() {
      LibraryInfo l;
      l.name[0] = '\0';
      l.version[0] = '\0';
      l.web[0] = '\0';
      l.developer[0] = '\0';
      l.developer_web[0] = '\0';
      l.info[0] = '\0';
      return l;
    }

    /// The name of the library.
    char name[256];
    /// The version of the library.
    char version[256];
    /// Web address with information about the library.
    char web[256];
    /// The developer of the library.
    char developer[256];
    /// Web address to the developer.
    char developer_web[256];
    /// Description of the library.
    char info[4096];
  };

  


}

#endif
