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
/// \file AudioClip.cpp
/// \brief CPP file for AudioClip, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/AudioClip.h>
#include <H3D/H3DSoundFileNode.h>
#include <H3D/ResourceResolver.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase AudioClip::database( "AudioClip", 
                                     &(newInstance<AudioClip>),
                                     typeid( AudioClip ),
                                     &X3DSoundSourceNode::database );

namespace AudioClipInternals {
  FIELDDB_ELEMENT( AudioClip, url, INPUT_OUTPUT );
}


AudioClip::AudioClip( 
                     Inst< SFString>  _description,
                     Inst< SFBool  >  _loop,
                     Inst< SFNode  >  _metadata,
                     Inst< SFTime  >  _pauseTime,
                     Inst< SFFloat >  _pitch,
                     Inst< SFTime  >  _resumeTime,
                     Inst< StartTime  >  _startTime,
                     Inst< StopTime  >  _stopTime,
                     Inst< MFString>  _url,
                     Inst<  SFTime  >  _duration_changed,
                     Inst<  SFTime  >  _elapsedTime,
                     Inst<  SFBool  >  _isActive,
                     Inst<  SFBool  >  _isPaused,
                     Inst< TimeHandler > _time_handler ) :
  X3DSoundSourceNode( _metadata, _description, _loop,  
                      _pauseTime, _pitch, _resumeTime, 
                      _startTime, _stopTime, _duration_changed, 
                      _elapsedTime, _isActive, _isPaused, _time_handler ),
  X3DUrlObject( _url ) {

  type_name = "AudioClip";
  database.initFields( this );
  url->route( sound_buffer );

#ifndef HAVE_OPENAL
  Console(LogLevel::Error) << "Warning: H3D API compiled without OpenAL. AudioClip nodes"
             << " will be unusable." << endl;
#endif
}

void AudioClip::ALrender() {
#ifdef HAVE_OPENAL
  if( !al_buffers[0] ) {
    // Generate Buffers
    alGenBuffers(NR_STREAM_BUFFERS, al_buffers );
  }

  if( sound_buffer->hasCausedEvent( url ) ) {
    // url field has generated an event, so we have to unload the previous audio
    // and replace it with the new
    
    for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
         i != parent_sound_nodes.end(); ++i ) {
      alSourceStop( (*i)->getALSourceId() );
      alSourcei( (*i)->getALSourceId(), AL_BUFFER, 0 );
    }
    
    setURLUsed( "" );
    for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) {
      string _url = resolveURLAsFile( *i );
      if( _url != "" ) {
        H3DSoundFileNode *sf = H3DSoundFileNode::getSupportedFileReader( _url );
        if( sf ) {
          sf->load( _url );
          reader.reset( sf );
          setURLUsed( *i );
          break;
        }
      }
    }

    if( reader.get() ) { 
      if( reader->nrChannels() == 1 )
        if( reader->bitsPerSample() == 16 )
          al_format = AL_FORMAT_MONO16;
        else if( reader->bitsPerSample() == 8 ) 
          al_format = AL_FORMAT_MONO8;
        else {
          Console(LogLevel::Warning) << "Warning: Invalid sample width(" 
                     << reader->bitsPerSample()
                     << "Only 16 and 8 bit mono " 
                     << "audio clips supported. " << endl;
          reader.reset( NULL );
        }
      else if( reader->nrChannels() == 2 ) {
        if( reader->bitsPerSample() == 16 )
          al_format = AL_FORMAT_STEREO16;
        else if( reader->bitsPerSample() == 8 ) 
          al_format = AL_FORMAT_STEREO8;
        else {
          Console(LogLevel::Warning) << "Warning: Invalid sample width(" 
                     << reader->bitsPerSample()
                     << "Only 16 and 8 bit stereo " 
                     << "audio clips supported. " << endl;
          reader.reset( NULL );
        }
      } else {
        Console(LogLevel::Warning) << "Warning: Invalid number of channels(" 
                   << reader->nrChannels()
                   << "Only 1 and 2 bit channels are supported" << endl;
         reader.reset( NULL );
      }
    }

    if( reader.get() ) {
      duration_changed->setValue( reader->duration(), id );
      initALBuffers( NR_STREAM_BUFFERS * STREAM_BUFFER_SIZE <= 
                     reader->totalDataSize() );
      for( list< X3DSoundNode * >::iterator i = parent_sound_nodes.begin();
           i != parent_sound_nodes.end(); ++i ) {
        if( isActive->getValue() && !isPaused->getValue() )
          alSourcePlay( (*i)->getALSourceId() );
      }
    } else {
      duration_changed->setValue( -1, id );
      Console(LogLevel::Warning) << "Warning: None of the urls in the node \"" << getName() 
                 << "\" with url [";
      for( MFString::const_iterator i = url->begin(); 
           i != url->end(); ++i ) {  
        Console(LogLevel::Warning) << " \"" << *i << "\"";
      }
      Console(LogLevel::Warning) << "] could be loaded. No sound will be played "
                 << " from this AudioClip." << endl;
    }
  }
  X3DSoundSourceNode::ALrender();

    /*
    // Load test.wav
    alutLoadWAVFile("ding.wav",&format,&data,&size,&freq,&loop);
    if ((error = alGetError()) != AL_NO_ERROR) {
      alDeleteBuffers(1, &al_buffer);
      Console(LogLevel::Warning) << "FDAFAS" << endl;
      exit(-1);
    }
    // Copy test.wav data into AL Buffer 0
    alBufferData(al_buffer,format,data,size,freq);
    if ((error = alGetError()) != AL_NO_ERROR) {
      Console(LogLevel::Warning) << "FDASF" << endl;
      alDeleteBuffers(1, &al_buffer);
      exit(-1);
    }
    // Unload test.wav
    alutUnloadWAV(format,data,size,freq);
    if ((error = alGetError()) != AL_NO_ERROR) {
      Console(LogLevel::Warning) << "as" << endl;
      alDeleteBuffers(1, &al_buffer);
      exit(-1);
    }

    */
  
#endif
}

