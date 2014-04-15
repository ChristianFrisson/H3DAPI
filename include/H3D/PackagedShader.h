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
/// \file PackagedShader.h
/// \brief Header file for PackagedShader, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PACKAGEDSHADER_H__
#define __PACKAGEDSHADER_H__

#include <H3D/X3DShaderNode.h>
#include <H3D/X3DProgrammableShaderObject.h>
#include <H3D/X3DUrlObject.h>
#include <GL/glew.h>

#ifdef HAVE_CG
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PackagedShader
  /// \brief A PackagedShader node describes a single file that may contain a 
  /// number of shaders and combined effects.
  ///
  /// The shader source is read from the URL specified by the url field. 
  ///
  /// The language field may be used to optionally determine the language type.
  /// Only the CG language strings are supported by H3D API. If you want to
  /// use GLSL use the ComposedShader node.
  ///
  /// The main function of the CG vertex shader must be named "vert_main" and
  /// the main function of the fragment shader must be named "frag_main".
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PackagedShader.x3d">PackagedShader.x3d</a>
  ///     ( <a href="examples/PackagedShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile PackagedShader.dot
  class H3DAPI_API PackagedShader : 
    public X3DShaderNode, 
    public X3DUrlObject,
    public X3DProgrammableShaderObject {
  public:
  
    /// Constructor.
    PackagedShader( Inst< DisplayList  > _displayList = 0,
                    Inst< SFNode       > _metadata    = 0,
                    Inst< SFBool       > _isSelected  = 0,
                    Inst< SFBool       > _isValid     = 0,
                    Inst< SFBool       > _activate    = 0,
                    Inst< SFString     > _language    = 0,
                    Inst< MFString     > _url         = 0 );

    /// Returns true if the shader node type is supported by the browser.
    virtual bool isSupported() {
      const string &l = language->getValue();
      bool is_cg = 
        ( l == "CG" || l == "CG_OPENGL_ARB" || l == "CG_OPENGL_NV40" 
          || l == "CG_OPENGL_NV30" || l == "CG_OPENGL_NV20" );
#ifndef HAVE_CG
      if( is_cg ) {
        Console(4) << "Warning: H3D API compiled without cg support. PackagedShader "
                   << " node will not support CG." << endl;
      }
      return false;
#endif
      return is_cg;  
    }


#ifdef HAVE_CG
    /// The addField method is specialized to add a route from the field
    /// added to the displayList field.
    virtual bool addField( const string &name,
                           const Field::AccessType &access,
                           Field *field );

    /// Sets up the shader program and sets uniform variable values.
    virtual void render();

    /// Enables the shader program.
    virtual void preRender();

    /// Disables the shader program.
    virtual void postRender();

    /// Initialize a cg shader program from the current field values.
    virtual void initCGShaderProgram();
#endif

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

#ifdef HAVE_CG
  protected:
    CGprofile cg_vertex_profile;
    CGprofile cg_fragment_profile;
    CGcontext cg_context;
    CGprogram cg_vertex_program;
    CGprogram cg_fragment_program;
    string source_url;
#endif
  };
}

#endif



