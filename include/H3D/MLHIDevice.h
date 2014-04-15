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
/// \file MLHIDevice.h
/// \brief Header file for MLHIDevice.
//
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MLHIDEVICE_H__
#define __MLHIDEVICE_H__

#include <H3D/H3DHapticsDevice.h>
#include <H3D/SFString.h>
namespace H3D {

  /// \ingroup H3DNodes
  /// \class  MLHIDevice
  /// \brief A MLHIDevice is a node for handling communication
  /// with MLHI devices.
  /// 
  /// That is a Maglev haptics device.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MLHIDevice.x3d">MLHIDevice.x3d</a>
  ///     ( <a href="examples/MLHIDevice.x3d.html">Source</a> )
  /// \note THIS NODE IS COMPLETELY UNTESTED.
  class H3DAPI_API MLHIDevice: public H3DHapticsDevice {
  public:

    /// Constructor.
    MLHIDevice( Inst< SFVec3f            > _devicePosition         = 0,
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
                Inst< SFHapticsRendererNode > _hapticsRenderer     = 0,
                Inst< MFVec3f            > _proxyPositions         = 0,
                Inst< SFBool             > _followViewpoint        = 0,
                Inst< SFString           > _serverIPAddress        = 0 );

    /// Creates a MLHIDevice in the hapi_device with IP address from server is serverIPAddress
    virtual void initialize();

    /// The IP address of the device in use
    auto_ptr< SFString > serverIPAddress;

    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif