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
/// \file H3DFakeHapticsDevice.h
/// \brief Header file for H3DFakeHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFAKEHAPTICSDEVICE_H__
#define __H3DFAKEHAPTICSDEVICE_H__

#include "H3DThreadedHapticsDevice.h"
#include "ThreadSafeFields.h"

namespace H3D {
  /// \ingroup AbstractNodes  
  /// \class H3DFakeHapticsDevice
  /// \brief H3DFakeHapticsDevice is a base class for making fake haptics
  /// devices, i.e. haptics devices that are controlled with e.g keyboard or
  /// mouse.
  /// The set_devicePosition, set_deviceOrientation and set_mainButton fields
  /// are used to control the values of the haptics device. 
  ///
  /// \par Internal routes:
  /// \dotfile H3DFakeHapticsDevice.dot
  class H3DAPI_API H3DFakeHapticsDevice: public H3DThreadedHapticsDevice {
  public:
    /// Constructor.
    H3DFakeHapticsDevice( 
         Inst< ThreadSafeSField< SFVec3f > > _devicePosition         = 0,
         Inst< ThreadSafeSField< SFRotation > > _deviceOrientation      = 0,
         Inst< TrackerPosition > _trackerPosition        = 0,
         Inst< TrackerOrientation > _trackerOrientation  = 0,
         Inst< PosCalibration  > _positionCalibration    = 0,
         Inst< OrnCalibration  > _orientationCalibration = 0,
         Inst< SFVec3f         > _proxyPosition          = 0,
         Inst< WeightedProxy   > _weightedProxyPosition  = 0,    
         Inst< SFFloat         > _proxyWeighting         = 0,
         Inst< ThreadSafeSField< SFBool > > _main_button = 0,
         Inst< ThreadSafeSField< SFVec3f > > _force      = 0,
         Inst< ThreadSafeSField< SFVec3f > > _torque     = 0,
         Inst< SFInt32         > _inputDOF               = 0,
         Inst< SFInt32         > _outputDOF              = 0,
         Inst< SFInt32         > _hapticsRate            = 0,
         Inst< SFNode          > _stylus                 = 0,
         Inst< SFBool          > _initialized            = 0,
         Inst< SFVec3f         > _set_devicePosition     = 0,
         Inst< SFRotation      > _set_deviceOrientation  = 0,
         Inst< SFBool          > _set_mainButton         = 0 );

    /// Node database entry
    static H3DNodeDatabase database;

    /// The set_devicePosition field can be used to set the 
    /// devicePosition field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile H3DFakeHapticsDevice_set_devicePosition.dot
    auto_ptr< SFVec3f > set_devicePosition;

    /// The set_deviceOrientation field can be used to set the 
    /// deviceOrientation field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile H3DFakeHapticsDevice_set_deviceOrientation.dot
    auto_ptr< SFRotation > set_deviceOrientation;

    /// The set_mainButton field can be used to set the 
    /// mainButton field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile H3DFakeHapticsDevice_set_mainButton.dot
    auto_ptr< SFBool > set_mainButton;

  protected:
    /// Get the position of the haptics device.
    virtual Vec3f getPosition();
    /// Get the velocity of the haptics device.
    virtual Vec3f getVelocity();
    /// Get the orientation of the haptics device.
    virtual Rotation getOrientation();
    /// Get the button status of the haptics device.
    virtual bool getButtonStatus();
    /// Forces are ignored.
    virtual void sendForce( const Vec3f &f ) {}
    /// Torques are ignored.
    virtual void sendTorque( const Vec3f &f ) {};
  };
}

#endif
