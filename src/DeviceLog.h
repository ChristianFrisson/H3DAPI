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
//
/// \file Node.h
/// \brief Node base class
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEVICE_LOG_H__
#define __DEVICE_LOG_H__

#include "X3DChildNode.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "TimeStamp.h"
using namespace std;

namespace H3D {

  /// \class DeviceLog
  /// \brief DeviceLog records haptic device position and orientation to a binary
  /// data file.
  class H3DAPI_API DeviceLog : public X3DChildNode {
  public:

    /// Contructor.
    DeviceLog( Inst< SFNode>  _metadata = 0,
               Inst< SFString > _filename = 0 ) :
      X3DChildNode( _metadata ),
      filename( _filename ) {
      log_file = 0;
      filename->setValue( "log.dat" );
    };
    
    /// Destructor.
    virtual ~DeviceLog() {
      closeLog();
    };
    
    virtual void openLog( );
    
    virtual void writeLog( Vec3f pos, Rotation rot );
    
    virtual void writeMessage( char *msg );
    
    virtual void closeLog();
    
    
    virtual string defaultXMLContainerField() {
      return "deviceLog";
    }
    
    auto_ptr< SFString > filename;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    FILE *log_file;
    double start_time;
  };

};

#endif
