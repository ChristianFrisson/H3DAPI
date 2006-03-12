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
/// \file TraverseInfo.cpp
/// \brief CPP file for TraverseInfo
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "H3DSurfaceNode.h"
#include "X3DGeometryNode.h"
#include "H3DHapticsDevice.h"
#include "TraverseInfo.h"

using namespace H3D;

void TraverseInfo::addHapticShapeToAll( HapticShape *shape ) {
  shape->ref();
  if( hapticsEnabled() ) {
    for( vector< HapticShapeVector >::iterator i = haptic_shapes.begin();
         i != haptic_shapes.end();
         i++ ) {
      (*i).push_back( shape );
    }
  } 
  shape->unref();
}

void TraverseInfo::addForceEffectToAll( HapticForceEffect *effect ) {
  effect->ref();
  if( hapticsEnabled() ) {
    for( vector< HapticEffectVector >::iterator i = 
           haptic_effects.begin();
         i != haptic_effects.end();
         i++ ) {
      (*i).push_back( effect );
    }
  } 
  effect->unref();
}

void TraverseInfo::addForceEffect( int device_index, 
                                   HapticForceEffect *effect ) {
  if( device_index < 0 || device_index >= (int)haptics_devices.size() ) {
    stringstream s;
    s << "TraverseInfo only has " << (unsigned int) haptics_devices.size() 
      << " haptics device(s) available. ";
    throw InvalidHapticsDeviceIndex( device_index,
                                     s.str(),
                                     H3D_FULL_LOCATION );
  }
  if( hapticsEnabled() )
    haptic_effects[device_index].push_back( effect );
  else {
    effect->ref();
    effect->unref();
  }
}

void TraverseInfo::addHapticShape( int device_index, 
                                   HapticShape *shape ) {
  if( device_index < 0 || device_index >= (int)haptics_devices.size() ) {
    stringstream s;
    s << "TraverseInfo only has " << (unsigned int) haptics_devices.size() 
      << " haptics device(s) available. ";
    throw InvalidHapticsDeviceIndex( device_index,
                                     s.str(),
                                     H3D_FULL_LOCATION );
  }
  if( hapticsEnabled() )
    haptic_shapes[device_index].push_back( shape );
  else {
    shape->ref();
    shape->unref();
  }
}

