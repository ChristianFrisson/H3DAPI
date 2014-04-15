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
/// \file H3DImageLoaderNode.cpp
/// \brief CPP file for H3DImageLoaderNode
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DImageLoaderNode.h>

using namespace H3D;

// initialize static variables.
H3DImageLoaderNode::local_auto_ptr<list< H3DImageLoaderNode::
FileReaderRegistration> >H3DImageLoaderNode::registered_file_readers; 

bool H3DImageLoaderNode::initialized = false; 

H3DImageLoaderNode *H3DImageLoaderNode::getSupportedFileReader( 
   const string &url ) {
  if( initialized ) {
    for( list< FileReaderRegistration >::iterator i = 
           registered_file_readers->begin();
         i != registered_file_readers->end(); ++i ) {
      if( (*i).supports_func( url ) )
        return (*(*i).create_func)();
    }
  }
  return NULL;
}

H3DImageLoaderNode *H3DImageLoaderNode::getSupportedFileReader( 
   istream &is ) {
  if( initialized ) {
    for( list< FileReaderRegistration >::iterator i = 
           registered_file_readers->begin();
         i != registered_file_readers->end(); ++i ) {
      if( (*i).supports_stream_func && (*i).supports_stream_func( is ) )
        return (*(*i).create_func)();
    }
  }
  return NULL;
}
