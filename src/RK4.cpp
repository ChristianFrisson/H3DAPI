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
/// \file RK4.cpp
/// \brief CPP file for fourth-order Runge Kutta integration functions.
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RK4.h>

using namespace H3D;



void LinearMotion::update( LMState &state ) {
  // update linear state:
  state.vel = state.mom / state.mass;

  // update angular state:
  //Quaternion x = state.orn;
  //x.normalize();
  //Quaternion xcnj = Quaternion( -x.v, x.w );
  //state.angVel = ( (1/state.inertiaTensor) * 
  //                 ( state.angMom *xcnj ) ) * x;
  state.angVel = state.inertiaTensor.inverse() * state.angMom;
  //state.orn.normalize();
  state.spin = 0.5 * Quaternion(state.angVel.x, state.angVel.y, 
                                state.angVel.z, 0) * state.orn;

  //state.spin.normalize();
}


DvState LinearMotion::evaluate( LMState state ) {
  DvState newState;
  newState.vel = state.vel;
  newState.force = state.force;   
  newState.spin = state.spin;
  newState.torque = state.torque;
  return newState;
}

DvState LinearMotion::evaluate( LMState state, 
                                H3DTime dt, DvState start ) {
  state.pos    += start.vel    * dt;
  state.mom    += start.force  * dt;
  state.orn    += start.spin   * dt;
  state.angMom += start.torque * dt;
  update( state );

  DvState newState;
  newState.vel = state.vel; // new velocity
  newState.force = state.force; // some function to establish force
  newState.spin = state.spin;
  newState.torque = state.torque;
  return newState;
}


void LinearMotion::solve( LMState &state, H3DTime dt ) {
  // Sample four different DvStates from the current state:
  DvState a = evaluate( state );  // current state
  DvState b = evaluate( state, 0.5*dt, a );
  DvState c = evaluate( state, 0.5*dt, b );
  DvState d = evaluate( state, dt, c );

  // update position and momentum from a weighted average of the
  // four evaluations performed above:
  state.pos    += dt * (a.vel + 2*(b.vel + c.vel) + d.vel) / 6;
  state.mom    += dt * (a.force + 2.0f*(b.force + c.force) + d.force) / 6;
  
  state.orn    += dt * (a.spin + 2.0f*(b.spin + c.spin) + d.spin) / 6;
  state.angMom += dt * (a.torque + 2.0f*(b.torque + c.torque) + d.torque) / 6;

  update( state );
}
