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
/// \brief CPP file for ShaderAtomicCounter.
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
    prev_travinfoadr = NULL;
}

void ShaderAtomicCounter::initialize ( ){
#ifdef GLEW_ARB_shader_atomic_counters
  if ( !GLEW_ARB_shader_atomic_counters )
  {
    Console(LogLevel::Error) << "No shader atomic counters extension support in your system"
      << endl;
  }
#endif
}

void ShaderAtomicCounter::preRender( unsigned int program )
{
  if ( program!=0 )
  {
    program_handle = program;
  }
}

void ShaderAtomicCounter::postRender()
{
  // texture_unit = GL_TEXTURE0;
}

void ShaderAtomicCounter::prepareAtomicCounterBuffer(){
#ifdef GLEW_ARB_shader_atomic_counters
  if( buffer_id == -1 ) {
    glGenBuffers(1, (GLuint *)&buffer_id);
  }
  glBindBuffer( GL_ATOMIC_COUNTER_BUFFER, buffer_id );
  // assume buffer size if one here temporarily, it can be more than one of course
  glBufferData( GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned int), NULL, GL_DYNAMIC_COPY );
#endif
}

void ShaderAtomicCounter::render ( ){
#ifdef GLEW_ARB_shader_atomic_counters
  if ( !glIsProgram(program_handle) )
  {
    // program_handle is not correctly compiled, no need to render 
    return;
  }
  if ( buffer_id == -1 )
  {// either it is the first that the buffer is used, or the size need to be changed
    prepareAtomicCounterBuffer();
  }
#ifdef DEBUG_GL_ERROR
  GLenum err = glGetError();
#endif
  // the second parameter here refers to the index of an active atomic counter buffer
  // it has to being in range from zero to GL_ACTIVE_ATOMIC_COUNTER_BUFFERS minus one
  // as i am not actually not very sure how to make the multiple atomic counter buffers
  // work for one shader program, so, it is set to zero here temporarily.
  //Console(LogLevel::Error)<<"program_handle is: "<<program_handle<<endl;
  glGetActiveAtomicCounterBufferiv( program_handle, 0, GL_ATOMIC_COUNTER_BUFFER_BINDING, (GLint*)&atomic_counter_binding );
#ifdef DEBUG_GL_ERROR
  err = glGetError();
  if( err!=GL_NO_ERROR ) {
    Console(LogLevel::Error)<<"error happens when getting atomic counter buffer: "<<gluErrorString(err)<<endl;
  }
#endif
  if( atomic_counter_binding==GL_INVALID_VALUE ) {
    Console(LogLevel::Error)<<"program is invalid or the bufferIndex do not exist for the buffer!"<<endl;
  }else if( atomic_counter_binding==GL_INVALID_ENUM ) {
    Console(LogLevel::Error)<<"parameter token is not accepted!"<<endl;
  }

  // setup barrier to ensure the previous read/write to the storage buffer is finished
  glMemoryBarrier ( GL_ATOMIC_COUNTER_BARRIER_BIT );
  // bind the actual buffer to the storage block binding on storage buffer
  // so shader program will have access to the buffer
#ifdef DEBUG_GL_ERROR
  err = glGetError();
  if( err!=GL_NO_ERROR ) {
    Console(LogLevel::Error)<<"error happens when set memory barrier: "<<gluErrorString(err)<<endl;
  }
#endif
  glBindBufferBase ( GL_ATOMIC_COUNTER_BUFFER, atomic_counter_binding, buffer_id );
#ifdef DEBUG_GL_ERROR
  err = glGetError();
  if( err!=GL_NO_ERROR ) {
    Console(LogLevel::Error)<<"error happens when bind buffer base: "<<gluErrorString(err)<<endl;
  }
  //glBufferSubData( GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero );
#endif
#endif
}

void ShaderAtomicCounter::traverseSG( TraverseInfo &ti ){
#ifdef GLEW_ARB_shader_atomic_counters

  if( prev_travinfoadr != &ti){
#ifdef DEBUG_GL_ERROR
    GLenum err = glGetError();
#endif
    if ( buffer_id == -1 )
    {// either it is the first that the buffer is used, or the size need to be changed
      prepareAtomicCounterBuffer();
    }
    // initialized once only per one traverse
    glBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, atomic_counter_binding, buffer_id );
#ifdef DEBUG_GL_ERROR
    err = glGetError();
    if( err!=GL_NO_ERROR ) {
      Console(LogLevel::Error)<<"error happens when binding the atomic counter : "<<gluErrorString(err)<<endl;
    }
#endif
    static const unsigned int initial_value = (unsigned int)initialValue->getValue();
    //static const unsigned int zero = 0;
    glBufferSubData( GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(initial_value),&initial_value); 
#ifdef DEBUG_GL_ERROR
    err = glGetError();
    if( err!=GL_NO_ERROR ) {
      Console(LogLevel::Error)<<"error happens when initializing the atomic counter value: "<<gluErrorString(err)<<endl;
    }
#endif
    prev_travinfoadr = &ti;
  }
#endif
}

ShaderAtomicCounter::~ShaderAtomicCounter ( ){
  if ( buffer_id!=-1 )
  {
    glDeleteBuffers ( 1, (GLuint*)&buffer_id );
  }
}
