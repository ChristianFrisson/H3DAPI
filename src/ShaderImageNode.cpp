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
/// \file ShaderImageNode.cpp
/// \brief CPP file for ShaderImageNode.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderImageNode.h>

using namespace H3D;

// initialize static member
H3DUtil::MutexLock ShaderImageNode::global_image_units_lock;
std::set<unsigned int> ShaderImageNode::global_image_units;
unsigned int ShaderImageNode::max_image_unit;


ShaderImageNode::ShaderImageNode( 
                                Inst< DisplayList > _displayList,
                                Inst< SFNode>  _metadata ) :
  H3DSingleTextureNode(_displayList,_metadata),
  reinitShaderImage( new Field ) {
  type_name = "ShaderImageNode";
  displayList->setName( "displayList" );
  displayList->setOwner( this );
  image_unit = -1;
  glGetIntegerv ( GL_MAX_IMAGE_SAMPLES_EXT, (GLint*)&max_image_unit );

}

ShaderImageNode::~ShaderImageNode ( ){
  if ( texture_id )
  {
    glDeleteTextures ( 1, &texture_id );
  }
}
