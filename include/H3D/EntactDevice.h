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
/// \file EntactDevice.h
/// \brief Header file for EntactDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ENTACTDEVICE_H__
#define __ENTACTDEVICE_H__

#include <H3D/H3DHapticsDevice.h>
#include <H3D/SFString.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class EntactDevice
  /// \brief A EntactDevice is a node for handling communication
  /// with a haptics device from Entact.
  ///
  /// Note that all EntactDevice nodes in the scene have to use the same
  /// way to connect to the device. Either serial number for all, or static
  /// IP addresses for all. Getting a connection when using static ip address
  /// seems to be more reliable so it the recommended approach.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/EntactDevice.x3d">EntactDevice.x3d</a>
  ///     ( <a href="examples/EntactDevice.x3d.html">Source</a> )
  class H3DAPI_API EntactDevice: public H3DHapticsDevice {
  public:

    /// Field class that calibrates the device when a true event is received
    /// or the field is set to true.
    class H3DAPI_API Calibrate: public AutoUpdate< OnValueChangeSField< SFBool > > {
      virtual void onValueChange( const bool &value );
    };

    /// Constructor.
    EntactDevice( 
            Inst< SFVec3f            > _devicePosition         = 0,
            Inst< SFRotation         > _deviceOrientation      = 0,
            Inst< TrackerPosition    > _trackerPosition        = 0,
            Inst< TrackerOrientation > _trackerOrientation     = 0,
            Inst< PosCalibration     > _positionCalibration    = 0,
            Inst< OrnCalibration     > _orientationCalibration = 0,
            Inst< SFVec3f            > _proxyPosition          = 0,
            Inst< WeightedProxy      > _weightedProxyPosition  = 0,     
            Inst< SFFloat            > _proxyWeighting         = 0,
            Inst< SFBool             > _mainButton            = 0,
            Inst< SFBool             > _secondaryButton       = 0,
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
            Inst< SFInt32            > _serialNumber           = 0,
            Inst< SFBool             > _needsCalibration       = 0,
            Inst< Calibrate          > _calibrate              = 0,
            Inst< SFString           > _ipAddress              = 0 );

    /// Creates a EntactHapticsDevice in the hapi_device
    /// member with the specified serial number.
    virtual void initialize();

    /// updateDeviceValues is specialized to set the needsCalibration field
    /// if needed.
    virtual void updateDeviceValues();

    /// The serial number of the device to use. If specified only the
    /// device with the specified serial number can be initialized
    /// by the node. If -1 any Entact device that is connected to 
    /// the computer can be used. Note that all devices in scene must use
    /// the same connection approach.
    /// 
    /// <b>Default value:</b> -1 \n 
    /// <b>Access type:</b> initializeOnly \n 
    auto_ptr< SFInt32  > serialNumber;

    /// This field is true if the device needs to be calibrated.
    /// 
    /// <b>Access type:</b> outputOnly \n   
    auto_ptr< SFBool > needsCalibration;

    /// When a true event is received, the calibration procedure for the 
    /// device is started. The calibration procedure depends on the 
    /// device type.
    /// 
    /// <b>Access type:</b> inputOnly \n   
    auto_ptr< Calibrate > calibrate;

    /// The ip address of the device to use. If specified only the
    /// device with the specified ip address can be initialized
    /// by the node. This field is used if serialNumber is -1.
    /// Note that all devices in scene must use
    /// the same connection approach.
    /// 
    /// <b>Default value:</b> "" \n 
    /// <b>Access type:</b> initializeOnly \n 
    auto_ptr< SFString  > ipAddress;

    
    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif
