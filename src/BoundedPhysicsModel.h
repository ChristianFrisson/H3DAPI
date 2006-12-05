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
/// \file BoundedPhysicsModel.h
/// \brief Header file for BoundedPhysicsModel, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOUNDEDPHYSICSMODEL_H__
#define __BOUNDEDPHYSICSMODEL_H__

#include <X3DParticlePhysicsModelNode.h>
#include <X3DGeometryNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class BoundedPhysicsModel
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

    auto_ptr< SFGeometryNode > geometry;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
