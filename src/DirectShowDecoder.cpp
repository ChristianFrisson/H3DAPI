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
/// \file DirectShowDecoder.cpp
/// \brief cpp file for DirectShowDecoder.
///
//
//////////////////////////////////////////////////////////////////////////////

#include "DirectShowDecoder.h"
#include "Scene.h"

using namespace H3D;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

// Define this if you want to render only the video component with no audio
//
   #define NO_AUDIO_RENDERER

// An application can advertise the existence of its filter graph
// by registering the graph with a global Running Object Table (ROT).
// The GraphEdit application can detect and remotely view the running
// filter graph, allowing you to 'spy' on the graph with GraphEdit.
//
// To enable registration in this sample, define REGISTER_FILTERGRAPH.
//


//-----------------------------------------------------------------------------
// Msg: Display an error message box if needed
//-----------------------------------------------------------------------------
void Msg(TCHAR *szFormat, ...)
{
    TCHAR szBuffer[1024];  // Large buffer for long filenames or URLs
    const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    const int LASTCHAR = NUMCHARS - 1;

    // Format the input string
    va_list pArgs;
    va_start(pArgs, szFormat);

    // Use a bounded buffer size to prevent buffer overruns.  Limit count to
    // character size minus one to allow for a NULL terminating character.
    (void)StringCchVPrintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);

    // Ensure that the formatted string is NULL-terminated
    szBuffer[LASTCHAR] = TEXT('\0');

    MessageBox(NULL, szBuffer, TEXT("DirectShow Texture3D9 Sample"),
               MB_OK | MB_ICONERROR);
}


//-----------------------------------------------------------------------------
// InitDShowTextureRenderer : Create DirectShow filter graph and run the graph
//-----------------------------------------------------------------------------
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
      cerr << "Failed to create CFrameGrabber" << endl;
      delete frame_grabber;
      frame_grabber = NULL;
      return E_FAIL;
    }

    // Get a pointer to the IBaseFilter on the TextureRenderer, add it to graph
    g_pRenderer = frame_grabber;
    if (FAILED(hr = g_pGB->AddFilter(g_pRenderer, L"TEXTURERENDERER")))
    {
        Msg(TEXT("Could not add renderer filter to graph!  hr=0x%x"), hr);
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
    if (hr == VFW_E_NOT_FOUND)
    {
        Msg(TEXT("Could not add source filter to graph!  (hr==VFW_E_NOT_FOUND)\r\n\r\n")
            TEXT("This sample reads a media file from your windows directory.\r\n")
            TEXT("This file is missing from this machine."));
        return hr;
    }
    else if(FAILED(hr))
    {
        Msg(TEXT("Could not add source filter to graph!  hr=0x%x"), hr);
        return hr;
    }

    if (FAILED(hr = pFSrc->FindPin(L"Output", &pFSrcPinOut)))
    {
        Msg(TEXT("Could not find output pin!  hr=0x%x"), hr);
        return hr;
    }

#ifdef NO_AUDIO_RENDERER

    // If no audio component is desired, directly connect the two video pins
    // instead of allowing the Filter Graph Manager to render all pins.

    CComPtr<IPin> pFTRPinIn;      // Texture Renderer Input Pin

    // Find the source's output pin and the renderer's input pin
    if (FAILED(hr = frame_grabber->FindPin(L"In", &pFTRPinIn)))
    {
        Msg(TEXT("Could not find input pin!  hr=0x%x"), hr);
        return hr;
    }

    // Connect these two filters
    if (FAILED(hr = g_pGB->Connect(pFSrcPinOut, pFTRPinIn)))
    {
        Msg(TEXT("Could not connect pins!  hr=0x%x"), hr);
        return hr;
    }

#else

    // Render the source filter's output pin.  The Filter Graph Manager
    // will connect the video stream to the loaded DirectShowDecoder
    // and will load and connect an audio renderer (if needed).

    if (FAILED(hr = g_pGB->Render(pFSrcPinOut)))
    {
        Msg(TEXT("Could not render source output pin!  hr=0x%x"), hr);
        return hr;
    }

#endif

    // Get the graph's media control, event & position interfaces
    g_pGB.QueryInterface(&g_pMC);
    g_pGB.QueryInterface(&g_pMS);
    g_pGB.QueryInterface(&g_pME);

    g_pMC->Pause();

    return S_OK;
}

//-----------------------------------------------------------------------------
// CleanupDShow
//-----------------------------------------------------------------------------
void DirectShowDecoder::cleanupDShow(void)
{
    // Shut down the graph
    if (!(!g_pMC)) g_pMC->Stop();

    if (!(!g_pMC)) g_pMC.Release();
    if (!(!g_pME)) g_pME.Release();
    if (!(!g_pMS)) g_pMS.Release();
    if (!(!g_pGB)) g_pGB.Release();
    if (!(!g_pRenderer)) g_pRenderer.Release();
}


//-----------------------------------------------------------------------------
// DirectShowDecoder constructor
//-----------------------------------------------------------------------------
DirectShowDecoder::DirectShowDecoder( )
                                  : frame_width( 0 ),
                                    frame_height( 0 ),
                                    data_size( 0 ),
                                    data( NULL ), 
                                    have_new_frame( false ),
                                    looping( false )
{
  event_handler.reset( new DShowEventHandler );
  event_handler->setOwner( this );
  Scene::time->route( event_handler );
}


//-----------------------------------------------------------------------------
// DirectShowDecoder destructor
//-----------------------------------------------------------------------------
DirectShowDecoder::~DirectShowDecoder()
{
  cleanupDShow();
    // Do nothing
}


//-----------------------------------------------------------------------------
// CheckMediaType: This method forces the graph to give us an R8G8B8 video
// type, making our copy to texture memory trivial.
//-----------------------------------------------------------------------------
HRESULT DirectShowDecoder::CFrameGrabber::CheckMediaType(const CMediaType *pmt)
{
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

//-----------------------------------------------------------------------------
// SetMediaType: Graph connection has been made.
//-----------------------------------------------------------------------------
HRESULT DirectShowDecoder::CFrameGrabber::SetMediaType(const CMediaType *pmt)
{
    HRESULT hr;

    UINT uintWidth = 2;
    UINT uintHeight = 2;

    // Retrive the size of this media type

    VIDEOINFO *pviBmp;                      // Bitmap info header
    pviBmp = (VIDEOINFO *)pmt->Format();

    decoder->frame_width  = pviBmp->bmiHeader.biWidth;
    decoder->frame_height = abs(pviBmp->bmiHeader.biHeight);

    return S_OK;
}


//-----------------------------------------------------------------------------
// DoRenderSample: A sample has been delivered. Copy it to the texture.
//-----------------------------------------------------------------------------
HRESULT DirectShowDecoder::CFrameGrabber::DoRenderSample( IMediaSample * pSample )
{
    BYTE  *pBmpBuffer, *pTxtBuffer; // Bitmap buffer, texture buffer
    LONG  lTxtPitch;                // Pitch of bitmap, texture

    BYTE  * pbS = NULL;
    DWORD * pdwS = NULL;
    DWORD * pdwD = NULL;
    UINT row, col, dwordWidth;
    
    // Get the video bitmap buffer
    pSample->GetPointer( &pBmpBuffer );
    
    unsigned int bytes_to_copy = decoder->frame_height * decoder->frame_width * 3;//pSample->GetActualDataLength();
    if( decoder->data_size < bytes_to_copy ) {
      if( decoder->data ) delete decoder->data;
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
  memcpy( buffer, data,  getFrameWidth() * getFrameHeight() * 3 );
  have_new_frame = false;
}

void DirectShowDecoder::startPlaying() {
  if( g_pMC && status != PLAYING ) {
    HRESULT hr = S_OK;
    // Start the graph running;
    if (FAILED(hr = g_pMC->Run()) ){
      Msg(TEXT("Could not run the DirectShow graph!  hr=0x%x"), hr);
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
     hr = g_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );
    hr = g_pMC->StopWhenReady();
    status = STOPPED;
 }
}

void DirectShowDecoder::pausePlaying() {
  if( g_pMC && status == PLAYING ) {
    HRESULT hr = S_OK;
    // Stop the graph running;
    if (FAILED(hr = g_pMC->Pause()) ){
      Msg(TEXT("Could not run the DirectShow graph!  hr=0x%x"), hr);
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
    if (EC_COMPLETE == lEventCode)
      {
        if( d->looping ) {
          LONGLONG pos = 0;
          hr = d->g_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );
        }
        
        // Free any memory associated with this event
        hr = d->g_pME->FreeEventParams(lEventCode, lParam1, lParam2);
      }
  }
}

bool DirectShowDecoder::loadClip( const string &url ) {
  cleanupDShow();
  return initDShowTextureRenderer( url ) == S_OK;
}
