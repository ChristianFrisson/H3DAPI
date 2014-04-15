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
/// \file BoundedPhysicsModel.h
/// \brief Header file for BoundedPhysicsModel, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOUNDEDPHYSICSMODEL_H__
#define __BOUNDEDPHYSICSMODEL_H__

#include <H3D/X3DParticlePhysicsModelNode.h>
#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class BoundedPhysicsModel
  /// \brief The BoundedPhysicsModel node specifies a physics model that
  /// applies a user-defined set of geometrical bounds to the particles.
  ///
  /// The geometry field specifies a piece of geometry that models the bounds
  /// that constrain the location of the particles. When a particle touches the
  /// surface of the bounds, it is reflected. The particles may be restricted
  /// to an inside location or an outside location. All geometry defined by the
  /// bounds are considered to be non-solid, regardless of the setting of the
  /// solid field. It does not matter whether the particle impacts the front or
  /// back side of the geometry. Particles are reflected at the same angle to
  /// the normal of the surface to which they impact, continuing in the same
  /// direction. The calculation of the correct normal is determined by the
  /// rules of the geometry that forms the bounds.
  ///
  /// EXAMPLE  A particle can be made to bounce off an elevation grid
  /// representing terrain.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BoundedPhysicsModel.x3d">BoundedPhysicsModel.x3d</a>
  ///     ( <a href="examples/BoundedPhysicsModel.x3d.html">Source</a> )
  class H3DAPI_API BoundedPhysicsModel : public X3DParticlePhysicsModelNode {
  public:
    typedef TypedSFNode< X3DGeometryNode > SFGeometryNode;
    

    /// Constructor.
    BoundedPhysicsModel( Inst< SFNode         > _metadata = 0,
                         Inst< SFBool         > _enabled  = 0,
                         Inst< SFGeometryNode > _geometry = 0);

    
    virtual void updateParticleValues( 
                const X3DParticleEmitterNode::Particle &last_particle,
                X3DParticleEmitterNode::Particle &particle,
                H3DTime last_time,
                H3DTime current_time );

    /// The geometry field specifies a piece of geometry that models the bounds
    /// that constrain the location of the particles
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFGeometryNode > geometry;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
