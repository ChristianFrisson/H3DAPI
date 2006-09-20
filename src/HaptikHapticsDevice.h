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
/// \file HaptikHapticsDevice.h
/// \brief Header file for HaptikHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HAPTIKHAPTICSDEVICE_H__
#define __HAPTIKHAPTICSDEVICE_H__

#include "H3DThreadedHapticsDevice.h"
#include "MFString.h"
#ifdef HAVE_HAPTIK
#include <RSLib/Haptik.hpp>
#endif 
namespace H3D {

  /// \ingroup H3DNodes
  /// \class HaptikHapticsDevice
  /// \brief The HaptikHapticsDevice uses the Haptik library
  /// (www.haptiklibrary.org) to access haptics devices.
  /// 
  /// The deviceName, modelName and manufacturer fields contain a string
  /// for each device available through the Haptik library interface, 
  /// specifying the name, model and manufacturer of the device.
  /// 
  /// The selectedDevice field is an SFInt32 specifying which device is 
  /// currently in use. It is an index into the deviceName, modelName and
  /// manufacturer fields. If it is -1 then no haptics device is currently
  /// used.
  ///
  /// The set_selectedDevice and preferredDeviceType fields lets you decide
  /// which of the available haptics devices to use. The set_selectedDevice
  /// lets you choose the index of the haptics device you want to use. If this
  /// is set to -1, then the preferredDeviceType field is used to specify which
  /// kind of device you want to use. The first device available of the 
  /// selected type will be used. Valid values for preferredDeviceType are:
  /// - "DEFAULT_DEVICE" - use any available device
  /// - "MOUSE_SPECTRE" - Mouse Spectre device (emulating haptics device with 
  /// mouse)
  /// - "BETAMAX_PLAYER" - Betamax Player software device.
  /// - "BETAMAX_RECORDER" - Betamax Recorder software device.
  /// - "PHANTOM_DESKTOP" - PHANToM Desktop device.
  /// - "PHANTOM_PREMIUM" - PHANToM Premium device.
  /// - "PHANTOM_OMNI" - PHANToM Omni device.
  /// - "DELTA" - Delta device.
  /// - "OMEGA" - Omega device.
  /// - "FREEDOM6S" - Freedom6S device.
  /// - "CUBIC" - Cubic device.
  /// 
  /// \par Internal routes:
  /// \dotfile HaptikHapticsDevice.dot
  class H3DAPI_API HaptikHapticsDevice: public H3DThreadedHapticsDevice {
  public:

    /// The SelectDevice field changes the haptics device to use depending
    /// on the values of set_selectedDevice and preferredDeviceType fields.
    ///
    /// routes_in[0] is the set_selectedDevice field
    /// routes_in[1] is the preferredDeviceType field.
    class H3DAPI_API SelectDevice: 
      public TypedField< PeriodicUpdate< SFInt32 >,
    Types< SFInt32, SFString > > {
      virtual void update();
    };

    /// Constructor.
    HaptikHapticsDevice( 
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
            Inst< SFBool          > _initialized            = 0,
            Inst< MFString        > _deviceName             = 0,
            Inst< MFString        > _modelName              = 0,     
            Inst< MFString        > _manufacturer           = 0,
            Inst< SelectDevice    > _selectedDevice         = 0,
            Inst< SFInt32         > _set_selectedDevice     = 0,
            Inst< SFString        > _preferredDeviceType    = 0 );
    
    /// Destructor. Stops haptics rendering and remove callback functions.
    virtual ~HaptikHapticsDevice() {
      disableDevice();
      if( thread ) {
        delete thread;
        thread = NULL;
      }
      
#ifdef HAVE_HAPTIK
      if( haptik_device ) {
        RSLib::IHaptikDeviceInterface t = haptik_device;
        haptik_device = NULL;
        t->Release();
      }
#endif
    }

    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual void initDevice();

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual void disableDevice();

    /// The deviceName field contains the name of each haptics device that is 
    /// available.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFString > deviceName;

    /// The modelName field contains the model name of each haptics device 
    /// that is available.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFString > modelName;

    /// The manufacturer field contains the name of the manufacturer of each
    /// haptics device that is available.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFString > manufacturer;

    /// The selectedDevice field contains the index of the haptics device
    /// that is currently in use. -1 means that no haptics device is used.
    ///
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile HaptikHapticsDevice_selectedDevice.dot
    auto_ptr< SelectDevice > selectedDevice;

    /// The set_selectedDevice field lets you choose the index of the haptics 
    /// device you want to use. If this is set to -1, then the 
    /// preferredDeviceType field is used to specify which kind of device you
    /// want to use.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> -1
    /// 
    /// \dotfile HaptikHapticsDevice_set_selectedDevice.dot
    auto_ptr< SFInt32  > set_selectedDevice;

    /// The preferredDeviceType field will be used if the set_selectedDevice field
    /// is -1 to specify which kind of device you want to use. The first device 
    /// available of the  selected type will be used . Valid values for 
    /// preferredDeviceType are:
    /// - "DEFAULT_DEVICE" - use any available device
    /// - "MOUSE_SPECTRE" - Mouse Spectre device (emulating haptics device with 
    /// mouse)
    /// - "BETAMAX_PLAYER" - Betamax Player software device.
    /// - "BETAMAX_RECORDER" - Betamax Recorder software device.
    /// - "PHANTOM_DESKTOP" - PHANToM Desktop device.
    /// - "PHANTOM_PREMIUM" - PHANToM Premium device.
    /// - "PHANTOM_OMNI" - PHANToM Omni device.
    /// - "DELTA" - Delta device.
    /// - "OMEGA" - Omega device.
    /// - "FREEDOM6S" - Freedom6S device.
    /// - "CUBIC" - Cubic device.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "DEFAULT_DEVICE"
    /// 
    /// \dotfile HaptikHapticsDevice_preferredDeviceType.dot
    auto_ptr< SFString > preferredDeviceType;

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

    /// Send the force to render on the haptics device. Only to be called in the 
    /// haptics loop.
    virtual void sendForce( const Vec3f &f );

    /// Send the torque to render on the haptics device. Only to be called in the 
    /// haptics loop.
    virtual void sendTorque( const Vec3f &f );

    /// Change the used Haptik device to the one with the given id.
    virtual void changeHaptikDevice( unsigned int new_id );

#ifdef HAVE_HAPTIK
    RSLib::Haptik haptik;
    /// The device currently in use.
    RSLib::IHaptikDeviceInterface haptik_device;
    Vec3f last_force;
    Vec3f last_torque;
#endif
  };
}

#endif
