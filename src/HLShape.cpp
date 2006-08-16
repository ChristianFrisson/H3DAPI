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
#include "OpenHapticsSettings.h"
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
  hl_shape_map.insert( make_pair( shape_id, this ) );
  shape_ids.push_back( shape_id );
  return shape_id;
}

HLShape::~HLShape() {
  for( vector< HLuint >::iterator i = shape_ids.begin();
       i != shape_ids.end();
       i++ ) {
    HLShape::HLShapeMap::iterator hl = hl_shape_map.find( *i );
    if( (*hl).second == this ) {
      hl_shape_map.erase( hl );
    }
  }
}

bool HLShape::closeEnoughToBound( const Vec3f &pos, 
                                  const Matrix4f &m, 
                                  X3DGeometryNode *geometry ) {
  Bound *b = geometry->bound->getValue();
  if( b ) {
    H3DFloat max_distance = 0.01;
    OpenHapticsSettings *default_settings = OpenHapticsSettings::getActive();
    if( default_settings ) {
      max_distance = default_settings->maxDistance->getValue();
    }

    for( X3DGeometryNode::MFOptionsNode::const_iterator i = 
           geometry->options->begin();
         i != geometry->options->end(); i++ ) {
      OpenHapticsOptions *options = dynamic_cast< OpenHapticsOptions * >( *i );
      if( options ) {
        max_distance = options->maxDistance->getValue();
      }
    }
    if( max_distance < 0 ) return true;

    Vec3f local_pos = m * pos;

    if( b->isInside( local_pos ) ) return true;

    Vec3f f = (b->closestPoint( local_pos ) - local_pos);
      
    Matrix3f m3 =  m.getScaleRotationPart();
    Vec3f scale ( ( m3 * Vec3f(1,0,0) ).length(),
                  ( m3 * Vec3f(0,1,0) ).length(),
                  ( m3 * Vec3f(0,0,1) ).length() );
    if( H3DAbs( f.x ) > Constants::f_epsilon ) f.x /= scale.x;
    else f.x = 0;
    if( H3DAbs( f.y ) > Constants::f_epsilon ) f.y /= scale.y;
    else f.y = 0;
    if( H3DAbs( f.z) > Constants::f_epsilon ) f.z /= scale.z;
    else f.z = 0;
    
    H3DFloat l = f.length();
    
    return( l < max_distance );
  }
  cerr << "NO BOUND" << endl;
  return true;
}

#endif
#endif
