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
/// \file DirectShowDecoder.h
/// \brief Header file for DirectShowDecoder.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __DIRECTSHOWDECODER_H__
#define __DIRECTSHOWDECODER_H__

#include <H3D/FieldTemplates.h>
#include <H3D/SFTime.h>

#ifdef HAVE_DSHOW

// Include from DirectShow BaseClasses
// Remove the DEBUG defines when including DShow classes in order
// to always use the release version. The debug version caused us
// problems.
#ifdef _DEBUG
#define RESTORE__DEBUG
#undef _DEBUG
#endif

#ifdef DEBUG
#define RESTORE_DEBUG
#undef DEBUG
#endif
#include <streams.h>
#include <atlcomcli.h>
#include <strsafe.h>

#ifdef RESTORE__DEBUG
#define _DEBUG
#undef RESTORE__DEBUG
#endif

#ifdef RESTORE_DEBUG
#define DEBUG
#undef RESTORE_DEBUG
#endif

#include <H3D/H3DVideoClipDecoderNode.h>

namespace H3D {

struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

  /// \ingroup H3DNodes
  /// \class DirectShowDecoder
  /// \brief This node uses DirectShow to decode video clips. All video types
  /// that are supported by an installed DirectShow filter can be decoded.
  ///
  class H3DAPI_API DirectShowDecoder : public H3DVideoClipDecoderNode {

    /// The class checks for DirectShow events and handles them.
    class DShowEventHandler: public AutoUpdate< SFTime > {
      virtual void update();
    };

    /// The CFrameGrabber class is a DirectShow class implementation
    /// for grabbing frames.
    class CFrameGrabber: public CBaseVideoRenderer {
    public:
      /// Constructor.
      CFrameGrabber(LPUNKNOWN pUnk, HRESULT *phr, DirectShowDecoder *d );

      // CBaseVideoRenderer functions

      /// We force the media type to be RGB24.
      HRESULT CheckMediaType(const CMediaType *pmt );

      /// We save the width and height of the video.
      HRESULT SetMediaType(const CMediaType *pmt );
    
      /// Called when a new frame is available
      HRESULT DoRenderSample(IMediaSample *pMediaSample); 

      /// Called when the first sample is ready, before we start playing.
      /// Implemented in order for the first frame of the video to be rendered
      /// before playing is started.
      void OnReceiveFirstSample(IMediaSample *pSample) { 
        DoRenderSample( pSample ); 
      }

      DirectShowDecoder *decoder;
    };

    friend class CFrameGrabber;

  public:
    /// Constructor.
    DirectShowDecoder();

    /// Destructor.
    ~DirectShowDecoder();

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
      looping = v;
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
      HRESULT hr;
      if( g_pMS ) 
        hr = g_pMS->SetRate( r ); 
      rate = r;
      return hr == S_OK;
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
    virtual unsigned int getFrameBitsPerPixel() { return 24; }

    /// The size in bytes of the current frame.
    virtual unsigned int getFrameSize() { 
      return frame_size;
    }

    /// The byte alignment of each row in the buffer data from getNewFrame.
    virtual unsigned int getFrameByteAlignment() {
      return 4;
    }

    /// The pixel type of the current frame.
    virtual Image::PixelType getFramePixelType() { return Image::BGR; }
    
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
    // DirectShow pointers
    CComPtr<IGraphBuilder>  g_pGB;          // GraphBuilder
    CComPtr<IMediaControl>  g_pMC;          // Media Control
    CComPtr<IMediaSeeking> g_pMS;          // Media Seeking
    CComPtr<IMediaEvent>    g_pME;          // Media Event
    CComPtr<IBaseFilter>    g_pRenderer;    // our custom renderer
  
    /// Set up a DirectShow graph for playing the given url.
    HRESULT initDShowTextureRenderer( const string &url );

    /// Clean up all resources used by the current DirectShow
    /// graph.
    void cleanupDShow( void );

    /// The width in pixels of the frame.
    unsigned int frame_width;

    /// The height in pixels of the frame.
    unsigned int frame_height;

    /// The size in bytes of a frame.
    unsigned int frame_size;

    /// The size in bytes of the memory allocated in the data member.
    unsigned int data_size;

    /// The latest frame.
    unsigned char *data;

    /// Set to true when a new frame is available, reset to false when
    /// getNewFrame is called
    bool have_new_frame;

    /// The speed at which to play the clip. 1 is normal speed, 0.5 half.
    double rate;
  
    /// The DirectShow object used to get frames.
    CFrameGrabber *frame_grabber;

    /// True if the video should loop when it reaches the end.
    bool looping;

    /// The duration of the current clip in seconds.
    H3DTime duration;

    /// Event handler for DirectShow events.
    auto_ptr< DShowEventHandler > event_handler;
  };

}
#endif
#endif
