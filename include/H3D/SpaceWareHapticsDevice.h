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
/// \file SpaceWareHapticsDevice.h
/// \brief Header file for SpaceWareHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPACEWAREHAPTICSDEVICE_H__
#define __SPACEWAREHAPTICSDEVICE_H__

#include "H3D/SpaceWareSensor.h"
#include "H3D/H3DFakeHapticsDevice.h"

namespace H3D {

  /// \ingroup H3DNodes
  /// \class SpaceWareHapticsDevice
  /// \brief A fake haptics device where the position and orientation is
  /// controlled via a SpaceWare device such as the SpaceMouse or SpaceTraveller.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SpaceWareHapticsDevice.x3d">SpaceWareHapticsDevice.x3d</a>
  ///     ( <a href="examples/SpaceWareHapticsDevice.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SpaceWareHapticsDevice.dot
  class H3DAPI_API SpaceWareHapticsDevice: public H3DFakeHapticsDevice {
  public:
    /// SetMainButton is set to true if any of the buttons on the SpaceWare
    /// device is pressed.
    ///
    /// routes_in[0] is the buttons field of the SpaceWareSensor.
    class SetMainButton: public TypedField< ThreadSafeSField< SFBool >,
                         Types< SFInt32 > > {
      virtual void update();
    };

    /// Constructor.
    SpaceWareHapticsDevice( 
         Inst< SFVec3f         > _devicePosition         = 0,
        Inst< SFRotation      > _deviceOrientation      = 0,
        Inst< TrackerPosition > _trackerPosition        = 0,
        Inst< TrackerOrientation > _trackerOrientation  = 0,
        Inst< PosCalibration  > _positionCalibration    = 0,
        Inst< OrnCalibration  > _orientationCalibration = 0,
        Inst< SFVec3f         > _proxyPosition          = 0,
        Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
        Inst< SFFloat         > _proxyWeighting         = 0,
        Inst< SFBool          > _mainButton            = 0,
        Inst< SFBool          > _secondaryButton       = 0,
        Inst< SFInt32         > _buttons                = 0,
        Inst< SFVec3f         > _force                  = 0,
        Inst< SFVec3f         > _torque                 = 0,
        Inst< SFInt32         > _inputDOF               = 0,
        Inst< SFInt32         > _outputDOF              = 0,
        Inst< SFInt32         > _hapticsRate            = 0,
        Inst< SFInt32         > _desiredHapticsRate     = 0,
        Inst< SFNode          > _stylus                 = 0,
        Inst< SFHapticsRendererNode > _hapticsRenderer  = 0,
        Inst< MFVec3f         > _proxyPositions         = 0,
        Inst< SFBool          > _followViewpoint        = 0,
        Inst< ThreadSafeSField< SFVec3f > > _set_devicePosition     = 0,
        Inst< ThreadSafeSField< SFRotation > > _set_deviceOrientation  = 0,
         Inst< SetMainButton > _set_mainButton         = 0,
         Inst< SFFloat         > _posSensitivity         = 0,
         Inst< SFFloat         > _ornSensitivity         = 0);

    /// updateDeviceValues is specialized to update the values of the
    /// SpaceWareSensor.
    virtual void updateDeviceValues() {
      H3DFakeHapticsDevice::updateDeviceValues();
    }
    
    /// Node database entry
    static H3DNodeDatabase database;
    
    /// posSensitivity defines how fast the movement of the haptics
    /// device should be when using the SpaceWare device.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.0001 \n
    /// 
    /// \dotfile SpaceWareHapticsDevice_posSensitivity.dot
    auto_ptr< SFFloat > posSensitivity;

    /// ornSensitivity defines how fast the orientation of the haptics
    /// device should move when using the SpaceWare device.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.05 \n
    /// 
    /// \dotfile SpaceWareHapticsDevice_ornSensitivity.dot
    auto_ptr< SFFloat > ornSensitivity;

  protected:
    /// The SpaceWareSensor node used to control the haptics device.
    AutoRef< SpaceWareSensor > spaceware_sensor;
  };
}

#endif
