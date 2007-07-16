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
/// \file ResourceResolver.cpp
/// \brief cpp file for ResourceResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <ResourceResolver.h>

using namespace H3D;
using namespace H3DUtil;

auto_ptr< URNResolver > ResourceResolver::urn_resolver( NULL );
AutoPtrVector< ResourceResolver > ResourceResolver::resolvers; 

string ResourceResolver::baseURL( "" );
ResourceResolver::TmpFileNameList ResourceResolver::tmp_files;

string ResourceResolver::resolveURLAsFile( const string &urn,
                                           bool *is_tmp_file ) {
  string filename = urn;
  if( urn_resolver.get() ) {
    filename = urn_resolver->resolveURN( urn );
  }
  
  // first try as relative path
  if( baseURL != "" ) {
    string full_url = baseURL + filename;
    
    // if is a local file, just return the file name
    ifstream is( full_url.c_str() );
    is.close();
    if( !is.fail() ) {
      if( is_tmp_file ) *is_tmp_file = false;
      return full_url;
    }
    
    // otherwise try the resolvers.
    for( AutoPtrVector< ResourceResolver >::iterator i = resolvers.begin();
	 i != resolvers.end(); i++ ) {
      string resolved_name = (*i)->resolveURLAsTmpFile( full_url );
      if( resolved_name != "" ) {
        if( is_tmp_file ) *is_tmp_file = true;
        return resolved_name;
      }
    }
  }
  
  // try as absolute path
  
  // if is a local file, just return the file name
  ifstream is( filename.c_str() );
  is.close();
  if( !is.fail() ) {
    if( is_tmp_file ) *is_tmp_file = false;
    return filename;
  }
  
  // otherwise try the resolvers.
  for( AutoPtrVector< ResourceResolver >::iterator i = resolvers.begin();
       i != resolvers.end(); i++ ) {
    string resolved_name = (*i)->resolveURLAsTmpFile( filename );
    if( resolved_name != "" ) {
      if( is_tmp_file ) *is_tmp_file = true;
      return resolved_name;
    }
  }
  if( is_tmp_file ) *is_tmp_file = false;
  return "";
}

string ResourceResolver::getTmpFileName() {
  char tmp_file[ L_tmpnam ];
  if( tmpnam( tmp_file ) ) {
    tmp_files.push_back( tmp_file );
    return tmp_file;
  } else {
    return "";
  } 
}

bool ResourceResolver::releaseTmpFileName( const string &file ) {
  for( list< string >::iterator i = tmp_files.begin();
       i != tmp_files.end(); i++ ) {
    if( file == (*i) ) {
      remove( (*i).c_str() );
      tmp_files.erase( i );
      return true;
    }
  }
  return false;
}
