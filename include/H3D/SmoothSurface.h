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
/// \file SmoothSurface.h
/// \brief Header file for SmoothSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SMOOTHSURFACE_H__
#define __SMOOTHSURFACE_H__

#include <H3D/H3DSurfaceNode.h>
#include <H3D/SFFloat.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class SmoothSurface
  /// A SmoothSurface has no friction at all. Only the stiffness of the surface
  /// can be specified. The stiffness is specified as a value between 0 and 1
  /// where 1 is the maximum stiffness the device can handle.
  ///
  class H3DAPI_API SmoothSurface: public H3DSurfaceNode {
  public:

    /// Specialized field which sets the stiffness variable in
    /// FrictionSurface when the stiffness field of SmoothSurface
    /// is changed.
    class H3DAPI_API UpdateStiffness: public AutoUpdate< SFFloat > {
    public:
      virtual void setValue( const H3DFloat &f, int id = 0 );

    protected:
      virtual void update();
    };

    /// Specialized field which sets the damping variable in
    /// FrictionSurface when the damping field of SmoothSurface
    /// is changed.
    class H3DAPI_API UpdateDamping: public AutoUpdate< SFFloat > {
    public:
      virtual void setValue( const H3DFloat &f, int id = 0 );

    protected:
      virtual void update();
    };

    /// Constructor.
    SmoothSurface( Inst< UpdateStiffness > _stiffness   = 0,
                   Inst< UpdateDamping   > _damping     = 0,
                   Inst< SFBool          > _mmStiffness = 0 );

    void initialize();

    /// The stiffness of the surface. Should be a value between 0 and 1
    /// where 1 is the maximum stiffness the haptics device can handle.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.5 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< UpdateStiffness > stiffness;

    /// The velocity based damping of the surface. Should be a value between
    /// 0 and 1 where 1 is the maximum damping the haptics device can handle.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< UpdateDamping > damping;

    /// Used for backwards compability. New applications should always
    /// have this set to false.
    ///
    /// <b>Access type: </b> initializeOnly \n
    /// <b>Default value: </b> false \n
    auto_ptr< SFBool > mmStiffness;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
