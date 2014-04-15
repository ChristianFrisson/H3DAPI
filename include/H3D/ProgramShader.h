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
/// \file ProgramShader.h
/// \brief Header file for ProgramShader, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PROGRAMSHADER_H__
#define __PROGRAMSHADER_H__

#include <H3D/X3DShaderNode.h>
#include <H3D/ShaderProgram.h>
#include <H3D/X3DProgrammableShaderObject.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ProgramShader
  /// \brief he ProgramShader node defines a shader that can consist 
  /// of one or more individually programmable, self contained pieces. 
  /// Each piece, represented by a ShaderProgram node, shall be a 
  /// self-contained source that does not rely on any other source file
  /// and can manage one part of the programmable pipeline (e.g., vertex or
  /// fragment processing). 
  /// 
  /// This node can be used with the CG shading language in H3D API. You 
  /// can also use the PackagedShader node with CG. If you want to use GLSL
  /// you will have to use the ComposedShader node.
  ///
  /// The programs field consists of zero or more ShaderProgram node 
  /// instances. In general, only two ShaderProgram instances will be needed:
  /// one each for vertex and fragment processing. Each shader language annex
  /// shall refine the required behaviour for processing this field.
  ///
  /// The isValid field may add an additional semantic to indicate whether
  /// all parts are available.  
  ///
  /// \par Internal routes:
  /// \dotfile ProgramShader.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ProgramShader.x3d">ProgramShader.x3d</a>
  ///     ( <a href="examples/ProgramShader.x3d.html">Source</a> )
  class H3DAPI_API ProgramShader : 
    public X3DShaderNode {
  public:
  
    /// The MFShaderProgram class sets the profile for each 
    /// ShaderProgram node that is added to it.
    class H3DAPI_API MFShaderProgram: 
      public DependentMFNode< ShaderProgram, 
                              FieldRef< H3DDisplayListObject,
                                        H3DDisplayListObject::DisplayList,
                                        &H3DDisplayListObject::displayList >, 
                              true > {
    protected:
      virtual void onAdd( Node * n ) {
        DependentMFNode< ShaderProgram, 
                         FieldRef< H3DDisplayListObject,
                                   H3DDisplayListObject::DisplayList,
                                   &H3DDisplayListObject::displayList >, 
                         true >::onAdd( n );
#ifdef HAVE_CG
        ShaderProgram *sp = dynamic_cast< ShaderProgram * >( n );
        if( sp ) {
          ProgramShader *ps = static_cast< ProgramShader * >( getOwner() );
          sp->setCGProfile( ps->language->getValue() );
        }
#endif
      }
    };

    /// Constructor.
    ProgramShader( Inst< DisplayList     > _displayList = 0,
                   Inst< SFNode          > _metadata    = 0,
                   Inst< SFBool          > _isSelected  = 0,
                   Inst< SFBool          > _isValid     = 0,
                   Inst< SFBool          > _activate    = 0,
                   Inst< SFString        > _language    = 0,
                   Inst< MFShaderProgram > _programs    = 0 );
    
    /// Returns true if the shader node type is supported by the browser.
    virtual bool isSupported() {
      const string &l = language->getValue();
      bool is_cg = 
        ( l == "CG" || l == "CG_OPENGL_ARB" || l == "CG_OPENGL_NV40" 
          || l == "CG_OPENGL_NV30" || l == "CG_OPENGL_NV20" );
#ifndef HAVE_CG
      if( is_cg ) {
        Console(4) << "Warning: H3D API compiled without cg support. ProgramShader "
                   << " node will not support CG." << endl;
      }
      return false;
#endif
      return is_cg;  
    }

#ifdef HAVE_CG
    /// Sets up the shaders program and sets uniform variable values.
    virtual void render();

    /// Enables the shader programs.
    virtual void preRender();

    /// Disables the shader programs.
    virtual void postRender();
#endif

    /// The shader programs to use in the ProgramShader 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile ProgramShader_programs.dot
    auto_ptr< MFShaderProgram > programs;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  };
}

#endif
