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
/// \file ParticleSystem.cpp
/// \brief CPP file for ParticleSystem, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ParticleSystem.h>

#include <H3D/X3DViewpointNode.h>

using namespace H3D;

list<ParticleSystem *> ParticleSystem::particlesystems;

H3DNodeDatabase ParticleSystem::database( 
        "ParticleSystem", 
        &(newInstance< ParticleSystem >),
        typeid( ParticleSystem ),
        &X3DShapeNode::database 
          );

namespace ParticleSystemInternals {
  FIELDDB_ELEMENT( ParticleSystem, createParticles, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, lifetimeVariation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, maxParticles, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, particleLifetime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, particleSize, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, isActive, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ParticleSystem, colorRamp, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, colorKey, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, emitter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, geometryType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, physics, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, texCoordRamp, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ParticleSystem, texCoordKey, INPUT_OUTPUT );
}


ParticleSystem::ParticleSystem( 
                    Inst< SFAppearanceNode > _appearance       ,
                    Inst< SFGeometryNode   > _geometry         ,
                    Inst< SFHapticGeometry > _hapticGeometry   ,
                    Inst< SFNode           > _metadata         ,
                    Inst< SFBound          > _bound            ,
                    Inst< SFVec3f          > _bboxCenter       ,
                    Inst< SFVec3f          > _bboxSize         ,
                    Inst< DisplayList      > _displayList      ,
                    Inst< SFBool           > _createParticles  ,
                    Inst< SFBool           > _enabled          ,
                    Inst< SFFloat          > _lifetimeVariation,
                    Inst< SFInt32          > _maxParticles     ,
                    Inst< SFFloat          > _particleLifetime ,
                    Inst< SFVec2f          > _particleSize     ,
                    Inst< SFBool           > _isActive         ,
                    Inst< SFColorNode      > _colorRamp        ,
                    Inst< MFFloat          > _colorKey         ,
                    Inst< SFEmitterNode    > _emitter          ,
                    Inst< SFString         > _geometryType     ,
                    Inst< MFPhysicsModelNode > _physics        ,
                    Inst< SFTextureCoordinate> _texCoordRamp   ,
                    Inst< MFFloat          > _texCoordKey       ):
  X3DShapeNode( _appearance, _geometry, _hapticGeometry, _metadata, _bound,
                _bboxCenter, _bboxSize, _displayList ),
  createParticles( _createParticles ),
  enabled( _enabled ),
  lifetimeVariation( _lifetimeVariation ),
  maxParticles( _maxParticles ),
  particleLifetime( _particleLifetime ),
  particleSize( _particleSize ), 
  isActive( _isActive ),
  colorRamp( _colorRamp ),
  colorKey( _colorKey ),
  emitter( _emitter ),
  geometryType( _geometryType ),
  physics( _physics ),
  texCoordRamp( _texCoordRamp ),
  texCoordKey( _texCoordKey ),
  particle_system_time( 0 ),
  update_bound( false ) {
  
  type_name = "ParticleSystem";
  database.initFields( this );

  createParticles->setValue( true );
  enabled->setValue( true );
  lifetimeVariation->setValue( 0.25 );
  maxParticles->setValue( 200 );
  particleLifetime->setValue( 5 );
  particleSize->setValue( Vec2f( (H3DFloat) 0.02, (H3DFloat)0.02 ) );

  geometryType->addValidValue( "QUAD" );
  geometryType->addValidValue( "TRIANGLE" );
  geometryType->addValidValue( "LINE" );
  geometryType->addValidValue( "POINT" );
  geometryType->addValidValue( "SPRITE" );
  geometryType->addValidValue( "GEOMETRY" );
  geometryType->setValue( "QUAD" );

  last_time = TimeStamp();
  particlesystems.push_back( this );
}


void ParticleSystem::render() {
  X3DChildNode::render();
  X3DAppearanceNode *a = appearance->getValue();
  X3DGeometryNode *g = geometry->getValue();

  if ( a ) {
    glPushAttrib( a->getAffectedGLAttribs() );
    a->preRender();
    a->displayList->callList();
  } 

  if ( particles.size() > 0 ) {
    if( geometry_render_mode == ALL ) {
      for( Particles::iterator p = particles.begin(); 
           p != particles.end(); ++p ) {
        (*p).render( this );
      }
    } else if( geometry_render_mode == SOLID ) {
      // only render non-transparent objects
      if( !a || !a->isTransparent() ) {
        for( Particles::iterator p = particles.begin(); 
           p != particles.end(); ++p ) {
          (*p).render( this );
        }
      }
    }
    else if( a && a->isTransparent() ) {
      if( geometry_render_mode == TRANSPARENT_ONLY ) {
        for( Particles::iterator p = particles.begin(); 
             p != particles.end(); ++p ) {
          (*p).render( this );
        }
      } else if( geometry_render_mode == TRANSPARENT_FRONT ) {
        glPushAttrib( GL_POLYGON_BIT );
        if( geometryType->getValue() == "GEOMETRY" && g ) {
          // we have a geometry so we use the geometry functions for culling
          GLenum previous_cull_face = g->getCullFace();
          bool previous_culling = g->usingCulling();
          if( !previous_culling || previous_cull_face != GL_FRONT ) { 
            g->setCullFace( GL_BACK );
            g->useCulling( true );
            for( Particles::iterator p = particles.begin(); 
              p != particles.end(); ++p ) {
                (*p).render( this );
            }
            g->setCullFace( previous_cull_face );
            g->useCulling( previous_culling );
          }
        } else {
          // not geometry so use OpenGL directly to do face culling
          glCullFace( GL_BACK );
          glEnable( GL_CULL_FACE );
          for( Particles::iterator p = particles.begin(); 
            p != particles.end(); ++p ) {
              (*p).render( this );
          }
        }
        glPopAttrib();
      } else if( geometry_render_mode == TRANSPARENT_BACK ) {
        glPushAttrib( GL_POLYGON_BIT );
        if( geometryType->getValue() == "GEOMETRY" && g ) {
          // we have a geometry so we use the geometry functions for culling
          GLenum previous_cull_face = g->getCullFace();
          bool previous_culling = g->usingCulling();
          if( !previous_culling || previous_cull_face != GL_BACK ) { 
            g->setCullFace( GL_FRONT );
            g->useCulling( true );
            for( Particles::iterator p = particles.begin(); 
              p != particles.end(); ++p ) {
                (*p).render( this );
              }
              g->setCullFace( previous_cull_face );
              g->useCulling( previous_culling );
          }
        } else {
          // not geometry so use OpenGL directly to do face culling
          glCullFace( GL_FRONT );
          glEnable( GL_CULL_FACE );
          for( Particles::iterator p = particles.begin(); 
            p != particles.end(); ++p ) {
              (*p).render( this );
          }
        }
        glPopAttrib();
      } 
    }
  }
  if( a ) {
    a->postRender();
    glPopAttrib();
  }
}

void ParticleSystem::initialize() {
  const Vec3f &size = bboxSize->getValue();
  if( size.x == -1 && size.y == -1 && size.z == -1 ) {
    update_bound = true;
  } else {
    BoxBound *bb = new BoxBound();
    bb->center->setValue( bboxCenter->getValue() );
    bb->size->setValue( bboxSize->getValue() );
    bound->setValue( bb );
  }
  X3DChildNode::initialize();
}

void ParticleSystem::traverseSG( TraverseInfo &ti ) {
  X3DShapeNode::traverseSG( ti );
  
  X3DParticleEmitterNode *emitter_node = emitter->getValue();

  if( emitter_node ) emitter_node->traverseSG( ti );

  for( MFPhysicsModelNode::const_iterator pm = physics->begin();
       pm != physics->end(); ++pm ) {
    if( *pm )(*pm)->traverseSG( ti );
  }

  if( !enabled->getValue() ) {
    particle_system_time = 0;
    particles.clear();
    last_time = TimeStamp();
    return;
  }
  
  // generate new particles
  
  TimeStamp current_time;
  H3DTime dt = current_time - last_time;
  H3DTime last_particle_system_time = particle_system_time;
  particle_system_time += dt;

  if( emitter_node && createParticles->getValue() ) {
    emitter_node->generateParticles( this,
                                     last_particle_system_time,
                                     particle_system_time,
                                     particles );
  }
  
  vector< Particles::iterator > to_remove;
  
  X3DViewpointNode *vp = X3DViewpointNode::getActive();
  if( vp ) {
    Matrix4f vp_to_local = ti.getAccInverseMatrix() *
      vp->accForwardMatrix->getValue();
    
    Matrix3f vp_to_local_rot = vp_to_local.getRotationPart();
      
    Vec3f vp_position =  vp_to_local * vp->totalPosition->getValue();
    Rotation vp_orn = vp->totalOrientation->getValue();
    Vec3f vp_up = vp_to_local_rot * ( vp_orn *  Vec3f( 0, 1, 0 ) );
    Vec3f vp_lookat = vp_to_local_rot * ( vp_orn *  Vec3f( 0, 0, -1 ) );

    for( Particles::iterator p = particles.begin(); 
         p != particles.end(); ++p ) {
      displayList->breakCache();
      X3DParticleEmitterNode::Particle last_particle = *p;
      // update the position of the particle due to velocity
      (*p).updateParticle( ti.getAccInverseMatrix(), 
                           vp_position,
                           vp_up,
                           vp_lookat,
                           dt );
      
      if( !(*p).isDead() ) {
        // update the particles values due to physics effects
        for( MFPhysicsModelNode::const_iterator i = physics->begin();
             i != physics->end(); ++i ) {
          X3DParticlePhysicsModelNode *pm = 
            static_cast< X3DParticlePhysicsModelNode * >( *i );
          pm->updateParticleValues( last_particle, *p, 
                                    last_particle_system_time, 
                                    particle_system_time );
        }
      } else {
        to_remove.push_back( p );
      }
    }
  }
  
  // remove all particles which lifetime has ended
  for( vector< Particles::iterator >::iterator i = to_remove.begin();
       i != to_remove.end(); ++i ) {
    particles.erase( *i );
  }

  particles.sort();


  // update bounding box
  if( particles.size() > 0 && 
      bboxSize->getValue() == Vec3f( -1, -1, -1 )) {

    BoxBound *bb = dynamic_cast< BoxBound * >( bound->getValue() );
    if( !bb ) bb = new BoxBound;
    
    Particles::iterator p = particles.begin(); 
    Vec3f min, max;
    min = max = (*p).position;
    ++p;

    // find the min and max center values for each particle
    for( ; p != particles.end(); ++p ) {
      if( (*p).position.x < min.x ) min.x = (*p).position.x;
      if( (*p).position.y < min.y ) min.y = (*p).position.y;
      if( (*p).position.z < min.z ) min.z = (*p).position.z;
      if( (*p).position.x > max.x ) max.x = (*p).position.x;
      if( (*p).position.y > max.y ) max.y = (*p).position.y;
      if( (*p).position.z > max.z ) max.z = (*p).position.z;
    }
    

    // calculate the size of each particle by looking at the first
    // particle.
    H3DFloat ps = 0;
    p = particles.begin(); 
    if( geometryType->getValue() == "GEOMETRY" ) {
      if( (*p).geometry.get() ) {
        X3DGeometryNode *g = (*p).geometry.get();
        BoxBound *bb = dynamic_cast< BoxBound * >( g->bound->getValue() );
        if( bb ) {
          Vec3f center = bb->center->getValue();
          Vec3f half_size = bb->size->getValue();
          Vec3f min = center - half_size;
          Vec3f max = center + half_size;
          ps = H3DMax( H3DAbs( min.x ), H3DAbs( min.y ) );
          ps = H3DMax( ps, H3DAbs( min.z ) );
          ps = H3DMax( ps, H3DAbs( max.x ) );
          ps = H3DMax( ps, H3DAbs( max.y ) );
          ps = H3DMax( ps, H3DAbs( min.z ) );
        }
      }
    } else {
      Vec2f particle_size = particleSize->getValue();
      ps= H3DMax( particle_size.x, particle_size.y );
    }

    // the final bounding box is the bounding box of point particles 
    // extended by the size of the particle.
    Vec3f s = max - min;
    bb->center->setValue( min + s / 2.0 );
    bb->size->setValue( s  + Vec3f( ps, ps, ps ) );
    bound->setValue( bb, id );
  }


  last_time = current_time;
}

/// Returns a random value in the given range.
H3DFloat ParticleSystem::getRandomValue( H3DFloat min, H3DFloat max ) {
  return min + ( max - min ) * ((H3DFloat)rand() / RAND_MAX );
}

/// Returns a random value in the range main_value +- (variation * main_value )  
H3DFloat ParticleSystem::getVariationValue( H3DFloat main_value, 
                                            H3DFloat variation ) {
  
  H3DFloat v = main_value * variation;
  return getRandomValue( main_value - v, main_value + v );

}

Vec3f ParticleSystem::getRandomPointOnUnitSphere() {
  H3DFloat x1 = getRandomValue( -1, 1 );
  H3DFloat x2 = getRandomValue( -1, 1 );

  H3DFloat x1_sqr = x1 * x1;
  H3DFloat x2_sqr = x2 * x2;
  while( x1_sqr + x2_sqr >= 1 ) {
    x1 = getRandomValue( -1, 1 );
    x1_sqr = x1 * x1;
    x2 = getRandomValue( -1, 1 );
    x2_sqr = x2 * x2;
  }

  H3DFloat s = H3DSqrt( 1 - x1_sqr - x2_sqr );
  return Vec3f( 2 * x1 * s,
                2 * x2 * s,
                1 - 2*(x1_sqr + x2_sqr ) );
}

X3DParticleEmitterNode::Particle::ParticleType
ParticleSystem::getCurrentParticleType() {
  const string &geom_type = geometryType->getValue();
  if( geom_type == "LINE" ) 
    return X3DParticleEmitterNode::Particle::LINE;
  else if( geom_type == "POINT" )
    return X3DParticleEmitterNode::Particle::POINT;
  else if( geom_type == "QUAD" ) 
    return X3DParticleEmitterNode::Particle::QUAD;
  else if( geom_type == "SPRITE" ) 
    return X3DParticleEmitterNode::Particle::SPRITE;
  else if( geom_type == "TRIANGLE" ) 
    return X3DParticleEmitterNode::Particle::TRIANGLE;
  else if( geom_type == "GEOMETRY" ) 
    return X3DParticleEmitterNode::Particle::GEOMETRY;
  else { 
    // TODO: warning message
    return X3DParticleEmitterNode::Particle::QUAD;
  }
}

