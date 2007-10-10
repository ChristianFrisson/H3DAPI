//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DeviceLog.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DeviceLog::database( 
                                    "DeviceLog", 
                                    &(newInstance<DeviceLog>),
                                    typeid( DeviceLog ),
                                    &X3DChildNode::database );

namespace DeviceLogInternal {
  FIELDDB_ELEMENT( DeviceLog, filename, INPUT_OUTPUT );
}

void DeviceLog::openLog() {
	if ( log_file )
		closeLog();
	log_file = fopen( filename->getValue().c_str(), "w" );
  start_time = TimeStamp::now();
}

void DeviceLog::writeLog( Vec3f pos, Rotation rot ) {
	if ( log_file ) {
    double t = TimeStamp::now() - start_time;
    
		fwrite( &t, sizeof( t ), 1, log_file );

		// write position
		fwrite( &pos, sizeof( pos ), 1, log_file );

		// write orientation
		fwrite( &rot, sizeof( rot ), 1, log_file );
	}
}

void DeviceLog::writeMessage( char *msg ) {
	// unimplemented
}

void DeviceLog::closeLog() {
	if ( log_file ) {
		fclose( log_file );
		log_file = 0;
	}
}
