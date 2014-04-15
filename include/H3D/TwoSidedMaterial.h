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
/// \file TwoSidedMaterial.h
/// \brief Header file for TwoSidedMaterial, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TWOSIDEDMATERIAL_H__
#define __TWOSIDEDMATERIAL_H__

#include <H3D/X3DMaterialNode.h>
#include <H3D/SFFloat.h>
#include <H3D/SFColor.h>

namespace H3D {

  /// \ingroup X3DNodes 
  /// \class TwoSidedMaterial
  /// \brief This node defines material properties that can effect both the
  /// front and back side of a polygon individually. 
  /// These materials are used for both the front and back side of the geometry
  /// whenever the X3D lighting model is active. 
  ///
  /// If the separateBackColor field is set to TRUE, then the rendering shall
  /// render the front and back faces of the geometry with different values.
  /// If the value is FALSE, then the front colours are used for both the front
  /// and back side of the polygon, as per the existing X3D lighting rules. 
  ///
  /// When calculating the terms in the lighting equations, the front geometry
  /// shall use the fields ambientIntensity, diffuseColor,emissiveColor, 
  /// shininess, specularColor, and transparency. The faces that are determined
  /// to be the back side are rendered using backAmbientIntensity, 
  /// backDiffuseColor, backEmissiveColor, backShininess, and backTransparency
  /// as the appropriate components in the lighting equations.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TwoSidedMaterial.x3d">TwoSidedMaterial.x3d</a>
  ///     ( <a href="examples/TwoSidedMaterial.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile TwoSidedMaterial.dot
  class H3DAPI_API TwoSidedMaterial : public X3DMaterialNode {
  public:

    /// Constructor
    TwoSidedMaterial( Inst< DisplayList > _displayList   = 0,
                      Inst< SFFloat >  _ambientIntensity = 0,
                      Inst< SFColor >  _diffuseColor     = 0,
                      Inst< SFColor >  _emissiveColor    = 0,
                      Inst< SFNode  >  _metadata         = 0,
                      Inst< SFFloat >  _shininess        = 0,
                      Inst< SFColor >  _specularColor    = 0,
                      Inst< SFFloat >  _transparency     = 0,
                      Inst< SFFloat >  _backAmbientIntensity = 0,
                      Inst< SFColor >  _backDiffuseColor     = 0,
                      Inst< SFColor >  _backEmissiveColor    = 0,
                      Inst< SFFloat >  _backShininess        = 0,
                      Inst< SFColor >  _backSpecularColor    = 0,
                      Inst< SFFloat >  _backTransparency     = 0,
                      Inst< SFBool  > _separateBackColor     = 0);

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
      return transparency->getValue() > 0 ||
        (separateBackColor->getValue() && backTransparency->getValue() > 0);
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
    /// \dotfile TwoSidedMaterial_ambientIntensity.dot
    auto_ptr< SFFloat >  ambientIntensity;

    /// The diffuseColor field reflects all light sources depending on the 
    /// angle of the surface with respect to the light source. The more 
    /// directly the surface faces the light, the more diffuse light reflects.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0.8, 0.8, 0.8 ) \n
    /// <b>Valid range:</b> [0-1, 0-1, 0-1]
    /// 
    /// \dotfile TwoSidedMaterial_diffuseColor.dot
    auto_ptr< SFColor >  diffuseColor;

    /// The emissiveColor field models "glowing" objects. This can be useful 
    /// for displaying pre-lit models (where the light energy of the room is
    /// computed explicitly), or for displaying scientific data.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0, 0, 0 ) \n
    /// <b>Valid range:</b> [0-1, 0-1, 0-1]
    /// 
    /// \dotfile TwoSidedMaterial_emissiveColor.dot
    auto_ptr< SFColor >  emissiveColor;
    
    /// Shininess effect the specular highlights. Lower shininess values 
    /// produce soft glows, while higher values result in sharper, smaller
    /// highlights.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.2 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile TwoSidedMaterial_shininess.dot
    auto_ptr< SFFloat >  shininess;

    /// The color of the specular highlights.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0, 0, 0 ) \n
    /// <b>Valid range:</b> [0-1, 0-1, 0-1]
    /// 
    /// \dotfile TwoSidedMaterial_specularColor.dot
    auto_ptr< SFColor >  specularColor;

    /// The transparency field specifies how "clear" an object is, with 
    /// 1.0 being completely transparent, and 0.0 completely opaque.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile TwoSidedMaterial_transparency.dot
    auto_ptr< SFFloat >  transparency;

    /// The backAmbientIntensity is the same as the ambientIntensity field 
    /// but will be used for the back side of the geometry if separateBackColor
    /// is true;
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.2 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile TwoSidedMaterial_backAmbientIntensity.dot
    auto_ptr< SFFloat >  backAmbientIntensity;

    /// The backDiffuseColor is the same as the diffuseColor field 
    /// but will be used for the back side of the geometry if separateBackColor
    /// is true;
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0.8, 0.8, 0.8 ) \n
    /// <b>Valid range:</b> [0-1, 0-1, 0-1]
    /// 
    /// \dotfile TwoSidedMaterial_backDiffuseColor.dot
    auto_ptr< SFColor >  backDiffuseColor;


    /// The backEmissiveColor is the same as the emissiveColor field 
    /// but will be used for the back side of the geometry if separateBackColor
    /// is true;
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0, 0, 0 ) \n
    /// <b>Valid range:</b> [0-1, 0-1, 0-1]
    /// 
    /// \dotfile TwoSidedMaterial_backEmissiveColor.dot
    auto_ptr< SFColor >  backEmissiveColor;
    
    /// The backShininess is the same as the shininess field 
    /// but will be used for the back side of the geometry if separateBackColor
    /// is true;
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.2 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile TwoSidedMaterial_backShininess.dot
    auto_ptr< SFFloat >  backShininess;


    /// The backSpecularColor is the same as the specularColor field 
    /// but will be used for the back side of the geometry if separateBackColor
    /// is true;
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 0, 0, 0 ) \n
    /// <b>Valid range:</b> [0-1, 0-1, 0-1]
    /// 
    /// \dotfile TwoSidedMaterial_backSpecularColor.dot
    auto_ptr< SFColor >  backSpecularColor;

    /// The backTransparency is the same as the transparency field 
    /// but will be used for the back side of the geometry if separateBackColor
    /// is true.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile TwoSidedMaterial_backTransparency.dot
    auto_ptr< SFFloat >  backTransparency;

    /// If the separateBackColor field is set to TRUE, then the rendering shall
    /// render the front and back faces of the geometry with different values.
    /// If the value is FALSE, then the front colours are used for both the 
    /// front and back side of the polygon,
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile TwoSidedMaterial_separateBackColor.dot
    auto_ptr< SFBool >  separateBackColor;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
