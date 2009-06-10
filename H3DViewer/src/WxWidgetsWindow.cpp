//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2007, SenseGraphics AB
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
  theWxGLCanvas( NULL ) {
  type_name = "WxWidgetsWindow";
  database.initFields( this );
  
  have_parent = theWindow != NULL;

  vector< string > valid_values;
  getSupportedCursorsTypes( valid_values );
  cursorType->addValidValues( valid_values.begin(), valid_values.end() );
 
  if( !theWindow ) {
     theWindow = new wxFrame( NULL, wxID_ANY, wxT("WxFrame"),
                              wxPoint( posX->getValue(), posY->getValue() ),
                              wxSize( width->getValue(), height->getValue() ));
  }
}

void WxWidgetsWindow::initWindow() {
  RenderMode::Mode stereo_mode = renderMode->getRenderMode();

  int attribList[8];
  attribList[0] = WX_GL_RGBA;
  attribList[1] = WX_GL_DOUBLEBUFFER;
  attribList[2] = WX_GL_DEPTH_SIZE;
  attribList[3] = 24;
  attribList[4] = WX_GL_STENCIL_SIZE;
  attribList[5] = 8;

  // TODO: FIX stereo mode
#ifdef MACOSX
  // TODO: stereo mode does not work with mac
  attribList[6] = 0;
#else
  if( stereo_mode == RenderMode::QUAD_BUFFERED_STEREO )
    attribList[6] = WX_GL_STEREO;
  attribList[7] = 0;
#endif
  // if we have a previous window, use same rendering context and destroy it.
  MyWxGLCanvas *old_canvas = theWxGLCanvas;

  if( old_canvas ) {
    theWxGLCanvas = 
      new MyWxGLCanvas( this,  theWindow, 
                        old_canvas->GetContext(), -1, wxDefaultPosition,
                        wxSize( width->getValue(), height->getValue() ), 
                        attribList );
    old_canvas->Destroy();
  } else {
    theWxGLCanvas = 
      new MyWxGLCanvas( this, theWindow, -1, wxDefaultPosition,
                        wxSize( width->getValue(), height->getValue() ), 
                        attribList );
  }
  
#if wxUSE_DRAG_AND_DROP
  theWxGLCanvas->SetDropTarget( new DragAndDropFile( this ) );
#endif

#ifdef USE_EXPLICIT_GLCONTEXT
  theWxGLContext = new wxGLContext( theWxGLCanvas );
#endif

  wxSizer *tmp_sizer = theWindow->GetSizer();
  if( tmp_sizer ) {
    tmp_sizer->Add( theWxGLCanvas, 1, wxEXPAND );
    theWindow->Layout();
  }
#ifdef WIN32
  hWnd = (HWND)(theWxGLCanvas->GetHandle());
  if( hWnd )
    wpOrigProc = (WNDPROC) SetWindowLongPtr(hWnd, 
                          GWL_WNDPROC, (LONG_PTR) WindowProc);
#endif

  last_fullscreen = !fullscreen->getValue();
  theWindow->Show();
  theWxGLCanvas->Show();
#ifdef USE_EXPLICIT_GLCONTEXT
  theWxGLCanvas->SetCurrent( *theWxGLContext );
#else
  theWxGLCanvas->SetCurrent();
#endif
  glClearColor( 0, 0, 0, 1 );
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glClear( GL_COLOR_BUFFER_BIT );
  is_initialized = true;
}

void WxWidgetsWindow::setFullscreen( bool fullscreen ) {
  if( last_fullscreen != fullscreen ) {
    wxTopLevelWindow * tlw = dynamic_cast< wxTopLevelWindow * >(theWindow);
#ifndef MACOSX
    //TODO: fullscreen does not work well on macosx
    if( tlw )
      tlw->ShowFullScreen(fullscreen);
#endif
    last_fullscreen = fullscreen;
  }
}

void WxWidgetsWindow::swapBuffers() {
   theWxGLCanvas->SwapBuffers();
}

void WxWidgetsWindow::makeWindowActive() {
#ifdef USE_EXPLICIT_GLCONTEXT
  theWxGLCanvas->SetCurrent( *theWxGLContext );
#else
  theWxGLCanvas->SetCurrent();
#endif
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
#ifdef USE_EXPLICIT_GLCONTEXT
wxGLCanvas( _parent, _id, _attribList, _pos, _size,
            _style | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, _name,
            _palette ),
#else
wxGLCanvas( _parent, _id, _pos, _size,
            _style | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, _name,
	    _attribList,  _palette ),
#endif
myOwner( _myOwner )
{
}

WxWidgetsWindow::MyWxGLCanvas::MyWxGLCanvas( WxWidgetsWindow *_myOwner,
                                             wxWindow* _parent,
                                             wxGLContext *context,
                                             wxWindowID _id,
                                             const wxPoint& _pos,
                                             const wxSize& _size,
                                             int* _attribList,
                                             long _style, 
                                             const wxString& _name,
                                             const wxPalette& _palette ):
#ifdef USE_EXPLICIT_GLCONTEXT
  wxGLCanvas( _parent, context,_id, _attribList, _pos, _size,
            _style | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, _name,
            _palette ),
#else
  wxGLCanvas( _parent, context, _id, _pos, _size,
            _style | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, _name,
	    _attribList,  _palette ),
#endif
myOwner( _myOwner )
{
}

void WxWidgetsWindow::MyWxGLCanvas::OnIdle(wxIdleEvent& event) {
  if( myOwner->is_initialized ) {
    static_cast< WxFrame * >(myOwner->theWindow)->updateFrameRates();
   
    // resize the window if the size is different from the current size.
    int w = myOwner->width->getValue();
    int h = myOwner->height->getValue();
    int current_w, current_h;
    GetSize( &current_w, &current_h );
    if( w != current_w || h != current_h ) {
      SetSize( w, h );
    }

    myOwner->theWindow->Fit();
                                                           
  }
  event.RequestMore();
}

void WxWidgetsWindow::MyWxGLCanvas::OnSize( wxSizeEvent& event ) {
  wxGLCanvas::OnSize(event);
  // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
  int w, h;
  GetClientSize(&w, &h);
#ifdef USE_EXPLICIT_GLCONTEXT
  myOwner->reshape( w, h );
#else
  if ( GetContext() )
    myOwner->reshape( w, h );
#endif
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
    case WXK_PRIOR: myOwner->onKeyDown( X3DKeyDeviceSensorNode::PGUP, true );
      break;
    case WXK_NEXT: myOwner->onKeyDown( X3DKeyDeviceSensorNode::PGDN, true );
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
    case WXK_PRIOR: myOwner->onKeyUp( X3DKeyDeviceSensorNode::PGUP, true );
      break;
    case WXK_NEXT: myOwner->onKeyUp( X3DKeyDeviceSensorNode::PGDN, true );
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
