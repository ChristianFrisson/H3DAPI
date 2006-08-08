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
/// \file LoadSensor.h
/// \brief Header file for LoadSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LOADSENSOR_H__
#define __LOADSENSOR_H__

#include "X3DNetworkSensorNode.h"
#include "X3DUrlObject.h"
#include "MFNode.h"
#include "SFTime.h"
#include "SFFloat.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class LoadSensor
  ///
  /// 
  class H3DAPI_API LoadSensor : public X3DNetworkSensorNode {
  public:
    
    typedef TypedMFNodeObject< X3DUrlObject > MFUrlObject;  

    /// Constructor.
    LoadSensor( Inst< SFBool > _enabled  = 0,
                Inst< SFNode > _metadata = 0,
                Inst< SFBool > _isActive = 0,
                Inst< SFTime > _timeOut  = 0,
                Inst< MFUrlObject > _watchList = 0,
                Inst< SFBool > isLoaded = 0,
                Inst< SFTime > loadTime = 0,
                Inst< SFFloat > progress = 0 );

#ifdef USE_HAPTICS
    virtual void traverseSG( TraverseInfo &ti );
#endif

    auto_ptr< MFUrlObject > watchList;
    auto_ptr< SFTime  > timeOut;
    auto_ptr< SFBool  > isLoaded;
    auto_ptr< SFTime  > loadTime;
    auto_ptr< SFFloat > progress;

    static H3DNodeDatabase database;
  protected:
    bool loading_last_loop;
    bool finished_loading;
    TimeStamp activation_time;

  };
}

#endif
