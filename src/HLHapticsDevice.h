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
/// \file HLHapticsDevice.h
/// \brief Header file for HLHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HLHAPTICSDEVICE_H__
#define __HLHAPTICSDEVICE_H__

#include "H3DHapticsDevice.h"
#include "DeviceLog.h"
#include "SFString.h" 
#ifdef HAVE_OPENHAPTICS
#include <HL/hl.h>
#endif
#include "GL/glew.h"
namespace H3D {
  /// \ingroup H3DNodes
  /// \class HLHapticsDevice
  /// HLHapticsDevice is a haptics device that uses the HLAPI from SensAble
  /// to do the haptic rendering.
  /// 
  /// \par Internal routes:
  /// \dotfile HLHapticsDevice.dot
  class H3DAPI_API HLHapticsDevice : public H3DHapticsDevice {
  public:
    typedef TypedSFNode< DeviceLog > SFDeviceLog;

    H3D_VALUE_EXCEPTION( string, CouldNotInitHapticsDevice );
    
    /// Construtor.
    HLHapticsDevice( Inst< SFVec3f         > _devicePosition         = 0,
                     Inst< SFRotation      > _deviceOrientation      = 0,
                     Inst< TrackerPosition > _trackerPosition        = 0,
                     Inst< TrackerOrientation > _trackerOrientation  = 0,
                     Inst< PosCalibration  > _positionCalibration    = 0,
                     Inst< OrnCalibration  > _orientationCalibration = 0,
                     Inst< SFVec3f         > _proxyPosition          = 0,
                     Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
                     Inst< SFFloat         > _proxyWeighting         = 0,
                     Inst< SFBool          > _mainButton             = 0,
                     Inst< SFVec3f         > _force                  = 0,
                     Inst< SFVec3f         > _torque                 = 0,
                     Inst< SFInt32         > _inputDOF               = 0,
                     Inst< SFInt32         > _outputDOF              = 0,
                     Inst< SFInt32         > _hapticsRate            = 0,
                     Inst< SFNode          > _stylus                 = 0,
                     Inst< SFBool          > _initialized            = 0,
                     Inst< SFString        > _deviceName             = 0,
                     Inst< SFBool          > _secondaryButton        = 0,
                     Inst< SFDeviceLog     > _deviceLog              = 0 ):
      H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
                        _trackerOrientation, _positionCalibration, 
                        _orientationCalibration, _proxyPosition,
                        _weightedProxyPosition, _proxyWeighting,
                        _mainButton, _force, _torque,
                        _inputDOF, _outputDOF, _hapticsRate, _stylus, _initialized ),
      deviceName( _deviceName ),
      secondaryButton( _secondaryButton ),
      deviceLog( _deviceLog )
#ifdef HAVE_OPENHAPTICS
     ,last_effect_change( TimeStamp() ),
      last_loop_time( (H3DFloat) 0.1 ), 
      log( NULL ),
      device_handle( 0 ),
      haptic_context( NULL )
#endif
{

      type_name = "HLHapticsDevice";
      database.initFields( this );
      deviceName->setValue( "Default PHANToM" );
      secondaryButton->setValue( false, id );
    }

    /// Destructor.
    virtual ~HLHapticsDevice() {
      if( initialized->getValue() )
        disableDevice();
    }

#ifdef HAVE_OPENHAPTICS

    // Callback function for setting the button fields when button events
    // occurs.
    static void HLCALLBACK hlButtonCallback( HLenum event,
                                             HLuint object,
                                             HLenum thread,
                                             HLcache *cache,
                                             void *data );


    /// Starts a new haptic frame.
    virtual void preRender();

    /// Ends the haptic frame.
    virtual void postRender();

    /// Does all the initialization needed for the device before starting to 
    /// use it.
    virtual void initDevice();

	virtual void initHLLayer();

    /// Reset the device.
    virtual void resetDevice() {}

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual void disableDevice();

    /// This function is used to transfer device values, such as position, 
    /// button status etc from the realtime loop to the fields of HLHapticsDevice,
    /// and possible vice versa.
    virtual void updateDeviceValues();

    /// Uses the HLAPI to render the objects. The objects given must be a 
    /// subclass to HLObject in order to be rendered.
    virtual void renderShapes( const HapticShapeVector &shapes );

    /// Uses HLAPI to render the ForceEffects. 
    /// \param objects The force effects to render.
    ///
    virtual void renderEffects( const HapticEffectVector &effects );
#endif
    /// The name of the device.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "Default PHANToM" \n
    /// 
    /// \dotfile HLHapticsDevice_deviceName.dot
    auto_ptr< SFString > deviceName;

    /// The status of the secondary button (if such a button exists). true
    /// means that the button is pressed
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile HLHapticsDevice_secondaryButton.dot
    auto_ptr< SFBool > secondaryButton;

    /// Logging Node that can be used to log device position and
    /// orientation information to a binary file.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> NONE \n
    /// 
    /// \dotfile HLTrackedHapticsDevice_deviceName.dot
    auto_ptr< SFDeviceLog > deviceLog;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
#ifdef HAVE_OPENHAPTICS
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

    /// Returns the HD device handle for this device.
    inline HHD getDeviceHandle() { 
      return device_handle;
    }

    /// Returns the haptic context for this device. Before
    /// rendering anything on this device this context must have been made active
    inline HHLRC getHapticContext() {
      return haptic_context;
    }
    
    /// The time for the last call to the changeForceEffects callback 
    /// function.
    TimeStamp last_effect_change;
    
    /// The time between the previous two calls to changeForceEffects.
    TimeStamp last_loop_time;

    /// Realtime pointer to deviceLog
    AutoRef< DeviceLog > log;

  protected:
    /// The device handle used in the HLAPI.
    HHD device_handle;
    
    /// The HLAPI haptic context used by this device.
    HHLRC haptic_context;

    /// Handle for the callback for rendering ForceEffects.  
    vector< HDCallbackCode > hd_handles; 
    
  private:
    // the force effects that are currently rendered in the realtime loop.
    // Should not be changed directly from the scenegraph loop but instead
    // use the renderEffects function to set the effects.
    HapticEffectVector current_force_effects;

    // the force effects that was current in the last scene graph loop.
    HapticEffectVector last_force_effects;

    // the shapes that are currently being rendered in the realtime loop.
    HapticShapeVector current_shapes;
#endif
  };
}

#endif
