
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
/// \file DynamicLibrary.h
/// \brief Header file for DynamicLibrary, which is used for loading
/// and unloading dynamic libraries at runtime.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DYNAMICLIBRARY_H__
#define __DYNAMICLIBRARY_H__

#include <string>
#include "H3DApi.h"
#include "Exception.h"

namespace H3D {
  struct H3DAPI_API DynamicLibrary {
    /// Exception thrown when there is an error loading a dynamic
    /// link library.
    H3D_VALUE_EXCEPTION( std::string, CouldNotLoadDynamicLibrary );
 #ifdef __BORLANDC__
    #if __WIN32__
    typedef HMODULE LIBHANDLE;
    typedef FARPROC SYMBOLPTR;
    #else
    typedef void * LIBHANDLE;
    typedef void * SYMBOLPTR;
    #endif
#else
    #if WIN32
    typedef HMODULE LIBHANDLE;
    typedef FARPROC SYMBOLPTR;
    #else
    typedef void * LIBHANDLE;
    typedef void * SYMBOLPTR;
    #endif
#endif

    /// Load a dynamic link library in order to get access to its' symbols.
    /// \param lib_name The name of the dynamic link library to load.
    /// \returns A handle to the loaded dynamic link library if successfully
    /// loaded. NULL otherwise. If failure, use getLastError() to get a more
    /// detailed description of the error.
    static LIBHANDLE load( const std::string &lib_name );

    /// Close a dynamic link library. After a call to this function the 
    /// symbols in the dynamic link library cannot be used.
    /// \param handle The handle to the library to close.
    /// \returns 0 on success, non-zero on failure. If failure, use 
    /// getLastError() to get a more detailed description of the error.
    static int close( LIBHANDLE handle ); 

    /// Retrieves the address of an exported symbol (function or variable) 
    /// from the specified dynamic link library.
    /// \param handle A handle to the library to look in.
    /// \param symbol_name The name of the symbol to get.
    /// \returns If successful, a pointer to the specified symbol. NULL
    /// otherwise. If failure, use getLastError() to get a more detailed
    /// description of the error.
    static SYMBOLPTR getSymbolAddress( LIBHANDLE handle, 
                                       const std::string &symbol_name );

    /// Get the string describing the last error that was encountered when
    /// using the DynamicLibrary functions.
    static string getLastError();

  private:
    static string last_error;
  };
}

#endif
