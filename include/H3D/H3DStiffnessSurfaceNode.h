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
/// \file H3DStiffnessSurfaceNode.h
/// \brief Header file for H3DStiffnessSurfaceNode
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSTIFFNESSSURFACENODE_H__
#define __H3DSTIFFNESSSURFACENODE_H__

#include <H3D/H3DSurfaceNode.h>
#include <H3D/SFFloat.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DStiffnessSurfaceNode
  /// A H3DStiffnessSurfaceNode is the base class for surfaces that have
  /// stiffness and damping. If the field useRelativeValues is true then
  /// stiffness is specified as a value between 0 and 1 where 1 is the
  /// maximum stiffness the device can handle. Otherwise the stiffness is
  /// in N/m.
  ///
  class H3DAPI_API H3DStiffnessSurfaceNode: public H3DSurfaceNode {
  public:

    /// Specialized field which sets the stiffness variable in
    /// H3DStiffnessSurfaceNode when the stiffness field of
    /// H3DStiffnessSurfaceNode is changed.
    class H3DAPI_API UpdateStiffness:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Specialized field which sets the damping variable in
    /// H3DStiffnessSurfaceNode when the damping field of
    /// H3DStiffnessSurfaceNode is changed.
    class H3DAPI_API UpdateDamping:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Constructor.
    H3DStiffnessSurfaceNode( Inst< UpdateStiffness > _stiffness   = 0,
                             Inst< UpdateDamping   > _damping     = 0,
                             Inst< SFBool          > _useRelativeValues = 0 );

    /// The stiffness of the surface.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.5 \n
    auto_ptr< UpdateStiffness > stiffness;

    /// The velocity based damping of the surface.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0 \n
    auto_ptr< UpdateDamping > damping;

    /// If false then values (such as stiffness) is in absolute values with
    /// SI units or equivalent. If true the units are relative to the maximum
    /// values that the haptics device, on which the surface is rendered on,
    /// can handle.
    ///
    /// <b>Access type: </b> initializeOnly \n
    /// <b>Default value: </b> true \n
    auto_ptr< SFBool > useRelativeValues;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
