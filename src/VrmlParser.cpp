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
/// \file VrmlParser.cpp
/// \brief CPP file for functions that can be called to parse VRML code.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/VrmlParser.h>
#include <H3D/IStreamInputSource.h>
#include <H3D/ResourceResolver.h>
#include <sstream>
#include <H3D/PrototypeVector.h>
#include <H3D/VrmlDriver.h>

using namespace H3D;
using namespace X3D;

extern int parse( istream*, const char*, X3D::DEFNodes *, X3D::DEFNodes*,
                  PrototypeVector * );
extern Group* getRoot();


  // A simple attempt to automatically determine VRMLedness of a 
  // file/string/url.
bool X3D::isVRML( const string &str ) {
  size_t x3d_pos = str.find( "<" );
  size_t vrml_pos = str.find( "{" );
  if ( vrml_pos == string::npos )
    return false;
  else if  ( x3d_pos == string::npos )
    return true;
  else if ( x3d_pos < vrml_pos )
    return false;
  else if ( x3d_pos > vrml_pos )
    return true;
  else if ( str.find( "#VRML" ) == 0 )
    return true;
  else
    return false;
}

bool X3D::isVRML( istream &is ) {
  char c;
  while ( is.get(c) ) {
    if ( c == '<' )
      return false;
    if ( c == '{' )
      return true;
    if ( c == '#' ) {
      // could be a VRML comment
      while ( is.get(c) ) {
        if ( c == '\n' )
          break;
      }
    }
  }
  return false;
}

Group* X3D::createVRMLFromString( const string &str,
                                  DEFNodes *dn,
                                  DEFNodes *exported_nodes,
                                  PrototypeVector *prototypes  ) {
  Group *g = new Group;
  stringstream inp;
  inp << str;
  VrmlDriver driver;
  if (driver.parse( &inp, "<string>", dn, exported_nodes, prototypes )) {
    Group *c = driver.getRoot();
    if ( c )
      g->children->push_back( c );
  } else {
    Console(3) << "Warning: Could not parse VRML from string" << endl;
  }
  return g;
}

Group* X3D::createVRMLFromURL( const string &url,
                               DEFNodes *dn,
                               DEFNodes *exported_nodes,
                               PrototypeVector *prototypes,
                               bool change_base_path_during_parsing ) {
  // First try to resolve the url to file contents and load via string buffer
  // Otherwise fallback on using temp files
  string url_contents= ResourceResolver::resolveURLAsString ( url );
  if ( url_contents != "" ) {
    return createVRMLFromString ( url_contents, dn, exported_nodes, prototypes );
  }

  Group *g = new Group;

  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );
  string::size_type pos = urn.find_last_of( "/\\" );
  string path = urn.substr( 0, pos + 1 );
  string old_base = ResourceResolver::getBaseURL();

  bool is_tmp_file;
  string resolved_url = ResourceResolver::resolveURLAsFile( url, 
                                                            &is_tmp_file );
  if( change_base_path_during_parsing )
    ResourceResolver::setBaseURL( path ); 

  if( resolved_url == "" ) {
    Console(3) << "Warning: Could not open file " 
               << url << " for parsing." << endl;
    ResourceResolver::setBaseURL( old_base );
    return g;
  }

  VrmlDriver driver;
  ifstream inp(resolved_url.c_str() );
  
  if (driver.parse( &inp, url.c_str(), dn, exported_nodes, prototypes )) {
    Group *c = driver.getRoot();
    if ( c )
      g->children->push_back( c );
  } 
  inp.close();
  ResourceResolver::setBaseURL( old_base );
  if( is_tmp_file ) ResourceResolver::releaseTmpFileName( resolved_url );
  return g;
}

Group* X3D::createVRMLFromStream( istream &is, 
                                   DEFNodes *dn,
                                   DEFNodes *exported_nodes,
                                   PrototypeVector *prototypes ) {
  Group *g = new Group;
  VrmlDriver driver;
  if (driver.parse( &is, "<stream>", dn, exported_nodes, prototypes )) {
    Group *c = driver.getRoot();
    if ( c )
      g->children->push_back( c );
  } else {
    Console(3) << "Warning: Could not parse VRML from stream" << endl;
  }
  return g;
}


AutoRef< Node > X3D::createVRMLNodeFromString( const string &str,
                                                DEFNodes *dn,
                                                DEFNodes *exported_nodes,
                                                PrototypeVector *prototypes ) {
  AutoRef< Node > g;
  stringstream inp;
  inp << str;
  VrmlDriver driver;
  if (driver.parse( &inp, "<string>", dn, exported_nodes, prototypes )) {
    Group *c = driver.getRoot();
    if ( c && !c->children->empty() )
      g.reset( c->children->front() );
  } else {
    Console(3) << "Warning: Could not parse VRML from string" << endl;
  }

  return g;
}

AutoRef< Node > X3D::createVRMLNodeFromURL( const string &url,
                                            DEFNodes *dn,
                                            DEFNodes *exported_nodes,
                                            PrototypeVector *prototypes,
                                            bool change_base_path_during_parsing ) {
  // First try to resolve the url to file contents and load via string buffer
  // Otherwise fallback on using temp files
  string url_contents= ResourceResolver::resolveURLAsString ( url );
  if ( url_contents != "" ) {
    return createVRMLNodeFromString ( url_contents, dn, exported_nodes, prototypes );
  }

  AutoRef< Node > g;
  VrmlDriver driver;
  
  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );
  string::size_type pos = urn.find_last_of( "/\\" );
  string path = urn.substr( 0, pos + 1 );
  string old_base = ResourceResolver::getBaseURL();

  bool is_tmp_file;
  string resolved_url = ResourceResolver::resolveURLAsFile( url, 
                                                            &is_tmp_file );

  if( change_base_path_during_parsing )
    ResourceResolver::setBaseURL( path ); 

  if( resolved_url == "" ) {
    Console(3) << "Warning: Could not open file " 
               << url << " for parsing." << endl;
    ResourceResolver::setBaseURL( old_base );
    return g;
  }

  ifstream inp(resolved_url.c_str() );  
  if (driver.parse( &inp, url.c_str(), dn, exported_nodes, prototypes )) {
    Group *c = driver.getRoot();
    if ( c && !c->children->empty() )
      g.reset( c->children->front() );
  } 
  inp.close();
  ResourceResolver::setBaseURL( old_base );
  if( is_tmp_file ) ResourceResolver::releaseTmpFileName( resolved_url );
  return g;
}

AutoRef< Node > X3D::createVRMLNodeFromStream( istream &is, 
                                                DEFNodes *dn,
                                                DEFNodes *exported_nodes,
                                                PrototypeVector *prototypes ) {
  AutoRef< Node > g;
  VrmlDriver driver;
  if (driver.parse( &is, "<stream>", dn, exported_nodes, prototypes )) {
    Group *c = driver.getRoot();
    if ( c && !c->children->empty() )
      g.reset( c->children->front() );
  } else {
    Console(3) << "Warning: Could not parse VRML from stream" << endl;
  }

  return g;
}
