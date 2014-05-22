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
/// \file X3DUrlObject.h
/// \brief Header file for X3DUrlObject, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DURLOBJECT_H__
#define __X3DURLOBJECT_H__

#include <H3D/Node.h>
#include <H3D/FieldTemplates.h>
#include <H3D/MFString.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \class X3DUrlObject
  /// \brief This abstract interface is inherited by all nodes that 
  /// contain data located on the World Wide Web, such as AudioClip, 
  /// ImageTexture and Inline. 
  ///
  /// All url fields can hold multiple string values. The strings in these
  /// fields indicate multiple locations to search for data in decreasing
  /// order of preference. If the browser cannot locate or interpret the
  /// data specified by the first location, it shall try the second and
  /// subsequent locations in order until a location containing
  /// interpretable data is encountered. X3D browsers only have to interpret
  /// a single string. If no interpretable locations are found, the node
  /// type defines the resultant default behaviour. 
  class H3DAPI_API X3DUrlObject {
  public:

    /// Constructor.
    X3DUrlObject( Inst< MFString>  _url = 0 );

    virtual ~X3DUrlObject() {}

    /// The load status of the given url.
    typedef enum {
      /// The url has not been tried to load yet.
      INIT,
      /// The url has been successfully loaded
      LOADED,
      /// The url is currently loading.
      LOADING,
      /// The loading of the url failed.
      FAILED
    } LoadStatus;

    /// The loadStatus function returns the status of the loading
    /// of the current urls in order to be used with the LoadSensor
    /// node.
    virtual LoadStatus loadStatus() { 
       if( url_used != "" ) return X3DUrlObject::LOADED;
       else return X3DUrlObject::FAILED;
    }

    /// The loadProgress() functions returns a value between 0 and 1 
    /// indicating how much of the current url has been loaded. 1 means
    /// that the file has been sucessfully loaded. The only requirement of
    /// this function is that if the url has been successfully loaded the
    /// function returns 1.
    virtual H3DFloat loadProgress() {
      if( loadStatus() == LOADED ) return 1;
      else return 0;
    }

    /// Set the url that is currently loaded.
    inline void setURLUsed( const string &_url_used ) {
      url_used = _url_used;
    }

    /// Get the url that is currently loaded.
    inline const string &getURLUsed() {
      return url_used;
    }

    /// Set the url base address.
    inline void setURLBase( const string &_url_base ) {
      url_base = _url_base;
    }

    /// Get the current url base address.
    inline const string &getURLBase() {
      return url_base;
    }

    /// Get the url as a local file. 
    string resolveURLAsFile( const string &url, 
                             bool *is_tmp_file = NULL );

    /// Get the content of the URL as a string
    string resolveURLAsString( const string &url );

    /// Remove a tmpfile with the given name.
    /// Returns true on success, or false if no such file exists
    /// or the removal failed.
    bool removeTmpFile( const string &file );
    
    /// The urls in decreasing order of preference.
    auto_ptr< MFString >  url;
  protected:
    /// Helper function for resolveURLAs*() functions
    string resolveURL ( const string& url, bool return_contents, bool *is_tmp_file = NULL ); 

    /// Add a inline prefix that this node supports.
    void addInlinePrefix( const string &s );

    /// If loadStatus() returns LOADED this string should contain
    /// the url that is loaded. All subclasses of X3DUrlObject should
    /// set this when successfully loaded a file.
    string url_used;

    /// The base URL for urls in this url object. All relative paths
    /// will start from this. Will be set to ResourceResolver::getBaseURL()
    /// when created.
    string url_base;

    /// A list of inline prefixes that are supported by this url object.
    /// E.g. "glsl", "python", etc
    list< string > supported_inline_prefixes;
  };
}

#endif
