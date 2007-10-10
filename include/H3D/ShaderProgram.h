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
/// \file ShaderProgram.h
/// \brief Header file for ShaderProgram, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <H3D/X3DNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/X3DProgrammableShaderObject.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/SFString.h>

#ifdef HAVE_CG
#include <GL/glew.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ShaderProgram
  /// \brief The ShaderProgram node provides the source and interface to a 
  /// self contained program that occupies one part of the rendering process: 
  /// either a vertex or fragment shader.
  ///
  /// The shader source is read from the URL specified by the url field. When
  /// the url field contains no values ([]), this object instance is ignored.
  /// Shader source files shall be plain text encoded as specified for MIME 
  /// type text/plain and interpreted according to the containing node's 
  /// language definition.
  ///
  /// \par Internal routes:
  /// \dotfile ShaderProgram.dot
  class H3DAPI_API ShaderProgram : 
    public X3DNode, 
    public X3DUrlObject,
    public X3DProgrammableShaderObject,
    public H3DDisplayListObject {
  public:
  
    /// Constructor.
    ShaderProgram( Inst< SFNode      > _metadata = 0,
                   Inst< DisplayList > _displayList = 0,
                   Inst< MFString    > _url      = 0,
                   Inst< SFString    > _type     = 0 );
    
#ifdef HAVE_CG
    /// Destructor.
    virtual ~ShaderProgram();

    /// Makes the shader program active.
    virtual void enableCGShaderProgram();

    /// Disable the shader program.
    virtual void disableCGShaderProgram();

    /// Initialize a shader program from the current field values.
    virtual void initCGShaderProgram();

    /// Destroy the current cg program object. 
    virtual void destroyCGShaderProgram(); 

    /// Set the cg profile to use. The string is one of the strings 
    /// specified in the X3D specification (CG Annex K). Returns true
    /// if profile is supported.
    virtual void setCGProfile( const string &profile ); 

    /// Render sets the uniform variables of the shader program.
    virtual void render();

    /// The addField method is specialized to add a route from the field
    /// added to the displayList field.
    virtual bool addField( const string &name,
                           const Field::AccessType &access,
                           Field *field );
#endif

    /// Returns the default xml containerField attribute value.
    /// For this node it is "programs".
    virtual string defaultXMLContainerField() {
      return "programs";
    }

    /// The type field indicates whether this object shall be compiled
    /// as a vertex shader, fragment shader, or other future-defined shader 
    /// type. Valid values are "VERTEX" and "FRAGMENT".
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// \dotfile ShaderProgram_type.dot
    auto_ptr< SFString > type;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  protected:
#ifdef HAVE_CG
    CGprofile cg_profile;
    CGcontext cg_context;
    CGprogram cg_program;
    string cg_profile_string;
    string source_url;
#endif
  };
}

#endif

