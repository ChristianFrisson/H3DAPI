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
/// \file GeneratedTexture3D.cpp
/// \brief CPP file for GeneratedTexture3D.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeneratedTexture3D.h>

using namespace H3D;

H3DNodeDatabase GeneratedTexture3D::database( 
        "GeneratedTexture3D", 
        NULL,
        typeid( GeneratedTexture3D ),
        &X3DTextureNode::database 
        );

namespace GeneratedTexture3DInternals {

}

GeneratedTexture3D::GeneratedTexture3D( 
                               Inst< DisplayList > _displayList,
                               Inst< SFNode>  _metadata ):
  X3DTexture3DNode( _displayList, _metadata ),
  texture_id_initialized( false ){

  type_name = "GeneratedTexture3D";
  database.initFields( this ); 

  displayList->setCacheMode( H3DDisplayListObject::DisplayList::OFF );

}

void GeneratedTexture3D::render() {
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &texture_unit );

  ensureInitialized();
  glBindTexture(  texture_target, texture_id );
  renderTextureProperties();
  enableTexturing();
}


bool GeneratedTexture3D::ensureInitialized( GLenum tex_target ) {
 if( !texture_id_initialized ) {
    // initialized texture parameters
    glGenTextures( 1, &texture_id );
    texture_id_initialized = true;
    texture_target = tex_target;
 }
 return true;
}


