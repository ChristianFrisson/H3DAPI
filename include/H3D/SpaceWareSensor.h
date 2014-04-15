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
/// \file SpaceWareSensor.h
/// \brief Header file for SpaceWareSensor.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPACEWARESENSOR_H__
#define __SPACEWARESENSOR_H__


#include <H3D/X3DSensorNode.h>
#include <list>
#include <H3D/SFVec3f.h>
#include <H3D/SFFloat.h>
#include <H3D/SFInt32.h>
#include <H3D/SFRotation.h>
#include <H3DUtil/Threads.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class SpaceWareSensor
  /// \brief This is a X3DSensorNode for reading values from a 3DConnexion motion
  /// controller such as the SpaceTraveller or SpaceMouse 5000.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SpaceWareSensor.x3d">SpaceWareSensor.x3d</a>
  ///     ( <a href="examples/SpaceWareSensor.x3d.html">Source</a> )
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
        AccumulateRotation() { last_time = TimeStamp(); }
      
      virtual void setValue( const Rotation &r, int id = 0 ) {
        SFRotation::setValue( r, id );
        last_time = TimeStamp();
      }

      virtual void update() {
        H3DTime current_time = TimeStamp();
        H3DTime time_scale = 1;
        if( static_cast< SpaceWareSensor * >(owner)->accumulateTimeDependent->getValue() )
          time_scale = current_time - last_time;
        last_time = current_time;
        Rotation r = static_cast< SFRotation * >( event.ptr )->getValue();
        value = ( r * time_scale ) * value;
      }
      protected:
      H3DTime last_time;
    };

    /// This class is an AutoUpdate class that sets the field
    /// accumulatedRotation to its input value and also sets
    /// accumulatedYaw, accumulatedPitch and accumulatedRoll to
    /// a value dependent on input value
    ///
    class H3DAPI_API ResetAccumulatedRotation: 
      public AutoUpdate< SFRotation > {

     public:
      virtual void setValue( const Rotation &r, int id = 0 ) {
        SFRotation::setValue( r );
        SpaceWareSensor * sws = static_cast< SpaceWareSensor * >(owner);
        sws->accumulatedRotation->setValue( r, sws->id );
        Rotation temp_r = r;
        Vec3f euler_angles = temp_r.toEulerAngles();
        sws->accumulatedYaw->setValue(
          Rotation( 1, 0, 0, euler_angles.x ), sws->id );
        sws->accumulatedPitch->setValue(
          Rotation( 0, 1, 0, euler_angles.y ), sws->id );
        sws->accumulatedRoll->setValue(
          Rotation( 0, 0, 1, euler_angles.z ), sws->id );        
      }
      
    protected:
      virtual void update() {
        SFRotation::update();
        SpaceWareSensor * sws = static_cast< SpaceWareSensor * >(owner);
        sws->accumulatedRotation->setValue( value, sws->id );
        Rotation temp_r = value;
        Vec3f euler_angles = temp_r.toEulerAngles();
        sws->accumulatedYaw->setValue(
          Rotation( 1, 0, 0, euler_angles.x ), sws->id );
        sws->accumulatedPitch->setValue(
          Rotation( 0, 1, 0, euler_angles.y ), sws->id );
        sws->accumulatedRoll->setValue(
          Rotation( 0, 0, 1, euler_angles.z ), sws->id );     
      }
    };
#ifdef __BORLANDC__
    friend class ResetAccumulatedRotation;
#endif

    /// This class is an AutoUpdate class that accumulates the Vec3f
    /// values routed to it, i.e. each time a field generates an event the 
    /// value of that field is added with the current value.
    ///
    struct H3DAPI_API AccumulateTranslation: 
      public AutoUpdate< SFVec3f > {
      AccumulateTranslation() { last_time = TimeStamp(); }

      virtual void setValue( const Vec3f &v, int id = 0 ) {
        SFVec3f::setValue( v, id );
        last_time = TimeStamp();
      }
      
      virtual void update() {
        H3DTime current_time = TimeStamp();
        H3DTime time_scale = 1;
        if( static_cast< SpaceWareSensor * >(owner)->accumulateTimeDependent->getValue() )
          time_scale = ( current_time - last_time ) * 0.025f;
        last_time = current_time;
        const Vec3f &v = static_cast< SFVec3f * >( event.ptr )->getValue();
        value = value + v * time_scale;
      }
    protected:
      H3DTime last_time;
    };

    /// This class is an AutoUpdate class that sets the field
    /// accumulatedTranslation to its input value
    ///
    class H3DAPI_API ResetAccumulatedTranslation: 
      public AutoUpdate< SFVec3f > {
    public:
      virtual void setValue( const Vec3f &v, int id = 0 ) {
        SFVec3f::setValue( v );
        SpaceWareSensor * sws = static_cast< SpaceWareSensor * >(owner);
        sws->accumulatedTranslation->setValue( v, sws->id );
      }
    protected: 
      virtual void update() {
        SFVec3f::update();
        SpaceWareSensor * sws = static_cast< SpaceWareSensor * >(owner);
        sws->accumulatedTranslation->setValue( value, sws->id );
      }
    };
#ifdef __BORLANDC__
    friend class ResetAccumulatedTranslation;
#endif
  
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
                    Inst< SFInt32               > _latestButtonRelease = 0,
                    Inst< SFBool                > _accumulateTimeDependent = 0,
          Inst< ResetAccumulatedTranslation > _resetAccumulatedTranslation = 0,
          Inst< ResetAccumulatedRotation > _resetAccumulatedRotation = 0
                    );

    ~SpaceWareSensor() {
      if( this == sws_instance )
        sws_instance = 0;
    }

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
    auto_ptr< ScaleRotation>     instantRotation;

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

    /// If true the Accumulated-fields depends on time passed since
    /// last time accumulated was updated instead of frame rate.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// 
    /// \dotfile SpaceWareSensor_accumulateTimeDependent.dot
    auto_ptr< SFBool > accumulateTimeDependent;

    /// Resets the value of accumulatedTranslation to its own value
    ///
    /// <b>Access type: </b> inputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_resetAccumulatedTranslation.dot
    auto_ptr< ResetAccumulatedTranslation > resetAccumulatedTranslation;

    /// Resets the value of accumulatedRotation to its own value.
    /// Also resets accumulatedYaw, accumulatedPitch and accumulatedRoll
    ///
    /// <b>Access type: </b> inputOnly \n
    /// 
    /// \dotfile SpaceWareSensor_resetAccumulatedRotation.dot
    auto_ptr< ResetAccumulatedRotation > resetAccumulatedRotation;

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
    // is_active in thread. Will be false if thread main loop
    // can not run due to some error.
    bool thread_is_active;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    // instance of the first SpaceWareSensor created
    // used if the spacewaresensor should be used to navigate the scene
    static SpaceWareSensor *sws_instance;

  private:
#ifdef HAVE_3DXWARE
    // the handle of the communication thread.
    auto_ptr< H3DUtil::SimpleThread > thread_handle;
#endif

    /// Transfers the values from the device communication thread to
    /// the scenegraph thread.
    void updateValues();
    
    struct H3DAPI_API Update
      : AutoUpdate<Field> {
      void update(){
        static_cast<SpaceWareSensor*>
          (owner)->updateValues();
      }
    };
    
    auto_ptr< Update > update;
  };
}

#endif
