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
/// \file LibCurlResolver.h
/// \brief Header file for LibCurlResolver.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LIBCURLRESOLVER_H__
#define __LIBCURLRESOLVER_H__

#include <H3D/H3DApi.h>

#ifdef HAVE_LIBCURL
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <curl/curl.h>
#include <H3D/ResourceResolver.h>

using namespace std;

namespace H3D {

  /// The LibCurlResolver uses libcurl to resolve URLs using e.g. the
  /// http or ftp protocols.
  class H3DAPI_API LibCurlResolver : public ResourceResolver {
  public:

    /// Constructor.       
    LibCurlResolver() {}

    /// Resolve a URL and download it to a tmp-file. The tmp-file
    /// name is returned. If the URL could not be downloaded "" is
    /// returned.
    virtual string resolveURLAsTmpFile( const string &url );

  protected:
    static size_t writeToStream( void *ptr, size_t size, 
                                 size_t nmemb, void *stream) {
      ostream *os = static_cast< ostream * >( stream );
      os->write( (char *)ptr, size * nmemb );
      return size * nmemb;
    }

  };
}

#endif
#endif // HAVE_LIBCURL
