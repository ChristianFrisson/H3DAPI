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
/// \file DeviceInfo.cpp
/// \brief CPP file for DeviceInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "DeviceInfo.h"
#include "X3DBindableNode.cpp"
#include <GL/glew.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DeviceInfo::database( "DeviceInfo", 
                                      &(newInstance<DeviceInfo>), 
                                      typeid( DeviceInfo ),
                                      &X3DBindableNode< DeviceInfo >::database  );

namespace DeviceInfoInternals {
  FIELDDB_ELEMENT( DeviceInfo, device, INPUT_OUTPUT );
}


DeviceInfo::DeviceInfo( 
                       Inst<   SFSetBind >  _set_bind,
                       Inst< SFNode    >  _metadata,
                       Inst<  SFTime    >  _bindTime,
                       Inst<  SFBool    >  _isBound,
                       Inst< MFDevice >  _device ) :
  X3DBindableNode<DeviceInfo>( _set_bind, _metadata, _bindTime, _isBound ),
  device        ( _device         ) {

  type_name = "DeviceInfo";
  database.initFields( this );
}

void DeviceInfo::renderStyli() {
  for( MFDevice::const_iterator i = device->begin();
       i != device->end(); i++ ) {
    H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
    Node *stylus = hd->stylus->getValue();
    if( stylus ) {
      const Vec3f &pos = hd->weightedProxyPosition->getValue();
      const Rotation &rot = hd->trackerOrientation->getValue();
      glPushMatrix();
      glTranslatef( pos.x, pos.y, pos.z );
      glRotatef( (GLfloat)(rot.angle * 180 / Constants::pi), 
                 rot.axis.x, rot.axis.y, rot.axis.z );
      stylus->render();
      glPopMatrix();
    }
  }
}
