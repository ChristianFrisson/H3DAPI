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
/// \file ThreadExample.cpp
/// \brief CPP file for ThreadExample
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ThreadExample.h"
#include "H3D/Scene.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ThreadExample::database( "ThreadExample", 
                                  &(newInstance<ThreadExample>), 
                                  typeid( ThreadExample ),
                                  &X3DGeometryNode::database );

namespace ThreadExampleInternals {
  FIELDDB_ELEMENT( ThreadExample, points, INPUT_ONLY );
  FIELDDB_ELEMENT( ThreadExample, animationDirection, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ThreadExample, limit, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ThreadExample, speed, INPUT_OUTPUT );
}

using namespace ThreadExampleInternals;

ThreadExample::ThreadExample( 
         Inst< SFNode  > _metadata,
         Inst< SFBound > _bound,
         Inst< MFVec3f > _points,
         Inst< SFVec3f > _animationDirection,
         Inst< SFVec3f > _limit,
         Inst< SFVec3f > _speed
          ) :
  X3DGeometryNode( _metadata, _bound ),
  points( _points ),
  animationDirection( _animationDirection ),
  limit( _limit ),
  speed( _speed ) {

  type_name = "ThreadExample";
  database.initFields( this );

  Scene::time->route( displayList );

  animationDirection->setValue( Vec3f( 1, 0, 0 ) );
  limit->setValue( Vec3f( 0.1f, 0.1f, 0.1f ) );
  speed->setValue( Vec3f( 0.01f, 0.01f, 0.01f ) );

  points->route( displayList, id );
}

void ThreadExample::initialize() {
  sg_points = points->getValue(id);
  l_points = sg_points;
  rt_points = l_points;
  l_animationDirection = animationDirection->getValue();
  l_limit = limit->getValue();
  l_speed = speed->getValue();
  for( unsigned int i = 0; i < rt_points.size(); ++i )
    rt_currentDirection.push_back( Vec3f( 1, 1, 1 ) );
  rt_last_time = TimeStamp();
  simulationThread = new PeriodicThread();
  simulationThread->asynchronousCallback( simulate, this );
  X3DGeometryNode::initialize();
}

void ThreadExample::render() {
  Vec3f temp_animationDirection = animationDirection->getValue();
  Vec3f temp_limit = limit->getValue();
  // Get and set values used by both. The reason we
  // store the value of animationDirection and limit is because
  // getValue might have to update the value of the fields before
  // assigning them to the variable that asks for it. This takes time
  // and we want to spend as little time locked as possible.
  lock.lock();
  sg_points = l_points;
  l_animationDirection = temp_animationDirection;
  l_limit = temp_limit;
  lock.unlock();

  if( !sg_points.empty() ) {
    glDisable( GL_LIGHTING );
    glBegin( GL_LINE_STRIP );
    for( H3DInt32 i = 0; i < (H3DInt32)sg_points.size(); ++i) {
      glVertex3f( sg_points[i].x, sg_points[i].y, sg_points[i].z );
    }
    glEnd();
    glEnable( GL_LIGHTING );
  }
}

void ThreadExample::mySimulation() {
  // Updates the properties of the simulation. That is, simply
  // move the points a bit.
  vector< Vec3f > tempPoints = rt_points;
  lock.lock();
  swap( tempPoints, l_points );
  rt_animationDirection = l_animationDirection;
  rt_limit = l_limit;
  rt_speed = l_speed;
  lock.unlock();

  H3DDouble thisTime = TimeStamp();
  H3DFloat deltaT = (H3DFloat)(thisTime - rt_last_time);
  rt_last_time = thisTime;
  Vec3f deltaMotion = deltaT * rt_speed;

  rt_limit.x = H3DAbs( rt_limit.x );
  rt_limit.y = H3DAbs( rt_limit.y );
  rt_limit.z = H3DAbs( rt_limit.z );
  rt_animationDirection.normalizeSafe();
  for( unsigned int i = 0; i < rt_points.size(); ++i ) {
    if( rt_points[i].x > rt_limit.x || rt_points[i].x < -rt_limit.x )
      rt_currentDirection[i].x = -rt_currentDirection[i].x;
    if( rt_points[i].y > rt_limit.y || rt_points[i].y < -rt_limit.y )
      rt_currentDirection[i].y = -rt_currentDirection[i].y;
    if( rt_points[i].z > rt_limit.z || rt_points[i].z < -rt_limit.z )
      rt_currentDirection[i].z = -rt_currentDirection[i].z;

    rt_points[i].x = rt_points[i].x +
      rt_currentDirection[i].x * deltaMotion.x * rt_animationDirection.x;
    rt_points[i].y = rt_points[i].y +
      rt_currentDirection[i].y * deltaMotion.y * rt_animationDirection.y;
    rt_points[i].z = rt_points[i].z +
      rt_currentDirection[i].z * deltaMotion.z * rt_animationDirection.z;
  }
}

PeriodicThread::CallbackCode ThreadExample::simulate( void *_data ) {
  // The callback function simply calls mySimulation of the ThreadExample class
  // contained in the data.
  ThreadExample * myThreadExample = static_cast< ThreadExample * >(_data);
  if( myThreadExample ) {
    myThreadExample->mySimulation();
  }
  return PeriodicThread::CALLBACK_CONTINUE;
}
