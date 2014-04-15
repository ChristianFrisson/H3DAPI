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
/// \file ComposedGeneratedShader.cpp
/// \brief CPP file for ComposedGeneratedShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ComposedGeneratedShader.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ComposedGeneratedShader::database( 
                                   "ComposedGeneratedShader", 
                                   &newInstance< ComposedGeneratedShader >,
                                   typeid( ComposedGeneratedShader ),
                                   &H3DGeneratedShaderNode::database );

namespace ComposedGeneratedShaderInternals {
  FIELDDB_ELEMENT( ComposedGeneratedShader, fragmentShaders, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedGeneratedShader, vertexShaders, INPUT_OUTPUT );
}

ComposedGeneratedShader::ComposedGeneratedShader( Inst< DisplayList  > _displayList,
                                                  Inst< SFNode       > _metadata,
                                                  Inst< SFBool       > _isSelected ,
                                                  Inst< SFBool       > _isValid,
                                                  Inst< SFBool       > _activate,
                                                  Inst< SFString     > _language,
                                                  Inst< MFShaderPart > _parts,
                                                  Inst< SFBool       > _suppressUniformWarnings,
                                                  Inst< MFString > _fragmentShaderString,
                                                  Inst< MFString > _vertexShaderString,
                                                  Inst< MFGeneratedFragmentShaderNode > _fragmentShaders,
                                                  Inst< MFGeneratedVertexShaderNode > _vertexShaders ) :
  H3DGeneratedShaderNode( _displayList, _metadata, _isSelected, 
                          _isValid, _activate, _language, _parts, 
                          _suppressUniformWarnings, _fragmentShaderString, 
                          _vertexShaderString ),
  fragmentShaders( _fragmentShaders ),
  vertexShaders( _vertexShaders ) {
  
  type_name = "ComposedGeneratedShader";
  database.initFields( this );

  fragmentShaders->route( rebuildShader );
  vertexShaders->route( rebuildShader );
}

/// Adds uniform fields to the shader. A ComposedShader uses its
/// dynamic fields to define uniform variables. Hence we need to
/// add dynamic fields for each field that we want to be accessable
/// in the shader.
string ComposedGeneratedShader::addUniformFields( ComposedShader *shader  ) {
  stringstream s;

  // add vertex shader uniform fields
  const NodeVector &v = vertexShaders->getValue();
  for( unsigned int i = 0; i < v.size(); ++i ) {
    if( v[i] ) {
      H3DGeneratedVertexShaderNode *vs = 
        static_cast< H3DGeneratedVertexShaderNode * >( v[i] );
      s << vs->addUniformFields( shader );
    }
  }

  // add fragment shader uniform fields
  const NodeVector &c = fragmentShaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->addUniformFields( shader );
    }
  }
  return s.str();
}

/// Traverse the scene-graph.
void ComposedGeneratedShader::traverseSG( TraverseInfo &ti ) {
 // vertex shader
  const NodeVector &v = vertexShaders->getValue();
  for( unsigned int i = 0; i < v.size(); ++i ) {
    if( v[i] ) {
      v[i]->traverseSG( ti );
    }
  }

  // fragment shader
  const NodeVector &c = fragmentShaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      c[i]->traverseSG( ti );
    }
  }
}

string ComposedGeneratedShader::getFunctionShaderString() {
  stringstream s;

  // vertex shader
  const NodeVector &v = vertexShaders->getValue();
  for( unsigned int i = 0; i < v.size(); ++i ) {
    if( v[i] ) {
      H3DGeneratedVertexShaderNode *vs = 
        static_cast< H3DGeneratedVertexShaderNode * >( v[i] );
      s << vs->getFunctionShaderString();
    }
  }

  // fragment shader
  const NodeVector &c = fragmentShaders->getValue();
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
void ComposedGeneratedShader::getVaryingVariables( vector< VaryingVariable > &variables ) {
  // vertex shader
  const NodeVector &v = vertexShaders->getValue();
  for( unsigned int i = 0; i < v.size(); ++i ) {
    if( v[i] ) {
      H3DGeneratedVertexShaderNode *vs = 
        static_cast< H3DGeneratedVertexShaderNode * >( v[i] );
      vs->getVaryingVariables( variables );
    }
  }

  // fragment shader
  const NodeVector &c = fragmentShaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      fs->getVaryingVariables( variables );
    }
  }
}

/// Get the attribues used by the shader generator.
void ComposedGeneratedShader::getAttributes( vector< Attribute > &attributes ) {
  // vertex shader
  const NodeVector &v = vertexShaders->getValue();
  for( unsigned int i = 0; i < v.size(); ++i ) {
    if( v[i] ) {
      H3DGeneratedVertexShaderNode *vs = 
        static_cast< H3DGeneratedVertexShaderNode * >( v[i] );
      vs->getAttributes( attributes );
    }
  }

  // fragment shader
  const NodeVector &c = fragmentShaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      fs->getAttributes( attributes );
    }
  }
}


/// Returns the shader code for the fragment shader.
string ComposedGeneratedShader::getVertexShaderString() {
  stringstream s;

  // vertex shader
  const NodeVector &v = vertexShaders->getValue();
  for( unsigned int i = 0; i < v.size(); ++i ) {
    if( v[i] ) {
      H3DGeneratedVertexShaderNode *vs = 
        static_cast< H3DGeneratedVertexShaderNode * >( v[i] );
      s << vs->getVertexShaderString();
    }
  }

  // fragment shader
  const NodeVector &c = fragmentShaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getVertexShaderString();
    }
  }
  return s.str();
}

/// Returns the shader code for the fragment shader.
string ComposedGeneratedShader::getFragmentShaderString() {
  stringstream s;
  const NodeVector &c = fragmentShaders->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    s << "   { " << endl;
    if( c[i] ) {
      H3DGeneratedFragmentShaderNode *fs = 
        static_cast< H3DGeneratedFragmentShaderNode * >( c[i] );
      s << fs->getFragmentShaderString();
    }
    s << "  } " << endl;
  }
  return s.str();
}





