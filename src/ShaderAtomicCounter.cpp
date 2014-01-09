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
/// \file ShaderAtomicCounter.cpp
/// \brief CPP file for ShaderAtomicCounter, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderAtomicCounter.h>

using namespace H3D;

H3DNodeDatabase ShaderAtomicCounter::database ( "ShaderAtomicCounter",
  &(newInstance<ShaderAtomicCounter>),
  typeid(ShaderAtomicCounter),
  &(ShaderChildNode::database) );


namespace ShaderStorageBufferInternals{
  FIELDDB_ELEMENT( ShaderAtomicCounter, initialValue, INPUT_OUTPUT);
}

ShaderAtomicCounter::ShaderAtomicCounter( 
                                Inst< DisplayList   > _displayList,
                                Inst< SFNode        > _metadata,
                                Inst< SFInt32       > _initialValue ) :
  ShaderChildNode(_displayList,_metadata),
  initialValue(_initialValue){
  type_name = "ShaderAtomicCounter";
  database.initFields(this);
  displayList->setName( "displayList" );
  displayList->setOwner( this );
  buffer_id = -1;
  initialValue->setValue(0);
}

void ShaderAtomicCounter::initialize ( ){
  if ( !GLEW_ARB_shader_atomic_counters )
  {
    Console ( 4 ) << "No shader atomic counters extension support in your system"
      << endl;
  }
}

void ShaderAtomicCounter::preRender( unsigned int program )
{
  if ( program!=0 )
  {
    program_handle = program;
  }
  else{
    Console ( 4 ) << "The program handle you specified is not valid shader program" << endl;
  }
}

void ShaderAtomicCounter::postRender()
{
  // texture_unit = GL_TEXTURE0;
}

void ShaderAtomicCounter::prepareAtomicCounterBuffer(){
  if( buffer_id == -1 ) {
    glGenBuffers(1, (GLuint *)&buffer_id);
  }
  glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, buffer_id );
  // assume buffer size if one here temporarily, it can be more than one of course
  glBufferData( GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned int), NULL, GL_DYNAMIC_COPY );
  // the second parameter here refers to the index of an active atomic counter buffer
  // it has to being in range from zero to GL_ACTIVE_ATOMIC_COUNTER_BUFFERS minus one
  // as i am not actually not very sure how to make the multiple atomic counter buffers
  // work for one shader program, so, it is set to zero here temporarily.
  glGetActiveAtomicCounterBufferiv( program_handle, 0, GL_ATOMIC_COUNTER_BUFFER_BINDING, (GLint*)&atomic_counter_binding );
  if( atomic_counter_binding==GL_INVALID_VALUE ) {
    Console(4)<<"program is invalid or the bufferIndex do not exist for the buffer!"<<endl;
  }else if( atomic_counter_binding==GL_INVALID_ENUM ) {
    Console(4)<<"parameter token is not accepted!"<<endl;
  }
}

void ShaderAtomicCounter::render ( ){
  //if ( buffer_id == -1 )
  {// either it is the first that the buffer is used, or the size need to be changed
    prepareAtomicCounterBuffer();
  }
  // setup barrier to ensure the previous read/write to the storage buffer is finished
  glMemoryBarrier ( GL_ATOMIC_COUNTER_BARRIER_BIT );
  // bind the actual buffer to the storage block binding on storage buffer
  // so shader program will have access to the buffer
  glBindBufferBase ( GL_ATOMIC_COUNTER_BUFFER, atomic_counter_binding, buffer_id );
  static const unsigned int zero = 0;
  glBufferSubData( GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero );
}

ShaderAtomicCounter::~ShaderAtomicCounter ( ){
  if ( buffer_id!=-1 )
  {
    glDeleteBuffers ( 1, (GLuint*)&buffer_id );
  }
}