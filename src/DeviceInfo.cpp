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
/// \file DeviceInfo.cpp
/// \brief CPP file for DeviceInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DApi.h>
#include <H3D/DeviceInfo.h>
#include <GL/glew.h>

using namespace H3D;

#ifdef MACOSXBLAH
template<>
X3DBindableNode< DeviceInfo >::StackType X3DBindableNode< DeviceInfo >::stack = 
X3DBindableNode< DeviceInfo >::StackType();
#endif

list<DeviceInfo *> DeviceInfo::deviceinfos;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DeviceInfo::database( "DeviceInfo", 
                                      &(newInstance<DeviceInfo>), 
                                      typeid( DeviceInfo ),
                                      &X3DBindableNode::database  );

namespace DeviceInfoInternals {
  FIELDDB_ELEMENT( DeviceInfo, device, INITIALIZE_ONLY );
}


DeviceInfo::DeviceInfo( 
                       Inst<   SFSetBind >  _set_bind,
                       Inst< SFNode    >  _metadata,
                       Inst<  SFTime    >  _bindTime,
                       Inst<  SFBool    >  _isBound,
                       Inst< MFDevice >  _device ) :
  X3DBindableNode( "DeviceInfo", _set_bind, _metadata, _bindTime, _isBound ),
  device        ( _device         ) {

  type_name = "DeviceInfo";
  database.initFields( this );

  deviceinfos.push_back( this );
}

void DeviceInfo::renderStyli() {
  for( MFDevice::const_iterator i = device->begin();
       i != device->end(); ++i ) {
    H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
    if( hd ) hd->renderStylus();
  }
}

void DeviceInfo::removeFromStack() {
  StackType &s =  stack[bindable_stack_name];
  bool is_active = false;
  if( s.size() > 0 ) {
    bool is_active = (s.front() == this);
  }
  X3DBindableNode::removeFromStack();

  for( MFDevice::const_iterator i = device->begin();
       i != device->end(); ++i ) {
    H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
    if( hd ) {
      hd->releaseDevice();
    }
  }
  if( is_active && !stack[bindable_stack_name].empty() ) {
    DeviceInfo *new_top = 
      static_cast< DeviceInfo * >( stack[bindable_stack_name].front() );
    if( new_top ) {
      for( MFDevice::const_iterator i = new_top->device->begin();
        i != new_top->device->end(); ++i ) {
          H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
          if( hd ) {
            hd->initDevice();
          }
      }
    }
  }
}

void DeviceInfo::toStackTop() {
  StackType &s =  stack[bindable_stack_name];
  DeviceInfo *active = NULL;
  if( s.size() > 0 ) active = static_cast< DeviceInfo * >( s.front() );

  vector< H3DHapticsDevice * > initialized_devices;
  if( active != this ) {
    if( active ) {
      for( MFDevice::const_iterator i = active->device->begin();
           i != active->device->end(); ++i ) {
        H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
        if( hd ) {
          hd->releaseDevice();
        }
      }
    }
    X3DBindableNode::toStackTop();
    for( MFDevice::const_iterator i = device->begin();
         i != device->end(); ++i ) {
      H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
      if( hd ) {
        hd->initDevice();
        initialized_devices.push_back( hd );
      }
    }
  } else {
    X3DBindableNode::toStackTop();
    for( MFDevice::const_iterator i = device->begin();
         i != device->end(); ++i ) {
      H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
      if( hd && !hd->initialized->getValue() ) {
        hd->initDevice();
        initialized_devices.push_back( hd );
      }
    }
  }
  for( unsigned int i = 0; i < initialized_devices.size(); ++i ) {
      initialized_devices[i]->postInit();
    }
}

void DeviceInfo::initialize() {
  X3DBindableNode::initialize();
  if( isStackTop() ) {
    vector< H3DHapticsDevice * > initialized_devices;
    for( MFDevice::const_iterator i = device->begin();
         i != device->end(); ++i ) {
      H3DHapticsDevice *hd = static_cast< H3DHapticsDevice * >( *i );
      if( hd && !hd->initialized->getValue() ) {
        hd->initDevice();
        initialized_devices.push_back( hd );
      }
    }
    for( unsigned int i = 0; i < initialized_devices.size(); ++i ) {
      initialized_devices[i]->postInit();
    }
  }
}
