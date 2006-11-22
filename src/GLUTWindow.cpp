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
/// \file GLUTWindow.cpp
/// \brief CPP file for GLUTWindow.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "GLUTWindow.h"
#if !( defined(FREEGLUT) && defined(WIN32) )
#include "X3DKeyDeviceSensorNode.h"
#endif
#ifndef WIN32
#include "MouseSensor.h"
#endif
#include <GL/glew.h>
#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifdef FREEGLUT
#include <GL/freeglut.h>
#if defined(_MSC_VER) || defined(__BORLANDC__)
#pragma comment( lib, "freeglut.lib" )
#endif
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GLUTWindow::database( "GLUTWindow", 
                                      &(newInstance<GLUTWindow>), 
                                      typeid( GLUTWindow ),
                                      &(H3DWindowNode::database) );

/////////////////////////////////////////////////////////////////////////////
//
// Internal functions for GLUT window handling
//
namespace GLUTWindowInternals {
  void reshapeFunc( int w, int h ) { 
    GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
    if( window )
      window->reshape( w, h );
  }

  void displayFunc() {
    GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
    if( window )
      window->display();
  }
}

bool GLUTWindow::GLUT_init = false;

void GLUTWindow::initGLUT() {
  if ( !GLUT_init ) {
    char *argv[1] = { "H3DLoad" };
    int argc = 1;
    glutInit(&argc, argv);
    GLUT_init = true;
  }
}

GLUTWindow::GLUTWindow( Inst< SFInt32     > _width,
                        Inst< SFInt32     > _height,
                        Inst< SFBool      > _fullscreen,
                        Inst< SFBool      > _mirrored,
                        Inst< RenderMode  > _renderMode, 
                        Inst< SFViewpoint > _viewpoint,
                        Inst< SFTime      > _time     ) :
  H3DWindowNode( _width, _height, _fullscreen, _mirrored, _renderMode,
                 _viewpoint, _time ) {
  
  type_name = "GLUTWindow";
  database.initFields( this );
}

GLUTWindow::~GLUTWindow() {
  if ( window_id > 0 ) {
    // If the window is closed with the x-button glut can be deinitialized
    // before getting here, so make sure glut is initialized before calling
    // glutDestroyWindow
#ifdef FREEGLUT
    if( glutGet( GLUT_INIT_STATE ) ) {
#else
    if( GLUT_init ) {
#endif
      glutDestroyWindow( window_id );
    }
  }
}

void GLUTWindow::initWindow() {
  unsigned int mode = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL;
  RenderMode::Mode stereo_mode = renderMode->getRenderMode();
    
  if( stereo_mode == RenderMode::VERTICAL_INTERLACED ||
      stereo_mode == RenderMode::HORIZONTAL_INTERLACED ||
      stereo_mode == RenderMode::VERTICAL_INTERLACED_GREEN_SHIFT ) {
    //mode |= GLUT_STENCIL;
  } else  if( stereo_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
    mode |= GLUT_STEREO;
  }
  glutInitDisplayMode( mode );
  glutInitWindowSize( width->getValue(), height->getValue() );
  window_id = glutCreateWindow( "H3D" );
  glutSetWindow( window_id );
  // set up GLUT callback functions
  glutDisplayFunc ( GLUTWindowInternals::displayFunc  );
  glutReshapeFunc ( GLUTWindowInternals::reshapeFunc  );
  // Only if FREEGLUT and WIN32 is defined the H3DWindowNode callback is used
#if !( defined(FREEGLUT) && defined(WIN32) )
  glutKeyboardFunc( glutKeyboardDownCallback );
  glutSpecialFunc( glutSpecialDownCallback );
  glutKeyboardUpFunc( glutKeyboardUpCallback );
  glutSpecialUpFunc( glutSpecialUpCallback );
#endif
#ifndef WIN32
  glutMouseFunc( glutMouseCallback );
  glutMotionFunc( glutMotionCallback );
  glutPassiveMotionFunc( glutMotionCallback );
#endif
#ifdef FREEGLUT
#ifndef WIN32
  glutMouseWheelFunc( glutMouseWheelCallback );
#endif
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, 
                 GLUT_ACTION_GLUTMAINLOOP_RETURNS );
#ifdef WIN32
  // Subclassing this window. Seems to work as it should.
  // It seems to work even if two GLUTWindow instances
  // are created but this may be system dependent.
  hWnd = FindWindow( "FREEGLUT", "H3D" );
  wpOrigProc = (WNDPROC) SetWindowLong(hWnd, 
                GWL_WNDPROC, (LONG) WindowProc);
#endif
#endif
}


/// Given the identifier of a GLUT window the GLWindow instance
/// that created that window is returned.
GLUTWindow *GLUTWindow::getGLUTWindow( int glut_id ) {
  for( set< H3DWindowNode* >::iterator i = H3DWindowNode::windows.begin(); 
       i != H3DWindowNode::windows.end(); i ++ ) {
    GLUTWindow *glut_window = dynamic_cast< GLUTWindow * >( *i );
    if( glut_window && glut_window->getGLUTWindowId() == glut_id )
      return glut_window;
  }
  return NULL;
}

void GLUTWindow::setFullscreen( bool fullscreen ) {
  if( fullscreen ) 
    glutFullScreen();
}

void GLUTWindow::swapBuffers() {
  glutSwapBuffers();
}

void GLUTWindow::makeWindowActive() {
  glutSetWindow( window_id );
}

#if !( defined(FREEGLUT) && defined(WIN32) )
void GLUTWindow::glutKeyboardDownCallback( unsigned char key, 
                                     int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    window->onKeyDown( key, false );
  }
}

void GLUTWindow::glutSpecialDownCallback( int key, 
                                    int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    switch( key ) {
      case GLUT_KEY_F1: window->onKeyDown( X3DKeyDeviceSensorNode::F1, true );
        break;
      case GLUT_KEY_F2: window->onKeyDown( X3DKeyDeviceSensorNode::F2, true );
        break;
      case GLUT_KEY_F3: window->onKeyDown( X3DKeyDeviceSensorNode::F3, true );
        break;
      case GLUT_KEY_F4: window->onKeyDown( X3DKeyDeviceSensorNode::F4, true );
        break;
      case GLUT_KEY_F5: window->onKeyDown( X3DKeyDeviceSensorNode::F5, true );
        break;
      case GLUT_KEY_F6: window->onKeyDown( X3DKeyDeviceSensorNode::F6, true );
        break;
      case GLUT_KEY_F7: window->onKeyDown( X3DKeyDeviceSensorNode::F7, true );
        break;
      case GLUT_KEY_F8: window->onKeyDown( X3DKeyDeviceSensorNode::F8, true );
        break;
      case GLUT_KEY_F9: window->onKeyDown( X3DKeyDeviceSensorNode::F9, true );
        break;
      case GLUT_KEY_F10: window->onKeyDown( X3DKeyDeviceSensorNode::F10,
                                            true );
        break;
      case GLUT_KEY_F11: window->onKeyDown( X3DKeyDeviceSensorNode::F11,
                                            true );
        break;
      case GLUT_KEY_F12: window->onKeyDown( X3DKeyDeviceSensorNode::F12,
                                            true );
        break;
      case GLUT_KEY_HOME: window->onKeyDown( X3DKeyDeviceSensorNode::HOME,
                                             true );
        break;
      case GLUT_KEY_END: window->onKeyDown( X3DKeyDeviceSensorNode::END,
                                            true );
        break;
      case GLUT_KEY_PAGE_UP: window->onKeyDown( X3DKeyDeviceSensorNode::PGUP,
                                                true );
        break;
      case GLUT_KEY_PAGE_DOWN: window->onKeyDown( X3DKeyDeviceSensorNode::PGDN,
                                                  true );
        break;
      case GLUT_KEY_UP: window->onKeyDown( X3DKeyDeviceSensorNode::UP, true );
        break;
      case GLUT_KEY_DOWN: window->onKeyDown( X3DKeyDeviceSensorNode::DOWN,
                                             true );
        break;
      case GLUT_KEY_LEFT: window->onKeyDown( X3DKeyDeviceSensorNode::LEFT,
                                             true );
        break;
      case GLUT_KEY_RIGHT: window->onKeyDown( X3DKeyDeviceSensorNode::RIGHT,
                                              true );
        break;
      default: {}
    }
  }
}

void GLUTWindow::glutKeyboardUpCallback( unsigned char key, 
                                   int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    window->onKeyDown( key, false );
  }
}

void GLUTWindow::glutSpecialUpCallback( int key, 
                                  int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    switch( key ) {
      case GLUT_KEY_F1: window->onKeyUp( X3DKeyDeviceSensorNode::F1, true );
        break;
      case GLUT_KEY_F2: window->onKeyUp( X3DKeyDeviceSensorNode::F2, true );
        break;
      case GLUT_KEY_F3: window->onKeyUp( X3DKeyDeviceSensorNode::F3, true );
        break;
      case GLUT_KEY_F4: window->onKeyUp( X3DKeyDeviceSensorNode::F4, true );
        break;
      case GLUT_KEY_F5: window->onKeyUp( X3DKeyDeviceSensorNode::F5, true );
        break;
      case GLUT_KEY_F6: window->onKeyUp( X3DKeyDeviceSensorNode::F6, true );
        break;
      case GLUT_KEY_F7: window->onKeyUp( X3DKeyDeviceSensorNode::F7, true );
        break;
      case GLUT_KEY_F8: window->onKeyUp( X3DKeyDeviceSensorNode::F8, true );
        break;
      case GLUT_KEY_F9: window->onKeyUp( X3DKeyDeviceSensorNode::F9, true );
        break;
      case GLUT_KEY_F10: window->onKeyUp( X3DKeyDeviceSensorNode::F10, true );
        break;
      case GLUT_KEY_F11: window->onKeyUp( X3DKeyDeviceSensorNode::F11, true );
        break;
      case GLUT_KEY_F12: window->onKeyUp( X3DKeyDeviceSensorNode::F12, true );
        break;
      case GLUT_KEY_HOME: window->onKeyUp( X3DKeyDeviceSensorNode::HOME,
                                           true );
        break;
      case GLUT_KEY_END: window->onKeyUp( X3DKeyDeviceSensorNode::END,
                                          true );
        break;
      case GLUT_KEY_PAGE_UP: window->onKeyUp( X3DKeyDeviceSensorNode::PGUP,
                                              true );
        break;
      case GLUT_KEY_PAGE_DOWN: window->onKeyUp( X3DKeyDeviceSensorNode::PGDN,
                                                true );
        break;
      case GLUT_KEY_UP: window->onKeyUp( X3DKeyDeviceSensorNode::UP, true );
        break;
      case GLUT_KEY_DOWN: window->onKeyUp( X3DKeyDeviceSensorNode::DOWN,
                                           true );
        break;
      case GLUT_KEY_LEFT: window->onKeyUp( X3DKeyDeviceSensorNode::LEFT,
                                           true );
        break;
      case GLUT_KEY_RIGHT: window->onKeyUp( X3DKeyDeviceSensorNode::RIGHT,
                                            true );
        break;
      default: {}
    }
  }
}
#endif

#ifndef WIN32
void GLUTWindow::glutMouseCallback( int button, int state, 
                                   int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    switch( button ) {
      case GLUT_LEFT_BUTTON:
        window->onMouseButtonAction( MouseSensor::LEFT_BUTTON, 
                                     state == GLUT_DOWN ? 
                                     MouseSensor::DOWN : MouseSensor::UP );
        break;
      case GLUT_MIDDLE_BUTTON:
        window->onMouseButtonAction( MouseSensor::MIDDLE_BUTTON,
                                     state == GLUT_DOWN ?
                                     MouseSensor::DOWN : MouseSensor::UP );
        break;
      case GLUT_RIGHT_BUTTON:
        window->onMouseButtonAction( MouseSensor::RIGHT_BUTTON,
                                     state == GLUT_DOWN ?
                                     MouseSensor::DOWN : MouseSensor::UP );
        break;
      default: {}
    }
  }
}
    
void GLUTWindow::glutMotionCallback( int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    window->onMouseMotionAction( x, y );
  }
}
#endif

#if defined(FREEGLUT) && !defined(WIN32)
void GLUTWindow::glutMouseWheelCallback( int wheel, 
                                        int direction, int x, int y ) {
  GLUTWindow *window = GLUTWindow::getGLUTWindow( glutGetWindow() );
  if( window ) {
    if( direction == 1 )
      window->onMouseWheelAction( MouseSensor::FROM );
    else
      window->onMouseWheelAction( MouseSensor::TOWARDS );
  }
}
#endif
