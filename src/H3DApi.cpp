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
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DApi.h>
#ifdef HAVE_FREEIMAGE
#include <FreeImage.h>
#endif
#ifdef HAVE_XERCES
#include <xercesc/util/PlatformUtils.hpp>
#endif

#include <H3DUtil/Exception.h>
#ifdef LINUX
#include <fontconfig/fontconfig.h>
#endif

#ifdef HAVE_LIBCURL
#include <curl/curl.h>
#include <H3D/LibCurlResolver.h>
#endif

#include <H3D/ResourceResolver.h>

#ifdef HAVE_FREEIMAGE
#if defined(_MSC_VER) || defined(__BORLANDC__)
#ifdef LINK_STATIC_EXTERNALS
#pragma comment( lib, "FreeImage_static.lib" )
#else
#pragma comment( lib, "FreeImage.lib" )
#endif
#endif
#endif

using namespace H3D;

/// Initialize H3D API(only needed if using H3D API as a static library). 
void H3D::initializeH3D() {

#ifdef HAVE_FREEIMAGE
    FreeImage_Initialise();
#endif
#ifdef LINUX
    FcInit();
#endif 
#ifdef HAVE_XERCES
  XERCES_CPP_NAMESPACE_USE
  XMLPlatformUtils::Initialize();
#endif
  /*
    string urn_config_file = "index.urn";
    char *buffer = getenv( "H3D_URN_CONFIG_FILE" );
    if( buffer ) urn_config_file = buffer;
    else if( buffer = getenv( "H3D_ROOT" ) ) {
    urn_config_file = buffer;
    urn_config_file += "/index.urn";
    }
    ResourceResolver::setURNResolver( new URNResolver( urn_config_file ) );
  */
#ifdef HAVE_LIBCURL
  curl_global_init( CURL_GLOBAL_ALL );
  ResourceResolver::addResolver( new LibCurlResolver );
#endif
}

  /// Deinitialize H3D API(only needed if using H3D API as a static library). 
void H3D::deinitializeH3D() {
#ifdef HAVE_FREEIMAGE
  FreeImage_DeInitialise();
#endif
#ifdef HAVE_LIBCURL
  curl_global_cleanup();
#endif

#ifdef HAVE_XERCES
  XERCES_CPP_NAMESPACE_USE
  XMLPlatformUtils::Terminate();
#endif
}


#ifdef WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                       ) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH: {
    initializeH3D();
    break;
  }
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    //deinitializeH3D();
    break;
  }
  return TRUE;
}
#else 
#ifdef __cplusplus
extern "C" {
#endif
  void __attribute__((constructor)) initAPI( void ) {
    initializeH3D();
  }
  void __attribute__((destructor)) finiAPI( void ) {
    deinitializeH3D();
  }
#ifdef __cplusplus
}
#endif

#endif

