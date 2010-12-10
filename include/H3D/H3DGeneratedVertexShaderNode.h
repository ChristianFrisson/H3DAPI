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
/// \file H3DGeneratedVertexShaderNode.h
/// \brief Header file for H3DGeneratedVertexShaderNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DGENERATEDVERTEXSHADERNODE_H__
#define __H3DGENERATEDVERTEXSHADERNODE_H__

#include <H3D/H3DGeneratedShaderNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DGeneratedVertexShaderNode
  /// \brief The H3DGeneratedVertexShaderNode node is the base class for shader
  /// generator nodes that only modify the vertex shader values and ignores the 
  /// fragment shader. Sub-classes should only specify the getVertexShaderString
  /// function which should change the default settings of gl_Position, 
  /// gl_TexCoord[0] or other vertex shader builtin variable. 
  /// 
  /// \par Internal routes:
  /// \dotfile H3DGeneratedVertexShaderNode.dot
  class H3DAPI_API H3DGeneratedVertexShaderNode : 
    public H3DGeneratedShaderNode {
  public:
  
    /// Constructor.
    H3DGeneratedVertexShaderNode( Inst< DisplayList  > _displayList = 0,
                                  Inst< SFNode       > _metadata    = 0,
                                  Inst< SFBool       > _isSelected  = 0,
                                  Inst< SFBool       > _isValid     = 0,
                                  Inst< SFBool       > _activate    = 0,
                                  Inst< SFString     > _language    = 0,
                                  Inst< MFShaderPart > _parts       = 0,
                                  Inst< SFBool       > _suppressUniformWarnings = 0,
                                  Inst< MFString > _fragmentShaderString = 0,
                                  Inst< MFString > _vertexShaderString = 0 );
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
