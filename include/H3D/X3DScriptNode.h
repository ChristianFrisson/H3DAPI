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
/// \file X3DScriptNode.h
/// \brief Base class for all scripting node interfaces
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSCRIPTNODE_H__
#define __X3DSCRIPTNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/SFString.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \brief The base class for script nodes.
  struct H3DAPI_API X3DScriptNode : public X3DChildNode,
                                    public X3DUrlObject {
    
    /// Update the string to use as script depending from the urls given.
    class H3DAPI_API SFScriptString: 
        public TypedField< SFString, MFString > {
    protected:
      virtual void update();
    };

    // Constructor
    X3DScriptNode( Inst< SFNode>  _metadata = 0,
                   Inst< MFString > _url = 0,
                   Inst< SFScriptString > _scriptString = 0 );

    /// The X3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// Internal field that contains the string loaded from file and
    /// to be used as input to the shader.
    /// 
    /// \dotfile ShaderPart_shaderString.dot
    auto_ptr< SFScriptString > scriptString;
  };
}

#endif
