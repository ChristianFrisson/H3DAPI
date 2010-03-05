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
  /// \par Internal routes:
  /// \dotfile ShaderPart.dot
  class ShaderPart : 
    public X3DNode, 
    public X3DUrlObject {
  public:
    /// Update the string to use as shader depending from the urls given.
    class SFShaderString: 
      public TypedField< SFString,
                         MFString > {
    protected:
      virtual void update();
    };

    /// Constructor.
    ShaderPart( Inst< SFNode         > _metadata      = 0,
                Inst< MFString       > _url           = 0,
                Inst< SFString       > _type          = 0,
                Inst< SFShaderString > _shader_string = 0);
    
    /// Compile the shader using the shader_string field as text input.
    /// Returns a handle to the compiled shader or 0 if compiling 
    /// failed.
    virtual GLhandleARB compileShader();

    /// Get the handle to the shader object that currently is used for
    /// this ShaderPart.
    inline GLhandleARB getShaderHandle() {
      return shader_handle;
    }

    /// The type field indicates whether this object shall be compiled
    /// as a vertex shader, fragment shader, or other future-defined shader 
    /// type. Valid values are "VERTEX" and "FRAGMENT".
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// \dotfile ShaderPart_type.dot
    auto_ptr< SFString > type;

    /// Internal field that contains the string loaded from file and
    /// to be used as input to the shader.
    /// 
    /// \dotfile ShaderPart_shaderString.dot
    auto_ptr< SFShaderString > shaderString;

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
  };
}

#endif
