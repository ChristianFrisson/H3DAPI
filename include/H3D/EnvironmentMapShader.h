//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file EnvironmentMapShader.h
/// \brief Header file for EnvironmentMapShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ENVIRONMENTMAPSHADER_H__
#define __ENVIRONMENTMAPSHADER_H__

#include <H3D/H3DGeneratedFragmentShaderNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DEnvironmentTextureNode.h>
#include <H3D/SFMatrix4f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class EnvironmentMapShader
  /// \brief The EnvironmentMapShader node defines a shader for environment
  /// mapping of shiny objects. 
  ///
  /// The environmentMap field contains a X3DEnvironmentTextureNode containing the 
  /// environment map to use.
  /// 
  /// The fresnel field specifies if the fresnel effect should be used in the
  /// reflection model and if so how much. The fresnel effect is the effect
  /// that surfaces reflect more light if looking at them from an angle than
  /// straight down along the normal. If set to 0 nor fresnel effect is used
  /// and the surface is fully reflective. If set to 1 only the reflectivity
  /// of the fresnel effect is used, which means no reflectivity when looking
  /// down perpendicular to the surace. One can think of the term as controlling
  /// the minimum reflectivness of the surface. By default it is set to 0 and
  /// no fresnel effect is used. 
  ///
  /// The type field determines how to lookup into the environment map.
  /// If set to "VIEW_SPACE" the texture coordinate for the cube map
  /// will be the reflected view vector in view space (camera space).
  /// This is the same as CAMERASPACEREFLECTIONVECTOR in 
  /// TextureCoordinateGenerator. It means that the result will be as if 
  /// the environment cube was attached to the camera and moved around
  /// with it when the camera moves.
  /// If set to "GLOBAL_SPACE" the texture coodinates are the reflected
  /// view vector in global space, with the result of the object reflecting
  /// the cube map like a mirror.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/EnvironmentMapShader.x3d">EnvironmentMapShader.x3d</a>
  ///     ( <a href="examples/EnvironmentMapShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile EnvironmentMapShader.dot
  class EnvironmentMapShader : 
    public H3DGeneratedFragmentShaderNode {
  public:

    /// The SFEnvironmentTextureNode field is dependent on the displayList field
    /// of the containing X3DEnvironmentTextureNode node.
    typedef DependentSFNode< X3DEnvironmentTextureNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFEnvironmentTextureNode;

    /// Constructor.
    EnvironmentMapShader( Inst< DisplayList  > _displayList = 0,
                          Inst< SFNode       > _metadata    = 0,
                          Inst< SFBool       > _isSelected  = 0,
                          Inst< SFBool       > _isValid     = 0,
                          Inst< SFBool       > _activate    = 0,
                          Inst< SFString     > _language    = 0,
                          Inst< MFShaderPart > _parts       = 0,
                          Inst< SFBool       > _suppressUniformWarnings = 0,
                          Inst< MFString     > _fragmentShaderString = 0,
                          Inst< MFString     > _vertexShaderString = 0,
                          Inst< SFEnvironmentTextureNode > _environmentMap = 0,
                          Inst< SFFloat                  > _fresnel = 0,
                          Inst< SFString     > _type = 0 );
    
    /// Contains the X3DEnvironmentTextureNode to use for environment mapping.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// 
    /// \dotfile EnvironmentMapShader_environmentMap.dot
    auto_ptr< SFEnvironmentTextureNode > environmentMap;

    /// The fresnel field specifies if the fresnel effect should be used in the
    /// reflection model and if so how much. The fresnel effect is the effect
    /// that surfaces reflect more light if looking at them from an angle than
    /// straight down along the normal. If set to 0 nor fresnel effect is used
    /// and the surface is fully reflective. If set to 1 only the reflectivity
    /// of the fresnel effect is used, which means no reflectivity when looking
    /// down perpendicular to the surace. One can think of the term as controlling
    /// the minimum reflectivness of the surface. By default it is set to 0 and
    /// no fresnel effect is used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile EnvironmentMapShader_fresnel.dot
    auto_ptr< SFFloat > fresnel;

    /// The type field determines how to lookup into the environment map.
    /// If set to "VIEW_SPACE" the texture coordinate for the cube map
    /// will be the reflected view vector in view space (camera space).
    /// This is the same as CAMERASPACEREFLECTIONVECTOR in 
    /// TextureCoordinateGenerator. It means that the result will be as if 
    /// the environment cube was attached to the camera and moved around
    /// with it when the camera moves.
    /// If set to "GLOBAL_SPACE" the texture coodinates are the reflected
    /// view vector in global space, with the result of the object reflecting
    /// the cube map like a mirror.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Valid values:</b> "GLOBAL_SPACE" and "VIEW_SPACE" \n
    /// <b>Default value:</b> "GLOBAL_SPACE" \n
    /// 
    /// \dotfile EnvironmentMapShader_type.dot
    auto_ptr< SFString > type;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Adds uniform fields to the shader. A ComposedShader uses its
    /// dynamic fields to define uniform variables. Hence we need to
    /// add dynamic fields for each field that we want to be accessable
    /// in the shader.
    virtual string addUniformFields( ComposedShader *shader );

    /// Get the varying variables used by the shader generator.
    virtual void getVaryingVariables( vector< VaryingVariable > &variables );

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

  };
}

#endif
