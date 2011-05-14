//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file TraverseInfo.cpp
/// \brief CPP file for TraverseInfo
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DSurfaceNode.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/H3DHapticsDevice.h>
#include <H3D/TraverseInfo.h>

using namespace H3D;

void TraverseInfo::addHapticShapeToAll( HAPI::HAPIHapticShape *shape ) {
  shape->ref();
  for( unsigned int i = 0; i < haptic_shapes.size(); i++ ) {
    if( hapticsEnabled(i) ) {
      if( shape->getShapeId() == -1 ) {
        X3DGeometryNode *geometry = 
          static_cast< X3DGeometryNode * >( shape->getUserData() );
        shape->setShapeId(
          geometry->getHapticShapeId( geometry_count[ geometry ] ));
        geometry_count[ geometry ]++;
      }
      haptic_shapes[i][current_layer].push_back( shape );
    }
  } 
  shape->unref();
}

void TraverseInfo::addForceEffectToAll( HAPI::HAPIForceEffect *effect ) {
  effect->ref();
  for( unsigned int i = 0; i < haptic_effects.size(); i++ ) {
    if( hapticsEnabled(i) ) {
      haptic_effects[i].push_back( effect );
    }
  } 
  effect->unref();
}

void TraverseInfo::addForceEffect( int device_index, 
                                   HAPI::HAPIForceEffect *effect ) {
  if( device_index < 0 || device_index >= (int)haptics_devices.size() ) {
    stringstream s;
    s << "TraverseInfo only has " << (unsigned int) haptics_devices.size() 
      << " haptics device(s) available. ";
    throw InvalidHapticsDeviceIndex( device_index,
                                     s.str(),
                                     H3D_FULL_LOCATION );
  }
  if( hapticsEnabled(device_index) )
    haptic_effects[device_index].push_back( effect );
  else {
    effect->ref();
    effect->unref();
  }
}

void TraverseInfo::addHapticShape( int device_index, 
                                   HAPI::HAPIHapticShape *shape ) {
  if( device_index < 0 || device_index >= (int)haptics_devices.size() ) {
    stringstream s;
    s << "TraverseInfo only has " << (unsigned int) haptics_devices.size() 
      << " haptics device(s) available. ";
    throw InvalidHapticsDeviceIndex( device_index,
                                     s.str(),
                                     H3D_FULL_LOCATION );
  }
  if( hapticsEnabled( device_index ) ) {
    if( shape->getShapeId() == -1 ) {
        X3DGeometryNode *geometry = 
          static_cast< X3DGeometryNode * >( shape->getUserData() );
        shape->setShapeId( 
          geometry->getHapticShapeId( geometry_count[ geometry ] ) );
        geometry_count[ geometry ]++;
      }
    haptic_shapes[device_index][current_layer].push_back( shape );
  } else {
    shape->ref();
    shape->unref();
  }
}

void TraverseInfo::callPostTraverseCallbacks() {
  for( CallbackList::iterator i = post_traverse_callbacks.begin();
       i != post_traverse_callbacks.end(); i++ ) {
    (*i).first( *this, (*i).second );
  }
}

void TraverseInfo::addActiveLightNode( X3DLightNode *light ) {
  x3dlightnode_vector.push_back( light );
} 
void TraverseInfo::removeActiveLightNode( X3DLightNode *light ) {
  x3dlightnode_vector.erase( light );
}

const TraverseInfo::RefCountedVector &TraverseInfo::getActiveLightNodes() {
  return x3dlightnode_vector;
}
