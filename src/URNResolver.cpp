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
/// \file URNResolver.cpp
/// \brief cpp file for URNResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/URNResolver.h>
#include <H3DUtil/Console.h>

using namespace H3D;
using namespace H3DUtil;

void URNResolver::loadConfigFile( const string &config_file ) {
  ifstream is( config_file.c_str() );
  if( !is.good() ) {
    Console(4) << "Warning: Could not open file URN resolver config file \"" 
               << config_file << "\"" << endl; 
    return;
  }
  string line;
  while(!is.eof() ) {
    getline( is, line );
    if(line.empty()) continue;
    unsigned int characters_processed = 0;
    string::size_type pos = 0;
    
    // skip all whitespaces
    while( (line[pos] == ' ' || line[ pos ] == '\t') &&
           pos < line.size() ) {
      ++pos;
      ++characters_processed;
    }
    
    // empty line
    if( pos >= line.size() ) continue;
    
    string urn = line.substr( pos, 4 );
    if( !nonCaseEquals( urn, "urn:" ) ) {
      Console(3) << "Invalid urn in URN resolve rule \"" << line 
                 << "\" in file \"" << config_file << "\"" << endl;
      continue;
    } 
    
    characters_processed += 4;
    
    string name_space = "";
    pos = line.find( ':', characters_processed );
    if( pos == string::npos ) {
      Console(3) << "Invalid urn in URN resolve rule \"" << line
                 << "\" in file \"" << config_file << "\"" << endl;
      continue;
    } 
    
    name_space = line.substr( characters_processed, 
                              pos - characters_processed );
    
    characters_processed += (unsigned int) pos - (characters_processed - 1);
    
    pos = line.find( ' ', characters_processed );
    string::size_type tab_pos = line.find( '\t', characters_processed );
    
    if( pos == string::npos )
      Console(3) << "Invalid urn spec" << endl;
    
    pos = pos < tab_pos ? pos: tab_pos;
    
    string prefix = line.substr( characters_processed, 
                                 pos - characters_processed );
    characters_processed += (unsigned int) prefix.size();
    
    // skip all whitespaces
    while( (line[pos] == ' ' || line[ pos ] == '\t') &&
           pos < line.size() ) {
      ++pos;
      ++characters_processed;
    }
    
    if( pos >= line.size() ) {
      Console(3) << "Invalid URN resolve rule \"" << line
                 << "\" in file \"" << config_file << endl;
      continue;
    }
    
    string path = line.substr( characters_processed, 
                               line.size() - characters_processed );
    
    addURNResolveRule( name_space, 
                       prefix,
                       path );
    
    
  }
  is.close();
}

string URNResolver::resolveURN( const string &urn ) {
  if( !initialised && config_file != "" ) {
    loadConfigFile( config_file );
    initialised=true;
  }
  string urn_string = toLower( urn.substr( 0, 4 ) );
  if( urn_string != "urn:" ) return urn;
  string::size_type pos = urn.find( ':', 4 );
  if( pos == string::npos ) return urn;
  string normalized_urn = "urn:";
  normalized_urn = normalized_urn + toLower( urn.substr( 4, pos - 4 ) ) + 
                   urn.substr( pos, urn.size() - pos ) ; 

  for( URNmap::reverse_iterator i = urn_prefix_map.rbegin(); 
       i != urn_prefix_map.rend(); ++i ) {
    string prefix = (*i).first;
    string path = (*i).second;
    if( hasPrefix( urn, prefix ) ) {
      string resolved_urn = urn;
      resolved_urn.replace( 0, prefix.size(), path );
      return resolved_urn;
    }
  }
  return urn;
}

string URNResolver::fromURLtoURN( const string &url ) {
  if( !initialised && config_file != "" ) {
    loadConfigFile( config_file );
    initialised=true;
  }

  for( URNmap::reverse_iterator i = urn_prefix_map.rbegin(); 
       i != urn_prefix_map.rend(); ++i ) {
    if( (*i).second == url.substr( 0, (*i).second.size() ) ) {
      string urn = url;
      urn.replace( 0, (*i).second.size(), (*i).first );
      return urn;
    }
  }
  return url;
}

