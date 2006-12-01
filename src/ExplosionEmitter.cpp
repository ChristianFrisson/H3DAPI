//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file ExplosionEmitter.cpp
/// \brief CPP file for ExplosionEmitter, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ExplosionEmitter.h"
#include "ParticleSystem.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ExplosionEmitter::database( 
                                   "ExplosionEmitter", 
                                   &(newInstance< ExplosionEmitter >), 
                                   typeid( ExplosionEmitter ),
                                   &X3DParticleEmitterNode::database );

namespace ExplosionEmitterInternals {
  FIELDDB_ELEMENT( ExplosionEmitter, position, INPUT_OUTPUT );
}

ExplosionEmitter::ExplosionEmitter( 
                      Inst< SFNode  > _metadata,
                      Inst< SFFloat > _speed,
                      Inst< SFFloat > _variation,
                      Inst< SFFloat > _mass,
                      Inst< SFFloat > _surfaceArea,
                      Inst< SFVec3f > _position ):
  X3DParticleEmitterNode( _metadata, _speed, _variation, _mass, 
                          _surfaceArea ),
  position( _position ) {

  type_name = "ExplosionEmitter";
  database.initFields( this );

  position->setValue( Vec3f( 0, 0, 0 ) );
}



void ExplosionEmitter::generateParticles( ParticleSystem *ps,
                                          H3DTime last_time,
                                          H3DTime now,
                                          list< Particle > &particles ) {
  if( last_time == 0 ) {
    H3DInt32 particles_to_emit = ps->maxParticles->getValue();

    while( particles_to_emit >= 1 ) {
      Vec3f dir( ParticleSystem::getRandomValue( -1, 1 ), 
                 ParticleSystem::getRandomValue( -1, 1 ), 
                 ParticleSystem::getRandomValue( -1, 1 ) ); 
      
      dir.normalizeSafe();
      dir *= speed->getValue();
      Particle p = Particle( Vec3f( 0, 0, 0 ),
                             dir );
      p.size = ps->particleSize->getValue();
      p.type = ps->getCurrentParticleType(); 
      p.total_time_to_live = 20;
      p.time_lived = 0;
      p.geometry.reset( ps->geometry->getValue() );
      p.mass = mass->getValue();
      particles.push_back( p );
      particles_to_emit--;
    }
  }
}
