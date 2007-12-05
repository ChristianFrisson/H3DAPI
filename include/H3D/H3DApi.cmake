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
/// \file H3DApi.h
/// \brief Base header file that handles all configuration related settings
///
//////////////////////////////////////////////////////////////////////////////

/// \mainpage
/// Copyright 2004, <a href="http://www.sensegraphics.com">SenseGraphics AB</a>

#ifndef __H3DApi_H__
#define __H3DApi_H__

/// Undef if you do not have xerces-c(http://xerces.apache.org/xerces-c/) 
/// installed. Required for parsing xml based x3d files.
#cmakedefine HAVE_XERCES

/// Undef if you do not have zlib(http://www.zlib.net/) installed. 
/// Required for support for parsing zipped x3d files.
#cmakedefine HAVE_ZLIB

/// Undef if you do not have OpenAL(www.openal.org) installed. Sounds will
/// then be disabled.
#cmakedefine HAVE_OPENAL

/// Undef if you do not have libvorbis(www.vorbis.com) installed. Ogg Vorbis
/// sound files will then not be supported.
#cmakedefine HAVE_LIBVORBIS

///Undef if you do not have libaudiofile(www.68k.org/~michael/audiofile)
/// installed. Most sound files will then not be supported.
#cmakedefine HAVE_LIBAUDIOFILE

/// Undef if you do not have the NVIDIA CG Toolkit
/// (developer.nvidia.com/object/cg_toolkit.html) installed. 
/// CG shader nodes will then not be supported.
#cmakedefine HAVE_CG

/// Undef if you do not have FTGL(homepages.paradise.net.nz/henryj/code)
/// installed. Text nodes will then not be supported.
#cmakedefine HAVE_FTGL

/// Undef if you do not have FreeType(www.freetype.org)
/// installed. Text nodes will then not be supported.
#cmakedefine HAVE_FREETYPE

/// Undef if you do not have 3dxware(www.3dconnexion.com) installed.
/// SpaceWareSensor node will then not be supported.
#ifdef WIN32
#define HAVE_3DXWARE
#endif

/// Undef if you do not have Python(www.python.org) installed.
/// PythonScript node will then not be supported.
#cmakedefine HAVE_PYTHON

/// Undef if you do not have FreeImage(freeimage.sourceforge.net) installed.
/// ImageTexture nodes will not be able to read image files.
#cmakedefine HAVE_FREEIMAGE

/// Undef if you do not have libcurl(http://sourceforge.net/projects/curl/)
/// installed. URLs using protocols like http and ftp will then not be 
/// supported. Only local filenames can be used. 
#cmakedefine HAVE_LIBCURL

/// Undef if you do not have DirectShow available. Needed for the 
/// DirectShowDecoder node.
#ifdef WIN32
#define HAVE_DSHOW
#endif

#define XML_USE_WIN32_TRANSCODER
#define XML_USE_INMEM_MESSAGELOADER
#define XML_USE_NETACCESSOR_WINSOCK

//#define LINK_STATIC_EXTERNALS

#ifdef LINK_STATIC_EXTERNALS
// define this if you are linking Xerces as a static library
#define XML_LIBRARY
// Define this if you are linking Freeimage as a static library
#define FREEIMAGE_LIB
#define FTGL_LIBRARY_STATIC
#define GLEW_STATIC
#define CURL_STATICLIB
#define FREEGLUT_STATIC
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

#if defined(__APPLE__) && defined(__MACH__)
#define MACOSX
#define H3DAPI_API
#define HAVE_SYS_TIME_H
#endif

#if defined(__linux)
#define LINUX
#define H3DAPI_API 
#define HAVE_SYS_TIME_H
#endif

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
}

#endif


