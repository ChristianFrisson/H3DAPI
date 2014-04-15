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
/// \file SimpleAudioClip.h
/// \brief Header file for SimpleAudioClip.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SIMPLEAUDIOCLIP_H__
#define __SIMPLEAUDIOCLIP_H__

#include <H3D/AudioClip.h>


namespace H3D {
  /// \ingroup H3DNodes 
  /// \class SimpleAudioClip
  /// \brief SimpleAudioClip is a node for playing audio
  /// with a simpler interface than the audio clip node.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SimpleAudioClip.x3d">SimpleAudioClip.x3d</a>
  ///     ( <a href="examples/SimpleAudioClip.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SimpleAudioClip.dot
  class H3DAPI_API SimpleAudioClip : public AudioClip {
  public:
    
    

    class UpdateAudioPlay: public TypedField< AutoUpdate< SFBool >,
                          Types< SFBool, SFBool, SFBool, 
    MFString, SFBool > >{
    public:
      virtual void update(); 
    };

    /// Constructor.
    SimpleAudioClip ( 
      Inst< SFString>  _description      = 0,
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
      Inst< TimeHandler > _time_handler   = 0,
      Inst< SFBool       > _play         = 0,
      Inst< SFBool       > _stop         = 0,
      Inst< SFBool       > _pause        = 0,
      Inst< SFString     > _audioState   = 0);

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// If a true event is received by the "play" field the audio
    /// will start playing, if it paused or stopped. If it is currently
    /// running nothing will happen.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile SimpleAudioClip_play.dot
    auto_ptr< SFBool > play;

    /// If a true event is received by the "stop" field the audio
    /// will stop playing.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile SimpleAudioClip_stop.dot
    auto_ptr< SFBool > stop;

    /// If a true event is received by the "pause" field the audio
    /// will be paused. It can be resumed again by sending 
    /// a start event to the "play" field.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile SimpleAudioClip_pause.dot
    auto_ptr< SFBool > pause;

    /// audioState describe if the audio is being playing, paused or stopped
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> "STOPPED"
    /// <b>Valid values:</b> "STOPPED", "PLAYING", "PAUSED"
    /// \dotfile SimpleAudioClip_audioState.dot
    auto_ptr< SFString > audioState;

  protected:
    /// update the playing of audio
    auto_ptr< UpdateAudioPlay > updateAudioPlay;
  };
}

#endif
