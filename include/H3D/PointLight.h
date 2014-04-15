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
/// \file PointLight.h
/// \brief Header file for PointLight, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

#include <H3D/X3DLightNode.h>
#include <H3D/SFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PointLight
  /// \brief The PointLight node specifies a point light source at a 3D 
  /// location in the local coordinate system. 
  ///
  /// A point light source emits light equally in all directions; that is,
  /// it is omnidirectional. PointLight nodes are specified in the local
  /// coordinate system and are affected by ancestor transformations.
  ///
  /// See X3DLightNode for a description of the ambientIntensity, color,
  /// and intensity fields. 
  ///
  /// Location is affected by ancestors' transformations (scales affect and
  /// transformations affect location). 
  ///
  /// PointLight node's illumination falls off with distance as specified by
  /// three attenuation coefficients. The attenuation factor is: 
  ///
  ///  1/max(attenuation[0] + attenuation[1] × r + attenuation[2] × r2, 1)
  ///
  /// where r is the distance from the light to the surface being
  /// illuminated. The default is no attenuation. An attenuation value of
  /// (0, 0, 0) is identical to (1, 0, 0). Attenuation values shall be
  /// greater than or equal to zero.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PointLight.x3d">PointLight.x3d</a>
  ///     ( <a href="examples/PointLight.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/PointLight1.x3d">PointLight1.x3d</a>
  ///     ( <a href="examples/PointLight1.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile PointLight.dot
  class H3DAPI_API PointLight : public X3DLightNode {
  public:
    /// Constructor.
    PointLight( Inst< SFNode >  _metadata         = 0,
                Inst< SFFloat>  _ambientIntensity = 0,
                Inst< SFColor>  _color            = 0,
                Inst< SFFloat>  _intensity        = 0,
                Inst< SFBool >  _on               = 0,
                Inst< SFVec3f>  _attenuation      = 0,
                Inst< SFVec3f>  _location         = 0,
                Inst< SFBool >  _global           = 0,
                Inst< SFFloat>  _radius           = 0 );

    /// Return the OpenGL parameters for this light.
    virtual GLLightInfo getGLLightInfo();

    /// Traversal function. Adding light to shadow caster.
    virtual void traverseSG( TraverseInfo &ti );

    /// The attenuation field defines how the PointLight node's 
    /// illumination falls off with distance from the light.
    /// The attenuation factor is: 
    ///
    ///  1/max(attenuation[0] + attenuation[1] × r + attenuation[2] × r2, 1)
    ///
    /// where r is the distance from the light to the surface being
    /// illuminated.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 0 0 \n
    /// <b>Valid range:</b> [0-inf]
    /// 
    /// \dotfile PointLight_attenuation.dot
    auto_ptr< SFVec3f >  attenuation;

    /// The location of the light source.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 \n
    /// 
    /// \dotfile PointLight_location.dot
    auto_ptr< SFVec3f >  location;

    /// This field is not supported by H3DAPI.
    auto_ptr< SFFloat >  radius;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
