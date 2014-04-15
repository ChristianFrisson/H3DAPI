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
/// \file VirtualHandGloveSensor.h
/// \brief Header file for VirtualHandGloveSensor.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VIRTUALHANDGLOVESENSOR_H__
#define __VIRTUALHANDGLOVESENSOR_H__

// H3D includes
#include <H3D/X3DSensorNode.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>
#include <H3D/MFDouble.h>

#ifdef HAVE_VIRTUAL_HAND_SDK
// Virtual Hand SDK includes
#include <vhandtk/vhtGlove.h>
#endif

namespace H3D {

  /// \ingroup H3DNodes
  /// \class VirtualHandGloveSensor
  /// \brief This is the base class for all sensors that handles connection to
  /// the vhtGlove class in Virtual Hand SDK.
  class H3DAPI_API VirtualHandGloveSensor : public X3DSensorNode {
  public:

    // Connects or disconnects the device depending on the value of the enabled
    // field.
    class OnEnable : public AutoUpdate< OnValueChangeSField< SFBool > > {
    protected:
      virtual void onValueChange( const bool &new_value) {
        VirtualHandGloveSensor * vhgs =
          static_cast< VirtualHandGloveSensor * >( owner );
        if( vhgs->isInitialized() ) {
          if( new_value ) {
            if( vhgs->connect() )
              vhgs->isActive->setValue( true, vhgs->id );
            
          } else {
            vhgs->disconnect();
            vhgs->isActive->setValue( false, vhgs->id );
          }
        }
      }
    };

    /// Constructor.
    VirtualHandGloveSensor( Inst< OnEnable   > _enabled  = 0,
                            Inst< SFNode   > _metadata = 0,
                            Inst< SFBool   > _isActive = 0,
                            Inst< SFString > _host = 0,
                            Inst< SFString > _port = 0 );

    /// Destructor
    ~VirtualHandGloveSensor() {
#ifdef HAVE_VIRTUAL_HAND_SDK
      if( glove ) {
        delete glove;
        glove = 0;
      }
#endif
    }

    /// Connect to the device if enabled is true on first reference of the
    /// node. Used in order to not accidently connect before the user has'
    /// decided the value of enabled.
    virtual void initialize();

#ifdef HAVE_VIRTUAL_HAND_SDK
    /// Return reference to the contained vhtGlove.
    inline vhtGlove * getGlove() { return glove; }
#endif

    /// Reset glove to 0, needed in cleanup phase to not accidently try to
    /// delete instance of vhtGlove twice.
    inline void resetGlove() {
#ifdef HAVE_VIRTUAL_HAND_SDK
      glove = 0;
#endif
    }

    /// Adress of the host running the VTIDM daemon process. Host should be
    /// a resolvable internet name or address.
    /// Only used when the sensor is enabled.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default Value: </b> "localhost" \n
    auto_ptr< SFString> host;

    /// The port number specified in the the Device Manager of
    /// Virtual Hand SDK.
    /// Only used when the sensor is enabled.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default Value: </b> "12345" \n
    auto_ptr< SFString> port;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Virtual function used to update field values in subclasses.
    virtual void updateValues() = 0;

    /// Virtual function that connects the vhtGlove
    virtual bool connect() = 0;

    /// Virtual function that disconnects the vhtGlove
    virtual void disconnect() = 0;

#ifdef HAVE_VIRTUAL_HAND_SDK
    // vhtGlove instance
    vhtGlove * glove;
#endif

  private:
    // Calls the updateValues function whenever it gets an event.
    // Scene::time will be routed to this field.
    struct H3DAPI_API Update
      : AutoUpdate<Field> {
      void update(){
        static_cast<VirtualHandGloveSensor*>
          (owner)->updateValues();
      }
    };

    auto_ptr< Update > update;
  };
}

#endif
