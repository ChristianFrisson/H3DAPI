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
/// \file H3DHapticsDevice.h
/// \brief Header file for H3DHapticsDevice.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DHAPTICSDEVICE_H__
#define __H3DHAPTICSDEVICE_H__

#include "H3DApi.h"
#include "FieldTemplates.h"
#include "HapticShape.h"
#include "HapticForceEffect.h"
#include "SFNode.h"
#include "Threads.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DHapticsDevice
  /// Base class for all haptic devices. It contains information about the 
  /// haptics device, e.g. the position and orientation of the device, the
  /// forces being rendered and supported degrees of freedom. It also
  /// contains functions for rendering HapticObject instances on the device.
  /// Subclasses should override renderObjects() to render
  /// the objects on the H3DHapticsDevice.
  /// 
  /// \par Internal routes:
  /// \dotfile H3DHapticsDevice.dot
  class H3DAPI_API H3DHapticsDevice: public Node {
  public:

    /// Saves the value of the field in a variable that can be accessed
    /// from the realtime loop.
    class H3DAPI_API PosCalibration: 
      public SFMatrix4f {
    public:
      /// Update the rt variables.
      virtual void setValue( const Matrix4f &v, int id = 0 ) {
        SFMatrix4f::setValue( v, id );
        rt_pos_calibration = value;
        rt_inv_pos_rotation = rt_pos_calibration.getRotationPart().inverse();
      }

      /// Update the rt variables.
      inline virtual void update() {
        SFMatrix4f::update();
        rt_pos_calibration = value;
        rt_inv_pos_rotation = rt_pos_calibration.getRotationPart().inverse();
      }

      /// The value of the PosCalibration field to be accessed from 
      /// the realtime loop.
      Matrix4f rt_pos_calibration;
      
      /// The inverse of the part of rt_pos_calibration that involves
      /// rotation.
      Matrix3f rt_inv_pos_rotation;
    };

    /// Saves the value of the field in a variable that can be accessed
    /// from the realtime loop.
    class H3DAPI_API OrnCalibration: 
      public SFRotation {
    public:
      /// Update the rt variable.
      inline virtual void setValue( const Rotation &v, int id = 0 ) {
        SFRotation::setValue( v, id );
        rt_orn_calibration = value;
      }

      /// Update the rt variable.
      virtual void update() {
        SFRotation::update();
        rt_orn_calibration = value;
      }

      /// The value of the OrnCalibration field to be accessed from 
      /// the realtime loop.
      Rotation rt_orn_calibration;
    };

    /// The TrackerPosition field updates itself from the device_position
    /// and position_calibration fields. 
    /// TrackerPosition = positionCalibration * devicePosition 
    ///
    /// routes_in[0] is the positionCalibration field
    /// routes_in[1] is the devicePosition field
    ///
    class H3DAPI_API TrackerPosition: 
      public TypedField< SFVec3f, Types< SFMatrix4f, SFVec3f > > {
      
      /// value = position_calibration * device_position.
      virtual void update() {
        const Matrix4f &m  = 
          static_cast< SFMatrix4f * >( routes_in[0] )->getValue();
        const Vec3f &d_pos = 
          static_cast< SFVec3f * >( routes_in[1] )->getValue();
        value = m * d_pos;
      }
    };

    /// The TrackerOrientation field updates itself from the device_orientation
    /// and orientation_calibration fields. 
    /// TrackerOrientation = orientationCalibration * deviceOrientation 
    ///
    /// routes_in[0] is the orientationCalibration
    /// routes_in[1] is the deviceOrientaiton
    ///
    class H3DAPI_API TrackerOrientation: 
      public TypedField< SFRotation, Types< SFRotation, SFRotation > > {

      virtual void update() {
        const Rotation &cal  = 
          static_cast< SFRotation * >( routes_in[0] )->getValue();
        const Rotation &d_orn = 
          static_cast< SFRotation * >( routes_in[1] )->getValue();
        value = cal * d_orn;
      }
  
    };

    /// The WeightedProxy field updates to be a value between the proxy position
    /// and tracker position depending on the value of a weighting factor. 
    /// If the weightting factor is 1 if will be the proxy position, if 0 the
    /// tracker position. 
    /// 
    /// routes_in[0] is the proxy position
    /// routes_in[1] is the tracker position.
    /// routes_in[2] is the weighting factor.
    ///
    class H3DAPI_API WeightedProxy:
      public TypedField< SFVec3f, Types< SFVec3f, SFVec3f, SFFloat > > {
      virtual void update() {
        const Vec3f &proxy_pos   = 
          static_cast< SFVec3f * >( routes_in[0] )->getValue();
        const Vec3f &tracker_pos = 
          static_cast< SFVec3f * >( routes_in[1] )->getValue();
        H3DFloat weighting = 
          static_cast< SFFloat * >( routes_in[2] )->getValue();
        value =  tracker_pos + weighting * ( proxy_pos - tracker_pos );
      }
    };


    /// Constructor.
    H3DHapticsDevice( Inst< SFVec3f         > _devicePosition         = 0,
                   Inst< SFRotation      > _deviceOrientation      = 0,
                   Inst< TrackerPosition > _trackerPosition        = 0,
                   Inst< TrackerOrientation > _trackerOrientation  = 0,
                   Inst< PosCalibration  > _positionCalibration    = 0,
                   Inst< OrnCalibration  > _orientationCalibration = 0,
                   Inst< SFVec3f         > _proxyPosition          = 0,
                   Inst< WeightedProxy   > _weightedProxyPosition  = 0,     
                   Inst< SFFloat         > _proxyWeighting         = 0,
                   Inst< SFBool          > _main_button            = 0,
                   Inst< SFVec3f         > _force                  = 0,
                   Inst< SFVec3f         > _torque                 = 0,
                   Inst< SFInt32         > _inputDOF               = 0,
                   Inst< SFInt32         > _outputDOF              = 0,
                   Inst< SFInt32         > _hapticsRate            = 0,
                   Inst< SFNode          > _stylus                 = 0,
                   Inst< SFBool          > _initialized            = 0 );

    /// Destructor. Stops haptics rendering and remove callback functions.
    virtual ~H3DHapticsDevice() {
      disableDevice();
      if( thread )
        delete thread;
    }

    /// Returns the default xml containerField attribute value.
    /// For this node it is "appearance".
    virtual string defaultXMLContainerField() {
      return "device";
    }

    /// Does all the initialization needed for the device before starting to
    /// use it.
    virtual void initDevice() {
      initialized->setValue( true );
    }

    /// Get the thread that is used to run this haptics device.
    inline ThreadBase *getThread() { return thread; }

    /// Reset the device.
    virtual void resetDevice() {}

    /// Perform cleanup and let go of all device resources that are allocated.
    /// After a call to this function no haptic rendering can be performed on
    /// the device until the initDevice() function has been called again.
    virtual void disableDevice() {
      initialized->setValue( false );
    }

    /// This function is used to transfer device values, such as position, 
    /// button status etc from the realtime loop to the fields of H3DHapticsDevice,
    /// and possible vice versa.
    virtual void updateDeviceValues() {}

    /// This function is called at the start of each scenegraph loop before any
    /// calls to other HapticDevice functions and can be used to perform any 
    /// necessary operation that are needed for the other calls to function
    /// properly.
    virtual void preRender() {}

    /// This function is called at the end of each scenegraph loop after all
    /// calls to other HapticDevice functions and can be used to perform any 
    /// necessary operation that are needed.
    virtual void postRender() {}

    /// Perform haptic rendering for the given HapticShape instances. 
    /// HapticShape objects that are to be be rendered haptically must be 
    /// rendered with this function each scenegraph loop. 
    /// \param objects The haptic shapes to render.
    ///
    virtual void renderShapes( const HapticShapeVector &shapes ) {};

    /// Perform haptic rendering for the given HapticForceEffect instances. 
    /// HapticForceEffect objects that are to be be rendered haptically must
    /// be rendered with this function each scenegraph loop.
    /// \param objects The haptic objects to render.
    ///
    virtual void renderEffects( const HapticEffectVector &effects ) {};
    
    /// The position of the device given in the coordinate system of the 
    /// device.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_devicePosition.dot
    auto_ptr< SFVec3f >    devicePosition;

    /// The orientation of the device given in the coordinate system of the 
    /// device. Only applicable if the device supports orientation as input.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_deviceOrientation.dot
    auto_ptr< SFRotation > deviceOrientation;
    
    /// The position of the device in the world coordinates of the API.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_trackerPosition.dot
    auto_ptr< TrackerPosition >    trackerPosition;

    /// The orientation of the device in the world coordinates of the API.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_trackerOrientation.dot
    auto_ptr< TrackerOrientation > trackerOrientation;

    /// The calibration matrix between devicePosition and trackerPosition.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Unit matrix \n
    /// 
    /// \dotfile H3DHapticsDevice_positionCalibration.dot
    auto_ptr< PosCalibration > positionCalibration;

    /// The calibration rotation between deviceOrientation and 
    /// trackerOrientation.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 1, 0, 0, 0 ) \n
    /// 
    /// \dotfile H3DHapticsDevice_orientationCalibration.dot
    auto_ptr< OrnCalibration > orientationCalibration;

    /// The position of the proxy used in the haptic rendering. 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_proxyPosition.dot
    auto_ptr< SFVec3f >   proxyPosition;

    /// A weighted position between proxyPosition and trackerPosition.
    /// The weighting factor is the proxyWeigting field. By default
    /// this is the field that is used to display the stylus graphically.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_weightedProxyPosition.dot
    auto_ptr< SFVec3f >   weightedProxyPosition;

    /// The weighting between proxyPosition and trackerPosition when
    /// calculating weightedProxyPosition. The value should be 
    /// between 0 and 1, with 1 meaning that the weightedProxyPosition
    /// is the proxyPosition and 0 that it is the trackerPosition.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.95 \n
    /// 
    /// \dotfile H3DHapticsDevice_proxyWeighting.dot
    auto_ptr< SFFloat >   proxyWeighting;

    /// The state of the main button. true means that the button is pressed.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_mainButton.dot
    auto_ptr< SFBool >   mainButton;

    /// The approximation of the force that has been rendered during the last 
    /// scenegraph loop.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_force.dot
    auto_ptr< SFVec3f >    force;

    /// The approximation of the torque that has been rendered during the last 
    /// scenegraph loop.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_torque.dot
    auto_ptr< SFVec3f >    torque;

    /// The degrees of freedom supported as input.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_inputDOF.dot
    auto_ptr< SFInt32 >   inputDOF;

    /// The degrees of freedom supported as output, i.e. forces and torques.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_outputDOF.dot
    auto_ptr< SFInt32 >   outputDOF;

    /// The update rate of the servoloop of the H3DHapticsDevice.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_hapticsRate.dot
    auto_ptr< SFInt32 >   hapticsRate;
    
    /// The Node used as the visual representation of the stylus.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DHapticsDevice_stylus.dot
    auto_ptr< SFNode > stylus;

    /// true if the device is initialized and ready for calls to 
    /// updateDeviceValues() and renderObjects(). If not initDevice() will have
    /// to be called in order to initialize it before calling those functions.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile H3DHapticsDevice_initialized.dot
    auto_ptr< SFBool > initialized;
    
    /// Node database entry
    static H3DNodeDatabase database;
  protected:
    /// The thread that this haptics device loop is run in.
    ThreadBase *thread;
  };
}

#endif
