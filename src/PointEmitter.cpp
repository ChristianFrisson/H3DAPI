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
/// \file PointEmitter.cpp
/// \brief CPP file for PointEmitter, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PointEmitter.h>
#include <H3D/ParticleSystem.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PointEmitter::database( 
                                   "PointEmitter", 
                                   &(newInstance< PointEmitter >), 
                                   typeid( PointEmitter ),
                                   &X3DParticleEmitterNode::database );

namespace PointEmitterInternals {
  FIELDDB_ELEMENT( PointEmitter, position, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointEmitter, direction, INPUT_OUTPUT );
}

PointEmitter::PointEmitter( 
                      Inst< SFNode  > _metadata,
                      Inst< SFFloat > _speed,
                      Inst< SFFloat > _variation,
                      Inst< SFFloat > _mass,
                      Inst< SFFloat > _surfaceArea,
                      Inst< SFVec3f > _position,
                      Inst< SFVec3f > _direction ):
  X3DParticleEmitterNode( _metadata, _speed, _variation, _mass, 
                          _surfaceArea ),
  position( _position ),
  direction( _direction ) {

  type_name = "PointEmitter";
  database.initFields( this );

  position->setValue( Vec3f( 0, 0, 0 ) );
  direction->setValue( Vec3f( 0, 1, 0 ) );
}

void PointEmitter::generateParticles( ParticleSystem *ps,
                                      H3DTime last_time,
                                      H3DTime now,
                                      list< Particle > &particles ) {
  if( ps->maxParticles->getValue() <= 0 ) return; 

  H3DFloat emission_rate = 
    ps->maxParticles->getValue() / 
    ps->particleLifetime->getValue();

  H3DTime dt = now - last_time;

  H3DFloat particles_to_emit = emission_rate * (H3DFloat)dt;

  while( particles_to_emit > 0 ) {
    // if the number of particles to emit is a fraction of
    // a particle only add it with the possibility of that fraction.
    // e.g. if particles_to_emit is 0.1, there is a 10 % chance that
    // a particle will be emitted.
    if( particles_to_emit < 1 && 
        rand() > RAND_MAX * particles_to_emit ) break;

    Vec3f dir = direction->getValue();

    // special case: if 0,0,0 use random direction
    if( dir == Vec3f( 0, 0, 0 ) ) {
      dir = ParticleSystem::getRandomPointOnUnitSphere();
    }
    
    Particle p = newParticle( ps, position->getValue(), dir );
    particles.push_back( p );
    --particles_to_emit;
  }
}


