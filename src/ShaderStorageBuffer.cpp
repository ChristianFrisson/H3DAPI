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
/// \brief CPP file for ShaderStorageBuffer.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderStorageBuffer.h>

using namespace H3D;

H3DNodeDatabase ShaderStorageBuffer::database ( "ShaderStorageBuffer",
  &(newInstance<ShaderStorageBuffer>),
  typeid(ShaderStorageBuffer),
  &(ShaderChildNode::database) );

// initialize static member
H3DUtil::MutexLock ShaderStorageBuffer::global_block_bindings_lock;
std::set<unsigned int> ShaderStorageBuffer::global_block_bindings;
unsigned int ShaderStorageBuffer::max_block_bindings;

namespace ShaderStorageBufferInternals{
  FIELDDB_ELEMENT ( ShaderStorageBuffer, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderStorageBuffer, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderStorageBuffer, depth, INPUT_OUTPUT  ); 
  FIELDDB_ELEMENT ( ShaderStorageBuffer, storageName, INPUT_OUTPUT );
}

ShaderStorageBuffer::ShaderStorageBuffer( 
                                Inst< DisplayList   > _displayList,
                                Inst< SFNode        > _metadata,
                                Inst< SFInt32       > _width,
                                Inst< SFInt32       > _height,
                                Inst< SFInt32       > _depth,
                                Inst< SFString      > _storageName ) :
  ShaderChildNode(_displayList,_metadata),
  width(_width),
  height(_height),
  depth(_depth),
  storageName(_storageName){
  type_name = "ShaderStorageBuffer";
  database.initFields(this);
  displayList->setName( "displayList" );
  displayList->setOwner( this );
  width->setValue ( 512 );
  height->setValue ( 512 );
  depth->setValue ( 16 );
  storage_block_binding = -1;
  buffer_id = -1;
  storageName->setValue(getName());
}

void ShaderStorageBuffer::initialize ( ){
#ifdef GLEW_ARB_shader_storage_buffer_object
  if ( !GLEW_ARB_shader_storage_buffer_object )
  {
    Console(LogLevel::Error) << "No shader storage buffer object extension support in your system"
      << endl;
  }
  else{
    glGetIntegerv ( GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, (GLint*)&max_block_bindings );
  }
#else
    Console(LogLevel::Error) << "Binary compiled without support for shader storage buffered object extension."
      << endl;
#endif
}


void ShaderStorageBuffer::preRender( unsigned int program )
{
#ifdef GLEW_ARB_shader_storage_buffer_object
  if ( program!=0 )
  {
    program_handle = program;
  }
#endif
}

void ShaderStorageBuffer::prepareStorageBuffer ( ){
#ifdef GLEW_ARB_shader_storage_buffer_object
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
#endif
}

void ShaderStorageBuffer::render ( ){
#ifdef GLEW_ARB_shader_storage_buffer_object
  if ( !glIsProgram(program_handle) )
  {
    // program_handle is not correctly compiled, no need to render  
    return;
  }
  if ( buffer_id == -1 || displayList->hasCausedEvent(width)
    || displayList->hasCausedEvent(height)||displayList->hasCausedEvent(depth))
  {// either it is the first that the buffer is used, or the size need to be changed
    prepareStorageBuffer ( );
  }
  storage_block_index = glGetProgramResourceIndex ( program_handle, GL_SHADER_STORAGE_BLOCK, storageName->getValue().c_str() );
  if ( storage_block_index == GL_INVALID_INDEX )
  {
    Console(LogLevel::Error) << "Warning[ShaderStorageBuffer]:"
      << "There is no active shader storage block named as " << storageName->getValue() << endl;
    return;
  }
  // bind the assigned storage block index to the storage block binding point for the shader storage buffer
  glShaderStorageBlockBinding ( program_handle, storage_block_index, storage_block_binding );

  // setup barrier to ensure the previous read/write to the storage buffer is finished
  glMemoryBarrier ( GL_SHADER_STORAGE_BARRIER_BIT );
  // bind the actual buffer to the storage block binding on storage buffer
  // so shader program will have access to the buffer
  glBindBufferBase ( GL_SHADER_STORAGE_BUFFER, storage_block_binding, buffer_id );
#endif
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
    Console(LogLevel::Error) << "The global block binding points are used up, the maximum number"
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
