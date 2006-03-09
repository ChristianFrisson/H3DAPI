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
/// \file CoordinateDeformer.cpp
/// \brief CPP file for CoordinateDeformer, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "CoordinateDeformer.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase CoordinateDeformer::database( 
                                    "CoordinateDeformer", 
                                    &(newInstance<CoordinateDeformer>), 
                                    typeid( CoordinateDeformer ),
                                    &H3DCoordinateDeformerNode::database );

namespace CoordinateDeformerInternals {
  FIELDDB_ELEMENT( CoordinateDeformer, distanceToDepth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CoordinateDeformer, plasticity, INPUT_OUTPUT );
}


void CoordinateDeformer::deformPoints( const vector< bool  > &is_touched,
                                       const vector< Vec3f > &touch_points,
                                       const vector< Vec3f > &touch_normal,
                                       const vector< Vec3f > &touch_force,
                                       const vector< Vec3f > &penetration_points,
                                       const vector< Vec3f > &orig_points,
                                       const vector< Vec3f > &resting_points,
                                       const vector< Vec3f > &deformed_points,
                                       vector< Vec3f > &new_resting_points,
                                       vector< Vec3f > &new_deformed_points ) {
  new_deformed_points.clear();
  new_resting_points.clear();

  bool touched = false;
  for( MFBool::const_iterator i = is_touched.begin(); 
       i != is_touched.end(); i++ ) {
    if( *i ) {
      touched = true;
      break;
    }
  }

  H3DFunctionNode *f = distanceToDepth->getValue();
  H3DFloat plasticity_value = plasticity->getValue();
  if( !touched && touched_last_time ) {
    new_deformed_points = resting_points;
  } else if( f && touched ) {
    unsigned int nr_devices = penetration_points.size();
    if( nr_devices > 0 ) {
      for( unsigned int i = 0; i < resting_points.size(); i++ ) {
        H3DFloat distance = ( resting_points[i] - touch_points[0] ).length();
        Vec3f offset = ( penetration_points[0] - touch_points[0] ) * f->get( distance );
        H3DFloat max_depth_sqr = offset * offset;
        for( unsigned int j = 1; j < nr_devices; j++ ) {
          H3DFloat d = ( resting_points[i] - touch_points[j] ).length();
          Vec3f o = ( penetration_points[j] - touch_points[j] ) * f->get( d );
          if( o*o > max_depth_sqr ) {
            offset = o;
          }
        }
        if( plasticity_value != 0 ) {
          new_resting_points.push_back( resting_points[i] + 
                                        offset * plasticity_value );
        }
        new_deformed_points.push_back( resting_points[i] + offset );
      }
    }
  }
  
  touched_last_time = touched;
      
};
