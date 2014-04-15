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
/// \file MouseSensor.h
/// \brief Header file for MouseSensor
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MOUSESENSOR_H__
#define __MOUSESENSOR_H__


#include <H3D/X3DSensorNode.h>
#include <list>
#include <H3D/SFVec2f.h>


namespace H3D {

  /// \ingroup H3DNodes
  /// \class MouseSensor
  /// \brief A sensor node for reading mouse values.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MouseSensor.x3d">MouseSensor.x3d</a>
  ///     ( <a href="examples/MouseSensor.x3d.html">Source</a> )
  class H3DAPI_API MouseSensor : public X3DSensorNode {
  public:

    typedef enum {
      LEFT_BUTTON = 1,   // The left mouse button;
      MIDDLE_BUTTON = 2, // The middle mouse button;
      RIGHT_BUTTON = 3,  // The right mouse button;
      DOWN = 4,          // The button is pressed
      UP = 5,            // The button is released
      FROM = 6,          // The wheel is scrolled from the user
      TOWARDS = 7        // The wheel is scrolled towards the user
    } MouseKeys;

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
    virtual void mouseMotionAction( int x, int y );

    /// Will be called  when a mouse button event occurs.
    /// \param button is one of LEFT_BUTTON, MIDDLE_BUTTON, or
    /// RIGHT_BUTTON. 
    /// \param state is either UP or DOWN indicating whether 
    /// the callback was due to a release or press respectively.
    virtual void mouseButtonAction( int button, int state );

    /// Will be called when a mouse scroll wheel event occurs.
    /// direction is one of FROM or TOWARDS
    virtual void mouseWheelAction( int direction );

    /// Calls mouseButtonAction() on all
    /// MouseSensor instances.
    static void buttonCallback( int button, int state );
    
    /// Calls mouseMotionAction() on all
    /// MouseSensor instances.
    static void motionCallback( int x, int y );

    /// Calls mouseWheelAction() on all MouseSensor instances.
    static void wheelCallback( int direction );
    
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
    static bool invalid_instance_ptr;
  };
}

#endif
