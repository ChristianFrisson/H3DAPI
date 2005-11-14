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

using namespace std;
XERCES_CPP_NAMESPACE_USE

namespace H3D {
  namespace X3D {
    /// Create H3D nodes given X3D data as a istream
    /// \param in The input stream to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param system_id Name of the source of the stream, e.g. file name.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group*createX3DFromStream( 
           istream &in,
           DEFNodes *dn = NULL,
           const XMLCh *const system_id =(const XMLCh *const)L"<stream input>"
         );

    /// Create H3D nodes given X3D data as a string.
    /// \param str The input string to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the string.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createX3DFromString( const string &str,
                                           DEFNodes *dn = NULL );


    /// Create H3D nodes given X3D data as a URL.
    /// \param str The input URL to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createX3DFromURL( const string &urn,
                                        DEFNodes *dn = NULL );

    /// Create a H3D Node given X3D data as a istream
    /// \param in The input stream to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param system_id Name of the source of the stream, e.g. file name.
    /// \return The created Node. 
    H3DAPI_API AutoRef<Node> createX3DNodeFromStream( 
           istream &in,
           DEFNodes *dn = NULL,
           const XMLCh *const system_id =(const XMLCh *const)L"<stream input>"
         );

    /// Create a H3D Node given X3D data as a string.
    /// \param str The input string to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the string.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createX3DNodeFromString( const string &str,
                                                      DEFNodes *dn = NULL );


    /// Create a H3D Node given X3D data as a URL.
    /// \param str The input URL to read X3D data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createX3DNodeFromURL( const string &urn,
                                                   DEFNodes *dn = NULL );

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
