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
/// \file LibCurlResolver.cpp
/// \brief cpp file for LibCurlResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#include "LibCurlResolver.h"

#ifdef HAVE_LIBCURL

using namespace H3D;

string LibCurlResolver::resolveURLAsTmpFile( const string &url ) {
  CURL *handle = curl_easy_init();
  char tmp_file[ L_tmpnam ];
  if( tmpnam( tmp_file ) ) {
    ofstream os( tmp_file, ios::binary );
    curl_easy_setopt( handle, CURLOPT_URL, url.c_str() );
    curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, &writeToStream );
    curl_easy_setopt( handle, CURLOPT_WRITEDATA, &os );
    CURLcode return_code = curl_easy_perform( handle );
    os.close();
    if( return_code != 0 ) {
      remove( tmp_file );
      return "";
    } else {
      temp_files.push_back( tmp_file );
      return tmp_file;
    }
  }
  return "";
}

#endif // HAVE_LIBCURL
