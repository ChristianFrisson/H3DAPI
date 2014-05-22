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
/// \file DeviceInfo.h
/// \brief Header file for DeviceInfo, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

#include <H3D/X3DBindableNode.h>
#include <H3D/H3DHapticsDevice.h>
#include <H3D/MFNode.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class DeviceInfo
  /// DeviceInfo is a X3DBindableNode that contains the devices that
  /// are used with the API. It contains e.g. all H3DHapticsDevice nodes
  /// that are used for haptic rendering, Mouse and Magellan devices.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PhantomDevice.x3d">PhantomDevice.x3d</a>
  ///     ( <a href="examples/PhantomDevice.x3d.html">Source</a> )
  class H3DAPI_API DeviceInfo : public X3DBindableNode {
  public:
    typedef TypedMFNode< H3DHapticsDevice > MFDevice;

    /// Construtor.
    DeviceInfo( Inst< SFSetBind >  _set_bind        = 0,
                Inst< SFNode    >  _metadata        = 0,
                Inst< SFTime    >  _bindTime        = 0,
                Inst< SFBool    >  _isBound         = 0,
                Inst< MFDevice  >  _device          = 0 );

    virtual ~DeviceInfo() {
      removeFromStack();
      deviceinfos.remove( this );
    }

    /// Convenience function to get the top of the DeviceInfo stack.
    static inline DeviceInfo *getActive() {
      return static_cast< DeviceInfo * >( X3DBindableNode::getActive( "DeviceInfo" ) );
    }

    /// If this is the stack top, initialize the devices in it.
    virtual void initialize();

    /// Move this instance to the stack top. 
    virtual void toStackTop();

    /// Remove the bindable node from the stack.
    virtual void removeFromStack();

    /// Render the styli of all devices in the device field.
    virtual void renderStyli();

    /// Contains the devices in this DeviceInfo instance.
    ///
    /// <b>Access type: </b> initializeOnly \n
    auto_ptr< MFDevice >  device;

    typedef list< DeviceInfo * > DeviceInfoList;
    
    /// Returns a list of all current DeviceInfo instances.
    static const DeviceInfoList &getAllDeviceInfos() {
      return deviceinfos;
    }

    static H3DNodeDatabase database;

  protected:
    static DeviceInfoList deviceinfos;
  };
}

#endif
