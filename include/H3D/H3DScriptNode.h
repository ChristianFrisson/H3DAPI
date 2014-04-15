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
/// \file H3DScriptNode.h
/// \brief Base class for all scripting node interfaces
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSCRIPTNODE_H__
#define __H3DSCRIPTNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/DEFNodes.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \brief The base class for script nodes.
  struct H3DAPI_API H3DScriptNode : public X3DChildNode,
                                    public X3DUrlObject {
    
    // Constructor
    H3DScriptNode( Inst< MFString > _url = 0 );
    
    /// Destructor.
    ~H3DScriptNode();

    // Standard node functions:
    // Script interfacing functions, implemented as pure virtuals, to
    // be implemented for each specifc scripting language
    
    virtual void initialiseParser() { 
      name = getInstanceName();   
    };

    virtual string getInstanceName() {
      ostringstream namestr;
      namestr << "H3DScriptNode_" << this; 
      return namestr.str();     
    };
    
    virtual void loadScript( const string &script_filename, const string &script_content= "" )= 0;
    
    virtual Field* lookupField( const string &name ) const = 0;

    /// Add all nodes from the given DEFNodes instance to the scripts
    /// set of named nodes. If nodes with the same name already exists
    /// they will be overwritten.
    void addNamedNodes( X3D::DEFNodes *dn );

    /// Add a named node to the script. If a node with the same name 
    /// already exists it will be overwritten.
    void addNamedNode( const string &name, Node *n );

    /// Remove the named node with the provided name. 
    /// \return 0 on success, -1 if node with that name does not exist.
    int removeNamedNode( const string &name );

    /// Remove all named nodes.
    void clearNamedNodes();

    /// Returns the named node with the given name. NULL if no such node
    /// exists.
    Node *getNamedNode( const string &name );
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    typedef std::map< string, Node * > NamedNodes;
    NamedNodes named_nodes;

    // Callback for removing named nodes when the nodes are destructed.  
    static void removeNamedNodeCB( Node *n, void *data );
  };
}

#endif
