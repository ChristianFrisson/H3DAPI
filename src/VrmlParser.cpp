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

extern int parse( char* );
extern Group* getRoot();

Group* X3D::createVRMLFromString( const string &str,
                                  DEFNodes *dn,
                                  DEFNodes *exported_nodes,
                                  PrototypeVector *prototypes  ) {
  /*Group *g = new Group;
  AutoRef< Node > n = createX3DNodeFromString( str, dn,
                                               exported_nodes, 
                                               prototypes  );
  if( n.get() )
    g->children->push_back( n.get() );
    return g;*/
}

Group* X3D::createVRMLFromURL( const string &url,
                               DEFNodes *dn,
                               DEFNodes *exported_nodes,
                               PrototypeVector *prototypes ) {
  parse( url.c_str() );
  return getRoot();
}

Group* X3D::createVRMLFromStream( istream &is, 
                                   DEFNodes *dn,
                                   DEFNodes *exported_nodes,
                                   PrototypeVector *prototypes ) {
  /*Group *g = new Group;
  AutoRef< Node > n = createX3DNodeFromStream( is, dn, exported_nodes, 
					       prototypes );
  if( n.get() )
    g->children->push_back( n.get() );
    return g;*/
}


AutoRef< Node > X3D::createVRMLNodeFromString( const string &str,
                                                DEFNodes *dn,
                                                DEFNodes *exported_nodes,
                                                PrototypeVector *prototypes ) {
  /*auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  stringstream s;
  s << str;
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  parser->parse( IStreamInputSource( s, (const XMLCh*)L"<string input>" ) );
  return handler.getResultingNode();*/
  AutoRef< Node > n;
  return n;
}

AutoRef< Node > X3D::createVRMLNodeFromURL( const string &url,
                                             DEFNodes *dn,
                                             DEFNodes *exported_nodes,
                                             PrototypeVector *prototypes ) {
  /*
  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  
  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );
  string::size_type pos = urn.find_last_of( "/\\" );
  string path = urn.substr( 0, pos + 1 );
  string old_base = ResourceResolver::getBaseURL();
  
  string resolved_url = ResourceResolver::resolveURLAsFile( url );
  ResourceResolver::setBaseURL( path ); 
  
  if( resolved_url == "" )
    parser->parse( url.c_str() );
  else {
    XERCES_CPP_NAMESPACE_USE
      ifstream is( resolved_url.c_str() );
    XMLCh *url_ch = new XMLCh[ url.size() + 1 ];
    for( unsigned int i = 0; i < url.size(); i++ ) {
      url_ch[i] = url[i];
    }
    url_ch[ url.size() ] = '\0'; 
    parser->parse( IStreamInputSource( is, url_ch ) );
    delete url_ch;
    is.close();
  }
  ResourceResolver::setBaseURL( old_base );
  return handler.getResultingNode();
*/
  AutoRef< Node > n;
  return n;
}

AutoRef< Node > X3D::createVRMLNodeFromStream( istream &is, 
                                                DEFNodes *dn,
                                                DEFNodes *exported_nodes,
                                                PrototypeVector *prototypes ) {
  /*  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  parser->parse( IStreamInputSource( is ) );
  return handler.getResultingNode();
  */
  AutoRef< Node > n;
  return n;
}
