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
/// \file CoordinateDeformer.cpp
/// \brief CPP file for CoordinateDeformer, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/CoordinateDeformer.h>

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
  FIELDDB_ELEMENT( CoordinateDeformer, deviceAlgorithm, INPUT_OUTPUT );
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
       i != is_touched.end(); ++i ) {
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
    unsigned int nr_devices = (unsigned int) penetration_points.size();
    if( nr_devices > 0 ) {
      bool sum_offsets = deviceAlgorithm->getValue() == "AVG";
      unsigned int count = 1;
      if( sum_offsets ) {
        // Calculate is_touched if device algorithm is to average
        // since it is the same for all resting_points.
        for( unsigned int i = 0; i < nr_devices; ++i )
          if( is_touched[i] ) ++count;
      }
      for( unsigned int i = 0; i < resting_points.size(); ++i ) {
        Vec3f offset = Vec3f( 0, 0, 0 );
        H3DFloat max_depth_sqr = 0;
        for( unsigned int j = 0; j < nr_devices; ++j ) {
          if( is_touched[j] ) {
            H3DFloat distance = ( resting_points[i] - touch_points[j] ).length();
            Vec3f o = ( penetration_points[j] - touch_points[j] ) *
                        f->evaluate( distance );
            if( sum_offsets )
              offset += o;
            else {
              H3DFloat tmp_o = o * o;
              if( tmp_o > max_depth_sqr ) {
                max_depth_sqr = tmp_o;
                offset = o;
              }
            }
            
          }
        }
        offset /= count;

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
