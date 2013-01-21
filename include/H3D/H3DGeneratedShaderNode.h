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
/// \file H3DGeneratedShaderNode.h
/// \brief Header file for H3DGeneratedShaderNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DGENERATEDSHADERNODE_H__
#define __H3DGENERATEDSHADERNODE_H__

#include <H3D/ComposedShader.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/SFMatrix4f.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DGeneratedShaderNode
  /// \brief The H3DGeneratedShaderNode node is the base class for nodes
  /// that generates GLSL shaders for use in place of a ComposedShader
  /// in the Appearance field.
  ///
  /// The generated strings for vertex and fragment shader can be found
  /// in the fragmentShaderString and vertexShaderString fields.
  ///
  /// Node developer info:
  /// H3DGeneratedShaderNode sub-classes generate a fragment and a vertex shader
  /// string based on its field values. All sub-classes have to implement
  /// the appropriate virtual functions in order to add what it wants to the 
  /// shader. The shader strings are build in the buildShader function.
  /// This basically builds a vertex and fragment shader based on virtual 
  /// function calls that are implemented in sub-classes. The virtual functions 
  /// that build the shader are:
  /// 
  /// addUniformFields(), getAttributes(), getVaryingVariables().
  /// 
  /// These function add the uniform variables, attributes and varying variables
  /// needed by the shader generator. Each sub-class should implement these to 
  /// add the needed variables.
  ///
  /// getVertexShaderString(), getFragmentShaderString(), getFinalFragmentShaderOutputString(), 
  /// and getFunctionShaderString() build up the main body of code of the shaders.
  ///
  /// The structure of the generated vertex shader is:
  /// \code
  /// 
  /// uniforms
  /// varying
  /// attributes
  ///
  /// getFunctionShaderString()
  ///
  /// void main() {
  ///   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  ///   gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
  ///   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
  ///
  ///   getVertexShaderString()
  ///   varying variable definitions
  /// }
  ///
  /// \endcode
  /// 
  /// The structure of the generated fragment shader is:
  /// \code
  /// 
  /// uniforms
  /// varying
  ///
  /// getFunctionShaderString()
  ///
  /// void main() {
  ///   vec4 generated_color = vec4( 1, 1, 1, 1 );
  ///   getFragmentShaderString() 
  ///   getFinalFragmentShaderOutputString() // this is usually gl_FragColor = generated_color;
  /// }
  ///
  /// \endcode
  ///
  /// See the documentation for each function for more info.
  ///
  /// Note: When setting names for uniform and varying variables it is important that they are
  /// unique for each node that generates them so that there is no name clashing if several 
  /// nodes are combined into on shader. In order to facilitate this one can use the function
  /// uniqueShaderName( base_name ) that provides such a name. This name should then be used
  /// throughout the generated code. Local variables in generated shader code are ok since
  /// the calls are encapsulated with {} in its own space.
  /// 
  /// \par Internal routes:
  /// \dotfile H3DGeneratedShaderNode.dot
  class H3DAPI_API H3DGeneratedShaderNode : 
    public ComposedShader {
  public:
  
    /// Specialized field that rebuilds the shader when it updates
    struct RebuildShader : 
      public TypedField< SFBool, void,  AnyNumber< Field > > {
      
      virtual void update() { 
        H3DGeneratedShaderNode *vn = 
          static_cast< H3DGeneratedShaderNode * >( getOwner() );
        vn->buildShader(); 
      }
    };

    /// Constructor.
    H3DGeneratedShaderNode( Inst< DisplayList  > _displayList = 0,
                            Inst< SFNode       > _metadata    = 0,
                            Inst< SFBool       > _isSelected  = 0,
                            Inst< SFBool       > _isValid     = 0,
                            Inst< SFBool       > _activate    = 0,
                            Inst< SFString     > _language    = 0,
                            Inst< MFShaderPart > _parts       = 0,
                            Inst< SFBool       > _suppressUniformWarnings = 0,
                            Inst< MFString     > _fragmentShaderString = 0,
                            Inst< MFString     > _vertexShaderString = 0 );
    
    /// Rebuilds the generated shader, causing a full rebuild and relink of the shader. 
    virtual void buildShader();

    /// The preRender method is extended to make sure that the shader is up to date
    /// before rendering.
    virtual void preRender();

    /// Contains the generated fragment shader code.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DGeneratedShaderNode_fragmentShaderString.dot
    auto_ptr< MFString > fragmentShaderString;

    /// Contains the generated vertex shader code.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile H3DGeneratedShaderNode_vertexShaderString.dot
    auto_ptr< MFString > vertexShaderString;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    //  protected:

    /// Field used to update the shader code when an event is received.
    /// Every field that should cause the shaders to be rebuild needs
    /// to be routed to this field.
    auto_ptr< RebuildShader > rebuildShader;

    /// Adds uniform fields to the shader. A ComposedShader uses its
    /// dynamic fields to define uniform variables. Hence we need to
    /// add dynamic fields for each field that we want to be accessable
    /// in the shader.
    /// 
    /// The function does two things for each uniform variable. 
    /// 1. Adds a field to the ComposedShader that holds the value of
    /// the uniform variable.
    /// 2. Adds a glsl string to the return value that defines 
    /// the uniform variable in the shader string.
    ///
    /// \param shader The ComposedShader node to add the fields to.
    /// \return A glsl string defining the uniform variables.
    virtual string addUniformFields( ComposedShader *shader );

    /// Get a string with GLSL function definitions to be used by
    /// the generated shader.
    virtual string getFunctionShaderString() { return ""; }

    /// Struct used to define a varying variable in the shader.
    struct VaryingVariable {
      /// The name of the variable.
      string name;

      /// The glsl type of the variable(int, mat3, sampler2D etc).
      string type;

      /// String defining glsl code to set the value of the varying variable.
      /// E.g. if name is "normal", calculation can be "normal = gl_NormalMatrix * gl_Normal;"
      string calculation;
      
      /// Constructor.
      VaryingVariable( const string &_name,
                       const string &_type,
                       const string &_calculation ):
        name( _name ),
        type( _type ),
        calculation( _calculation ) {
      }
      
    };

    /// Struct used to define an attribute in the shader.
    struct Attribute {
      /// The name of the attribute.
      string name;

       /// The glsl type of the variable(int, float, etc).
      string type;

      /// Constructor.
      Attribute( const string &_name,
                 const string &_type ) :
      name( _name ),
        type( _type ) {}
    };

    /// Get the varying variables used by the shader generator.
    /// Should be implemented by subclasses to add all the varying
    /// variables that are needed by the shader generator.
    virtual void getVaryingVariables( vector< VaryingVariable > &variables ) {}

    /// Get the attribues used by the shader generator.
    /// Should be implemented by subclasses to add all the varying
    /// variables that are needed by the shader generator.
    virtual void getAttributes( vector< Attribute > &attributes ) {}

    /// Returns the string that is the last thing that is done in the
    /// fragment shader. Usually this transfers the value of 
    /// generated_color to gl_FragColor but can be changed by sub-classes
    /// to go to e.g. gl_FragData by overriding this function.
    virtual string getFinalFragmentShaderOutputString();

    /// Returns the shader code for the vertex shader. 
    /// The job of this string is to possibly change to values of 
    /// the default calculation of e.g. gl_Position before 
    /// calculating the value of the varying variables. Can be used
    /// to e.g. do deformations etc.
    virtual string getVertexShaderString() { return ""; }

    /// Returns the shader code for the fragment shader.
    ///
    /// The job of this string is to set the variable generated_color
    /// to the fragment color generated by the node. 
    /// This function should be implemented by subclasses that want to change
    /// the output of the fragment shader.
    virtual string getFragmentShaderString() { return ""; }

    /// The uniqueShaderName function is used to generate a name unique for
    /// this instance of the shader generator node class. This should to 
    /// generate names for all uniform and varying variables used
    /// in the node in order to avoid name clashes between other nodes
    /// or multiple instances of the same node type.
    ///
    /// \param base_name The original name of a variable.
    /// \return A representation of the name unique for this node.
    virtual string uniqueShaderName( const string &base_name );

    /// Convenience function to add field in addUniformFields function
    /// to add uniform to fragment shader.
    ///
    /// \param shader The ComposedShader to add the field to.
    /// \param name The name of the field.
    /// \param glsl_type The glsl type of the field(int, mat3, sampler2D etc).
    /// \param access The access type of the field.
    /// \param field The field to add to the shader.
    /// \param array_size If != -1 it assumes that the uniform is an
    /// array with this size, if not it just a normal variable.
    /// \return A glsl string defining the uniform variable just added to the
    /// shader.
    static string addUniformToFragmentShader( ComposedShader *shader,
                                              const string &name,
                                              const string &glsl_type,
                                              const Field::AccessType &access,
                                              Field *field,
                                              int array_size = -1 );

    // Duplicates the input field and routes it to the output.
    // Use this to add the render shader's auto_ptr<field> to the shader.
    template<class T>
      T* copyAndRouteField(T *input) {
      T* f = new T;
      input->route( f );
      return f;
    }
    template<class T>
      T* copyAndRouteField(const auto_ptr<T> &input) {
      T* f = new T;
      input->route( f );
      return f;
    } 

    string shader_id;

 };
}

#endif
