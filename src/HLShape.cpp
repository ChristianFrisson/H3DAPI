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

#include "HLShape.h"
#include "X3DGeometryNode.h"
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

#endif
