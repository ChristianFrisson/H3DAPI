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
/// \file ShaderCombiner.h
/// \brief Header file for ShaderCombiner.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADERCOMBINER_H__
#define __SHADERCOMBINER_H__

#include <H3D/H3DGeneratedFragmentShaderNode.h>
#include <H3D/H3DGeneratedVertexShaderNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ShaderCombiner
  /// \brief The ShaderCombiner node combines the output of several fragment
  /// shaders in different ways to produce a final value by a combination
  /// of the output values of the shaders.
  ///
  /// Several functions are provided both to modify the shader output 
  /// before combining and to combine the different shaders. Alpha channel
  /// and rgb values are treated separately with all fields affecting alpha
  /// starts with alpha..
  ///
  /// There are two kinds of functions: combine functions and modifiers.
  /// Combine functions combine two values in some way and modifiers
  /// change a single value. 
  ///
  /// Let the input to the combine functions be 
  /// called arg0 and arg1, the combine function f and the modifier function
  /// for arg0 and arg1 be mod0 and mod1 respectively.
  /// 
  /// The value from a combine function is then:
  /// value = f( mod0( arg0 ), mod1( arg1 ) )
  ///
  /// The function and alphaFunction field specifies the combine function
  /// to use for RGB values and alpha values respectively. For RGB values
  /// all operations are component-wise and for arg0 and arg1 below have 
  /// the modifier already applied:
  /// <table>
  /// <tr><td>"SELECT0"</td><td>arg0</td></tr>
  /// <tr><td>"SELECT1"</td><td>arg1</td></tr>
  /// <tr><td>"ADD"</td><td>arg0 + arg1</td></tr>
  /// <tr><td>"SUBTRACT"</td><td>arg0 - arg1</td></tr>
  /// <tr><td>"MULTIPLY"</td><td>arg0 * arg1</td></tr>
  /// <tr><td>"DIVIDE"</td><td>arg0 / arg1</td></tr>
  /// <tr><td>"BLEND_VALUE"</td><td> value * arg0 + (1-value)*arg1 where value is the value field.</td></tr>
  /// </table>
  /// 
  /// For the RGB modifier fields arg0Modifyier and arg1Modifier the following
  /// values are possible. The modifiers are applied before the combine 
  /// function and is per component. value in the table below is arg0Value
  /// or arg1Value depending on which argument is modified.
  /// <table>
  /// <tr><td>"COMPLEMENT"</td><td>1-v</td></tr>
  /// <tr><td>"ADD"</td><td>v + value</td></tr>
  /// <tr><td>"SUBTRACT"</td><td>v - value</td></tr>
  /// <tr><td>"MULTIPLY"</td><td>v * value</td></tr>
  /// <tr><td>"DIVIDE"</td><td>v / value</td></tr>
  /// <tr><td>"NEGATE"</td><td> -v </td></tr>
  /// <tr><td>"MULTIPLY_ALPHA"</td><td>v.rgb * v.a.</td></tr>
  /// <tr><td>"MULTIPLY_ONE_MINUS_ALPHA" </td><td>v.rgb * (1-v.a)</td></tr>
  /// </table>
  ///
  /// For the alpha modifier fields alphaArg0Modifyier and alphaArg1Modifier
  /// the following values are possible. The modifiers are applied before
  /// the combine function and is per component. value in the table below
  /// is alphaArg0Value or alphaArg1Value depending on which argument
  /// is modified.
  ///
  /// <table>
  /// <tr><td>"COMPLEMENT"</td><td>1-v</td></tr>
  /// <tr><td>"ADD"</td><td>v + value</td></tr>
  /// <tr><td>"SUBTRACT"</td><td>v - value</td></tr>
  /// <tr><td>"MULTIPLY"</td><td>v * value</td></tr>
  /// <tr><td>"DIVIDE"</td><td>v / value</td></tr>
  /// <tr><td>"NEGATE"</td><td> -v </td></tr>
  /// </table>
  /// 
  /// The shaders field contains the shaders to be combined.  If only one
  /// shader is provided only the arg0Modifyier and alphaArg0Modifier is 
  /// applied and the function is ignored.
  /// 
  /// If more than two shaders are provided the functions are applied
  /// in succession like e.g. for three shaders:
  ///
  /// \code
  /// [s0, s1, s2]  
  /// t0 = func( mod0(s0), mod1(s1) )
  /// final_color = func( mod0( t0 ), mod1( s2) ) 
  /// \endcode
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ShaderCombiner.x3d">ShaderCombiner.x3d</a>
  ///     ( <a href="examples/ShaderCombiner.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/ShaderCombiner_functions.x3d">ShaderCombiner_functions.x3d</a>
  ///     ( <a href="examples/ShaderCombiner_functions.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ShaderCombiner.dot
  class H3DAPI_API ShaderCombiner : 
    public H3DGeneratedShaderNode {
  public:
  
    typedef DependentMFNode< H3DGeneratedFragmentShaderNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >
      MFGeneratedFragmentShaderNode;    


    /// Constructor.
    ShaderCombiner( Inst< DisplayList  > _displayList = 0,
		    Inst< SFNode       > _metadata    = 0,
		    Inst< SFBool       > _isSelected  = 0,
		    Inst< SFBool       > _isValid     = 0,
		    Inst< SFBool       > _activate    = 0,
		    Inst< SFString     > _language    = 0,
		    Inst< MFShaderPart > _parts       = 0,
		    Inst< SFBool       > _suppressUniformWarnings = 0,
		    Inst< MFString     > _fragmentShaderString = 0,
		    Inst< MFString     > _vertexShaderString = 0,
		    Inst< MFGeneratedFragmentShaderNode > _shaders = 0,
		    Inst< SFString     > _function = 0,
		    Inst< SFString     > _arg0Modifier = 0,
		    Inst< SFString     > _arg1Modifier = 0,
		    Inst< SFFloat      > _value = 0,
		    Inst< SFFloat      > _arg0Value = 0,
		    Inst< SFFloat      > _arg1Value = 0,
		    Inst< SFString     > _alphaFunction = 0,
		    Inst< SFString     > _alphaArg0Modifier = 0,
		    Inst< SFString     > _alphaArg1Modifier = 0,
		    Inst< SFFloat      > _alphaValue = 0,
		    Inst< SFFloat      > _alphaArg0Value = 0,
		    Inst< SFFloat      > _alphaArg1Value = 0 );
    

    /// Traverse the scene-graph.
    virtual void traverseSG( TraverseInfo &ti );
 
    /// The shaders field specifies a number of H3DGeneratedFragmentShaderNode
    /// instances that are to be combined.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ShaderCombiner_shaders.dot
    auto_ptr< MFGeneratedFragmentShaderNode > shaders;

    /// The function field specifies the combine function
    /// to use for RGB values. For RGB values
    /// all operations are component-wise and for arg0 and arg1 below have 
    /// the modifier already applied:
    /// <table>
    /// <tr><td>"SELECT0"</td><td>arg0</td></tr>
    /// <tr><td>"SELECT1"</td><td>arg1</td></tr>
    /// <tr><td>"ADD"</td><td>arg0 + arg1</td></tr>
    /// <tr><td>"SUBTRACT"</td><td>arg0 - arg1</td></tr>
    /// <tr><td>"MULTIPLY"</td><td>arg0 * arg1</td></tr>
    /// <tr><td>"DIVIDE"</td><td>arg0 / arg1</td></tr>
    /// <tr><td>"BLEND_VALUE"</td><td> value * arg0 + (1-value)*arg1 where value is the value field.</td></tr>
    /// </table> 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>"SELECT1"  \n
    /// 
    /// \dotfile ShaderCombiner_function.dot
    auto_ptr< SFString > function;

    /// arg0Modifyier field specifies a modifier function that can
    /// be applied to the RGB output of the shader before being combined.
    /// Operations are component-wise. Possible values are:
    ///
    /// <table>
    /// <tr><td>"COMPLEMENT"</td><td>1-v</td></tr>
    /// <tr><td>"ADD"</td><td>v + arg0Value</td></tr>
    /// <tr><td>"SUBTRACT"</td><td>v - arg0Value</td></tr>
    /// <tr><td>"MULTIPLY"</td><td>v * arg0Value</td></tr>
    /// <tr><td>"DIVIDE"</td><td>v / arg0Value</td></tr>
    /// <tr><td>"NEGATE"</td><td> -v </td></tr>
    /// <tr><td>"MULTIPLY_ALPHA" </td><td>v.rgb * v.a.</td></tr>
    /// <tr><td>"MULTIPLY_ONE_MINUS_ALPHA" </td><td>v.rgb * (1-v.a)</td></tr>
    /// </table>
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>"NONE"  \n
    /// 
    /// \dotfile ShaderCombiner_arg0Modifier.dot
    auto_ptr< SFString > arg0Modifier;
    
    /// arg1Modifyier field specifies a modifier function that can
    /// be applied to the RGB output of the shader before being combined.
    /// Operations are component-wise. Possible values are:
    ///
    /// <table>
    /// <tr><td>"COMPLEMENT"</td><td>1-v</td></tr>
    /// <tr><td>"ADD"</td><td>v + arg1Value</td></tr>
    /// <tr><td>"SUBTRACT"</td><td>v - arg1Value</td></tr>
    /// <tr><td>"MULTIPLY"</td><td>v * arg1Value</td></tr>
    /// <tr><td>"DIVIDE"</td><td>v / arg1Value</td></tr>
    /// <tr><td>"NEGATE"</td><td> -v </td></tr>
    /// <tr><td>"MULTIPLY_ALPHA" </td><td>v.rgb * v.a.</td></tr>
    /// <tr><td>"MULTIPLY_ONE_MINUS_ALPHA" </td><td>v.rgb * (1-v.a)</td></tr>
    /// </table>
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>"NONE"  \n
    /// 
    /// \dotfile ShaderCombiner_arg1Modifier.dot
    auto_ptr< SFString > arg1Modifier;

    /// The value field specifies a value that is used together with some
    /// of the combiner functions, e.g. the "BLEND_VALUE" function.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>0  \n
    /// 
    /// \dotfile ShaderCombiner_value.dot
    auto_ptr< SFFloat  > value;

    /// The arg0Value field specifies a value that is used together with some
    /// of the arg0Modifier functions.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>0  \n
    /// 
    /// \dotfile ShaderCombiner_value.dot
    auto_ptr< SFFloat  > arg0Value;

    /// The arg0Value field specifies a value that is used together with some
    /// of the arg0Modifier functions.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>0  \n
    /// 
    /// \dotfile ShaderCombiner_value.dot
    auto_ptr< SFFloat  > arg1Value;

    /// The function field specifies the combine function
    /// to use for alpha values. arg0 and arg1 have already
    /// had its modifier applied to it.
    ///
    /// <table>
    /// <tr><td>"SELECT0"</td><td>arg0</td></tr>
    /// <tr><td>"SELECT1"</td><td>arg1</td></tr>
    /// <tr><td>"ADD"</td><td>arg0 + arg1</td></tr>
    /// <tr><td>"SUBTRACT"</td><td>arg0 - arg1</td></tr>
    /// <tr><td>"MULTIPLY"</td><td>arg0 * arg1</td></tr>
    /// <tr><td>"DIVIDE"</td><td>arg0 / arg1</td></tr>
    /// <tr><td>"BLEND_VALUE"</td><td> alphaValue * arg0 + (1-value)*arg1 where value is the value field.</td></tr>
    /// </table> 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>"SELECT1"  \n
    /// 
    /// \dotfile ShaderCombiner_alphaFunction.dot
    auto_ptr< SFString > alphaFunction;

    /// alphaArg0Modifyier field specifies a modifier function that can
    /// be applied to the alpha output of the shader before being combined.
    /// Operations are component-wise. Possible values are:
    ///
    /// <table>
    /// <tr><td>"COMPLEMENT"</td><td>1-v</td></tr>
    /// <tr><td>"ADD"</td><td>v + alphaArg0Value</td></tr>
    /// <tr><td>"SUBTRACT"</td><td>v - alphaArg0Value</td></tr>
    /// <tr><td>"MULTIPLY"</td><td>v * alphaArg0Value</td></tr>
    /// <tr><td>"DIVIDE"</td><td>v / alphaArg0Value</td></tr>
    /// <tr><td>"NEGATE"</td><td> -v </td></tr>
    /// </table>
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>"NONE"  \n
    /// 
    /// \dotfile ShaderCombiner_alphaArg0Modifier.dot
    auto_ptr< SFString > alphaArg0Modifier;

    /// alphaArg1Modifyier field specifies a modifier function that can
    /// be applied to the alpha output of the shader before being combined.
    /// Operations are component-wise. Possible values are:
    ///
    /// <table>
    /// <tr><td>"COMPLEMENT"</td><td>1-v</td></tr>
    /// <tr><td>"ADD"</td><td>v + alphaArg1Value</td></tr>
    /// <tr><td>"SUBTRACT"</td><td>v - alphaArg1Value</td></tr>
    /// <tr><td>"MULTIPLY"</td><td>v * alphaArg1Value</td></tr>
    /// <tr><td>"DIVIDE"</td><td>v / alphaArg1Value</td></tr>
    /// <tr><td>"NEGATE"</td><td> -v </td></tr>
    /// </table>
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>"NONE"  \n
    /// 
    /// \dotfile ShaderCombiner_alphaArg1Modifier.dot
    auto_ptr< SFString > alphaArg1Modifier;

    /// The alphavalue field specifies a value that is used together with some
    /// of the alpha combiner functions, e.g. the "BLEND_VALUE" function.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>0  \n
    /// 
    /// \dotfile ShaderCombiner_alphaValue.dot
    auto_ptr< SFFloat  > alphaValue;

    /// The alphaArg0Value field specifies a value that is used together with
    /// some of the alphaArg0Modifier functions.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>0  \n
    /// 
    /// \dotfile ShaderCombiner_alphaArg0Value.dot
    auto_ptr< SFFloat  > alphaArg0Value;

    /// The alphaArg1Value field specifies a value that is used together with
    /// some of the alphaArg1Modifier functions.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>0  \n
    /// 
    /// \dotfile ShaderCombiner_alphaArg1Value.dot    
    auto_ptr< SFFloat  > alphaArg1Value;

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

    /// Returns the shader code for the fragment shader.
    virtual string getVertexShaderString();

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

    /// Returns a glsl string to perform the specified modifier function.
    /// It returns a line
    /// variable_name.rgb = ...;
    /// that modifies the variable according to the modifier name.
    /// Possible values for modifier is the same as for the arg0Modifier
    /// field.
    string applyModifier( const string &variable_name,
			  const string &modifier,
			  const string &value = "0" );

    /// Returns a glsl string to perform the specified modifier function.
    /// It returns a line
    /// variable_name.a = ...;
    /// that modifies the variable according to the modifier name.
    /// Possible values for modifier is the same as for the alphaArg0Modifier
    /// field.
    string applyAlphaModifier( const string &variable_name,
			       const string &modifier,
			       const string &value = "0" );

    /// Returns a glsl string that evaluates to the variable v0 and v1 combined
    /// with the specified funtion.
    /// Possible values for modifier is the same as for the function
    /// field.
    string combineFunction( const string &v0,
			    const string &v1,
			    const string &function,
			    const string &value = "0" );
  };
}

#endif
