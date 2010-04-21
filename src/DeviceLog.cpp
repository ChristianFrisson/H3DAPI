//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2009, SenseGraphics AB
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
//
/// \file H3DAPI/src/DeviceLog.cpp
/// \brief CPP file for DeviceLog, scene-graph node.
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DeviceLog.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DeviceLog::database( "DeviceLog", 
                                     &(newInstance<DeviceLog>),
                                     typeid( DeviceLog ),
                                     &H3DForceEffect::database );

namespace DeviceLogInternal {
  FIELDDB_ELEMENT( DeviceLog, url, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( DeviceLog, frequency, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( DeviceLog, deviceIndex, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( DeviceLog, logBinary, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( DeviceLog, logData, INITIALIZE_ONLY );
}

DeviceLog::DeviceLog( Inst< SFNode> _metadata,
                      Inst< MFString     > _url,
                      Inst< SFInt32     > _frequency,
                      Inst< SFInt32  > _deviceIndex,
                      Inst< SFBool   > _logBinary,
                      Inst< MFString > _logData ):
  H3DForceEffect( _metadata ),
  X3DUrlObject(_url),
  frequency(_frequency),
  deviceIndex( _deviceIndex ),
  logBinary( _logBinary ),
  logData( _logData ) {
  type_name = "DeviceLog";

  database.initFields( this );

  frequency->setValue( 100 );
  url->setValueFromString( "log.txt" );
  deviceIndex->setValue( 0 );
  logBinary->setValue( false );
  logData->setValueFromString( "ALL" );
}

void DeviceLog::initialize() {
  const vector< string > &urls = url->getValue();
  if( urls.empty() ) {
    Console(3) << "No file given " << endl;
  } else {
    string url_used = urls[0];
    // Go through the entries in the logData field and put it in a
    // LogTypeVector which will be used in the constructor of HAPI::DeviceLog.
    HAPI::DeviceLog::LogTypeVector log_types;
    const vector< string > &log_data = logData->getValue();
    for( vector< string >::const_iterator i = log_data.begin();
      i != log_data.end(); i++ ) {
      if( (*i) == "ALL" ) {
        log_types.clear();
        log_types.push_back( HAPI::DeviceLog::TIME );
        log_types.push_back( HAPI::DeviceLog::POSITION );
        log_types.push_back( HAPI::DeviceLog::ORIENTATION );
        log_types.push_back( HAPI::DeviceLog::VELOCITY );
        log_types.push_back( HAPI::DeviceLog::BUTTONS );
        log_types.push_back( HAPI::DeviceLog::FORCE );
        log_types.push_back( HAPI::DeviceLog::TORQUE );
        log_types.push_back( HAPI::DeviceLog::RAW_POSITION );
        log_types.push_back( HAPI::DeviceLog::RAW_ORIENTATION );
        log_types.push_back( HAPI::DeviceLog::RAW_VELOCITY );
        break;
      } else if( (*i) == "TIME" ) {
        log_types.push_back( HAPI::DeviceLog::TIME );
      } else if( (*i) == "TRACKER_POSITION" ) {
        log_types.push_back( HAPI::DeviceLog::POSITION );
      } else if( (*i) == "TRACKER_ORIENTATION" ) {
        log_types.push_back( HAPI::DeviceLog::ORIENTATION );
      } else if( (*i) == "TRACKER_VELOCITY" ) {
        log_types.push_back( HAPI::DeviceLog::VELOCITY );
      } else if( (*i) == "BUTTONS" ) {
        log_types.push_back( HAPI::DeviceLog::BUTTONS );
      } else if( (*i) == "FORCE" ) {
        log_types.push_back( HAPI::DeviceLog::FORCE );
      } else if( (*i) == "TORQUE" ) {
        log_types.push_back( HAPI::DeviceLog::TORQUE );
      } else if( (*i) == "RAW_POSITION" ) {
        log_types.push_back( HAPI::DeviceLog::RAW_POSITION );
      } else if( (*i) == "RAW_ORIENTATION" ) {
        log_types.push_back( HAPI::DeviceLog::RAW_ORIENTATION );
      } else if( (*i) == "RAW_VELOCITY" ) {
        log_types.push_back( HAPI::DeviceLog::RAW_VELOCITY );
      }
    }
    // Create an instance of the HAPI::DeviceLog class.
    log_force_effect.reset( new HAPI::DeviceLog( url_used,
                                                 log_types,
                                                 frequency->getValue(),
                                                 logBinary->getValue() ) );
  }
}

void DeviceLog::traverseSG( TraverseInfo &ti ) {
  // Add the instance of HAPI::DeviceLog to be rendered on the selected
  // haptics device.
  if( !ti.getHapticsDevices().empty()) {
    H3DInt32 device_index = deviceIndex->getValue();
    if( ti.hapticsEnabled( device_index ) && 
	device_index >= 0 && 
	(unsigned int)device_index < ti.getHapticsDevices().size() )
      ti.addForceEffect( device_index, log_force_effect.get() );
  }
}

