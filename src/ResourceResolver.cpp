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
/// \file ResourceResolver.cpp
/// \brief cpp file for ResourceResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ResourceResolver.h>

#include <sys/stat.h>

#ifndef S_ISREG
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif

using namespace H3D;
using namespace H3DUtil;

string ResourceResolver::baseURL( "" );
ResourceResolver::TmpFileNameList ResourceResolver::tmp_files;

string ResourceResolver::resolveURLAs( const string &urn,
                                       bool *is_tmp_file,
                                       bool folder,
                                       bool return_contents ) {
  if( urn == "" ) return "";
  string filename = urn;
  if( urn_resolver().get() ) {
    filename = urn_resolver()->resolveURN( urn );
  }
  
  // first try as relative path
  if( baseURL != "" ) {
    string full_url = baseURL + filename;
    
    // Only return file contents if a resolver explicitly supports it
    // i.e. if it implements resolveURLAsStringInternal()
    if ( !return_contents ) {
      // if is a local file, just return the file name
      struct stat file_info;
      int file_status = ::stat(full_url.c_str(),&file_info);
      if( file_status == 0 && ( folder ?
                                S_ISDIR(file_info.st_mode) :
                                S_ISREG(file_info.st_mode) ) != 0 ) {
        if( is_tmp_file ) *is_tmp_file = false;
        return full_url;
      }
    }
    
    // otherwise try the resolvers.
    for( AutoPtrVector< ResourceResolver >::iterator i = resolvers().begin();
         i != resolvers().end(); ++i ) {
      if ( !return_contents ) {
        string resolved_name = (*i)->resolveURLAsTmpFile( full_url );
        if( resolved_name != "" ) {
          if( is_tmp_file ) *is_tmp_file = true;
          return resolved_name;
        }
      } else {
        string contents = (*i)->resolveURLAsStringInternal( full_url );
        if( contents != "" ) {
          if( is_tmp_file ) *is_tmp_file = false;
          return contents;
        }
      }
    }
  }
  
  // try as absolute path
  
  // if is a local file, just return the file name
  if ( !return_contents ) {
    struct stat file_info;
    int file_status = ::stat(filename.c_str(),&file_info);
    if( file_status == 0 && ( folder ?
                              S_ISDIR(file_info.st_mode) :
                              S_ISREG(file_info.st_mode) ) != 0 ) {
      if( is_tmp_file ) *is_tmp_file = false;
      return filename;
    }
  }
  
  // otherwise try the resolvers.
  for( AutoPtrVector< ResourceResolver >::iterator i = resolvers().begin();
       i != resolvers().end(); ++i ) {
    if ( !return_contents ) {
      string resolved_name = (*i)->resolveURLAsTmpFile( filename );
      if( resolved_name != "" ) {
        if( is_tmp_file ) *is_tmp_file = true;
        return resolved_name;
      }
    } else {
      string contents = (*i)->resolveURLAsStringInternal( filename );
      if( contents != "" ) {
        if( is_tmp_file ) *is_tmp_file = false;
        return contents;
      }
    }
  }
  if( is_tmp_file ) *is_tmp_file = false;
  return "";
}

string ResourceResolver::getTmpFileName() {
#ifdef H3D_WINDOWS
  // special version on Windows that supports Windows Vista.
  // using tmpnam gives filenames in the root directory which
  // only administrators have write access to.
  char * tmp_file_ptr = _tempnam( "", "" );
  string tmp_file( tmp_file_ptr );
  delete tmp_file_ptr;
  if( tmp_file.length() > 0 ) {
      tmp_files.push_back( tmp_file );
      return tmp_file;
  } else {
      return "";
  }
#else
  char tmp_file[ L_tmpnam ];
  if( tmpnam( tmp_file ) ) {
    tmp_files.push_back( tmp_file );
    return tmp_file;
  } else {
    return "";
  } 
#endif
}

bool ResourceResolver::releaseTmpFileName( const string &file ) {
  for( list< string >::iterator i = tmp_files.begin();
       i != tmp_files.end(); ++i ) {
    if( file == (*i) ) {
      remove( (*i).c_str() );
      tmp_files.erase( i );
      return true;
    }
  }
  return false;
}

auto_ptr< URNResolver > & ResourceResolver::urn_resolver() {
  static auto_ptr< URNResolver > urn_resolver( NULL );
  return urn_resolver;
}
