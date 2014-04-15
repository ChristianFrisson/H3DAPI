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
/// \file X3DSensorNode.h
/// \brief Header file for X3DSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSENSORNODE_H__
#define __X3DSENSORNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DSensorNode
  /// \brief This abstract node type is the base type for all sensors. 
  class H3DAPI_API X3DSensorNode : public X3DChildNode {
  public:
    
    /// Constructor.
    X3DSensorNode( Inst< SFBool >  _enabled  = 0,
                   Inst< SFNode >  _metadata = 0,
                   Inst< SFBool >  _isActive = 0 );


    /// If enabled is true the sensor node is enabled otherwise it is
    /// disabled and will not generate any output.
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< SFBool >  enabled;

    /// A sensor is active when some special conditions are fulfilled and
    /// it will then generate extra output.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr<  SFBool >  isActive;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
