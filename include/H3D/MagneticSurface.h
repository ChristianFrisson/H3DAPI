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
/// \file MagneticSurface.h
/// \brief Header file for MagneticSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MAGNETICSURFACE_H__
#define __MAGNETICSURFACE_H__

#include <H3D/H3DSurfaceNode.h>
#include <H3D/SFFloat.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class MagneticSurface
  /// The MagneticSurface is a surface where the proxy is attracted to
  /// the surface, and forces are generated in order to keep the proxy 
  /// on the surface.
  ///
  /// The snapDistance field defines a distance in meters from the surface
  /// within which forces are generated to pull the proxy towards the
  /// surface. If the device is pulled outside this distance from the
  /// surface it will be freed from the magnetic attraction.
  ///
  /// TODO: This node only works for OpenHapticsRenderer. All values has
  /// to be given relative to maximum.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Surfaces.x3d">Surfaces.x3d</a>
  ///     ( <a href="examples/Surfaces.x3d.html">Source</a> )
  class H3DAPI_API MagneticSurface:  public H3DSurfaceNode {
  public:

    /// Specialized field which sets the stiffness variable in
    /// HAPI::OpenHapticsRenderer::OpenHapticsSurface when the
    /// stiffness field of SmoothSurface is changed.
    ///
    /// routes_in[0] is the stiffness field
    class H3DAPI_API UpdateStiffness:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Specialized field which sets the damping variable in
    /// HAPI::OpenHapticsRenderer::OpenHapticsSurface when the damping
    /// field of MagneticSurface is changed.
    ///
    /// routes_in[0] is the damping field
    class H3DAPI_API UpdateDamping:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Specialized field which sets the static_friction variable in
    /// HAPI::OpenHapticsRenderer::OpenHapticsSurface when the staticFriction
    /// field of MagneticSurface is changed.
    ///
    /// routes_in[0] is the staticFriction field
    class H3DAPI_API UpdateStaticFriction:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Specialized field which sets the dynamic_friction variable in
    /// HAPI::OpenHapticsRenderer::OpenHapticsSurface when the dynamicFriction
    /// field of MagneticSurface is changed.
    ///
    /// routes_in[0] is the dynamicFriction field
    class H3DAPI_API UpdateDynamicFriction:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Specialized field which sets the snap_distance variable in
    /// HAPI::OpenHapticsRenderer::OpenHapticsSurface when the snapDistance
    /// field of MagneticSurface is changed.
    ///
    /// routes_in[0] is the snapDistance field
    class H3DAPI_API UpdateSnapDistance:
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Constructor.
    MagneticSurface( Inst< UpdateStiffness       > _stiffness         = 0,
                     Inst< UpdateDamping         > _damping           = 0,
                     Inst< UpdateStaticFriction  > _staticFriction    = 0,
                     Inst< UpdateDynamicFriction > _dynamicFriction   = 0,
                     Inst< UpdateSnapDistance    > _snapDistance      = 0 );

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

    /// The distance from the surface within which forces are generated
    /// to pull the proxy towards the surface. If the device is pulled
    /// outside this distance from the surface it will be freed from the
    /// magnetic attraction.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> 0.01 \n
    auto_ptr< UpdateSnapDistance > snapDistance;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
