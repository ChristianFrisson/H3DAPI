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
/// \file FalconDevice.h
/// \brief Header file for FalconDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FALCONDEVICE_H__
#define __FALCONDEVICE_H__

#include <H3D/H3DHapticsDevice.h>
#include <H3D/MFString.h>
#include <H3D/SFString.h>
#include <H3D/SFDouble.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class FalconDevice
  /// \brief A FalconDevice is a node for handling communication
  /// with a haptics device from Novint, such as the Falcon Novint.
  /// Note: The value of desiredHapticsRate is ignored for this node.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FalconDevice.x3d">FalconDevice.x3d</a>
  ///     ( <a href="examples/FalconDevice.x3d.html">Source</a> )
  class H3DAPI_API FalconDevice: public H3DHapticsDevice {
  public:

    /// Constructor.
    FalconDevice( 
            Inst< SFVec3f            > _devicePosition         = 0,
            Inst< SFRotation         > _deviceOrientation      = 0,
            Inst< TrackerPosition    > _trackerPosition        = 0,
            Inst< TrackerOrientation > _trackerOrientation     = 0,
            Inst< PosCalibration     > _positionCalibration    = 0,
            Inst< OrnCalibration     > _orientationCalibration = 0,
            Inst< SFVec3f            > _proxyPosition          = 0,
            Inst< WeightedProxy      > _weightedProxyPosition  = 0,
            Inst< SFFloat            > _proxyWeighting         = 0,
            Inst< SFBool             > _mainButton             = 0,
            Inst< SFBool             > _secondaryButton        = 0,
            Inst< SFInt32            > _buttons                = 0,
            Inst< SFVec3f            > _force                  = 0,
            Inst< SFVec3f            > _torque                 = 0,
            Inst< SFInt32            > _inputDOF               = 0,
            Inst< SFInt32            > _outputDOF              = 0,
            Inst< SFInt32            > _hapticsRate            = 0,
            Inst< SFInt32            > _desiredHapticsRate     = 0,
            Inst< SFNode             > _stylus                 = 0,
            Inst< SFHapticsRendererNode > _hapticsRenderer     = 0,
            Inst< MFVec3f            > _proxyPositions         = 0,
            Inst< SFBool             > _followViewpoint        = 0,
            Inst< SFString           > _deviceName             = 0,
            Inst< SFInt32            > _deviceIndex            = 0 );
    
    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual ErrorCode initDevice();

    /// Creates a FalconHapticsDevice in the hapi_device
    /// with name deviceName
    virtual void initialize();

    /// The name of the device, as specified in hdal.ini file for
    /// Falcon Configuration. If set to "", the default device with
    /// index 0 will be used.
    /// Only valid if usedDriver field is "NOVINT". Use deviceIndex
    /// field instead to work with any driver.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "" \n
    auto_ptr< SFString > deviceName;

    /// The index of the device to use. The index identifies which
    /// Falcon device to use if several are connected to the computer.
    /// 
    /// If usedDriver is "NOVINT" and deviceName is specified this 
    /// field will be ignored.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFInt32 > deviceIndex;

    /// The device model of the device. Undefined if device not
    /// initialized. Only valid if the usedDriver field is "NOVINT".
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFString > deviceModelType;

    /// \brief The maximum workspace dimensions of the device, i.e. the
    /// mechanical limits of the device. Undefined if
    /// device not initialized. Contains two values where the first
    /// value is the minimum values and the second the maximum values
    /// for each axis. Only valid if the usedDriver field is "NOVINT".
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< MFVec3f > maxWorkspaceDimensions;

    /// \brief The preferred driver to use to communicate with the falcon
    /// device if several exists on the system. The NIFALCON driver
    /// is in beta stage.
    /// 
    /// <b>Access type: initializeOnly</b> 
    /// <b>Default value: "NOVINT" </b>
    /// <b>Valid values: "NOVINT", "NIFALCON" </b>
    auto_ptr< SFString > preferredDriver;

    /// \brief The driver that is used to communicate with the falcon
    /// device.
    /// 
    /// <b>Access type: outputOnly</b> 
    /// <b>Default value: "NONE" </b>
    /// <b>Possible values: "NOVINT", "NIFALCON", "NONE" </b>
    auto_ptr< SFString > usedDriver;

    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif
