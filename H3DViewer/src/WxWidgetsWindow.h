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
/// \file H3DWxWidgetsWindow.h
/// \brief Header file for H3DWxWidgetsWindow.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __WXWIDGETSWINDOW_H__
#define __WXWIDGETSWINDOW_H__

#include <wx/wx.h>

#include <H3D/H3DWindowNode.h>
#include <H3DViewerConfig.h>
#include <wx/glcanvas.h>
#include <wx/dnd.h>
#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

namespace H3D {

  /// \ingroup Nodes
  /// \class WxWidgetsWindow
  /// \brief H3DWindowNode implemented using wxWidgets. 
  /// 
  /// Valid values for the cursorType field depend on the platform and
  /// are different on Windows, OSX and Linux. Use cursorType->getValidValues()
  /// to get the ones supported.
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

      // Destructor
      ~MyWxGLCanvas() {
        // Set new drop target, which deletes the old one. Needed for memory
        // cleanup since it seems like wxWidgets does not automatically clean
        // up the drop target at glCanvas deletion.
        SetDropTarget( NULL );
      }
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
      friend class WxWidgetsWindow;
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
                     Inst< SFInt32     > _posX       = 0,
                     Inst< SFInt32     > _posY       = 0,
                     Inst< SFBool      > _manualCursorControl = 0,
                     Inst< SFString    > _cursorType = 0 );

    ///// Destructor.
    ~WxWidgetsWindow() {
      if( theWxGLCanvas )
        theWxGLCanvas->myOwner = NULL;

      if( !have_parent ) {
        theWindow->Destroy();
      }
      
      // Seems like wxWidgets 2.9 does not destruct explicit wxGLCanvas.
      if( theWxGLContext ) {
        delete theWxGLContext;
        theWxGLContext = NULL;
      }
    }

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

    typedef void (*OnDropFileFunc)( wxCoord x, wxCoord y,
                                    const wxArrayString&,
                                    void * );

    /// Set a callback function that will be called when a file is
    /// dragged and dropped over the window.
    void onFileDraggedAndDroppedFunction( OnDropFileFunc func, 
                                          void * arg = NULL ) {
      drag_file_func = func;
      drag_file_func_arg = arg;
    }

  protected:

    /// Set the cursor to the given cursor type. See cursorType field
    /// for valid values. Returns 0 on success. -1 if the cursor_type is 
    /// not supported.
    virtual int setCursorType( const std::string & cursor_type );

    /// Return the cursor type to use for given modes. This should
    /// be implemented for each subclass to choose appropriate cursors.
    /// The standard modes are:
    /// "DEFAULT" - normal mode
    /// "ON_SENSOR_OVER" - when mouse pointer is over a pointing device
    /// sensor.
    /// "ON_SENSOR_ACTIVE" - when a sensor node is active
    /// "ON_NAV_LOOKAT" - when lookat mode is chosen
    string getCursorForMode( const string &mode );

    /// Adds the supported cursor type for the current platform to the
    /// given vector.
    void getSupportedCursorsTypes( vector< string > &types );


#if wxUSE_DRAG_AND_DROP
    
    class DragAndDropFile : public wxFileDropTarget {
    public:
      DragAndDropFile(WxWidgetsWindow *_owner) { owner = _owner; }
      
      virtual bool OnDropFiles(wxCoord x, wxCoord y,
                               const wxArrayString& filenames) {
        if( owner->drag_file_func ) {
          owner->drag_file_func( x, y, filenames, owner->drag_file_func_arg );
        }
        return true;
      }
    protected:
      WxWidgetsWindow *owner;
    };
#endif

    friend class DragAndDropFile;
    OnDropFileFunc drag_file_func;
    void *drag_file_func_arg;

    bool use_h3d_settings;
    bool is_initialized;
    bool have_parent;
    wxWindow * theWindow;
    wxSizer *sizer;
    MyWxGLCanvas * theWxGLCanvas;
    wxGLContext * theWxGLContext;
    bool last_fullscreen;

    bool fullscreen_initialized;
    bool allow_new_pixel_format_creation;

 

#ifdef HAVE_DX9
  vector< unsigned char > gl_frame_buffer;
  vector< unsigned char > gl_d3d_frame_buffer;

  class D3D9Canvas : public wxWindow {
  public:
    D3D9Canvas( WxWidgetsWindow *owner,
                wxWindow *parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxString& name = wxT("TestGLCanvas"));
    
    ~D3D9Canvas();
    void updateGLFrameBufferSize( int w, int h );
  protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnIdle( wxIdleEvent& event );
    
  private:
    void InitD3D();
    void Render();
    bool m_init;
    bool resized;
    WxWidgetsWindow *owner;
    DECLARE_EVENT_TABLE()
  };


  D3D9Canvas *d3d_canvas;

#endif

  private:
      string viewpoint_file;
  };
}

#endif
