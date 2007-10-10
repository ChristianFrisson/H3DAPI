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
/// \file INIFile.cpp Class for parsing an .ini file
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/INIFile.h>
#include <fstream>

using namespace std;
using namespace H3D;


namespace { 

  bool nonCaseEquals( const string &s1, 
                      const string &s2 ) {
    if( s1.size() != s2.size() ) return false;
    
    for( unsigned int i = 0; i < s1.size(); i++ ) {
      if( toupper( s1[i]) != toupper( s2[i] ) )
        return false;
    }
    return true;
  }

  void cleanup( std::string& line ) {
    std::string copy = line;
    unsigned begin = 0;
    while ( begin != line.size() && isspace( line[ begin ] ) ) ++begin;
    
    bool inQuote = false;
    unsigned end;
    for ( end = begin; end != line.size(); ++end ) {
      if ( line[ end ] == '\"' ) {
        inQuote = !inQuote;
      } else if ( line[ end ] == '#' && !inQuote ) {
        break;
      } else if ( line[ end ] == '\\' ) {
        ++end; // ignore next character
        if ( end == line.size() ) {
          std::cerr << "Error parsing file: \\ character "
            "at the end of line (sorry, not supported)\n";
          break;
        }
      }
    }
    while ( end > begin && isspace( line[ end - 1 ] ) ) --end;
    line.assign( line, begin, end - begin );
    //std::cerr << "cleanup(\'" << copy << "\') = \'" << line << "\'\n";
  }

  bool parseGroupName( string line, string& group ) {
    if ( line[ 0 ] != '[' ) return false;
    if ( line[ line.size() - 1 ] != ']' ) return false;
    
    group.assign( line, 1, line.size() - 2 );
    return true;
  }
}

void INIFile::parseLine( string line, string group, unsigned count ) {
  unsigned equals = line.find( '=' );
  if ( equals == string::npos ) {
    std::cerr << "Could not parse line " << count << ", ignoring.\n";
    return;
  }

  string name( line, 0, equals );
  cleanup( name );
  
  string value( string( line, equals + 1, string::npos ) );
  cleanup( value );
  parsed_[ group ][ name ] = value;
}

INIFile::INIFile( const std::string& file ) {
  ifstream is( file.c_str() );
  parseStream( is );
  is.close();
}

INIFile::INIFile( std::istream& in ) {
  parseStream( in );
}

void INIFile::parseStream( std::istream &in ) {
  unsigned count = 0;
  string line;
  bool inside = false;

  string group;
  while ( getline( in, line ) ) {
    ++count;
    cleanup( line );
    if ( line.empty() ) continue;
    if ( parseGroupName( line, group ) ) {
      inside = true;
    } else if ( inside ) {
      parseLine( line, group, count );
    }
  }
}

string INIFile::get( string group, string name ) const {
  map<string, map<string, string> >::const_iterator one = 
    parsed_.find( group );
  if ( one == parsed_.end() ) return "";
  
  map<string, string>::const_iterator two = 
    one->second.find( name );
  if ( two == one->second.end() ) return "";
  return two->second;
}

bool INIFile::getBoolean( string group, string name ) const {
  string v = get( group, name ); 
  return( nonCaseEquals( v, "1" ) ||
          nonCaseEquals( v, "yes" ) ||
          nonCaseEquals( v, "true" ) ||
          nonCaseEquals( v, "on" ) );
}

bool INIFile::hasOption( string group, string name ) const {
  return get( group, name ) != "";
}
