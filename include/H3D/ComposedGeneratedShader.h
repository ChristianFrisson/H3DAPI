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
/// \file ComposedGeneratedShader.h
/// \brief Header file for ComposedGeneratedShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COMPOSEDGENERATEDSHADER_H__
#define __COMPOSEDGENERATEDSHADER_H__

#include <H3D/H3DGeneratedFragmentShaderNode.h>
#include <H3D/H3DGeneratedVertexShaderNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ComposedGeneratedShader
  /// \brief The ComposedGeneratedShader node composes vertex shader manipulator
  /// shaders with fragment shaders.
  ///
  /// The vertexShaders field specifies a number of H3DGeneratedVertexShaderNode
  /// instances that modify the behaviour of the default vertex shader. Multiple
  /// nodes can be used and they will then be applied in succession in the order
  /// specified in the field.
  ///
  /// The fragmentShaders field specifies a number of H3DGeneratedFragmentShaderNode
  /// instances that build up the fragment shader. Multiple
  /// nodes can be used and they will then be applied in succession in the order
  /// specified in the field. The output from the previous shader can be accessed
  /// through the generated_color variable.
  /// 
  /// 
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ComposedGeneratedShader.x3d">ComposedGeneratedShader.x3d</a>
  ///     ( <a href="examples/ComposedGeneratedShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ComposedGeneratedShader.dot
  class H3DAPI_API ComposedGeneratedShader : 
    public H3DGeneratedShaderNode {
  public:
  
    typedef DependentMFNode< H3DGeneratedFragmentShaderNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >
      MFGeneratedFragmentShaderNode;    

    typedef DependentMFNode< H3DGeneratedVertexShaderNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >
      MFGeneratedVertexShaderNode;    

    /// Constructor.
    ComposedGeneratedShader( Inst< DisplayList  > _displayList = 0,
                             Inst< SFNode       > _metadata    = 0,
                             Inst< SFBool       > _isSelected  = 0,
                             Inst< SFBool       > _isValid     = 0,
                             Inst< SFBool       > _activate    = 0,
                             Inst< SFString     > _language    = 0,
                             Inst< MFShaderPart > _parts       = 0,
                             Inst< SFBool       > _suppressUniformWarnings = 0,
                             Inst< MFString     > _fragmentShaderString = 0,
                             Inst< MFString     > _vertexShaderString = 0,
                             Inst< MFGeneratedFragmentShaderNode > _fragmentShaders = 0,
                             Inst< MFGeneratedVertexShaderNode   > _vertexShaders = 0 );
    
    /// Traverse the scene-graph.
    virtual void traverseSG( TraverseInfo &ti );
 
    /// The fragmentShaders field specifies a number of H3DGeneratedFragmentShaderNode
    /// instances that build up the fragment shader. Multiple
    /// nodes can be used and they will then be applied in succession in the order
    /// specified in the field. The output from the previous shader can be accessed
    /// through the generated_color variable.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedGeneratedShader_fragmentShaders.dot
    auto_ptr< MFGeneratedFragmentShaderNode > fragmentShaders;
 
    /// The vertexShaders field specifies a number of H3DGeneratedVertexShaderNode
    /// instances that modify the behaviour of the default vertex shader. Multiple
    /// nodes can be used and they will then be applied in succession in the order
    /// specified in the field.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedGeneratedShader_vertexShaders.dot
    auto_ptr< MFGeneratedVertexShaderNode > vertexShaders;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Adds uniform fields to the shader. A ComposedShader uses its
    /// dynamic fields to define uniform variables. Hence we need to
    /// add dynamic fields for each field that we want to be accessable
    /// in the shader.
    virtual string addUniformFields( ComposedShader * );

    /// Get a string with GLSL function definitions to be used by
    /// the generated shader.
    virtual string getFunctionShaderString();

    /// Get the varying variables used by the shader generator.
    virtual void getVaryingVariables( vector< VaryingVariable > &variables );

    /// Get the attribues used by the shader generator.
    virtual void getAttributes( vector< Attribute > &attributes );

    /// Returns the shader code for the fragment shader.
    virtual string getVertexShaderString();

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();
  };
}

#endif
