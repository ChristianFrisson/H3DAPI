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
/// \file ThreadExample.h
/// \brief Header file for ThreadExample, H3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __THREADEXAMPLE_H__
#define __THREADEXAMPLE_H__

#include "H3D/X3DGeometryNode.h"
#include "H3DUtil/Threads.h"

namespace H3D {

  /// This class is just a simple example of how to set up a thread that
  /// can take care of some calculations and then transfer this to the graphics
  /// loop.
  class ThreadExample : public X3DGeometryNode {
  public:

    /// Constructor.
    ThreadExample( Inst< SFNode  > _metadata    = 0,
            Inst< SFBound > _bound       = 0,
            Inst< MFVec3f > _points      = 0,
            Inst< SFVec3f > _animationDirection   = 0,
            Inst< SFVec3f > _limit       = 0,
            Inst< SFVec3f > _speed       = 0);

    ~ThreadExample(){ if(simulationThread) delete simulationThread; }

    /// Renders the ThreadExample using OpenGL.
    virtual void render();

    virtual void initialize();

    /// Points to define the line
    auto_ptr< MFVec3f > points;

    /// Normal for the direction of movement
    auto_ptr< SFVec3f > animationDirection;

    /// Used to specify the turning points
    auto_ptr< SFVec3f > limit;

    /// The speed in m/s
    auto_ptr< SFVec3f > speed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    PeriodicThread *simulationThread;
    static PeriodicThread::CallbackCode simulate(void *data);
    MutexLock lock;

    /// Function which takes care of one simulation step.
    void mySimulation();
    
    // The variables without a prefix belongs to the
    // scenegraph thread. The prefix rt indicates that the
    // variable should only be used in the simulation thread
    // (or before the simulationthread is started) and the
    // l prefix indicates that the variable is use by both
    // threads and must be surrouned by a lock.lock() lock.unlock() pair

    vector< Vec3f > sg_points;
    vector< Vec3f > l_points;
    vector< Vec3f > rt_points;

    Vec3f l_animationDirection;
    Vec3f rt_animationDirection;

    Vec3f l_limit;
    Vec3f rt_limit;

    Vec3f l_speed;
    Vec3f rt_speed;

    vector< Vec3f > rt_currentDirection;
    H3DDouble rt_last_time;
  };
}

#endif
