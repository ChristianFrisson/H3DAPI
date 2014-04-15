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
/// \file DirectShowDecoder.cpp
/// \brief cpp file for DirectShowDecoder.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DirectShowDecoder.h>
#include <H3D/Scene.h>

#ifdef HAVE_DSHOW

using namespace H3D;

// Define this if you want to render only the video component with no audio
//
//   #define NO_AUDIO_RENDERER


// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DirectShowDecoder::database( "DirectShowDecoder", 
                                           &(newInstance<DirectShowDecoder>), 
                                           typeid( DirectShowDecoder ) );

H3DVideoClipDecoderNode::DecoderRegistration 
DirectShowDecoder::reader_registration(
                            "DirectShowDecoder",
                            &(newImageLoaderNode< DirectShowDecoder >),
                            &DirectShowDecoder::supportsFileType 
                            );


HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    IEnumPins  *pEnum = NULL;
    IPin       *pPin = NULL;
    HRESULT    hr;

    if (ppPin == NULL)
    {
        return E_POINTER;
    }

    hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
        }

    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        hr = pPin->QueryDirection(&PinDirThis);
        if (FAILED(hr))
        {
            pPin->Release();
            pEnum->Release();
            return hr;
        }

        bool supports_video = false;
        
        IEnumMediaTypes *pEnumMediaTypes;
        pPin->EnumMediaTypes( &pEnumMediaTypes );
        AM_MEDIA_TYPE *t;
        while(pEnumMediaTypes->Next(1, &t, 0) == S_OK) {
          if( IsEqualGUID( t->majortype, MEDIATYPE_Video) || 
              IsEqualGUID( t->majortype, MEDIATYPE_Stream)) {
            supports_video = true;
          }
        }
                
        if (PinDir == PinDirThis && supports_video ) {
          // Found a match. Return the IPin pointer to the caller.
          *ppPin = pPin;
          pEnum->Release();
          return S_OK;
        }
        
        // Release the pin for the next time through the loop.
        pPin->Release();
    }
    // No more pins. We did not find a match.
    pEnum->Release();
    return E_FAIL;  
}

HRESULT DirectShowDecoder::initDShowTextureRenderer( const string &url )
{
    HRESULT hr = S_OK;
    CComPtr<IBaseFilter>    pFSrc;          // Source Filter
    CComPtr<IPin>           pFSrcPinOut;    // Source Filter Output Pin

    CoInitialize (NULL);

    // Create the filter graph
    if (FAILED(g_pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return E_FAIL;

    CFrameGrabber *frame_grabber = new CFrameGrabber( NULL, &hr, this );
    if (FAILED(hr) ) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Failed to create CFrameGrabber(" 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
      delete frame_grabber;
      frame_grabber = NULL;
      return E_FAIL;
    }

    // Get a pointer to the IBaseFilter on the TextureRenderer, add it to graph
    g_pRenderer = frame_grabber;
    if (FAILED(hr = g_pGB->AddFilter(g_pRenderer, L"TEXTURERENDERER"))) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not add renderer filter to graph! (" 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
        return hr;
    }

    // Determine the file to load based on windows directory
    // Use the standard win32 API to do this.
    WCHAR wFileName[MAX_PATH];

    wFileName[MAX_PATH-1] = 0;    // NULL-terminate

    USES_CONVERSION;
    (void)StringCchCopyW(wFileName, NUMELMS(wFileName), T2W(url.c_str()));

    // Add the source filter to the graph.
    hr = g_pGB->AddSourceFilter (wFileName, L"SOURCE", &pFSrc);

    // If the media file was not found, inform the user.
    if (hr == VFW_E_NOT_FOUND) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not add source filter to graph! (hr==VFW_E_NOT" 
                 << "_FOUND)" << endl 
                 << "This sample reads a media file from your windows"
                 <<" directory. This file is missing from this machine." 
                 << endl << "(" 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
      return hr;
    }
    else if(FAILED(hr)) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not add source filter to graph! (" 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
      return hr;
    }

    //if (FAILED(hr = pFSrc->FindPin(L"Output", &pFSrcPinOut))) {
    if (FAILED(hr = GetPin( pFSrc, PINDIR_OUTPUT, &pFSrcPinOut))) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not find output pin! " 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
        return hr;
    }

#ifdef NO_AUDIO_RENDERER

    // If no audio component is desired, directly connect the two video pins
    // instead of allowing the Filter Graph Manager to render all pins.

    CComPtr<IPin> pFTRPinIn;      // Texture Renderer Input Pin

    // Find the source's output pin and the renderer's input pin
    if (FAILED(hr = frame_grabber->FindPin(L"In", &pFTRPinIn))) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not find input pin!  (" 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
        return hr;
    }

    // Connect these two filters
    if (FAILED(hr = g_pGB->Connect(pFSrcPinOut, pFTRPinIn))) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not connect pins! (" 
                 << DirectShowDecoder::initDShowTextureRenderer << ")" << endl;
#endif
      return hr;
    }

#else

    // Render the source filter's output pin.  The Filter Graph Manager
    // will connect the video stream to the loaded DirectShowDecoder
    // and will load and connect an audio renderer (if needed).

    if (FAILED(hr = g_pGB->Render(pFSrcPinOut))) {
#ifdef DSHOW_DEBUG_MSG
      Console(4) << "Could not render source output pin!  ("
                 << DirectShowDecoder::initDShowTextureRenderer << ")" 
                 << endl;
#endif
        return hr;
    }

#endif

    // Get the graph's media control, event & position interfaces
    g_pGB.QueryInterface(&g_pMC);
    g_pGB.QueryInterface(&g_pMS);
    g_pGB.QueryInterface(&g_pME);

    setRate( rate );
    LONGLONG dur;
    g_pMS->GetDuration( &dur );
    // convert to seconds.
    duration = dur * 1e-7;
    g_pMC->Pause();

    return S_OK;
}

void DirectShowDecoder::cleanupDShow(void)
{
    // Shut down the graph
    if (!(!g_pMC)) g_pMC->Stop();

    if (!(!g_pMC)) g_pMC.Release();
    if (!(!g_pME)) g_pME.Release();
    if (!(!g_pMS)) g_pMS.Release();
    if (!(!g_pGB)) g_pGB.Release();
    if (!(!g_pRenderer)) g_pRenderer.Release();
                if( data ) {
                        delete[] data;
                        data = NULL;
                }
    duration = 0;
}


DirectShowDecoder::DirectShowDecoder( )
                                  : frame_width( 0 ),
                                    frame_height( 0 ),
                                    frame_size( 0 ),
                                    data_size( 0 ),
                                    data( NULL ), 
                                    have_new_frame( false ),
                                    looping( false ),
                                    rate( 1 )
{
  type_name = "DirectShowDecoder";
  event_handler.reset( new DShowEventHandler );
  event_handler->setOwner( this );
  Scene::time->route( event_handler );
}


// DirectShowDecoder destructor
DirectShowDecoder::~DirectShowDecoder() {
  cleanupDShow();
    // Do nothing
}



// CheckMediaType: This method forces the graph to give us an R8G8B8 video
// type.
HRESULT DirectShowDecoder::CFrameGrabber::CheckMediaType(const CMediaType *pmt){
    HRESULT   hr = E_FAIL;
    VIDEOINFO *pvi=0;

    CheckPointer(pmt,E_POINTER);

    // Reject the connection if this is not a video type
    if( *pmt->FormatType() != FORMAT_VideoInfo ) {
        return E_INVALIDARG;
    }

    // Only accept RGB24 video
    pvi = (VIDEOINFO *)pmt->Format();

    if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
       IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_RGB24))
    {
        hr = S_OK;
    }

    return hr;
}

// Graph connection has been made. Set the dimensions of the video
HRESULT DirectShowDecoder::CFrameGrabber::SetMediaType(const CMediaType *pmt) {
    UINT uintWidth = 2;
    UINT uintHeight = 2;

    // Retrive the size of this media type
    VIDEOINFO *pviBmp;                      // Bitmap info header
    pviBmp = (VIDEOINFO *)pmt->Format();

    decoder->frame_size = pviBmp->bmiHeader.biSizeImage;
    decoder->frame_width  = pviBmp->bmiHeader.biWidth;
    decoder->frame_height = abs(pviBmp->bmiHeader.biHeight);

    return S_OK;
}


// We have a new sample. Copy it.
HRESULT DirectShowDecoder::CFrameGrabber::DoRenderSample( IMediaSample * pSample ) {
    BYTE  *pBmpBuffer;  

    BYTE  * pbS = NULL;
    DWORD * pdwS = NULL;
    DWORD * pdwD = NULL;
    
    // Get the video bitmap buffer
    pSample->GetPointer( &pBmpBuffer );
    
    unsigned int bytes_to_copy = pSample->GetSize();
    if( decoder->data_size < bytes_to_copy ) {
      if( decoder->data ) delete[] decoder->data;
      decoder->data = new unsigned char[ bytes_to_copy ];
      decoder->data_size = bytes_to_copy;
    }
    memcpy( decoder->data, pBmpBuffer, bytes_to_copy );
    decoder->have_new_frame = true;

    return S_OK;
}

DirectShowDecoder::CFrameGrabber::CFrameGrabber( LPUNKNOWN pUnk, 
                                                 HRESULT *phr, 
                                                 DirectShowDecoder *d ):
  CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer),
                     NAME("Texture Renderer"), pUnk, phr),
  decoder( d ) {
  
}

void DirectShowDecoder::getNewFrame( unsigned char *buffer ) {
  memcpy( buffer, data,  getFrameSize() );
  have_new_frame = false;
}

void DirectShowDecoder::startPlaying() {
  if( g_pMC && status != PLAYING ) {
    HRESULT hr = S_OK;
    // Start the graph running;
    if (FAILED(hr = g_pMC->Run()) ){
      Console( 4 ) << "Could not run the DirectShow graph! "
                   << "(DirectShowDecoder::startPlaying)"<< endl; 
    } else {
      status = PLAYING;
    }
  }
}

void DirectShowDecoder::stopPlaying() {
 if( g_pMC && g_pMS && status != STOPPED ) {
    HRESULT hr = S_OK;
    // Stop the graph running;
    LONGLONG pos = 0;
    hr = g_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, 
                             NULL, AM_SEEKING_NoPositioning );
    hr = g_pMC->StopWhenReady();
    status = STOPPED;
 }
}

void DirectShowDecoder::pausePlaying() {
  if( g_pMC && status == PLAYING ) {
    HRESULT hr = S_OK;
    // Stop the graph running;
    if (FAILED(hr = g_pMC->Pause()) ){
      Console( 4 ) << "Pause failed (" << (&DirectShowDecoder::pausePlaying)
                   << ")" << endl;
    } else {
      status = PAUSED;
    }
  }
}

void DirectShowDecoder::DShowEventHandler::update() {
  DirectShowDecoder *d = static_cast< DirectShowDecoder * >( getOwner() );
  long lEventCode;
  LONG_PTR lParam1, lParam2;
  HRESULT hr;

  if (!d->g_pME)
    return;

  // Check for completion events
  hr = d->g_pME->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
  if (SUCCEEDED(hr)) {
    // If we have reached the end of the media file, reset to beginning
    if (EC_COMPLETE == lEventCode) {
      if( d->looping ) {
        LONGLONG pos = 0;
        hr = d->g_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, 
                                    NULL, AM_SEEKING_NoPositioning );
      }
      
      // Free any memory associated with this event
      hr = d->g_pME->FreeEventParams(lEventCode, lParam1, lParam2);
    }
  }
}

bool DirectShowDecoder::loadClip( const string &url ) {
  cleanupDShow();
  status = STOPPED;
  return initDShowTextureRenderer( url ) == S_OK;
}

// TODO: should find a better way to check if a file type is supported.
bool DirectShowDecoder::supportsFileType( const string &url ) {
  AutoRef< DirectShowDecoder > dec( new DirectShowDecoder() );
  return dec->loadClip( url );
}

/// Get the current position in the clip (in seconds from start position)
H3DTime DirectShowDecoder::getPosition() {
  LONGLONG pos;
  if( g_pMS ) {
                // Set time format to be expressed in nano seconds, then get it and
                // convert to seconds.
                if( SUCCEEDED( g_pMS->SetTimeFormat( &TIME_FORMAT_MEDIA_TIME ) ) &&
                                SUCCEEDED( g_pMS->GetCurrentPosition(&pos) ) ) {
                        return H3DTime(pos / 10000000.0);
                }
  }
  return 0;
}

/// Set the current position in the clip(in seconds from start position)
void DirectShowDecoder::setPosition( H3DTime pos ) {
  LONGLONG p = (LONGLONG )( pos * 1e7 );
  if( g_pMS )
    g_pMS->SetPositions(&p, AM_SEEKING_AbsolutePositioning, 
                        NULL, AM_SEEKING_NoPositioning );
}

#endif
