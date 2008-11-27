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
/// \file X3D.h
/// \brief This file contains functions for creating H3D nodes from X3D input.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __X3D_H__
#define __X3D_H__

#include <iostream>
#ifdef __BORLANDC__
#include <H3D/mem.h>
#endif

#include <H3D/H3DApi.h>
#include <H3D/Group.h>
#include <H3D/DEFNodes.h>
#include <H3DUtil/AutoRef.h>
#include <H3D/PrototypeVector.h>

#ifdef HAVE_XERCES
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
XERCES_CPP_NAMESPACE_USE
#else
typedef char XMLCh;
#endif

using namespace std;


namespace H3D {
  /// X3D namespace
  namespace X3D {

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
    H3DAPI_API AutoRef<Node> createX3DNodeFromString( 
           const string &str,
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
    H3DAPI_API AutoRef<Node> createX3DNodeFromURL( 
           const string &urn,
           DEFNodes *dn = NULL,
           DEFNodes *exported_nodes = NULL,
           PrototypeVector *prototypes = NULL );

#ifdef HAVE_XERCES
    SAX2XMLReader* getNewXMLParser();
#endif
    
    /// Write the scene graph part of the node given in X3D/XML format
    /// to the given ostream.
    H3DAPI_API void writeNodeAsX3D( ostream& os, 
                                    Node *node,
                                    const string& container_field = 
                                    "children" );

  }
};

#endif
