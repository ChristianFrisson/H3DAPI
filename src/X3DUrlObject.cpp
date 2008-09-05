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
/// \file X3DUrlObject.cpp
/// \brief CPP file for X3DUrlObject, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DUrlObject.h>
#include <H3D/ResourceResolver.h>

using namespace H3D;


X3DUrlObject::X3DUrlObject( 
                           Inst< MFString>  _url ):
  url( _url ),
  url_used( "" ),
  url_base( ResourceResolver::getBaseURL() ) {
  
}


string X3DUrlObject::resolveURLAsFile( const string &url,
                                       bool *is_tmp_file ) {
  for( list< string >::const_iterator i = supported_inline_prefixes.begin();
       i != supported_inline_prefixes.end(); i++ ) {
    if( url.compare( 0, (*i).size(), *i ) == 0 ) {
      string tmp_file = ResourceResolver::getTmpFileName();
      if( tmp_file != "" ) {
        ofstream os( tmp_file.c_str() );
        os << url.substr((*i).size(), url.size()-1);
        os.close();
        if( is_tmp_file ) *is_tmp_file = true;
        return tmp_file;
      }
    }
  }

  string old_base = ResourceResolver::getBaseURL();
  ResourceResolver::setBaseURL( url_base );
  string result = ResourceResolver::resolveURLAsFile( url, is_tmp_file );
  ResourceResolver::setBaseURL( old_base );
  return result;
}

void X3DUrlObject::addInlinePrefix( const string &s ) {
  supported_inline_prefixes.push_back( s + ":" );
}

bool X3DUrlObject::removeTmpFile( const string &file ) {
  return ResourceResolver::releaseTmpFileName( file );
}
