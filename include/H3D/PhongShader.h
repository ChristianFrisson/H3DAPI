//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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

#include <H3D/ComposedShader.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/SFMatrix4f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PhongShader
  /// \brief The PhongShader node defines a shader for using the Phong
  /// illumination model.
  ///
  /// For each light we have
  /// C = att(d)*(Ma * La + Md*Ld*(L dot N) + Ms*Ls*(R dot V )^Mshininess)
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
  /// If modulateMaps is FALSE the color from the map is used directly
  /// in all texture maps except for Mshininess which is a floating point
  /// value. In this case for a 1 or 2 component image, we use the intensity
  /// and for RGB and RGBA texture the R component is used.
  ///
  /// If modulateMaps is TRUE the values are modulated with the material 
  /// value, e.g. Md = Material diffuse * diffuseMap value.
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
  /// - "TANGENT" - the normals are specified in the coordinate space 
  /// with the normal as z-axis and ... Not supported yet..
  ///
  /// It is possible to have different materials for front facing and
  /// back facing polygons. By default back and front use the same color
  /// but by setting the separateBackColor to TRUE a separate set of
  /// maps will be used. In this case all fields starting with back..
  /// will be used instead for all back facing polygons. Back Material
  /// values can be set using a TwoSidedMaterial node.
  /// 
  /// \par Internal routes:
  /// \dotfile PhongShader.dot
  class PhongShader : 
    public ComposedShader {
  public:
  
    /// The SFTexture2DNode field is dependent on the displayList field
    /// of the containing X3DTexture2DNode node.
    typedef DependentSFNode< X3DTexture2DNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTexture2DNode;

    /// Field for updating the string for the fragment shader to use.
    class FragmentShaderString: public TypedField< MFString, Field > {
    protected:
      virtual void update();
    };

    /// Field for updating the string for the vertex shader to use.
    class VertexShaderString: public TypedField< MFString, Field >  {
    protected:
      virtual void update();
    };

    /// Constructor.
    PhongShader( Inst< DisplayList  > _displayList = 0,
                   Inst< SFNode       > _metadata    = 0,
                   Inst< SFBool       > _isSelected  = 0,
                   Inst< SFBool       > _isValid     = 0,
                   Inst< SFBool       > _activate    = 0,
                   Inst< SFString     > _language    = 0,
                   Inst< MFShaderPart > _parts       = 0,
                   Inst< SFBool       > _suppressUniformWarnings = 0,
                   Inst< SFTexture2DNode > _ambientMap  = 0,
                   Inst< SFTexture2DNode > _diffuseMap  = 0,
                   Inst< SFTexture2DNode > _emissionMap  = 0,
                   Inst< SFTexture2DNode > _normalMap   = 0,
                   Inst< SFString        > _normalMapCoordSpace = 0,
                   Inst< SFMatrix4f      > _normalMapMatrix = 0,
                   Inst< SFTexture2DNode > _specularMap = 0,
                   Inst< SFTexture2DNode > _glossMap    = 0,
                   Inst< SFBool          > _modulateMaps = 0,
                   Inst< SFTexture2DNode > _backAmbientMap  = 0,
                   Inst< SFTexture2DNode > _backDiffuseMap  = 0,
                   Inst< SFTexture2DNode > _backEmissionMap  = 0,
                   Inst< SFTexture2DNode > _backNormalMap   = 0,
                   Inst< SFString        > _backNormalMapCoordSpace = 0,
                   Inst< SFMatrix4f      > _backNormalMapMatrix = 0,
                   Inst< SFTexture2DNode > _backSpecularMap = 0,
                   Inst< SFTexture2DNode > _backGlossMap    = 0,
                   Inst< SFBool          > _backModulateMaps = 0,
                   Inst< SFBool          > _separateBackColor = 0,
                   Inst< FragmentShaderString > _fragmentShaderString = 0,
                   Inst< VertexShaderString > _vertexShaderString = 0 );
    
    /// The preRender method is extended to add all uniform variables
    /// before the shader is rendered.
    virtual void preRender();

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

    /// Contains the generated fragment shader code.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile PhongShader_fragmentShaderString.dot
    auto_ptr< FragmentShaderString > fragmentShaderString;

    /// Contains the generated vertex shader code.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile PhongShader_vertexShaderString.dot
    auto_ptr< VertexShaderString > vertexShaderString;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Field used to update the shader code when an event is received.
    /// Every field that should cause the shaders to be rebuild needs
    /// to be routed to this field.
    auto_ptr< Field > rebuildShader;

    /// Adds uniform fields to the shader. A ComposedShader uses its
    /// dynamic fields to define uniform variables. Hence we need to
    /// add dynamic fields for each field that we want to be accessable
    /// in the shader.
    virtual void addUniformFields();

    /// Returns the shader code for the vertex shader.
    virtual string getVertexShaderString();

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

  };
}

#endif
