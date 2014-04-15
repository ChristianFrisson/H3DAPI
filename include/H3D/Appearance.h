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
/// \file Appearance.h
/// \brief Header file for Appearance, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __APPEARANCE_H__
#define __APPEARANCE_H__

#include <H3D/X3DAppearanceNode.h>
#include <H3D/FillProperties.h>
#include <H3D/LineProperties.h>
#include <H3D/RenderProperties.h>
#include <H3D/X3DMaterialNode.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/X3DTextureTransformNode.h>
#include <H3D/X3DShaderNode.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \ingroup X3DNodes 
  /// \class Appearance
  /// \brief The Appearance node specifies the visual properties of geometry.
  /// The value for each of the fields in this node may be NULL. However,
  /// if the field is non-NULL, it shall contain one node of the appropriate
  /// type.
  ///
  /// - The material field, if specified, shall contain a Material node. If
  ///   the material field is NULL or unspecified, lighting is off (all lights
  ///   are ignored during rendering of the object that references this
  ///   Appearance) and the unlit object colour is (1, 1, 1). 
  ///
  /// 
  /// - The texture field, if specified, shall contain one of the various
  ///   types of X3DTextureNode nodes . If the texture node is NULL or the
  ///   texture field is unspecified, the object that references this 
  ///   Appearance is not textured. 
  ///
  /// - The textureTransform field, if specified, shall contain a
  ///   TextureTransform node. If the textureTransform is NULL or unspecified,
  ///   the textureTransform field has no effect. 
  ///
  /// - The lineProperties field, if specified, shall contain a LineProperties
  ///   node. If lineProperties is NULL or unspecified, the lineProperties 
  ///   field has no effect. 
  ///
  /// - The fillProperties field, if specified, shall contain a FillProperties
  ///   node. If fillProperties is NULL or unspecified, the fillProperties 
  ///   field has no effect.
  ///
  /// - The shaders field contains a listing, in order of preference, of nodes
  ///   that describe programmable shaders that replace the fixed rendering 
  ///   requirements with user-provided functionality. If the field is not
  ///   empty, one shader node is selected and the fixed rendering requirements
  ///   defined by this specification are ignored.
  ///
  /// - The renderProperties field, if specified, shall contain a 
  ///   RenderProperties node. If renderProperties is NULL or unspecified, 
  ///   the renderProperties field has no effect (this field is not part of the
  ///   X3D specification)
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FillProperties.x3d">FillProperties.x3d</a>
  ///     ( <a href="examples/FillProperties.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/LineProperties.x3d">LineProperties.x3d</a>
  ///     ( <a href="examples/LineProperties.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/Material.x3d">Material.x3d</a>
  ///     ( <a href="examples/Material.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/TwoSidedMaterial.x3d">TwoSidedMaterial.x3d</a>
  ///     ( <a href="examples/TwoSidedMaterial.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties.x3d">RenderProperties.x3d</a>
  ///     ( <a href="examples/RenderProperties.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Appearance.dot
  class H3DAPI_API Appearance : public X3DAppearanceNode {
  public:
    /// The SFFillProperties field is dependent on the displayList field
    /// of the containing FillProperties node.
    typedef DependentSFNode< FillProperties, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFFillProperties;

    /// The SFLineProperties field is dependent on the displayList field
    /// of the containing LineProperties node.
    typedef DependentSFNode< LineProperties, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFLineProperties;

    /// The SFMaterialNode field is dependent on the displayList field
    /// of the containing X3DMaterialNode.
    typedef DependentSFNode< X3DMaterialNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFMaterialNode;
    
    /// The SFTextureTransformNode field is dependent on the displayList field
    /// of the containing X3DTextureTransformNode node.
    typedef DependentSFNode< X3DTextureTransformNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTextureTransformNode;

    /// The SFTextureNode field is dependent on the displayList field
    /// of the containing X3DTextureNode node.
    typedef DependentSFNode< X3DTextureNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTextureNode;

    /// The MFShaderNode field is dependent on the displayList field
    /// of the containing X3DShaderNode node.
    typedef DependentMFNode< X3DShaderNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    MFShaderNode;

    /// The SFRenderProperties field is dependent on the displayList field
    /// of the containing X3DShaderNode node.
    typedef DependentSFNode< RenderProperties, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFRenderProperties;

    /// Constructor.
    Appearance( Inst< DisplayList            > _displayList = 0,
                Inst< SFFillProperties       > _fillProperties   = 0,
                Inst< SFLineProperties       > _lineProperties   = 0,
                Inst< SFMaterialNode         > _material         = 0,
                Inst< SFNode                 > _metadata         = 0,
                Inst< SFTextureNode          > _texture          = 0,
                Inst< SFTextureTransformNode > _textureTransform = 0,
                Inst< SFSurface              > _surface          = 0,
                Inst< MFShaderNode           > _shaders          = 0,
                Inst< SFRenderProperties     > _renderProperties = 0,
                Inst< SFBool                 > _shadow           = 0);

    /// Set up the appearance in OpenGL.
    virtual void render();

    /// This function will be called by the X3DShapeNode before any rendering 
    /// of geometry and before the call to the render function. 
    virtual void preRender();

    /// This function will be called by the X3DShapeNode after the geometry
    /// has been rendered to restore the states to what it was before 
    /// the call to preRender().
    virtual void postRender();

    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs();

    virtual void traverseSG( TraverseInfo &ti ); 

    /// This function checks the transparency field to determine if the
    /// material requires that the geometry is rendered with transparency
    virtual bool isTransparent();

    /// This function return true if the appearance has decided that
    /// shadow should be cast. 
    virtual bool hasGeometryShadow() {
      return shadow->getValue();
    }

    /// This function checks if multi-pass transparency should be used or not
    /// (see RenderProperties_multiPassTransparency)
    virtual bool usingMultiPassTransparency() {
      RenderProperties *rp = renderProperties->getValue();
      if ( rp )
        return rp->multiPassTransparency->getValue();
      return default_using_multi_pass_transparency;
    }
    
    /// If specified it contains a FillProperties node that specifies
    /// additional properties to be applied to all polygonal areas.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_fillProperties.dot
    auto_ptr< SFFillProperties >  fillProperties;

    ///  If specified it contains a LineProperties node that specifies
    /// additional properties to be applied to all line geometry. 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_lineProperties.dot
    auto_ptr< SFLineProperties >  lineProperties;

    /// The material field, if specified, shall contain a Material node. If
    /// the material field is NULL or unspecified, lighting is off (all lights
    /// are ignored during rendering of the object that references this
    /// Appearance) and the unlit object colour is (1, 1, 1). 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_material.dot
    auto_ptr< SFMaterialNode >  material;

    /// Contains a X3DTextureNode to be used in the texturing.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_texture.dot
    auto_ptr< SFTextureNode >  texture;

    /// Contains a X3DTextureTransformNode that specifies a transformation
    /// of texture coordinates.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_textureTransform.dot
    auto_ptr< SFTextureTransformNode >  textureTransform;

    /// The shaders field defines the collection of shaders of various 
    /// languages in the order of preference. The first node declared 
    /// is the highest preference. If the language is not supported for
    /// the current preference level, the X3DShaderNode's isSelected 
    /// field is set to false, and the next shader is checked. 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_shaders.dot
    auto_ptr< MFShaderNode >  shaders;

    /// The renderProperties field, if specified, shall contain a 
    /// RenderProperties node. If renderProperties is NULL or unspecified, 
    /// the renderProperties field has no effect (this field is not part of the
    /// X3D specification)
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Appearance_renderProperties.dot
    auto_ptr< SFRenderProperties > renderProperties;

    /// The shadow field specifies if the geometry this Appearance is used
    /// on should cast a shadow or not. 
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> false
    /// 
    /// \dotfile Appearance_shadow.dot
    auto_ptr< SFBool > shadow;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
