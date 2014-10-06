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
/// \file PlaybackDevice.h
/// \brief Header file for PlaybackDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PLAYBACKDEVICE_H__
#define __PLAYBACKDEVICE_H__

#include <H3D/X3DUrlObject.h>
#include <H3D/H3DHapticsDevice.h>
#include <H3D/PeriodicUpdate.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class PlaybackDevice
  /// \brief A PlaybackDevice is a fake haptics device node that can play back
  ///        device values previously recorded using the DeviceLog node.
  ///
  /// This haptics device plays back device values from a file specified 
  /// by the url field. The format of the file is the same as that produced
  /// by the DeviceLog force effect. Both binary and text formats are 
  /// supported.
  ///
  /// Any combination of columns are supported, however a TIME
  /// column is required in order to play back the values at the original
  /// speed.
  ///
  /// The following columns are used to define the device's raw values:
  /// RAW_POSITION, RAW_ORIENTATION, RAW_VELOCITY, BUTTONS. Any other 
  /// columns are ignored. The calibrated device values are calculated
  /// based on the raw values in the usual way and the recorded calibrated
  /// values (if present) are ignored.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PlaybackDevice.x3d">PlaybackDevice.x3d</a>
  ///     ( <a href="examples/PlaybackDevice.x3d.html">Source</a> )
  class H3DAPI_API PlaybackDevice: 
    public X3DUrlObject,
    public H3DHapticsDevice {
  public:

    /// A field used to start and stop playback
    class H3DAPI_API OnPlay : public OnNewValueSField < AutoUpdate < SFBool > > {
      virtual void onNewValue( const bool& new_value );
    };

    /// A field used to move playback to a specified time
    class H3DAPI_API OnSeekToTime : public OnNewValueSField < AutoUpdate < SFTime > > {
      virtual void onNewValue( const H3DTime& new_value );
    };

    /// A field used to adjust the playback speed
    class H3DAPI_API OnPlaybackSpeed : public OnNewValueSField < AutoUpdate < SFFloat > > {
      virtual void onNewValue( const H3DFloat& new_value );
    };

    /// A field used to update default playback values
    class H3DAPI_API OnDefaultValuesChanged : public PeriodicUpdate < Field > {
      virtual void update ();
    };

    /// Constructor.
    PlaybackDevice( 
        Inst< MFString        >  _url                   = 0,
        Inst< SFVec3f         > _devicePosition         = 0,
        Inst< SFRotation      > _deviceOrientation      = 0,
        Inst< TrackerPosition > _trackerPosition        = 0,
        Inst< TrackerOrientation > _trackerOrientation  = 0,
        Inst< SFMatrix4f      > _positionCalibration    = 0,
        Inst< SFRotation      > _orientationCalibration = 0,
        Inst< SFVec3f         > _proxyPosition          = 0,
        Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
        Inst< SFFloat         > _proxyWeighting         = 0,
        Inst< SFBool          > _mainButton             = 0,
        Inst< SFBool          > _secondaryButton        = 0,
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
        Inst< OnPlay          > _play                   = 0,
        Inst< SFBool          > _binary                 = 0,
        Inst< OnSeekToTime    > _seekToTime             = 0,
        Inst< OnPlaybackSpeed > _playbackSpeed          = 0,
        Inst< SFTime          > _playbackTime           = 0,
        Inst< SFBool          > _playing                = 0,
        Inst< MFString        > _playbackData           = 0,
        Inst< SFVec3f         > _defaultDevicePosition  = 0,
        Inst< SFRotation      > _defaultDeviceOrientation = 0,
        Inst< SFVec3f         > _defaultDeviceVelocity  = 0,
        Inst< SFInt32         > _defaultButtons         = 0
        );

    /// Destructor
    virtual ~PlaybackDevice ();

    /// Initialize the node
    virtual void initialize ();

    /// Update the output fields
    virtual void updateDeviceValues ();

    /// Start and stop the playback. 
    ///
    /// If a value of true is received and a valid url has been
    /// specified then playback will begin. If a value of false is
    /// recieved then the playback is paused and can be resumed with
    /// a subsiquent true event.
    ///
    /// To start the playback from the begining again, set seekToTime
    /// to zero first.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    ///
    auto_ptr < OnPlay > play;

    /// If true, then the recording pointed to by url is assumed to contain
    /// binary data. Otherwise it is assumed to contain text data.
    ///
    /// The value set here will take effect the next time that a new url is 
    /// specified and playback is started.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    ///
    auto_ptr< SFBool > binary;

    /// Move playback to the specified time.
    ///
    /// The value set here will take effect the next time playback is started.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.0 \n
    ///
    auto_ptr < OnSeekToTime > seekToTime;

    /// A scaling factor to apply to the speed of playback. 
    ///
    /// A value of 1 corresponds to the original recording speed.
    /// Changing this value during playback will result in discontinuities
    /// in the playback position.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    ///
    auto_ptr < OnPlaybackSpeed > playbackSpeed;

    /// Current playback time.
    ///
    /// Indicates the current playback time in the recording.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> 0.0 \n
    ///
    auto_ptr < SFTime > playbackTime;

    /// True if the device is currently playing back values. False
    /// if idle, or playback has ended.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> false \n
    ///
    auto_ptr < SFBool > playing;

    /// The list of field names in the recording that should be played back.
    ///
    /// If the list is empty then all available columns are played back. If a field
    /// is not played back, then the default value specified using the default* fields
    /// is used instead.
    ///
    /// The TIME field is always used if present in the recording.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    ///
    /// <b>Allowed values:</b> "DEVICE_POSITION", "DEVICE_ORIENTATION", "DEVICE_VELOCITY",
    ///                        "BUTTONS"
    auto_ptr < MFString > playbackData;

    /// The default value to use for the devicePosition field if no value is
    /// played back from the recording.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f(0,0,0) \n
    ///
    auto_ptr < SFVec3f > defaultDevicePosition;

    /// The default value to use for the deviceOrientation field if no value is
    /// played back from the recording.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation(1,0,0,0) \n
    ///
    auto_ptr < SFRotation > defaultDeviceOrientation;

    /// The default value to use for the deviceVelocity field if no value is
    /// played back from the recording.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f(0,0,0) \n
    ///
    auto_ptr < SFVec3f > defaultDeviceVelocity;

    /// The default value to use for the buttons field if no value is
    /// played back from the recording.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    ///
    auto_ptr < SFInt32 > defaultButtons;

    /// Node database entry
    static H3DNodeDatabase database;

  protected:

    /// Function to update the default device values in the HAPI device
    virtual void updateDefaultValues ();

    /// A field used to detect if the url field has changed
    /// since last playback started
    auto_ptr < Field > playback_url_changed;

    /// A field used to update the default device values in the HAPI device
    auto_ptr < OnDefaultValuesChanged > default_values_changed;

    /// Store the name of the temp file used to resolve the url
    /// so that it can be removed later
    std::string tmp_filename;
  };
}

#endif
