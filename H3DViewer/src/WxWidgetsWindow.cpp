//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2014, SenseGraphics AB
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
/// \file WxWidgetsWindow.cpp
/// \brief CPP file for WxWidgetsWindow.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "WxWidgetsWindow.h"
#include "WxFrame.h"
#include "Envini.h"
#include <H3D/Viewpoint.h>
#include <H3D/INIFile.h>

#ifndef WIN32
#include <H3D/X3DKeyDeviceSensorNode.h>
#include <H3D/MouseSensor.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase WxWidgetsWindow::database( "WxWidgetsWindow", 
                                      &(newInstance<WxWidgetsWindow>), 
                                      typeid( WxWidgetsWindow ),
                                      &(H3DWindowNode::database) );


WxWidgetsWindow::WxWidgetsWindow( wxWindow *_theParent,
                                  Inst< SFInt32     > _width,
                                  Inst< SFInt32     > _height,
                                  Inst< SFBool      > _fullscreen,
                                  Inst< SFBool      > _mirrored,
                                  Inst< RenderMode  > _renderMode, 
                                  Inst< SFViewpoint > _viewpoint,
                                  Inst< SFInt32     > _posX,
                                  Inst< SFInt32     > _posY,
                                  Inst< SFBool      > _manualCursorControl,
                                  Inst< SFString    > _cursorType ) :
  H3DWindowNode( _width, _height, _fullscreen, _mirrored, _renderMode,
                 _viewpoint, _posX, _posY, _manualCursorControl,
                 _cursorType ),
  theWindow( _theParent ),
  drag_file_func( NULL ),
  drag_file_func_arg( NULL ),
  is_initialized( false ),
  use_h3d_settings( true ),
  theWxGLCanvas( NULL ),
  fullscreen_initialized ( false ),
  theWxGLContext( NULL ),
  allow_new_pixel_format_creation( true ) {
  type_name = "WxWidgetsWindow";
  database.initFields( this );
 
#ifdef HAVE_DX9
  gl_d3d_frame_buffer.resize(1);
  gl_frame_buffer.resize(1);
  d3d_canvas = NULL;
#endif

  have_parent = theWindow != NULL;
  vector< string > valid_values;
  getSupportedCursorsTypes( valid_values );
  cursorType->addValidValues( valid_values.begin(), valid_values.end() );


 
  if( !theWindow ) {
     theWindow = new wxFrame( NULL, wxID_ANY, wxT("WxFrame"),
                              wxPoint( posX->getValue(), posY->getValue() ),
                              wxSize( width->getValue(), height->getValue() ));
  }
  sizer = new wxBoxSizer( wxHORIZONTAL );
  theWindow->SetSizer( sizer );
}

void WxWidgetsWindow::initWindow() {
  if( isInitialized() && !allow_new_pixel_format_creation ) {
    Console(4) << "WxWidgetsWindow does not support changing pixel format from/to "
               << "quad buffered stereo support after initialization." << endl;
    if( last_render_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
      renderMode->setValue( "QUAD_BUFFERED_STEREO" );
    } else {
      renderMode->setValue( "MONO" );
    }
    return;
  }
  RenderMode::Mode stereo_mode = renderMode->getRenderMode();

  int attribList[20];
  unsigned int i = 0;
  attribList[i++] = WX_GL_RGBA;
  attribList[i++] = WX_GL_DOUBLEBUFFER;
  attribList[i++] = WX_GL_DEPTH_SIZE;
  attribList[i++] = 24;
  attribList[i++] = WX_GL_STENCIL_SIZE;
  attribList[i++] = 8;

  // fullscreen anti-aliasing only supported on wxWidgets 2.9 or later
#if( wxMAJOR_VERSION > 2 || wxMINOR_VERSION >= 9 ) 
  if( useFullscreenAntiAliasing->getValue() ) {
    attribList[i++] = WX_GL_SAMPLE_BUFFERS;
    attribList[i++] = 1;
    attribList[i++] = WX_GL_SAMPLES;
    attribList[i++] = 4;
  }
#endif

  // TODO: FIX stereo mode
#ifdef MACOSX
  // TODO: stereo mode does not work with mac
  attribList[i++] = 0;
#else
  // On Windows we can not check here if stereo is supported, simply because
  // on some systems GL_STEREO is false even though QUAD_BUFFERED_STEREO
  // is supported. The check have to be made after SetCurrent.
  if( stereo_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
    check_if_stereo_obtained = true;
#ifndef H3D_WINDOWS
    GLboolean quad_stereo_supported;
    glGetBooleanv( GL_STEREO, &quad_stereo_supported);
    if( quad_stereo_supported ) {
#endif
      attribList[i++] = WX_GL_STEREO;
#ifndef H3D_WINDOWS
    }
#endif
  }
  attribList[i++] = 0;
#endif
  // if we have a previous window, use same rendering context and destroy it.
  MyWxGLCanvas *old_canvas = theWxGLCanvas;
  theWxGLCanvas = 
      new MyWxGLCanvas( this, theWindow, -1, wxDefaultPosition,
                        wxSize( width->getValue(), height->getValue() ), 
                        attribList );

  if( !theWxGLContext || ( last_render_mode != stereo_mode && ( last_render_mode == RenderMode::QUAD_BUFFERED_STEREO || stereo_mode == RenderMode::QUAD_BUFFERED_STEREO ) ) ) {
    if( theWxGLContext )
      delete theWxGLContext;
    theWxGLContext = new wxGLContext( theWxGLCanvas );
  }

  if( old_canvas ) {
    old_canvas->Destroy();
  }
  
#if wxUSE_DRAG_AND_DROP
  theWxGLCanvas->SetDropTarget( new DragAndDropFile( this ) );
#endif




  last_fullscreen = !fullscreen->getValue();

  

  sizer->Clear( true );
#ifdef HAVE_DX9
  if( stereo_mode == RenderMode::NVIDIA_3DVISION ) {
    d3d_canvas = new D3D9Canvas( this, theWindow, 
                                             wxID_ANY, 
                                             wxDefaultPosition,
                                             wxSize( width->getValue(), height->getValue() ) );
 
    sizer->Add( d3d_canvas, 1, wxALL|wxEXPAND, 0 );
  }
#endif

  sizer->Add( theWxGLCanvas, 1, wxALL|wxEXPAND, 0 );
    
#ifdef H3D_WINDOWS

#ifdef HAVE_DX9
  if( stereo_mode == RenderMode::NVIDIA_3DVISION ) {
    hWnd = (HWND)(d3d_canvas->GetHWND());
  } else {
    hWnd = (HWND)(theWxGLCanvas->GetHandle());
  }
#else
   hWnd = (HWND)(theWxGLCanvas->GetHandle());
#endif // HAVE_DX9

#ifdef H3D_WIN64
  wpOrigProc = (WNDPROC) SetWindowLongPtr(hWnd, 
                GWLP_WNDPROC, (LONG_PTR) WindowProc);
#else
  wpOrigProc = (WNDPROC) SetWindowLongPtr(hWnd, 
                GWL_WNDPROC, (LONG_PTR) WindowProc);
#endif
#endif
    //  theWindow->Layout();
  theWindow->Show();
  setFullscreen( fullscreen->getValue() );
  theWxGLCanvas->Show();
  theWindow->Raise();
  theWindow->Layout();

  makeWindowActive();
  if( stereo_mode == RenderMode::NVIDIA_3DVISION )
    theWxGLCanvas->Hide();
  
  glClearColor( 0, 0, 0, 1 );
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glClear( GL_COLOR_BUFFER_BIT );
  is_initialized = true;
}

void WxWidgetsWindow::setFullscreen( bool fullscreen ) {
  if ( last_fullscreen != fullscreen || !fullscreen_initialized ) {
    wxTopLevelWindow * tlw = dynamic_cast< wxTopLevelWindow * >(theWindow);
#ifndef MACOSX
    //TODO: fullscreen does not work well on macosx
    if( tlw ) {
      long style = wxFULLSCREEN_ALL;
      WxFrame * tmp_frame = dynamic_cast< WxFrame * >( theWindow );
      if( tmp_frame ) {
        if( fullscreen )
          style = tmp_frame->getFullScreenStyle();
        else
          tmp_frame->showPreviouslyHiddenDialogs();
      }
      tlw->ShowFullScreen( fullscreen, style );
      if( fullscreen && tmp_frame && style != wxFULLSCREEN_ALL )
        tmp_frame->hideAllDialogs();
    }
#endif
    last_fullscreen = fullscreen;
    fullscreen_initialized= true;
  }
}

//#include<FreeImage.h>

void WxWidgetsWindow::swapBuffers() {

  RenderMode::Mode stereo_mode = renderMode->getRenderMode();

#ifdef HAVE_DX9
  if( stereo_mode == RenderMode::NVIDIA_3DVISION ) {
    int current_w = generator->width->getValue();
    int current_h = generator->height->getValue();
       
    if( generator->isFBOInitialized() ) {
      glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, generator->getFBOId() );
    }
    glReadPixels( 0, 0, current_w, current_h, GL_BGRA, GL_UNSIGNED_BYTE, &gl_frame_buffer[0]  );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    
    int row_length = 4 * current_w;
    for( unsigned int i = 0; i < current_h; ++i ) {
      memcpy( &gl_d3d_frame_buffer[0] + (current_h - i -1 )*row_length,
              &gl_frame_buffer[0] + i *row_length, 
              row_length );
    }

    /*   
         // Debug code to save the content of gl_frame_buffer to file to see the 
         // values.
    static TimeStamp last;
    if( TimeStamp() - last > 2 ) {
      last = TimeStamp();
      stringstream s;
      s << "c:\\fb0.bmp";
      FIBITMAP *bitmap = FreeImage_Allocate( current_w, current_h, 32 );
      BYTE *data = FreeImage_GetBits( bitmap );
      memcpy( data, &gl_frame_buffer[0], current_w*current_h*4 );
      FreeImage_Save( FIF_BMP, bitmap, s.str().c_str() );
      FreeImage_Unload( bitmap );
      }*/
  }
#endif

  theWxGLCanvas->SwapBuffers();
}

void WxWidgetsWindow::makeWindowActive() {
  if( theWxGLCanvas->IsShownOnScreen() ) {
    theWxGLCanvas->SetCurrent( *theWxGLContext );
    window_is_made_active = true;
  }
}

BEGIN_EVENT_TABLE(WxWidgetsWindow::MyWxGLCanvas, wxGLCanvas)
  EVT_IDLE(WxWidgetsWindow::MyWxGLCanvas::OnIdle)
  EVT_SIZE(WxWidgetsWindow::MyWxGLCanvas::OnSize)
  EVT_PAINT(WxWidgetsWindow::MyWxGLCanvas::OnPaint)
  EVT_ERASE_BACKGROUND(WxWidgetsWindow::MyWxGLCanvas::OnEraseBackground)
#ifndef WIN32
  EVT_KEY_DOWN( WxWidgetsWindow::MyWxGLCanvas::OnKeyDown )
  EVT_KEY_UP( WxWidgetsWindow::MyWxGLCanvas::OnKeyUp )
  EVT_CHAR( WxWidgetsWindow::MyWxGLCanvas::OnCharDown )
  EVT_LEFT_DOWN( WxWidgetsWindow::MyWxGLCanvas::onLeftMouseButtonDown )
  EVT_LEFT_UP( WxWidgetsWindow::MyWxGLCanvas::onLeftMouseButtonUp )
  EVT_LEFT_DCLICK( WxWidgetsWindow::MyWxGLCanvas::onLeftMouseButtonDown )
  EVT_MIDDLE_DOWN( WxWidgetsWindow::MyWxGLCanvas::onMiddleMouseButtonDown )
  EVT_MIDDLE_UP( WxWidgetsWindow::MyWxGLCanvas::onMiddleMouseButtonUp )
EVT_MIDDLE_DCLICK( WxWidgetsWindow::MyWxGLCanvas::onMiddleMouseButtonDown )
  EVT_RIGHT_DOWN( WxWidgetsWindow::MyWxGLCanvas::onRightMouseButtonDown )
  EVT_RIGHT_UP( WxWidgetsWindow::MyWxGLCanvas::onRightMouseButtonUp )
  EVT_RIGHT_DCLICK( WxWidgetsWindow::MyWxGLCanvas::onRightMouseButtonDown )
  EVT_MOTION( WxWidgetsWindow::MyWxGLCanvas::onMouseMotion )
  EVT_MOUSEWHEEL( WxWidgetsWindow::MyWxGLCanvas::onMouseWheelRotation )
#endif
END_EVENT_TABLE()

WxWidgetsWindow::MyWxGLCanvas::MyWxGLCanvas( WxWidgetsWindow *_myOwner,
                                                  wxWindow* _parent,
                                                  wxWindowID _id,
                                                  const wxPoint& _pos,
                                                  const wxSize& _size,
                                                  int* _attribList,
                                                  long _style, 
                                                  const wxString& _name,
                                                  const wxPalette& _palette ):
wxGLCanvas( _parent, _id, _attribList, _pos, _size,
            _style | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, _name,
                                                _palette ),
myOwner( _myOwner )
{
}


void WxWidgetsWindow::MyWxGLCanvas::OnIdle(wxIdleEvent& event) {

  if( myOwner && myOwner->is_initialized ) {
    if( WxFrame * owner_is_frame = dynamic_cast< WxFrame * >(myOwner->theWindow) ) {
      owner_is_frame->updateFrameRates();
      if( myOwner->allow_new_pixel_format_creation && owner_is_frame->isFileLoaded() )
        myOwner->allow_new_pixel_format_creation = false;
    }
   
    // resize the window if the size is different from the current size.
    int w = myOwner->width->getValue();
    int h = myOwner->height->getValue();
    int current_w, current_h;
    GetSize( &current_w, &current_h );
    if( w != current_w || h != current_h ) {
      SetSize( w, h );
      myOwner->theWindow->SetClientSize( GetSize() );
    }
                                                           
  }
  event.RequestMore();
}

void WxWidgetsWindow::MyWxGLCanvas::OnSize( wxSizeEvent& event ) {
#if wxOSX_USE_CARBON
  wxGLCanvas::OnSize(event);
#endif
  // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
  int w, h;
  GetClientSize(&w, &h);
  if( myOwner ) {
    RenderMode::Mode stereo_mode = myOwner->renderMode->getRenderMode();
    if( stereo_mode != RenderMode::NVIDIA_3DVISION ) {
      myOwner->reshape( w, h );
    }
  }
}

void WxWidgetsWindow::MyWxGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);  
  myOwner->display();
}

void WxWidgetsWindow::MyWxGLCanvas::OnEraseBackground(
  wxEraseEvent& WXUNUSED(event))
{
  // Do nothing, to avoid flashing.
}

int WxWidgetsWindow::setCursorType( const std::string & cursor_type ) {
  if( cursor_type == "DEFAULT" ) {
    theWxGLCanvas->SetCursor( wxNullCursor );
  } else if( cursor_type == "HOURGLASS" ) {
    theWxGLCanvas->SetCursor( *wxHOURGLASS_CURSOR );
  } else if( cursor_type == "STANDARD" ) {
    theWxGLCanvas->SetCursor( *wxSTANDARD_CURSOR );
  } else if( cursor_type == "CROSS" ) {
    theWxGLCanvas->SetCursor( *wxCROSS_CURSOR );
  } else if( cursor_type == "ARROW" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_ARROW ) );
  } else if( cursor_type == "RIGHT_ARROW" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_RIGHT_ARROW ) );
  } else if( cursor_type == "BLANK" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_BLANK ) );
  } else if( cursor_type == "BULLSEYE" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_BULLSEYE ) );
  } else if( cursor_type == "CHAR" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_CHAR ) );
  } else if( cursor_type == "CROSS" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_CROSS ) );
  } else if( cursor_type == "HAND" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_HAND ) );
  } else if( cursor_type == "IBEAM" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_IBEAM ) );
  } else if( cursor_type == "LEFT_BUTTON" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_LEFT_BUTTON ) );
  } else if( cursor_type == "MAGNIFIER" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_MAGNIFIER ) );
  } else if( cursor_type == "MIDDLE_BUTTON" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_MIDDLE_BUTTON ) );
  } else if( cursor_type == "NO_ENTRY" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_NO_ENTRY ) );
  } else if( cursor_type == "PAINT_BRUSH" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_PAINT_BRUSH ) );
  } else if( cursor_type == "PENCIL" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_PENCIL ) );
  } else if( cursor_type == "POINT_LEFT" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_POINT_LEFT ) );
  } else if( cursor_type == "POINT_RIGHT" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_POINT_RIGHT ) );
  } else if( cursor_type == "QUESTION_ARROW" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_QUESTION_ARROW ) );
  } else if( cursor_type == "RIGHT_BUTTON" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_RIGHT_BUTTON ) );
  } else if( cursor_type == "SIZENESW" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_SIZENESW ) );
  } else if( cursor_type == "SIZENS" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_SIZENS ) );
  } else if( cursor_type == "SIZENWSE" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_SIZENWSE ) );
  } else if( cursor_type == "SIZEWE" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_SIZEWE ) );
  } else if( cursor_type == "SIZING" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_SIZING ) );
  } else if( cursor_type == "SPRAYCAN" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_SPRAYCAN ) );
  } else if( cursor_type == "WAIT" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_WAIT ) );
  } else if( cursor_type == "WATCH" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_WATCH ) );
  } else if( cursor_type == "ARROWWAIT" ) {
    theWxGLCanvas->SetCursor( wxCursor( wxCURSOR_ARROWWAIT ) );
  } else {
    return -1;
  }
  return 0;
}

void WxWidgetsWindow::getSupportedCursorsTypes( vector< string > &valid_values ) {
  if( wxHOURGLASS_CURSOR->Ok() ) valid_values.push_back( "HOURGLASS" );
  if( wxSTANDARD_CURSOR->Ok() ) valid_values.push_back( "STANDARD" );
  if( wxCROSS_CURSOR->Ok() ) valid_values.push_back( "CROSS" );
  if( wxCursor( wxCURSOR_WAIT ).Ok() ) valid_values.push_back( "WAIT" );
  if( wxCursor( wxCURSOR_ARROW ).Ok() ) valid_values.push_back( "ARROW" );
  if( wxCursor( wxCURSOR_RIGHT_ARROW ).Ok() ) valid_values.push_back( "RIGHT_ARROW" );
  if( wxCursor( wxCURSOR_BLANK ).Ok() ) valid_values.push_back( "BLANK" );
  if( wxCursor( wxCURSOR_BULLSEYE ).Ok() ) valid_values.push_back( "BULLSEYE" );
  if( wxCursor( wxCURSOR_CHAR ).Ok() ) valid_values.push_back( "CHAR" );
  if( wxCursor( wxCURSOR_CROSS ).Ok() ) valid_values.push_back( "CROSS" );
  if( wxCursor( wxCURSOR_HAND ).Ok() ) valid_values.push_back( "HAND" );
  if( wxCursor( wxCURSOR_IBEAM ).Ok() ) valid_values.push_back( "IBEAM" );
  if( wxCursor( wxCURSOR_LEFT_BUTTON ).Ok() ) valid_values.push_back( "LEFT_BUTTON" );
  if( wxCursor( wxCURSOR_MAGNIFIER ).Ok() ) valid_values.push_back( "MAGNIFIER" );
  if( wxCursor( wxCURSOR_MIDDLE_BUTTON ).Ok() ) valid_values.push_back( "MIDDLE_BUTTON" );
  if( wxCursor( wxCURSOR_NO_ENTRY ).Ok() ) valid_values.push_back( "NO_ENTRY" );
  if( wxCursor( wxCURSOR_PAINT_BRUSH ).Ok() ) valid_values.push_back( "PAINT_BRUSH" );
  if( wxCursor( wxCURSOR_PENCIL ).Ok() ) valid_values.push_back( "PENCIL" );
  if( wxCursor( wxCURSOR_POINT_LEFT ).Ok() ) valid_values.push_back( "POINT_LEFT" );
  if( wxCursor( wxCURSOR_POINT_RIGHT ).Ok() ) valid_values.push_back( "POINT_RIGHT" );
  if( wxCursor( wxCURSOR_QUESTION_ARROW ).Ok() ) valid_values.push_back( "QUESTION_ARROW" );
  if( wxCursor( wxCURSOR_RIGHT_BUTTON ).Ok() ) valid_values.push_back( "RIGHT_BUTTON" );
  if( wxCursor( wxCURSOR_SIZENESW ).Ok() ) valid_values.push_back( "SIZENESW" );
  if( wxCursor( wxCURSOR_SIZENS ).Ok() ) valid_values.push_back( "SIZENS" );
  if( wxCursor( wxCURSOR_SIZENWSE ).Ok() ) valid_values.push_back( "SIZENWSE" );
  if( wxCursor( wxCURSOR_SIZEWE ).Ok() ) valid_values.push_back( "SIZEWE" );
  if( wxCursor( wxCURSOR_SIZING ).Ok() ) valid_values.push_back( "SIZING" );
  if( wxCursor( wxCURSOR_SPRAYCAN ).Ok() ) valid_values.push_back( "SPRAYCAN" );
  if( wxCursor( wxCURSOR_WAIT ).Ok() ) valid_values.push_back( "WAIT" );
  if( wxCursor( wxCURSOR_WATCH ).Ok() ) valid_values.push_back( "WATCH" );
  if( wxCursor( wxCURSOR_ARROWWAIT ).Ok() ) valid_values.push_back( "ARROWWAIT" );
}

string WxWidgetsWindow::getCursorForMode( const string &mode ) {
  if( mode == "ON_SENSOR_OVER" ) {
    return "HAND";
  } else if( mode == "ON_SENSOR_ACTIVE" ) {
    return "HAND";
  } else if( mode == "ON_NAV_LOOKAT" ) {
    if( cursorType->isValidValue( "CROSS" )  ) {
      return "CROSS";
    } else {
      return "HAND";
    }
  } 

  return "DEFAULT";
}

#ifndef WIN32
void WxWidgetsWindow::MyWxGLCanvas::OnKeyDown(wxKeyEvent& event)
{
  switch( event.GetKeyCode() ) {
    case WXK_F1: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F1, true ); break;
    case WXK_F2: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F2, true ); break;
    case WXK_F3: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F3, true ); break;
    case WXK_F4: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F4, true ); break;
    case WXK_F5: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F5, true ); break;
    case WXK_F6: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F6, true ); break;
    case WXK_F7: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F7, true ); break;
    case WXK_F8: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F8, true ); break;
    case WXK_F9: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F9, true ); break;
    case WXK_F10: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F10, true );
      break;
    case WXK_F11: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F11, true );
      break;
    case WXK_F12: myOwner->onKeyDown( X3DKeyDeviceSensorNode::F12, true );
      break;
    case WXK_HOME: myOwner->onKeyDown( X3DKeyDeviceSensorNode::HOME, true );
      break;
    case WXK_END: myOwner->onKeyDown( X3DKeyDeviceSensorNode::END, true );
      break;
    case WXK_PAGEUP: myOwner->onKeyDown( X3DKeyDeviceSensorNode::PGUP, true );
      break;
    case WXK_PAGEDOWN: myOwner->onKeyDown( X3DKeyDeviceSensorNode::PGDN, true );
      break;
    case WXK_UP: myOwner->onKeyDown( X3DKeyDeviceSensorNode::UP, true ); break;
    case WXK_DOWN: myOwner->onKeyDown( X3DKeyDeviceSensorNode::DOWN, true );
      break;
    case WXK_LEFT: myOwner->onKeyDown( X3DKeyDeviceSensorNode::LEFT, true );
      break;
    case WXK_RIGHT: myOwner->onKeyDown( X3DKeyDeviceSensorNode::RIGHT, true );
      break;
    case WXK_MENU: myOwner->onKeyDown( X3DKeyDeviceSensorNode::ALT, true );
      break;
    case WXK_CONTROL: myOwner->onKeyDown( X3DKeyDeviceSensorNode::CONTROL,
                        true ); break;
    case WXK_SHIFT: myOwner->onKeyDown( X3DKeyDeviceSensorNode::SHIFT, true );
      break;
    default: { event.Skip(); }
  }
}

void WxWidgetsWindow::MyWxGLCanvas::OnKeyUp(wxKeyEvent& event)
{
  switch( event.GetKeyCode() ) {
    case WXK_F1: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F1, true ); break;
    case WXK_F2: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F2, true ); break;
    case WXK_F3: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F3, true ); break;
    case WXK_F4: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F4, true ); break;
    case WXK_F5: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F5, true ); break;
    case WXK_F6: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F6, true ); break;
    case WXK_F7: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F7, true ); break;
    case WXK_F8: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F8, true ); break;
    case WXK_F9: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F9, true ); break;
    case WXK_F10: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F10, true ); break;
    case WXK_F11: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F11, true ); break;
    case WXK_F12: myOwner->onKeyUp( X3DKeyDeviceSensorNode::F12, true ); break;
    case WXK_HOME: myOwner->onKeyUp( X3DKeyDeviceSensorNode::HOME, true );
      break;
    case WXK_END: myOwner->onKeyUp( X3DKeyDeviceSensorNode::END, true );
      break;
    case WXK_PAGEUP: myOwner->onKeyUp( X3DKeyDeviceSensorNode::PGUP, true );
      break;
    case WXK_PAGEDOWN: myOwner->onKeyUp( X3DKeyDeviceSensorNode::PGDN, true );
      break;
    case WXK_UP: myOwner->onKeyUp( X3DKeyDeviceSensorNode::UP, true ); break;
    case WXK_DOWN: myOwner->onKeyUp( X3DKeyDeviceSensorNode::DOWN, true );
      break;
    case WXK_LEFT: myOwner->onKeyUp( X3DKeyDeviceSensorNode::LEFT, true );
      break;
    case WXK_RIGHT: myOwner->onKeyUp( X3DKeyDeviceSensorNode::RIGHT, true );
      break;
    case WXK_MENU: myOwner->onKeyUp( X3DKeyDeviceSensorNode::ALT, true );
      break;
    case WXK_CONTROL: myOwner->onKeyUp( X3DKeyDeviceSensorNode::CONTROL,
                        true ); break;
    case WXK_SHIFT: myOwner->onKeyUp( X3DKeyDeviceSensorNode::SHIFT, true );
      break;
    default: { myOwner->onKeyUp(event.GetKeyCode(), false ); }
  }
}

void WxWidgetsWindow::MyWxGLCanvas::OnCharDown(wxKeyEvent& event)
{
  myOwner->onKeyDown( event.GetKeyCode(), false );
  event.Skip();
}

void WxWidgetsWindow::MyWxGLCanvas::onLeftMouseButtonDown(
  wxMouseEvent & event ) {
  myOwner->onMouseButtonAction( MouseSensor::LEFT_BUTTON, MouseSensor::DOWN );
  event.Skip();
}

void WxWidgetsWindow::MyWxGLCanvas::onLeftMouseButtonUp(
  wxMouseEvent & event ) {
  myOwner->onMouseButtonAction( MouseSensor::LEFT_BUTTON, MouseSensor::UP );
}

void WxWidgetsWindow::MyWxGLCanvas::onMiddleMouseButtonDown(
  wxMouseEvent & event ) {
  myOwner->onMouseButtonAction( MouseSensor::MIDDLE_BUTTON, MouseSensor::DOWN );
}

void WxWidgetsWindow::MyWxGLCanvas::onMiddleMouseButtonUp(
  wxMouseEvent & event ) {
  myOwner->onMouseButtonAction( MouseSensor::MIDDLE_BUTTON, MouseSensor::UP );
}

void WxWidgetsWindow::MyWxGLCanvas::onRightMouseButtonDown(
  wxMouseEvent & event ) {
  myOwner->onMouseButtonAction( MouseSensor::RIGHT_BUTTON, MouseSensor::DOWN );
}

void WxWidgetsWindow::MyWxGLCanvas::onRightMouseButtonUp(
  wxMouseEvent & event ) {
  myOwner->onMouseButtonAction( MouseSensor::RIGHT_BUTTON, MouseSensor::UP );
}
void WxWidgetsWindow::MyWxGLCanvas::onMouseMotion( wxMouseEvent & event ) {
  myOwner->onMouseMotionAction( event.GetX(), event.GetY() );
}
void WxWidgetsWindow::MyWxGLCanvas::onMouseWheelRotation(
  wxMouseEvent & event ) {
  myOwner->onMouseWheelAction(
    event.GetWheelRotation() > 0 ? MouseSensor::FROM : MouseSensor::TOWARDS );
}


#endif

#ifdef HAVE_DX9
#include <d3d9.h>
#include <d3dx9.h>
D3DPRESENT_PARAMETERS d3dpp;
IDirect3DSurface9* src_surface = NULL;
IDirect3DSurface9* back_surface = NULL;
//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                    g_hWnd          = NULL;
LPDIRECT3D9             g_pD3D          = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice    = NULL;

// ---------------------------------------------------------------------------
// D3D9Canvas
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(WxWidgetsWindow::D3D9Canvas, wxWindow)
    EVT_SIZE(WxWidgetsWindow::D3D9Canvas::OnSize)
    EVT_PAINT(WxWidgetsWindow::D3D9Canvas::OnPaint)
    EVT_ERASE_BACKGROUND(WxWidgetsWindow::D3D9Canvas::OnEraseBackground)
    EVT_IDLE(WxWidgetsWindow::D3D9Canvas::OnIdle)
END_EVENT_TABLE()

WxWidgetsWindow::D3D9Canvas::D3D9Canvas(WxWidgetsWindow *_owner,
                                        wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxWindow(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name),
  owner( _owner )
{
    m_init = false;
   g_hWnd = (HWND)GetHWND();
}

WxWidgetsWindow::D3D9Canvas::~D3D9Canvas()
{
  if(src_surface){
    src_surface->Release();
    src_surface = NULL;
  }
  
  if( back_surface ) {
    back_surface->Release();
    back_surface = NULL;
  }

  if( g_pd3dDevice != NULL ) {
    g_pd3dDevice->Release();
    g_pd3dDevice = NULL;
  }

  if( g_pD3D != NULL ) {
    g_pD3D->Release();
    g_pD3D = NULL;
  }
}

void WxWidgetsWindow::D3D9Canvas::OnIdle(wxIdleEvent &event) {
  event.RequestMore(true);
  // resize the window if the size is different from the current size.
  int w = owner->width->getValue();
  int h = owner->height->getValue();
  int current_w, current_h;
  GetSize( &current_w, &current_h );
  if( w != current_w || h != current_h ) {
    SetSize( w, h );
    owner->theWindow->SetClientSize( GetSize() );
  }
  
  Render();
}

void WxWidgetsWindow::D3D9Canvas::OnPaint( wxPaintEvent& WXUNUSED(event) ) {

  wxPaintDC dc(this);
  
  // Initialize D3D
  if (!m_init) {
    InitD3D();
    m_init= true;
  }
}

// Stereo Blitdefines
#define NVSTEREO_IMAGE_SIGNATURE 0x4433564e //NV3D
typedef struct _Nv_Stereo_Image_Header {
  unsigned int dwSignature;
  unsigned int dwWidth;
  unsigned int dwHeight;
  unsigned int dwBPP;
  unsigned int dwFlags;
} NVSTEREOIMAGEHEADER, *LPNVSTEREOIMAGEHEADER;

// ORedflags in the dwFlagsfielsof the _Nv_Stereo_Image_Headerstructure above
#define SIH_SWAP_EYES 0x00000001
#define SIH_SCALE_TO_FIT 0x00000002

void WxWidgetsWindow::D3D9Canvas::updateGLFrameBufferSize( int w, int h ) {
  if(src_surface)
    src_surface->Release();
  
  if( w * h > 0 ) {

    // generator buffer needs to be twice the size of the frame buffer
    // to fit two stereo images for NVidia 3DVision stereo
    owner->generator->width->setValue( w * 2 );
    owner->generator->height->setValue( h );

    /// size in bytes of generator frame buffer (RGBA)
    unsigned int frame_buffer_size = 2 * w * h * 4;
    if( owner->gl_frame_buffer.size() < frame_buffer_size ) {
      owner->gl_frame_buffer.resize( frame_buffer_size );
    }
    
    if( owner->gl_d3d_frame_buffer.size() < frame_buffer_size ) {
      owner->gl_d3d_frame_buffer.resize( frame_buffer_size );
    } 
    
    /// create DirectX offscreen surface to render 3DVision frame to. A 
    /// 3DVision frame is a vertical split stereo image pair with left 
    /// and right eye next to each other as well as one extra row 
    /// with a stereo header.
    if( g_pd3dDevice->CreateOffscreenPlainSurface(w * 2, h + 1, 
                                                  D3DFMT_A8R8G8B8, 
                                                  D3DPOOL_DEFAULT, 
                                                  &src_surface, 
                                                  NULL) != D3D_OK ) {
      Console(4) << "Warning: Could not create DirectX offscreen "
                 << "surface for NVidia 3DVision rendering." << endl;
    } else {
      // Add NVidia stereo header in last row of offscreen surface.

      // Lock the stereo image
      D3DLOCKED_RECT lr;
      src_surface->LockRect(&lr,NULL,0);
      // write stereo signature in the last raw of the stereo image
      LPNVSTEREOIMAGEHEADER pSIH=
        (LPNVSTEREOIMAGEHEADER)(((unsigned char *) lr.pBits) + (lr.Pitch* h));
      // Update the signature header values
      pSIH->dwSignature= NVSTEREO_IMAGE_SIGNATURE;
      pSIH->dwBPP= 32;
      pSIH->dwFlags= 0;//SIH_SWAP_EYES; // Srcimage has left on left and right on right
      pSIH->dwWidth= w*2;
      pSIH->dwHeight= h;
      // Unlock surface
      src_surface->UnlockRect();
    }
  }
}

void WxWidgetsWindow::D3D9Canvas::OnSize(wxSizeEvent& event)
{
  // Initialize D3D
  if (!m_init) {
    InitD3D();
    m_init= true;
  }

  // get the size of the canvas.
  int w, h;
  GetClientSize(&w, &h);
  
  // we need to reset the d3d device. In order to do that we are required
  // to first delete all surfaces that have been created previously and 
  // then recreate them with the new reset d3d device.
  d3dpp.BackBufferHeight = h; 
  d3dpp.BackBufferWidth = w;
  d3dpp.BackBufferFormat =  D3DFMT_UNKNOWN;
  
  // release surfaces
  if(src_surface){
    src_surface->Release();
    src_surface = NULL;
  }
  
  if( back_surface ) {
    back_surface->Release();
    back_surface = NULL;
  }
  
  // reset device
  HRESULT hr = g_pd3dDevice->Reset( &d3dpp );
  if(  hr != D3D_OK ) {
    Console(4) << "DirectX device reset failed: ";
    if( hr == D3DERR_DEVICELOST ) 
      Console(4) << "Device lost" << endl;
    else if( hr == D3DERR_DEVICEREMOVED ) 
      Console(4) << "Device removed" << endl;
    else if( hr == D3DERR_DRIVERINTERNALERROR )
      Console(4) << "Driver internal error" << endl;
    else if( hr == D3DERR_OUTOFVIDEOMEMORY )
      Console(4) << "Out of mem" << endl;
    else
      Console(4) << "Unknown error"<< endl;
  } 

  // restore d3d surfaces with the new size
  g_pd3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&back_surface );
  
  owner->reshape( w, h );
  updateGLFrameBufferSize( w, h );
}

void WxWidgetsWindow::D3D9Canvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
    // Do nothing, to avoid flashing on MSW
}

void WxWidgetsWindow::D3D9Canvas::InitD3D() {
  g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

  D3DDISPLAYMODE d3ddm;
  g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
  
  //D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory( &d3dpp, sizeof(d3dpp) );
  
  d3dpp.Windowed               = TRUE;
  d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat       = d3ddm.Format;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
  d3dpp.hDeviceWindow = g_hWnd;
    
  g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
                        &d3dpp, &g_pd3dDevice );

  g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}



void WxWidgetsWindow::D3D9Canvas::Render() {
  // 
  int w, h;
  GetClientSize(&w, &h); 
 
  bool render = src_surface != NULL;
  
  if( render ) {
    
    // DEBUG: Save src_surface to bitmap file/
    //D3DXSaveSurfaceToFile( L"c:\\test.bmp", D3DXIFF_BMP, src_surface, NULL, NULL ); 
    
    // copy the data aquired from OpenGL frame buffer to DirectX src_surface.
    RECT dstRect= { 0, 0, w*2, h};   
    if(D3DXLoadSurfaceFromMemory(src_surface, 
                                 NULL, 
                                 &dstRect,
                                 &owner->gl_d3d_frame_buffer[0],
                                 D3DFMT_A8R8G8B8,
                                 4*owner->generator->width->getValue(),
                                 NULL,
                                 &dstRect,
                                 D3DX_DEFAULT,
                                 0)!=D3D_OK ) {
      Console(4) <<"Warning: Error loading OpenGL data to DirectX offscreen surface" << endl;
    }
    
    // Debug: save surface to bitmap file
    //D3DXSaveSurfaceToFile( L"c:\\test.bmp", D3DXIFF_BMP, src_surface, NULL, NULL ); 
  }
  
  // Stretch the offscreen surface to the back buffer.
  g_pd3dDevice->BeginScene();
  if(render) 
    g_pd3dDevice->StretchRect(src_surface, NULL, back_surface, NULL, D3DTEXF_NONE);
  g_pd3dDevice->EndScene();
  g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
  back_surface->Release();
}

#endif // HAVE_DX9
