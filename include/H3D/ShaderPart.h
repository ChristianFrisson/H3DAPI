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
/// \file ShaderPart.h
/// \brief Header file for ShaderPart, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADERPART_H__
#define __SHADERPART_H__

#include <H3D/X3DNode.h>
#include <H3D/X3DUrlObject.h>
#include <GL/glew.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ShaderPart
  /// \brief The ShaderPart node defines the source for a single object to
  /// be used by a ComposedShader node. The source is not required to be
  /// a complete shader for all of the vertex/fragment processing.
  ///
  /// The type field indicates whether this object shall be compiled as a
  /// vertex shader, fragment shader, or other future-defined shader type.
  ///
  /// The shader source is read from the URL specified by the url field. 
  /// When the url field contains no values ([]), this object instance is 
  /// ignored. Shader source files shall be plain text encoded as specified
  /// for MIME type text/plain and interpreted according to the type field.
  ///
  /// \par Preprocessor
  /// Preprocessor commands are supported as an extension to the %X3D standard.
  /// These begin "#pragma h3dapi [command] [arguments]".
  ///
  /// Supported commands:
  /// <ul>
  ///   <li>
  ///     <b>include "url_to_include"</b> - 
  ///     The contents of the file pointed to by the specified URL are imported
  ///     and used to replace the include statement before the shader is compiled.
  ///   </li>
  /// </ul>
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ComposedShader.x3d">ComposedShader.x3d</a>
  ///     ( <a href="examples/ComposedShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ShaderPart.dot
  class H3DAPI_API ShaderPart : 
    public X3DNode, 
    public X3DUrlObject {
  public:
    /// Update the string to use as shader depending from the urls given.
    class H3DAPI_API SFShaderString: 
      public TypedField< SFString,
                         Types<MFString,SFBool> > {
    protected:
      virtual void update();
    };

    typedef SFBool ForceReload;
    /// Constructor.
    ShaderPart( Inst< SFNode         > _metadata      = 0,
                Inst< MFString       > _url           = 0,
                Inst< SFString       > _type          = 0,
                Inst< SFShaderString > _shader_string = 0,
                Inst< SFBool         > _forceReload   = 0);
    
    /// Compile the shader using the shader_string field as text input.
    /// Returns a handle to the compiled shader or 0 if compiling 
    /// failed.
    virtual GLhandleARB compileShader();

    /// Get the handle to the shader object that currently is used for
    /// this ShaderPart.
    inline GLhandleARB getShaderHandle() {
      return shader_handle;
    }

    /// Returns true if the shader part is compiled and up to date. False
    /// if compilation is required.
    virtual bool isCompiled ();

    /// The type field indicates whether this object shall be compiled
    /// as a vertex shader, fragment shader, or other future-defined shader 
    /// type. Valid values are "VERTEX", "FRAGMENT","GEOMETRY" "TESS_CONTROL"
    /// or "TESS_EVALUATION".
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// \dotfile ShaderPart_type.dot
    auto_ptr< SFString > type;

    /// Internal field that contains the string loaded from file and
    /// to be used as input to the shader.
    /// 
    /// \dotfile ShaderPart_shaderString.dot
    auto_ptr< SFShaderString > shaderString;

    /// The forceReload field is used to force the reloading of this shaderPart
    /// and also cause the composed shader recompile and relink.
    /// Directly change its value from tree view of h3dviewer, or route someother
    /// field to it will both work
    /// <b>Access type:<\b> inputOutput \n
    /// \dotfile ShaderPart_forceReload
    auto_ptr< SFBool > forceReload;

    /// Returns the default xml containerField attribute value.
    /// For this node it is "parts".
    virtual string defaultXMLContainerField() {
      return "parts";
    }

    /// The loadStatus function returns the status of the loading
    /// of the current urls in order to be used with the LoadSensor
    /// node.
    virtual X3DUrlObject::LoadStatus loadStatus();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  protected:
    /// The handle to the shader object used for this ShaderPart.
    GLhandleARB shader_handle;
    GLhandleARB compileShaderPart();

    /// Given the URL of a shader source, return the source code, or "" on failure
    std::string shaderStringFromURL ( const std::string& shader_url );

    /// Given the raw shader source, return the preprocessed source having parsed all preprocessor
    /// commands, e.g., includes etc.
    ///
    /// \param input The raw shader source code, including preprocessor commands
    /// \param url   The URL from which the raw source code was loaded (used for base of relative includes)
    /// \param depth The current recursion depth. The root caller should always leave this as the default 0.
    ///
    /// \return The preprocessed shader source, with all preprocessor commands applied.
    ///
    std::string preProcess ( const std::string& input, const std::string& url, int depth= 0 );
  };
}

#endif
