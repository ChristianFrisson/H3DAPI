//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Danel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file VRML.h This file contains functions for creating H3D nodes from
/// VRML/WRL input files.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __VRMLPARSER_H__
#define __VRMLPARSER_H__

#include <iostream>
#include <H3DApi.h>
#include <Group.h>
#include <DEFNodes.h>
#include <H3DUtil/AutoRef.h>
#include <ProtoDeclaration.h>
#include <X3D.h>

using namespace std;

namespace H3D {
  namespace X3D {
    
    // A simple attempt to automatically determine VRMLedness of a 
    // file/string/url.
    bool isVRML( const string &str );
    bool isVRML( istream &is );

    /// Create H3D nodes given VRML data as an istream
    /// \param in The input stream to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the stream.
    /// \param system_id Name of the source of the stream, e.g. file name.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group*createVRMLFromStream( 
           istream &in,
           DEFNodes *dn = NULL,
           DEFNodes *exported_nodes = NULL,
           PrototypeVector *prototypes = NULL
           );

    /// Create H3D nodes given VRML data as a string.
    /// \param str The input string to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the string.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the string.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createVRMLFromString( 
                    const string &str,
                    DEFNodes *dn = NULL,
                    DEFNodes *exported_nodes = NULL,
                    PrototypeVector *prototypes = NULL );


    /// Create H3D nodes given VRML data as a URL.
    /// \param str The input URL to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the URL.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createVRMLFromURL( const string &urn,
                                         DEFNodes *dn = NULL,
                                         DEFNodes *exported_nodes = NULL,
                                         PrototypeVector *prototypes = NULL  );


    /// Create a H3D Node given VRML data as a istream
    /// \param in The input stream to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the stream.
    /// \param system_id Name of the source of the stream, e.g. file name.
    /// \return The created Node. 
    H3DAPI_API AutoRef<Node> createVRMLNodeFromStream( 
         istream &in,
         DEFNodes *dn = NULL,
         DEFNodes *exported_nodes = NULL,
         PrototypeVector *prototypes = NULL
         );

    /// Create a H3D Node given VRML data as a string.
    /// \param str The input string to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the string.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the string.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createVRMLNodeFromString( 
                  const string &str,
                  DEFNodes *dn = NULL,
						      DEFNodes *exported_nodes = NULL,
						      PrototypeVector *prototypes = NULL 
                  );


    /// Create a H3D Node given VRML data as a URL.
    /// \param str The input URL to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the URL.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createVRMLNodeFromURL( 
                 const string &urn,
                 DEFNodes *dn = NULL,
                 DEFNodes *exported_nodes = NULL,
                 PrototypeVector *prototypes = NULL 
                 );


  }
};

#endif
