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
/// \file MouseSensor.h
/// \brief Header file for MouseSensor
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MOUSESENSOR_H__
#define __MOUSESENSOR_H__


#include "X3DSensorNode.h"
#include <list>
#include <GL/glew.h>
#include <GL/glut.h>

namespace H3D {

  /// \ingroup Nodes
  /// \class MouseSensor
  /// \brief A sensor node for reading mouse values. 
  ///
  /// The scrollUp and scrollDown fields will only be usable if you
  /// are compiling against freeglut instead of GLUT.
  /// 
  ///
  class H3DAPI_API MouseSensor : public X3DSensorNode {
  public:

    /// Constructor.
    MouseSensor( Inst< SFBool  > _enabled      = 0,
                 Inst< SFNode  > _metadata     = 0,
                 Inst< SFBool  > _isActive     = 0,
                 Inst< SFVec2f > _position     = 0,
                 Inst< SFBool  > _leftButton   = 0,
                 Inst< SFBool  > _middleButton = 0,
                 Inst< SFBool  > _rightButton  = 0,
                 Inst< SFVec2f > _motion       = 0,
                 Inst< SFBool  > _scrollUp     = 0,
                 Inst< SFBool  > _scrollDown   = 0
                 );

    /// Destructor.
    virtual ~MouseSensor();

    /// Will be called when the mouse has moved. The x and y parameters 
    /// indicate the mouse location in window relative coordinates.
    void addGLUTMouseMotionAction( int x, int y );

    /// Will be called  when a mouse button event occurs.
    /// \param button is one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or
    /// GLUT_RIGHT_BUTTON. 
    /// \param state is either GLUT_UP or GLUT_DOWN indicating whether 
    /// the callback was due to a release or press respectively.
    virtual void addGLUTMouseButtonAction( int button, int state );

    /// Will be called  when a mouse scroll wheel event occurs.
    virtual void addGLUTMouseWheelAction( int wheel, int direction );

    /// glut callback function. Calls addGLUTMouseButtonAction() on all
    /// MouseSensor instances.
    static void glutMouseCallback( int button, int state, 
                                   int x, int y );
    
    /// glut callback function. Calls addGLUTMouseMotionAction() on all
    /// MouseSensor instances.
    static void glutMotionCallback( int x, int y );

    /// glut callback function for mouse scroll wheel. 
    static void glutMouseWheelCallback( int wheel, 
                                        int direction, int x, int y );
    
    /// The position of the mouse in window relative coordinates.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFVec2f>  position;

    /// The status of the left button. TRUE indicates that it is pressed.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFBool>   leftButton;

    /// The status of the middle button. TRUE indicates that it is pressed.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFBool>   middleButton;

    /// The status of the right button. TRUE indicates that it is pressed.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFBool>   rightButton;

    /// The relative motion since in the last change of the position field.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFVec2f>  motion;

    /// The scrollUp field generates a TRUE event when the mouse scroll wheel
    /// is moved up.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFBool >  scrollUp;

    /// The scrollDown field generates a TRUE event when the mouse scroll wheel
    /// is moved down.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFBool >  scrollDown;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
    static list< MouseSensor * > instances;  
  };
}

#endif
