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
/// \file SpotLight.h
/// \brief Header file for SpotLight, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#include <H3D/X3DLightNode.h>
#include <H3D/SFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class SpotLight
  /// \brief The SpotLight node defines a light source that emits light from a
  /// specific point along a specific direction vector and constrained
  /// within a solid angle. 
  ///
  /// Spotlights may illuminate geometry nodes that
  /// respond to light sources and intersect the solid angle defined by the
  /// SpotLight. Spotlight nodes are specified in the local coordinate
  /// system and are affected by ancestors' transformations. 
  ///
  /// See X3DLightNode for a description of the ambientIntensity, color,
  /// and intensity fields. 
  /// 
  /// The location field specifies a translation offset of the centre point
  /// of the light source from the light's local coordinate system
  /// origin. This point is the apex of the solid angle which bounds light
  /// emission from the given light source. The direction field specifies
  /// the direction vector of the light's central axis defined in the local
  /// coordinate system. 
  ///
  /// The on field specifies whether the light source emits light. If on is
  /// TRUE, the light source is emitting light and may illuminate geometry
  /// in the scene. If on is FALSE, the light source does not emit light and
  /// does not illuminate any geometry. 
  ///
  /// Location is affected by ancestors' transformations.
  ///
  /// The cutOffAngle field specifies the outer bound of the solid
  /// angle. The light source does not emit light outside of this solid
  /// angle. The beamWidth field specifies an inner solid angle in
  /// which the light source emits light at uniform full intensity (in
  /// H3DAPI it can only be the entire beamWidth angle or 0). 
  /// If beamWidth is less than cutOffAngle then the intensity drops
  /// of linearly from the center of the spotlight to the beamWidth, i.e.
  /// the same as a beamWidth of 0. If the beamWidth is greater than 
  /// the cutOffAngle, beamWidth is defined to be equal to the 
  /// cutOffAngle and the light source emits full intensity within the 
  /// entire solid angle defined by cutOffAngle.
  ///
  /// SpotLight illumination falls off with distance as specified by three
  /// attenuation coefficients. The attenuation factor is: 
  ///
  ///  1/max(attenuation[0] + attenuation[1] × r + attenuation[2] × r2 , 1)
  ///
  /// where r is the distance from the light to the surface being
  /// illuminated. The default is no attenuation. An attenuation value of
  /// (0, 0, 0) is identical to (1, 0, 0). Attenuation values shall be
  /// greater than or equal to zero. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SpotLight.x3d">SpotLight.x3d</a>
  ///     ( <a href="examples/SpotLight.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SpotLight.dot
  class H3DAPI_API SpotLight : public X3DLightNode {
  public:

    /// Constructor.    
    SpotLight( Inst< SFNode >  _metadata         = 0,
               Inst< SFFloat>  _ambientIntensity = 0,
               Inst< SFColor>  _color            = 0,
               Inst< SFFloat>  _intensity        = 0,
               Inst< SFBool >  _global           = 0,
               Inst< SFBool >  _on               = 0,
               Inst< SFVec3f>  _attenuation      = 0,
               Inst< SFFloat>  _beamWidth        = 0,
               Inst< SFFloat>  _cutOffAngle      = 0,
               Inst< SFVec3f>  _direction        = 0,
               Inst< SFVec3f>  _location         = 0,
               Inst< SFFloat>  _radius           = 0 );

    /// Return the OpenGL parameters for this light.
    GLLightInfo getGLLightInfo();

    /// The attenuation field defines how the SpotLight node's 
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
    /// \dotfile SpotLight_attenuation.dot
    auto_ptr< SFVec3f >  attenuation;

    /// \brief The beamWidth field specifies an inner solid angle in
    /// which the light source emits light at uniform full intensity (in
    /// H3DAPI it can only be the entire beamWidth angle or 0). 
    /// If beamWidth is less than cutOffAngle then the intensity drops
    /// of linearly from the center of the spotlight to the beamWidth, i.e.
    /// the same as a beamWidth of 0. If the beamWidth is greater than 
    /// the cutOffAngle, beamWidth is defined to be equal to the 
    /// cutOffAngle and the light source emits full intensity within the 
    /// entire solid angle defined by cutOffAngle.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> pi/2 \n
    /// <b>Valid range:</b> [0-pi/2]
    /// 
    /// \dotfile SpotLight_beamWidth.dot
    auto_ptr< SFFloat >  beamWidth;

    /// The cutOffAngle field specifies the outer bound of the solid angle.
    /// The light source does not emit light outside of this solid angle.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> pi/4 \n
    /// <b>Valid range:</b> [0-pi/2]
    /// 
    /// \dotfile SpotLight_cutOffAngle.dot
    auto_ptr< SFFloat >  cutOffAngle;

    /// The direction field specifies the direction vector of the light's 
    /// central axis defined in the local coordinate system.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 -1 \n
    /// 
    /// \dotfile SpotLight_location.dot
    auto_ptr< SFVec3f >  direction;
    
    /// The location field specifies a translation offset of the centre
    /// point of the light source from the light's local coordinate system
    /// origin. This point is the apex of the solid angle which bounds light
    /// emission from the given light source.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 \n
    /// 
    /// \dotfile SpotLight_location.dot
    auto_ptr< SFVec3f >  location;

    /// This field is not supported by H3DAPI.
    auto_ptr< SFFloat >  radius;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
