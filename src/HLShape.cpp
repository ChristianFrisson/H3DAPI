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
/// \file HLShape.cpp
/// \brief Cpp file for HLShape
///
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DApi.h"
#ifdef USE_HAPTICS
#include "HLShape.h"
#include "X3DGeometryNode.h"
#include "GlobalSettings.h"
#include "OpenHapticsOptions.h"
#include "HapticShape.h"

using namespace H3D;

HLShape::ShapeIdMap HLShape::shape_id_db;

#ifdef HAVE_OPENHAPTICS

HLShape::HLShapeMap HLShape::hl_shape_map;

/// Get the HLAPI shape id that is used to render this shape.
HLuint HLShape::getShapeId( HLHapticsDevice *hd ) { 
  HapticShape *haptic_shape =  dynamic_cast< HapticShape * >( this );
  if( !haptic_shape ) {
    Exception::H3DAPIException( "HLShape object is not a HapticShape",
                                H3D_FULL_LOCATION );
  }
  HLuint shape_id = 
    haptic_shape->geometry->getHLShapeId( hd,
                       HLShape::getFreeShapeIdIndex( haptic_shape->geometry,
                                                     hd ) );
  //cerr << "add: " << shape_id << " " << this << endl;
  hl_shape_map.insert( make_pair( shape_id, this ) );
  shape_ids.push_back( shape_id );
  return shape_id;
}

HLShape::~HLShape() {
  // Removes all instances of this HLShape from
  // the hl_shape_map vector. The assumption is made
  // that there is only one pointer to this object stored
  // in hl_shape_map for each id in shape_ids
  for( vector< HLuint >::iterator i = shape_ids.begin();
       i != shape_ids.end();
       i++ ) {
    HLShape::HLShapeMap::iterator hl = hl_shape_map.find( *i );
    if( (*hl).second == this ) {
      hl_shape_map.erase( hl );
    }
    else {
      for( ; hl != hl_shape_map.end(); hl++ ) {
        if( (*hl).second == this && *i == (*hl).first ) {
          hl_shape_map.erase( hl );
          break;
        }
      }
    }
  }
}


#endif

bool HLShape::closeEnoughToBound( const Vec3f &pos, 
                                  const Vec3f &previous_pos,
                                  const Matrix4f &m, 
                                  X3DGeometryNode *geometry ) {
  Bound *b = geometry->bound->getValue();
  if( b ) {
    H3DFloat max_distance = 0.01f;
    H3DFloat look_ahead_factor = 3;
    OpenHapticsOptions *options = NULL;

    geometry->getOptionNode( options );
    
    if( !options ) {
      GlobalSettings *default_settings = GlobalSettings::getActive();
      if( default_settings ) {
        default_settings->getOptionNode( options );
      }
    }
    
    if( options ) {
      max_distance = options->maxDistance->getValue();
      look_ahead_factor = options->lookAheadFactor->getValue();
    }

    if( max_distance < 0 ) return true;

    Vec3f local_pos = m * pos;

    if( b->isInside( local_pos ) ) return true;

    BoxBound *bb = dynamic_cast< BoxBound * >( b );

    Matrix3f m3 =  m.getScaleRotationPart();
    Vec3f scale ( ( m3 * Vec3f(1,0,0) ).length(),
                  ( m3 * Vec3f(0,1,0) ).length(),
                  ( m3 * Vec3f(0,0,1) ).length() );

    // check if the point is close enough to the point.
    Vec3f f = (b->closestPoint( local_pos ) - local_pos);
    
    if( H3DAbs( f.x ) > Constants::f_epsilon ) f.x /= scale.x;
    else f.x = 0;
    if( H3DAbs( f.y ) > Constants::f_epsilon ) f.y /= scale.y;
    else f.y = 0;
    if( H3DAbs( f.z) > Constants::f_epsilon ) f.z /= scale.z;
    else f.z = 0;
      
    H3DFloat l = f.length();
    
    if( l < max_distance ) return true;

    // look ahead to include geometries in the direction the proxy is moving
    if( bb && look_ahead_factor != 0 ) {
      // expand the bounding box in order for the line segment 
      // intersection test to be with a line with the radius of 
      // max_distance
      BoxBound expanded_box_bound;
      expanded_box_bound.center->setValue( bb->center->getValue() );
      H3DFloat double_max_dist = max_distance * 2;
      expanded_box_bound.size->setValue( bb->size->getValue() + 
                                         scale *  double_max_dist );
      Vec3f previous_local_pos = m * previous_pos;
      Vec3f end_pos = local_pos + 
        ( local_pos - previous_local_pos ) * look_ahead_factor;

      return expanded_box_bound.lineSegmentIntersect( local_pos, end_pos );
      
    } else {
      return false;
    }
  }
  cerr << "NO BOUND" << endl;
  return true;
}

#endif
