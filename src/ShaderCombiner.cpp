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
/// \file ShaderCombiner.cpp
/// \brief CPP file for ShaderCombiner.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderCombiner.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShaderCombiner::database( 
                                   "ShaderCombiner", 
                                   &newInstance< ShaderCombiner >,
                                   typeid( ShaderCombiner ),
                                   &H3DGeneratedFragmentShaderNode::database );

namespace ShaderCombinerInternals {
  FIELDDB_ELEMENT( ShaderCombiner, shaders, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, function, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, arg0Modifier, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, arg1Modifier, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, value, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, arg0Value, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, arg1Value, INPUT_OUTPUT );

  FIELDDB_ELEMENT( ShaderCombiner, alphaFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, alphaArg0Modifier, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, alphaArg1Modifier, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, alphaValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, alphaArg0Value, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderCombiner, alphaArg1Value, INPUT_OUTPUT );
}

ShaderCombiner::ShaderCombiner( Inst< DisplayList  > _displayList,
				Inst< SFNode       > _metadata,
				Inst< SFBool       > _isSelected ,
				Inst< SFBool       > _isValid,
				Inst< SFBool       > _activate,
				Inst< SFString     > _language,
				Inst< MFShaderPart > _parts,
				Inst< SFBool       > _suppressUniformWarnings,
				Inst< MFString > _fragmentShaderString,
				Inst< MFString > _vertexShaderString,
				Inst< MFGeneratedFragmentShaderNode > _shaders,
                                Inst< SFString     > _function ,
				Inst< SFString     > _arg0Modifier,
				Inst< SFString     > _arg1Modifier,
				Inst< SFFloat      > _value,
				Inst< SFFloat      > _arg0Value,
				Inst< SFFloat      > _arg1Value,
				Inst< SFString     > _alphaFunction ,
				Inst< SFString     > _alphaArg0Modifier,
				Inst< SFString     > _alphaArg1Modifier,
				Inst< SFFloat      > _alphaValue,
				Inst< SFFloat      > _alphaArg0Value,
				Inst< SFFloat      > _alphaArg1Value ) :
  H3DGeneratedFragmentShaderNode( _displayList, _metadata, _isSelected, 
				  _isValid, _activate, _language, _parts, 
				  _suppressUniformWarnings, _fragmentShaderString, 
				  _vertexShaderString ),
  shaders( _shaders ),
  function( _function ),
  arg0Modifier( _arg0Modifier ),
  arg1Modifier( _arg1Modifier ),
  value( _value ),
  arg0Value( _arg0Value ),
  arg1Value( _arg1Value ),
  alphaFunction( _alphaFunction ),
  alphaArg0Modifier( _alphaArg0Modifier ),
  alphaArg1Modifier( _alphaArg1Modifier ),
  alphaValue( _alphaValue ),
  alphaArg0Value( _alphaArg0Value ),
  alphaArg1Value( _alphaArg1Value ) {
  
  type_name = "ShaderCombiner";
  database.initFields( this );

  // default values
  value->setValue( 0 );
  arg0Value->setValue( 0 );  
  arg1Value->setValue( 0 );

  alphaValue->setValue( 0 );
  alphaArg0Value->setValue( 0 );  
  alphaArg1Value->setValue( 0 );
  
  function->addValidValue( "SELECT0" );
  function->addValidValue( "SELECT1" );
  function->addValidValue( "ADD" );
  function->addValidValue( "SUBTRACT" );
  function->addValidValue( "MULTIPLY" );
  function->addValidValue( "DIVIDE" );
  function->addValidValue( "BLEND_VALUE" );
  //  function->addValidValue( "FRESNEL" );
  function->setValue( "SELECT1" );

 
  alphaFunction->addValidValue( "SELECT0" );
  alphaFunction->addValidValue( "SELECT1" );
  alphaFunction->addValidValue( "ADD" );
  alphaFunction->addValidValue( "SUBTRACT" );
  alphaFunction->addValidValue( "MULTIPLY" );
  alphaFunction->addValidValue( "DIVIDE" );
  alphaFunction->addValidValue( "BLEND_VALUE" );
  //  alphaFunction->addValidValue( "FRESNEL" );
  alphaFunction->setValue( "SELECT1" );
  
  arg0Modifier->addValidValue( "NONE" );
  arg0Modifier->addValidValue( "COMPLEMENT" );
  arg0Modifier->addValidValue( "MULTIPLY" );
  arg0Modifier->addValidValue( "DIVIDE" );
  arg0Modifier->addValidValue( "ADD" );
  arg0Modifier->addValidValue( "SUBTRACT" );
  arg0Modifier->addValidValue( "NEGATE" );
  arg0Modifier->addValidValue( "MULTIPLY_ALPHA" );
  arg0Modifier->addValidValue( "MULTIPLY_ONE_MINUS_ALPHA" );
  arg0Modifier->setValue( "NONE" );

  arg1Modifier->addValidValue( "NONE" );
  arg1Modifier->addValidValue( "COMPLEMENT" );
  arg1Modifier->addValidValue( "MULTIPLY" );
  arg1Modifier->addValidValue( "DIVIDE" );
  arg1Modifier->addValidValue( "ADD" );
  arg1Modifier->addValidValue( "SUBTRACT" );
  arg1Modifier->addValidValue( "NEGATE" );
  arg1Modifier->addValidValue( "MULTIPLY_ALPHA" );
  arg1Modifier->addValidValue( "MULTIPLY_ONE_MINUS_ALPHA" );
  arg1Modifier->setValue( "NONE" );

  alphaArg0Modifier->addValidValue( "NONE" );
  alphaArg0Modifier->addValidValue( "COMPLEMENT" );
  alphaArg0Modifier->addValidValue( "MULTIPLY" );
  alphaArg0Modifier->addValidValue( "DIVIDE" );
  alphaArg0Modifier->addValidValue( "ADD" );
  alphaArg0Modifier->addValidValue( "SUBTRACT" );
  alphaArg0Modifier->addValidValue( "NEGATE" );
  alphaArg0Modifier->setValue( "NONE" );

  alphaArg1Modifier->addValidValue( "NONE" );
  alphaArg1Modifier->addValidValue( "COMPLEMENT" );
  alphaArg1Modifier->addValidValue( "MULTIPLY" );
  alphaArg1Modifier->addValidValue( "DIVIDE" );
  alphaArg1Modifier->addValidValue( "ADD" );
  alphaArg1Modifier->addValidValue( "SUBTRACT" );
  alphaArg1Modifier->addValidValue( "NEGATE" );
  alphaArg1Modifier->setValue( "NONE" );

  // routes
  shaders->route( rebuildShader );
  function->route( rebuildShader );
  arg0Modifier->route( rebuildShader );
  arg1Modifier->route( rebuildShader );
  alphaFunction->route( rebuildShader );
  alphaArg0Modifier->route( rebuildShader );
  alphaArg1Modifier->route( rebuildShader );

  value->route( displayList );
  arg0Value->route( displayList );
  arg1Value->route( displayList );

  alphaValue->route( displayList );
  alphaArg0Value->route( displayList );
  alphaArg1Value->route( displayList );
}

/// Adds uniform fields to the shader. A ComposedShader uses its
/// dynamic fields to define uniform variables. Hence we need to
/// add dynamic fields for each field that we want to be accessable
/// in the shader.
string ShaderCombiner::addUniformFields( ComposedShader *shader  ) {
  stringstream s;

  // add fragment shader uniform fields
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->addUniformFields( shader );
    }
  }
  
  const string &f = function->getValue();
  if( f == "BLEND_VALUE" ) {
    s << addUniformToFragmentShader( shader,
				     uniqueShaderName( "value" ), 
				     "float",
				     H3D::Field::INPUT_OUTPUT,
				     copyAndRouteField( value ) );
  }

  const string &m0 = arg0Modifier->getValue();
  if( m0 == "ADD" || m0 == "SUBTRACT" || m0 == "MULTIPLY" || m0 == "DIVIDE" ) {
    s << addUniformToFragmentShader( shader,
				     uniqueShaderName( "arg0Value" ), 
				     "float",
				     H3D::Field::INPUT_OUTPUT,
				     copyAndRouteField( arg0Value ) );
  }

  const string &m1 = arg1Modifier->getValue();
  if( m1 == "ADD" || m1 == "SUBTRACT" || m1 == "MULTIPLY" || m1 == "DIVIDE" ) {
    s << addUniformToFragmentShader( shader,
				     uniqueShaderName( "arg1Value" ), 
				     "float",
				     H3D::Field::INPUT_OUTPUT,
				     copyAndRouteField( arg1Value ) );
  }

  const string &af = alphaFunction->getValue();
  if( af == "BLEND_VALUE" ) {
    s << addUniformToFragmentShader( shader,
				     uniqueShaderName( "alphaValue" ), 
				     "float",
				     H3D::Field::INPUT_OUTPUT,
				     copyAndRouteField( alphaValue ) );
  }

  const string &am0 = alphaArg0Modifier->getValue();
  if( am0 == "ADD" || am0 == "SUBTRACT" || am0 == "MULTIPLY" || am0 == "DIVIDE" ) {
    s << addUniformToFragmentShader( shader,
				     uniqueShaderName( "alphaArg0Value" ), 
				     "float",
				     H3D::Field::INPUT_OUTPUT,
				     copyAndRouteField( alphaArg0Value ) );
  }

  const string &am1 = alphaArg1Modifier->getValue();
  if( am1 == "ADD" || am1 == "SUBTRACT" || am1 == "MULTIPLY" || am1 == "DIVIDE" ) {
    s << addUniformToFragmentShader( shader,
				     uniqueShaderName( "alphaArg1Value" ), 
				     "float",
				     H3D::Field::INPUT_OUTPUT,
				     copyAndRouteField( alphaArg1Value ) );
  }

  return s.str();
}

string ShaderCombiner::getFunctionShaderString() {
  stringstream s;

  // fragment shader
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getFunctionShaderString();
    }
  }
  return s.str();
}

/// Get the varying variables used by the shader generator.
void ShaderCombiner::getVaryingVariables( vector< VaryingVariable > &variables ) {
  // fragment shader
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      fs->getVaryingVariables( variables );
    }
  }
}

/// Get the attribues used by the shader generator.
void ShaderCombiner::getAttributes( vector< Attribute > &attributes ) {
  // fragment shader
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      fs->getAttributes( attributes );
    }
  }
}


/// Returns the shader code for the fragment shader.
string ShaderCombiner::getVertexShaderString() {
  stringstream s;

  // fragment shader
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getVertexShaderString();
    }
  }
  return s.str();
}

string ShaderCombiner::applyModifier( const string &variable_name,
				      const string &modifier,
				      const string &value ) {

  if( modifier == "COMPLEMENT" ) {
    return variable_name + ".rgb = 1.0 - " + variable_name + ".rgb;";
  } else if( modifier == "MULTIPLY" ) {
    return variable_name + ".rgb  *= " + value + ";";
  } else if( modifier == "DIVIDE" ) {
    return variable_name + ".rgb  /= " + value + ";";
  } else if( modifier == "ADD" ) {
    return variable_name + ".rgb  += " + value + ";";
  } else if( modifier == "SUBTRACT" ) {
    return variable_name + ".rgb  -= " + value + ";";
  } else if( modifier == "NEGATE" ) {
    return variable_name + ".rgb  = -" + variable_name + ".rgb;";
  } else if( modifier == "MULTIPLY_ALPHA" ) {
    return variable_name + ".rgb  = " + variable_name + ".rgb *" + variable_name + ".a;";
  } else if( modifier == "MULTIPLY_ONE_MINUS_ALPHA" ) {
    return variable_name + ".rgb  = " + variable_name + ".rgb *(1.0-" + variable_name + ".a);";
  } 

  if( modifier != "NONE" ) {
    Console(4) << "Invalid modifier: \"" << modifier << "\" in ShaderCombiner. Using \"NONE\" instead. " << endl;
  }
  return "";
}

string ShaderCombiner::applyAlphaModifier( const string &variable_name,
					   const string &modifier,
					   const string &value ) {

  if( modifier == "COMPLEMENT" ) {
    return variable_name + ".a = 1.0 - " + variable_name + ".a;";
  } else if( modifier == "MULTIPLY" ) {
    return variable_name + ".a  *= " + value + ";";
  } else if( modifier == "DIVIDE" ) {
    return variable_name + ".a  /= " + value + ";";
  } else if( modifier == "ADD" ) {
    return variable_name + ".a  += " + value + ";";
  } else if( modifier == "SUBTRACT" ) {
    return variable_name + ".a  -= " + value + ";";
  } else if( modifier == "NEGATE" ) {
    return variable_name + ".a  = -" + variable_name + ".a;";
  } 
  if( modifier != "NONE" ) {
    Console(4) << "Invalid modifier: \"" << modifier << "\" in ShaderCombiner. Using \"NONE\" instead. " << endl;
  }
  return "";
}

string ShaderCombiner::combineFunction( const string &v0,
					const string &v1,
					const string &function,
					const string &value ) {

  if( function == "SELECT0" ) {
    return v0;
  } else if( function == "SELECT1" ) {
    return v1 ;
  } else if( function == "ADD" ) {
    return v0 + " + " + v1;
  } else if( function == "SUBTRACT" ) {
    return v0 + " - " + v1;
  } else if( function == "MULTIPLY" ) {
    return v0 + " * " + v1;
  } else if( function == "DIVIDE" ) {
    return v0 + " / " + v1;
  } else if( function == "BLEND_VALUE" ) {
    return value + " * " + v0 + " + (1.0-" + value + ")*" + v1;
  } 

  Console(4) << "Invalid function: \"" << function << "\" in ShaderCombiner. Using \"SELECT1\" instead. " << endl;
  return v1;
}

/// Returns the shader code for the fragment shader.
string ShaderCombiner::getFragmentShaderString() {
  stringstream s;
  const NodeVector &c = shaders->getValue();

  if( c.empty() ) return "";

  string v0_string = uniqueShaderName( "v0" );
  string v1_string = uniqueShaderName( "v1" );

  s << "    vec4 " << v0_string << " = vec4(0.0, 0.0, 0.0, 0.0); " << endl;
  s << "    vec4 " << v1_string << " = vec4(0.0, 0.0, 0.0, 0.0); " << endl;

  H3DGeneratedFragmentShaderNode *fs = 
    static_cast< H3DGeneratedFragmentShaderNode * >( c[0] );

  const string &f = function->getValue();
  const string &m0 = arg0Modifier->getValue();
  const string &m1 = arg1Modifier->getValue();

  const string &af = alphaFunction->getValue();
  const string &am0 = alphaArg0Modifier->getValue();
  const string &am1 = alphaArg1Modifier->getValue();
 
  if( fs ) {
    s << "   { " << endl;
    s << fs->getFragmentShaderString() << endl;
    s << "     " << v0_string << " = generated_color; " << endl;   
    s << "  } " << endl;
  }

  if( c.size() == 1 ) {
    s << applyModifier( v0_string, m0, uniqueShaderName( "arg0Value" ) ) << endl;   
    s << applyAlphaModifier( v0_string, am0, uniqueShaderName( "alphaArg0Value" ) ) << endl;   
  }

  for( unsigned int i = 1; i < c.size(); ++i ) {
    s << "   { " << endl;
    if( c[i] ) {

      s << applyModifier( v0_string, m0, uniqueShaderName( "arg0Value" ) ) << endl;   
      s << applyAlphaModifier( v0_string, am0, uniqueShaderName( "alphaArg0Value" ) ) << endl;   

      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getFragmentShaderString();
      s << "     " << v1_string << " = generated_color; " << endl;   
      s << "     " << applyModifier( v1_string, m1, uniqueShaderName( "arg1Value" ) ) << endl;
      s << "     " << applyAlphaModifier( v1_string, am1, uniqueShaderName( "alphaArg1Value" ) ) << endl;
      s << "     " << v0_string << ".rgb = " << combineFunction( v0_string + ".rgb", v1_string + ".rgb", f, uniqueShaderName("value" ) ) << ";" << endl; 
      s << "     " << v0_string << ".a = " << combineFunction( v0_string + ".a", v1_string + ".a", af, uniqueShaderName("alphaValue" ) ) << ";" << endl; 

    }
    s << "  } " << endl;
  }

  s << "   generated_color = " << v0_string << "; " << endl;

  return s.str();
}


/// Traverse the scene-graph.
void ShaderCombiner::traverseSG( TraverseInfo &ti ) {
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      c[i]->traverseSG( ti );
    }
  }
}




