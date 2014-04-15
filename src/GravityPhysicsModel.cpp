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
/// \file GravityPhysicsModel.cpp
/// \brief CPP file for GravityPhysicsModel, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GravityPhysicsModel.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GravityPhysicsModel::database( 
                                   "GravityPhysicsModel", 
                                   &(newInstance< GravityPhysicsModel >), 
                                   typeid( GravityPhysicsModel ),
                                   &X3DParticlePhysicsModelNode::database );

namespace GravityPhysicsModelInternals {
  FIELDDB_ELEMENT( GravityPhysicsModel, gravity, INPUT_OUTPUT );
}

GravityPhysicsModel::GravityPhysicsModel( 
                      Inst< SFNode  > _metadata,
                      Inst< SFBool  > _enabled,
                      Inst< SFVec3f > _gravity ):
  X3DParticlePhysicsModelNode( _metadata, _enabled ),
  gravity( _gravity ) {

  type_name = "GravityPhysicsModel";
  database.initFields( this );

  gravity->setValue( Vec3f( 0, -9.8f, 0 ) );
}


void GravityPhysicsModel::updateParticleValues( 
                          const X3DParticleEmitterNode::Particle &last_particle,
                          X3DParticleEmitterNode::Particle &particle,
                          H3DTime last_time,
                          H3DTime current_time ) {
  if( !enabled->getValue() || particle.mass == 0 ) return;
  H3DTime dt = current_time - last_time;
  particle.velocity = particle.velocity + gravity->getValue() * dt;
  
}



