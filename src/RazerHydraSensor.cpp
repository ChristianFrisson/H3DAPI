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
/// \file RazerHydraSensor.cpp
/// \brief CPP file for RazerHydraSensor.
///
//
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/RazerHydraSensor.h>
#include <H3D/Scene.h>

#ifdef HAVE_SIXENSE
#ifdef H3D_WINDOWS
#include <H3DUtil/DynamicLibrary.h>
#endif
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
}

#ifdef HAVE_SIXENSE
auto_ptr< H3DUtil::PeriodicThread > RazerHydraSensor::thread_handle = auto_ptr< H3DUtil::PeriodicThread >(NULL);
list< RazerHydraSensor * > RazerHydraSensor::razer_hydra_instances = list< RazerHydraSensor * >();
vector< unsigned char > RazerHydraSensor::last_sequence_number = vector< unsigned char >();
#endif

RazerHydraSensor::RazerHydraSensor( 
              Inst< EnableHydraSensor     > _enabled,
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
    c1ButtonJoystick( _c1ButtonJoystick ) {
  type_name = "RazerHydraSensor";

  database.initFields( this );

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
  
#ifndef HAVE_SIXENSE
  Console(4) << "Warning: H3D API compiled without Sixense SDK. RazerHydraSensor node "
             << "will be unusable." << endl;
#endif
}

RazerHydraSensor::~RazerHydraSensor() {
  enabled->setValue( false );
}

void RazerHydraSensor::EnableHydraSensor::onValueChange( const bool &new_value ) {
#ifdef HAVE_SIXENSE
#ifdef H3D_WINDOWS
  if( DynamicLibrary::load(
#ifdef H3D_WIN32
                            "sixense.dll"
#else
                            "sixense_x64.dll"
#endif
                                          ) ) {
#endif
  RazerHydraSensor * rhs = static_cast< RazerHydraSensor * >(getOwner());
  if( new_value ) {
    if( !thread_handle.get() ) {
      int sixense_initialized = sixenseInit();
      if( sixense_initialized == SIXENSE_FAILURE ) {
        Console(4) << "Warning: Could not initialize Sixense SDK. RazerHydraSensor node "
                   << "will be unusable." << endl;
        rhs->isActive->setValue( false, rhs->id );
        return;
      }
      razer_hydra_instances.push_back( rhs );
      thread_handle.reset( 
            new H3DUtil::PeriodicThread( H3DUtil::PeriodicThread::NORMAL_PRIORITY,
                                                                    60 ) );
      thread_handle->setThreadName( "RazerHydraSensor com thread" );
      thread_handle->asynchronousCallback( razerHydraCallback, NULL );
    } else
      thread_handle->synchronousCallback( RazerHydraSensor::addHydraInstanceCB, rhs );
    rhs->isActive->setValue( true, rhs->id );
  } else {
    thread_handle->synchronousCallback( RazerHydraSensor::removeHydraInstanceCB, rhs );
    if( razer_hydra_instances.empty() && thread_handle.get() ) {
      thread_handle.reset( NULL );
      sixenseExit();
    }
    rhs->isActive->setValue( false, rhs->id );
  }
#ifdef H3D_WINDOWS
  } else {
    Console(4) << "Warning: Sixense SDK not found on this system. "
               << "RazerHydraSensor node will be unusable until sixense.dll "
               << "is added to PATH environment variable." << endl;
  }
#endif
#endif
}

void RazerHydraSensor::traverseSG( TraverseInfo &ti ) {
  X3DSensorNode::traverseSG( ti );
#ifdef HAVE_SIXENSE
  if( enabled->getValue() && isActive->getValue() ) {
    list< RazerHydraData > tmp_hydra_data;
    data_lock.lock();
    tmp_hydra_data.swap( hydra_data );
    data_lock.unlock();
    while( !tmp_hydra_data.empty() ) {
      RazerHydraData &current_data = tmp_hydra_data.front();
      if( current_data.controller == 0 ) {
        c0Connected->setValue( current_data.connected, id );
        if( current_data.connected ) {
          c0Position->setValue( current_data.position, id );
          c0Orientation->setValue( current_data.orientation, id );
          c0Joystick->setValue( current_data.joystick, id );
          c0Trigger->setValue( current_data.trigger, id );
          c0Buttons->setValue( current_data.buttons, id );
          c0Docked->setValue( current_data.docked, id );
          c0WhichHand->setValue( current_data.which_hand, id );
          c0HemiTrackingEnabled->setValue( current_data.hemi_tracking_enabled, id );
          c0Button1->setValue( (current_data.buttons & SIXENSE_BUTTON_1) > 0, id );
          c0Button2->setValue( (current_data.buttons & SIXENSE_BUTTON_2) > 0, id );
          c0Button3->setValue( (current_data.buttons & SIXENSE_BUTTON_3) > 0, id );
          c0Button4->setValue( (current_data.buttons & SIXENSE_BUTTON_4) > 0, id );
          c0ButtonStart->setValue( (current_data.buttons & SIXENSE_BUTTON_START) > 0, id );
          c0ButtonBumper->setValue( (current_data.buttons & SIXENSE_BUTTON_BUMPER) > 0, id );
          c0ButtonJoystick->setValue( (current_data.buttons & SIXENSE_BUTTON_JOYSTICK) > 0, id );
        }
      } else if( current_data.controller == 1 ) {
        c1Connected->setValue( current_data.connected, id );
        if( current_data.connected ) {
          c1Position->setValue( current_data.position, id );
          c1Orientation->setValue( current_data.orientation, id );
          c1Joystick->setValue( current_data.joystick, id );
          c1Trigger->setValue( current_data.trigger, id );
          c1Buttons->setValue( current_data.buttons, id );
          c1Docked->setValue( current_data.docked, id );
          c1WhichHand->setValue( current_data.which_hand, id );
          c1HemiTrackingEnabled->setValue( current_data.hemi_tracking_enabled, id );
          c1Button1->setValue( (current_data.buttons & SIXENSE_BUTTON_1) > 0, id );
          c1Button2->setValue( (current_data.buttons & SIXENSE_BUTTON_2) > 0, id );
          c1Button3->setValue( (current_data.buttons & SIXENSE_BUTTON_3) > 0, id );
          c1Button4->setValue( (current_data.buttons & SIXENSE_BUTTON_4) > 0, id );
          c1ButtonStart->setValue( (current_data.buttons & SIXENSE_BUTTON_START) > 0, id );
          c1ButtonBumper->setValue( (current_data.buttons & SIXENSE_BUTTON_BUMPER) > 0, id );
          c1ButtonJoystick->setValue( (current_data.buttons & SIXENSE_BUTTON_JOYSTICK) > 0, id );
        }
      } else {
        Console(3) << "Warning: Developer forgot to take care of a new case of controller index for RazerHydraSensor " << getName() << " make sure the lazy **** fixes it." << endl;
      }
      tmp_hydra_data.pop_front();
    }
  }
#endif
}

#ifdef HAVE_SIXENSE
void RazerHydraSensor::transferValues( const vector< RazerHydraData > &new_data ) {
  // Only call this function when new_data is not empty.
  data_lock.lock();
  for( int i = new_data.size() -1; i >= 0; --i )
    hydra_data.push_back( new_data[i] );
  data_lock.unlock();
}

PeriodicThread::CallbackCode RazerHydraSensor::addHydraInstanceCB( void *data ) {
  RazerHydraSensor * hydra_to_add = static_cast< RazerHydraSensor * >(data);
  if( find( razer_hydra_instances.begin(),
      razer_hydra_instances.end(),
      hydra_to_add ) == razer_hydra_instances.end() ) {
    razer_hydra_instances.push_back( hydra_to_add );
  }
  return PeriodicThread::CALLBACK_DONE;
}

PeriodicThread::CallbackCode RazerHydraSensor::removeHydraInstanceCB( void *data ) {
  razer_hydra_instances.remove( static_cast< RazerHydraSensor * >(data) );
  return PeriodicThread::CALLBACK_DONE;
}

PeriodicThread::CallbackCode RazerHydraSensor::razerHydraCallback( void* data ) {
  vector< RazerHydraData > razer_hydra_datas;
  vector< unsigned char > next_last_sequence_number;
  vector< bool > controller_updated;
  controller_updated.resize( 2, false );
  for( unsigned int i = 0; i < 10; ++i ) {
    bool all_controller_updated = true;
    for( unsigned int j = 0; j < controller_updated.size(); ++j )
      if( !controller_updated[j] ) {
        all_controller_updated = false;
        break;
      }
    if( all_controller_updated )
      break;
    sixenseAllControllerData controller_data;
    int got_values = sixenseGetAllData( i, &controller_data );
    if( got_values == SIXENSE_FAILURE )
      continue;
    // Currently we only check two controllers, since that is all
    // our razerHydraSensor has.
    for( unsigned int j = 0; j < 2; ++j ) {
      // The check of hardware_revision is basically a "hack way" to figure out if the
      // returned data is valid, since it seems like the razor hydra returns a lot of
      // 0 positions for the first frames and I found no other way to test for that.
      if( controller_updated[j] )
        continue;
      if( next_last_sequence_number.size() <= j ) {
        next_last_sequence_number.resize( j + 1 );
        next_last_sequence_number[j] = controller_data.controllers[j].sequence_number;
      }
      if( last_sequence_number.size() <= j ) {
        last_sequence_number.resize( j + 1 );
        last_sequence_number[j] = controller_data.controllers[j].sequence_number - 1;
      }
      RazerHydraData razer_hydra_data;
      razer_hydra_data.controller = j;
      razer_hydra_data.position = Vec3f( controller_data.controllers[j].pos[0] / 1000,
                                         controller_data.controllers[j].pos[1] / 1000,
                                         controller_data.controllers[j].pos[2] / 1000 );
      // Rotation r(m);
      razer_hydra_data.orientation = Rotation(Quaternion( controller_data.controllers[j].rot_quat[0],
                                                          controller_data.controllers[j].rot_quat[1],
                                                          controller_data.controllers[j].rot_quat[2],
                                                          controller_data.controllers[j].rot_quat[3]));
      razer_hydra_data.trigger = controller_data.controllers[j].trigger;
      razer_hydra_data.buttons = controller_data.controllers[j].buttons;
      razer_hydra_data.joystick = Vec2f( controller_data.controllers[j].joystick_x,
                                         controller_data.controllers[j].joystick_y );
      razer_hydra_data.connected = controller_data.controllers[j].enabled == 1;
      razer_hydra_data.docked = controller_data.controllers[j].is_docked == 1;
      razer_hydra_data.hemi_tracking_enabled = controller_data.controllers[j].hemi_tracking_enabled == 1;
      razer_hydra_data.which_hand = controller_data.controllers[j].which_hand;
      razer_hydra_datas.push_back( razer_hydra_data );
      if( (unsigned char)(last_sequence_number[j] + 1) == controller_data.controllers[j].sequence_number ) {
        controller_updated[j] = true;
      }
    }
  }
  if( !next_last_sequence_number.empty() )
    next_last_sequence_number.swap( last_sequence_number );
  if( !razer_hydra_datas.empty() ) {
    for( list< RazerHydraSensor * >::iterator i = razer_hydra_instances.begin();
         i != razer_hydra_instances.end(); ++i ) {
      (*i)->transferValues( razer_hydra_datas );
    }
  }
  return PeriodicThread::CALLBACK_CONTINUE;
}
#endif
