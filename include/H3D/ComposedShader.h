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
/// \file ComposedShader.h
/// \brief Header file for ComposedShader, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COMPOSEDSHADER_H__
#define __COMPOSEDSHADER_H__

#include <H3D/X3DShaderNode.h>
#include <H3D/ShaderPart.h>
#include <H3D/X3DProgrammableShaderObject.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SField.h>
#include <string>
#include <H3D/ShaderFunctions.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ComposedShader
  /// \brief The ComposedShader node defines a shader where the individual 
  /// source files are not individually programmable. All access to the
  /// shading capabilities is defined through a single interface that 
  /// applies to all parts.
  ///
  /// In H3D API this is the shader node to use if you want to use the
  /// GLSL shading language. If you want to use CG, use either the 
  /// ProgramShader or PackagedShader nodes.
  ///
  /// The isValid field adds an additional semantic indicating whether 
  /// the current shader parts can be linked together to form a complete
  /// valid shader program.
  /// 
  /// The activate field forces the shader to activate the contained 
  /// objects. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ComposedShader.x3d">ComposedShader.x3d</a>
  ///     ( <a href="examples/ComposedShader.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ComposedShader.dot
  /// 


//#define EXPORT_SHADER
  
  class H3DAPI_API ComposedShader : 
    public X3DShaderNode, 
    public X3DProgrammableShaderObject {
  public:
  
    

    // a map to maintain the uniform values and their related properties 
    std::map< string , H3D::Shaders::UniformInfo > uniformFields; 
    /// The MFShaderPart is dependent on the url field of the
    /// containing ShaderPart node.
    typedef DependentMFNode< ShaderPart,
                             FieldRef< X3DUrlObject, 
                                       MFString, 
                                       &ShaderPart::url >,
                             true >
    MFShaderPart;

#ifdef EXPORT_SHADER
    /// UpdateSaveShadersToUrl 
    class H3DAPI_API UpdateSaveShadersToUrl: public OnNewValueSField < AutoUpdate < SFString > >
    {
      protected:
        virtual void onNewValue( const std::string &v );
    };
    

#endif

    /// Constructor.
    ComposedShader( Inst< DisplayList  > _displayList = 0,
                    Inst< SFNode       > _metadata    = 0,
                    Inst< SFBool       > _isSelected  = 0,
                    Inst< SFBool       > _isValid     = 0,
                    Inst< SFBool       > _activate    = 0,
                    Inst< SFString     > _language    = 0,
                    Inst< MFShaderPart > _parts       = 0,
                    Inst< SFBool       > _suppressUniformWarnings = 0,
                    Inst< SFString     > _geometryInputType = 0,
                    Inst< SFString     > _geometryOutputType = 0,
                    Inst< SFInt32      > _geometryVerticesOut = 0,
                    Inst< SFString     > _transparencyDetectMode = 0,
                    Inst< MFString     > _transformFeedbackVaryings = 0
#ifdef EXPORT_SHADER
                    ,
                    Inst< UpdateSaveShadersToUrl > _saveShadersToUrl = 0
#endif
                    );

    /// The addField method is specialized to add a route from the field
    /// added to the displayList field.
    virtual bool addField( const string &name,
                           const Field::AccessType &access,
                           Field *field );
    
    /// Traverse the node
    ///
    /// Determines if GL_PATCHES should be rendered by geometry, if
    /// a tessellation shader is present
    virtual void traverseSG ( TraverseInfo& ti );

    /// Sets up the shader program and sets uniform variable values.
    virtual void render();

    /// Enables the shader program.
    virtual void preRender();

    /// Disables the shader program.
    virtual void postRender();

    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs();

    /// Returns a hint if the X3DShaderNode produces an alpha channel
    /// that is < 1, i.e. it is semi-transparent, or not. This
    /// is used to determine render order of transparent vs opaque
    /// objects.
    virtual bool isTransparent( X3DMaterialNode *material );

    /// Returns the OpenGL shader program handle used by the 
    /// ComposedShader.
    GLhandleARB getProgramHandle() {
      return program_handle;
    }
   
#ifdef EXPORT_SHADER
    /// field to update shader program exporting
    auto_ptr< UpdateSaveShadersToUrl > saveShadersToUrl;
#endif
    

    /// The shader parts to use in the ComposedShader 
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile ComposedShader_parts.dot
    auto_ptr< MFShaderPart > parts;

    /// Suppress the warnings printed to the console about setting uniform fields
    /// that do not exist in the shader. These warnings are printed when you have
    /// added a field to your shader which is not defined in the glsl shader code,
    /// if the uniform variable is defined in the shader code but not used anywhere,
    /// or when the variable is used in the code but optimized away by the glsl 
    /// compiler. Usually we would only like to print a warning in the first case
    /// and not in the others, but there is no way to distinguish between them.
    /// We therefore leave it to the setting of this field if they should be
    /// printed or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile ComposedShader_parts.dot
    auto_ptr< SFBool > suppressUniformWarnings;

    /// The input geometry type of the geometry being rendered
    /// with a geometry shader. 
    /// Only used when type a ShaderPart is of type "GEOMETRY".
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "TRIANGLES" \n
    /// <b>Valid values:</b> "POINTS", "LINES", "TRIANGLES", 
    /// "LINES_ADJACENCY", "TRIANGLES_ADJACENCY",
    /// \dotfile ComposedShader_geometryInputType.dot
    auto_ptr< SFString > geometryInputType;

    /// The output geometry type of the geometry being rendered
    /// with a geometry shader. 
    /// Only used when type a ShaderPart is of type "GEOMETRY".
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "TRIANGLE_STRIP" \n
    /// <b>Valid values:</b> "POINTS", "LINE_STRIP", "TRIANGLE_STRIP"
    /// \dotfile ComposedShader_geometryOutputType.dot
    auto_ptr< SFString > geometryOutputType;

    /// The number of vertices the geometry shader generates. 
    /// Only used when type a ShaderPart is of type "GEOMETRY".
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 64 \n
    ///
    /// \dotfile ComposedShader_geometryVerticesOut.dot
    auto_ptr< SFInt32  > geometryVerticesOut;


    /// Determines how the render system will determine if the 
    /// shader used generates any fragment that has an alpha channel
    /// that is < 1, i.e. it is semi-transparent, or not. This
    /// is used to determine render order of transparent vs opaque
    /// objects.
    ///
    /// Valid values are:
    /// <table>
    /// <tr><td>"AS_MATERIAL"</td><td>If the Material node in the same
    /// Appearance node produces a transparent object we consider the 
    /// ComposedShader to be transparent.</td></tr>
    /// <tr><td>"TRANSPARENT"</td><td>The shader is considered transparent.</td></tr>
    /// <tr><td>"OPAQUE"</td><td>The shader is considered opaque.</td></tr>
    /// </table>
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Valid values:</b> "AS_MATERIAL", "TRANSPARENT", "OPAQUE" \n
    /// <b>Default value:</b> "AS_MATERIAL" \n
    ///
    /// \dotfile ComposedShader_transparencyDetectMode.dot
    auto_ptr< SFString  > transparencyDetectMode;

    /// List of varying names to capture during transform feedback. Allows the use
    /// of the OpenGL feature 'transform feedback'. Which makes it possible to
    /// render to a buffer on the gpu using a vertex ( and optionally geometry )
    /// shader(s) for later user. The listed names must match an output variable
    /// of the shader.
    /// See http://www.opengl.org/sdk/docs/man3/xhtml/glTransformFeedbackVaryings.xml
    /// for more info.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    auto_ptr< MFString > transformFeedbackVaryings;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:

    /// A field used to update any uniforms from their corresponding fields
    class UpdateUniforms : public EventCollectingField < Field > {
      virtual void update ();
    };

    static bool shader_support_checked;

    /// True if a warning has already been output about
    /// missing tessellation shader support
    static bool tessellation_support_checked;

    /// The handle to the program object used for the shader in OpenGL.
    GLhandleARB program_handle;

    /// A vector of the handles to all shader objects that are currently
    /// linked into the program object.
    vector< GLhandleARB > current_shaders;

    // list of program handles separated by key
    static map<string, GLhandleARB> phandles_map;
    static map<GLhandleARB, int> phandle_counts;

    // create program handle for a shader
    static GLhandleARB createHandle(ComposedShader* shader);

    // try to find existing handle (with the same signature), if not, go create one
    static std::string genKeyFromShader(ComposedShader* shader);

    /// Sets geometry shader paramters based on fields.
    void setGeometryShaderParameters( GLenum program_handle);

    /// Class used to set up routes from nodes in SFNode and MFNode fields
    /// to correctly break cache. Since it is impossible to know which fields
    /// of a node should break cache only nodes inheriting from
    /// H3DDisplayListObject actually breaks cache.
    class SetupDynamicRoutes : public AutoUpdate< Field > {
    protected:
      virtual void update();
      // Map from Fields to nodes in the fields, needed in
      // order to remove nodes properly.
      map< Field *, NodeVector > fields_to_nodes;
    };

    /// C++ fields only. Contains instance of SetupDynamicRoutes.
    auto_ptr< SetupDynamicRoutes > setupDynamicRoutes;

    /// A field used to update any uniforms from their corresponding fields
    auto_ptr<UpdateUniforms> updateUniforms;
    
    bool printShaderLog();
    DebugOptions *debug_options_previous;
  public:
    virtual void initialize();
  };
}

#endif
