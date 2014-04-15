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
/// \file X3DScriptNode.cpp
/// \brief cpp file for X3DScriptNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/X3DScriptNode.h>
#include <H3D/ResourceResolver.h>

#include <fstream>
using namespace H3D;

H3DNodeDatabase X3DScriptNode::database( 
                                        "X3DScriptNode", 
                                        NULL,
                                        typeid( X3DScriptNode ),
                                        &X3DChildNode::database 
                                         );

namespace X3DScriptNodeInternals {
  FIELDDB_ELEMENT( X3DScriptNode, url, INPUT_OUTPUT );
}

X3DScriptNode::X3DScriptNode( Inst< SFNode>  _metadata,
                              Inst< MFString > _url,
                              Inst< SFScriptString > _scriptString ): 
  X3DChildNode( _metadata ),
  X3DUrlObject( _url ),
  scriptString( _scriptString ) {
  type_name = "X3DScriptNode";
  database.initFields( this );
  
  scriptString->setOwner( this );

#ifdef HAVE_SPIDERMONKEY
  addInlinePrefix( "ecmascript" );
#endif

  // set up internal routes
  url->route( scriptString );
}

void X3DScriptNode::SFScriptString::update() {
  X3DScriptNode *script_node = static_cast< X3DScriptNode * >( getOwner() ); 
  MFString *urls = static_cast< MFString * >( routes_in[0] );

  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
    // First try to resolve the url to file contents and load via string buffer
    // Otherwise fallback on using temp files
    string url_contents= ResourceResolver::resolveURLAsString ( *i );
    if ( url_contents != "" ) {
      script_node->setURLUsed( *i );
      value= url_contents;
      return;
    }

    bool is_tmp_file;
    string url = script_node->resolveURLAsFile( *i, &is_tmp_file );
    if( url != "" ) {
      ifstream is( url.c_str() );
      if( is.good() ) {
        std::streamsize length;
        char * buffer;
        
        // get length of file:
        is.seekg (0, ios::end);
        length = is.tellg();
        is.seekg (0, ios::beg);
        
      // allocate memory:
        buffer = new char [(unsigned int)length + 1];
        // read data as a block:
        is.read (buffer,length);
        length = is.gcount();
        is.close();
        if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
        buffer[length] = '\0';
        script_node->setURLUsed( *i );
        value = string( buffer );
        delete [] buffer;
        return;
      }
      is.close();
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
     }
  }
  Console(4) << "None of the urls [";
  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {  
    Console(4) << " \"" << *i << "\"";
  }
  Console(4) << "] could be loaded in Script node.";
  script_node->setURLUsed( "" );
  value = "";
}
