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
/// \file MultiRenderTargetShader.cpp
/// \brief CPP file for MultiRenderTargetShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MultiRenderTargetShader.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MultiRenderTargetShader::database( 
                                   "MultiRenderTargetShader", 
                                   &newInstance< MultiRenderTargetShader >,
                                   typeid( MultiRenderTargetShader ),
                                   &H3DGeneratedShaderNode::database );

namespace MultiRenderTargetShaderInternals {
  FIELDDB_ELEMENT( MultiRenderTargetShader, shaders, INPUT_OUTPUT );
}

MultiRenderTargetShader::MultiRenderTargetShader( Inst< DisplayList  > _displayList,
                                                  Inst< SFNode       > _metadata,
                                                  Inst< SFBool       > _isSelected ,
                                                  Inst< SFBool       > _isValid,
                                                  Inst< SFBool       > _activate,
                                                  Inst< SFString     > _language,
                                                  Inst< MFShaderPart > _parts,
                                                  Inst< SFBool       > _suppressUniformWarnings,
                                                  Inst< MFString > _fragmentShaderString,
                                                  Inst< MFString > _vertexShaderString,
                                                  Inst< MFGeneratedFragmentShaderNode > _shaders ) :
  H3DGeneratedShaderNode( _displayList, _metadata, _isSelected, 
                          _isValid, _activate, _language, _parts, 
                          _suppressUniformWarnings, _fragmentShaderString, 
                          _vertexShaderString ),
  shaders( _shaders ) {
  
  type_name = "MultiRenderTargetShader";
  database.initFields( this );

  shaders->route( rebuildShader );

}

/// Adds uniform fields to the shader. A ComposedShader uses its
/// dynamic fields to define uniform variables. Hence we need to
/// add dynamic fields for each field that we want to be accessable
/// in the shader.
string MultiRenderTargetShader::addUniformFields( ComposedShader *shader  ) {
  stringstream s;
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->addUniformFields( shader );
    }
  }
  return s.str();
}

string MultiRenderTargetShader::getFunctionShaderString() {
  stringstream s;
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getFunctionShaderString();
    }
  }
  return s.str();
}

/// Get the varying variables used by the shader generator.
void MultiRenderTargetShader::getVaryingVariables( vector< VaryingVariable > &variables ) {
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      fs->getVaryingVariables( variables );
    }
  }
}

/// Get the attribues used by the shader generator.
void MultiRenderTargetShader::getAttributes( vector< Attribute > &attributes ) {
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      fs->getAttributes( attributes );
    }
  }
}

string MultiRenderTargetShader::getFinalFragmentShaderOutputString() {
  return "";
}

/// Returns the shader code for the fragment shader.
string MultiRenderTargetShader::getVertexShaderString() {
  stringstream s;
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getVertexShaderString();
    }
  }
  return s.str();
}

/// Returns the shader code for the fragment shader.
string MultiRenderTargetShader::getFragmentShaderString() {
  stringstream s;
  const NodeVector &c = shaders->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    s << "   generated_color = vec4(1.0, 1.0, 1.0, 1.0 ); " << endl;
    s << "   { " << endl;
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getFragmentShaderString();
    }
    s << "  } " << endl;
    s << "  gl_FragData[" << i << "] = generated_color; " << endl;
  }
  return s.str();
}




