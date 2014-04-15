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
/// \file SimpleMovieTexture.h
/// \brief Header file for SimpleMovieTexture.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SIMPLEMOVIETEXTURE_H__
#define __SIMPLEMOVIETEXTURE_H__

#include <H3D/H3DVideoTextureNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/SFFloat.h>
#include <H3D/SFTime.h>
#include <H3D/SFInt32.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class SimpleMovieTexture
  /// \brief SimpleMovieTexture is a node for showing movies as textures
  /// with a simpler interface than the movie texture node.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SimpleMovieTexture.x3d">SimpleMovieTexture.x3d</a>
  ///     ( <a href="examples/SimpleMovieTexture.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SimpleMovieTexture.dot
  class H3DAPI_API SimpleMovieTexture : public H3DVideoTextureNode,
                                        public X3DUrlObject{
  public:

    /// The DecoderManager class manages all state changes in the
    /// decoder depending on the values of the fields in  
    /// SimpleMovieTexture.
    class DecoderManager: public TypedField< AutoUpdate< SFBool >,
                          Types< SFBool, SFBool, SFBool, 
    SFBool, MFString, SFFloat > > {
      virtual void update();
    };
#ifdef __BORLANDC__
    friend class DecoderManager;
#endif

    /// Constructor.
    SimpleMovieTexture ( 
                 Inst< DisplayList  > _displayList  = 0,
                 Inst< SFNode       > _metadata     = 0,
                 Inst< SFBool       > _repeatS      = 0,
                 Inst< SFBool       > _repeatT      = 0,
                 Inst< SFBool       > _scaleToP2    = 0,
                 Inst< SFImage      > _image        = 0,
                 Inst< SFTextureProperties > _textureProperties = 0,
                 Inst< SFBool       > _play         = 0,
                 Inst< SFBool       > _stop         = 0,
                 Inst< SFBool       > _pause        = 0,
                 Inst< SFTime      > _duration       = 0,
                 Inst< SFFloat     > _rate    = 0,
                 Inst< SFBool      > _playAudio    = 0,
                 Inst< SFBool      > _loop         = 0,
                 Inst< SFInt32     > _width        = 0,
                 Inst< SFInt32     > _height       = 0,
                 Inst< MFString >  _url             = 0 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// If a true event is received by the "play" field the movie
    /// will start playing, if it paused or stopped. If it is currently
    /// running nothing will happen.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile SimpleMovieTexture_play.dot
    auto_ptr< SFBool > play;

    /// If a true event is received by the "stop" field the movie
    /// will stop playing, and the playback position will be reset to the
    /// beginning, so that a later "play" event will start playing from 
    /// the start.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile SimpleMovieTexture_stop.dot
    auto_ptr< SFBool > stop;

    /// If a true event is received by the "pause" field the movie
    /// will be paused. It can be resumed again by sending a false event
    /// to the "pause" field or a start event to the "play" field.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile SimpleMovieTexture_pause.dot
    auto_ptr< SFBool > pause;

    /// The duration field is an output only field for the duration of 
    /// the currently loaded video clip played at normal rate in seconds. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile SimpleMovieTexture_duration.dot
    auto_ptr< SFTime > duration;

    /// The rate field determines the rate at which the movie should be played.
    /// A value of 1 specifies normal playback rate, and a value of e.g. 2 
    /// doubles the playback rate. A negative value means that the movie
    /// should be played backwards. All video decoders does not support
    /// all rates (especially not negative). 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value: </b> 1.0
    /// 
    /// \dotfile SimpleMovieTexture_rate.dot
    auto_ptr< SFFloat > rate;

    /// The playAudio field determines whether audio should be played or not
    /// if the movie contains an audio stream.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value: </b> true
    /// 
    /// \dotfile SimpleMovieTexture_playAudio.dot
    auto_ptr< SFBool > playAudio;

    /// The loop field determines or not the movie should start over from
    /// the beginning when the end has been reached.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value: </b> false
    /// 
    /// \dotfile SimpleMovieTexture_loop.dot
    auto_ptr< SFBool > loop;

    /// The videoWidth contains the width in pixels of the currently
    /// loaded video clip.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile SimpleMovieTexture_videoWidth.dot
    auto_ptr< SFInt32 > videoWidth;

    /// The videoWidth contains the width in pixels of the currently
    /// loaded video clip.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile SimpleMovieTexture_videoHeight.dot
    auto_ptr< SFInt32 > videoHeight;

    /// The DecoderManager class manages all state changes in the
    /// decoder depending on the values of the fields in  
    /// SimpleMovieTexture.
    auto_ptr< DecoderManager > decoderManager;
    
  };
}

#endif
