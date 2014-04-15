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
/// \file WindPhysicsModel.h
/// \brief Header file for WindPhysicsModel, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __WINDPHYSICSMODEL_H__
#define __WINDPHYSICSMODEL_H__

#include <H3D/X3DParticlePhysicsModelNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class WindPhysicsModel
  /// \brief The WindPhysicsModel node specifies a physics model that 
  /// applies a wind effect to the particles. The wind has a random
  /// variation factor that allows for the gustiness of the wind to be
  /// modelled.
  ///
  /// The direction field specifies the direction in which the wind is
  /// travelling in the form of a  normalized, unit vector. 
  ///
  /// The speed field specifies the current wind speed in metres per second.
  /// From the wind speed, the force applied per unit-area on the particle
  /// is calculated using the following formula:
  ///
  /// pressure = 10(2 × log(speed)) × 0.64615
  /// 
  /// The gustiness specifies how much the wind speed varies from the average
  /// value defined by the speed field. The wind speed variation is calculated
  /// once per frame and applied equally to all particles.
  ///
  /// The turbulence field specifies how much the wind acts directly in line
  /// with the direction, and how much variation is applied in directions
  /// other than the wind direction. This is determined per-particle to
  /// model how the particle is effected by turbulence.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/WindPhysicsModel.x3d">WindPhysicsModel.x3d</a>
  ///     ( <a href="examples/WindPhysicsModel.x3d.html">Source</a> )
  class H3DAPI_API WindPhysicsModel : public X3DParticlePhysicsModelNode {
  public:

    /// Constructor.
    WindPhysicsModel( Inst< SFNode  > _metadata   = 0,
                      Inst< SFBool  > _enabled    = 0,
                      Inst< SFVec3f > _direction  = 0,
                      Inst< SFFloat > _gustiness  = 0,
                      Inst< SFFloat > _speed      = 0,
                      Inst< SFFloat > _turbulence = 0 );

    /// Traverse the scene graph. Updates the wind_speed member.
    virtual void traverseSG( TraverseInfo &ti );

    virtual void updateParticleValues( 
                const X3DParticleEmitterNode::Particle &last_particle,
                X3DParticleEmitterNode::Particle &particle,
                H3DTime last_time,
                H3DTime current_time );

    /// The direction field specifies the direction in which the wind
    /// is travelling in the form of a  normalized, unit vector.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f > direction;

    /// The gustiness specifies how much the wind speed varies from the 
    /// average value defined by the speed field. The wind speed variation
    /// is calculated once per frame and applied equally to all particles.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.1 \n
    /// <b>Valid range:</b> [0, inf)
    auto_ptr< SFFloat > gustiness;

    /// The speed field specifies the current wind speed in metres per second.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.1 \n
    /// <b>Valid range:</b> [0, inf)
    auto_ptr< SFFloat > speed;

    /// The turbulence field specifies how much the wind acts directly in 
    /// line with the direction, and how much variation is applied in 
    /// directions other than the wind direction. This is determined 
    /// per-particle to model how the particle is effected by turbulence.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0,1]
    auto_ptr< SFFloat > turbulence;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// The wind speed updated once per frame with gustiness taken
    /// into consideration.
    H3DFloat wind_speed;
  };
}

#endif
