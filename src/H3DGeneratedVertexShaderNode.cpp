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
/// \file H3DGeneratedVertexShaderNode.cpp
/// \brief CPP file for H3DGeneratedVertexShaderNode.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DGeneratedVertexShaderNode.h>
#include <H3D/ShaderPart.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DGeneratedVertexShaderNode::database( 
                                   "H3DGeneratedVertexShaderNode", 
                                   NULL,
                                   typeid( H3DGeneratedVertexShaderNode ),
                                   &H3DGeneratedShaderNode::database );

namespace H3DGeneratedVertexShaderNodeInternals {
}

H3DGeneratedVertexShaderNode::H3DGeneratedVertexShaderNode( 
                              Inst< DisplayList  > _displayList,
                              Inst< SFNode       > _metadata,
                              Inst< SFBool       > _isSelected ,
                              Inst< SFBool       > _isValid,
                              Inst< SFBool       > _activate,
                              Inst< SFString     > _language,
                              Inst< MFShaderPart > _parts,
                              Inst< SFBool       > _suppressUniformWarnings,
                              Inst< MFString > _fragmentShaderString,
                              Inst< MFString > _vertexShaderString ) :
  H3DGeneratedShaderNode( _displayList, _metadata, _isSelected, 
                          _isValid, _activate, _language, _parts, 
                          _suppressUniformWarnings, _fragmentShaderString, 
                          _vertexShaderString ) {

  type_name = "H3DGeneratedVertexShaderNode";
  database.initFields( this );

}





