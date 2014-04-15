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
/// \file X3DSoundSourceNode.cpp
/// \brief CPP file for X3DSoundSourceNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DSoundSourceNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DSoundSourceNode::database( 
        "X3DSoundSourceNode", 
        NULL,
        typeid( X3DSoundSourceNode ),
        &X3DTimeDependentNode::database );

namespace X3DSoundSourceNodeInternals {
  FIELDDB_ELEMENT( X3DSoundSourceNode, description, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DSoundSourceNode, pitch, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DSoundSourceNode, duration_changed, OUTPUT_ONLY );
}

X3DSoundSourceNode::X3DSoundSourceNode( Inst< SFNode  >  _metadata,
                                        Inst< SFString>  _description,
                                        Inst< SFBool  >  _loop,
                                        Inst< SFTime  >  _pauseTime,
                                        Inst< SFFloat >  _pitch,
                                        Inst< SFTime  >  _resumeTime,
                                        Inst< StartTime  >  _startTime,
                                        Inst< StopTime  >  _stopTime,
                                        Inst<  SFTime  >  _duration_changed,
                                        Inst<  SFTime  >  _elapsedTime,
                                        Inst<  SFBool  >  _isActive,
                                        Inst<  SFBool  >  _isPaused,
                                        Inst< TimeHandler > _time_handler ) :
  X3DTimeDependentNode( _metadata, _loop, _pauseTime, _resumeTime, _startTime,
                        _stopTime, _elapsedTime, _isActive, _isPaused,
                        _time_handler ),
  description     ( _description      ),
  pitch           ( _pitch            ),
  duration_changed( _duration_changed ),
  sound_buffer( new ALSoundBuffer ) {
  type_name = "X3DSoundSourceNode";
  database.initFields( this );
#ifdef HAVE_OPENAL
  al_buffers[0] = 0;
#endif
  sound_buffer->setName( "soundBuffer" );
  sound_buffer->setOwner( this );

  description->setValue( "" );
  pitch->setValue( 1.0 );
  duration_changed->setValue( 0, id );

  pitch->route( sound_buffer );
  loop->route( sound_buffer );
}

X3DSoundSourceNode::~X3DSoundSourceNode() {
#ifdef HAVE_OPENAL
  if( al_buffers ) {
    alDeleteBuffers( NR_STREAM_BUFFERS, al_buffers );
  }
#endif
}

bool X3DSoundSourceNode::initALBuffers( bool stream ) {
#ifdef HAVE_OPENAL
  if( reader.get() ) reader->reset();
  else return false;

  if( !stream ) {
    sound_as_stream = false;
    char *buffer = new char[ reader->totalDataSize() ];
    reader->read( buffer, reader->totalDataSize() );
    alBufferData( al_buffers[0], al_format, 
                  buffer, reader->totalDataSize(), 
                  reader->samplesPerSecond() );
    delete[] buffer;
    for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
         i != parent_sound_nodes.end(); ++i ) {
      if( loop->getValue() )
        alSourcei( (*i)->getALSourceId(), AL_LOOPING, AL_TRUE );
      else
        alSourcei( (*i)->getALSourceId(), AL_LOOPING, AL_FALSE );
      alSourcei( (*i)->getALSourceId(), AL_BUFFER, 0 );
      alSourcei( (*i)->getALSourceId(), AL_BUFFER, al_buffers[0] );
    }
  } else {
    sound_as_stream = true;
    char *buffer = new char[ STREAM_BUFFER_SIZE ];
    for( unsigned int i = 0; i < NR_STREAM_BUFFERS; ++i ) {
      unsigned int bytes_read = reader->read( buffer, STREAM_BUFFER_SIZE );
      alBufferData( al_buffers[i], al_format, 
                    buffer, bytes_read, 
                    reader->samplesPerSecond() );
    }
    delete[] buffer;
    
    for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
         i != parent_sound_nodes.end(); ++i ) {
      alSourcei( (*i)->getALSourceId(), AL_LOOPING, AL_FALSE );
      alSourcei( (*i)->getALSourceId(), AL_BUFFER, 0 );
      alSourceQueueBuffers( (*i)->getALSourceId(), NR_STREAM_BUFFERS, 
                            al_buffers );  
    }
  }
  
  return true;
#else
  return false;
#endif
}

void X3DSoundSourceNode::onStart() {
  X3DTimeDependentNode::onStart();
#ifdef HAVE_OPENAL
  sound_buffer->upToDate();

  if( initALBuffers( sound_as_stream ) ) {
    for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
         i != parent_sound_nodes.end(); ++i ) {
      alSourcei( (*i)->getALSourceId(), AL_BUFFER, 0 );
      if( !sound_as_stream ) {
        alSourcei( (*i)->getALSourceId(), AL_BUFFER, al_buffers[0] );
      } else {
        alSourceQueueBuffers( (*i)->getALSourceId(), NR_STREAM_BUFFERS, 
                              al_buffers );  
      }
      alSourcePlay( (*i)->getALSourceId() );  
    }
  }
#endif
}

void X3DSoundSourceNode::onStop() {
  X3DTimeDependentNode::onStop();
  
#ifdef HAVE_OPENAL
  for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
       i != parent_sound_nodes.end(); ++i ) {
    alSourceStop( (*i)->getALSourceId() );  
    alSourcei( (*i)->getALSourceId(), AL_BUFFER, 0 );
  }

  if( reader.get() ) reader->reset();
#endif
}

void X3DSoundSourceNode::onPause() {
  X3DTimeDependentNode::onPause();
#ifdef HAVE_OPENAL
  for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
       i != parent_sound_nodes.end(); ++i ) {
    alSourcePause( (*i)->getALSourceId() );  
  }
#endif
}

void X3DSoundSourceNode::onResume() {
  X3DTimeDependentNode::onResume();
#ifdef HAVE_OPENAL
  for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
       i != parent_sound_nodes.end(); ++i ) {
    alSourcePlay( (*i)->getALSourceId() );  
  }
#endif
}


void X3DSoundSourceNode::TimeHandler::update() {
  H3DTime time = static_cast< SFTime * >( event.ptr )->getValue();
  X3DSoundSourceNode *sound_source = 
    static_cast< X3DSoundSourceNode * >( getOwner() );
  X3DTimeDependentNode::TimeHandler::update();

#ifdef HAVE_OPENAL
  if( sound_source->isActive->getValue() &&
      !sound_source->isPaused->getValue() ) { 
    if( !sound_source->sound_as_stream ) {
      ALint state;
      bool playing = false;
      for( list< X3DSoundNode * >::iterator i = sound_source->parent_sound_nodes.begin();
           i != sound_source->parent_sound_nodes.end(); ++i ) {
        alGetSourcei( (*i)->getALSourceId(), AL_SOURCE_STATE, &state );
        if( state == AL_PLAYING ) playing = true;
      }
      if( !playing ) {
        deactivate( time );
      }
    } else {
      ALint processed = 0;
      if( sound_source->parent_sound_nodes.size() > 0 ) {
        alGetSourcei( sound_source->parent_sound_nodes.front()->getALSourceId(), 
                      AL_BUFFERS_PROCESSED, &processed );
      }

      bool have_buffers = false;
      for( list< X3DSoundNode * >::iterator i = sound_source->parent_sound_nodes.begin();
           i != sound_source->parent_sound_nodes.end(); ++i ) {
        ALint processed_in_src;
        ALint in_queue;
        alGetSourcei( (*i)->getALSourceId(), AL_BUFFERS_PROCESSED, &processed_in_src );
        alGetSourcei( (*i)->getALSourceId(), AL_BUFFERS_QUEUED, &in_queue );
        if( processed_in_src < processed )
          processed = processed_in_src;
        
        if( in_queue > 0 ) have_buffers = true;
      }
      
      if( !have_buffers ) {
        deactivate( time );
      } else {
        if( sound_source->reader.get() && processed > 0 ) {
          // All sound souces have processed at least one buffer
          ALuint buffer_id;
        
          // Unqueue from all sound sources
          for( list< X3DSoundNode * >::iterator i =
                 sound_source->parent_sound_nodes.begin();
               i != sound_source->parent_sound_nodes.end(); ++i ) {
            alSourceUnqueueBuffers( (*i)->getALSourceId(), 1, &buffer_id);
          }
        
          // Update buffer.
          char buffer[ STREAM_BUFFER_SIZE ];
          unsigned int bytes_read = sound_source->reader->read( buffer, STREAM_BUFFER_SIZE );
          if( bytes_read < STREAM_BUFFER_SIZE ) {
            alBufferData( buffer_id, sound_source->al_format, 
                          buffer, bytes_read , 
                          sound_source->reader->samplesPerSecond() );
            if( sound_source->loop->getValue() ) {
              sound_source->reader->reset();
            } 
          } else {
            alBufferData( buffer_id, sound_source->al_format, 
                          buffer, STREAM_BUFFER_SIZE, 
                          sound_source->reader->samplesPerSecond() );
          }
          
          ALint bsize;
          alGetBufferi( buffer_id, AL_SIZE, &bsize );

          if( bsize > 0 ) {
            // Queue the buffer on all sound sources.
            for( list< X3DSoundNode * >::iterator i =
                   sound_source->parent_sound_nodes.begin();
                 i != sound_source->parent_sound_nodes.end(); ++i ) {
              alSourceQueueBuffers((*i)->getALSourceId(), 1, 
                                   &buffer_id );  
              ALint state;
              alGetSourcei( (*i)->getALSourceId(), AL_SOURCE_STATE, &state );
              if( sound_source->isActive->getValue() &&
                  !sound_source->isPaused->getValue() &&
                  state != AL_PLAYING ) {
                alSourcePlay( (*i)->getALSourceId() );
              }
            }
          }
        }
      }
    }
  }
  #endif
}

void X3DSoundSourceNode::ALrender() {
#ifdef HAVE_OPENAL
  if( !al_buffers[0] ) {
    // Generate Buffers
    alGenBuffers(NR_STREAM_BUFFERS, al_buffers );
  }

  for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
       i != parent_sound_nodes.end(); ++i ) {
    if( !sound_as_stream ) {
      if( loop->getValue() )
        alSourcei( (*i)->getALSourceId(), AL_LOOPING, AL_TRUE );
      else
        alSourcei( (*i)->getALSourceId(), AL_LOOPING, AL_FALSE );
    }
    alSourcef( (*i)->getALSourceId(), AL_PITCH, 
               pitch->getValue() );
  }
#endif
}


    /// Register this sound node with the X3DSoundSourceNode. All 
    /// X3DSoundSourceNodes that want to play this sound source must
    /// register with this function.
void X3DSoundSourceNode::registerSoundNode( X3DSoundNode *n ) {
#ifdef HAVE_OPENAL
  sound_buffer->upToDate();
  if( !sound_as_stream ) {
    alSourcei( n->getALSourceId(), AL_BUFFER, al_buffers[0] );
    if( loop->getValue() )
      alSourcei( n->getALSourceId(), AL_LOOPING, AL_TRUE );
    else
      alSourcei( n->getALSourceId(), AL_LOOPING, AL_FALSE );
  } else {
    alSourceQueueBuffers( n->getALSourceId(), NR_STREAM_BUFFERS, 
                          al_buffers );  
  }
  alSourcef( n->getALSourceId(), AL_PITCH, 
             pitch->getValue() );
  if( isActive->getValue() && !isPaused->getValue() )
    alSourcePlay( n->getALSourceId() );
#endif
  parent_sound_nodes.push_front( n );
}

/// Unregister this sound node with the X3DSoundSourceNode.   
void X3DSoundSourceNode::unregisterSoundNode( X3DSoundNode *n ) {
#ifdef HAVE_OPENAL
  alSourceStop( n->getALSourceId() );  
  alSourcei( n->getALSourceId(), AL_BUFFER, 0 );
#endif
  parent_sound_nodes.remove( n );
}
