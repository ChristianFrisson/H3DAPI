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
/// \file CyberGloveSensor.h
/// \brief Header file for CyberGloveSensor.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CYBERGLOVESENSOR_H__
#define __CYBERGLOVESENSOR_H__

// H3D includes
#include <H3D/VirtualHandGloveSensor.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class CyberGloveSensor
  /// \brief Connects to a CyberGlove through the Virtual HandSDK.
  ///
  /// A correctly installed CyberGlove is needed, the Device Manager have to
  /// be started and a default configuration has to be set. See manual for
  /// CyberGlove to accomplish that.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/HumanHand.x3d">HumanHand.x3d</a>
  ///     ( <a href="examples/HumanHand.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile CyberGloveSensor.dot
  class H3DAPI_API CyberGloveSensor : public VirtualHandGloveSensor {
  public:

    /// Constructor.
    CyberGloveSensor( Inst< OnEnable > _enabled  = 0,
                      Inst< SFNode   > _metadata = 0,
                      Inst< SFBool   > _isActive = 0,
                      Inst< SFString > _host = 0,
                      Inst< SFString > _port = 0,
                      Inst< SFInt32  > _comPort = 0,
                      Inst< SFInt32  > _hardwareSpeed = 0,
                      Inst< MFDouble > _thumb = 0,
                      Inst< MFDouble > _index = 0,
                      Inst< MFDouble > _middle = 0,
                      Inst< MFDouble > _ring = 0,
                      Inst< MFDouble > _pinky = 0 );

    /// Describes how the physical device is connected to host. -1 means that
    /// default com port should be used. If the default is not desired simply
    /// set this to the com port number. That is, if the device is connected to
    /// com8, set this field to 8.
    /// Only used when the sensor is enabled.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default Value: </b> "-1" \n
    auto_ptr< SFInt32> comPort;

    /// Describes the connection speed. Only change this if you know what you
    /// are doing.
    /// Only used when the sensor is enabled.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default Value: </b> "115200" \n
    auto_ptr< SFInt32> hardwareSpeed;

    /// Outputs the angles for the joints on the thumb.
    /// The joints are outputed in the following order:
    /// metacarpa, proximal, distal
    ///
    /// <b>Access type: </b> outputOnly \n
    auto_ptr< MFDouble> thumb;

    /// Outputs the angles for the joints on the index finger.
    /// The joints are outputed in the following order:
    /// metacarpa, proximal, distal
    ///
    /// <b>Access type: </b> outputOnly \n
    auto_ptr< MFDouble> index;

    /// Outputs the angles for the joints on the middle finger.
    /// The joints are outputed in the following order:
    /// metacarpa, proximal, distal
    ///
    /// <b>Access type: </b> outputOnly \n
    auto_ptr< MFDouble> middle;

    /// Outputs the angles for the joints on the ring finger.
    /// The joints are outputed in the following order:
    /// metacarpa, proximal, distal
    ///
    /// <b>Access type: </b> outputOnly \n
    auto_ptr< MFDouble> ring;

    /// Outputs the angles for the joints on the pinky.
    /// The joints are outputed in the following order:
    /// metacarpa, proximal, distal
    ///
    /// <b>Access type: </b> outputOnly \n
    auto_ptr< MFDouble> pinky;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
    /// Update field values.
    virtual void updateValues();

    /// Virtual function that connects the vhtGlove
    virtual bool connect();

    /// Virtual function that disconnects the vhtGlove
    virtual void disconnect();
  };
}

#endif
