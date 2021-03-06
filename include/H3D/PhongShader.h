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
/// \file PhongShader.h
/// \brief Header file for PhongShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PHONGSHADER_H__
#define __PHONGSHADER_H__

#include <H3D/H3DGeneratedFragmentShaderNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/SFMatrix4f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class PhongShader
  /// \brief The PhongShader node defines a shader for using the Phong
  /// illumination model.
  ///
  /// Restrictions: The PhongShader can only be used on geometries that
  /// have had their texture coordinates specified with glTexCoord calls.
  /// That means that geometries which have there texture coordinates 
  /// generated by a TextureCoordinateGenerator or default coordinates
  /// based on the bounding box(e.g. IndexedFaceSet without texCoord
  /// specified) will not work.
  /// In order to use tangent space normal maps you must provide vertex
  /// attributes for tangent and binormal(see below). This means that
  /// only geometries that allow specification of vertex attributes can be
  /// used as geometry then, e.g. all X3DComposedGeometryNode nodes. For
  /// IndexedTriangleSet nodes these will be automatically generated but for
  /// all other node they have to be set explicitly by the modeller.
  ///
  /// For each light we have
  /// C = att(d)*(Ma * La + Md*Ld*(L dot N) + Fs*Ms*Ls*(R dot V )^Mshininess)
  /// where Ma, Md, Ms and Mshininess are the ambient, diffuse, specular
  /// and shininess values for a point and La, Ld and Ls are the same
  /// for the light. 
  /// 
  /// L is the direction from the point to the light.
  /// N is the normal at the point.
  /// V is the direction from the point to the viewer.
  /// R is the reflected light direction.
  /// att(d) is the attenuation of the light based on the constant, linear,
  /// and quadratic attenuation parameters set for each light based on the 
  /// distance d from the light.
  /// Fs is the fresnel term and is calculated by
  ///    Fs = 1-fresnel + fresnel * (1-N dot V)^5
  ///
  /// The final color is the sum of all lights color contributions and 
  /// the emissive color of the material(Me).
  /// 
  /// Ma, Md, Me, Ms and Mshininess are by default the values specified by 
  /// an X3DMaterialNode. Instead of just using these constant values
  /// each of these can be controlled by a texture instead allowing
  /// different values at different parts of the 3d object. 
  /// Ma is controlled by ambientMap, Md by diffuseMap, Me by emissionMap
  /// Ms by specularMap and Mshininess by glossMap. 
  /// 
  /// The specularColorRamp and diffuseColorRamp can be used to control
  /// the color gradient across a material. It contains a 1D texture that
  /// defines the color ramp and the value used for lookup is controlled by 
  /// the specularColorRampMode and diffuseColorRampMode fields. 
  /// The looked up color is then multiplied 
  /// with the diffuse or specular color respectively to generate the 
  /// final diffuse or specular color. This allows for different colors
  /// depending on the view angle to the surface.
  ///
  /// The specularColorRampMode and diffuseColorRampMode defines what
  /// value should be used to lookup into the color ramp textures.
  /// 
  /// Valid values are:
  /// - "LIGHT" - light direction dot normal
  /// - "CAMERA" - camera direction dot normal
  ///
  /// If modulateMaps is FALSE the color from the map is used directly
  /// in all texture maps except for Mshininess which is a floating point
  /// value. In this case for a 1 or 2 component image, we use the intensity
  /// and for RGB and RGBA texture the R component is used.
  ///
  /// If modulateMaps is TRUE the values are modulated with the material 
  /// value, e.g. Md = Material diffuse * diffuseMap value.
  /// 
  /// The fresnel field specifies if the fresnel effect should be used in the
  /// lighting model and if so how much. The fresnel effect is the effect
  /// that surfaces reflect more light if looking at them from an angle than
  /// straight down along the normal. If set to 0 no fresnel effect is used
  /// and the surface is fully reflective. If set to 1 only the reflectivity
  /// of the fresnel effect is used, which means no reflectivity when looking
  /// down perpendicular to the surace. One can think of the term as controlling
  /// the minimum reflectivness of the surface. By default it is set to 0 and
  /// no fresnel effect is used.
  /// The fresnel value only affects the specular component of the Phong
  /// lighting calculations since this is the only part that deals with
  /// reflective properties. 
  ///
  /// The normal N can also be modified across the surface of a 3d object
  /// using a normalMap texture. The normalMap encodes the normal at each
  /// point of an object in the RGB value of an image. This allows for 
  /// adding detailed features without increasing polygon count.
  /// 
  /// Since a texture holds values between [0,1] and normals have values
  /// between [-1, 1] we need to translate the RGB values of the texture
  /// to the correct normal values.
  /// The matrix in the normalMapMatrix performes this translation. The
  /// normal value is the matrix multiplied by the RGB value. By default 
  /// the mapping is for each component x = R*2 - 1, transforming 0 to -1
  /// and 1 to 1. 
  /// Normals can be defined in different coordinate spaces. The 
  /// normalMapCoordSpace defines which one is used:
  /// - "OBJECT" - the normals are in the coordinate space of the object.
  /// This means that the normals will be correct as long as the object
  /// does not change the local position of its vertices.
  /// - "TANGENT" - the normals are specified in the tangent coordinate 
  /// space with the normal as z-axis and the texture coordinate axis 
  /// directions as the other two axis. These axis are called tangent
  /// and binormal and have to be provided as a 3 component 
  /// FloatVertexAttribute for each vertex by the geometry used. 
  ///
  /// It is possible to have different materials for front facing and
  /// back facing polygons. By default back and front use the same color
  /// but by setting the separateBackColor to TRUE a separate set of
  /// maps will be used. In this case all fields starting with back..
  /// will be used instead for all back facing polygons. Back Material
  /// values can be set using a TwoSidedMaterial node.
  /// 
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PhongShader.x3d">PhongShader.x3d</a>
  ///     ( <a href="examples/PhongShader.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/PhongShader_textures.x3d">PhongShader_textures.x3d</a>
  ///     ( <a href="examples/PhongShader_textures.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/PhongShader_twosided.x3d">PhongShader_twosided.x3d</a>
  ///     ( <a href="examples/PhongShader_twosided.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/PhongShader_tangentSpaceNormals.x3d">PhongShader_tangentSpaceNormals.x3d</a>
  ///     ( <a href="examples/PhongShader_tangentSpaceNormals.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile PhongShader.dot
  class H3DAPI_API PhongShader : 
    public H3DGeneratedFragmentShaderNode {
  public:
  
    /// The SFTexture2DNode field is dependent on the displayList field
    /// of the containing X3DTexture2DNode node.
    typedef DependentSFNode< X3DTexture2DNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTexture2DNode;

    /// Constructor.
    PhongShader( Inst< DisplayList  > _displayList = 0,
                 Inst< SFNode       > _metadata    = 0,
                 Inst< SFBool       > _isSelected  = 0,
                 Inst< SFBool       > _isValid     = 0,
                 Inst< SFBool       > _activate    = 0,
                 Inst< SFString     > _language    = 0,
                 Inst< MFShaderPart > _parts       = 0,
                 Inst< SFBool       > _suppressUniformWarnings = 0,
                 Inst< MFString > _fragmentShaderString = 0,
                 Inst< MFString > _vertexShaderString = 0,
                 Inst< SFTexture2DNode > _ambientMap  = 0,
                 Inst< SFTexture2DNode > _diffuseMap  = 0,
                 Inst< SFTexture2DNode > _emissionMap  = 0,
                 Inst< SFTexture2DNode > _normalMap   = 0,
                 Inst< SFString        > _normalMapCoordSpace = 0,
                 Inst< SFMatrix4f      > _normalMapMatrix = 0,
                 Inst< SFTexture2DNode > _specularMap = 0,
                 Inst< SFTexture2DNode > _glossMap    = 0,
                 Inst< SFTexture2DNode > _specularColorRamp = 0,
                 Inst< SFString        > _specularColorRampMode = 0,
                 Inst< SFTexture2DNode > _diffuseColorRamp = 0,
                 Inst< SFString        > _diffuseColorRampMode = 0,
                 Inst< SFFloat         > _fresnel      = 0,
                 Inst< SFBool          > _modulateMaps = 0,
                 Inst< SFTexture2DNode > _backAmbientMap  = 0,
                 Inst< SFTexture2DNode > _backDiffuseMap  = 0,
                 Inst< SFTexture2DNode > _backEmissionMap  = 0,
                 Inst< SFTexture2DNode > _backNormalMap   = 0,
                 Inst< SFString        > _backNormalMapCoordSpace = 0,
                 Inst< SFMatrix4f      > _backNormalMapMatrix = 0,
                 Inst< SFTexture2DNode > _backSpecularMap = 0,
                 Inst< SFTexture2DNode > _backGlossMap    = 0,
                 Inst< SFTexture2DNode > _backSpecularColorRamp = 0,
                 Inst< SFString        > _backSpecularColorRampMode = 0,
                 Inst< SFTexture2DNode > _backDiffuseColorRamp = 0,
                 Inst< SFString        > _backDiffuseColorRampMode = 0,
                 Inst< SFFloat         > _backFresnel = 0,
                 Inst< SFBool          > _backModulateMaps = 0,
                 Inst< SFBool          > _separateBackColor = 0,
                 Inst< SFString        > _model = 0 );
    
    /// The traverseSG method is specialized to add the 
    /// shaderRequiresTangents user data to the TraverseInfo
    /// object in order to let supported geometries to render
    /// tangents if they can.
    virtual void traverseSG( TraverseInfo &ti );

    /// Defines the Ma part in the Phong illumination model specified 
    /// above.
    /// 
    /// If NULL                              Ma = Material_ambientIntensity * Material_diffuseColor
    /// If non-NULL and modulateMaps FALSE   Ma = texture color 
    /// If non-NULL and modulateMaps TRUE    Ma = Material_ambientIntensity * texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_ambientMap.dot
    auto_ptr< SFTexture2DNode > ambientMap;

    /// Defines the Md part in the Phong illumination model specified 
    /// above.
    /// 
    /// If NULL                              Md = Material_diffuseColor
    /// If non-NULL and modulateMaps FALSE   Md = texture color 
    /// If non-NULL and modulateMaps TRUE    Md = Material_diffuseColor * texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_diffuseMap.dot
    auto_ptr< SFTexture2DNode > diffuseMap;

    /// Defines the Me part in the Phong illumination model specified 
    /// above.
    /// 
    /// If NULL                              Me = Material_emissiveColor
    /// If non-NULL                          Me = texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_emissionMap.dot
    auto_ptr< SFTexture2DNode > emissionMap;

    /// Defines the normal(N) in the Phong illumination model specified 
    /// above.
    /// 
    /// If NULL                              N = normal of the OpenGL primitive
    /// If non-NULL                          N = normalMapMatrix * texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_normalMap.dot
    auto_ptr< SFTexture2DNode > normalMap;

    /// Defines the transformation from RGB values of the normalMap(in range [0,1]) to
    /// the actual normals in range[-1,1].
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [ 2 0 0 -1 \n
    ///                         0 2 0 -1 \n
    ///                         0 0 2 -1 \n
    ///                         0 0 0 1 ] \n
    /// 
    /// \dotfile PhongShader_normalMapMatrix.dot
    auto_ptr< SFMatrix4f > normalMapMatrix;

    /// The normalMapCoordSpace field defines which coordinate space the normals in
    /// the normalMap are specified in.
    ///
    /// Valid values are:
    /// - "OBJECT" - the normals are in the coordinate space of the object.
    /// This means that the normals will be correct as long as the object
    /// does not change the local position of its vertices.
    /// - "TANGENT" - the normals are specified in the coordinate space 
    /// with the normal as z-axis and ... Not supported yet..
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "OBJECT"
    /// <b>Valid values:</b> "OBJECT", "TANGENT"
    /// 
    /// \dotfile PhongShader_normalMapMatrix.dot
    auto_ptr< SFString > normalMapCoordSpace;

    /// Defines the Ms part in the Phong illumination model specified 
    /// above.
    /// 
    /// If NULL                              Ms = Material_specularColor
    /// If non-NULL and modulateMaps FALSE   Ms = texture color 
    /// If non-NULL and modulateMaps TRUE    Ms = Material_specularColor * texture color
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_specularMap.dot
    auto_ptr< SFTexture2DNode > specularMap;

    /// Defines the Mshininess part in the Phong illumination model specified 
    /// above. 
    /// 
    /// If NULL                              Mshininess = Material_shininess
    /// If non-NULL and modulateMaps FALSE   Mshininess = texture color.r 
    /// If non-NULL and modulateMaps TRUE    Mshininess = Material_shininess * texture color.r
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_glossMap.dot
    auto_ptr< SFTexture2DNode > glossMap;

    /// The diffuseColorRamp can be used to control
    /// the diffuse color gradient across a material. It contains a 1D texture
    /// that defines the color ramp and the value used for lookup
    /// is controlled by the diffuseColorRampMode field.
    /// The looked up color is then multiplied 
    /// with the diffuse to generate the final diffuse color used in the lighting
    /// equations. This allows for different colors depending on the view angle
    /// to the surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_diffuseColorRamp.dot
    auto_ptr< SFTexture2DNode > diffuseColorRamp;

    /// The diffuseColorRampMode defines what value should be used to lookup
    /// up into the diffuseColorRamp texture.
    /// 
    /// Valid values are:
    /// - "LIGHT" - light direction dot normal
    /// - "CAMERA" - camera direction dot normal
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "LIGHT" \n
    /// <b>Valid values:</b> "LIGHT", "CAMERA" \n
    /// 
    /// \dotfile PhongShader_diffuseColorRampMode.dot
    auto_ptr< SFString > diffuseColorRampMode;

    /// The specularColorRamp can be used to control
    /// the specular color gradient across a material. It contains a 1D texture
    /// that defines the color ramp and the value used for lookup
    /// is controlled by the diffuseColorRampMode field. The looked up color is then multiplied 
    /// with the specular to generate the final specular color used in the lighting
    /// equations. This allows for different colors depending on the view angle
    /// to the surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_specularColorRamp.dot
    auto_ptr< SFTexture2DNode > specularColorRamp;


    /// The specularColorRampMode defines what value should be used to lookup
    /// up into the specularColorRamp texture.
    /// 
    /// Valid values are:
    /// - "LIGHT" - light direction dot normal
    /// - "CAMERA" - camera direction dot normal
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "LIGHT" \n
    /// <b>Valid values:</b> "LIGHT", "CAMERA" \n
    /// 
    /// \dotfile PhongShader_specularColorRampMode.dot
    auto_ptr< SFString > specularColorRampMode;

    /// The fresnel field specifies if the fresnel effect should be used in the
    /// lighting model and if so how much. The fresnel effect is the effect
    /// that surfaces reflect more light if looking at them from an angle than
    /// straight down along the normal. If set to 0 no fresnel effect is used
    /// and the surface is fully reflective. If set to 1 only the reflectivity
    /// of the fresnel effect is used, which means no reflectivity when looking
    /// down perpendicular to the surace. One can think of the term as controlling
    /// the minimum reflectivness of the surface. By default it is set to 0 and
    /// no fresnel effect is used.
    /// The fresnel value only affects the specular component of the Phong
    /// lighting calculations since this is the only part that deals with
    /// reflective properties. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile PhongShader_fresnel.dot
    auto_ptr< SFFloat > fresnel;

    /// The modulateMaps field determines if the map values should be modulated
    /// with the corresponding Material value or not. See documentation for
    /// the map fields for each component for details(diffuseMap, emissiveMap, etc)
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile PhongShader_modulateMaps.dot
    auto_ptr< SFBool > modulateMaps;

    /// Defines the Ma part in the Phong illumination model specified 
    /// above for back facing polygons. Only used if separateBackColor is
    /// TRUE.
    /// 
    /// If NULL                              Ma = Material_ambientIntensity * Material_diffuseColor
    /// If non-NULL and modulateMaps FALSE   Ma = texture color 
    /// If non-NULL and modulateMaps TRUE    Ma = Material_ambientIntensity * texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backAmbientMap.dot
    auto_ptr< SFTexture2DNode > backAmbientMap;

    /// Defines the Md part in the Phong illumination model specified 
    /// above for back facing polygons. Only used if separateBackColor is
    /// TRUE.
    /// 
    /// If NULL                              Md = Material_diffuseColor
    /// If non-NULL and modulateMaps FALSE   Md = texture color 
    /// If non-NULL and modulateMaps TRUE    Md = Material_diffuseColor * texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backDiffuseMap.dot
    auto_ptr< SFTexture2DNode > backDiffuseMap;

    /// Defines the Me part in the Phong illumination model specified 
    /// above for back facing polygons. Only used if separateBackColor is
    /// TRUE.
    /// 
    /// If NULL                              Me = Material_emissiveColor
    /// If non-NULL                          Me = texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backEmissionMap.dot
    auto_ptr< SFTexture2DNode > backEmissionMap;

    /// Defines the normal(N) in the Phong illumination model specified 
    /// above for back facing polygons. Only used if separateBackColor is
    /// TRUE.
    /// 
    /// If NULL                              N = normal of the OpenGL primitive
    /// If non-NULL                          N = normalMapMatrix * texture color 
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backNormalMap.dot
    auto_ptr< SFTexture2DNode > backNormalMap;

    /// Defines the transformation from RGB values of the normalMap(in range [0,1]) to
    /// the actual normals in range[-1,1]. Only used if separateBackColor is
    /// TRUE
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [ 2 0 0 -1 \n
    ///                         0 2 0 -1 \n
    ///                         0 0 2 -1 \n
    ///                         0 0 0 1 ] \n
    /// 
    /// \dotfile PhongShader_backNormalMapMatrix.dot
    auto_ptr< SFMatrix4f > backNormalMapMatrix;

    /// The normalMapCoordSpace field defines which coordinate space the normals in
    /// the normalMap are specified in. Only used if separateBackColor is
    /// TRUE
    ///
    /// Valid values are:
    /// - "OBJECT" - the normals are in the coordinate space of the object.
    /// This means that the normals will be correct as long as the object
    /// does not change the local position of its vertices.
    /// - "TANGENT" - the normals are specified in the coordinate space 
    /// with the normal as z-axis and ... Not supported yet..
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "OBJECT"
    /// <b>Valid values:</b> "OBJECT", "TANGENT"
    /// 
    /// \dotfile PhongShader_backNormalMapMatrix.dot
    auto_ptr< SFString > backNormalMapCoordSpace;

    /// Defines the Ms part in the Phong illumination model specified 
    /// above for back facing polygons. Only used if separateBackColor is
    /// TRUE.
    /// 
    /// If NULL                              Ms = Material_specularColor
    /// If non-NULL and modulateMaps FALSE   Ms = texture color 
    /// If non-NULL and modulateMaps TRUE    Ms = Material_specularColor * texture color
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backSpecularMap.dot
    auto_ptr< SFTexture2DNode > backSpecularMap;

    /// Defines the Mshininess part in the Phong illumination model specified 
    /// above for back facing polygons. Only used if separateBackColor is
    /// TRUE. 
    /// 
    /// If NULL                              Mshininess = Material_shininess
    /// If non-NULL and modulateMaps FALSE   Mshininess = texture color.r 
    /// If non-NULL and modulateMaps TRUE    Mshininess = Material_shininess * texture color.r
    ///
    /// The multiplications above are componentwise multiplication.
    //
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backGlossMap.dot
    auto_ptr< SFTexture2DNode > backGlossMap;

    /// The diffuseColorRamp can be used to control the diffuse color 
    /// gradient across a material for back facing polygons. 
    /// It contains a 1D texture that defines the color ramp and the value used
    /// for lookup is controlled by the diffuseColorRampMode field.
    /// The looked up color is then multiplied 
    /// with the diffuse to generate the final diffuse color used in the lighting
    /// equations. This allows for different colors depending on the view angle
    /// to the surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backDiffuseColorRamp.dot
    auto_ptr< SFTexture2DNode > backDiffuseColorRamp;

    /// The backDiffuseColorRampMode defines what value should be used to lookup
    /// up into the backDiffuseColorRamp texture.
    /// 
    /// Valid values are:
    /// - "LIGHT" - light direction dot normal
    /// - "CAMERA" - camera direction dot normal
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "LIGHT" \n
    /// <b>Valid values:</b> "LIGHT", "CAMERA" \n
    /// 
    /// \dotfile PhongShader_backDiffuseColorRampMode.dot
    auto_ptr< SFString > backDiffuseColorRampMode;

    /// The specularColorRamp can be used to control
    /// the specular color gradient across a material for back facing polygons.
    /// It contains a 1D texture that defines the color ramp and the value used
    /// for lookup is controlled by the specularColorRampMode field.
    /// The looked up color is then multiplied 
    /// with the specular to generate the final specular color used in the lighting
    /// equations. This allows for different colors depending on the view angle
    /// to the surface.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile PhongShader_backSpecularColorRamp.dot
    auto_ptr< SFTexture2DNode > backSpecularColorRamp;

    /// The backSpecularColorRampMode defines what value should be used to lookup
    /// up into the backSpecularColorRamp texture.
    /// 
    /// Valid values are:
    /// - "LIGHT" - light direction dot normal
    /// - "CAMERA" - camera direction dot normal
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "LIGHT" \n
    /// <b>Valid values:</b> "LIGHT", "CAMERA" \n
    /// 
    /// \dotfile PhongShader_backSpecularColorRampMode.dot
    auto_ptr< SFString > backSpecularColorRampMode;

    /// The backFresnel field specifies the fresnel value for back facing
    /// polygons when separateBackColor is TRUE. Se fresnel field for 
    /// more info.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile PhongShader_backFresnel.dot
    auto_ptr< SFFloat > backFresnel;

    /// The modulateMaps field determines if the map values should be modulated
    /// with the corresponding Material value or not. See documentation for
    /// the map fields for each component for details(diffuseMap, emissiveMap, etc).
    /// Only used if separateBackColor is TRUE
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile PhongShader_backModulateMaps.dot
    auto_ptr< SFBool > backModulateMaps;

    /// If the separateBackColor field is set to TRUE, then the rendering shall
    /// render the front and back faces of the geometry with different values using
    /// the backDiffuseMap, backSpecularMap, etc. fields for its values. 
    /// If the value is FALSE, then the front colours are used for both the front
    /// and back side of the polygon. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile PhongShader_separateBackColor.dot
    auto_ptr< SFBool > separateBackColor;

    /// The shading model to use. Available values:
    /// "BLINN-PHONG" - modified Phong model using half-vector instead of reflection vector.
    /// "BLINN-PHONG-EYEATINF" - same as "BLINN-PHONG" with the difference that the eye is assumed
    /// to be at infinity. 
    /// "PHONG" - normal phong shading using reflection vector.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "BLINN-PHONG" \n
    /// <b>Valid values:</b> "PHONG" \n
    /// 
    /// \dotfile PhongShader_model.dot
    auto_ptr< SFString > model;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:

    /// The X3DLightNode instances that this shader is built for.
    /// The ligts are ordered in the same order as the gl light 
    /// index they use.
    TraverseInfo::LightVector current_light_nodes;

    /// The transform matrices for all lights with index > maximum
    /// number lightsources supported by OpenGL fixed functionallity.
    /// Index 0 in this vector matches index max_gl_lights in 
    /// current light nodes.
    AutoPtrVector< SFMatrix4f > current_light_node_transforms;

    /// The number of light sources the shader currently handles(including headlight)
    unsigned int current_nr_lightsources;

    /// Returns a unique name to use in this shader for a field of a light node.
    string uniqueLightFieldName( const string &field_name,
                                 X3DLightNode *light);

    /// Adds uniform fields to the shader. A ComposedShader uses its
    /// dynamic fields to define uniform variables. Hence we need to
    /// add dynamic fields for each field that we want to be accessable
    /// in the shader.
    virtual string addUniformFields( ComposedShader *shader );

    /// Adds uniform fields needed for a light to the shader. 
    /// \param shader The shader to add the uniform to.
    /// \param light The light for which to a uniforms.
    /// \param gl_index The OpenGL light index used for the light.
    /// If the index is larger than the number of supported light
    /// sources in the OpenGL implementation all fields of the light
    /// are added in order to support more lights than that.
    virtual string addUniformFieldsForLight( ComposedShader *shader,
                                             X3DLightNode *light,
                                             int gl_index);

    /// Get a string with GLSL function definitions to be used by
    /// the generated shader.
    virtual string getFunctionShaderString();

    /// Get the varying variables used by the shader generator.
    virtual void getVaryingVariables( vector< VaryingVariable > &variables );

    /// Get the attribues used by the shader generator.
    virtual void getAttributes( vector< Attribute > &attributes );

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

    /// Returns a glsl string that sets the variable diffuse_color 
    /// to the diffuse component to use in the shading model and 
    /// back_diffuse_color to the diffuse component for back facing faces.
    virtual string getBaseDiffuseColor( const string &diffuse_color, 
                                        const string &back_diffuse_color );

    /// Returns a glsl string that sets the variable emission_color 
    /// to the emission component to use in the shading model and 
    /// back_emission_color to the emission component for back facing faces.
    virtual string getBaseEmissionColor( const string &emission_color, 
                                         const string &back_emission_color );

    /// Returns a glsl string that sets the variable ambient_color 
    /// to the ambient component to use in the shading model and 
    /// back_ambient_color to the ambient component for back facing faces.
    virtual string getBaseAmbientColor( const string &ambient_color, 
                                        const string &back_ambient_color );

    /// Returns a glsl string that sets the variable specular_color 
    /// to the specular component to use in the shading model and 
    /// back_specular_color to the specular component for back facing faces.
    virtual string getBaseSpecularColor( const string &specular_color, 
                                         const string &back_specular_color );
    
    /// Returns a glsl string that sets the variable shininess 
    /// to the shininess component to use in the shading model and 
    /// back_shininess to the shininess component for back facing faces.
    virtual string getBaseShininess( const string &shininess, 
                                     const string &back_shininess );

    /// Returns a glsl string that sets the variable normal 
    /// to the normal to use in the shading model and 
    /// back_normal to the normal for back facing faces.
    virtual string getBaseNormal( const string &normal, 
                                  const string &back_normal );

    /// Returns a glsl string that sets all the given variables to 
    /// its appropriate values. Default implementation is to call the 
    /// getBaseSpecularColor, getBaseDiffuseColor etc.
    virtual string getBaseColors( const string &emission_color,
                                  const string &shininess, 
                                  const string &ambient_color,
                                  const string &diffuse_color,
                                  const string &specular_color,
                                  const string &normal,
                                  const string &back_emission_color,
                                  const string &back_shininess, 
                                  const string &back_ambient_color,
                                  const string &back_diffuse_color,
                                  const string &back_specular_color,
                                  const string &back_normal );

    /// Returns a string with glsl code that adds the color contribution
    /// of a light node to the variable "final_color".
    /// \param light The light node for which to calculate the contributtion. 
    /// If NULL, headlight is used.
    /// \param index The OpenGL light index for the light.
    /// \param normal The glsl variable name in the shader for the normal.
    /// \param vertex The glsl variable name in the shader for the vertex.
    /// \param shininess The glsl variable name in the shader for the shininess.
    /// \param ambient_color The glsl variable name in the shader for the ambient_color.
    /// \param diffuse_color The glsl variable name in the shader for the diffuse_color.
    /// \param specular_color The glsl variable name in the shader for the specular_color.
    virtual string getLightContribution( X3DLightNode *light,
                                         unsigned int index, 
                                         string normal, 
                                         string vertex, 
                                         string shininess, 
                                         string ambient_color,
                                         string diffuse_color,
                                         string specular_color );

    /// Returns a string with glsl code that sets adds and initialized
    /// a variable of type gl_LightParameters and name light to the 
    /// values of the light source.
    /// 
    /// \param light The light to generate glsl code for.
    /// \param gl_index The OpenGL light index for the light.
    virtual string setupLight( X3DLightNode *light,
                               unsigned int gl_index );
  };
}

#endif
