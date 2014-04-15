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
/// \file Material.h
/// \brief Header file for Material, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <H3D/X3DMaterialNode.h>
#include <H3D/SFFloat.h>
#include <H3D/SFColor.h>

namespace H3D {

  /// \ingroup X3DNodes 
  /// \class Material
  /// \brief The Material node specifies surface material properties for 
  /// associated geometry nodes and is used by the X3D lighting equations
  /// during rendering. See the Lighting component of the X3D specification
  /// for a detailed description of the X3D lighting model equations.
  /// All of the fields in the Material node range from 0.0 to 1.0.
  /// 
  /// The fields in the Material node determine how light reflects off an
  /// object to create colour: 
  ///
  /// - The ambientIntensity field specifies how much ambient light from light
  /// sources this surface shall reflect. Ambient light is omnidirectional
  /// and depends only on the number of light sources, not their positions
  /// with respect to the surface. Ambient colour is calculated as
  /// ambientIntensity × diffuseColor.
  ///
  /// - The diffuseColor field reflects all X3D light sources depending on the
  /// angle of the surface with respect to the light source. The more
  /// directly the surface faces the light, the more diffuse light reflects.  
  ///
  /// - The emissiveColor field models "glowing" objects. This can be useful
  /// for displaying pre-lit models (where the light energy of the room is
  /// computed explicitly), or for displaying scientific data.  
  ///
  /// - The specularColor and shininess fields determine the specular
  /// highlights (e.g., the shiny spots on an apple). When the angle from
  /// the light to the surface is close to the angle from the surface to the
  /// viewer, the specularColor is added to the diffuse and ambient colour
  /// calculations. Lower shininess values produce soft glows, while higher
  /// values result in sharper, smaller highlights.  
  ///
  /// - The transparency field specifies how "clear" an object is, with 1.0
  /// being completely transparent, and 0.0 completely opaque.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Material.x3d">Material.x3d</a>
  ///     ( <a href="examples/Material.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Material.dot
  class H3DAPI_API Material : public X3DMaterialNode {
  public:

    /// Constructor
    Material( Inst< DisplayList > _displayList   = 0,
              Inst< SFFloat >  _ambientIntensity = 0,
              Inst< SFColor >  _diffuseColor     = 0,
              Inst< SFColor >  _emissiveColor    = 0,
              Inst< SFNode  >  _metadata         = 0,
              Inst< SFFloat >  _shininess        = 0,
              Inst< SFColor >  _specularColor    = 0,
              Inst< SFFloat >  _transparency     = 0 );

    /// Set up the material properties in OpenGL.
    virtual void render();

    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() { 
      return GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT; 
    }

    /// This function checks the transparency field to determine if the
    /// material requires that the geometry is rendered with transparency
    virtual bool isTransparent() {
      return transparency->getValue() > 0;
    }
    
    /// The ambientIntensity field specifies how much ambient light from light
    /// sources this surface shall reflect. Ambient light is omnidirectional
    /// and depends only on the number of light sources, not their positions
    /// with respect to the surface. Ambient colour is calculated as 
    /// ambientIntensity × diffuseColor.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.2 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile Material_ambientIntensity.dot
    auto_ptr< SFFloat >  ambientIntensity;

    /// The diffuseColor field reflects all light sources depending on the 
    /// angle of the surface with respect to the light source. The more 
    /// directly the surface faces the light, the more diffuse light reflects.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0.8, 0.8, 0.8 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile Material_diffuseColor.dot
    auto_ptr< SFColor >  diffuseColor;

    /// The emissiveColor field models "glowing" objects. This can be useful 
    /// for displaying pre-lit models (where the light energy of the room is
    /// computed explicitly), or for displaying scientific data.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0, 0, 0 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile Material_emissiveColor.dot
    auto_ptr< SFColor >  emissiveColor;
    
    /// Shininess effect the specular highlights. Lower shininess values 
    /// produce soft glows, while higher values result in sharper, smaller
    /// highlights.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.2 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile Material_shininess.dot
    auto_ptr< SFFloat >  shininess;

    /// The color of the specular highlights.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0, 0, 0 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile Material_specularColor.dot
    auto_ptr< SFColor >  specularColor;

    /// The transparency field specifies how "clear" an object is, with 
    /// 1.0 being completely transparent, and 0.0 completely opaque.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile Material_transparency.dot
    auto_ptr< SFFloat >  transparency;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
