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
/// \file PhantomDevice.h
/// \brief Header file for PhantomDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PHANTOMDEVICE_H__
#define __PHANTOMDEVICE_H__

#include <H3D/H3DHapticsDevice.h>
#include <H3D/MFString.h>
#include <H3D/SFString.h>
#include <H3D/SFDouble.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class PhantomDevice
  /// \brief A PhantomDevice is a node for handling communication
  /// with a haptics device from SensAble, such as the Phantom Omni and
  /// the Phantom Desktop haptics devices.
  /// Note: The valid values for desiredHapticsRate depends on the type of
  /// interface and communication protocol used. PCI and EPP support 500,
  /// 1000, and 2000 Hz. Firewire supports 500, 1000, 1600 Hz, plus some
  /// increments in between based on the following expression:
  /// floor(8000/N + 0.5). The first successful initialization of PhantomDevice
  /// will decide the haptics thread rate since only one scheduler is used by
  /// OpenHaptics even for dual device configurations.
  class H3DAPI_API PhantomDevice: public H3DHapticsDevice {
  public:

    /// Constructor.
    PhantomDevice( 
            Inst< SFVec3f            > _devicePosition         = 0,
            Inst< SFRotation         > _deviceOrientation      = 0,
            Inst< TrackerPosition    > _trackerPosition        = 0,
            Inst< TrackerOrientation > _trackerOrientation     = 0,
            Inst< PosCalibration     > _positionCalibration    = 0,
            Inst< OrnCalibration     > _orientationCalibration = 0,
            Inst< SFVec3f            > _proxyPosition          = 0,
            Inst< WeightedProxy      > _weightedProxyPosition  = 0,     
            Inst< SFFloat            > _proxyWeighting         = 0,
            Inst< MainButton         > _main_button            = 0,
            Inst< SecondaryButton    > _secondary_button       = 0,
            Inst< SFInt32            > _buttons                = 0,
            Inst< SFVec3f            > _force                  = 0,
            Inst< SFVec3f            > _torque                 = 0,
            Inst< SFInt32            > _inputDOF               = 0,
            Inst< SFInt32            > _outputDOF              = 0,
            Inst< SFInt32            > _hapticsRate            = 0,
            Inst< SFInt32            > _desiredHapticsRate     = 0,
            Inst< SFNode             > _stylus                 = 0,
            Inst< SFFloat            > _proxyRadius            = 0,
            Inst< SFString           > _deviceName             = 0 );
    
    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual ErrorCode initDevice();

    /// This function is used to transfer device values, such as position, 
    /// button status etc from the realtime loop to the fields of 
    /// H3DHapticsDevice, and possible vice versa.
    virtual void updateDeviceValues();

    /// Creates a PhantomHapticsDevice in the hapi_device
    /// with name deviceName
    virtual void initialize();

    /// The name of the device, as specified in Phantom Configuration
    /// utility. If set to "", the default device will be used. 
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "" \n
    auto_ptr< SFString > deviceName;

    ///  The HDAPI software version, in the form major.minor.build
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFString > HDAPIVersion;

    /// The device model of the device. Undefined if device not
    /// initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFString > deviceModelType;

    /// The device driver version of the device. Undefined if device not
    /// initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFString > deviceDriverVersion;

    /// The device firmware version. Undefined if device not
    /// initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFDouble > deviceFirmwareVersion;

    /// The vendor of the device. Undefined if device not
    /// initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFString > deviceVendor;

    /// The serial number of the device. Undefined if device not
    /// initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFString > deviceSerialNumber;

    /// \brief The maximum workspace dimensions of the device, i.e. the
    /// mechanical limits of the device. Undefined if
    /// device not initialized. Contains two values where the first
    /// value is the minimum values and the second the maximum values
    /// for each axis.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< MFVec3f > maxWorkspaceDimensions;

    /// \brief the usable workspace dimensions of the device, i.e. the 
    /// workspace in which forces are guaranteed to be reliably render.
    /// Undefined if device not initialized. Contains two values where the
    /// first value is the minimum values and the second the maximum values
    /// for each axis.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< MFVec3f > usableWorkspaceDimensions;

    /// The mechanical offest of the device end-effector in y from
    /// the table top. Undefined if device not initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFFloat > tabletopOffset;

    /// The maximum force, i.e. the amount of force that the
    /// device can sustain when the motors are at room temperature
    /// Undefined if device not initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFFloat > maxForce;

    /// \brief The maximum continuous force, i.e. the amount of
    /// force that the device can sustain through a period of time.
    /// Undefined if device not initialized.
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFFloat > maxContinuousForce;

    /// Current gimbal angles for the device (in radians).
    /// From neutral position:
    ///  - x - Right     = +
    ///  - y - Up        = -
    ///  - z - Clockwise = +
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFVec3f > gimbalAngles;

    /// Current joint angles for the device (in radians).
    /// - x - turret, + = left
    /// - y - thigh,  + = up
    /// - z - shin,   + = up
    /// 
    /// <b>Access type: outputOnly</b> 
    auto_ptr< SFVec3f > jointAngles;

    /// This field is true if the device needs to be calibrated.
    /// 
    /// <b>Access type:</b> outputOnly \n   
    auto_ptr< SFBool > needsCalibration;

    /// When a true event is received, the calibration procedure for the 
    /// device is started. The calibration procedure depends on the 
    /// device type.
    /// 
    /// <b>Access type:</b> inputOnly \n   
    auto_ptr< SFBool > calibrate;

    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif
