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
/// \file X3DSoundSourceNode.h
/// \brief Header file for X3DSoundSourceNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSOUNDSOURCENODE_H__
#define __X3DSOUNDSOURCENODE_H__

#include <H3D/X3DTimeDependentNode.h>
#include <H3D/X3DSoundNode.h>
#include <H3D/PeriodicUpdate.h>
#include <H3D/H3DSoundStreamNode.h>
#include <list>
#include <fstream>

#ifdef HAVE_OPENAL
#if defined( MACOSX )
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#endif 

#define NR_STREAM_BUFFERS 3
#define STREAM_BUFFER_SIZE (4096*8)

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DSoundSourceNode
  /// \brief This abstract node type is used to derive node types that can emit
  /// audio data. 
  ///
  /// \par Internal routes:
  /// \dotfile X3DSoundSourceNode.dot
  class H3DAPI_API X3DSoundSourceNode : public X3DTimeDependentNode {
  public:
    /// TimeHandler is extended to stream audio data to OpenAL buffers
    /// if the sound source is a streaming source.
    class H3DAPI_API TimeHandler: public X3DTimeDependentNode::TimeHandler {
    public:
      /// Constructor.
      TimeHandler() {}
    protected:
      virtual void update();
    };
#ifdef __BORLANDC__
    friend class TimeHandler;
#endif

    /// Field that calls ALrender() when a field routed to it
    /// has generated an event.
    class H3DAPI_API ALSoundBuffer: 
      public PeriodicUpdate< Field > {
    protected:
      virtual void update() {
        X3DSoundSourceNode *s = 
          static_cast< X3DSoundSourceNode * >( getOwner());
        s->ALrender();
        event_fields.clear();
      }

      /// When the event is propagated the display list is invalidated.
      virtual void propagateEvent( Event e ) {
        Field::propagateEvent( e );
        event_fields.insert( e.ptr );
      }
    public:

      /// Returns true if the Field given has generated an event to this
      /// field since the last call to the update() function.
      inline bool hasCausedEvent( Field *f ) {
        return event_fields.find( f ) != event_fields.end();
      }

      /// Returns true if the Field given has generated an event to this
      /// field since the last call to the update() function.
      template< class FieldType >
      inline bool hasCausedEvent( auto_ptr< FieldType > &f ) {
        return hasCausedEvent( f.get() );
      }

    protected:
      /// The fields that has generated an event since the last call to
      /// update()
      set< Field * > event_fields;
    };


    /// Constructor.
    X3DSoundSourceNode( Inst< SFNode   > _metadata         = 0,
                        Inst< SFString > _description      = 0,
                        Inst< SFBool   > _loop             = 0,
                        Inst< SFTime   > _pauseTime        = 0,
                        Inst< SFFloat  > _pitch            = 0,
                        Inst< SFTime   > _resumeTime       = 0,
                        Inst< StartTime   > _startTime        = 0,
                        Inst< StopTime   > _stopTime         = 0,
                        Inst< SFTime   > _duration_changed = 0,
                        Inst< SFTime   > _elapsedTime      = 0,
                        Inst< SFBool   > _isActive         = 0,
                        Inst< SFBool   > _isPaused         = 0,
                        Inst< TimeHandler > _timeHandler   = 0 );

    virtual ~X3DSoundSourceNode();

    /// Reads new data into the OpenAL from the reader and attaches them
    /// to the sound sources that use this sound source. The stream param 
    /// indicates if it should init a stream source or a static source.
    virtual bool initALBuffers( bool stream );

    /// Perform the OpenAL calls to render the sound.
    virtual void ALrender();

    /// Returns the default xml containerField attribute value.
    /// For this node it is "children".
    virtual string defaultXMLContainerField() {
      return "source";
    }

    /// Register this sound node with the X3DSoundSourceNode. All 
    /// X3DSoundSourceNodes that want to play this sound source must
    /// register with this function.
    virtual void registerSoundNode( X3DSoundNode *n );

    /// Unregister this sound node with the X3DSoundSourceNode.   
    virtual void unregisterSoundNode( X3DSoundNode *n );

    /// On pause, pause the sound.
    virtual void onPause();

    /// On resume, resume paused sound.
    virtual void onResume();

    /// On start, initialize AL buffers and start playing.
    virtual void onStart();

    /// On stop, stop playing the sound.
    virtual void onStop();

    /// The description field specifies a textual description of the audio
    /// source. 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    ///
    /// \dotfile X3DSoundSourceNode_description.dot 
    auto_ptr< SFString > description;

    /// The pitch field specifies a multiplier for the rate at which sampled
    /// sound is played. Values for the pitch field shall be greater than
    /// zero.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    ///
    /// \dotfile X3DSoundSourceNode_pitch.dot 
    auto_ptr< SFFloat  > pitch;

    /// The duration field is the length of time in seconds for one cycle 
    /// of the audio for a pitch set to 1.0.
    /// 
    /// <b>Access type:</b> outputOnly
    ///
    /// \dotfile X3DSoundSourceNode_duration_changed.dot 
    auto_ptr< SFTime   > duration_changed;

     /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The X3DSoundNodes that use this node as sound source.
    list< X3DSoundNode * > parent_sound_nodes;

#ifdef HAVE_OPENAL
    /// OpenAL buffer ids used for playing sound.
    ALuint al_buffers[NR_STREAM_BUFFERS];

    /// The OpenAL format of the current sound source.
    ALenum al_format;
#endif

    /// The H3DSoundStreamNode used to read sound data.
    AutoRef< H3DSoundStreamNode > reader;

    /// Indicates if the current sounds source is being streamed or not.
    bool sound_as_stream;

    /// Field that calls ALrender() when a field routed to it
    /// has generated an event.
    auto_ptr< ALSoundBuffer > sound_buffer;
 };
}

#endif
