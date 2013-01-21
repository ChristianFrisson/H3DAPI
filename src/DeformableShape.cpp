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
/// \file DeformableShape.cpp
/// \brief CPP file for DeformableShape.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DeformableShape.h>
#include <H3D/Coordinate.h>
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DeformableShape::database( "DeformableShape", 
                                              &(newInstance<DeformableShape>), 
                                              typeid( DeformableShape ),
                                              &X3DShapeNode::database );

namespace DeformableShapeInternals {
  FIELDDB_ELEMENT( DeformableShape, origCoord, OUTPUT_ONLY );
  FIELDDB_ELEMENT( DeformableShape, restingCoord, OUTPUT_ONLY );
  FIELDDB_ELEMENT( DeformableShape, deformedCoord, OUTPUT_ONLY );
  FIELDDB_ELEMENT( DeformableShape, deformer, INPUT_OUTPUT );
}


DeformableShape::DeformableShape(
                  Inst< SFAppearanceNode > _appearance,
                  Inst< SFGeometryNode   > _geometry,
                  Inst< SFHapticGeometry > _hapticGeometry,
                  Inst< SFNode           > _metadata,
                  Inst< SFBound          > _bound,
                  Inst< SFVec3f          > _bboxCenter,
                  Inst< SFVec3f          > _bboxSize,
                  Inst< SFCoordinateNode > _origCoord,
                  Inst< SFCoordinateNode > _restingCoord,
                  Inst< SFCoordinateNode > _deformedCoord,
                  Inst< SFCoordinateDeformer > _deformer):
  X3DShapeNode( _appearance, _geometry, _hapticGeometry, _metadata,
                   _bound, _bboxCenter, _bboxSize ),
  origCoord( _origCoord ),
  restingCoord( _restingCoord ), 
  deformedCoord( _deformedCoord ),
  deformer( _deformer ) {

  type_name = "DeformableShape";
  database.initFields( this );

  origCoord->setValue( new Coordinate, id );
  restingCoord->setValue( new Coordinate, id );
  deformedCoord->setValue( new Coordinate, id );
}

void DeformableShape::traverseSG( TraverseInfo &ti ) {
  bool do_deformation = last_ti != &ti;
  last_ti = &ti;

  X3DShapeNode::traverseSG( ti );
  X3DComposedGeometryNode *graphics_geom = dynamic_cast< X3DComposedGeometryNode * >( geometry->getValue() );
  if( !graphics_geom ) {
    if( geometry->getValue() )
      cerr << "Warning: Invalid geometry type in geometry field for "
           << "DeformableShape node. Must be a X3DComposedGeometryNode." 
           << endl;
    return;
  }
  X3DGeometryNode *haptics_geom = hapticGeometry->getValue();
  if( !haptics_geom ) haptics_geom = graphics_geom;

  if( do_deformation && haptics_geom ) {
    Coordinate *coord_node = 
      dynamic_cast< Coordinate * >( graphics_geom->coord->getValue() );

    H3DCoordinateDeformerNode *deformer_node = deformer->getValue();

    Coordinate *orig_coord = origCoord->getValue();
    Coordinate *resting_coord = restingCoord->getValue();
    Coordinate *deformed_coord = deformedCoord->getValue();

    if( deformer_node && coord_node ) {
      vector< Vec3f > new_resting_coords( resting_coord->point->size() );
      vector< Vec3f > new_deformed_coords( resting_coord->point->size() );

      const vector< bool > &is_touched = haptics_geom->isTouched->getValue();
      unsigned int i = 0;
      vector< Vec3f > penetration_points;
      const vector< H3DHapticsDevice *> &haptics_devices = 
        ti.getHapticsDevices();
      for( vector< H3DHapticsDevice *>::const_iterator hd = 
             haptics_devices.begin();
           hd != haptics_devices.end() && i < is_touched.size();
           hd++, i++ ) {
        penetration_points.push_back( ti.getAccInverseMatrix() * 
                                      (*hd)->weightedProxyPosition->getValue() );
      }

      deformer_node->deformPoints( haptics_geom->isTouched->getValue(), 
                                   haptics_geom->contactPoint->getValue(),
                                   haptics_geom->contactNormal->getValue(),
                                   haptics_geom->force->getValue(),
                                   penetration_points,
                                   orig_coord->point->getValue(),
                                   resting_coord->point->getValue(),
                                   deformed_coord->point->getValue(),
                                   new_resting_coords,
                                   new_deformed_coords );
      if( new_resting_coords.size() > 0 ) {
        resting_coord->point->setValue( new_resting_coords );
      }

      if( new_deformed_coords.size() > 0 ) {
        deformed_coord->point->setValue( new_deformed_coords );
        coord_node->point->setValue( new_deformed_coords );
      }
    }
  }
}

