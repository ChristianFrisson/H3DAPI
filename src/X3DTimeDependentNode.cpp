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
/// \file X3DTimeDependentNode.cpp
/// \brief CPP file for X3DTimeDependentNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTimeDependentNode.h>
#include <H3D/Scene.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DTimeDependentNode::database( 
                                   "X3DTimeDependentNode", 
                                   NULL, 
                                   typeid( X3DTimeDependentNode ),
                                   &X3DChildNode::database );

namespace X3DTimeDependentNodeInternals {
  FIELDDB_ELEMENT( X3DTimeDependentNode, loop, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTimeDependentNode, pauseTime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTimeDependentNode, resumeTime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTimeDependentNode, startTime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTimeDependentNode, stopTime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTimeDependentNode, elapsedTime, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DTimeDependentNode, isActive, OUTPUT_ONLY );
  FIELDDB_ELEMENT( X3DTimeDependentNode, isPaused, OUTPUT_ONLY );
}


X3DTimeDependentNode::X3DTimeDependentNode( 
                                           Inst< SFNode > _metadata,
                                           Inst< SFBool > _loop,
                                           Inst< SFTime > _pauseTime,
                                           Inst< SFTime > _resumeTime,
                                           Inst< StartTime > _startTime,
                                           Inst< StopTime > _stopTime,
                                           Inst< SFTime > _elapsedTime,
                                           Inst< SFBool > _isActive,
                                           Inst< SFBool > _isPaused,
                                           Inst< TimeHandler > _timeHandler ) :
  X3DChildNode( _metadata ),
  loop       ( _loop        ),
  pauseTime  ( _pauseTime   ),
  resumeTime ( _resumeTime  ),
  startTime  ( _startTime   ),
  stopTime   ( _stopTime    ),
  elapsedTime( _elapsedTime ),
  isActive   ( _isActive    ),
  isPaused   ( _isPaused    ),
  timeHandler( _timeHandler ) {

  type_name = "X3DTimeDependentNode";
  database.initFields( this );
  timeHandler->setOwner( this );

  isActive->setValue( false, id );
  isPaused->setValue( false, id );  
  loop->setValue( false );
  pauseTime->setValue( 0 );
  resumeTime->setValue( 0 );
  startTime->setValue( 0 );
  stopTime->setValue( 0 );
  elapsedTime->setValue( 0, id );
  timeHandler->setValue( Scene::time->getValue() );

  Scene::time->routeNoEvent( timeHandler );
}

void X3DTimeDependentNode::initialize() {
  X3DChildNode::initialize();
  if( loop->getValue() && stopTime->getValue() <= startTime->getValue() ) {
    H3DTime now = TimeStamp();
    startTime->setValue( now );
    timeHandler->activate( now );
  }
}

void X3DTimeDependentNode::TimeHandler::activate( H3DTime time ) {
  X3DTimeDependentNode *time_node = 
    static_cast< X3DTimeDependentNode * >( getOwner() );
  H3DTime start_time = time_node->startTime->getValue();
  H3DTime pause_time = time_node->pauseTime->getValue();
  H3DTime resume_time = time_node->resumeTime->getValue();
  
  if( time >= pause_time && pause_time > resume_time ) {
    // getting paused
    if( pause_time < start_time ) {
      time_node->pauseTime->setValue( time ); //?? pause_time, start_time
      time_node->elapsedTime->setValue( 0, time_node->id );
    } else {
      time_node->pauseTime->setValue( time );
      time_node->elapsedTime->setValue( pause_time - start_time, 
                                        time_node->id ); //??
    }
    time_node->isPaused->setValue( true, time_node->id );
    time_node->onPause();
  } else {
    // not getting paused
    time_node->elapsedTime->setValue( time - start_time, time_node->id );
    time_node->onStart();
  }
  time_node->isActive->setValue( true, time_node->id );
}

void X3DTimeDependentNode::TimeHandler::deactivate( H3DTime time ) {
  X3DTimeDependentNode *time_node = 
    static_cast< X3DTimeDependentNode * >( getOwner() );
  time_node->isActive->setValue( false, time_node->id );
  time_node->onStop();
}

void X3DTimeDependentNode::TimeHandler::update() {
  H3DTime time = static_cast< SFTime * >( event.ptr )->getValue();
  X3DTimeDependentNode *time_node = 
    static_cast< X3DTimeDependentNode * >( getOwner() );
  H3DTime start_time = time_node->startTime->getValue();
  H3DTime stop_time = time_node->stopTime->getValue();
  H3DTime pause_time = time_node->pauseTime->getValue();
  H3DTime resume_time = time_node->resumeTime->getValue();
  H3DTime elapsed_time = time_node->elapsedTime->getValue();
  H3DTime last_time = value;

  if( time_node->isActive->getValue() ) {
    if( !time_node->isPaused->getValue() ) {
      if( stop_time > start_time && time >= stop_time ) {
        deactivate( time );
      } else {
        if( time >= pause_time && pause_time > resume_time ) {
          // getting paused
          time_node->pauseTime->setValue( time ); //?? pause_time, start_time
          time_node->elapsedTime->setValue( elapsed_time + 
                                            pause_time - last_time, 
                                            time_node->id ); //??
          time_node->isPaused->setValue( true, time_node->id );
          time_node->onPause();
        } else {
          time_node->elapsedTime->setValue( elapsed_time + 
                                            time - last_time,
                                            time_node->id );
        }
      }
    } else {
      // isPaused
      if( time >= resume_time && resume_time > pause_time ) {
        time_node->isPaused->setValue( false, time_node->id );
        time_node->resumeTime->setValue( time );
        time_node->onResume();
      }
    }
  } else {
    // isActive is false
    if( last_time <= start_time && time >= start_time ) {
      activate( time );
    }
  }
  value = time;
}

