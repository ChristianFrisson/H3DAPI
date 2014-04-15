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
/// \file SurfaceEmitter.cpp
/// \brief CPP file for SurfaceEmitter, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SurfaceEmitter.h>
#include <H3D/ParticleSystem.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SurfaceEmitter::database( 
                                   "SurfaceEmitter", 
                                   &(newInstance< SurfaceEmitter >), 
                                   typeid( SurfaceEmitter ),
                                   &X3DParticleEmitterNode::database );

namespace SurfaceEmitterInternals {
  FIELDDB_ELEMENT( SurfaceEmitter, surface, INPUT_OUTPUT );
}

SurfaceEmitter::SurfaceEmitter( 
                      Inst< SFNode  > _metadata,
                      Inst< SFFloat > _speed,
                      Inst< SFFloat > _variation,
                      Inst< SFFloat > _mass,
                      Inst< SFFloat > _surfaceArea,
                      Inst< SFGeometryNode > _surface ):
  X3DParticleEmitterNode( _metadata, _speed, _variation, _mass, 
                          _surfaceArea ),
  surface( _surface ) {

  type_name = "SurfaceEmitter";
  database.initFields( this );
}

void SurfaceEmitter::generateParticles( ParticleSystem *ps,
                                      H3DTime last_time,
                                      H3DTime now,
                                      list< Particle > &particles ) {
  X3DGeometryNode *geom = surface->getValue();  
  if( ps->maxParticles->getValue() <= 0 || !geom ) return; 

  H3DFloat emission_rate = 
    ps->maxParticles->getValue() / 
    ps->particleLifetime->getValue();

  H3DTime dt = now - last_time;

  H3DFloat particles_to_emit = (H3DFloat)(emission_rate * dt);

  vector< HAPI::Collision::Triangle > tris;
  tris.reserve( 200 );
  geom->boundTree->getValue()->getAllTriangles( tris );

  SFBool *solid_field = dynamic_cast< SFBool * >( geom->getField( "solid" ) );

  bool solid = solid_field ? solid_field->getValue() : false;
  
  while( particles_to_emit > 0 ) {
    // if the number of particles to emit is a fraction of
    // a particle only add it with the possibility of that fraction.
    // e.g. if particles_to_emit is 0.1, there is a 10 % chance that
    // a particle will be emitted.
    if( particles_to_emit < 1 && 
        rand() > RAND_MAX * particles_to_emit ) break;

    // find random triangle
    unsigned int triangle_index = (unsigned int)
      ParticleSystem::getRandomValue( 0, 
                                      tris.size() - 1e-7f  );
    const HAPI::Collision::Triangle &triangle = tris[ triangle_index ];
    
    Vec3f ab = (Vec3f)(triangle.b - triangle.a);
    Vec3f ac = (Vec3f)(triangle.c - triangle.a);
    
    // Compute normal. TODO: normal per vertex, need boundTree
    /// triangles updated with normal info
    Vec3f dir = ab % ac;
    dir.normalizeSafe();

    // maybe flip normal if solid field is false 
    if( !solid && ParticleSystem::getRandomValue( 0, 1 ) > 0.5 ) {
      dir = -dir;
    } 

    // find a random point in the triangle
    H3DFloat u = ParticleSystem::getRandomValue( 0, 1 );
    H3DFloat v = ParticleSystem::getRandomValue( 0, 1 );
    if( u + v > 1 ) {
      u = 1 - u;
      v = 1 - v; 
    }

    Vec3f pos = 
      (Vec3f)(triangle.a * u + triangle.b * v + triangle.c * (1-u-v ));
    
    // create new particle
    Particle p = newParticle( ps, pos, dir );
    particles.push_back( p );
    --particles_to_emit;
  }
}


