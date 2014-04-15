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
/// \file H3D/HapticTexturesSurface.h
/// \brief Header file for HapticTexturesSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HAPTICTEXTURESSURFACE_H__
#define __HAPTICTEXTURESSURFACE_H__

#include <H3D/H3DFrictionalSurfaceNode.h>
#include <H3D/X3DTexture2DNode.h>

#include <HAPI/HapticTexturesSurface.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class HapticTexturesSurface
  /// Surface in which textures can be used to control the values of the
  /// parameters stiffness, damping, staticFriction and dynamicFriction.
  /// It is optimized for once channel gray scale images using unsigned
  /// integers from 0 to 255. Black (0) corresponds
  /// to the lowest allowed value and White(255) corresponds to the highest
  /// allowed value.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Surfaces.x3d">Surfaces.x3d</a>
  ///     ( <a href="examples/Surfaces.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HapticTexturesSurface.dot
  class H3DAPI_API HapticTexturesSurface: public H3DFrictionalSurfaceNode {
  public:

    /// Field class that contains an X3DTexture2DNode. Need to override onAdd
    /// and onRemove in order to update the Image used by HAPI.
    class SFTexture2DNode : public DependentSFNode< X3DTexture2DNode, 
                                    FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                                    true > {
    public:

      typedef DependentSFNode< X3DTexture2DNode, 
                               FieldRef< H3DDisplayListObject,
                               H3DDisplayListObject::DisplayList,
                               &H3DDisplayListObject::displayList >, 
                               true >
        SFTexture2DNodeBase;

      /// Set up the routes we want to maintain.
      virtual void onAdd( Node *n );

      /// Remove the routes we want to maintain.
      virtual void onRemove( Node *n );

      HAPI::HapticTexturesSurface::ParameterType parameter_type;
    };

    /// Field class used to transfer the routed image to the HAPISurfaceObject.
    ///
    /// routes_in[0] is the image field in the X3DTexture2DNode in
    /// any of the fields stiffnessMap, dampingMap, staticFrictionMap
    /// and dynamicFrictionMap.
    class SetImagePtr : public AutoUpdate< 
      TypedField< SFBool, X3DTexture2DNode::SFImage,
                  AnyNumber< X3DTexture2DNode::SFImage > > > {
    public:
      HAPI::HapticTexturesSurface::ParameterType parameter_type;
    protected:
      virtual void update();
    };

    /// Field class used to set the min and max values of the different
    /// parameters in the HAPISurfaceObject.
    class UpdateMinMaxParamValue :
      public AutoUpdate< OnValueChangeSField< SFFloat > > {
    public:
      HAPI::HapticTexturesSurface::ParameterType parameter_type;
      bool max_value;
    protected:
      virtual void onValueChange( const H3DFloat &v );
    };

    /// Constructor.
    HapticTexturesSurface(
                     Inst< UpdateStiffness        > _stiffness          = 0,
                     Inst< UpdateDamping          > _damping            = 0,
                     Inst< UpdateStaticFriction   > _staticFriction     = 0,
                     Inst< UpdateDynamicFriction  > _dynamicFriction    = 0,
                     Inst< SFBool                 > _useRelativeValues  = 0,
                     Inst< SFTexture2DNode        > _stiffnessMap       = 0,
                     Inst< UpdateMinMaxParamValue > _maxStiffness       = 0,
                     Inst< UpdateMinMaxParamValue > _minStiffness       = 0,
                     Inst< SFTexture2DNode        > _dampingMap         = 0,
                     Inst< UpdateMinMaxParamValue > _maxDamping         = 0,
                     Inst< UpdateMinMaxParamValue > _minDamping         = 0,
                     Inst< SFTexture2DNode        > _staticFrictionMap  = 0,
                     Inst< UpdateMinMaxParamValue > _maxStaticFriction  = 0,
                     Inst< UpdateMinMaxParamValue > _minStaticFriction  = 0,
                     Inst< SFTexture2DNode        > _dynamicFrictionMap = 0,
                     Inst< UpdateMinMaxParamValue > _maxDynamicFriction = 0,
                     Inst< UpdateMinMaxParamValue > _minDynamicFriction = 0 );

    /// Initialize the variables. Creates the HAPISurfaceObject used for this
    /// this node.
    void initialize();

    /// The texture used to map stiffness values.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile HapticTexturesSurface_stiffnessMap.dot
    auto_ptr< SFTexture2DNode > stiffnessMap;

    /// The texture used to map damping values.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile HapticTexturesSurface_dampingMap.dot
    auto_ptr< SFTexture2DNode > dampingMap;

    /// The texture used to map static friction values.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile HapticTexturesSurface_staticFrictionMap.dot
    auto_ptr< SFTexture2DNode > staticFrictionMap;

    /// The texture used to map dynamic friction values.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile HapticTexturesSurface_dynamicFrictionMap.dot
    auto_ptr< SFTexture2DNode > dynamicFrictionMap;

    /// The maximum allowed value of the stiffness. Only used to calculate
    /// the stiffness if stiffnessMap is not null. The white in the texture
    /// will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile HapticTexturesSurface_maxStiffness.dot
    auto_ptr< UpdateMinMaxParamValue > maxStiffness;

    /// The maximum allowed value of the damping. Only used to calculate
    /// the damping if dampingMap is not null. The white in the texture
    /// will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile HapticTexturesSurface_maxDamping.dot
    auto_ptr< UpdateMinMaxParamValue > maxDamping;

    /// The maximum allowed value of the static friction. Only used to
    /// calculate the static friction if staticFrictionMap is not null.
    /// The white in the texture will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile HapticTexturesSurface_maxStaticFriction.dot
    auto_ptr< UpdateMinMaxParamValue > maxStaticFriction;

    /// The maximum allowed value of the dynamic friction. Only used to
    /// calculate the dynamic friction if dynamicFrictionMap is not null.
    /// The white in the texture will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile HapticTexturesSurface_maxDynamicFriction.dot
    auto_ptr< UpdateMinMaxParamValue > maxDynamicFriction;

    /// The minimum allowed value of the stiffness. Only used to calculate
    /// the stiffness if stiffnessMap is not null. The black in the texture
    /// will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile HapticTexturesSurface_minStiffness.dot
    auto_ptr< UpdateMinMaxParamValue > minStiffness;

    /// The minimum allowed value of the damping. Only used to calculate
    /// the damping if dampingMap is not null. The black in the texture
    /// will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile HapticTexturesSurface_minDamping.dot
    auto_ptr< UpdateMinMaxParamValue > minDamping;

    /// The minimum allowed value of the static friction. Only used to
    /// calculate the static friction if staticFrictionMap is not null.
    /// The black in the texture will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile HapticTexturesSurface_minStaticFriction.dot
    auto_ptr< UpdateMinMaxParamValue > minStaticFriction;

    /// The minimum allowed value of the dynamic friction. Only used to
    /// calculate the dynamic friction if dynamicFrictionMap is not null.
    /// The black in the texture will correspond to this value.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile HapticTexturesSurface_minDynamicFriction.dot
    auto_ptr< UpdateMinMaxParamValue > minDynamicFriction;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // Used to update the image used for the stiffness parameter
    // in HAPI::HapticTexturesSurface.
    auto_ptr< SetImagePtr > setStiffnessPtr;

    // Used to update the image used for the damping parameter
    // in HAPI::HapticTexturesSurface.
    auto_ptr< SetImagePtr > setDampingPtr;

    // Used to update the image used for the static_friction parameter
    // in HAPI::HapticTexturesSurface.
    auto_ptr< SetImagePtr > setStaticFrictionPtr;

    // Used to update the image used for the dynamic_friction parameter
    // in HAPI::HapticTexturesSurface.
    auto_ptr< SetImagePtr > setDynamicFrictionPtr;
  };
}

#endif
