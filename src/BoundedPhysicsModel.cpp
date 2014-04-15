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
/// \file BoundedPhysicsModel.cpp
/// \brief CPP file for BoundedPhysicsModel, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/BoundedPhysicsModel.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase BoundedPhysicsModel::database( 
                                   "BoundedPhysicsModel", 
                                   &(newInstance< BoundedPhysicsModel >), 
                                   typeid( BoundedPhysicsModel ),
                                   &X3DParticlePhysicsModelNode::database );

namespace BoundedPhysicsModelInternals {
  FIELDDB_ELEMENT( BoundedPhysicsModel, geometry, INPUT_OUTPUT );
}

BoundedPhysicsModel::BoundedPhysicsModel( 
                      Inst< SFNode  > _metadata,
                      Inst< SFBool  > _enabled,
                      Inst< SFGeometryNode > _geometry ):
  X3DParticlePhysicsModelNode( _metadata, _enabled ),
  geometry( _geometry ) {

  type_name = "BoundedPhysicsModel";
  database.initFields( this );
}


void BoundedPhysicsModel::updateParticleValues( 
                       const X3DParticleEmitterNode::Particle &last_particle,
                       X3DParticleEmitterNode::Particle &particle,
                       H3DTime last_time,
                       H3DTime current_time ) {
  X3DGeometryNode *geom = geometry->getValue();
  if( !enabled->getValue() || !geom ) return;

  HAPI::Collision::BinaryBoundTree *tree = geom->boundTree->getValue();
  if( !tree ) return;
  
  HAPI::Collision::IntersectionInfo intersection;
  if( tree->lineIntersect( last_particle.position,
                           particle.position,
                           intersection ) ) {
    Vec3f neg_v = -particle.velocity;
    Vec3f v_par = (Vec3f) ((neg_v * intersection.normal) * intersection.normal); 
    Vec3f v_perp = neg_v - v_par;
    
    particle.velocity = v_par - v_perp;
    Vec3f n = (Vec3f ) (
      intersection.face == HAPI::Collision::FRONT ? 
      intersection.normal : -intersection.normal );
    particle.position = 
      (Vec3f) intersection.point + 
      1e-7 * n;
    
  }
}



