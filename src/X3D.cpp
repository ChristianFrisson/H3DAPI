//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Danel Evestedt, Mark Dixon
//    All Rights Reserved
//
//
//
//////////////////////////////////////////////////////////////////////////////
#include "X3D.h"
#include "X3DSAX2Handlers.h"
#include "IStreamInputSource.h"
#include "ResourceResolver.h"
#include <sstream>

using namespace H3D;

SAX2XMLReader* X3D::getNewXMLParser() {
  SAX2XMLReader::ValSchemes    valScheme    = SAX2XMLReader::Val_Never;
  bool                         doNamespaces = true;
  bool                         doSchema = false;
  bool                         schemaFullChecking = false;
  //bool                         doList = false;
  //bool                         errorOccurred = false;
  bool                         namespacePrefixes = false;
  //bool                         recognizeNEL = false;
  char                         localeStr[64];
  memset(localeStr, 0, sizeof localeStr);
  SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
  parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, doNamespaces);
  parser->setFeature(XMLUni::fgXercesSchema, doSchema);
  parser->setFeature(XMLUni::fgXercesSchemaFullChecking, 
                     schemaFullChecking);
  parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, 
                     namespacePrefixes);
  if (valScheme == SAX2XMLReader::Val_Auto) {
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgXercesDynamic, true);
  }
  if (valScheme == SAX2XMLReader::Val_Never) {
    parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
  }
  if (valScheme == SAX2XMLReader::Val_Always) {
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
    parser->setFeature(XMLUni::fgXercesDynamic, false);
  }
  return parser;
}

Group* X3D::createX3DFromString( const string &str,
                                 DEFNodes *dn,
				 DEFNodes *exported_nodes,
				 PrototypeVector *prototypes  ) {
  Group *g = new Group;
  AutoRef< Node > n = createX3DNodeFromString( str, dn,
					       exported_nodes, 
					       prototypes  );
  if( n.get() )
    g->children->push_back( n.get() );
  return g;
}

Group* X3D::createX3DFromURL( const string &url,
                              DEFNodes *dn,
			      DEFNodes *exported_nodes,
			      PrototypeVector *prototypes ) {
  Group *g = new Group;
  AutoRef< Node > n = createX3DNodeFromURL( url, dn, exported_nodes, 
					    prototypes );
  if( n.get() )
    g->children->push_back( n.get() );
  return g;
}

Group* X3D::createX3DFromStream( istream &is, 
                                 DEFNodes *dn,
				 DEFNodes *exported_nodes,
				 PrototypeVector *prototypes,
                                 const XMLCh *const system_id ) {
  Group *g = new Group;
  AutoRef< Node > n = createX3DNodeFromStream( is, dn, exported_nodes, 
					       prototypes,system_id );
  if( n.get() )
    g->children->push_back( n.get() );
  return g;
}


AutoRef< Node > X3D::createX3DNodeFromString( const string &str,
                                              DEFNodes *dn,
					      DEFNodes *exported_nodes,
					      PrototypeVector *prototypes ) {
  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  stringstream s;
  s << str << ends;
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  parser->parse( IStreamInputSource( s, (const XMLCh*)L"<string input>" ) );
  return handler.getResultingNode();
}

AutoRef< Node > X3D::createX3DNodeFromURL( const string &url,
                                           DEFNodes *dn,
					   DEFNodes *exported_nodes,
					   PrototypeVector *prototypes ) {
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
}

AutoRef< Node > X3D::createX3DNodeFromStream( istream &is, 
                                              DEFNodes *dn,
					      DEFNodes *exported_nodes,
					      PrototypeVector *prototypes,
                                              const XMLCh *const system_id ) {
  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  parser->parse( IStreamInputSource( is, system_id ) );
  return handler.getResultingNode();
}

void X3D::writeNodeAsX3D( ostream& os, 
                          Node *node, 
                          const string& container_field ) {
  if( !node ) {
    return;
  }
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *node ) );
  string node_name = node->getTypeName();    

  os << "<" << node_name << " containerField=\"" 
     << container_field << "\" ";
    
  vector< pair< string, SFNode * > > sf_nodes;
  vector< pair< string, MFNode * > > mf_nodes;

  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       i != db->fieldDBEnd(); i++ ) {
    Field *f = node->getField( *i );
    Field::AccessType access_type = f->getAccessType();
    if( access_type != Field::INPUT_ONLY &&
        access_type != Field::OUTPUT_ONLY ) {
      if( MFNode *mf_node = dynamic_cast< MFNode * >( f ) ) {
        mf_nodes.push_back( make_pair( f->getName(), mf_node ) );
      } else if( SFNode *sf_node = dynamic_cast< SFNode * >( f ) ) {
        sf_nodes.push_back( make_pair( f->getName(), sf_node ) );
      } else if( ParsableField *p_field = 
                 dynamic_cast< ParsableField * >( f ) ){
        os << f->getName() << "=\'" << p_field->getValueAsString() << "\' ";
      }
    }
  }

  os << ">" << endl;

  for( unsigned int i = 0; i < sf_nodes.size(); i++ ) {
    X3D::writeNodeAsX3D( os, sf_nodes[i].second->getValue(), sf_nodes[i].first );
  }

  for( unsigned int i = 0; i < mf_nodes.size(); i++ ) {
    for( MFNode::const_iterator n = mf_nodes[i].second->begin();
         n != mf_nodes[i].second->end(); n++ ) {
      X3D::writeNodeAsX3D( os, *n, mf_nodes[i].first );
    }
  }
  
  os << "</" << node_name << ">" << endl;
}
