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
/// \file GLUTWindow.h
/// \brief Header file for GLUTWindow.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GLUTWINDOW_H__
#define __GLUTWINDOW_H__

#include "H3DWindowNode.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class GLUTWindow
  /// \brief H3DWindowNode implementet using GLUT. 
  /// 
  class H3DAPI_API GLUTWindow : public H3DWindowNode {
  public:
    
    /// Constructor.
    GLUTWindow( Inst< SFInt32     > _width      = 0,
                Inst< SFInt32     > _height     = 0,
                Inst< SFBool      > _fullscreen = 0,
                Inst< SFBool      > _mirrored   = 0,
                Inst< RenderMode  > _renderMode = 0, 
                Inst< SFViewpoint > _viewpoint  = 0,
                Inst< SFTime      > _time       = 0 );

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

#if !( defined(FREEGLUT) && defined(WIN32) )
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
#endif

#ifndef WIN32
    /// glut callback function. Calls onMouseButtonAction with the
    /// button and state translated to the enumerated values in KeySensor.h
    static void glutMouseCallback( int button, int state, 
                                   int x, int y );
    
    /// glut callback function. Calls onMouseMotionAction
    static void glutMotionCallback( int x, int y );
#endif

#if defined(FREEGLUT) && !defined(WIN32)
    /// glut callback function for mouse scroll wheel.
    /// Calls onMouseWheelAction with direction translated to the
    /// values in KeySensor.h
    static void glutMouseWheelCallback( int wheel, 
                                        int direction, int x, int y );
#endif


    /// Initialize GLUT. 
    static void initGLUT();
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    static bool GLUT_init;
    int window_id;
  };
}
#endif
