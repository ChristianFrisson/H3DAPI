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
/// \file CyberGloveSensor.cpp
/// \brief CPP file for CyberGloveSensor.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// H3D includes
#include <H3D/CyberGloveSensor.h>
#include <H3D/Scene.h>

#ifdef HAVE_VIRTUAL_HAND_SDK
// Virtual Hand SDK includes
#include <vhandtk/vhtIOConn.h>
#include <vhandtk/vhtBaseException.h>
#include <vhandtk/vhtCyberGlove.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase CyberGloveSensor::database( 
                                          "CyberGloveSensor", 
                                          &(newInstance<CyberGloveSensor>),
                                          typeid( CyberGloveSensor ),
                                          &VirtualHandGloveSensor::database
                                          );

namespace CyberGloveSensorInternal {
  FIELDDB_ELEMENT( CyberGloveSensor, comPort, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CyberGloveSensor, hardwareSpeed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( CyberGloveSensor, thumb, OUTPUT_ONLY );
  FIELDDB_ELEMENT( CyberGloveSensor, index, OUTPUT_ONLY );
  FIELDDB_ELEMENT( CyberGloveSensor, middle, OUTPUT_ONLY );
  FIELDDB_ELEMENT( CyberGloveSensor, ring, OUTPUT_ONLY );
  FIELDDB_ELEMENT( CyberGloveSensor, pinky, OUTPUT_ONLY );
}



CyberGloveSensor::CyberGloveSensor( Inst< OnEnable   > _enabled,
                                    Inst< SFNode   > _metadata,
                                    Inst< SFBool   > _isActive,
                                    Inst< SFString > _host,
                                    Inst< SFString > _port,
                                    Inst< SFInt32  > _comPort,
                                    Inst< SFInt32  > _hardwareSpeed,
                                    Inst< MFDouble > _thumb,
                                    Inst< MFDouble > _index,
                                    Inst< MFDouble > _middle,
                                    Inst< MFDouble > _ring,
                                    Inst< MFDouble > _pinky ) :
  VirtualHandGloveSensor( _enabled, _metadata, _isActive, _host, _port ),
  comPort( _comPort ),
  hardwareSpeed( _hardwareSpeed ),
  thumb( _thumb ),
  index( _index ),
  middle( _middle ),
  ring( _ring ),
  pinky( _pinky ) {

  type_name = "CyberGloveSensor";
  database.initFields( this );

  comPort->setValue( -1 );
  hardwareSpeed->setValue( 115200 );
}

bool CyberGloveSensor::connect() {
#ifdef HAVE_VIRTUAL_HAND_SDK
  if( !glove ) {
    H3DInt32 com_port = comPort->getValue();
    if( com_port == -1 ) {
      // If no comPort is given, use default.
      try {
        glove =
          new vhtCyberGlove(vhtIOConn::getDefault( vhtIOConn::glove ) );
        return true;
      } catch( vhtBaseException *e ) {
        glove = 0;
        Console(3) << "Warning: " << e->getMessage()
                   << " in Node " << getName()
                   << " Check that the VirtualHand Device Manager is started"
                   << " and use DCU to check that a default configuration"
                   << " exists." << endl;
      }
    } else {
      // Use values supplied by user to connect to the hand.
      string tmp_host = host->getValue();
      string tmp_port = port->getValue();
      stringstream com_port_stm;
      com_port_stm << "com" << com_port;
      if( tmp_host != "" && tmp_port != "" ) {
        vhtIOConn gloveAddress( "cyberglove",
                                tmp_host.c_str(),
                                tmp_port.c_str(),
                                com_port_stm.str().c_str(),
                                hardwareSpeed->getValueAsString().c_str() );
        try {
          glove = new vhtCyberGlove(&gloveAddress);
          return true;
        } catch( vhtBaseException *e ) {
          glove = 0;
          Console(3) << "Warning: " << e->getMessage()
                   << " in Node " << getName()
                   << " Check that the VirtualHand Device Manager is started"
                   << " and that the values of the fields deviceClass, host,"
                   << " port, connectionType and hardwareSpeed are correct."
                   << endl;
        }
      } else {
        Console(3) << "Warning: Incorrect value for field host, "
                   << "port or connectionType of Node "
                   << getName() << ". No cyber glove will be used." << endl;
      }
    }
  } else {
    // A glove is already created, simply connect to it.
    return glove->connect();
  }
#endif
  return false;
}

void CyberGloveSensor::disconnect() {
#ifdef HAVE_VIRTUAL_HAND_SDK
  // Disconnect glove.
  if( glove )
    glove->disconnect();
#endif
}

void CyberGloveSensor::updateValues() {
#ifdef HAVE_VIRTUAL_HAND_SDK
  // Update field values if we have a glove and the sensor node is active.
  // That is, if a connection to the CyberGlove was established.
  if( glove && isActive->getValue() ) {
    //glove->update();
    // No check is done to see if there are more data than this for one finger.
    vector< H3DDouble > finger_data;
    finger_data.push_back( glove->getAngle( GHM::thumb, GHM::metacarpal ) );
    finger_data.push_back( glove->getAngle( GHM::thumb, GHM::proximal ) );
    finger_data.push_back( glove->getAngle( GHM::thumb, GHM::distal ) );
    thumb->setValue( finger_data, id );
    finger_data.clear();
    finger_data.push_back( glove->getAngle( GHM::index, GHM::metacarpal ) );
    finger_data.push_back( glove->getAngle( GHM::index, GHM::proximal ) );
    finger_data.push_back( glove->getAngle( GHM::index, GHM::distal ) );
    index->setValue( finger_data, id );
    finger_data.clear();
    finger_data.push_back( glove->getAngle( GHM::middle, GHM::metacarpal ) );
    finger_data.push_back( glove->getAngle( GHM::middle, GHM::proximal ) );
    finger_data.push_back( glove->getAngle( GHM::middle, GHM::distal ) );
    middle->setValue( finger_data, id );
    finger_data.clear();
    finger_data.push_back( glove->getAngle( GHM::ring, GHM::metacarpal ) );
    finger_data.push_back( glove->getAngle( GHM::ring, GHM::proximal ) );
    finger_data.push_back( glove->getAngle( GHM::ring, GHM::distal ) );
    ring->setValue( finger_data, id );
    finger_data.clear();
    finger_data.push_back( glove->getAngle( GHM::pinky, GHM::metacarpal ) );
    finger_data.push_back( glove->getAngle( GHM::pinky, GHM::proximal ) );
    finger_data.push_back( glove->getAngle( GHM::pinky, GHM::distal ) );
    pinky->setValue( finger_data, id );
  }
#endif
}


