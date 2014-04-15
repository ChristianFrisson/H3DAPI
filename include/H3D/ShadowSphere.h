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
//    A commercia l license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file ShadowSphere.h
/// \brief Header file for ShadowSphere.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADOWSPHERE_H__
#define __SHADOWSPHERE_H__

#include <H3D/H3DShadowObjectNode.h>
#include <H3D/DirectionalLight.h>
#include <H3D/PointLight.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ShadowSphere
  /// The ShadowSphere object specifies a sphere casting a shadow for
  /// use in the ShadowCaster node.
  ///
  /// The radius field is the radius of the sphere and the position field
  /// is the position of the field.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ShadowCaster.x3d">ShadowCaster.x3d</a>
  ///     ( <a href="examples/ShadowCaster.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ShadowSphere.dot
  class H3DAPI_API ShadowSphere : public H3DShadowObjectNode {
  public:

    /// Constructor.
    ShadowSphere( Inst< SFNode > _metadata = 0,
                  Inst< SFFloat > _radius  = 0,
                  Inst< SFVec3f > _pos     = 0,
                  Inst< SFInt32 > _detailLevel = 0 );

    virtual void renderShadow( X3DLightNode *light, 
                               bool render_caps,
                               const Matrix4f &local_to_global = Matrix4f() );
 
    /// The radius of the sphere.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// \dotfile ShadowSphere_radius.dot
    auto_ptr< SFFloat > radius;

    /// The position of the sphere.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f(0,0,0) \n
    /// \dotfile ShadowSphere_position.dot
    auto_ptr< SFVec3f > position;

    /// The detailLevel field specifies the nr of faces to 
    /// use for the sides of the shadow cone/cylinder that
    /// this object produces. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 120 \n
    /// \dotfile ShadowSphere_detailLevel.dot
    auto_ptr< SFInt32 > detailLevel;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
