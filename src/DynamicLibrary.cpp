
//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file DynamicLibrary.cpp
/// \brief Header file for DynamicLibrary, which is used for loading
/// and unloading dynamic libraries at runtime.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <DynamicLibrary.h>
#ifndef WIN32
#include <dlfcn.h>
#endif

using namespace std;
using namespace H3D;

string DynamicLibrary::last_error = "";

DynamicLibrary::LIBHANDLE DynamicLibrary::load( const string &lib_name ) {
#ifdef WIN32
  // If already loaded just return the handle to the loaded library.
  LIBHANDLE handle = GetModuleHandle( lib_name.c_str() );
  if( handle ) return handle;

  handle = LoadLibrary( lib_name.c_str() );
  if( !handle ) {
    TCHAR buffer[80]; 
    DWORD dw = GetLastError(); 
    FormatMessage(
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  dw,
                  0,
                  buffer,
                  80, NULL );
    last_error = buffer;
  }
  return handle;
#else
  LIBHANDLE handle = dlopen( lib_name.c_str(), RTLD_LAZY | RTLD_GLOBAL );
  if( !handle ) 
    last_error = dlerror();
  
  return handle;
#endif
}

int DynamicLibrary::close( LIBHANDLE handle ) {
#ifdef WIN32
  if( !FreeLibrary( handle ) ) {
    TCHAR buffer[80]; 
    DWORD dw = GetLastError(); 
    FormatMessage(
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  dw,
                  0,
                  buffer,
                  80, NULL );
    last_error = buffer;
    return 1;
  }
  return 0;
#else
  if( dlclose( handle ) ) {
    last_error = dlerror();
    return 1;
  }
  return 0;
#endif
}

string DynamicLibrary::getLastError() {
  return last_error;
}

DynamicLibrary::SYMBOLPTR DynamicLibrary::getSymbolAddress( 
                                            LIBHANDLE handle, 
                                            const std::string &symbol_name ) {
#ifdef WIN32
  SYMBOLPTR ptr = GetProcAddress( handle, symbol_name.c_str() );
  if( !ptr ) {
    TCHAR buffer[80]; 
    DWORD dw = GetLastError(); 
    FormatMessage(
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  dw,
                  0,
                  buffer,
                  80, NULL );
    last_error = buffer;
  }
  return ptr;
  
#else
  SYMBOLPTR ptr = dlsym( handle, symbol_name.c_str() );
  if( !ptr ) {
    last_error = dlerror();
  }
  return ptr;
#endif
}
