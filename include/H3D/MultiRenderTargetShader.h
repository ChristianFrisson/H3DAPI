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
/// \file MultiRenderTargetShader.h
/// \brief Header file for MultiRenderTargetShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MULTIRENDERTARGETSHADER_H__
#define __MULTIRENDERTARGETSHADER_H__

#include <H3D/H3DGeneratedFragmentShaderNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class MultiRenderTargetShader
  /// \brief The MultiRenderTargetShader node defines a way to use different
  /// shaders for different render targets in one pass. 
  /// 
  /// This node is usually used in combination with a FrameBufferTextureGenerator
  /// with several color buffers using different shaders for each of these
  /// buffers.
  ///
  /// The shaders field specify the shaders to use for each render target. The
  /// first shader maps to render target 0, which is the same as the first color
  /// buffer in a FrameBufferTextureGenerator and so on.
  /// 
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MultiRenderTargetShader.x3d">MultiRenderTargetShader.x3d</a>
  ///     ( <a href="examples/MultiRenderTargetShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile MultiRenderTargetShader.dot
  class H3DAPI_API MultiRenderTargetShader : 
    public H3DGeneratedShaderNode {
  public:
  
    typedef DependentMFNode< H3DGeneratedFragmentShaderNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >
      MFGeneratedFragmentShaderNode;    


    /// Constructor.
    MultiRenderTargetShader( Inst< DisplayList  > _displayList = 0,
                             Inst< SFNode       > _metadata    = 0,
                             Inst< SFBool       > _isSelected  = 0,
                             Inst< SFBool       > _isValid     = 0,
                             Inst< SFBool       > _activate    = 0,
                             Inst< SFString     > _language    = 0,
                             Inst< MFShaderPart > _parts       = 0,
                             Inst< SFBool       > _suppressUniformWarnings = 0,
                             Inst< MFString > _fragmentShaderString = 0,
                             Inst< MFString > _vertexShaderString = 0,
                             Inst< MFGeneratedFragmentShaderNode > _shaders = 0);

    /// Traverse the scene-graph.
    virtual void traverseSG( TraverseInfo &ti );

    /// The shaders field specify the shaders to use for each render target. The
    /// first shader maps to render target 0, which is the same as the first color
    /// buffer in a FrameBufferTextureGenerator and so on.    
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile MultiRenderTargetShader_shaders.dot
    auto_ptr< MFGeneratedFragmentShaderNode > shaders;

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

    /// Returns the string that is the last thing that is done in the
    /// fragment shader. In the MultiRenderTargetShader this is set to
    /// an empty string since getFragmentShaderString is setting output
    /// to gl_FragData instead of gl_FragColor.
    virtual string getFinalFragmentShaderOutputString();

   /// Returns the shader code for the fragment shader.
    virtual string getVertexShaderString();

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

  };
}

#endif
