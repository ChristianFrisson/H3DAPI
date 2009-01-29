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
#ifndef WIN32
#include <H3D/X3DKeyDeviceSensorNode.h>
#include <H3D/MouseSensor.h>
#endif

#include "WxFrame.h"


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
                        Inst< SFTime      > _time     ) :
  H3DWindowNode( _width, _height, _fullscreen, _mirrored, _renderMode,
                 _viewpoint, _time ),
  theWindow( _theParent ){
  type_name = "WxWidgetsWindow";
  database.initFields( this );
  
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
  //  attribList[6] = WX_GL_STEREO;
  attribList[7] = 0;
#endif
 
  if( !theWindow ) {
     theWindow = new wxFrame( NULL, wxID_ANY, wxT("WxFrame"), wxDefaultPosition,
                              wxSize( width->getValue(), height->getValue() ));
  }

  theWxGLCanvas = new MyWxGLCanvas( this, theWindow, -1, wxDefaultPosition,
              wxSize( width->getValue(), height->getValue() ), attribList );
#ifdef WIN32
  hWnd = (HWND)(theWxGLCanvas->GetHandle());
  if( hWnd )
    wpOrigProc = (WNDPROC) SetWindowLongPtr(hWnd, 
                          GWL_WNDPROC, (LONG_PTR) WindowProc);
#endif
}

void WxWidgetsWindow::initWindow() {
  last_fullscreen = !fullscreen->getValue();
  theWindow->Show();
  theWxGLCanvas->Show();
  theWxGLCanvas->SetCurrent();
  glClearColor( 0, 0, 0, 1 );
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glClear( GL_COLOR_BUFFER_BIT );
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
  theWxGLCanvas->SetCurrent();
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
wxGLCanvas(_parent, _id, _pos, _size, _style | wxFULL_REPAINT_ON_RESIZE, _name,
           _attribList, _palette),
myOwner( _myOwner )
{
}

void WxWidgetsWindow::MyWxGLCanvas::OnIdle(wxIdleEvent& event) {
  static_cast< WxFrame * >(myOwner->theWindow)->updateFrameRates();
  event.RequestMore();
}

void WxWidgetsWindow::MyWxGLCanvas::OnSize( wxSizeEvent& event ) {
  wxGLCanvas::OnSize(event);
  // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
  int w, h;
  GetClientSize(&w, &h);
  if ( GetContext() )
    myOwner->reshape( w, h );
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
