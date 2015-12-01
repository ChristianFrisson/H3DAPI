//////////////////////////////////////////////////////////////////////////////
//    Copyright 2011-2013, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file ClutchedDevice.h
/// \brief Header file for ClutchedDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CLUTCHEDDEVICE_H__
#define __CLUTCHEDDEVICE_H__

#include <H3D/H3DHapticsDevice.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ClutchedDevice
  /// \brief ClutchedDevice is a wrapper for a H3DHapticsDevice which adds clutching
  /// functionality. 
  ///
  /// The clucthing can be useful when the workspace and calibration of the haptic device
  /// does not allow comfortable control.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ClutchedDevice.x3d">ClutchedDevice.x3d</a>
  ///     ( <a href="examples/ClutchedDevice.x3d.html">Source</a> )
  class H3DAPI_API ClutchedDevice: public H3DHapticsDevice {
  public:
    typedef TypedSFNode<H3DHapticsDevice> SFHapticsDevice;

    /// Enables/Disbles the clutching.
    class EnableClutch: public AutoUpdate< OnValueChangeSField< SFBool > > {
      virtual void onValueChange( const bool &value );
    };

    /// If True, resets the orientation offset due to clutching.
    class ResetClutchOrientation: public AutoUpdate< OnNewValueSField< SFBool > > {
      virtual void onNewValue( const bool &value );
    };

    /// If True, resets the position offset due to clutching.
    class ResetClutchPosition: public AutoUpdate< OnNewValueSField< SFBool > > {
      virtual void onNewValue( const bool &value );
    };

    /// Constructor.
    ClutchedDevice( 
            Inst< SFVec3f            > _devicePosition         = 0,
            Inst< SFRotation         > _deviceOrientation      = 0,
            Inst< TrackerPosition    > _trackerPosition        = 0,
            Inst< TrackerOrientation > _trackerOrientation     = 0,
            Inst< PosCalibration     > _positionCalibration    = 0,
            Inst< OrnCalibration     > _orientationCalibration = 0,
            Inst< SFVec3f            > _proxyPosition          = 0,
            Inst< WeightedProxy      > _weightedProxyPosition  = 0,     
            Inst< SFFloat            > _proxyWeighting         = 0,
            Inst< SFBool             > _mainButton             = 0,
            Inst< SFBool             > _secondaryButton        = 0,
            Inst< SFInt32            > _buttons                = 0,
            Inst< SFVec3f            > _force                  = 0,
            Inst< SFVec3f            > _torque                 = 0,
            Inst< SFInt32            > _inputDOF               = 0,
            Inst< SFInt32            > _outputDOF              = 0,
            Inst< SFInt32            > _hapticsRate            = 0,
            Inst< SFInt32            > _desiredHapticsRate     = 0,
            Inst< SFNode             > _stylus                 = 0,
            Inst< SFHapticsDevice    > _hapticsDevice          = 0,
            Inst< EnableClutch       > _enableClutch           = 0,
            Inst< ResetClutchOrientation > _resetClutchOrientation = 0,
            Inst< ResetClutchPosition > _resetClutchPosition       = 0,
            Inst< SFVec3f            > _unclutchedPosition     = 0,
            Inst< SFRotation         > _unclutchedOrientation  = 0 );

    /// Initialize the node
    virtual void initialize ();

    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual ErrorCode initDevice();

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual ErrorCode releaseDevice();

    /// Perform haptic rendering for the given HapticShape instances. 
    /// HapticShape objects that are to be be rendered haptically must be 
    /// rendered with this function each scenegraph loop. 
    /// Used in PhantomDevice to determine when to start the scheduler
    /// for OpenHaptics in order to avoid random crashes when creating
    /// hlContext.
    /// \param shapes The haptic shapes to render.
    /// \param layer The haptic layer to render them in.
    virtual void renderShapes( const HapticShapeVector &shapes, 
                               unsigned int layer = 0 );

    /// This function is used to transfer device values, such as position, 
    /// button status etc from the realtime loop to the fields of H3DHapticsDevice,
    /// and possible vice versa.
    virtual void updateDeviceValues();

    static H3DHapticsDevice* getActualHapticsDevice ( H3DHapticsDevice& d ) {
      if ( ClutchedDevice* cd= dynamic_cast<ClutchedDevice*>(&d) ) {
        return cd->hapticsDevice->getValue();
      } else {
        return &d;
      }
    }

    /// The wrapped haptics device to clutch
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    auto_ptr<SFHapticsDevice> hapticsDevice;

    /// Enable and disable the clutch
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> False \n
    auto_ptr < EnableClutch > enableClutch;

    /// Reset clutch orientation
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> False \n
    auto_ptr< ResetClutchOrientation > resetClutchOrientation;

    /// Reset clutch position
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// <b>Default value:</b> False \n
    auto_ptr< ResetClutchPosition > resetClutchPosition;

    /// The position ignoring clutching
    ///
    /// <b>Access type:</b> OutputOnly \n
    auto_ptr < SFVec3f > unclutchedPosition;

    /// The orientation ignoring clutching
    ///
    /// <b>Access type:</b> OutputOnly \n
    auto_ptr < SFRotation > unclutchedOrientation;

    /// Node database entry
    static H3DNodeDatabase database;

  };
}

#endif
