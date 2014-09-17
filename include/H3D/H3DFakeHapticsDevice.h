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
/// \file H3DFakeHapticsDevice.h
/// \brief Header file for H3DFakeHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFAKEHAPTICSDEVICE_H__
#define __H3DFAKEHAPTICSDEVICE_H__

#include "H3D/H3DHapticsDevice.h"
#include "H3D/ThreadSafeFields.h"

#include <HAPI/HAPIHapticsDevice.h>

namespace H3D {
  /// \ingroup AbstractNodes  
  /// \class H3DFakeHapticsDevice
  /// \brief H3DFakeHapticsDevice is a base class for making fake haptics
  /// devices, i.e. haptics devices that are controlled with e.g keyboard or
  /// mouse.
  /// The set_devicePosition, set_deviceOrientation and set_mainButton fields
  /// are used to control the values of the haptics device. 
  class H3DAPI_API H3DFakeHapticsDevice: public H3DHapticsDevice {
  public:
    class H3DAPI_API FakeHapticsDevice: public HAPI::HAPIHapticsDevice {
    public:
      /// Implementation of updateDeviceValues using the set_... fields
      /// to specify values.
      virtual void updateDeviceValues( DeviceValues &dv, HAPI::HAPITime dt );

      /// Output is ignored on a fake haptics device.
      virtual void sendOutput( DeviceOutput &dv,
             HAPI::HAPITime dt ) {}

      /// Implementation of initHapticsDevice.
      virtual bool initHapticsDevice( int _thread_frequency = 1000 ) {
        return true;
      }

      /// Releases all resources allocated in initHapticsDevice. 
      virtual bool releaseHapticsDevice() { return true; }
    protected:
      H3DFakeHapticsDevice *owner;
    
  friend class H3DFakeHapticsDevice;
    };

    /// Constructor.
    H3DFakeHapticsDevice( Inst< SFVec3f         > _devicePosition         = 0,
        Inst< SFRotation      > _deviceOrientation      = 0,
        Inst< TrackerPosition > _trackerPosition        = 0,
        Inst< TrackerOrientation > _trackerOrientation  = 0,
        Inst< PosCalibration  > _positionCalibration    = 0,
        Inst< OrnCalibration  > _orientationCalibration = 0,
        Inst< SFVec3f         > _proxyPosition          = 0,
        Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
        Inst< SFFloat         > _proxyWeighting         = 0,
        Inst< SFBool          > _mainButton             = 0,
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
        Inst< ThreadSafeSField< SFBool > > _set_mainButton         = 0 );

    /// Destructor.
    ~H3DFakeHapticsDevice() {
      releaseDevice();
      hapi_device.reset( NULL );
    }

    /// Node database entry
    static H3DNodeDatabase database;

    /// The set_devicePosition field can be used to set the 
    /// devicePosition field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< SFVec3f > set_devicePosition;

    /// The set_deviceOrientation field can be used to set the 
    /// deviceOrientation field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< SFRotation > set_deviceOrientation;

    /// The set_mainButton field can be used to set the 
    /// mainButton field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< SFBool > set_mainButton;
  };
}

#endif
