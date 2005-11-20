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
/// \file FakeHapticsDevice.h
/// \brief Header file for FakeHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FAKEHAPTICSDEVICE_H__
#define __FAKEHAPTICSDEVICE_H__

#include "H3DFakeHapticsDevice.h"
#include "ThreadSafeFields.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class FakeHapticsDevice
  /// \brief A H3DHapticsDevice node where the position, orientation
  /// and button can be controlled via the set_devicePosition, 
  /// set_deviceOrientation and set_mainButton fields.
  ///
  /// \par Internal routes:
  /// \dotfile FakeHapticsDevice.dot
  class H3DAPI_API FakeHapticsDevice: public H3DFakeHapticsDevice {
  public:

    /// Constructor.
    FakeHapticsDevice( 
         Inst< ThreadSafeSField< SFVec3f > > _devicePosition         = 0,
         Inst< ThreadSafeSField< SFRotation > > _deviceOrientation      = 0,
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
         Inst< SFVec3f         > _set_devicePosition     = 0,
         Inst< SFRotation      > _set_deviceOrientation  = 0,
         Inst< SFBool          > _set_mainButton         = 0 );

    /// Node database entry
    static H3DNodeDatabase database;
  };
}

#endif
