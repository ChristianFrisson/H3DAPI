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
/// \file ShaderStorageBuffer.cpp
/// \brief CPP file for ShaderStorageBuffer, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderStorageBuffer.h>

using namespace H3D;

// initialize static member
H3DUtil::MutexLock ShaderStorageBuffer::global_block_bindings_lock;
std::set<unsigned int> ShaderStorageBuffer::global_block_bindings;
unsigned int ShaderStorageBuffer::max_block_bindings;


ShaderStorageBuffer::ShaderStorageBuffer( 
                                Inst< DisplayList > _displayList,
                                Inst< SFNode>  _metadata ) :
  ShaderChildNode(_displayList,_metadata) {
  type_name = "ShaderStorageBuffer";
  displayList->setName( "displayList" );
  displayList->setOwner( this );

  storage_block_binding = -1;
  buffer_id = -1;
  //image_unit = generateImage ( );
  
}

void ShaderStorageBuffer::initialize ( ){
  if ( !GLEW_ARB_shader_storage_buffer_object )
  {
    Console ( 4 ) << "No shader storage buffer object extenstion support in your system"
      << endl;
  }
  else{
    glGetIntegerv ( GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, (GLint*)max_block_bindings );
  }
}

void ShaderStorageBuffer::preRender( unsigned int program )
{
  if ( program!=0 )
  {
    program_handle = program;
  }
  else{
    Console ( 4 ) << "The program handle you specified is not valid shader program" << endl;
  }
}

void ShaderStorageBuffer::postRender()
{
  // texture_unit = GL_TEXTURE0;
}

void ShaderStorageBuffer::prepareStorageBuffer ( ){
  if ( buffer_id==-1 )
  {// buffer id is not generated yet
    glGenBuffers ( 1, (GLuint*)&buffer_id );
  }
  if ( storage_block_binding==-1 )
  {
    storage_block_binding = generateShaderStorageBinding ( );
  }
  glBindBuffer ( GL_SHADER_STORAGE_BUFFER, buffer_id );
  // As the data will be frequently modified by the shader and used for rendering
  // we use GL_DYNAMIC_COPY here
  glBufferData ( GL_SHADER_STORAGE_BUFFER, width->getValue ( )*height->getValue ( )*depth->getValue ( ),
    NULL, GL_DYNAMIC_COPY ); 

  storage_block_index = glGetProgramResourceIndex ( program_handle, GL_SHADER_STORAGE_BLOCK,
    (GLchar*)name->getValue ( ).c_str ( ) );
  if ( storage_block_index == GL_INVALID_INDEX )
  {
    Console ( 4 ) << "Warning[ShaderStorageBuffer]:"
      << "There is no active shader storage block named as " << name->getValue ( ) << endl;
    return;
  }
  // bind the assigned storage block index to the storage block binding point for the shader storage buffer
  glShaderStorageBlockBinding ( program_handle, storage_block_index, storage_block_binding );
}

void ShaderStorageBuffer::render ( ){
  if ( buffer_id == -1 || displayList->hasCausedEvent(width)
    || displayList->hasCausedEvent(height)||displayList->hasCausedEvent(depth))
  {// either it is the first that the buffer is used, or the size need to be changed
    prepareStorageBuffer ( );
  }
  // setup barrier to ensure the previous read/write to the storage buffer is finished
  glMemoryBarrier ( GL_SHADER_STORAGE_BARRIER_BIT );
  // bind the actual buffer to the storage block binding on storage buffer
  glBindBufferBase ( GL_SHADER_STORAGE_BUFFER, storage_block_binding, buffer_id );
}

ShaderStorageBuffer::~ShaderStorageBuffer ( ){
  if ( storage_block_binding!=-1 )
  {// there is valid storage block binding
    deleteShaderStorageBinding ( storage_block_binding );
  }
  if ( buffer_id!=-1 )
  {
    glDeleteBuffers ( 1, (GLuint*)&buffer_id );
  }
}

int ShaderStorageBuffer::generateShaderStorageBinding ( ){
  global_block_bindings_lock.lock ( );
  set<unsigned int>::const_iterator it_end = global_block_bindings.end ( );
  if ( global_block_bindings.size()>=max_block_bindings )
  {
    global_block_bindings_lock.unlock ( );
    Console ( 4 ) << "The global block binding points are used up, the maximum number"
      << " of binding points is: " << max_block_bindings <<"."<<endl;
    return -1;
  }
  for ( size_t i = 0; i < max_block_bindings; i++ )
  {
    if ( global_block_bindings.find(i) == it_end )
    {
      global_block_bindings.insert ( i );
      global_block_bindings_lock.unlock ( );
      return i;
    }
  }
  global_block_bindings_lock.unlock ( );
  return -1;
}

void ShaderStorageBuffer::deleteShaderStorageBinding ( int binding ){
  global_block_bindings_lock.lock ( );
  set<unsigned int>::const_iterator it_end = global_block_bindings.end ( );
  if ( global_block_bindings.find( binding ) == it_end )
  {
    global_block_bindings.erase(binding);
    global_block_bindings_lock.unlock ( );
  }
  global_block_bindings_lock.unlock ( );
}