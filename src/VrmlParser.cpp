//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Danel Evestedt, Mark Dixon
//    All Rights Reserved
//
//
//
//////////////////////////////////////////////////////////////////////////////
#include "VrmlParser.h"
#include "IStreamInputSource.h"
#include "ResourceResolver.h"
#include <sstream>
#include "PrototypeVector.h"
#include "VrmlDriver.h"

using namespace H3D;
using namespace X3D;

extern int parse( istream*, const char*, X3D::DEFNodes *, X3D::DEFNodes*,
                  PrototypeVector * );
extern Group* getRoot();


  // A simple attempt to automatically determine VRMLedness of a 
  // file/string/url.
bool X3D::isVRML( const string &str ) {
  int x3d_pos = str.find( "<" );
  int vrml_pos = str.find( "{" );
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
    Console(3) << "WARNING: Could not parse VRML from string" << endl;
  }
  return g;
}

Group* X3D::createVRMLFromURL( const string &url,
                               DEFNodes *dn,
                               DEFNodes *exported_nodes,
                               PrototypeVector *prototypes ) {
  Group *g = new Group;

  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );
  string::size_type pos = urn.find_last_of( "/\\" );
  string path = urn.substr( 0, pos + 1 );
  string old_base = ResourceResolver::getBaseURL();

  string resolved_url = ResourceResolver::resolveURLAsFile( url );
  ResourceResolver::setBaseURL( path ); 

  if( resolved_url == "" ) {
    Console(3) << "WARNING: Could not open file " 
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
  ResourceResolver::setBaseURL( old_base );
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
    Console(3) << "WARNING: Could not parse VRML from stream" << endl;
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
    Console(3) << "WARNING: Could not parse VRML from string" << endl;
  }

  return g;
}

AutoRef< Node > X3D::createVRMLNodeFromURL( const string &url,
                                             DEFNodes *dn,
                                             DEFNodes *exported_nodes,
                                             PrototypeVector *prototypes ) {
  AutoRef< Node > g;
  VrmlDriver driver;
  
  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );
  string::size_type pos = urn.find_last_of( "/\\" );
  string path = urn.substr( 0, pos + 1 );
  string old_base = ResourceResolver::getBaseURL();

  string resolved_url = ResourceResolver::resolveURLAsFile( url );
  ResourceResolver::setBaseURL( path ); 

  if( resolved_url == "" ) {
    Console(3) << "WARNING: Could not open file " 
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
  ResourceResolver::setBaseURL( old_base );
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
    Console(3) << "WARNING: Could not parse VRML from stream" << endl;
  }

  return g;
}
