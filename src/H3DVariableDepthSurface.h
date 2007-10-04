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
/// \file H3DVariableDepthSurface.h
/// \brief Header file for H3DVariableDepthSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DVARIABLEDEPTHSURFACE_H__
#define __H3DVARIABLEDEPTHSURFACE_H__

#include <H3DSurfaceNode.h>
#include <FieldTemplates.h>
#include <SFFloat.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class H3DVariableDepthSurface
  /// Base class for all surfaces that use HAPIVariableDepthSurface
  /// as the contained HAPI surface.
  class H3DAPI_API H3DVariableDepthSurface: public H3DSurfaceNode {
  public:

    /// Specialized field which sets the stiffness variable in
    /// HAPIVariableDepthSurface when the stiffness field of
    /// H3DVariableDepthSurface is changed.
		///
    /// routes_in[0] is the stiffness field
    class H3DAPI_API UpdateStiffness: public AutoUpdate< SFFloat > {
    public:
      virtual void setValue( const H3DFloat &f, int id = 0 );

    protected:
      virtual void update();
    };

    /// Specialized field which sets the static_friction variable in
    /// HAPIVariableDepthSurface when the staticFriction field of 
    /// H3DVariableDepthSurface is changed.
		///
    /// routes_in[0] is the staticFriction field
    class H3DAPI_API UpdateStaticFriction: public AutoUpdate< SFFloat > {
    public:
      virtual void setValue( const H3DFloat &f, int id = 0 );

    protected:
      virtual void update();
    };

    /// Specialized field which sets the dynamic_friction variable in
    /// HAPIVariableDepthSurface when the dynamicFriction field of
    /// H3DVariableDepthSurface is changed.
		///
    /// routes_in[0] is the dynamicFriction field
    class H3DAPI_API UpdateDynamicFriction: public AutoUpdate< SFFloat > {
    public:
      virtual void setValue( const H3DFloat &f, int id = 0 );

    protected:
      virtual void update();
    };

    /// Constructor.
    H3DVariableDepthSurface(
                     Inst< UpdateStiffness       > _stiffness       = 0,
                     Inst< UpdateStaticFriction  > _staticFriction  = 0,
                     Inst< UpdateDynamicFriction > _dynamicFriction = 0 );
   
    /// The stiffness of the surface. Should be a value between 0 and 1
    /// where 1 is the maximum stiffness the haptics device can handle.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.3 \n
    /// <b>Value range: </b> [0-1]
    auto_ptr< UpdateStiffness > stiffness;

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