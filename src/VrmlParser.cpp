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

using namespace H3D;

extern int parse( istream*, X3D::DEFNodes *, X3D::DEFNodes* );
extern Group* getRoot();

Group* X3D::createVRMLFromString( const string &str,
                                  DEFNodes *dn,
                                  DEFNodes *exported_nodes,
                                  PrototypeVector *prototypes  ) {
  Group *g = new Group;
  AutoRef< Node > n = createVRMLNodeFromString( str, dn,
                                                exported_nodes, 
                                                prototypes  );
  if( n.get() )
    g->children->push_back( n.get() );
    return g;
}

Group* X3D::createVRMLFromURL( const string &url,
                               DEFNodes *dn,
                               DEFNodes *exported_nodes,
                               PrototypeVector *prototypes ) {
  Group *g = new Group;
  AutoRef< Node > n = createVRMLNodeFromURL( url, dn, exported_nodes, 
					    prototypes );
  if( n.get() )
    g->children->push_back( n.get() );
  return g;
}

Group* X3D::createVRMLFromStream( istream &is, 
                                   DEFNodes *dn,
                                   DEFNodes *exported_nodes,
                                   PrototypeVector *prototypes ) {
  Group *g = new Group;
  AutoRef< Node > n = createVRMLNodeFromStream( is, dn, exported_nodes, 
					       prototypes );
  if( n.get() )
    g->children->push_back( n.get() );
    return g;
}


AutoRef< Node > X3D::createVRMLNodeFromString( const string &str,
                                                DEFNodes *dn,
                                                DEFNodes *exported_nodes,
                                                PrototypeVector *prototypes ) {
  AutoRef< Node > g;
  stringstream inp;
  inp << str;
  if (parse( &inp, dn, exported_nodes )) {
    Console(3) << "Finished parsing!" << endl;
    Group *c = getRoot();
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
  ifstream inp(url.c_str() );
  if (parse( &inp, dn, exported_nodes )) {
    Console(3) << "Finished parsing!" << endl;
    Group *c = getRoot();
    if ( c && !c->children->empty() )
      g.reset( c->children->front() );
  } else {
    Console(3) << "WARNING: Could not open file " 
               << url << " for parsing." << endl;
  }

  return g;
}

AutoRef< Node > X3D::createVRMLNodeFromStream( istream &is, 
                                                DEFNodes *dn,
                                                DEFNodes *exported_nodes,
                                                PrototypeVector *prototypes ) {
  AutoRef< Node > g;
  if (parse( &is, dn, exported_nodes )) {
    Console(3) << "Finished parsing!" << endl;
    Group *c = getRoot();
    if ( c && !c->children->empty() )
      g.reset( c->children->front() );
  } else {
    Console(3) << "WARNING: Could not parse VRML from stream" << endl;
  }

  return g;
}
