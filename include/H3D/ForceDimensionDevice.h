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
/// \file ForceDimensionDevice.h
/// \brief Header file for ForceDimensionDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FORCEDIMENSIONDEVICE_H__
#define __FORCEDIMENSIONDEVICE_H__

#include <H3D/H3DHapticsDevice.h>
#include <H3D/MFString.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ForceDimensionDevice
  /// \brief A ForceDimensionDevice is a node for handling communication
  /// with a haptics device from ForceDimension, such as the Omega and
  /// Delta haptics devices.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ForceDimensionDevice.x3d">ForceDimensionDevice.x3d</a>
  ///     ( <a href="examples/ForceDimensionDevice.x3d.html">Source</a> )
  class H3DAPI_API ForceDimensionDevice: public H3DHapticsDevice {
  public:

    /// Reset specializes SFBool to go into reset mode when a true event
    /// is received.
    class Reset: public OnNewValueSField< SFBool > {
      virtual void onNewValue( const bool &v );
    };

    /// WaitReset specializes SFBool to go into wait on reset mode when
    /// a true event is received.
    class WaitReset: public OnNewValueSField< SFBool > {
      virtual void onNewValue( const bool &v );
    };

    /// GravityComp specializes SFBool to toggle gravity compensation 
    /// on and off.
    class GravityComp: public OnValueChangeSField< SFBool > {
      virtual void onValueChange( const bool &v );
    };

    /// EffectorMass specializes SFFloat to change the end effector 
    /// mass at change of value.
    class EffectorMass: public OnValueChangeSField< SFFloat > {
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Brakes specializes SFBool to toggle brakes
    /// on and off.
    class Brakes: public OnValueChangeSField< SFBool > {
      virtual void onValueChange( const bool &v );
    };

    /// Constructor.
    ForceDimensionDevice( 
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
            Inst< GravityComp        > _useGravityCompensation = 0,
            Inst< Reset              > _reset                  = 0,
            Inst< WaitReset          > _waitForReset           = 0,
            Inst< EffectorMass       > _endEffectorMass        = 0,
            Inst< Brakes             > _useBrakes              = 0,
            Inst< SFInt32            > _deviceType             = 0 );
    
    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual ErrorCode initDevice();

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual ErrorCode releaseDevice();

    /// Enable/disable gravity compensation. A value of true enables it.
    /// When gravity compensation is enabled, the weights of the arms and of
    /// the end-effector are taken into account and a vertical force is 
    /// dynamically applied to the end-effector on top of the user command.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > useGravityCompensation;

    /// When a true event is received, the device is put into RESET mode.
    /// In this mode, the user is expected to put the device end-effector
    /// at its rest position. This is how the device performs its calibration. 
    /// 
    /// <b>Access type:</b> inputOnly \n   
    auto_ptr< SFBool > reset;

    /// When a true event is received, the device is put into RESET mode 
    /// and wait for the user to calibrate the device.  Optionally, a 
    /// timeout can be defined after which the call returns even if
    /// calibration has not occured.
    /// 
    /// <b>Access type:</b> inputOnly \n   
    auto_ptr< SFBool > waitForReset;

    /// Set the end effector mass used in gravity compensation in order
    /// to provide accurate gravity compensation when custom-made or 
    /// modified end-effectors are used.
    /// 
    /// <b>Access type:</b> inputOnly \n   
    auto_ptr< SFFloat > endEffectorMass;

    /// Enable/disable the device electromagnetic brakes. If enabled
    /// the device motor circuits are shortcut to produce electromagnetic
    /// viscosity. The viscosity is sufficient to prevent the device from
    /// falling too hard onto if forces are disabled abruptly, either by
    /// pressing the force button or by action of a safety feature.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > useBrakes;

    /// Contains the device type of the Force Dimension device.
    /// Possible values are:
    /// - DHD_DEVICE_3DOF - the Delta Haptic Device 3DOF
    /// - DHD_DEVICE_6DOF - the Delta Haptic Device 6DOF
    /// - DHD_DEVICE_OMEGA - the OMEGA Haptic Device
    /// - DHD_DEVICE_OMEGA3, DHD_DEVICE_OMEGA33, DHD_DEVICE_OMEGA331 - the 
    /// second generation OMEGA.X Haptic Devices
    /// - DHD_DEVICE_CONTROLLER - the Force Dimension stand-alone USB 2.0
    ///  controller (DHD_DEVICE_CONTROLLER) 
    /// - DHD_DEVICE_SIMULATOR - the Force Dimension haptic device
    /// simulator
    /// - DHD_DEVICE_CUSTOM - Unknown devices.
    /// - -1, device not initialized
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFInt32 > deviceType;

    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif
