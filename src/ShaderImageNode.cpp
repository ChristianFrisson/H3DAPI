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
/// \brief CPP file for ShaderImageNode, X3D scene-graph node
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
  ShaderChildNode(_displayList,_metadata) {
  type_name = "ShaderImageNode";
  displayList->setName( "displayList" );
  displayList->setOwner( this );
  texture_id = 0;
  texture_unit = GL_TEXTURE0;
  image_unit = -1;
  glGetIntegerv ( GL_MAX_IMAGE_SAMPLES_EXT, (GLint*)&max_image_unit );
  //image_unit = generateImage ( );
  
}

void ShaderImageNode::preRender( GLenum texture_unit_id )
{
  // set the actual texture_unit will be used for this node
  texture_unit = texture_unit_id;
}

void ShaderImageNode::postRender()
{
  texture_unit = GL_TEXTURE0;
}

//void ShaderImageNode::prepareShaderImage ( ){
//  if ( !texture_id )
//  {// current texture id is zero , needs to create new texture object
//    glGenTextures ( 1, &texture_id );
//  }
//
//}

ShaderImageNode::~ShaderImageNode ( ){
  if ( image_unit!=-1 )
  {
    deleteImage ( image_unit );
  }
  if ( texture_id )
  {
    glDeleteTextures ( 1, &texture_id );
  }
}

unsigned int ShaderImageNode::generateImage ( ){

  global_image_units_lock.lock ( );
  set<unsigned int>::const_iterator it_end = global_image_units.end ( );
  if ( global_image_units.size()>=max_image_unit )
  {// all valid id for image units are used, return -1 represent invalid
    global_image_units_lock.unlock ( );
    return -1;
  }
  for ( size_t i = 0; i < max_image_unit; i++ )
  {
    if ( global_image_units.find ( i ) == it_end )
    {
      // no previous hit in global image units, use this one and insert it into the set
      global_image_units.insert ( i );
      global_image_units_lock.unlock ( );
      return i;
    }
  }
  // it should never goes here, return -1 just in case it does
  global_image_units_lock.unlock ( );
  return -1;
}

void ShaderImageNode::deleteImage ( unsigned int id ){
  global_image_units_lock.lock ( );
  
  set<unsigned int>::const_iterator it_end = global_image_units.end ( );

  if ( global_image_units.find ( id ) == it_end )
  {// erase the id if it exist, and actually it should exist if the id is a valid image id
    global_image_units.erase ( id );
    global_image_units_lock.unlock ( );
  }
  // the shader image you want to delete is not valid, release the lock anyway
  global_image_units_lock.unlock ();
}