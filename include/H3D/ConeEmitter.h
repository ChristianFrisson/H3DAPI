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
/// \file ConeEmitter.h
/// \brief Header file for ConeEmitter, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CONEEMITTER_H__
#define __CONEEMITTER_H__

#include <H3D/X3DParticleEmitterNode.h>
#include <H3D/SFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ConeEmitter
  /// \brief The ConeEmitter node is an emitter that generates all the 
  /// available particles from a specific point in space. 
  /// Particles are emitted from the single point specified by the position
  /// field emanating in a direction randomly distributed within the cone 
  /// specified by the angle and direction fields at the speed specified by
  /// the speed field.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ConeEmitter.x3d">ConeEmitter.x3d</a>
  ///     ( <a href="examples/ConeEmitter.x3d.html">Source</a> )
  class H3DAPI_API ConeEmitter : public X3DParticleEmitterNode {
  public:
    
    /// Constructor.
    ConeEmitter( Inst< SFNode  > _metadata    = 0,
                 Inst< SFFloat > _speed       = 0,
                 Inst< SFFloat > _variation   = 0,
                 Inst< SFFloat > _mass        = 0,
                 Inst< SFFloat > _surfaceArea = 0,
                 Inst< SFVec3f > _position    = 0,
                 Inst< SFVec3f > _direction   = 0,
                 Inst< SFFloat > _angle       = 0 );

    virtual void generateParticles( ParticleSystem *ps,
                                    H3DTime last_time,
                                    H3DTime now,
                                    list< Particle > &particles );

    /// The position from which the particles are emitted. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0\n
    auto_ptr< SFVec3f > position;

    /// The direction of the cone in which the particles are emitted. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 1 0\n
    auto_ptr< SFVec3f > direction;

    /// The angle of the cone in which the particles are emitted.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 1 0\n
    /// <b>Allowed values:</b> [0, pi]\n
    auto_ptr< SFFloat > angle;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
