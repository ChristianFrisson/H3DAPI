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
/// \file GravityPhysicsModel.h
/// \brief Header file for GravityPhysicsModel, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GRAVITYPHYSICSMODEL_H__
#define __GRAVITYPHYSICSMODEL_H__

#include <H3D/X3DParticlePhysicsModelNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class GravityPhysicsModel
  /// \brief The GravityPhysicsModel node specifies a physics model that 
  /// applies a constant gravity value to the particles. Gravity may act
  /// in any given direction vector at any strength.
  ///
  /// The gravity field is used to indicate the strength and direction
  /// of the force of gravity that should be applied. Force is specified
  /// in metres per seconds-squared (m/s2). If the particles are defined
  /// to have zero mass by the emitter, the GravityPhysicsModel node has 
  /// no effect.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GravityPhysicsModel.x3d">GravityPhysicsModel.x3d</a>
  ///     ( <a href="examples/GravityPhysicsModel.x3d.html">Source</a> )
  class H3DAPI_API GravityPhysicsModel : public X3DParticlePhysicsModelNode {
  public:

    /// Constructor.
    GravityPhysicsModel( Inst< SFNode  > _metadata = 0,
                         Inst< SFBool  > _enabled  = 0,
                         Inst< SFVec3f > _gravity  = 0);

    
    virtual void updateParticleValues( const X3DParticleEmitterNode::Particle &last_particle,
                                       X3DParticleEmitterNode::Particle &particle,
                                       H3DTime last_time,
                                       H3DTime current_time );

    /// The gravity field is used to indicate the strength and direction
    /// of the force of gravity that should be applied. Gravity is specified
    /// in metres per seconds-squared (m/s2). 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, -9.8, 0 ) \n
    auto_ptr< SFVec3f > gravity;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
