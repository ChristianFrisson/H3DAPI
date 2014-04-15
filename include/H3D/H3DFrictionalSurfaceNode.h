
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
/// \file H3DFrictionalSurfaceNode.h
/// \brief Header file for H3DFrictionalSurfaceNode
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFRICTIONALSURFACENODE_H__
#define __H3DFRICTIONALSURFACENODE_H__

#include <H3D/H3DStiffnessSurfaceNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DFrictionalSurfaceNode
  /// Base class for surfaces with friction.
  class H3DAPI_API H3DFrictionalSurfaceNode: public H3DStiffnessSurfaceNode {
  public:

    /// Specialized field which sets the static_friction variable in
    /// H3DFrictionalSurfaceNode when the staticFriction field of
    /// H3DFrictionalSurfaceNode is changed.
    class H3DAPI_API UpdateStaticFriction:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Specialized field which sets the dynamic_friction variable in
    /// H3DFrictionalSurfaceNode when the dynamicFriction field of 
    /// H3DFrictonalSurface is changed.
    class H3DAPI_API UpdateDynamicFriction:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Constructor.
    H3DFrictionalSurfaceNode(
                         Inst< UpdateStiffness >  _stiffness = 0,
                         Inst< UpdateDamping >  _damping   = 0,
                         Inst< UpdateStaticFriction >  _staticFriction = 0,
                         Inst< UpdateDynamicFriction >  _dynamicFriction = 0,
                         Inst< SFBool          > _useRelativeValues = 0 );

    /// The friction that is experienced upon initial movement when resting on 
    /// the surface.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.1 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< UpdateStaticFriction > staticFriction;

    /// The friction that is experienced when moving along the surface 
    /// the surface.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.4 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< UpdateDynamicFriction > dynamicFriction;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
