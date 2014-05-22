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
//
/// \file H3DApi.h
/// \brief Base header file that handles all configuration related settings
///
//////////////////////////////////////////////////////////////////////////////

/// \mainpage H3D API Documentation
/// Copyright 2004 - 2014, <a href="http://www.sensegraphics.com">SenseGraphics AB</a>

#ifndef __H3DApi_H__
#define __H3DApi_H__

#include <HAPI/HAPI.h>

#if defined(__APPLE__) && defined(__MACH__)
#define MACOSX
#define H3D_OSX
#define H3DAPI_API
#define HAVE_SYS_TIME_H
#endif

#if defined(__linux)
#define LINUX
#define H3D_LINUX
#define H3DAPI_API 
#define HAVE_SYS_TIME_H
#endif

#if( defined( _WIN64 ) || defined(WIN64) )
// set when on 64 bit Windows
#define H3D_WIN64
#define H3D_ARCH64
#elif( defined( _WIN32 ) || defined(WIN32) )
// set when on 32 bit Windows
#define H3D_WIN32
#define H3D_ARCH32
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define H3D_ARCH64
#else
#define H3D_ARCH32
#endif
#endif

#if( defined( H3D_WIN32 ) || defined( H3D_WIN64 ) )
// set when on 32 or 64 bit Windows
#define H3D_WINDOWS
#endif

/// Undef if you do not have xerces-c(http://xerces.apache.org/xerces-c/) 
/// installed. Required for parsing xml based x3d files.
#cmakedefine HAVE_XERCES

/// Undef if you do not have OpenAL(www.openal.org) installed. Sounds will
/// then be disabled.
#cmakedefine HAVE_OPENAL

/// Undef if you do not have libvorbis(www.vorbis.com) installed. Ogg Vorbis
/// sound files will then not be supported.
#cmakedefine HAVE_LIBVORBIS

/// Undef if you do not have libaudiofile(www.68k.org/~michael/audiofile)
/// installed. Most sound files will then not be supported.
#cmakedefine HAVE_LIBAUDIOFILE

/// Undef if you do not have the NVIDIA CG Toolkit
/// (developer.nvidia.com/object/cg_toolkit.html) installed. 
/// CG shader nodes will then not be supported.
#cmakedefine HAVE_CG

/// Undef if you do not have FTGL(homepages.paradise.net.nz/henryj/code)
/// installed. Text nodes will then not be supported.
#cmakedefine HAVE_FTGL

/// Declare this if the header file for ftgl is FTGL/FTGL.h instead
/// of FTGL/ftgl.h. Only used if HAVE_FTGL is defined.
#cmakedefine FTGL_INCLUDE_IS_UPPER

/// Undef if you do not have FreeType(www.freetype.org)
/// installed. Text nodes will then not be supported.
#cmakedefine HAVE_FREETYPE

/// Undef if you do not have FontConfig. 
#cmakedefine HAVE_FONTCONFIG

#ifdef WIN32
/// Undef if you do not have 3dxware(www.3dconnexion.com) installed.
/// SpaceWareSensor node will then not be supported.
#cmakedefine HAVE_3DXWARE
#endif

/// Undef if you do not have Python(www.python.org) installed.
/// PythonScript node will then not be supported.
#cmakedefine HAVE_PYTHON

/// Undef if you do not have libcurl(http://sourceforge.net/projects/curl/)
/// installed. URLs using protocols like http and ftp will then not be 
/// supported. Only local filenames can be used. 
#cmakedefine HAVE_LIBCURL

/// Undef if you do not have SpiderMonkey Javascript engine
/// (http://www.mozilla.org/js/spidermonkey/)
/// installed. Ecmascript support in Script node will not be available
/// without it
#cmakedefine HAVE_SPIDERMONKEY

#ifdef H3D_WINDOWS
/// Undef if you do not have DirectShow available. Needed for the 
/// DirectShowDecoder node.
#cmakedefine HAVE_DSHOW
#endif

/// Undef if you do not have Sixense SDK available. Needed for the 
/// RazerHydraSensor node.
#cmakedefine HAVE_SIXENSE

/// Undef if you do not have FFmpeg available. Needed for the 
/// FFmpegDecoder node.
#cmakedefine HAVE_FFMPEG

/// Undef if you do not Virtual Hand SDK available. Needed for the
/// CyberGloveSensor and HumanHand nodes.
#cmakedefine HAVE_VIRTUAL_HAND_SDK

/// Undef if you do not have Glut or FreeGlut.
#cmakedefine HAVE_GLUT

#define XML_USE_WIN32_TRANSCODER
#define XML_USE_INMEM_MESSAGELOADER
#define XML_USE_NETACCESSOR_WINSOCK

#ifdef WIN32
// define this if you are linking Xerces as a static library
#cmakedefine XML_LIBRARY
// define this if you are linking ftgl as a static library
#cmakedefine FTGL_LIBRARY_STATIC
// define this if you are linking GLEW as a static library
#cmakedefine GLEW_STATIC
// define this if you are linking curl as a static library
#cmakedefine CURL_STATICLIB
// define this if you are linking FreeGlut as a static library
#cmakedefine FREEGLUT_STATIC
#endif

// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler. All files within this DLL
// are compiled with the H3DAPI_EXPORTS symbol defined on the command
// line. this symbol should not be defined on any project that uses
// this DLL. This way any other project whose source files include
// this file see H3DAPI_API functions as being imported from a DLL,
// whereas this DLL sees symbols defined with this macro as being
// exported.
#if defined(WIN32) || defined(__WIN32__)
#include <windows.h>

#ifdef H3DAPI_LIB
#define H3DAPI_API
#else

#ifdef H3DAPI_EXPORTS
#define H3DAPI_API __declspec(dllexport)
#else
#define H3DAPI_API __declspec(dllimport)
#endif
#if defined(_MSC_VER) || defined(__BORLANDC__)
// disable dll-interface warnings for stl-exports
#pragma warning( disable: 4251 )
//#pragma warning( disable: 4275 )

#endif

#endif
#endif

// Borland uses strnicmp.
#ifdef __BORLANDC__
#define _strnicmp strnicmp
#define __timeb64 timeb
#define _ftime64 ftime
#endif

#define H3DAPI_MAJOR_VERSION ${H3DAPI_MAJOR_VERSION}
#define H3DAPI_MINOR_VERSION ${H3DAPI_MINOR_VERSION}
#define H3DAPI_BUILD_VERSION ${H3DAPI_BUILD_VERSION}

/// H3D API namespace
namespace H3D {
  /// Initialize H3D API(only needed if using H3D API as a static library). 
  void initializeH3D();

  /// Deinitialize H3D API(only needed if using H3D API as a static library). 
  void deinitializeH3D();

  /// Function for determining if the machine we are running on is uses
  /// little endian byte order or not.
  inline bool isLittleEndian() {
    union probe{ 
      unsigned int num;
      unsigned char bytes[sizeof(unsigned int)];
    };
    //initialize first member of p with unsigned 1
    probe p = { 1U };
    // in a big endian architecture, p.bytes[0] equals 0  
    bool little_endian = (p.bytes[0] == 1U); 
    return little_endian;
  }


  /// Will return the version of H3DAPI as a double on the form
  /// H3DAPI_MAJOR_VERSION.H3DAPI_MINOR_VERSION
  double H3DAPI_API getH3DApiVersion();

}

#endif



