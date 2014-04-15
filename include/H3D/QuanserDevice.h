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
/// \file QuanserDevice.h
/// \brief Header file for QuanserDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __QUANSERDEVICE_H__
#define __QUANSERDEVICE_H__

#include <H3D/H3DHapticsDevice.h>
#include <H3D/MFString.h>
#include <H3D/SFString.h>
#include <H3D/SFDouble.h>
#include <H3D/MFVec3f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class QuanserDevice
  /// \brief A QuanserDevice is a node for handling communication
  /// with a haptics device from Quanser, such as the 2-DOF Pantograph. NOTE
  /// that this class is extremely untested.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/QuanserDevice.x3d">QuanserDevice.x3d</a>
  ///     ( <a href="examples/QuanserDevice.x3d.html">Source</a> )
  class H3DAPI_API QuanserDevice: public H3DHapticsDevice {
  public:

    /// Constructor.
    QuanserDevice( 
            Inst< SFVec3f            > _devicePosition         = 0,
            Inst< SFRotation         > _deviceOrientation      = 0,
            Inst< TrackerPosition    > _trackerPosition        = 0,
            Inst< TrackerOrientation > _trackerOrientation     = 0,
            Inst< PosCalibration     > _positionCalibration    = 0,
            Inst< OrnCalibration     > _orientationCalibration = 0,
            Inst< SFVec3f            > _proxyPosition          = 0,
            Inst< WeightedProxy      > _weightedProxyPosition  = 0,     
            Inst< SFFloat            > _proxyWeighting         = 0,
            Inst< SFBool             > _mainButton            = 0,
            Inst< SFBool             > _secondaryButton       = 0,
            Inst< SFInt32            > _buttons                = 0,
            Inst< SFVec3f            > _force                  = 0,
            Inst< SFVec3f            > _torque                 = 0,
            Inst< SFInt32            > _inputDOF               = 0,
            Inst< SFInt32            > _outputDOF              = 0,
            Inst< SFInt32            > _hapticsRate            = 0,
            Inst< SFInt32            > _desiredHapticsRate     = 0,
            Inst< SFNode             > _stylus                 = 0,
            Inst< SFString           > _uri                    = 0 );
    
    /// Creates a QuanserHapticsDevice in the hapi_device
    virtual void initialize();

    /// An uri to some quanser file or something. The one that implemented
    /// this should comment it. Lazy bugger.
    /// \todo Comment this properly.
    ///
    /// <b>Access type:</b> initalizeOnly \n
    auto_ptr< SFString > uri;

    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif
