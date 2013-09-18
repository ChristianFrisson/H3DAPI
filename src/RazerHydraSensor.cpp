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
/// \file RazerHydraSensor.cpp
/// \brief CPP file for RazerHydraSensor.
///
//
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/RazerHydraSensor.h>

#include <H3D/Scene.h>

#ifdef HAVE_SIXENSE
#include <sixense.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase RazerHydraSensor::database( 
                                          "RazerHydraSensor", 
                                          &(newInstance<RazerHydraSensor>),
                                          typeid( RazerHydraSensor ),
                                          &X3DSensorNode::database
                                          );

namespace RazerHydraSensorInternal {
  FIELDDB_ELEMENT( RazerHydraSensor, c0Position, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Orientation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Joystick, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Trigger, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Buttons, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Connected, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Docked, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0WhichHand, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0HemiTrackingEnabled, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Button1, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Button2, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Button3, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0Button4, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0ButtonStart, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0ButtonBumper, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c0ButtonJoystick, OUTPUT_ONLY );

  FIELDDB_ELEMENT( RazerHydraSensor, c1Position, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Orientation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Joystick, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Trigger, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Buttons, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Connected, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Docked, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1WhichHand, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1HemiTrackingEnabled, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Button1, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Button2, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Button3, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1Button4, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1ButtonStart, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1ButtonBumper, OUTPUT_ONLY );
  FIELDDB_ELEMENT( RazerHydraSensor, c1ButtonJoystick, OUTPUT_ONLY );

  H3D_API_EXCEPTION( CouldNotInitRazerHydra );
  H3D_API_EXCEPTION( CreateWindowError );
  H3D_API_EXCEPTION( WindowGetMessageError );


  /// The function used as the thread for reading data from
  /// the Razer Hydra device.
  PeriodicThread::CallbackCode razerHydraCallback( void* data ) {
    
      //pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
      // the creator of this thread
      RazerHydraSensor *hydra_sensor = 
        static_cast< RazerHydraSensor * >( data );

//      sixenseAllControllerData controller_data[3];

  //    sixenseGetAllData( 3, controller_data ); 


/*
          Vec3f c0_position( hydra_sensor->controller_data.controllers[0].pos[0] / 1000,
                       hydra_sensor->controller_data.controllers[0].pos[1] / 1000,
                       hydra_sensor->controller_data.controllers[0].pos[2] / 1000 );

    Vec3f c1_position( hydra_sensor->controller_data.controllers[1].pos[0] / 1000,
                       hydra_sensor->controller_data.controllers[1].pos[1] / 1000,
                       hydra_sensor->controller_data.controllers[1].pos[2] / 1000 );

    hydra_sensor->c0Position->setValue( c0_position, hydra_sensor->id );
    */


   // hydra_sensor->controller_data->controllers[0]
    //      Console(4) << "0(pos): " << controller_data[0].controllers[0].pos[0] << " " << controller_data[0].controllers[0].pos[1] << " " << controller_data[0].controllers[0].pos[2] << endl;
      //    Console(4) << "1(pos): " << controller_data[0].controllers[1].pos[0] << " " << controller_data[0].controllers[1].pos[1] << " " << controller_data[0].controllers[1].pos[2] << endl;
        //  Console(4) << "2(pos): " << controller_data[0].controllers[2].pos[0] << " " << controller_data[0].controllers[2].pos[1] << " " << controller_data[0].controllers[2].pos[2] <<endl;
         // Console(4) << "3(pos): " << controller_data[0].controllers[3].pos[0] << " " << controller_data[0].controllers[3].pos[1] << " " << controller_data[0].controllers[3].pos[2] <<endl;


      return PeriodicThread::CALLBACK_CONTINUE; 
  }
}

RazerHydraSensor::RazerHydraSensor( 
              Inst< SFBool                > _enabled,
              Inst< SFNode                > _metadata,
              Inst< SFBool                > _isActive,

              Inst< SFVec3f               > _c0Position,
              Inst< SFRotation            > _c0Orientation,
              Inst< SFVec2f               > _c0Joystick,
              Inst< SFFloat               > _c0Trigger,
              Inst< SFInt32               > _c0Buttons,
              Inst< SFBool                > _c0Connected,
              Inst< SFBool                > _c0Docked,
              Inst< SFInt32               > _c0WhichHand,
              Inst< SFBool                > _c0HemiTrackingEnabled,
              Inst< SFBool                > _c0Button1,
              Inst< SFBool                > _c0Button2,
              Inst< SFBool                > _c0Button3,
              Inst< SFBool                > _c0Button4,
              Inst< SFBool                > _c0ButtonStart,
              Inst< SFBool                > _c0ButtonBumper,
              Inst< SFBool                > _c0ButtonJoystick,

              Inst< SFVec3f               > _c1Position,
              Inst< SFRotation            > _c1Orientation,
              Inst< SFVec2f               > _c1Joystick,
              Inst< SFFloat               > _c1Trigger,
              Inst< SFInt32               > _c1Buttons,
              Inst< SFBool                > _c1Connected,
              Inst< SFBool                > _c1Docked,
              Inst< SFInt32               > _c1WhichHand,
              Inst< SFBool                > _c1HemiTrackingEnabled,
              Inst< SFBool                > _c1Button1,
              Inst< SFBool                > _c1Button2,
              Inst< SFBool                > _c1Button3,
              Inst< SFBool                > _c1Button4,
              Inst< SFBool                > _c1ButtonStart,
              Inst< SFBool                > _c1ButtonBumper,
              Inst< SFBool                > _c1ButtonJoystick ):
X3DSensorNode( _enabled, _metadata, _isActive ),
    c0Position( _c0Position ),
    c0Orientation( _c0Orientation ),
    c0Joystick( _c0Joystick ),
    c0Trigger( _c0Trigger ),
    c0Buttons( _c0Buttons ),
    c0Connected( _c0Connected ),
    c0Docked( _c0Docked ),
    c0WhichHand( _c0WhichHand ),
    c0HemiTrackingEnabled( _c0HemiTrackingEnabled ),
    c0Button1( _c0Button1 ),
    c0Button2( _c0Button2 ),
    c0Button3( _c0Button3 ),
    c0Button4( _c0Button4 ),
    c0ButtonStart( _c0ButtonStart ),
    c0ButtonBumper( _c0ButtonBumper ),
    c0ButtonJoystick( _c0ButtonJoystick ),

    c1Position( _c1Position ),
    c1Orientation( _c1Orientation ),
    c1Joystick( _c1Joystick ),
    c1Trigger( _c1Trigger ),
    c1Buttons( _c1Buttons ),
    c1Connected( _c1Connected ),
    c1Docked( _c1Docked ),
    c1WhichHand( _c1WhichHand ),
    c1HemiTrackingEnabled( _c1HemiTrackingEnabled ),
    c1Button1( _c1Button1 ),
    c1Button2( _c1Button2 ),
    c1Button3( _c1Button3 ),
    c1Button4( _c1Button4 ),
    c1ButtonStart( _c1ButtonStart ),
    c1ButtonBumper( _c1ButtonBumper ),
    c1ButtonJoystick( _c1ButtonJoystick ),
    update( new Update ),
    thread_motion_event( false ),
    thread_is_active( false ) {
  
  type_name = "RazerHydraSensor";

  database.initFields( this );
  
  update->setOwner(this);
  Scene::time->routeNoEvent(update);
  
  c0Trigger->setValue( 0, id );
  c0Buttons->setValue( 0, id );
  c0Connected->setValue( false, id );
  c0Docked->setValue( false, id );
  c0WhichHand->setValue( 0, id );
  c0HemiTrackingEnabled->setValue( false, id );

  c1Trigger->setValue( 0, id );
  c1Buttons->setValue( 0, id );
  c1Connected->setValue( false, id );
  c1Docked->setValue( false, id );
  c1WhichHand->setValue( 0, id );
  c1HemiTrackingEnabled->setValue( false, id );
  
#ifdef HAVE_SIXENSE
  thread_handle.reset( 
          new H3DUtil::PeriodicThread( H3DUtil::PeriodicThread::NORMAL_PRIORITY, 
                                                                   60 ) );
  //thread_handle->asynchronousCallback( RazerHydraSensorInternal::razerHydraCallback, this );
  thread_handle->setThreadName( "RazerHydraSensor com thread" );
  sixenseInit();
#else
  Console(4) << "Warning: H3D API compiled without Sixense SDK. RazerHydraSensor node "
             << "will be unusable." << endl;
#endif
}

void RazerHydraSensor::updateValues(){
  using namespace RazerHydraSensorInternal;
#ifdef HAVE_SIXENSE
  if( isActive->getValue() != thread_is_active )
    isActive->setValue( thread_is_active, id );

    sixenseAllControllerData controller_data[3];

    //sixenseGetAllData( 3, controller_data ); 

    sixenseGetAllNewestData( controller_data );

    Vec3f c0_position( controller_data[0].controllers[0].pos[0] / 1000,
                       controller_data[0].controllers[0].pos[1] / 1000,
                       controller_data[0].controllers[0].pos[2] / 1000 );

    Vec3f c1_position( controller_data[0].controllers[1].pos[0] / 1000,
                       controller_data[0].controllers[1].pos[1] / 1000,
                       controller_data[0].controllers[1].pos[2] / 1000 );

    Quaternion c0_orientation( controller_data[0].controllers[0].rot_quat[0],
                               controller_data[0].controllers[0].rot_quat[1],
                               controller_data[0].controllers[0].rot_quat[2],
                               controller_data[0].controllers[0].rot_quat[3]);

    Quaternion c1_orientation( controller_data[0].controllers[1].rot_quat[0],
                               controller_data[0].controllers[1].rot_quat[1],
                               controller_data[0].controllers[1].rot_quat[2],
                               controller_data[0].controllers[1].rot_quat[3]);

    c0Position->setValue( c0_position, id );
    c1Position->setValue( c1_position, id );
    Matrix3f m( controller_data[0].controllers[0].rot_mat[0][0], controller_data[0].controllers[0].rot_mat[1][0], controller_data[0].controllers[0].rot_mat[2][0],
                controller_data[0].controllers[0].rot_mat[0][1], controller_data[0].controllers[0].rot_mat[1][1], controller_data[0].controllers[0].rot_mat[2][1], 
                controller_data[0].controllers[0].rot_mat[0][2], controller_data[0].controllers[0].rot_mat[1][2], controller_data[0].controllers[0].rot_mat[2][2] );
   // Rotation r(m);
    c0Orientation->setValue( Rotation(c0_orientation), id );
    c1Orientation->setValue( Rotation(c1_orientation), id );
    c0Trigger->setValue( controller_data[0].controllers[0].trigger, id );
    c1Trigger->setValue( controller_data[0].controllers[1].trigger, id );
 //   Console(4) << m << endl << endl;
    unsigned int c0_buttons = controller_data[0].controllers[0].buttons;
    unsigned int c1_buttons = controller_data[0].controllers[1].buttons;

    c0Buttons->setValue( c0_buttons, id );
    c1Buttons->setValue( c1_buttons, id );

    c0Button1->setValue( c0_buttons & SIXENSE_BUTTON_1, id );
    c0Button2->setValue( c0_buttons & SIXENSE_BUTTON_2, id );
    c0Button3->setValue( c0_buttons & SIXENSE_BUTTON_3, id );
    c0Button4->setValue( c0_buttons & SIXENSE_BUTTON_4, id );
    c0ButtonStart->setValue( c0_buttons & SIXENSE_BUTTON_START, id );
    c0ButtonBumper->setValue( c0_buttons & SIXENSE_BUTTON_BUMPER, id );
    c0ButtonJoystick->setValue( c0_buttons & SIXENSE_BUTTON_JOYSTICK, id );

    c1Button1->setValue( c1_buttons & SIXENSE_BUTTON_1, id );
    c1Button2->setValue( c1_buttons & SIXENSE_BUTTON_2, id );
    c1Button3->setValue( c1_buttons & SIXENSE_BUTTON_3, id );
    c1Button4->setValue( c1_buttons & SIXENSE_BUTTON_4, id );
    c1ButtonStart->setValue( c1_buttons & SIXENSE_BUTTON_START, id );
    c1ButtonBumper->setValue( c1_buttons & SIXENSE_BUTTON_BUMPER, id );
    c1ButtonJoystick->setValue( c1_buttons & SIXENSE_BUTTON_JOYSTICK, id );
  //RazerHydraSensorInternal::razerHydraCallback( this );
#endif
}



