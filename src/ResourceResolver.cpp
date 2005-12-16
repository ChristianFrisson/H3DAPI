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
/// \file ResourceResolver.cpp
/// \brief cpp file for ResourceResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#include "ResourceResolver.h"

using namespace H3D;

auto_ptr< URNResolver > ResourceResolver::urn_resolver( NULL );
AutoPtrVector< ResourceResolver > ResourceResolver::resolvers; 

string ResourceResolver::baseURL( "" );


string ResourceResolver::resolveURLAsFile( const string &urn ) {
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
    if( !is.fail() ) 
      return full_url;
    
    // otherwise try the resolvers.
    for( AutoPtrVector< ResourceResolver >::iterator i = resolvers.begin();
	 i != resolvers.end(); i++ ) {
      string resolved_name = (*i)->resolveURLAsTmpFile( full_url );
      if( resolved_name != "" ) return resolved_name;
    }
  }
  
  // try as absolute path
  
  // if is a local file, just return the file name
  ifstream is( filename.c_str() );
  is.close();
  if( !is.fail() ) 
    return filename;
  
  // otherwise try the resolvers.
  for( AutoPtrVector< ResourceResolver >::iterator i = resolvers.begin();
       i != resolvers.end(); i++ ) {
    string resolved_name = (*i)->resolveURLAsTmpFile( filename );
    if( resolved_name != "" ) return resolved_name;
  }
  return "";
}
