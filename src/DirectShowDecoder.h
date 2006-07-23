//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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

// Include from DirectShow BaseClasses
#include <streams.h>

#include "H3DVideoClipDecoderNode.h"

namespace H3D {
//-----------------------------------------------------------------------------
// Define GUID for Texture Renderer
// {71771540-2017-11cf-AE26-0020AFD79767}
//-----------------------------------------------------------------------------
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

class DirectShowDecoder : public H3DVideoClipDecoderNode
{
  class CFrameGrabber: public CBaseVideoRenderer {
  public:
    CFrameGrabber(LPUNKNOWN pUnk, HRESULT *phr, DirectShowDecoder *d );
    // CBaseVideoRenderer functions
    HRESULT CheckMediaType(const CMediaType *pmt ); // Format acceptable?
    HRESULT SetMediaType(const CMediaType *pmt );   // Video format notification
    
    /// Called when a new frame is available
    HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample
    
    DirectShowDecoder *decoder;
  };

  friend class CFrameGrabber;

public:
  DirectShowDecoder();
  ~DirectShowDecoder();

  virtual bool loadClip( const string &url ) { return true; }
  virtual void startPlaying() {}
  virtual void stopPlaying() {}

  virtual bool haveNewFrame() { return have_new_frame; }
  virtual void getNewFrame( unsigned char *buffer );
  virtual H3DInt32 getFrameWidth() { return frame_width; }
  virtual H3DInt32 getFrameHeight() { return frame_height; }
  virtual Image::PixelType getFramePixelType() { return Image::RGB; }
  virtual Image::PixelComponentType getFramePixelComponentType() { 
    return Image::UNSIGNED;
  }

protected:
  // DirectShow pointers
  CComPtr<IGraphBuilder>  g_pGB;          // GraphBuilder
  CComPtr<IMediaControl>  g_pMC;          // Media Control
  CComPtr<IMediaPosition> g_pMP;          // Media Position
  CComPtr<IMediaEvent>    g_pME;          // Media Event
  CComPtr<IBaseFilter>    g_pRenderer;    // our custom renderer
  
  HRESULT initDShowTextureRenderer( CBaseVideoRenderer *pCTR );
  void cleanupDShow( void );

  /// The width in pixels of the frame.
  unsigned int frame_width;

  /// The height in pixels of the frame.
  unsigned int frame_height;

  /// The size in bytes of the data.
  unsigned int data_size;

  /// The latest frame.
  unsigned char *data;

  /// Set to true when a new frame is available, reset to false when
  /// getNewFrame is called
  bool have_new_frame;
  
  /// The DirectShow object used to get frames.
  CFrameGrabber *frame_grabber;
};
}
#endif
