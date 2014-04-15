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
/// \file AudioClip.h
/// \brief Header file for AudioClip, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __AUDIOCLIP_H__
#define __AUDIOCLIP_H__

#include <H3D/X3DSoundSourceNode.h>
#include <H3D/X3DUrlObject.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class AudioClip
  /// \brief An AudioClip node specifies audio data that can 
  /// be referenced by Sound nodes. 
  ///
  /// The description field specifies a textual description of the audio
  /// source. A browser is not required to display the description field but
  /// may choose to do so in addition to playing the sound. 
  ///
  /// The url field specifies the URL from which the sound is
  /// loaded. 
  ///
  /// The loop, pauseTime, resumeTime, startTime, and stopTime inputOutput
  /// fields and the elapsedTime, isActive, and isPaused outputOnly fields,
  /// and their effects on the AudioClip node, are discussed in detail in 8
  /// Time component. The "cycle" of an AudioClip is the length of time in
  /// seconds for one playing of the audio at the specified pitch. 
  ///
  /// The pitch field specifies a multiplier for the rate at which sampled
  /// sound is played. Values for the pitch field shall be greater than
  /// zero. Changing the pitch field affects both the pitch and playback
  /// speed of a sound. A set_pitch event to an active AudioClip is ignored
  /// and no pitch_changed field is generated. If pitch is set to 2.0, the
  /// sound shall be played one octave higher than normal and played twice
  /// as fast. For a sampled sound, the pitch field alters the sampling rate
  /// at which the sound is played. 
  ///
  /// A duration_changed event is sent whenever there is a new value for the
  /// "normal" duration of the clip. Typically, this will only occur when
  /// the current url in use changes and the sound data has been loaded,
  /// indicating that the clip is playing a different sound source. The
  /// duration is the length of time in seconds for one cycle of the audio
  /// for a pitch set to 1.0. Changing the pitch field will not trigger a
  /// duration_changed event. A duration value of "-1" implies that the
  /// sound data has not yet loaded or the value is unavailable for some
  /// reason. A duration_changed event shall be generated if the AudioClip
  /// node is loaded when the X3D file is read or the AudioClip node is
  /// added to the scene graph. 
  ///
  /// The isActive field may be used by other nodes to determine if the clip
  /// is currently active. If an AudioClip is active, it shall be playing
  /// the sound corresponding to the sound time (i.e., in the sound's local
  /// time system with sample 0 at time 0): 
  ///
  ///    t = (now - startTime) modulo (duration / pitch)
  /// 
  ///
  /// \par Internal routes:
  /// \dotfile AudioClip.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/AudioClip.x3d">AudioClip.x3d</a>
  ///     ( <a href="examples/AudioClip.x3d.html">Source</a> )
  class H3DAPI_API AudioClip : 
    public X3DSoundSourceNode,
    public X3DUrlObject {
  public:
    /// Constructor.
    AudioClip( Inst< SFString>  _description      = 0,
               Inst< SFBool  >  _loop             = 0,
               Inst< SFNode  >  _metadata         = 0,
               Inst< SFTime  >  _pauseTime        = 0,
               Inst< SFFloat >  _pitch            = 0,
               Inst< SFTime  >  _resumeTime       = 0,
               Inst< StartTime  >  _startTime        = 0,
               Inst< StopTime  >  _stopTime         = 0,
               Inst< MFString>  _url              = 0,
               Inst<  SFTime  >  _duration_changed = 0,
               Inst<  SFTime  >  _elapsedTime      = 0,
               Inst<  SFBool  >  _isActive         = 0,
               Inst<  SFBool  >  _isPaused         = 0,
               Inst< TimeHandler > _time_handler   = 0 );

    /// Perform the OpenAL calls to render the audio clip.
    virtual void ALrender();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
