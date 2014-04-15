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
/// \file X3DEnvironmentalSensorNode.h
/// \brief Header file for X3DEnvironmentalSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DENVIRONMENTALSENSORNODE_H__
#define __X3DENVIRONMENTALSENSORNODE_H__

#include <H3D/X3DSensorNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFTime.h>


namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DEnvironmentalSensorNode
  /// \brief This abstract node type is the base type for all sensors. 
  class H3DAPI_API X3DEnvironmentalSensorNode : public X3DSensorNode {
  public:

    /// Constructor.
    X3DEnvironmentalSensorNode( 
                Inst< SFNode >  _metadata   = 0,
                Inst< SFVec3f > _center     = 0,
                Inst< SFBool >  _enabled    = 0,
                Inst< SFVec3f > _size       = 0,
                Inst< SFTime >  _enterTime  = 0,
                Inst< SFTime >  _exitTime   = 0,
                Inst< SFBool >  _isActive   = 0 );


    // Fields
    /// Center gives the space location of the box
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> SFVec3f = 0,0,0 \n
    auto_ptr< SFVec3f > center;

    /// Size gives the size of the box (width=x, height=y, depth=z) 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> SFVec3f = 0,0,0 \n
    auto_ptr< SFVec3f > size;

    /// enterTime
    ///
    /// <b>Access type:</b> OutputOnly \n
    /// <b>Default value:</b> SFTime \n
    auto_ptr< SFTime > enterTime;
    
    /// exitTime
    ///
    /// <b>Access type:</b> OutputOnly \n
    /// <b>Default value:</b> SFTime \n
    auto_ptr< SFTime > exitTime;

  /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
