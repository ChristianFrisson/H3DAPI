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
//
/// \file RK4.h
/// \brief Contains fourth-order Runge Kutta integration functions.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RK4_H__
#define __RK4_H__

#include <H3D/H3DTypes.h>

using namespace std;

namespace H3D {

  struct DvState {
    // linear state:
    Vec3f vel;
    Vec3f force;

    // angular state:
    Quaternion spin;
    Vec3f torque;
  };

  struct LMState {
    // linear state:
    Vec3f pos;
    Vec3f mom;
    Vec3f vel;
    Vec3f force;

    // angular state:
    Quaternion orn;
    Quaternion spin;
    Vec3f angMom;
    Vec3f angVel;
    Matrix3f inertiaTensor;
    Vec3f torque;

    // constants:
    float mass;
  };

  // Performs fourth-order Runge Kutta integration for solving
  // linear motion of a rigid body.
  struct LinearMotion {

    static void update( LMState &state );

    static DvState evaluate( LMState state );

    static DvState evaluate( LMState state, H3DTime dt, DvState start );

    static void solve( LMState &state, H3DTime dt );
    
  };


};

#endif
