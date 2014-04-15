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
/// \file H3D/DepthMapSurface.h
/// \brief Header file for DepthMapSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEPTHMAPSURFACE_H__
#define __DEPTHMAPSURFACE_H__

#include <H3D/H3DFrictionalSurfaceNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/SFVec2d.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class DepthMapSurface
  /// Uses a 2D texture to modify the depth of the surface. For best
  /// performance the texture should be a 8-bit grayscale image. The user will
  /// be able to feel the edges between white and black in the surface using
  /// the haptics device and each gray-scale value will correspond to a depth
  /// that the user will feel. Non-gray-scale images will use the red channel
  /// to modify depth.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Surfaces.x3d">Surfaces.x3d</a>
  ///     ( <a href="examples/Surfaces.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile DepthMapSurface.dot
  class H3DAPI_API DepthMapSurface: public H3DFrictionalSurfaceNode {
  public:

    /// Field that contains an X3DTexture2DNode. Need to override onAdd
    /// and onRemove in order to update the Image used by HAPI.
    class H3DAPI_API SFTexture2DNode :
      public DependentSFNode< X3DTexture2DNode,
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
    };

    /// Field used to transfer the routed image to the HAPISurfaceObject.
    ///
    /// routes_in[0] is the image field in the X3DTexture2DNode in
    /// the depthMap field.
    class SetImagePtr : public AutoUpdate< 
      TypedField< SFBool, Types< X3DTexture2DNode::SFImage > > > {
    protected:
      virtual void update();
    };

    /// Constructor.
    DepthMapSurface( Inst< UpdateStiffness       > _stiffness       = 0,
                     Inst< UpdateDamping         > _damping         = 0,
                     Inst< UpdateStaticFriction  > _staticFriction  = 0,
                     Inst< UpdateDynamicFriction > _dynamicFriction = 0,
                     Inst< SFBool                > _useRelativeValues = 0,
                     Inst< SFTexture2DNode       > _depthMap        = 0,
                     Inst< SFFloat               > _maxDepth        = 0,
                     Inst< SFBool                > _whiteIsOut      = 0 );
   
    /// Initialize the variables. Creates the HAPISurfaceObject used for this
    /// this node.
    virtual void initialize();

    /// The texture used as depthMap.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile DepthMapSurface_depthMap.dot
    auto_ptr< SFTexture2DNode > depthMap;

    /// The maximum difference in meter between totally black and totally white in
    /// the image.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0.001 \n
    /// 
    /// \dotfile DepthMapSurface_maxDepth.dot
    auto_ptr< SFFloat > maxDepth;

    /// If this parameter is true then white will be out from surface
    /// and black will be in. If the parameter is false the it is the opposite.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> true \n
    ///
    /// \dotfile DepthMapSurface_whiteIsOut.dot
    auto_ptr< SFBool > whiteIsOut;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  protected:
    /// Used to update the image pointer in HAPISurfaceObject.
    auto_ptr< SetImagePtr > setImagePtr;
  };
}

#endif
