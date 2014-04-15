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
/// \file URNResolver.h
/// \brief Header file for URNResolver.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __URNRESOLVER_H__
#define __URNRESOLVER_H__

#include <H3D/H3DApi.h>
#include <map>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace H3D {
  /// The URNResolver resolves URNs (Uniform Resource Name) into paths. A
  /// URN allows an abstract resolution mechanism to be invoked to locate
  /// a resource (IETF RFC 2141 http://www.ietf.org/rfc/rfc2141.txt?number=2141).
  /// This allows a resource to be located on the local machine or a platform
  /// dependent resource to be located using the URN along with 
  /// platform-specific identifiers.
  /// 
  /// A URN is specified on the following form
  /// urn:namespace:namespace_string
  /// 
  /// The namespace identifies how the namespace_string should be interpreted.
  /// 
  /// The URNResolver has a map from urn prefixes to paths that it will use
  /// to resolve the urn. Normally the rules resides in a config file that
  /// are given to the URNResolver constructor. The rules are on the form
  ///
  /// urn:web3d: c:\\h3d\\h3dapi
  /// urn:h3d:textures http://www.h3d.org/textures
  ///
  /// This means that e.g. urn:h3d:textures/concrete.jpg will be resolved
  /// to http://www.h3d.org/textures/concrete.jpg/
  class H3DAPI_API URNResolver {
  public:

    
    /// Constructor. The config_file specifies a file containing 
    /// URN lookup rules.
    URNResolver( const string &_config_file = "" ){
      config_file = _config_file;
      initialised = false;
    }

    /// Loads a config file and adds its resolution rules to the URNResolver.
    void loadConfigFile( const string &config_file );

    /// Addes a new urn resolve rule to the URN resolver.
    /// \param name_space The urn name space of the rule
    /// \param prefix The prefix of the namespace string to match
    /// \param path The path to replace the urn:name_space:prefix with
    /// if match.
    void addURNResolveRule( const string &name_space,
                            const string &prefix,
                            const string &path ) {
      string key = "urn:";
      key = key + toLower( name_space ) + ":" + prefix;
      urn_prefix_map[ key ] = path;
    }


    /// Given a URN name the resolved path is returned. If no resolution
    /// could be found using the rules, urn is returned.
    string resolveURN( const string &urn );

    /// Given a URL the corresponding path containing URN is returned.
    /// \param url The url input.
    /// \returns The transformed urn. If no match is found the
    /// inputed url is returned.
    string fromURLtoURN( const string &url );

  protected:

    static const unsigned int MAX_LINE_SIZE = 1024;
    bool nonCaseEquals( const string &s1, 
                        const string &s2 ) {
      if( s1.size() != s2.size() ) return false;
      
      for( unsigned int i = 0; i < s1.size(); ++i ) {
        if( toupper( s1[i]) != toupper( s2[i] ) )
          return false;
      }
      return true;
    }

    bool hasPrefix( const string &s,
                    const string &prefix ) {
      if( prefix.size() > s.size() ) return false;
      for( unsigned int i = 0; i < prefix.size(); ++i )
        if( s[i] != prefix[i] ) return false;
      return true;
    }

    string toLower( const string &s ) {
      string res = s;
      for( unsigned int i = 0; i < s.size(); ++i )
        res[i] = tolower( s[i] );
      return res;
    }

    struct lessthan_string {
      bool operator()(const string & s1, const string & s2) const {
        if( s1.size() == s2.size() )
          return s1 < s2;
        else
          return s1.size() < s2.size();
      }
    };

    typedef map< string, string, lessthan_string > URNmap;
    URNmap urn_prefix_map;
    string config_file;
    bool initialised;
  };
}

#endif
