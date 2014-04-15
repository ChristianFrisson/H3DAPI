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
/// \file TimeSensor.cpp
/// \brief CPP file for TimeSensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TimeSensor.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TimeSensor::database( 
                                     "TimeSensor", 
                                     &(newInstance<TimeSensor>), 
                                     typeid( TimeSensor ),
                                     &X3DTimeDependentNode::database );

namespace TimeSensorInternals {
  FIELDDB_ELEMENT( TimeSensor, cycleInterval, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TimeSensor, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TimeSensor, cycleTime, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TimeSensor, fraction_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TimeSensor, time, OUTPUT_ONLY );
}


TimeSensor::TimeSensor( Inst< SFNode >  _metadata,
                        Inst< SFBool >  _loop,
                        Inst< SFTime >  _pauseTime,
                        Inst< SFTime >  _resumeTime,
                        Inst< StartTime > _startTime,
                        Inst< StopTime  > _stopTime,
                        Inst<  SFTime >  _elapsedTime,
                        Inst<  SFBool >  _isActive,
                        Inst<  SFBool >  _isPaused,
                        Inst< TimeHandler > _timeHandler, 
                        Inst< CycleInterval >  _cycleInterval,
                        Inst<  SFTime >  _cycleTime,
                        Inst< SFBool >  _enabled,
                        Inst<  SFFloat>  _fraction_changed,
                        Inst<  SFTime >  _time ) :
  X3DTimeDependentNode( _metadata,_loop,  _pauseTime, _resumeTime, _startTime, 
                        _stopTime, _elapsedTime, _isActive, _isPaused,
                        _timeHandler ),
  cycleInterval   ( _cycleInterval    ),
  cycleTime       ( _cycleTime        ),
  enabled         ( _enabled          ),
  fraction_changed( _fraction_changed ),
  time            ( _time             ),
  activate_through_enabled( false ),
  previous_enabled( true ) {

  type_name = "TimeSensor";
  database.initFields( this );

  cycleInterval->setValue( 1, id );
  cycleTime->setValue( 0, id );
  fraction_changed->setValue( 0, id );
  enabled->setValue( true, id );
  time->setValue( TimeStamp(), id );
}

void TimeSensor::TimeHandler::activate( H3DTime time ) {
  TimeSensor *time_node = 
    static_cast< TimeSensor * >( getOwner() );
  H3DTime start_time = time_node->startTime->getValue();
  time_node->cycleTime->setValue( start_time, time_node->id );
  X3DTimeDependentNode::TimeHandler::activate( time );
  elapsed_cycle_time = time_node->elapsedTime->getValue();

  H3DTime cycle_interval = time_node->cycleInterval->getValue();
  if( elapsed_cycle_time > cycle_interval ) {
    if( time_node->activate_through_enabled &&
        cycle_interval > Constants::d_epsilon ) {
      H3DTime temp_time = elapsed_cycle_time / cycle_interval;
      elapsed_cycle_time = cycle_interval *
        (temp_time - H3DFloor( temp_time ));
      time_node->fraction_changed->setValue( 
                           (H3DFloat)( elapsed_cycle_time / 
                                       cycle_interval ),
                           time_node->id );
    } else
      time_node->fraction_changed->setValue( 1, time_node->id );
  } else {
    time_node->fraction_changed->setValue( 
                           (H3DFloat)( elapsed_cycle_time / 
                                       cycle_interval ),
                           time_node->id );
  }
  time_node->time->setValue( time, time_node->id );
}

void TimeSensor::TimeHandler::update() {
  H3DTime time = static_cast< SFTime * >( event.ptr )->getValue();
  TimeSensor *time_node = 
    static_cast< TimeSensor * >( getOwner() );
  H3DTime cycle_time = time_node->cycleTime->getValue();
  H3DTime cycle_interval = time_node->cycleInterval->getValue();

  bool this_enabled = time_node->enabled->getValue();
  if( this_enabled ) {
    if( time_node->isActive->getValue() ) {
      if( !time_node->isPaused->getValue() ) {
        elapsed_cycle_time = elapsed_cycle_time + time - value;
        if( elapsed_cycle_time >= cycle_interval ) {
          if( time_node->loop->getValue() ) {
            // maybe make sure the not two intervals have
            H3DTime paused_time = (time - cycle_time ) - elapsed_cycle_time;
            H3DTime new_cycle_time = 
              cycle_time +  paused_time;
            new_cycle_time += cycle_interval * 
              H3DFloor( (time - new_cycle_time) / cycle_interval);
            time_node->cycleTime->setValue( new_cycle_time, time_node->id ); 
            elapsed_cycle_time = time - new_cycle_time;
          } else {
            // reached end of cycle and loop is FALSE.
            deactivate( time );
            elapsed_cycle_time = cycle_interval;
          }
        }
        time_node->fraction_changed->setValue( (H3DFloat)( elapsed_cycle_time/ 
                                               cycle_interval ),
                                               time_node->id );
        time_node->time->setValue( time, time_node->id );
      }
    }

    if( time_node->loop->getValue() &&
        time_node->previous_enabled != this_enabled &&
        !time_node->isActive->getValue() &&
        time_node->stopTime->getValue() <= time_node->startTime->getValue() ) {
      time_node->activate_through_enabled = true;
    }

    X3DTimeDependentNode::TimeHandler::update();

    if( time_node->activate_through_enabled &&
        !time_node->isActive->getValue() )
      activate( time );

    if( time_node->activate_through_enabled )
      time_node->activate_through_enabled = false;
  } else {
    if( time_node->isActive->getValue() ) {
      deactivate( time );
    }
  }

  time_node->previous_enabled = this_enabled;
}
