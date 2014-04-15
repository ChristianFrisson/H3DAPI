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
           const string & system_id = "<stream input>"
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
    /// \param urn The input URL to read X3D data from.
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
    H3DAPI_API Group* createX3DFromURL( const string &urn,
                                        DEFNodes *dn = NULL,
                                        DEFNodes *exported_nodes = NULL,
                                        PrototypeVector *prototypes = NULL,
                                        bool change_base_path_during_parsing = true);
    
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
           const string & system_id = "<stream input>"
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
    /// \param urn The input URL to read X3D data from.
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
    H3DAPI_API AutoRef<Node> createX3DNodeFromURL( 
           const string &urn,
           DEFNodes *dn = NULL,
           DEFNodes *exported_nodes = NULL,
           PrototypeVector *prototypes = NULL,
           bool change_base_path_during_parsing = true );

    /// Write the scene graph part of the node given in X3D/XML format
    /// to the given ostream.
    H3DAPI_API void writeNodeAsX3D( ostream& os, 
                                    Node *node,
                                    const string &container_field = "" );

    /// Write the scene graph part of the node given in X3D/VRML format
    /// to the given ostream.
    H3DAPI_API void writeNodeAsVRML( ostream& os, 
                                     Node *node );


    /// Write the triangles rendered by the geometry node as STL to
    /// the given ostream. The name parameter specifies the name to
    /// use in the stl file for the solid. 
    H3DAPI_API void writeGeometryAsSTL( ostream &os,
                                        X3DGeometryNode *geom,
                                        const string &name = "",
                                        bool use_binary_format = false );

    /// Help function for writeNodeAsX3D and writeNodeAsVRML
    /// output_type = 0 means xml output, 1 means vrml output.
    H3DAPI_API void writeNodeAsX3DHelp( ostream& os, 
                                        Node *node,
                                        const string& container_field,
                                        const string & prefix,
                                        std::set< Node * > &visited_nodes,
                                        unsigned int output_type = 0 );

  }
};

#endif
