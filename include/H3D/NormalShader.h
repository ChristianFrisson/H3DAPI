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
/// \file NormalShader.h
/// \brief Header file for NormalShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NORMALSHADER_H__
#define __NORMALSHADER_H__

#include <H3D/H3DGeneratedFragmentShaderNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class NormalShader
  /// \brief The NormalShader node generates a shader that renders the normal in view 
  /// space of the geometry as color.
  /// 
  /// If the normal for a fragment is n then the output color is RGBA( n.x, n.y, n.z, 1 )
  /// 
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NormalShader.x3d">NormalShader.x3d</a>
  ///     ( <a href="examples/NormalShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NormalShader.dot
  class H3DAPI_API NormalShader : 
    public H3DGeneratedFragmentShaderNode {
  public:
  
    /// Constructor.
    NormalShader( Inst< DisplayList  > _displayList = 0,
                  Inst< SFNode       > _metadata    = 0,
                  Inst< SFBool       > _isSelected  = 0,
                  Inst< SFBool       > _isValid     = 0,
                  Inst< SFBool       > _activate    = 0,
                  Inst< SFString     > _language    = 0,
                  Inst< MFShaderPart > _parts       = 0,
                  Inst< SFBool       > _suppressUniformWarnings = 0,
                  Inst< MFString     > _fragmentShaderString    = 0,
                  Inst< MFString     > _vertexShaderString      = 0 );
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    virtual void getVaryingVariables( vector< VaryingVariable > &variables );

    /// Returns the shader code for the fragment shader.
    virtual string getVertexShaderString();

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

  };
}

#endif
