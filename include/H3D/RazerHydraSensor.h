//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file RazerHydraSensor.h
/// \brief Header file for RazerHydraSensor.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RAZERHYDRASENSOR_H__
#define __RAZERHYDRASENSOR_H__


#include <H3D/X3DSensorNode.h>
#include <list>
#include <H3D/SFVec3f.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFFloat.h>
#include <H3D/SFInt32.h>
#include <H3D/SFRotation.h>
#include <H3DUtil/Threads.h>


//#include "sixense.h"
namespace H3D {

  /// \ingroup H3DNodes
  /// \class RazerHydraSensor
  /// \brief This is a X3DSensorNode for reading values from a 3DConnexion motion
  /// controller such as the SpaceTraveller or SpaceMouse 5000.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RazerHydraSensor.x3d">RazerHydraSensor.x3d</a>
  ///     ( <a href="examples/RazerHydraSensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile RazerHydraSensor.dot
  class H3DAPI_API RazerHydraSensor : public X3DSensorNode {
  public:

    /// Constructor.
    RazerHydraSensor( 
                    Inst< SFBool                > _enabled  = 0,
                    Inst< SFNode                > _metadata = 0,
                    Inst< SFBool                > _isActive = 0,
  
                    Inst< SFVec3f               > _c0Position = 0,
                    Inst< SFRotation            > _c0Orientation = 0,
                    Inst< SFVec2f               > _c0Joystick = 0,
                    Inst< SFFloat               > _c0Trigger = 0,
                    Inst< SFInt32               > _c0Buttons = 0,
                    Inst< SFBool                > _c0Connected = 0,
                    Inst< SFBool                > _c0Docked = 0,
                    Inst< SFInt32               > _c0WhichHand = 0,
                    Inst< SFBool                > _c0HemiTrackingEnabled = 0,
                    Inst< SFBool                > _c0Button1 = 0,
                    Inst< SFBool                > _c0Button2 = 0,
                    Inst< SFBool                > _c0Button3 = 0,
                    Inst< SFBool                > _c0Button4 = 0,
                    Inst< SFBool                > _c0ButtonStart = 0,
                    Inst< SFBool                > _c0ButtonBumper = 0,
                    Inst< SFBool                > _c0ButtonJoystick = 0,

                    Inst< SFVec3f               > _c1Position = 0,
                    Inst< SFRotation            > _c1Orientation = 0,
                    Inst< SFVec2f               > _c1Joystick = 0,
                    Inst< SFFloat               > _c1Trigger = 0,
                    Inst< SFInt32               > _c1Buttons = 0,
                    Inst< SFBool                > _c1Connected = 0,
                    Inst< SFBool                > _c1Docked = 0,
                    Inst< SFInt32               > _c1WhichHand = 0,
                    Inst< SFBool                > _c1HemiTrackingEnabled = 0,
                    Inst< SFBool                > _c1Button1 = 0,
                    Inst< SFBool                > _c1Button2 = 0,
                    Inst< SFBool                > _c1Button3 = 0,
                    Inst< SFBool                > _c1Button4 = 0,
                    Inst< SFBool                > _c1ButtonStart = 0,
                    Inst< SFBool                > _c1ButtonBumper = 0,
                    Inst< SFBool                > _c1ButtonJoystick = 0
                    );

    ~RazerHydraSensor() {
    }

    auto_ptr< SFVec3f    >          c0Position;
    auto_ptr< SFRotation >          c0Orientation;
    auto_ptr< SFVec2f    >          c0Joystick;
    auto_ptr< SFFloat    >          c0Trigger;
    auto_ptr< SFInt32    >          c0Buttons;
    auto_ptr< SFBool     >          c0Connected;
    auto_ptr< SFBool     >          c0Docked;
    auto_ptr< SFInt32    >          c0WhichHand;
    auto_ptr< SFBool     >          c0HemiTrackingEnabled;
    auto_ptr< SFBool     >          c0Button1;
    auto_ptr< SFBool     >          c0Button2;
    auto_ptr< SFBool     >          c0Button3;
    auto_ptr< SFBool     >          c0Button4;
    auto_ptr< SFBool     >          c0ButtonStart;
    auto_ptr< SFBool     >          c0ButtonBumper;
    auto_ptr< SFBool     >          c0ButtonJoystick;

    auto_ptr< SFVec3f    >          c1Position;
    auto_ptr< SFRotation >          c1Orientation;
    auto_ptr< SFVec2f    >          c1Joystick;
    auto_ptr< SFFloat    >          c1Trigger;
    auto_ptr< SFInt32    >          c1Buttons;
    auto_ptr< SFBool     >          c1Connected;
    auto_ptr< SFBool     >          c1Docked;
    auto_ptr< SFInt32    >          c1WhichHand;
    auto_ptr< SFBool     >          c1HemiTrackingEnabled;
    auto_ptr< SFBool     >          c1Button1;
    auto_ptr< SFBool     >          c1Button2;
    auto_ptr< SFBool     >          c1Button3;
    auto_ptr< SFBool     >          c1Button4;
    auto_ptr< SFBool     >          c1ButtonStart;
    auto_ptr< SFBool     >          c1ButtonBumper;
    auto_ptr< SFBool     >          c1ButtonJoystick;


    // This data structure is used to transfer button data from the 
    // device communication thread to the scene graph thread.
    struct ButtonData {
      ButtonData( int _pressed, 
                  int _released,
                  int _current_state ) :
        pressed( _pressed ),
        released( _released ),
        current_state( _current_state ) {}
      unsigned int pressed, released, current_state;
    };
  public:
//      sixenseAllControllerData controller_data; 
          
          // has a motion event occured since last scenegraph loop.
    bool thread_motion_event;
    // button data since last scenegraph loop.
    vector< ButtonData > thread_buttons; 
    // rotation values from the last motion event.
    Vec3f thread_rotation;
    // translation values from the last motion event.
    Vec3f thread_translation;
    // is_active in thread. Will be false if thread main loop
    // can not run due to some error.
    bool thread_is_active;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
#ifdef HAVE_SIXENSE
    // the handle of the communication thread.
    auto_ptr< H3DUtil::PeriodicThread > thread_handle;
#endif

    /// Transfers the values from the device communication thread to
    /// the scenegraph thread.
    void updateValues();
    
    struct H3DAPI_API Update
      : AutoUpdate<Field> {
      void update(){
        static_cast<RazerHydraSensor*>
          (owner)->updateValues();
      }
    };
    
    auto_ptr< Update > update;
  };
}

#endif
