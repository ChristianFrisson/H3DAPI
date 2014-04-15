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
/// \file VrmlParser.h
/// \brief This file contains functions for creating H3D nodes from
/// VRML/WRL input files.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __VRMLPARSER_H__
#define __VRMLPARSER_H__

#include <iostream>
#include <H3D/H3DApi.h>
#include <H3D/Group.h>
#include <H3D/DEFNodes.h>
#include <H3DUtil/AutoRef.h>
#include <H3D/ProtoDeclaration.h>
#include <H3D/X3D.h>

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
    /// \param urn The input URL to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the URL.
    /// \param change_base_path_during_parsing If true, the base path of the 
    /// ResourceResolver will be set to the part of urn that is before
    /// the file name during parsing of this file. 
    /// E.g. if urn is /test/test.x3d, the base path will be set to /test/. 
    /// If false, it will remain what it was before calling this function.
    /// \return A Group containing the nodes created.
    H3DAPI_API Group* createVRMLFromURL( const string &urn,
                                         DEFNodes *dn = NULL,
                                         DEFNodes *exported_nodes = NULL,
                                         PrototypeVector *prototypes = NULL,
                                         bool change_base_path_during_parsing = true  );


    /// Create a H3D Node given VRML data as a istream
    /// \param in The input stream to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the stream.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the stream.
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
    /// \param urn The input URL to read VRML data from.
    /// \param dn A DEFNodes structure to store the DEF nodes found
    /// in the URL.
    /// \param exported_nodes A DEFNodes structure to store the nodes
    /// that are exported with the EXPORT statement.
    /// \param prototypes A map from the PROTO declaration nodes specified
    /// in the URL.
    /// \param change_base_path_during_parsing If true, the base path of the 
    /// ResourceResolver will be set to the part of urn that is before
    /// the file name during parsing of this file. 
    /// E.g. if urn is /test/test.x3d, the base path will be set to /test/. 
    /// If false, it will remain what it was before calling this function.
    /// \return The created Node.
    H3DAPI_API AutoRef<Node> createVRMLNodeFromURL( 
                 const string &urn,
                 DEFNodes *dn = NULL,
                 DEFNodes *exported_nodes = NULL,
                 PrototypeVector *prototypes = NULL,
                 bool change_base_path_during_parsing = true 
                 );


  }
};

#endif
