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
/// \file H3DScriptNode.h
/// \brief Base class for all scripting node interfaces
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSCRIPTNODE_H__
#define __H3DSCRIPTNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DUrlObject.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \brief The base class for script nodes.
  struct H3DAPI_API H3DScriptNode : public X3DChildNode,
                                    public X3DUrlObject {
    
    // Constructor
    H3DScriptNode( Inst< MFString > _url = 0 );
    
    
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
    
    virtual void loadScript( const string &url ) = 0;
    
    virtual Field* lookupField( const string &name ) = 0;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
