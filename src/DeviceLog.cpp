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
  FIELDDB_ELEMENT( DeviceLog, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DeviceLog, frequency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DeviceLog, logBinary, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DeviceLog, logData, INPUT_OUTPUT );
}

DeviceLog::DeviceLog( Inst< SFNode> _metadata,
                      Inst< MFString     > _url,
                      Inst< SFInt32     > _frequency,
                      Inst< MFInt32  > _deviceIndex,
                      Inst< SFBool   > _logBinary,
                      Inst< MFString > _logData ):
  H3DForceEffect( _metadata, _deviceIndex ),
  X3DUrlObject(_url),
  frequency(_frequency),
  logBinary( _logBinary ),
  logData( _logData ),
  updateLogForceEffect( new Field ) {
  type_name = "DeviceLog";

  database.initFields( this );

  frequency->setValue( 100 );
  url->setValueFromString( "log.txt" );
  logBinary->setValue( false );
  logData->setValueFromString( "ALL" );

  url->route( updateLogForceEffect );
  frequency->route( updateLogForceEffect );
  deviceIndex->route( updateLogForceEffect );
  logBinary->route( updateLogForceEffect );
  logData->route( updateLogForceEffect );
}

void DeviceLog::traverseSG( TraverseInfo &ti ) {
  const vector< H3DInt32 > &device_index = deviceIndex->getValue();
  if( !updateLogForceEffect->isUpToDate() ) {
    updateLogForceEffect->upToDate();
    if( device_index.empty() ) {
      for( unsigned int i = 0; i < ti.getHapticsDevices().size(); ++i ) {
        createLogForceEffect( i );
      }
    } else {
      for( unsigned int i = 0; i < device_index.size(); ++i ) {
        createLogForceEffect( device_index[i] );
      }
    }
  }
  // Add the instance of HAPI::DeviceLog to be rendered on the selected
  // haptics device.
  if( !ti.getHapticsDevices().empty() ) {
    if( device_index.empty() ) {
      for( unsigned int i = 0; i < ti.getHapticsDevices().size(); ++i ) {
        if( ti.hapticsEnabled( i ) ) {
          ti.addForceEffect( i, log_force_effect[i] );
        }
      }
    } else {
      for( unsigned int i = 0; i < device_index.size(); ++i ) {
        if( device_index[i] >= 0 && ti.hapticsEnabled( device_index[i] )
            && log_force_effect[device_index[i]] ) {
          ti.addForceEffect( device_index[i], log_force_effect[device_index[i]] );
        }
      }
    }
  }
}

void DeviceLog::createLogForceEffect( int index ) {
  if( index < 0 ) {
    Console(3) << "Warning: Invalid index " << index
               << " in deviceIndex field in node "
               << getName() << ". Index is ignored." << endl;
    return;
  }
  if( index >= (int)log_force_effect.size() )
    log_force_effect.resize( index + 1, NULL );
  const vector< string > &urls = url->getValue();
  if( urls.empty() ) {
    Console(3) << "Warning: The url field in node "
               << getName() << " is empty. No logging can be done." << endl;
  } else if( index >= (int)urls.size() ) {
    Console(3) << "Warning: There is no url in the url field in node "
               << getName() << " for the given index " << index
               << " Each device needs a corresponding url in the url field."
               << endl;
  } else {
    string url_used = urls[index];
    // Go through the entries in the logData field and put it in a
    // LogTypeVector which will be used in the constructor of HAPI::DeviceLog.
    HAPI::DeviceLog::LogTypeVector log_types;
    const vector< string > &log_data = logData->getValue();
    for( vector< string >::const_iterator i = log_data.begin();
      i != log_data.end(); ++i ) {
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
      } else if( (*i) == "DEVICE_POSITION" ) {
        log_types.push_back( HAPI::DeviceLog::RAW_POSITION );
      } else if( (*i) == "DEVICE_ORIENTATION" ) {
        log_types.push_back( HAPI::DeviceLog::RAW_ORIENTATION );
      } else if( (*i) == "DEVICE_VELOCITY" ) {
        log_types.push_back( HAPI::DeviceLog::RAW_VELOCITY );
      }
    }
    // Create an instance of the HAPI::DeviceLog class.
    log_force_effect.set( index, new HAPI::DeviceLog( url_used,
                                                      log_types,
                                                      frequency->getValue(),
                                                      logBinary->getValue() ));
  }
}

