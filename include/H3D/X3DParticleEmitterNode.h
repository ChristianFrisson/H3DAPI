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
/// \file X3DParticleEmitterNode.h
/// \brief Header file for X3DParticleEmitterNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPARTICLEEMITTERNODE_H__
#define __X3DPARTICLEEMITTERNODE_H__

#include <H3D/X3DNode.h>
#include <H3D/SFFloat.h>
#include <GL/glew.h>

#include <H3D/X3DColorNode.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/TextureCoordinate.h>

// stl includes
#include <list>

namespace H3D {
  // forward declaration
  class ParticleSystem;

  /// \ingroup AbstractNodes
  /// \class X3DParticleEmitterNode
  /// \brief The X3DParticleEmitterNode abstract type represents any node that 
  /// is an emitter of particles.
  ///
  /// The shape and distribution of particles is dependent on the type of the
  /// concrete node.
  ///
  /// The speed field specifies an initial linear speed that will be imparted
  /// to all particles. It does not signify the direction of the particles. The
  /// directional component of the velocity is specified by the concrete node
  /// representation.
  ///
  /// The variation field specifies a multiplier for the randomness that is 
  /// used to control the range of possible output values. The bigger the 
  /// value, the more random the output and the bigger the range of possible
  /// initial values possible. A variation of zero does not allow any 
  /// randomness.
  ///
  /// The mass field specifies the basic mass of each particle in kilograms. 
  /// Mass is needed if gravity or other force-related calculations are to be
  /// performed per-particle.
  ///
  /// The surfaceArea field specifies the surface area of the particle in
  /// metres-squared. Surface area is used for calculations such as wind
  /// effects per particle. The surfaceArea field value represents an average
  /// frontal area that would be presented to the wind, assuming a spherical 
  /// model for each particle (i. e., the surface area is the same regardless
  /// of direction).
  class H3DAPI_API X3DParticleEmitterNode : public X3DNode {
  public:
    
    /// Constructor.
    X3DParticleEmitterNode( Inst< SFNode  > _metadata    = 0,
                            Inst< SFFloat > _speed       = 0,
                            Inst< SFFloat > _variation   = 0,
                            Inst< SFFloat > _mass        = 0,
                            Inst< SFFloat > _surfaceArea = 0 );

    class H3DAPI_API Particle {
    public:
      typedef enum {
        LINE,
        POINT,
        QUAD,
        SPRITE,
        TRIANGLE,
        GEOMETRY
      } ParticleType;

      /// Base class for user defined data stored with each particle
      typedef RefCountedClass UserData;

      /// Constructor.
      Particle( const Vec3f &_position = Vec3f( 0, 0, 0 ),
                const Vec3f &_velocity = Vec3f( 0, 0, 0 ),
                const Vec2f &_size     = Vec2f( 0, 0 ),
                H3DFloat _mass         = 0,
                H3DFloat _surface_area = 0 ):
        mass( _mass ),
        surface_area( _surface_area ),
        position( _position ),
        velocity( _velocity ),
        time_lived( 0 ),
        size( _size ),
        geometry( NULL ),
        distance_from_viewer( 0 ),
        user_data ( NULL ),
		new_particle ( true ) {}
      
      inline void updateParticle( const Matrix4f &_global_to_local,
                                  Vec3f vp_pos_local,
                                  Vec3f vp_up_local,
                                  Vec3f vp_look_at_local,
                                  H3DTime dt ) {
        global_to_local = _global_to_local;
        position += velocity * dt;
        time_lived += dt;
        distance_from_viewer = vp_look_at_local * ( position - vp_pos_local );
        //distance_from_viewer = ( position - vp_pos_local ).lengthSqr();
      }

      /// Utility function for Interpolator nodes to be able to find the index
      /// for the key pair to interpolate between for a given fraction value f. 
      /// The weighting of f between the key pair is set in w.
      int lookupKey( H3DFloat f, H3DFloat &w, const vector< H3DFloat > &keys ) {
        if ( f <= keys[0] ) {
          w = 0;
          return 0;
        }
        if ( f >= keys[keys.size()-1] ) {
          w = 1;
          return (int)keys.size() - 1;
        }

        // we know here that f is larger than keys[0] so we know that we will
        // go into the if-statement at some point.
        for( size_t i = keys.size()-1; true; --i) {
          if ( f > keys[i] ) {
            w = (f-keys[i])/(keys[i+1]-keys[i]);
            if( w < 0 ) w = 0;
            if( w > 1 ) w = 1;
            return (int) i;
          }
        }
        return -1;  // something must have gone wrong to get here
      }   

      void render( ParticleSystem *ps );

      inline bool isDead() {
        return time_lived > total_time_to_live;
      }

      bool operator<( const Particle &p ) {
        return distance_from_viewer > p.distance_from_viewer;
      }

      /// Set user defined data to store with this particle
      ///
      /// The data is assumed to be allocted with new and is destroyed
      /// when the particle is destroyed.
      ///
      void setUserData ( UserData* _data ) {
        user_data.reset ( _data );
      }

      UserData* getUserData () const {
        return user_data.get();
      }

      H3DFloat mass;
      H3DFloat surface_area;
      Vec3f position;
      Vec3f velocity;

	  bool new_particle;

      H3DTime time_lived;
      // the total lifetime of the particle
      H3DTime total_time_to_live;
      Vec2f size;
      ParticleType type;
      AutoRef< X3DGeometryNode > geometry;
      Matrix4f global_to_local;
      H3DFloat distance_from_viewer;
    protected:
      void renderTexCoord( const Vec3f &tc );
      void renderTexCoord( unsigned int i, X3DTextureCoordinateNode *tc );

      AutoRef < UserData > user_data;
    };
 
    virtual void generateParticles( ParticleSystem *ps,
                                    H3DTime last_time,
                                    H3DTime now,
                                    std::list< Particle > &particles ) = 0;

    /// Returns the default xml containerField attribute value.
    /// For this node it is "emitter".
    virtual string defaultXMLContainerField() {
      return "emitter";
    }

    /// The speed field specifies an initial linear speed that will be
    /// imparted to all particles. It does not signify the direction of
    /// the particles. The directional component of the velocity is specified
    /// by the concrete node representation.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFFloat > speed;

    /// The variation field specifies a multiplier for the randomness that is 
    /// used to control the range of possible output values. The bigger the
    /// value, the more random the output and the bigger the range of possible
    /// initial values possible. A variation of zero does not allow any randomness.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.25 \n
    auto_ptr< SFFloat > variation;

    /// The mass field specifies the basic mass of each particle in kilograms.
    /// Mass is needed if gravity or other force-related calculations are to 
    /// be performed per-particle.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFFloat > mass;

    /// The surfaceArea field specifies the surface area of the particle in 
    /// metres-squared. Surface area is used for calculations such as wind
    /// effects per particle. The surfaceArea field value represents an 
    /// average frontal area that would be presented to the wind, assuming
    /// a spherical model for each particle (i.e., the surface area is the
    /// same regardless of direction).
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFFloat > surfaceArea;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    Particle newParticle( ParticleSystem *ps, const Vec3f &pos, const Vec3f &dir );
  };
}

#endif
