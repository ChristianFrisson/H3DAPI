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
/// \file DynamicTransform.cpp
/// \brief CPP file for DynamicTransform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DynamicTransform.h>
#include <H3D/RK4.h>
#include <H3D/Scene.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DynamicTransform::database( 
                                       "DynamicTransform", 
                                       &(newInstance<DynamicTransform>), 
                                       typeid( DynamicTransform ) );

namespace DynamicTransformInternals {
  FIELDDB_ELEMENT( DynamicTransform, children,        INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, metadata,        INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, bboxCenter,      INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, bboxSize,        INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, orientation,     INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, position,        INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, velocity,        OUTPUT_ONLY  );
  FIELDDB_ELEMENT( DynamicTransform, momentum,        INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, force,           INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, angularVelocity, OUTPUT_ONLY  );
  FIELDDB_ELEMENT( DynamicTransform, angularMomentum, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, spin,            OUTPUT_ONLY  );
  FIELDDB_ELEMENT( DynamicTransform, torque,          INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, mass,            INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, inertiaTensor,   INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, accumulatedInverse, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, accumulatedForward, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DynamicTransform, matrix,          INPUT_OUTPUT );
}


DynamicTransform::DynamicTransform(
                           Inst< AddChildren        > _addChildren,
                           Inst< RemoveChildren     > _removeChildren,
                           Inst< MFChild            > _children,
                           Inst< SFNode             > _metadata,
                           Inst< SFBound            > _bound,  
                           Inst< SFVec3f            > _bboxCenter,
                           Inst< SFVec3f            > _bboxSize,
                           Inst< SFTransformedBound > _transformedBound,
                           Inst< SFMatrix4f         > _matrix,
                           Inst< SFMatrix4f         > _accumulatedForward,
                           Inst< SFMatrix4f         > _accumulatedInverse,
                           Inst< SFVec3f            > _position,
                           Inst< SFRotation         > _orientation,
                           Inst< SFVelocity         > _velocity,
                           Inst< SFVec3f            > _momentum,
                           Inst< SFVec3f            > _force,
                           Inst< SFAngularVelocity  > _angularVelocity,
                           Inst< SFVec3f            > _angularMomentum,
                           Inst< SFSpin             > _spin,
                           Inst< SFVec3f            > _torque,
                           Inst< SFFloat            > _mass,
                           Inst< SFMatrix3f         > _inertiaTensor,
                           Inst< SFMotion           > _motion ):
  MatrixTransform( _addChildren, _removeChildren, _children,
                   _metadata, _bound,_bboxCenter, _bboxSize,
                   _transformedBound, _matrix, 
                   _accumulatedForward, _accumulatedInverse ),
  position        ( _position         ),
  orientation     ( _orientation      ),
  velocity        ( _velocity         ),
  momentum        ( _momentum         ),
  force           ( _force            ),
  angularVelocity ( _angularVelocity  ),
  angularMomentum ( _angularMomentum  ),
  spin            ( _spin             ),
  torque          ( _torque           ),
  mass            ( _mass             ),
  inertiaTensor   ( _inertiaTensor    ),
  motion          ( _motion           )   {
  type_name = "DynamicTransform";
  database.initFields( this );

  position->setValue( Vec3f( 0, 0, 0 ) );
  orientation->setValue( Rotation( 0, 0, 1, 0 ) );
  momentum->setValue( Vec3f( 0, 0, 0 ) );
  force->setValue( Vec3f( 0, 0, 0 ) );
  angularMomentum->setValue( Vec3f( 0, 0, 0 ) );
  torque->setValue( Vec3f( 0, 0, 0 ) );
  
  mass->setValue( 1 );
  inertiaTensor->setValue( Matrix3f( 1, 0, 0,
                                     0, 1, 0,
                                     0, 0, 1 ) );

  orientation->route( matrix );
  position->route( matrix );

  mass->route( velocity, id );
  momentum->route( velocity, id );

  angularVelocity->route( spin, id );
  orientation->route( spin, id );

  inertiaTensor->route( angularVelocity, id );
  angularMomentum->route( angularVelocity, id );

  motion->setOwner( this );
  Scene::time->route( motion );
  motion->route( Scene::eventSink );
}


void DynamicTransform::SFMotion::update() {
  if ( routes_in.size() != 1 )
    return;
  
  H3DTime  t  = static_cast< SFTime *>(routes_in[0])->getValue();

  if( last_t == 0 ) last_t = t;
  H3DTime dt = t - last_t;
  last_t = t;
  if ( dt > 0.2 || dt <= 0 ) return; // ignore large dt's

  // iteratively perform the RK4 integration at 1ms intervals until
  // we have exceeded the current dt.
  LMState state;
  DynamicTransform *ds = static_cast<DynamicTransform*>(owner);
  state.pos = ds->position->getValue(); 
  state.vel = ds->velocity->getValue();
  state.mom = ds->momentum->getValue();
  state.force = ds->force->getValue(); 
  state.orn = Quaternion( ds->orientation->getValue() ); 
  state.spin = ds->spin->getValue();
  state.angVel = ds->angularVelocity->getValue();
  state.angMom = ds->angularMomentum->getValue();
  state.torque = ds->torque->getValue(); 
  state.mass = ds->mass->getValue();
  state.inertiaTensor = ds->inertiaTensor->getValue();

  updateState( state, dt );

  if( ( ds->position->getValue() - state.pos ).length() > Constants::f_epsilon )
    ds->position->setValue( state.pos );
  if( ( ds->momentum->getValue() - state.mom ).length() > Constants::f_epsilon )
    ds->momentum->setValue( state.mom );
  if( H3DSqrt( ( Quaternion( ds->orientation->getValue() ) - state.orn ).norm() ) > Constants::f_epsilon )
    ds->orientation->setValue( Rotation( state.orn ) );
  if( ( ds->angularMomentum->getValue() - state.angMom ).length() > Constants::f_epsilon )
    ds->angularMomentum->setValue( state.angMom );
}

 
void DynamicTransform::SFMotion::updateState( LMState &state, H3DTime dt ) {
  //DynamicTransform *ds = static_cast<DynamicTransform*>(owner);

  // only perform integration if the momentum is non-zero
  // H3DFloat stepsize=0.01; // 1ms
  //while ( dt > stepsize/2 ) {  
  //  LinearMotion::solve( state, stepsize );
  ////  dt = dt - stepsize;
  // }
  
  LinearMotion::solve( state, dt );
}

void DynamicTransform::SFMatrix4f::update() {
  Rotation r  = static_cast< SFRotation *>(routes_in[0])->getValue();
  Vec3f    t  = static_cast< SFVec3f * >(routes_in[1])->getValue();

  H3DFloat t1 = r.axis.x*r.axis.x;
  H3DFloat t2 = H3DCos(r.angle);
  H3DFloat t5 = t1+t2*(1.0f-t1);
  H3DFloat t7 = 1.0f-t2;
  H3DFloat t8 = r.axis.x*r.axis.y*t7;
  H3DFloat t9 = H3DSin(r.angle);
  H3DFloat t10 = r.axis.z*t9;
  H3DFloat t11 = t8-t10;
  H3DFloat t13 = r.axis.x*r.axis.z*t7;
  H3DFloat t14 = r.axis.y*t9;
  H3DFloat t15 = t13+t14;
  H3DFloat t20 = t8+t10;
  H3DFloat t21 = r.axis.y*r.axis.y;
  H3DFloat t24 = t21+t2*(1.0f-t21);
  H3DFloat t26 = r.axis.y*r.axis.z*t7;
  H3DFloat t27 = t9*r.axis.x;
  H3DFloat t28 = t26-t27;
  H3DFloat t33 = t13-t14;
  H3DFloat t34 = t26+t27;
  H3DFloat t35 = r.axis.z*r.axis.z;
  H3DFloat t38 = t35+t2*(1.0f-t35);
  value[0][0] = t5;
  value[0][1] = t11;
  value[0][2] = t15;
  value[0][3] = t.x;
  value[1][0] = t20;
  value[1][1] = t24;
  value[1][2] = t28;
  value[1][3] = t.y;
  value[2][0] = t33;
  value[2][1] = t34;
  value[2][2] = t38;
  value[2][3] = t.z;
  value[3][0] = 0.0f;
  value[3][1] = 0.0f;
  value[3][2] = 0.0f;
  value[3][3] = 1.0f;
}



