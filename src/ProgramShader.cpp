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
/// \file ProgramShader.cpp
/// \brief CPP file for ProgramShader.
///
//
//
/////////////////////////////////////////////////////////////////////////// ///

#include <H3D/ProgramShader.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ProgramShader::database( 
                                   "ProgramShader", 
                                   &(newInstance<ProgramShader>), 
                                   typeid( ProgramShader ),
                                   &X3DShaderNode::database );

namespace ProgramShaderInternals {
  FIELDDB_ELEMENT( ProgramShader, programs, INPUT_OUTPUT );
}

ProgramShader::ProgramShader( Inst< DisplayList     > _displayList,
                              Inst< SFNode          > _metadata,
                              Inst< SFBool          > _isSelected ,
                              Inst< SFBool          > _isValid,
                              Inst< SFBool          > _activate,
                              Inst< SFString        > _language,
                              Inst< MFShaderProgram > _programs ) :
  X3DShaderNode( _displayList, _metadata, _isSelected, 
                 _isValid, _activate, _language ),
  programs( _programs ) {
  type_name = "ProgramShader";
  database.initFields( this );
  activate->route( displayList, id );
  programs->route( displayList, id );
}

#ifdef HAVE_CG
void ProgramShader::preRender() {
  for( MFShaderProgram::const_iterator i = programs->begin();
       i != programs->end(); i++ ) {
    ShaderProgram *sp = static_cast< ShaderProgram * >( *i );
    sp->enableCGShaderProgram();
  }
  X3DShaderNode::preRender();
}

void ProgramShader::postRender() {
  for( MFShaderProgram::const_iterator i = programs->begin();
       i != programs->end(); i++ ) {
    ShaderProgram *sp = static_cast< ShaderProgram * >( *i );
    sp->disableCGShaderProgram();
  }
  X3DShaderNode::postRender();
}

void ProgramShader::render() {
  // reinitialize the shaders if activate has caused a TRUE event.
  bool reinitialize = displayList->hasCausedEvent( activate ) && 
    activate->getValue( id );

  for( MFShaderProgram::const_iterator i = programs->begin();
       i != programs->end(); i++ ) {
    ShaderProgram *sp = static_cast< ShaderProgram * >( *i );
    if( reinitialize ) sp->initCGShaderProgram();
    sp->displayList->callList();
  }
}

#endif // HAVE_CG
