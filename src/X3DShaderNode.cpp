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
/// \file X3DShaderNode.cpp
/// \brief CPP file for X3DShaderNode.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DShaderNode.h>
#include <H3D/GlobalSettings.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DShaderNode::database( 
                                   "X3DShaderNode", 
                                   NULL, 
                                   typeid( X3DShaderNode ),
                                   &X3DAppearanceChildNode::database );

namespace X3DShaderNodeInternals {
  FIELDDB_ELEMENT( X3DShaderNode, isSelected, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DShaderNode, isValid, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DShaderNode, activate, INPUT_ONLY );
  FIELDDB_ELEMENT( X3DShaderNode, language, INITIALIZE_ONLY );
}

X3DShaderNode * X3DShaderNode::active_shader = NULL;

X3DShaderNode::X3DShaderNode( Inst< DisplayList > _displayList,
                              Inst< SFNode      > _metadata,
                              Inst< SFBool      > _isSelected ,
                              Inst< SFBool      > _isValid,
                              Inst< SFBool      > _activate,
                              Inst< SFString    > _language ) :
  X3DAppearanceChildNode( _displayList, _metadata ),
  isSelected( _isSelected ),
  isValid( _isValid ),
  activate( _activate ),
  language( _language ) {
  type_name = "X3DShaderNode";
  database.initFields( this );
  
  isSelected->setValue( false, id );
  isValid->setValue( false, id );
  activate->setValue( false, id );
}

