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
/// \file GLUTWindow.cpp
/// \brief CPP file for GLUTWindow.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GLUTWindow.h>
#ifdef HAVE_GLUT

#include <GL/glew.h>
#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifdef FREEGLUT
#include <GL/freeglut.h>
#endif

#include <H3D/X3DKeyDeviceSensorNode.h>
#include <H3D/MouseSensor.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GLUTWindow::database( "GLUTWindow", 
                                      &(newInstance<GLUTWindow>), 
                                      typeid( GLUTWindow ),
                                      &(H3DWindowNode::database) );

namespace GLUTWindowInternals {
  FIELDDB_ELEMENT( GLUTWindow, gameMode, INITIALIZE_ONLY );
}

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
    char *argv[1] = { (char *)("H3DLoad") };
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
                        Inst< SFInt32     > _posX,
                        Inst< SFInt32     > _posY,
                        Inst< SFBool      > _manualCursorControl,
                        Inst< SFString    > _cursorType,
                        Inst< SFString    > _gameMode ) :
  H3DWindowNode( _width, _height, _fullscreen, _mirrored, _renderMode,
                 _viewpoint, _posX, _posY, _manualCursorControl, _cursorType ),
  gameMode( _gameMode ),
  last_x_pos( -1 ),
  last_y_pos( -1 ),
  last_width( -1 ),
  last_height( -1 ) {
  
  type_name = "GLUTWindow";
  database.initFields( this );

  cursorType->addValidValue( "RIGHT_ARROW" ); 
  cursorType->addValidValue( "LEFT_ARROW" ); 
  cursorType->addValidValue( "INFO" ); 
  cursorType->addValidValue( "DESTROY" ); 
  cursorType->addValidValue( "HELP" ); 
  cursorType->addValidValue( "CYCLE" ); 
  cursorType->addValidValue( "SPRAY" ); 
  cursorType->addValidValue( "WAIT" ); 
  cursorType->addValidValue( "TEXT" ); 
  cursorType->addValidValue( "CROSSHAIR" ); 
  cursorType->addValidValue( "UP_DOWN" ); 
  cursorType->addValidValue( "LEFT_RIGHT" ); 
  cursorType->addValidValue( "TOP_SIDE" ); 
  cursorType->addValidValue( "BOTTOM_SIDE" ); 
  cursorType->addValidValue( "LEFT_SIDE" ); 
  cursorType->addValidValue( "RIGHT_SIDE" ); 
  cursorType->addValidValue( "TOP_LEFT_CORNER" ); 
  cursorType->addValidValue( "TOP_RIGHT_CORNER" ); 
  cursorType->addValidValue( "BOTTOM_RIGHT_CORNER" ); 
  cursorType->addValidValue( "BOTTOM_LEFT_CORNER" ); 
  cursorType->addValidValue( "FULL_CROSSHAIR" ); 
  cursorType->addValidValue( "NONE" );
  last_fullscreen = fullscreen->getValue();
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
  if( isInitialized() ) {
    Console(4) << "GLUTWindow does not support changing pixel format from/to "
               << "quad buffered stereo support after initialization." << endl;
    if( last_render_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
      renderMode->setValue( "QUAD_BUFFERED_STEREO" );
    } else {
      renderMode->setValue( "MONO" );
    }
    return;
  }
  unsigned int mode = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL;

  if( useFullscreenAntiAliasing->getValue() ) {
    mode = mode | GLUT_MULTISAMPLE;
  }

  RenderMode::Mode stereo_mode = renderMode->getRenderMode();
      
  if( stereo_mode == RenderMode::VERTICAL_INTERLACED ||
      stereo_mode == RenderMode::HORIZONTAL_INTERLACED ||
      stereo_mode == RenderMode::VERTICAL_INTERLACED_GREEN_SHIFT ) {
    //mode |= GLUT_STENCIL;
  } else if( stereo_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
    mode |= GLUT_STEREO;
  }
  
  glutInitDisplayMode( mode );
  if( gameMode->getValue() == "" ) {
    glutInitWindowSize( width->getValue(), height->getValue() );
    glutInitWindowPosition( posX->getValue(), posY->getValue() );
    window_id = glutCreateWindow( "H3D" );
    setFullscreen( fullscreen->getValue() );
    // This code is here to check if we got a stereo window using stereo mode.
    // It seems like on some systems GLUT_MULTISAMPLE can not be combined with
    // GLUT_STEREO.
    if( stereo_mode == RenderMode::QUAD_BUFFERED_STEREO ) {
      GLboolean quad_stereo_supported;
      glGetBooleanv( GL_STEREO, &quad_stereo_supported);
      if( !quad_stereo_supported ) {
        // No stereo, destroy the window, and then use a mode without
        // GLUT_MULTISAMPLE to try to create a stereo window.
        mode = mode ^ GLUT_MULTISAMPLE;
        glutDestroyWindow(window_id);
        glutInitDisplayMode( mode );
        window_id = glutCreateWindow( "H3D" );
        glGetBooleanv( GL_STEREO, &quad_stereo_supported);
      }
    }
    glutSetWindow( window_id );
  } else {
    glutGameModeString(gameMode->getValue().c_str());
    if( ! glutGameModeGet(GLUT_GAME_MODE_POSSIBLE) ) {
      Console(4) << "The selected GLUT gamemode ("
                 << gameMode->getValue()
                 << ") is not available\n"
                 << std::endl;
    }
    glutEnterGameMode();
    window_id = glutGetWindow();
  }
  // set up GLUT callback functions
  glutDisplayFunc ( GLUTWindowInternals::displayFunc  );
  glutReshapeFunc ( GLUTWindowInternals::reshapeFunc  );
 
#if defined(H3D_WINDOWS) && defined( FREEGLUT )
  // When in gameMode events will not pass through the normal Windows callback
  // system so we have to use the glut way instead of the default Windows key and
  // mouse handling from H3DWindowNode
  if( gameMode->getValue() != "" ) {
#endif  // H3D_WINDOWS

    glutKeyboardFunc( glutKeyboardDownCallback );
    glutSpecialFunc( glutSpecialDownCallback );
    glutKeyboardUpFunc( glutKeyboardUpCallback );
    glutSpecialUpFunc( glutSpecialUpCallback );
    glutMouseFunc( glutMouseCallback );
    glutMotionFunc( glutMotionCallback );
    glutPassiveMotionFunc( glutMotionCallback );
#ifdef FREEGLUT
    glutMouseWheelFunc( glutMouseWheelCallback );
#endif // FREEGLUT

#if defined(H3D_WINDOWS) && defined( FREEGLUT )
   }
#endif // H3D_WINDOWS

#ifdef FREEGLUT
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, 
                 GLUT_ACTION_GLUTMAINLOOP_RETURNS );
#ifdef H3D_WINDOWS
  // Subclassing this window. Seems to work as it should.
  // It seems to work even if two GLUTWindow instances
  // are created but this may be system dependent.
  hWnd = FindWindow( "FREEGLUT", "H3D" );
#ifdef H3D_WIN64
  wpOrigProc = (WNDPROC) SetWindowLongPtr(hWnd, 
                GWLP_WNDPROC, (LONG_PTR) WindowProc);
#else
 wpOrigProc = (WNDPROC) SetWindowLongPtr(hWnd, 
                GWL_WNDPROC, (LONG_PTR) WindowProc);
#endif // H3D_WIN64
#endif // H3D_WINDOWS
#endif // FREEGLUT
  window_is_made_active = true;
}


// Given the identifier of a GLUT window the GLWindow instance
// that created that window is returned.
GLUTWindow *GLUTWindow::getGLUTWindow( int glut_id ) {
  for( set< H3DWindowNode* >::iterator i = H3DWindowNode::windows.begin(); 
       i != H3DWindowNode::windows.end(); ++i ) {
    GLUTWindow *glut_window = dynamic_cast< GLUTWindow * >( *i );
    if( glut_window && glut_window->getGLUTWindowId() == glut_id )
      return glut_window;
  }
  return NULL;
}

void GLUTWindow::setFullscreen( bool fullscreen ) {
  if( gameMode->getValue() == "" ) {
    if( fullscreen != last_fullscreen ) {
// Note: This define check is needed since freeglut does not have a
// working version compile time definition. freegluts svn repository
// was checked and GLUT_FULL_SCREEN was added at the same time as
// glutFullScreenToggle.
#if defined( FREEGLUT ) && defined( GLUT_FULL_SCREEN )
      glutFullScreenToggle();
#else
      if( fullscreen ) {
        last_x_pos = glutGet( GLUT_WINDOW_X );
        last_y_pos = glutGet( GLUT_WINDOW_Y );
        last_width = width->getValue();
        last_height = height->getValue();
        glutFullScreen();
      } else {
        glutReshapeWindow(last_width, last_height);
        glutPositionWindow(last_x_pos,last_y_pos);
      }
#endif
      last_fullscreen = fullscreen;
    }
  }
}

void GLUTWindow::swapBuffers() {
  glutSwapBuffers();
}

void GLUTWindow::makeWindowActive() {
  glutSetWindow( window_id );
}


int GLUTWindow::setCursorType(const std::string & cursor_type) {
  static map<std::string, int> cursor_shapes;

  if( cursor_shapes.size() == 0 ) {
    // see man glutSetCursor(3)
    cursor_shapes[ "DEFAULT"    ]  = GLUT_CURSOR_INHERIT;
    cursor_shapes[ "RIGHT_ARROW" ]  = GLUT_CURSOR_RIGHT_ARROW;
    cursor_shapes[ "LEFT_ARROW"    ]  = GLUT_CURSOR_LEFT_ARROW;
    cursor_shapes[ "INFO"    ]  = GLUT_CURSOR_INFO;
    cursor_shapes[ "DESTROY" ]  = GLUT_CURSOR_DESTROY;           
    cursor_shapes[ "HELP"     ]  = GLUT_CURSOR_HELP;
    cursor_shapes[ "CYCLE"  ]  = GLUT_CURSOR_CYCLE;
    cursor_shapes[ "SPRAYCAN" ]  = GLUT_CURSOR_WAIT;
    cursor_shapes[ "WAIT"  ]  = GLUT_CURSOR_WAIT;
    cursor_shapes[ "TEXT"  ]  = GLUT_CURSOR_TEXT;
    cursor_shapes[ "CROSSHAIR"  ]  = GLUT_CURSOR_CROSSHAIR;
    cursor_shapes[ "UP_DOWN"  ]  = GLUT_CURSOR_UP_DOWN;
    cursor_shapes[ "LEFT_RIGHT"  ]  = GLUT_CURSOR_LEFT_RIGHT;
    cursor_shapes[ "TOP_SIDE"  ]  = GLUT_CURSOR_TOP_SIDE;
    cursor_shapes[ "BOTTOM_SIDE"  ]  = GLUT_CURSOR_BOTTOM_SIDE;
    cursor_shapes[ "LEFT_SIDE"  ]  = GLUT_CURSOR_LEFT_SIDE;
    cursor_shapes[ "RIGHT_SIDE"  ]  = GLUT_CURSOR_RIGHT_SIDE;
    cursor_shapes[ "TOP_LEFT_CORNER"  ]  = GLUT_CURSOR_TOP_LEFT_CORNER;
    cursor_shapes[ "TOP_RIGHT_CORNER" ]  = GLUT_CURSOR_TOP_RIGHT_CORNER;
    cursor_shapes[ "BOTTOM_RIGHT_CORNER" ]  = GLUT_CURSOR_BOTTOM_RIGHT_CORNER;
    cursor_shapes[ "BOTTOM_LEFT_CORNER"  ]  = GLUT_CURSOR_BOTTOM_LEFT_CORNER;
    cursor_shapes[ "FULL_CROSSHAIR"  ]  = GLUT_CURSOR_FULL_CROSSHAIR;
    cursor_shapes[ "NONE"  ]  = GLUT_CURSOR_NONE;
  }

  if ( GLUT_init ) {
    int current_window_id = glutGetWindow();
    if (current_window_id != window_id)
      glutSetWindow( window_id );

  
    std::map<std::string, int>::iterator itr = cursor_shapes.find( cursor_type );
    if( itr != cursor_shapes.end() ) {
      glutSetCursor( itr->second );
    } 
    
    if (current_window_id != 0 && current_window_id != window_id)
      glutSetWindow( current_window_id );

    if(  itr != cursor_shapes.end() ) return 0;
    return -1;
  }

  return -1;
}


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
    window->onKeyUp( key, false );
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

string GLUTWindow::getCursorForMode( const string &mode ) {
  if( mode == "ON_SENSOR_OVER" ) {
    return "CROSSHAIR";
  } else if( mode == "ON_SENSOR_ACTIVE" ) {
    return "CYCLE";
  } else if( mode == "ON_NAV_LOOKAT" ) {
    return "CROSSHAIR";
  } 

  return "DEFAULT";
}
#endif // HAVE_GLUT

