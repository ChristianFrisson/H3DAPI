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
/// \file GeneralDirectionalLight.h
/// \brief Header file for GeneralDirectionalLight.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GENERALDIRECTIONALLIGHT_H__
#define __GENERALDIRECTIONALLIGHT_H__

#include <H3D/DirectionalLight.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class GeneralDirectionalLight
  /// \brief The GeneralDirectionalLight node defines a directional light
  /// source that illuminates along rays parallel to a given 3-dimensional
  /// vector. 
  ///
  /// It allows different values for the  ambient, specular and diffuse 
  /// color parts of the light specification.
  ///
  /// The direction field specifies the direction vector of the illumination
  /// emanating from the light source in the local coordinate system. Light
  /// is emitted along parallel rays from an infinite distance away. A
  /// directional light source illuminates only the objects in its enclosing
  /// parent group. The light may illuminate everything within this
  /// coordinate system, including all children and descendants of its
  /// parent group. The accumulated transformations of the parent nodes
  /// affect the light. 
  ///
  /// GeneralDirectionalLight nodes do not attenuate with distance. 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GeneralDirectionalLight.x3d">GeneralDirectionalLight.x3d</a>
  ///     ( <a href="examples/GeneralDirectionalLight.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile GeneralDirectionalLight.dot
  class H3DAPI_API GeneralDirectionalLight : public DirectionalLight {
  public:
    
    /// Constructor.
    GeneralDirectionalLight( Inst< SFNode >  _metadata         = 0,
                             Inst< SFFloat>  _ambientIntensity = 0,
                             Inst< SFColor>  _color            = 0,
                             Inst< SFFloat>  _intensity        = 0,
                             Inst< SFBool >  _on               = 0,
                             Inst< SFBool >  _global           = 0,
                             Inst< SFVec3f>  _direction        = 0,
                             Inst< SFColor>  _ambientColor     = 0,
                             Inst< SFColor>  _specularColor    = 0,
                             Inst< SFFloat > _specularIntensity = 0 );

    /// Return the OpenGL parameters for this light.
    virtual GLLightInfo getGLLightInfo();
    
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
