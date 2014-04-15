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
/// \file GeneralSpotLight.h
/// \brief Header file for GeneralSpotLight, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GENERALSPOTLIGHT_H__
#define __GENERALSPOTLIGHT_H__

#include <H3D/SpotLight.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class GeneralSpotLight
  /// \brief The GeneralSpotLight node defines a light source that emits 
  /// light from a specific point along a specific direction vector and
  /// constrained within a solid angle. 
  /// 
  /// It allows different values for the  ambient, specular and diffuse 
  /// color parts of the light specification.
  /// 
  /// Specifically:
  /// diffuse = color * intensity
  /// ambient = ambientColor * ambientIntensity
  /// specular = specularColor * specularIntensity
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GeneralSpotLight.x3d">GeneralSpotLight.x3d</a>
  ///     ( <a href="examples/GeneralSpotLight.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile GeneralSpotLight.dot
  class H3DAPI_API GeneralSpotLight : public SpotLight {
  public:

    /// Constructor.    
    GeneralSpotLight( Inst< SFNode >  _metadata         = 0,
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
          Inst< SFFloat>  _radius           = 0,
          Inst< SFColor>  _ambientColor     = 0,
          Inst< SFColor>  _specularColor    = 0,
          Inst< SFFloat > _specularIntensity = 0 );

    /// Return the OpenGL parameters for this light.
    GLLightInfo getGLLightInfo();
    
    /// The color field specifies the spectral colour properties of the 
    /// ambient light emission as an RGB value.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1, 1, 1 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile GeneralSpotlight_ambientColor.dot
    auto_ptr< SFColor >  ambientColor;

    /// The color field specifies the spectral colour properties of the 
    /// specular light emission as an RGB value.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1, 1, 1 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile GeneralSpotlight_specularColor.dot
    auto_ptr< SFColor >  specularColor;

    /// Specifies the intensity of the specular emission from the light.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile GeneralSpotlight_specularIntensity.dot
    auto_ptr< SFFloat >  specularIntensity;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
