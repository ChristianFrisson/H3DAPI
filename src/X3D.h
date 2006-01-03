//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Danel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file X3D.h This file contains functions for creating H3D nodes from
/// X3D input.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __X3D_H__
#define __X3D_H__

#include <iostream>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include "H3DApi.h"
#include "Group.h"
#include "DEFNodes.h"
#include "AutoRef.h"
#include "ProtoDeclaration.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

namespace H3D {
  namespace X3D {

    class PrototypeVector: public AutoPtrVector< ProtoDeclaration > {
    public:
      PrototypeVector(): first_proto( NULL ) {}
      ProtoDeclaration *getProtoDeclaration( const string & name ) {
	for( PrototypeVector::iterator i = begin(); i != end(); i++ ) {
	  if( (*i)->getName() == name )
	    return *i;
	}
	return NULL;
      }
      inline ProtoDeclaration *getFirstProtoDeclaration() {
	return first_proto;
      }

      inline void setFirstProtoDeclaration( ProtoDeclaration *pd ) {
	first_proto = pd;
      }
    protected:
      ProtoDeclaration *first_proto;
    };

    /// Create H3D nodes given X3D data as a istream
    /// \param in The input stream to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the stream.
    /// \param system_id Name of the source of the stream, e.g. file name.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group*createX3DFromStream( 
           istream &in,
           DEFNodes *dn = NULL,
	   DEFNodes *exported_nodes = NULL,
	   PrototypeVector *prototypes = NULL,
           const XMLCh *const system_id =(const XMLCh *const)L"<stream input>"
         );

    /// Create H3D nodes given X3D data as a string.
    /// \param str The input string to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the string.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the string.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createX3DFromString( const string &str,
                                           DEFNodes *dn = NULL,
					   DEFNodes *exported_nodes = NULL,
					   PrototypeVector *prototypes = NULL );


    /// Create H3D nodes given X3D data as a URL.
    /// \param str The input URL to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the URL.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createX3DFromURL( const string &urn,
                                        DEFNodes *dn = NULL,
					DEFNodes *exported_nodes = NULL,
					PrototypeVector *prototypes = NULL  );

    /// Create a H3D Node given X3D data as a istream
    /// \param in The input stream to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the stream.
    /// \param system_id Name of the source of the stream, e.g. file name.
    /// \return The created Node. 
    H3DAPI_API AutoRef<Node> createX3DNodeFromStream( 
           istream &in,
           DEFNodes *dn = NULL,
	   DEFNodes *exported_nodes = NULL,
	   PrototypeVector *prototypes = NULL,
           const XMLCh *const system_id =(const XMLCh *const)L"<stream input>"
         );

    /// Create a H3D Node given X3D data as a string.
    /// \param str The input string to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the string.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the string.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createX3DNodeFromString( const string &str,
                                                      DEFNodes *dn = NULL,
						      DEFNodes *exported_nodes = NULL,
						      PrototypeVector *prototypes = NULL );


    /// Create a H3D Node given X3D data as a URL.
    /// \param str The input URL to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the URL.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createX3DNodeFromURL( const string &urn,
                                                   DEFNodes *dn = NULL,
						   DEFNodes *exported_nodes = NULL,
						   PrototypeVector *prototypes = NULL );

    SAX2XMLReader* getNewXMLParser();
    
    /// Write the scene graph part of the node given in X3D/XML format
    /// to the given ostream.
    H3DAPI_API void writeNodeAsX3D( ostream& os, 
                                    Node *node,
                                    const string& container_field = 
                                    "children" );

  }
};

#endif
