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
/// \file X3DParticlePhysicsModelNode.h
/// \brief Header file for X3DParticlePhysicsModelNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPARTICLEPHYSICSMODELNODE_H__
#define __X3DPARTICLEPHYSICSMODELNODE_H__

#include <X3DNode.h>
#include <SFBool.h>
#include <X3DParticleEmitterNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DParticlePhysicsModelNode
  /// \brief The X3DParticlePhysicsModelNode is the abstract representation 
  /// of any node that will apply a form of constraints on the particles after
  /// they have been generated.
  ///
  /// The enabled field specifies whether this physics model is currently being
  /// applied to the particles.
  class H3DAPI_API X3DParticlePhysicsModelNode : public X3DNode {
  public:

    /// Constructor.
    X3DParticlePhysicsModelNode( Inst< SFNode > _metadata    = 0,
                                 Inst< SFBool > _enabled       = 0 );

    
    virtual void updateParticleValues( const X3DParticleEmitterNode::Particle &last_particle,
                                       X3DParticleEmitterNode::Particle &particle,
                                       H3DTime last_time,
                                       H3DTime current_time ) = 0;

    /// Returns the default xml containerField attribute value.
    /// For this node it is "physics".
    ///
    virtual string defaultXMLContainerField() {
      return "physics";
    }

    /// The enabled field specifies whether this physics model is currently being
    /// applied to the particles.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > enabled;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
