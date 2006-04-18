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
/// \file SpaceWareSensor.h
/// \brief Header file for MouseSensor
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPACEWARESENSOR_H__
#define __SPACEWARESENSOR_H__


#include "X3DSensorNode.h"
#include <list>
#include "SFVec3f.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFRotation.h"
#include "Threads.h"

namespace H3D {

  /// \ingroup H3DNodes
  /// \class SpaceWareSensor
  /// \brief This is a X3DSensorNode for reading values from a 3DConnexion motion
  /// controller such as the SpaceTraveller or SpaceMouse 5000.
  ///
  /// \par Internal routes:
  /// \dotfile SpaceWareSensor.dot
  class H3DAPI_API SpaceWareSensor : public X3DSensorNode {
  public:

    /// Updates itself to the Rotation routed to it where the angle 
    /// is scaled by the scale factor. 
    /// 
    /// routes_in[0] the rotation to scale.
    /// routes_in[1] the scale factor.
    ///
    struct H3DAPI_API ScaleRotation: 
      public TypedField< SFRotation,
                         Types< SFRotation, SFFloat > > {
      virtual void update() {
        Rotation r = 
          static_cast< SFRotation * >( routes_in[0] )->getValue();
        H3DFloat s = 
          static_cast< SFFloat * >( routes_in[1] )->getValue();
  
        //TODO: fix so that operator can be used
        r.angle = r.angle * s;
        value = r;
      }
    };

    /// Updates itself to the SFVec3f routed to it scaled by the scale factor.
    /// 
    /// routes_in[0] the SFVec3f to scale.
    /// routes_in[1] the scale factor.
    ///
    struct H3DAPI_API ScaleTranslation: 
      public TypedField< SFVec3f,
                         Types< SFVec3f, SFFloat > > {
      virtual void update() {
        const Vec3f &r = static_cast< SFVec3f * >( routes_in[0] )->getValue();
        H3DFloat s = static_cast< SFFloat * >( routes_in[1] )->getValue();
        value = r * s;
      }
    };


    /// This class is an AutoUpdate class that accumulates the rotation
    /// values routed to it, i.e. each time a field generates an event the 
    /// value of that field is multiplied with the current rotation.
    ///
    struct H3DAPI_API AccumulateRotation: 
      public AutoUpdate< SFRotation > {
      virtual void update() {
        Rotation r = static_cast< SFRotation * >( event.ptr )->getValue();
        value = r * value;
      }
    };

    /// This class is an AutoUpdate class that accumulates the Vec3f
    /// values routed to it, i.e. each time a field generates an event the 
    /// value of that field is added with the current value.
    ///
    struct H3DAPI_API AccumulateTranslation: 
      public AutoUpdate< SFVec3f > {
      virtual void update() {
        const Vec3f &v = static_cast< SFVec3f * >( event.ptr )->getValue();
        value = value + v;
      }
    };
  
    /// Constructor.
    SpaceWareSensor( 
                    Inst< SFBool                > _enabled  = 0,
                    Inst< SFNode                > _metadata = 0,
                    Inst< SFBool                > _isActive = 0,
                    Inst< SFVec3f               > _rawTranslation = 0,
                    Inst< SFRotation            > _rawYaw = 0,
                    Inst< SFRotation            > _rawPitch = 0,
                    Inst< SFRotation            > _rawRoll = 0,
                    Inst< SFRotation            > _rawRotation = 0,
                    Inst< ScaleTranslation      > _instantTranslation = 0,
                    Inst< ScaleRotation         > _instantYaw = 0,
                    Inst< ScaleRotation         > _instantPitch = 0,
                    Inst< ScaleRotation         > _instantRoll = 0,
                    Inst< ScaleRotation         > _instantRotation = 0,
                    Inst< AccumulateTranslation > _accumulatedTranslation = 0,
                    Inst< AccumulateRotation    > _accumulatedYaw = 0,
                    Inst< AccumulateRotation    > _accumulatedPitch = 0,
                    Inst< AccumulateRotation    > _accumulatedRoll = 0,
                    Inst< AccumulateRotation    > _accumulatedRotation = 0,
                    Inst< SFFloat               > _translationScale = 0,
                    Inst< SFFloat               > _rotationScale = 0,
                    Inst< SFInt32               > _buttons = 0,
                    Inst< SFInt32               > _latestButtonPress = 0,
                    Inst< SFInt32               > _latestButtonRelease = 0
                    );

    /// Contains the current translation as reported by the device.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_rawTranslation.dot
    auto_ptr< SFVec3f>             rawTranslation;

    /// Contains the current rotation around the x-axis as reported 
    /// by the device.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_rawYaw.dot
    auto_ptr< SFRotation>          rawYaw;

    /// Contains the current rotation around the y-axis as reported 
    /// by the device.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_rawPitch.dot
    auto_ptr< SFRotation>          rawPitch;

    /// Contains the current rotation around the z-axis as reported 
    /// by the device.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_rawRoll.dot
    auto_ptr< SFRotation>          rawRoll;

    /// Contains the current total rotation as reported 
    /// by the device.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_rawRotation.dot
    auto_ptr< SFRotation>        rawRotation;

    /// rawTranslation scaled by rotationScale
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_instantTranslation.dot
    auto_ptr< ScaleTranslation>  instantTranslation;

    /// rawYaw scaled by rotationScale
    ///
    /// <b>Access type: </b> outputOnly  \n
    /// 
    /// \dotfile SpaceWareSensor_instantYaw.dot
    auto_ptr< ScaleRotation>     instantYaw;

    /// rawPitch scaled by rotationScale
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_instantPitch.dot
    auto_ptr< ScaleRotation>     instantPitch;

    /// rawRoll scaled by rotationScale
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_instantRoll.dot
    auto_ptr< ScaleRotation>     instantRoll;

    /// Composite rotation of instantYaw, instantPitch and instantRoll.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_instantRotation.dot
    auto_ptr< ScaleRotation>      instantRotation;

    /// The sum of all instantTranslation values.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_accumulatedTranslation.dot
    auto_ptr< AccumulateTranslation>  accumulatedTranslation;

    /// The sum of all instantYaw values.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_accumulatedYaw.dot
    auto_ptr< AccumulateRotation>     accumulatedYaw;

    /// The sum of all instantPitch values.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_accumulatedPitch.dot
    auto_ptr< AccumulateRotation>     accumulatedPitch;

    /// The sum of all instantRoll values.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_accumulatedRoll.dot
    auto_ptr< AccumulateRotation>     accumulatedRoll; 

    /// The sum of all instantRotation values.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_accumulatedRotation.dot
    auto_ptr< AccumulateRotation>     accumulatedRotation;

    /// The scale factor for the instantTranslation field.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 1 \n
    /// 
    /// \dotfile SpaceWareSensor_translationScale.dot
    auto_ptr< SFFloat>          translationScale; 

    /// The scale factor for the instant version of the different
    /// rotation fields.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 1 \n
    /// 
    /// \dotfile SpaceWareSensor_rotationScale.dot
    auto_ptr< SFFloat>          rotationScale;

    /// Integer representing the current status of the buttons. Each bit
    /// represents the status of a button where 1 means that it is pressed.
    /// There are three special button bits: SI_RESET_BIT, SI_PICK_BIT and  
    /// SI_DIALOG_BIT (these buttons are not present on all devices). Aside
    /// from these the first button is the 0x2 bit, the second is the 0x4 
    /// and so on.
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_buttons.dot
    auto_ptr< SFInt32 > buttons;

    /// The button number of the last button that was pressed. 
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_latestButtonPress.dot
    auto_ptr< SFInt32 > latestButtonPress;

    /// The button number of the last button that was released. 
    ///
    /// <b>Access type: </b> outputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_latestButtonRelease.dot
    auto_ptr< SFInt32 > latestButtonRelease;

    /// Transfers the values from the device communication thread to
    /// the scenegraph thread.
    virtual void traverseSG( TraverseInfo &ti );

    // This data structure is used to transfer button data from the 
    // device communication thread to the scene graph thread.
    struct ButtonData {
      ButtonData( int _pressed, 
                  int _released,
                  int _current_state ) :
        pressed( _pressed ),
        released( _released ),
        current_state( _current_state ) {}
      unsigned int pressed, released, current_state;
    };
  public:
    // has a motion event occured since last scenegraph loop.
    bool thread_motion_event;
    // button data since last scenegraph loop.
    vector< ButtonData > thread_buttons; 
    // rotation values from the last motion event.
    Vec3f thread_rotation;
    // translation values from the last motion event.
    Vec3f thread_translation;
   /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
#ifdef HAVE_3DXWARE
    // the handle of the communication thread.
    auto_ptr< SimpleThread > thread_handle;
#endif
  };
}

#endif
