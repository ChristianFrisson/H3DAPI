//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB, PolyDimensions
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
//    Original file contributed by PolyDimensions.
//
/// \file FFmpegDecoder.h
/// \brief Header file for FFmpegDecoder.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __FFMPEGDECODER_H__
#define __FFMPEGDECODER_H__

#include <H3D/FieldTemplates.h>
#include <H3D/SFTime.h>
#include <time.h>

#ifdef HAVE_FFMPEG

extern "C" {
#define __STDC_CONSTANT_MACROS

#ifdef _STDINT_H

#undef _STDINT_H

#endif
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
}

#include <H3D/H3DVideoClipDecoderNode.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class FFmpegDecoder
  /// \brief This node uses FFmpeg to decode video clips. All video types
  /// that are supported by an installed FFmpeg filter can be decoded.
  ///
  class H3DAPI_API FFmpegDecoder : public H3DVideoClipDecoderNode {
    /// The class checks for FFmpeg events and handles them.
    class FFmpegEventHandler: public AutoUpdate< SFTime > {
      virtual void update();
    };

  public:
    /// Constructor.
    FFmpegDecoder();

    /// Destructor.
    ~FFmpegDecoder();

    ///test if video clip is valid. Returns true on success.
    virtual bool testClip( const string &url );

    ///load an video clip from a url. Returns true on success.
    virtual bool loadClip( const string &url );

    /// Start decoding the video clip.
    virtual void startPlaying();

    /// Stop decoding the video clip and set the position to the start
    /// position.
    virtual void stopPlaying();

    /// Pause the decoding of the video clip.
    virtual void pausePlaying();

    /// Set whether the clip should loop and start from the start again
    /// when the end has been reached.
    virtual void setLooping( bool v ) {

      // Quick fix - for some reason the incorrect value of v gets passed 
      looping = true;

/*
      if (v == true) 
        Console(4) << "setLooping(true)" << endl;
      if (v == false) 
        Console(4) << "setLooping(false)" << endl;
      looping = v;
*/
    }

    /// Get the current position in the clip (in seconds from start position)
    virtual H3DTime getPosition();

    /// Set the current position in the clip(in seconds from start position)
    virtual void setPosition( H3DTime pos );

    /// Set the playback rate. A rate of 1 means normal playback speed, 
    /// 2 double.
    /// Negative values means playing backwards. 
    /// \returns true if the new rate is supported by the decoder.
    virtual bool setRate( double r ) { 
      rate = r;
      return 1;
    } 

    /// Returns the duration in seconds at normal play rate of the currently
    /// loaded video clip
    virtual H3DTime getDuration() {
      return duration;
    }

    /// Returns true when a new frame is available.
    virtual bool haveNewFrame() { return have_new_frame; }

    /// Get the new frame. The buffer must be at least getFrameSize() bytes. 
    virtual void getNewFrame( unsigned char *buffer );

    /// The width in pixels of the current frame.
    virtual unsigned int getFrameWidth() { return frame_width; }

    /// The height in pixels of the current frame.
    virtual unsigned int getFrameHeight() { return frame_height; }

    /// Get the number of bits per pixel in the current frame.
    virtual unsigned int getFrameBitsPerPixel() { return frame_bpp; }

    /// The size in bytes of the current frame.
    virtual unsigned int getFrameSize() { 
      return frame_height * frame_width * (frame_bpp / 8); 
    } 

    /// Should the next frame be processed?  
    virtual bool theTimeIsRight();

    /// The pixel type of the current frame.
    virtual Image::PixelType getFramePixelType() { return Image::RGB; }
    
    /// The pixel component type of the current frame.
    virtual Image::PixelComponentType getFramePixelComponentType() { 
      return Image::UNSIGNED;
    }

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;  
  
    /// Register this node to the H3DVideoClipDecoderNodes available.
    static DecoderRegistration reader_registration;

    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );

  protected:

    uint8_t *temp_buffer;

    struct SwsContext *imgConvertCtx;

    AVFormatContext *pFormatCtx;

    AVCodecContext *pCodecCtx;

    AVCodecContext *aCodecCtx;

    AVCodec *pCodec;

    AVCodec *aCodec;

    AVFrame *pFrame;

    AVFrame *pFrameRGB;

    unsigned char *data;

    AVPacket packet;

    double pts;

    int frameFinished;

    int videoStream;

    int audioStream;

    int numBytes;

    long current_frame;

    float video_fps;

    unsigned long current_time;

    unsigned long start_time;
  
    /// Clean up all resources used by the current FFmpeg
    /// graph.
    void cleanupFFmpeg( void );

    /// The width in pixels of the frame.
    unsigned int frame_width;

    /// The height in pixels of the frame.
    unsigned int frame_height;

    /// Bits per pixel of the frame.
    unsigned int frame_bpp;

    /// The size in bytes of the data.
    unsigned int data_size;

    /// Set to true when a new frame is available, reset to false when
    /// getNewFrame is called
    bool have_new_frame;

    /// The speed at which to play the clip. 1 is normal speed, 0.5 half.
    double rate;
  
    /// True if the video should loop when it reaches the end.
    bool looping;

    /// The duration of the current clip in seconds.
    H3DTime duration;

    /// For proper functioning of testClip()
    bool just_a_test;

  };

}
#endif // HAVE_FFMPEG
#endif // __FFMPEGDECODER_H__
