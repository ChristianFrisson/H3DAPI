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
//
/// \file H3D/DeviceLog.h
/// \brief Header file for DeviceLog, scene-graph node.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DAPI_DEVICE_LOG_H__
#define __H3DAPI_DEVICE_LOG_H__

#include <H3D/H3DForceEffect.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/SFInt32.h>
#include <H3D/SFBool.h>

#include <HAPI/DeviceLog.h>

namespace H3D {

  /// \ingroup H3DNodes 
  /// \class DeviceLog
  /// \brief DeviceLog records haptic device data to a
  /// binary data file. It is essentially a ForceEffect that always calculates
  /// a zero force and logs data for the haptics device.
  ///
  /// Warning. If any of the fields of this node changes ( with the exception
  /// of metadata ) a new log will be created and possibly old data in the
  /// file(s) pointed to in the url field will be overwritten. 
  ///
  /// For information on how to read the log created by this node see comments
  /// for the logBinary field.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DeviceLog.x3d">DeviceLog.x3d</a>
  ///     ( <a href="examples/DeviceLog.x3d.html">Source</a> )
  class H3DAPI_API DeviceLog: public H3DForceEffect,
                              public X3DUrlObject {
  public:

    /// Constructor
    DeviceLog( Inst< SFNode   >  _metadata   = 0,
               Inst< MFString > _url         = 0,
               Inst< SFInt32  > _frequency   = 0,
               Inst< MFInt32  > _deviceIndex = 0,
               Inst< SFBool   > _logBinary   = 0,
               Inst< MFString > _logData     = 0 );

    /// Adds the effect to be rendered in the haptics loop.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// The log frequency.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> 100 \n
    auto_ptr< SFInt32 > frequency;

    /// If true the logging will be done to a binary file. If false
    /// The logging will be done to a ASCII-text file.
    /// When logging to a text file the first line will contain information
    /// about what will be logged encoded as strings which is one of the
    /// strings which are valid for the logData field. The ALL string will
    /// never show up.
    /// When logging to a binary file the file format will be the following.
    /// The first bytes should be read as characters into a string. When two
    /// consecutive '\\0'-characters are found the section with different
    /// strings are over. These strings correspond to the first line in a text
    /// file.
    /// When the first strings are read the data that follows will be in the
    /// order listed by the strings. In an ascii-file each line will contain
    /// one log entry for each of the strings, each number is separated by
    /// space. In the binary file the numbers will follow eachother without any
    /// separating bytes.
    /// The encoding will be as follows:
    /// TIME                - double
    /// TRACKER_POSITION    - double double double
    /// TRACKER_ORIENTATION - double double double double
    /// TRACKER_VELOCITY    - double double double
    /// DEVICE_POSITION     - double double double
    /// DEVICE_ORIENTATION  - double double double double
    /// DEVICE_VELOCITY     - double double double
    /// BUTTONS             - int
    /// FORCE               - double double double
    /// TORQUE              - double double double
    ///
    /// The data type of the containing value in the field corresponding to the
    /// value decides how to interpret the value when reading from file later.
    /// For example, TRACKER_POSITION could be put in an Vec3f type.
    /// For an example on how to read from a binary log file see
    /// HAPI::DeviceLog.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> false \n
    auto_ptr< SFBool > logBinary;

    /// Specifies what to log. The allowed values correspond to the following.
    /// ALL - All possible information will be stored.
    /// TIME - The time of logging. The first log entry counts as time 0.
    /// TRACKER_POSITION - The trackerPosition field of H3DHapticsDevice.
    /// TRACKER_ORIENTATION - The trackerOrientation field of H3DHapticsDevice.
    /// TRACKER_VELOCITY - The trackerVelocity field of H3DHapticsDevice.
    /// DEVICE_POSITION - The devicePosition field of H3DHapticsDevice.
    /// DEVICE_ORIENTATION - The deviceOrientation field of H3DHapticsDevice.
    /// DEVICE_VELOCITY - The deviceVelocity field of H3DHapticsDevice.
    /// BUTTONS - The buttons field of H3DHapticsDevice.
    /// FORCE - The force field of H3DHapticsDevice.
    /// TORQUE - The torque field of H3DHapticsDevice.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "ALL" \n
    /// <b>Allowed values:</b> "ALL", "TIME", "TRACKER_POSITION",
    ///                        "TRACKER_ORIENTATION", "TRACKER_VELOCITY"
    ///                        "DEVICE_POSITION", "DEVICE_ORIENTATION"
    ///                        "DEVICE_VELOCITY", "BUTTONS"
    ///                        "FORCE", "TORQUE"
    auto_ptr< MFString > logData;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Internal haptic force effect instance
    AutoRefVector< HAPI::DeviceLog > log_force_effect;

    /// Internal function used to create a log force effect for the
    /// haptics device of the given index.
    void createLogForceEffect( int index );

    auto_ptr< Field > updateLogForceEffect;
  };

};

#endif
