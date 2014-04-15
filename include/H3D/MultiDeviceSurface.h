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
/// \file MultiDeviceSurface.h
/// \brief Header file for MultiDeviceSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MULTIDEVICESURFACE_H__
#define __MULTIDEVICESURFACE_H__

#include <H3D/H3DSurfaceNode.h>
#include <H3D/MFNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class MultiDeviceSurface
  /// The MultiDeviceSurface allows you to specify different surface
  /// for different haptics devices in a multi device setup.
  ///
  /// The surface field contains H3DSurfaceNode instances to use by different
  /// devices. The surfaces are mapped onto different devices in the same
  /// order as the devices are specified in DeviceInfo, i.e. the first
  /// surface will be mapped to the first device, the second surface to 
  /// the second device and so on. If there are more devices than surfaces
  /// the last surface is used for these. An empty set of surfaces means 
  /// no surface for any device. If a value is NULL it means no surface 
  /// for that device.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MultiDeviceSurface.x3d">MultiDeviceSurface.x3d</a>
  ///     ( <a href="examples/MultiDeviceSurface.x3d.html">Source</a> )
  class H3DAPI_API MultiDeviceSurface: public H3DSurfaceNode {
  public:

    typedef TypedMFNode< H3DSurfaceNode > MFSurfaceNode;

    /// Constructor.
    MultiDeviceSurface( Inst< MFSurfaceNode > _surface = 0 );

    /// Get the surface to use for the haptics device with the
    /// specified index. Specialization to get different surfaces
    /// depending on the surface field.
    virtual HAPI::HAPISurfaceObject *getSurface( unsigned int device_index = 0 );

    /// The surface field contains H3DSurfaceNode instances to use by different
    /// devices. The surfaces are mapped onto different devices in the same
    /// order as the devices are specified in DeviceInfo, i.e. the first
    /// surface will be mapped to the first device, the second surface to 
    /// the second device and so on. If there are more devices than surfaces
    /// the last surface is used for these. An empty set of surfaces means 
    /// no surface for any device. If a value is NULL it means no surface 
    /// for that device.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile MultiDeviceSurface_surface.dot
    auto_ptr< MFSurfaceNode > surface;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
