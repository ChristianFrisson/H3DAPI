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
/// \file H3DThreadedHapticsDevice.h
/// \brief Header file for H3DThreadedHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DTHREADEDHAPTICSDEVICE_H__
#define __H3DTHREADEDHAPTICSDEVICE_H__

#include "H3DHapticsDevice.h"
#include "ThreadSafeFields.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DThreadedHapticsDevice
  /// \brief The H3DThreadedHapticsDevice provides threading and interface for 
  /// easily add support for new haptics devices. 
  /// Since it does not use OpenHaptics
  /// no geometry surface rendering will be supported by haptics devices that inherit
  /// from this class. However all force effects will work and hence e.g. VHTK will
  /// run. A thread is set up to run at 1000 Hz and rendering of force effects is
  /// done. What is needed by subclasses of this class is to implement the five virtual
  /// functions getPosition(), getVelocity(), getOrientation(), 
  /// sendForce( const Vec3f &f ) and sendTorque( const Vec3f &f ). 
  ///
  /// \par Internal routes:
  /// \dotfile H3DHapticsDevice.dot
  class H3DAPI_API H3DThreadedHapticsDevice: public H3DHapticsDevice {
  public:

    /// Constructor.
    H3DThreadedHapticsDevice( Inst< SFVec3f         > _devicePosition         = 0,
                              Inst< SFRotation      > _deviceOrientation      = 0,
                              Inst< TrackerPosition > _trackerPosition        = 0,
                              Inst< TrackerOrientation > _trackerOrientation  = 0,
                              Inst< PosCalibration  > _positionCalibration    = 0,
                              Inst< OrnCalibration  > _orientationCalibration = 0,
                              Inst< SFVec3f         > _proxyPosition          = 0,
                              Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
                              Inst< SFFloat         > _proxyWeighting         = 0,
                              Inst< SFBool          > _main_button            = 0,
                              Inst< ThreadSafeSField< SFVec3f > > _force      = 0,
                              Inst< ThreadSafeSField< SFVec3f > > _torque     = 0,
                              Inst< SFInt32         > _inputDOF               = 0,
                              Inst< SFInt32         > _outputDOF              = 0,
                              Inst< SFInt32         > _hapticsRate            = 0,
                              Inst< SFNode          > _stylus                 = 0,
                              Inst< SFBool          > _initialized            = 0 );

    /// Destructor. Stops haptics rendering and remove callback functions.
    virtual ~H3DThreadedHapticsDevice() {
      disableDevice();
    }

    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual void initDevice();

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual void disableDevice();

    /// Perform haptic rendering for the given HapticShape instances. 
    /// This is not supported on a H3DThreadedHapticsDevice, only 
    /// HLHapticDevice can do this.
    /// \param objects The haptic shapes to render.
    ///
    virtual void renderShapes( const HapticShapeVector &shapes ) {};

    /// Perform haptic rendering for the given HapticForceEffect instances. 
    /// HapticForceEffect objects that are to be be rendered haptically must
    /// be rendered with this function each scenegraph loop.
    /// \param objects The haptic objects to render.
    ///
    virtual void renderEffects( const HapticEffectVector &effects );
    
    /// Node database entry
    static H3DNodeDatabase database;

    /// Returns the force effects to render in the realtime loop.
    /// Should not be called from the scenegraph loop.
    inline HapticEffectVector &getCurrentForceEffects() {
      return current_force_effects;
    }

    /// Returns the force effects that was rendered during last scene graph loop.
    /// Should not be called from the scenegraph loop.
    inline HapticEffectVector &getLastForceEffects() {
      return last_force_effects;
    }

    /// This function is used to transfer device values, such as position, 
    /// button status etc from the realtime loop to the fields of H3DHapticsDevice,
    /// and possible vice versa.
    virtual void updateDeviceValues();

  protected:
    /// Get the position of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual Vec3f getPosition() = 0;

    /// Get the velocity of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual Vec3f getVelocity() = 0;

    /// Get the orientation of the haptics device. Only to be called in the 
    /// haptics loop.
    virtual Rotation getOrientation() = 0;

    /// Send the force to render on the haptics device. Only to be called in the 
    /// haptics loop.
    virtual void sendForce( const Vec3f &f ) = 0;

    /// Send the torque to render on the haptics device. Only to be called in the 
    /// haptics loop.
    virtual void sendTorque( const Vec3f &f ) = 0;

    // the force effects that are currently rendered in the realtime loop.
    // Should not be changed directly from the scenegraph loop but instead
    // use the renderEffects function to set the effects.
    HapticEffectVector current_force_effects;

    // the force effects that was current in the last scene graph loop.
    HapticEffectVector last_force_effects;

    // the shapes that are currently being rendered in the realtime loop.
    HapticShapeVector current_shapes;

    unsigned int nr_haptics_loops;

    // The time of the last call to updateDeviceValues.
    TimeStamp last_update_values;

    /// The time for the last call to the changeForceEffects callback 
    /// function.
    TimeStamp last_effect_change;
    
    /// The time between the previous two calls to changeForceEffects.
    TimeStamp last_loop_time;

    /// Callback function to render force effects.
    static Thread::CallbackCode forceEffectCallback( void *data );

    /// Callcack function to transfer the force effect vector to the 
    /// haptics loop.
    static Thread::CallbackCode changeForceEffects( void *_data ); 
  };
}

#endif
