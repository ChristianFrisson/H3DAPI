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
/// \file RazerHydraSensor.h
/// \brief Header file for RazerHydraSensor.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RAZERHYDRASENSOR_H__
#define __RAZERHYDRASENSOR_H__


#include <H3D/X3DSensorNode.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFFloat.h>
#include <H3D/SFInt32.h>
#include <H3D/SFRotation.h>
#include <H3DUtil/Threads.h>


namespace H3D {
  /// \ingroup H3DNodes
  /// \class RazerHydraSensor
  /// \brief This is a X3DSensorNode for reading values from a Razer hydra sensor.
  ///
  /// All fields that start with c0 belong to one controller. All fields that start
  /// with c1 belongs to the other controller.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RazerHydraSensor.x3d">RazerHydraSensor.x3d</a>
  ///     ( <a href="examples/RazerHydraSensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile RazerHydraSensor.dot
  /// \todo Implement checking of sixenseGetMaxControllers instead and create controller fields
  /// dynamically. Or perhaps just output some max base or something.
  class H3DAPI_API RazerHydraSensor : public X3DSensorNode {
  public:
    
    class H3DAPI_API EnableHydraSensor : public OnValueChangeSField< SFBool > {
    protected:
      /// Enables/disables the RazerHydraSensor.
      virtual void onValueChange( const bool &new_value );
    };

    /// Constructor.
    RazerHydraSensor( 
                    Inst< EnableHydraSensor     > _enabled  = 0,
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

    /// Destructor
    ~RazerHydraSensor();

    /// Update the fields if 
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// c0Position is the position for controller c0.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Position.dot
    auto_ptr< SFVec3f    >          c0Position;

    /// c0Orientation is the orientation for controller c0.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Orientation.dot
    auto_ptr< SFRotation >          c0Orientation;

    /// c0Joystick is the position of the (thumb) joystick for controller
    /// c0. x is the horizontal position and y is the vertical position.
    /// -1 is full left/down, 0 is centered and 1 is full right/up.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Valid range:</b> [-1,1]
    /// 
    /// \dotfile RazerHydraSensor_c0Joystick.dot
    auto_ptr< SFVec2f    >          c0Joystick;

    /// c0Trigger is the status of the analog trigger for controller c0.
    /// 0 is unpressed, 1 is fully pressed.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Valid range:</b> [0,1]
    /// 
    /// \dotfile RazerHydraSensor_c0Trigger.dot
    auto_ptr< SFFloat    >          c0Trigger;

    /// c0Buttons is a bit mask describing the state of each button for
    /// controller c0. If any bit is 1 it means a button is pressed.
    /// Last time this documentation was updated the bits in Sixense API
    /// were defined to mean this:
    /// (0x01<<0) - SIXENSE_BUTTON_START - H3D field c0buttonStart
    /// (0x01<<3) - SIXENSE_BUTTON_3 - H3D field c0Button3
    /// (0x01<<4) - SIXENSE_BUTTON_4 - H3D field c0Button4
    /// (0x01<<5) - SIXENSE_BUTTON_1 - H3D field c0Button1
    /// (0x01<<6) - SIXENSE_BUTTON_2 - H3D field c0Button2
    /// (0x01<<7) - SIXENSE_BUTTON_BUMPER - H3D field c0ButtonBumper
    /// (0x01<<8) - SIXENSE_BUTTON_JOYSTICK - H3D field c0ButtonJoystick
    /// 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Buttons.dot
    auto_ptr< SFInt32    >          c0Buttons;

    /// c0Connected is true if controller c0 is connected.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Connected.dot
    auto_ptr< SFBool     >          c0Connected;

    /// c0Docked is true if controller c0 is docked. A docked
    /// controller can be expected to not be in use.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Docked.dot
    auto_ptr< SFBool     >          c0Docked;

    /// c0WhichHand is 0 until controller c0 is docked. If placed on the
    /// left side of the dock this field will be set to 1. If placed on the
    /// right side this field will be set to 2.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0WhichHand.dot
    auto_ptr< SFInt32    >          c0WhichHand;

    /// c0HemiTrackingEnabled is false until both controllers have been
    /// docked.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0HemiTrackingEnabled.dot
    auto_ptr< SFBool     >          c0HemiTrackingEnabled;

    /// c0Button1 is true if button 1 of controller c0 has been pressed.
    /// Button 1 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_1.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Button1.dot
    auto_ptr< SFBool     >          c0Button1;

    /// c0Button2 is true if button 2 of controller c0 has been pressed.
    /// Button 2 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_2.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Button2.dot
    auto_ptr< SFBool     >          c0Button2;

    /// c0Button3 is true if button 3 of controller c0 has been pressed.
    /// Button 3 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_3.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Button3.dot
    auto_ptr< SFBool     >          c0Button3;

    /// c0Button4 is true if button 4 of controller c0 has been pressed.
    /// Button 4 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_4.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0Button4.dot
    auto_ptr< SFBool     >          c0Button4;

    /// c0ButtonStart is true if start button of controller c0 has been
    /// pressed. Start button is the button which corresponds to Sixense SDKs
    /// value SIXENSE_BUTTON_START.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0ButtonStart.dot
    auto_ptr< SFBool     >          c0ButtonStart;

    /// c0ButtonBumper is true if bumper button of controller c0 has been
    /// pressed. Bumper button is the button which corresponds to Sixense SDKs
    /// value SIXENSE_BUTTON_BUMPER.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0ButtonBumper.dot
    auto_ptr< SFBool     >          c0ButtonBumper;

    /// c0ButtonJoystick is true if joystick button of controller c0 has been
    /// pressed. joystick button is the button which corresponds to Sixense
    /// SDKs value SIXENSE_BUTTON_JOYSTICK.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c0ButtonJoystick.dot
    auto_ptr< SFBool     >          c0ButtonJoystick;

    /// c1Position is the position for controller c1.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Position.dot
    auto_ptr< SFVec3f    >          c1Position;

    /// c1Orientation is the orientation for controller c1.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Orientation.dot
    auto_ptr< SFRotation >          c1Orientation;

    /// c1Joystick is the position of the (thumb) joystick for controller
    /// c1. x is the horizontal position and y is the vertical position.
    /// -1 is full left/down, 0 is centered and 1 is full right/up.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Valid range:</b> [-1,1]
    /// 
    /// \dotfile RazerHydraSensor_c1Joystick.dot
    auto_ptr< SFVec2f    >          c1Joystick;

    /// c1Trigger is the status of the analog trigger for controller c1.
    /// 0 is unpressed, 1 is fully pressed.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Valid range:</b> [0,1]
    /// 
    /// \dotfile RazerHydraSensor_c1Trigger.dot
    auto_ptr< SFFloat    >          c1Trigger;

    /// c1Buttons is a bit mask describing the state of each button for
    /// controller c1. If any bit is 1 it means a button is pressed.
    /// Last time this documentation was updated the bits in Sixense API
    /// were defined to mean this:
    /// (0x01<<0) - SIXENSE_BUTTON_START - H3D field c1buttonStart
    /// (0x01<<3) - SIXENSE_BUTTON_3 - H3D field c1Button3
    /// (0x01<<4) - SIXENSE_BUTTON_4 - H3D field c1Button4
    /// (0x01<<5) - SIXENSE_BUTTON_1 - H3D field c1Button1
    /// (0x01<<6) - SIXENSE_BUTTON_2 - H3D field c1Button2
    /// (0x01<<7) - SIXENSE_BUTTON_BUMPER - H3D field c1ButtonBumper
    /// (0x01<<8) - SIXENSE_BUTTON_JOYSTICK - H3D field c1ButtonJoystick
    /// 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Buttons.dot
    auto_ptr< SFInt32    >          c1Buttons;

    /// c1Connected is true if controller c1 is connected.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Connected.dot
    auto_ptr< SFBool     >          c1Connected;

    /// c1Docked is true if controller c1 is docked. A docked
    /// controller can be expected to not be in use.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Docked.dot
    auto_ptr< SFBool     >          c1Docked;

    /// c1WhichHand is 0 until controller c1 is docked. If placed on the
    /// left side of the dock this field will be set to 1. If placed on the
    /// right side this field will be set to 2.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1WhichHand.dot
    auto_ptr< SFInt32    >          c1WhichHand;

    /// c1HemiTrackingEnabled is false until both controllers have been
    /// docked.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1HemiTrackingEnabled.dot
    auto_ptr< SFBool     >          c1HemiTrackingEnabled;

    /// c1Button1 is true if button 1 of controller c1 has been pressed.
    /// Button 1 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_1.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Button1.dot
    auto_ptr< SFBool     >          c1Button1;

    /// c1Button2 is true if button 2 of controller c1 has been pressed.
    /// Button 2 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_2.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Button2.dot
    auto_ptr< SFBool     >          c1Button2;

    /// c1Button3 is true if button 3 of controller c1 has been pressed.
    /// Button 3 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_3.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Button3.dot
    auto_ptr< SFBool     >          c1Button3;

    /// c1Button4 is true if button 4 of controller c1 has been pressed.
    /// Button 4 is the button which corresponds to Sixense SDKs value
    /// SIXENSE_BUTTON_4.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1Button4.dot
    auto_ptr< SFBool     >          c1Button4;

    /// c1ButtonStart is true if start button of controller c1 has been
    /// pressed. Start button is the button which corresponds to Sixense SDKs
    /// value SIXENSE_BUTTON_START.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1ButtonStart.dot
    auto_ptr< SFBool     >          c1ButtonStart;

    /// c1ButtonBumper is true if bumper button of controller c1 has been
    /// pressed. Bumper button is the button which corresponds to Sixense SDKs
    /// value SIXENSE_BUTTON_BUMPER.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1ButtonBumper.dot
    auto_ptr< SFBool     >          c1ButtonBumper;

    /// c1ButtonJoystick is true if joystick button of controller c1 has been
    /// pressed. joystick button is the button which corresponds to Sixense
    /// SDKs value SIXENSE_BUTTON_JOYSTICK.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile RazerHydraSensor_c1ButtonJoystick.dot
    auto_ptr< SFBool     >          c1ButtonJoystick;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
#ifdef HAVE_SIXENSE
    struct RazerHydraData {
      Vec3f position;
      Rotation orientation;
      Vec2f joystick;
      H3DFloat trigger;
      H3DInt32 buttons;
      bool connected, docked, hemi_tracking_enabled;
      H3DInt32 which_hand, controller;
    };
    list< RazerHydraData > hydra_data;
    MutexLock data_lock;

    static PeriodicThread::CallbackCode addHydraInstanceCB( void *data );
    static PeriodicThread::CallbackCode removeHydraInstanceCB( void *data );
    // the handle of the communication thread.
    static auto_ptr< H3DUtil::PeriodicThread > thread_handle;
    static list< RazerHydraSensor * > razer_hydra_instances;
    static vector< unsigned char > last_sequence_number;
    // The function used as the thread for reading data from
    // the Razer Hydra device.
    static PeriodicThread::CallbackCode razerHydraCallback( void* data );
    /// Transfers the values from the device communication thread to
    /// the scenegraph thread.
    void transferValues( const vector< RazerHydraData > &new_data );
#endif
  };
}

#endif
