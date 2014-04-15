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
/// \file GLUTWindow.h
/// \brief Header file for GLUTWindow.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GLUTWINDOW_H__
#define __GLUTWINDOW_H__

#include <H3D/H3DWindowNode.h>
#ifdef HAVE_GLUT

namespace H3D {

  /// \ingroup Nodes
  /// \class GLUTWindow
  /// \brief H3DWindowNode implementet using GLUT. 
  /// 
  /// Valid values for the cursorType field are:
  /// "RIGHT_ARROW", "LEFT_ARROW", "INFO", "DESTROY", "HELP",
  /// "CYCLE", "SPRAYCAN", "WAIT", "TEXT", "CROSSHAIR", "UP_DOWN",
  /// "LEFT_RIGHT", "TOP_SIDE", "BOTTOM_SIDE", "LEFT_SIDE", 
  /// "RIGHT_SIDE", "TOP_LEFT_CORNER", "TOP_RIGHT_CORNER",
  /// "BOTTOM_RIGHT_CORNER", "BOTTOM_LEFT_CORNER",
  /// "FULL_CROSSHAIR", "NONE", "DEFAULT"
  ///
  /// For example usage of this class see the H3DLoad application.
  class H3DAPI_API GLUTWindow : public H3DWindowNode {
  public:

    /// Constructor.
    GLUTWindow( Inst< SFInt32       > _width      = 0,
                Inst< SFInt32       > _height     = 0,
                Inst< SFBool        > _fullscreen = 0,
                Inst< SFBool        > _mirrored   = 0,
                Inst< RenderMode    > _renderMode = 0, 
                Inst< SFViewpoint   > _viewpoint  = 0, 
                Inst< SFInt32       > _posX       = 0,
                Inst< SFInt32       > _posY       = 0,
                Inst< SFBool        > _manualCursorControl = 0,
                Inst< SFString      > _cursorType = 0, 
                Inst< SFString      > _gameMode   = 0 );

    /// Destructor.
    ~GLUTWindow();

    /// Calls glutSwapBuffers.
    virtual void swapBuffers(); 

    /// Creates and initializes a GLUT window/
    virtual void initWindow();

    /// Initializes GLUT.
    virtual void initWindowHandler() { initGLUT(); }

    /// Sets fullscreen mode.
    virtual void setFullscreen( bool fullscreen );

    /// Make this the current window.
    virtual void makeWindowActive();

    /// Given the identifier of a GLUT window the GLWindow instance
    /// that created that window is returned.
    static GLUTWindow * getGLUTWindow( int glut_id ); 

    /// Returns the GLUT window id for this window.
    int getGLUTWindowId() {
      return window_id;
    }

    /// GLUT callback function for glutKeyboardFunc.
    static void glutKeyboardDownCallback( unsigned char key, 
                                          int x, int y );
    
    /// GLUT callback function for glutSpecialFunc.
    static void glutSpecialDownCallback( int key, 
                                         int x, int y );

    /// GLUT callback function for glutKeyboardUpFunc.
    static void glutKeyboardUpCallback( unsigned char key, 
                                          int x, int y );
    
    /// GLUT callback function for glutSpecialUpFunc.
    static void glutSpecialUpCallback( int key, 
                                       int x, int y );

    /// glut callback function. Calls onMouseButtonAction with the
    /// button and state translated to the enumerated values in KeySensor.h
    static void glutMouseCallback( int button, int state, 
                                   int x, int y );
    
    /// glut callback function. Calls onMouseMotionAction
    static void glutMotionCallback( int x, int y );

    /// glut callback function for mouse scroll wheel.
    /// Calls onMouseWheelAction with direction translated to the
    /// values in KeySensor.h
    static void glutMouseWheelCallback( int wheel, 
                                        int direction, int x, int y );


    /// Initialize GLUT. 
    static void initGLUT();

    /// Support for GLUT GameMode which is basically a string used to optimize
    /// the glut window for gaming. Some features are disabled for such things.
    /// For more information about GLUT GameMode see for example
    /// http://freeglut.sourceforge.net/docs/api.php#GameMode
    ///
    /// <b>Access type:</b> initializeOnly \n
    auto_ptr< SFString >  gameMode;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Set cursor shape according to given cursor mode.
    virtual int setCursorType( const std::string & cursor_type );

    /// Return the cursor type to use for given mode. This should
    /// be implemented for each subclass to choose appropriate cursors.
    /// 
    /// The standard events are:
    /// "DEFAULT" - normal mode ("DEFAULT")
    /// "ON_SENSOR_OVER" - when mouse pointer is over a pointing device
    /// sensor ("CROSS")
    /// "ON_SENSOR_ACTIVE" - when a sensor node is active ("NAVIGATE" )
    /// "ON_NAV_LOOKAT" - when lookat mode is chosen 
    virtual string getCursorForMode( const string &mode );

    static bool GLUT_init;
    int window_id;

    // Define these even though they are not used for when we use FREEGLUT.
    bool last_fullscreen;
    int last_x_pos, last_y_pos, last_width, last_height;
  };
}
#endif // HAVE_GLUT
#endif
