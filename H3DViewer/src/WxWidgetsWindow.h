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
/// \file H3DWxWidgetsWindow.h
/// \brief Header file for H3DWxWidgetsWindow.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __WXWIDGETSWINDOW_H__
#define __WXWIDGETSWINDOW_H__

#include "wx/wx.h"
#include <H3D/H3DWindowNode.h>
//#include "Scene.h"
#include "wx/glcanvas.h"

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

namespace H3D {

  /// \ingroup Nodes
  /// \class WxWidgetsWindow
  /// \brief H3DWindowNode implemented using wxWidgets. 
  /// 
  class WxWidgetsWindow : public H3DWindowNode {
  public:

    class MyWxGLCanvas: public wxGLCanvas
    {
    public:
      MyWxGLCanvas(WxWidgetsWindow *_myOwner, wxWindow* _parent,
                   wxWindowID _id, const wxPoint& _pos, const wxSize& _size,
                   int* _attribList = 0, long _style=0, 
                   const wxString& _name = wxT("MyWxGLCanvas"),
                   const wxPalette& _palette = wxNullPalette );
      void OnSize(wxSizeEvent& event);
      void OnPaint(wxPaintEvent& event);
      void OnEraseBackground(wxEraseEvent& WXUNUSED(event));
      void OnIdle(wxIdleEvent& event);
#ifndef WIN32
      void OnKeyDown( wxKeyEvent& event );
      void OnKeyUp( wxKeyEvent& event );
      void OnCharDown( wxKeyEvent& event );
      void onLeftMouseButtonDown( wxMouseEvent & event );
      void onLeftMouseButtonUp( wxMouseEvent & event );
      void onMiddleMouseButtonDown( wxMouseEvent & event );
      void onMiddleMouseButtonUp( wxMouseEvent & event );
      void onRightMouseButtonDown( wxMouseEvent & event );
      void onRightMouseButtonUp( wxMouseEvent & event );
      void onMouseMotion( wxMouseEvent & event );
      void onMouseWheelRotation( wxMouseEvent & event );
#endif

    protected:
      WxWidgetsWindow * myOwner;
      DECLARE_EVENT_TABLE()
    };

    /// Constructor.
    WxWidgetsWindow( wxWindow *_theParent = 0,
      Inst< SFInt32     > _width      = 0,
                Inst< SFInt32     > _height     = 0,
                Inst< SFBool      > _fullscreen = 0,
                Inst< SFBool      > _mirrored   = 0,
                Inst< RenderMode  > _renderMode = 0, 
                Inst< SFViewpoint > _viewpoint  = 0,
                Inst< SFTime      > _time       = 0 );

    ///// Destructor.
    //~WxWidgetsWindow();

    /// Calls wxGLCanvas::SwapBuffers
    virtual void swapBuffers(); 

    /// Creates and initializes a wxWindow for OpenGL rendering
    virtual void initWindow();

    /// Initializes wxWindow
    virtual void initWindowHandler() { }

    /// Sets fullscreen mode.
    virtual void setFullscreen( bool fullscreen );

    /// Make this the current window.
    virtual void makeWindowActive();
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    wxWindow * theWindow;
    MyWxGLCanvas * theWxGLCanvas;
	bool last_fullscreen;
  };
}

#endif
