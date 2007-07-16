//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file ConeEmitter.cpp
/// \brief CPP file for ConeEmitter, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <ConeEmitter.h>
#include <ParticleSystem.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ConeEmitter::database( 
                                   "ConeEmitter", 
                                   &(newInstance< ConeEmitter >), 
                                   typeid( ConeEmitter ),
                                   &X3DParticleEmitterNode::database );

namespace ConeEmitterInternals {
  FIELDDB_ELEMENT( ConeEmitter, position, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConeEmitter, direction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConeEmitter, angle, INPUT_OUTPUT );
}

ConeEmitter::ConeEmitter( 
                      Inst< SFNode  > _metadata,
                      Inst< SFFloat > _speed,
                      Inst< SFFloat > _variation,
                      Inst< SFFloat > _mass,
                      Inst< SFFloat > _surfaceArea,
                      Inst< SFVec3f > _position,
                      Inst< SFVec3f > _direction,
                      Inst< SFFloat > _angle ):
  X3DParticleEmitterNode( _metadata, _speed, _variation, _mass, 
                          _surfaceArea ),
  position( _position ),
  direction( _direction ),
  angle( _angle ) {

  type_name = "ConeEmitter";
  database.initFields( this );

  position->setValue( Vec3f( 0, 0, 0 ) );
  direction->setValue( Vec3f( 0, 1, 0 ) );
  angle->setValue( Constants::pi / 4.0f );
}

void ConeEmitter::generateParticles( ParticleSystem *ps,
                                      H3DTime last_time,
                                      H3DTime now,
                                      list< Particle > &particles ) {
  if( ps->maxParticles->getValue() <= 0 ) return; 

  H3DFloat emission_rate = 
    ps->maxParticles->getValue() / 
    ps->particleLifetime->getValue();

  H3DTime dt = now - last_time;

  H3DFloat particles_to_emit = emission_rate * dt;

  while( particles_to_emit > 0 ) {
    // if the number of particles to emit is a fraction of
    // a particle only add it with the possibility of that fraction.
    // e.g. if particles_to_emit is 0.1, there is a 10 % chance that
    // a particle will be emitted.
    if( particles_to_emit < 1 && 
        rand() > RAND_MAX * particles_to_emit ) break;

    Vec3f dir = direction->getValue();

    // find a vector perpendicular to the dir
    Vec3f perp = dir % Vec3f( dir.z, dir.x, dir.y );
    
    // TODO: what is the angle? should it really be halved?
    dir = Rotation( perp, 
                    ParticleSystem::getRandomValue( 0, 
                                                    angle->getValue() / 2 ) ) *
      dir;
    dir = Rotation( direction->getValue(), 
                    ParticleSystem::getRandomValue( 0, Constants::pi * 2.0f ) ) *
      dir;
                    

    Particle p = newParticle( ps, position->getValue(), dir );
    particles.push_back( p );
    particles_to_emit--;
  }
}


