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
/// \file X3DShaderNode.h
/// \brief Header file for X3DShaderNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSHADERNODE_H__
#define __X3DSHADERNODE_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3D/SFString.h>
#include <H3D/X3DMaterialNode.h>

namespace H3D {
  /// \ingroup AbstractNodes
  /// \class X3DShaderNode
  /// \brief This abstract node type is the base type for all node types which
  ///specify a programmable shader.
  ///
  /// The isSelected output field is used to indicate that this shader instance
  /// is the one selected for use by the browser. A TRUE value indicates that 
  /// this instance is in use. A FALSE value indicates that this instance is
  /// not in use. 
  ///
  /// The isValid field is used to indicate whether the current shader objects 
  /// can be run as a shader program. For example, there are no syntax errors
  /// and the hardware can support all the required features. The definition 
  /// of this field may also add additional semantics on a per-language basis.
  /// 
  /// The language field is used to indicate to the browser which shading 
  /// language is used for the source file(s). This field may be used as a 
  /// hint for the browser if the shading language is not immediately 
  /// determinable from the source (e.g., a generic MIME type of text/plain
  /// is returned). A browser may use this field for determining which node
  /// instance will be selected and to ignore languages that it is not capable
  /// of supporting. H3D API only supports the "GLSL" language.
  class H3DAPI_API X3DShaderNode : 
    public X3DAppearanceChildNode {
  public:
  
    /// Constructor.
    X3DShaderNode( Inst< DisplayList > _displayList = 0,
                   Inst< SFNode      > _metadata    = 0,
                   Inst< SFBool      > _isSelected  = 0,
                   Inst< SFBool      > _isValid     = 0,
                   Inst< SFBool      > _activate    = 0,
                   Inst< SFString    > _language    = 0);
    
    /// Returns true if the shader node type is supported by the browser.
    /// By default it checks that the language is GLSL.
    virtual bool isSupported() {
      return language->getValue() == "GLSL"; 
    }

    /// Set the isSelected field.
    void setSelected( bool selected ) {
      isSelected->setValue( selected, id );
    }

    /// Returns the default xml containerField attribute value.
    /// For this node it is "shader".
    ///
    virtual string defaultXMLContainerField() {
      return "shaders";
    }

    /// Returns a hint if the X3DShaderNode produces an alpha channel
    /// that is < 1, i.e. it is semi-transparent, or not. This
    /// is used to determine render order of transparent vs opaque
    /// objects.
    virtual bool isTransparent( X3DMaterialNode *material ) {
      return false;
    }

    /// Gets the currently active shader, i.e. the shader that is currently 
    /// used in OpenGL. NULL us returned if no X3DShaderNode is used. 
    static X3DShaderNode *getActiveShader() { return active_shader; }

    /// The isSelected output field is used to indicate that this shader
    /// instance is the one selected for use by the browser. A TRUE value
    /// indicates that this instance is in use. A FALSE value indicates 
    /// that this instance is not in use.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFBool > isSelected;

    /// The isValid field is used to indicate whether the current shader
    /// objects can be run as a shader program. For example, there are no
    /// syntax errors and the hardware can support all the required features.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFBool > isValid;

    /// The activate field forces the shader to activate the contained 
    /// objects.
    /// 
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< SFBool > activate;

    /// The language field is used to indicate to the browser which shading 
    /// language is used for the source file(s). This field is ignored in H3D 
    /// API since only "GLSL" source is allowed.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFString > language;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    protected:
    static X3DShaderNode *active_shader;
  };
}

#endif
