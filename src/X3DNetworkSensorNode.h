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
/// \file X3DNetworkSensorNode.h
/// \brief Header file for X3DNetworkSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DNETWORKSENSORNODE_H__
#define __X3DNETWORKSENSORNODE_H__

#include "X3DSensorNode.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DNetworkSensorNode
  /// \brief This abstract node type is the basis for all sensors that generate
  /// events based on network activity. 
  ///
  /// 
  class H3DAPI_API X3DNetworkSensorNode : public X3DSensorNode {
  public:
    
    /// Constructor.
    X3DNetworkSensorNode( Inst< SFBool>  _enabled  = 0,
                          Inst< SFNode>  _metadata = 0,
                          Inst< SFBool>  _isActive = 0 );
  };
}

#endif
