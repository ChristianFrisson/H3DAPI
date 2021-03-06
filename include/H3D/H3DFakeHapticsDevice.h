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
/// \file H3DFakeHapticsDevice.h
/// \brief Header file for H3DFakeHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFAKEHAPTICSDEVICE_H__
#define __H3DFAKEHAPTICSDEVICE_H__

#include "H3D/H3DHapticsDevice.h"
#include "H3D/ThreadSafeFields.h"
#include <H3DUtil/Threads.h>

#include <HAPI/HAPIHapticsDevice.h>

namespace H3D {
  /// \ingroup AbstractNodes  
  /// \class H3DFakeHapticsDevice
  /// \brief H3DFakeHapticsDevice is a base class for making fake haptics
  /// devices, i.e. haptics devices that are controlled with e.g keyboard or
  /// mouse.
  /// The set_devicePosition, set_deviceOrientation and set_mainButton fields
  /// are used to control the values of the haptics device. 
  class H3DAPI_API H3DFakeHapticsDevice: public H3DHapticsDevice {
  public:
    class H3DAPI_API FakeHapticsDevice: public HAPI::HAPIHapticsDevice {
    public:
      /// Implementation of updateDeviceValues using the set_... fields
      /// to specify values.
      virtual void updateDeviceValues( DeviceValues &dv, HAPI::HAPITime dt );

      /// Output is ignored on a fake haptics device.
      virtual void sendOutput( DeviceOutput &dv,
             HAPI::HAPITime dt ) {}

      /// Implementation of initHapticsDevice.
      virtual bool initHapticsDevice( int _thread_frequency = 1000 ) {
        return true;
      }

      /// Releases all resources allocated in initHapticsDevice. 
      virtual bool releaseHapticsDevice() { return true; }
    protected:
      H3DFakeHapticsDevice *owner;
    
  friend class H3DFakeHapticsDevice;
    };
    /// A field which ensure that getValue and only getValue can be called within
    /// haptic thread with thread safety.  
    /// Note: All other function such as setValue, upToDate is not allowed to 
    /// called within haptic thread. The actual value update happens in main 
    /// thread either through explicitly calling getValue or upToDate or wait 
    /// until the end of the main thread loop let PeriodicUpdate update itself.
    /// Note2: getValue() can only be called within either in haptic thread 
    /// or main thread. All other type of threads are not allowed
    template< class BaseField >
    class GetValueSafeField: public PeriodicUpdate< BaseField > {
    public:
      /// Get the value of the field.
      inline virtual const typename BaseField::value_type &getValue( int _id = 0 ) {
        if( H3DUtil::HapticThread::inHapticThread() ) {
          // if in haptic thread only get the rt_value and return
          value_lock.readLock();
          value_for_haptic = rt_value;
          value_lock.unlock();
          return value_for_haptic;
        } else {
          assert( H3DUtil::ThreadBase::inMainThread() );
          return PeriodicUpdate< BaseField >::getValue( _id );
        }
      }
      /// update can not happens in non-main thread
      inline virtual void update() {
        assert( H3DUtil::ThreadBase::inMainThread() ); // terminate if update in non-main thread
        PeriodicUpdate< BaseField >::update();
        value_lock.writeLock();
        rt_value = this->value;
        value_lock.unlock();
      }
      /// Make sure that the field is up-to-date, but never in non-main thread
      virtual void upToDate() {
        assert( H3DUtil::ThreadBase::inMainThread() );
        PeriodicUpdate< BaseField >::upToDate();
      }
      /// Set the value of the field, also only allowed in main thread
      inline virtual void setValue( const typename BaseField::value_type &v,
                                    int _id = 0 ) {
          assert( H3DUtil::ThreadBase::inMainThread() );
          PeriodicUpdate< BaseField >::setValue( v, _id );
          value_lock.writeLock();
          rt_value = this->value;
          value_lock.unlock();
      }
    protected:
      // value used to exchange value between haptic and main thread 
      typename BaseField::value_type rt_value;
      /// value to be used for haptic thread
      typename BaseField::value_type value_for_haptic;
      H3DUtil::ReadWriteLock value_lock;
    };

    /// Constructor.
    H3DFakeHapticsDevice( Inst< SFVec3f         > _devicePosition         = 0,
        Inst< SFRotation      > _deviceOrientation      = 0,
        Inst< TrackerPosition > _trackerPosition        = 0,
        Inst< TrackerOrientation > _trackerOrientation  = 0,
        Inst< SFMatrix4f      > _positionCalibration    = 0,
        Inst< SFRotation      > _orientationCalibration = 0,
        Inst< SFVec3f         > _proxyPosition          = 0,
        Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
        Inst< SFFloat         > _proxyWeighting         = 0,
        Inst< SFBool          > _mainButton             = 0,
        Inst< SFBool          > _secondaryButton       = 0,
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
        Inst< GetValueSafeField< SFVec3f > > _set_devicePosition     = 0,
        Inst< GetValueSafeField< SFRotation > > _set_deviceOrientation  = 0,
        Inst< GetValueSafeField< SFBool > > _set_mainButton         = 0,
        Inst< GetValueSafeField< SFBool > > _set_secondaryButton    = 0,
        Inst< GetValueSafeField< SFBool > > _set_pauseDevice        = 0 );

    /// Destructor.
    ~H3DFakeHapticsDevice() {
      releaseDevice();
      hapi_device.reset( NULL );
    }

    /// Node database entry
    static H3DNodeDatabase database;

    /// The set_devicePosition field can be used to set the 
    /// devicePosition field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< GetValueSafeField<SFVec3f> > set_devicePosition;

    /// The set_deviceOrientation field can be used to set the 
    /// deviceOrientation field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< GetValueSafeField<SFRotation> > set_deviceOrientation;

    /// The set_mainButton field can be used to set the 
    /// mainButton field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< GetValueSafeField<SFBool> > set_mainButton;

    /// The set_secondaryButton field can be used to set the 
    /// secondaryButton field of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< GetValueSafeField<SFBool> > set_secondaryButton;

    /// The set_pauseDeviceTransform field can be used to pause 
    /// the transform of the haptics device.
    ///
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< GetValueSafeField<SFBool> > set_pauseDeviceTransform;

    bool pauseDeviceTransform;

    HAPI::HAPIHapticsDevice::DeviceValues pauseDeviceTransformDV;
  };
}

#endif
