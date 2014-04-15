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
/// \file X3DDragSensorNode.h
/// \brief Header file for X3DDragSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DDRAGSENSORNODE_H__
#define __X3DDRAGSENSORNODE_H__

#include <H3D/X3DPointingDeviceSensorNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DDragSensorNode
  /// \brief This abstract node type is the base type for all drag-style
  /// pointing device  sensors.
  
  class H3DAPI_API X3DDragSensorNode : 
    public X3DPointingDeviceSensorNode {
  public:

    /// Constructor.
    X3DDragSensorNode( Inst< SFBool >  _autoOffset = 0,
                       Inst< SFString > _description = 0,
                       Inst< SFBool >  _enabled  = 0,
                       Inst< SFNode >  _metadata = 0,
                       Inst< SFBool >  _isActive = 0,
                       Inst< SFBool > _isOver = 0,
                       Inst< SFVec3f >  _trackPoint_changed = 0 );

    // fields
    /// If autoOffset is TRUE when the pointing-device is deactivated,
    /// the offset field is set to the sensor's last <value>_changed
    /// value and offset sends an offset_changed output event. This enables
    /// subsequent grabbing operations to accumulate the changes. If autoOffset
    /// is FALSE, the sensor does not set the offset field value at
    /// deactivation (or any other time).
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFBool > autoOffset;

    /// The trackPoint_changed outputOnly field sends the intersection point of
    /// the bearing with the drag sensor's virtual geometry.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec3f >  trackPoint_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // Clamp n in the range [min, max]
    H3DFloat Clamp( H3DFloat n, H3DFloat min, H3DFloat max );
    // Check to send error message only once each time the user drags
    // the mouse pointer outside cylinder.
    bool send_warning_message;
  };
}

#endif
