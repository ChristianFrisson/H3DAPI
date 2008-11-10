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
/// \file X3D.cpp
/// \brief CPP file for functions needed when parsing X3D code.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/X3D.h>

#ifdef HAVE_XERCES
#include <H3D/X3DSAX2Handlers.h>
#include <H3D/IStreamInputSource.h>
#endif

#include <H3D/ResourceResolver.h>
#include <H3D/VrmlParser.h>
#include <sstream>

#ifdef HAVE_ZLIB
#include <zlib.h>

#define ZIP_MAGIC_NR 0x8B1F

#endif

using namespace H3D;

#ifdef HAVE_XERCES

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

#endif

Group* X3D::createX3DFromString( const string &str,
                                 DEFNodes *dn,
                                 DEFNodes *exported_nodes,
                                 PrototypeVector *prototypes  ) {
  if ( isVRML( str ) )
    return createVRMLFromString( str, dn, exported_nodes, prototypes );
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
  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );

  bool is_tmp_file;
  string resolved_url = ResourceResolver::resolveURLAsFile( url, 
    &is_tmp_file );

  string::size_type pos = resolved_url.find_last_of( "/\\" );
  string path = resolved_url.substr( 0, pos + 1 );
  if( is_tmp_file ) {
    pos = url.find_last_of( "/\\" );
    path = url.substr( 0, pos + 1 );
  }
  string old_base = ResourceResolver::getBaseURL();

#ifdef WIN32
  // needed when running H3DAPI as a plugin to a web-browser
  // and if url points to a local file.
  // Needed for plugin for IE
  string remove_string = "file:///";
  if( path.find( remove_string ) == 0 ) {
    path = path.substr( remove_string.size(), pos + 1 );
  }

  // Needed for plugin for Netscape ( tested on Opera )
  remove_string = "file://localhost/";
  if( path.find( remove_string ) == 0 ) {
    path = path.substr( remove_string.size(), pos + 1 );
  }
#endif

  ResourceResolver::setBaseURL( path ); 

#ifdef HAVE_XERCES
  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
#endif

  if( resolved_url == "" ) {
#ifdef HAVE_XERCES
    parser->parse( url.c_str() );
#endif

  } else {

#ifdef HAVE_ZLIB
    // check if zip-file
    ifstream ifs( resolved_url.c_str(), ios::binary );
    unsigned short magic_nr;
    ifs.read( (char *)&magic_nr, sizeof( unsigned short ) );
    bool file_exists = ifs.good();
    ifs.close();

    // then unpack it
    if( file_exists && magic_nr == ZIP_MAGIC_NR ) {
      string tmp_file = ResourceResolver::getTmpFileName();
      if( tmp_file != "" ) { 
        gzFile in  = gzopen( resolved_url.c_str(),"rb");
        ofstream ofs( tmp_file.c_str(), ios::binary );
        if( in && ofs.good() ) {
          char buf[ 16384 ];  
          int len = 0;
          while( (len = gzread( in, buf, sizeof(buf))) != 0 ) {
            ofs.write( buf, len );
          }
          gzclose(in);
          ofs.close();
          if( is_tmp_file ) 
            ResourceResolver::releaseTmpFileName( resolved_url );
          resolved_url = tmp_file;
          is_tmp_file = true;
        }
      }
    }
#endif


    ifstream istest( resolved_url.c_str() );
    if ( isVRML( istest ) ) {
      istest.close();
      Group *g = createVRMLFromURL( resolved_url, dn,
                                    exported_nodes, prototypes );
      if( is_tmp_file ) 
        ResourceResolver::releaseTmpFileName( resolved_url );
      ResourceResolver::setBaseURL( old_base );
      return g;
    }
    istest.close();

    #ifdef HAVE_XERCES
    XERCES_CPP_NAMESPACE_USE
    // else...
    ifstream is( resolved_url.c_str() );
    XMLCh *url_ch = new XMLCh[ url.size() + 1 ];
    for( unsigned int i = 0; i < url.size(); i++ ) {
      url_ch[i] = url[i];
    }
    url_ch[ url.size() ] = '\0'; 
    parser->parse( IStreamInputSource( is, url_ch ) );
    delete[] url_ch;
    is.close();
#endif   
  }
  if( is_tmp_file ) 
    ResourceResolver::releaseTmpFileName( resolved_url );
  ResourceResolver::setBaseURL( old_base );

#ifdef HAVE_XERCES
  Group *g = new Group;
  AutoRef< Node > n = handler.getResultingNode();
  if( n.get() )
    g->children->push_back( n.get() );
  return g;
#else
  Console(3) << "H3D API compiled without HAVE_XERCES flag. X3D-XML files "
       << "are not supported" << endl;
  return 0;
#endif
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
  if ( isVRML( str ) )
    return createVRMLNodeFromString( str, dn, exported_nodes, prototypes );
  else {
#ifdef HAVE_XERCES
    auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
    X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
    stringstream s;
    s << str;
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler); 
    parser->parse( IStreamInputSource( s, (const XMLCh*)L"<string input>" ) );
    return handler.getResultingNode();
#else
  Console(3) << "H3D API compiled without HAVE_XERCES flag. X3D-XML files "
       << "are not supported" << endl;
  return AutoRef< Node >(NULL);
#endif
  }
}

AutoRef< Node > X3D::createX3DNodeFromURL( const string &url,
                                           DEFNodes *dn,
             DEFNodes *exported_nodes,
             PrototypeVector *prototypes ) {

  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  string urn = url;
  if( urn_resolver ) urn = urn_resolver->resolveURN( urn );

  bool is_tmp_file;
  string resolved_url = ResourceResolver::resolveURLAsFile( url, 
                                                            &is_tmp_file );
  
  string::size_type pos = resolved_url.find_last_of( "/\\" );
  string path = resolved_url.substr( 0, pos + 1 );
  if( is_tmp_file ) {
    pos = url.find_last_of( "/\\" );
    path = url.substr( 0, pos + 1 );
  }
  string old_base = ResourceResolver::getBaseURL();

#ifdef WIN32
  // needed when running H3DAPI as a plugin to a web-browser
  // and if url points to a local file.
  // Needed for plugin for IE
  string remove_string = "file:///";
  if( path.find( remove_string ) == 0 ) {
    path = path.substr( remove_string.size(), pos + 1 );
  }

  // Needed for plugin for Netscape ( tested on Opera )
  remove_string = "file://localhost/";
  if( path.find( remove_string ) == 0 ) {
    path = path.substr( remove_string.size(), pos + 1 );
  }
#endif

  ResourceResolver::setBaseURL( path ); 

#ifdef HAVE_XERCES
  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
#endif

  if( resolved_url == "" ) {
#ifdef HAVE_XERCES
    parser->parse( url.c_str() );
#endif

  } else {
 
#ifdef HAVE_ZLIB
    // check if zip-file
    ifstream ifs( resolved_url.c_str(), ios::binary );
    unsigned short magic_nr;
    ifs.read( (char *)&magic_nr, sizeof( unsigned short ) );
    bool file_exists = ifs.good();
    ifs.close();

    // then unpack it
    if( file_exists && magic_nr == ZIP_MAGIC_NR ) {
      string tmp_file = ResourceResolver::getTmpFileName();
      if( tmp_file != "" ) { 
        gzFile in  = gzopen( resolved_url.c_str(),"rb");
        ofstream ofs( tmp_file.c_str(), ios::binary );
        if( in && ofs.good() ) {
          char buf[ 16384 ];  
          int len = 0;
          while( (len = gzread( in, buf, sizeof(buf))) != 0 ) {
            ofs.write( buf, len );
          }
          gzclose(in);
          ofs.close();
          if( is_tmp_file ) 
            ResourceResolver::releaseTmpFileName( resolved_url );
          resolved_url = tmp_file;
          is_tmp_file = true;
        }
      }
    }
#endif


    ifstream istest( resolved_url.c_str() );
    if ( isVRML( istest ) ) {
      istest.close();
      // reset base to what it was before since createVRMLNodeFromURL
      // will to its own resolving.
      ResourceResolver::setBaseURL( old_base );
      AutoRef< Node > n = createVRMLNodeFromURL( resolved_url, dn,
                                                 exported_nodes, prototypes );
      if( is_tmp_file ) 
        ResourceResolver::releaseTmpFileName( resolved_url );
      ResourceResolver::setBaseURL( old_base );
      return n;
    }
    istest.close();

#ifdef HAVE_XERCES
    XERCES_CPP_NAMESPACE_USE
    // else...
    ifstream is( resolved_url.c_str() );
    XMLCh *url_ch = new XMLCh[ url.size() + 1 ];
    for( unsigned int i = 0; i < url.size(); i++ ) {
      url_ch[i] = url[i];
    }
    url_ch[ url.size() ] = '\0'; 
    parser->parse( IStreamInputSource( is, url_ch ) );
    delete[] url_ch;
    is.close();
#endif   
  }
  if( is_tmp_file ) 
    ResourceResolver::releaseTmpFileName( resolved_url );
  ResourceResolver::setBaseURL( old_base );

#ifdef HAVE_XERCES
  return handler.getResultingNode();
#else
  Console(3) << "H3D API compiled without HAVE_XERCES flag. X3D-XML files "
       << "are not supported" << endl;
  return AutoRef< Node >(NULL);
#endif
}

AutoRef< Node > X3D::createX3DNodeFromStream( istream &is, 
                                              DEFNodes *dn,
                DEFNodes *exported_nodes,
                PrototypeVector *prototypes,
                                              const XMLCh *const system_id ) {
#ifdef HAVE_XERCES
  auto_ptr< SAX2XMLReader > parser( getNewXMLParser() );
  X3DSAX2Handlers handler( dn, exported_nodes, prototypes );
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  parser->parse( IStreamInputSource( is, system_id ) );
  return handler.getResultingNode();
#else
  Console(3) << "H3D API compiled without HAVE_XERCES flag. X3D-XML files "
       << "are not supported" << endl;
  return AutoRef< Node >( NULL );
#endif
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
    
  if( node->hasName() ) {
    os << "DEF=\"" << node->getName() << "\" ";
  }

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
