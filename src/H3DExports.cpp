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
/// \file H3DExports.cpp
/// \brief cpp file for getH3DExportNode function.
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DExports.h>
#include <H3D/DeviceInfo.h>
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

namespace H3DExportsInternal {
  static AutoRef< H3DHapticsDevice > no_device( NULL ); 
}

Node *H3DExports::getH3DExportNode( const string &name ) {
  if( name == "HDEV" || name == "HDEV0" ) {
    DeviceInfo *di = DeviceInfo::getActive();
    if( di && di->device->size() > 0 ) 
      return di->device->getValueByIndex( 0 );
    else {
      Console(3) << "Warning: No device exists in DeviceInfo when " 
                 << "trying to IMPORT from HDEV from H3D_EXPORTS. "
                 << "Using dummy device instead. " << endl;
      if( !H3DExportsInternal::no_device.get() )
        H3DExportsInternal::no_device.reset( new H3DHapticsDevice );
      return H3DExportsInternal::no_device.get();
    }
  } else if( name == "HDEV1" ) {
    DeviceInfo *di = DeviceInfo::getActive();
    if( di && di->device->size() > 1 ) 
      return di->device->getValueByIndex( 1 );
    else {
      Console(3) << "Warning: No device exists in DeviceInfo when " 
                 << "trying to IMPORT from HDEV1 from H3D_EXPORTS. "
                 << "Using dummy device instead. " << endl;
      if( !H3DExportsInternal::no_device.get() )
        H3DExportsInternal::no_device.reset( new H3DHapticsDevice );
      return H3DExportsInternal::no_device.get();
    }
  }  else if( name == "HDEV2" ) {
    DeviceInfo *di = DeviceInfo::getActive();
    if( di && di->device->size() > 2 ) 
      return di->device->getValueByIndex( 2 );
    else {
      Console(3) << "Warning: No device exists in DeviceInfo when " 
                 << "trying to IMPORT from HDEV2 from H3D_EXPORTS. "
                 << "Using dummy device instead. " << endl;
      if( !H3DExportsInternal::no_device.get() )
        H3DExportsInternal::no_device.reset( new H3DHapticsDevice );
      return H3DExportsInternal::no_device.get();
    }
  }
  return NULL;
}
