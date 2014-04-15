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
/// \file LibCurlResolver.cpp
/// \brief cpp file for LibCurlResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/LibCurlResolver.h>

#ifdef HAVE_LIBCURL

using namespace H3D;

string LibCurlResolver::resolveURLAsTmpFile( const string &url ) {
  // only process files that have a protocol defined to avoid long timeouts
  // on files that will not succeed to load anyway
  if( url.find( "://" ) == string::npos ) return "";
  CURL *handle = curl_easy_init();
  
  string tmp_file = ResourceResolver::getTmpFileName();
  if( tmp_file != "" ) {
    ofstream os( tmp_file.c_str(), ios::binary );
    curl_easy_setopt( handle, CURLOPT_URL, url.c_str() );
    curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, &writeToStream );
    curl_easy_setopt( handle, CURLOPT_WRITEDATA, &os );
    CURLcode return_code = curl_easy_perform( handle );
    os.close();
    if( return_code != CURLE_OK ) {
      ResourceResolver::releaseTmpFileName( tmp_file );
      return "";
    } else {
      long response_code;
      return_code = 
        curl_easy_getinfo( handle, CURLINFO_RESPONSE_CODE, &response_code );
      if( return_code == CURLE_OK && response_code != 404 ) {
        return tmp_file;
      } else {
        ResourceResolver::releaseTmpFileName( tmp_file );
        return "";
      }
    }
  }
  return "";
}

#endif // HAVE_LIBCURL
