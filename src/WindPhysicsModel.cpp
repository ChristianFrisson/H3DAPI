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
/// \file WindPhysicsModel.cpp
/// \brief CPP file for WindPhysicsModel, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/WindPhysicsModel.h>
#include <H3D/ParticleSystem.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase WindPhysicsModel::database( 
                                   "WindPhysicsModel", 
                                   &(newInstance< WindPhysicsModel >), 
                                   typeid( WindPhysicsModel ),
                                   &X3DParticlePhysicsModelNode::database );

namespace WindPhysicsModelInternals {
  FIELDDB_ELEMENT( WindPhysicsModel, direction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( WindPhysicsModel, gustiness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( WindPhysicsModel, speed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( WindPhysicsModel, turbulence, INPUT_OUTPUT );
}

WindPhysicsModel::WindPhysicsModel( 
                      Inst< SFNode  > _metadata,
                      Inst< SFBool  > _enabled,
                      Inst< SFVec3f > _direction ,
                      Inst< SFFloat > _gustiness ,
                      Inst< SFFloat > _speed     ,
                      Inst< SFFloat > _turbulence ):
  X3DParticlePhysicsModelNode( _metadata, _enabled ),
  direction( _direction ),
  gustiness( _gustiness ),
  speed( _speed ),
  turbulence( _turbulence ) {

  type_name = "WindPhysicsModel";
  database.initFields( this );

  direction->setValue( Vec3f( 0, 0, 0 ) );
  gustiness->setValue( (H3DFloat )0.1 );
  speed->setValue( (H3DFloat )0.1 );
  turbulence->setValue( 0 );
}


void WindPhysicsModel::updateParticleValues( 
                       const X3DParticleEmitterNode::Particle &last_particle,
                       X3DParticleEmitterNode::Particle &particle,
                       H3DTime last_time,
                       H3DTime current_time ) {
  if( !enabled->getValue() || 
      particle.surface_area == 0 ) return;

  Vec3f dir = direction->getValue();
  /// TODO: this is not random direction, fix it

  /// TODO: what should happen really with turbulation
  if( dir == Vec3f( 0, 0, 0 ) ) {
    dir = ParticleSystem::getRandomPointOnUnitSphere();
  } else if( turbulence->getValue() > 0 ) {

    // Don't know how turbulence should work.

    // the direction will be the directions given +- rotation a random
    // angle between 0 and pi * turbulence
    /*
    // find two perpendicular axis
    Vec3f t( dir.z, dir.x, dir.y );
    Vec3f a1 = dir % t;
    Vec3f a2 = a1 % dir;
    a1.normalizeSafe();
    a2.normalizeSafe();

    H3DFloat angle = Constants::pi * turbulence->getValue(); 
    dir = Rotation( a1, ParticleSystem::getRandomValue( -angle, angle ) ) *
         (Rotation( a2, ParticleSystem::getRandomValue( -angle, angle ) ) * 
         dir );*/

#if 0
    Java3D Xj3D implementation
        public boolean newFrame(int deltaT)
    {
        // calculate the current wind pressure from the speed
        currentPressure = pressure * gustiness * randomiser.nextFloat();

        return true;
    }

    /**
     * Apply this function to the given particle right now.
     *
     * @param particle The particle to apply the function to
     * @return true if the particle has changed, false otherwise
     */
    public boolean apply(Particle particle)
    {

        float force_x = currentPressure * direction[0];
        float force_y = currentPressure * direction[1];
        float force_z = currentPressure * direction[2];

        if(turbulence != 0)
        {
            // apply the turbulence
            force_x += randomiser.nextFloat() * turbulence;
            force_y += randomiser.nextFloat() * turbulence;
            force_z += randomiser.nextFloat() * turbulence;
        }

        // Now scale it by the surface area to get back to newtowns of force
        force_x *= particle.surfaceArea;
        force_y *= particle.surfaceArea;
        force_z *= particle.surfaceArea;

        particle.resultantForce.x += force_x;
        particle.resultantForce.y += force_y;
        particle.resultantForce.z += force_z;

        return true;
    }


    private void recalculatePressure()
    {
        pressure = (float)(Math.pow(10, 2 * Math.log(speed)) * 0.64615);
    }
} 
#endif
  
  }

  H3DFloat wind_diff =  wind_speed - particle.velocity * dir;
  if( wind_diff > 0 ) {
    if( particle.mass == 0 ) {
      particle.velocity += ( wind_diff ) * dir;
    } else {
      // from X3D spec.
      H3DFloat pressure = 
        H3DPow( (H3DFloat) 10.0f,
                2.0f * H3DLog( wind_diff ) )
        * 0.64615f;
      
      H3DFloat force = particle.surface_area * pressure;
      H3DFloat acceleration = force / particle.mass;
      H3DTime dt = current_time - last_time;

      particle.velocity += dir * acceleration * dt;
    }
  }
}



void WindPhysicsModel::traverseSG( TraverseInfo &ti ) {
  X3DParticlePhysicsModelNode::traverseSG( ti );
  wind_speed = ParticleSystem::getVariationValue( speed->getValue(), 
                                                  gustiness->getValue() );
}
