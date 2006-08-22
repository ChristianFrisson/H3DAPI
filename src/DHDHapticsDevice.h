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
/// \file DHDHapticsDevice.h
/// \brief Header file for DHDHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DHDHAPTICSDEVICE_H__
#define __DHDHAPTICSDEVICE_H__

#include "H3DThreadedHapticsDevice.h"
#include "MFString.h"

#ifdef HAVE_DHDAPI
#include <dhdc.h>

#ifdef _MSC_VER
#pragma comment( lib, "dhdms.lib" )
#endif

#endif

namespace H3D {

  /// \ingroup H3DNodes
  /// \class DHDHapticsDevice
  /// \brief A DHDHapticsDevice is a node for handling communication
  /// with a haptics device from ForceDimension, such as the Omega and
  /// Delta haptics devices.
  ///
  /// Haptics rendering of geometry surfaces will not work with this 
  /// haptics device, but force effects will.
  class H3DAPI_API DHDHapticsDevice: public H3DThreadedHapticsDevice {
  public:

    /// Constructor.
    DHDHapticsDevice( 
            Inst< ThreadSafeSField< SFVec3f > > _devicePosition = 0,
            Inst< ThreadSafeSField< SFRotation > > _deviceOrientation   = 0,
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
            Inst< SFBool          > _initialized            = 0
);
    
    /// Destructor. Stops haptics rendering and remove callback functions.
    virtual ~DHDHapticsDevice() {
      disableDevice();
      if( thread ) {
        delete thread;
        thread = NULL;
      }
    }

    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual void initDevice();

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual void disableDevice();

    /// Node database entry
    static H3DNodeDatabase database;

  protected:
    /// Get the position of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual Vec3f getPosition();

    /// Get the velocity of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual Vec3f getVelocity();

    /// Get the orientation of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual Rotation getOrientation();

    /// Get the button status of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual bool getButtonStatus();

    /// Send the force to render on the haptics device. Only to be called in 
    /// the haptics loop.
    virtual void sendForce( const Vec3f &f );

    /// Send the torque to render on the haptics device. Only to be called
    /// in the  haptics loop.
    virtual void sendTorque( const Vec3f &f );


    int device_id;
  };
}

#endif
