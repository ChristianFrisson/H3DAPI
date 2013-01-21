//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file GeneratedTexture.cpp
/// \brief CPP file for GeneratedTexture.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeneratedTexture.h>

using namespace H3D;

H3DNodeDatabase GeneratedTexture::database( 
        "GeneratedTexture", 
        NULL,
        typeid( GeneratedTexture ),
        &X3DTextureNode::database 
        );

namespace GeneratedTextureInternals {

}

GeneratedTexture::GeneratedTexture( 
                               Inst< DisplayList > _displayList,
                               Inst< SFNode>  _metadata ):
  X3DTexture2DNode( _displayList, _metadata ),
  texture_id_initialized( false ){

  type_name = "GeneratedTexture";
  database.initFields( this ); 
   displayList->setCacheMode( H3DDisplayListObject::DisplayList::OFF );

}

void GeneratedTexture::render() {
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &texture_unit );
  ensureInitialized();
  glBindTexture(  texture_target, texture_id );
  renderTextureProperties();
  enableTexturing();
}


bool GeneratedTexture::ensureInitialized( GLenum tex_target ) {
 if( !texture_id_initialized ) {
    // initialized texture paramters
    glGenTextures( 1, &texture_id );
    texture_id_initialized = true;
    texture_target = tex_target;
  }
 return true;
}


